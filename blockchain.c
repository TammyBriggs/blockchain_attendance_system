#include "blockchain.h"

// Instantiate global variables
Student registry[MAX_STUDENTS];
int student_count = 0;
Block* blockchain_head = NULL;

// --- SEGMENT 1: STUDENT REGISTRY ---

int load_students(const char* filename) {
    FILE *file = fopen(filename, "r");
    
    // Error handling: Missing file
    if (file == NULL) {
        printf("ERROR: Could not open file '%s'. File may be missing.\n", filename);
        return 0; 
    }

    // Error handling: Empty file
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {   
        printf("ERROR: '%s' is empty.\n", filename);
        fclose(file);
        return 0;
    }
    rewind(file); 

    char line[150];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Strip newline

        char *id = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *course = strtok(NULL, ",");

        if (id && name && course) {
            strncpy(registry[student_count].student_id, id, sizeof(registry[student_count].student_id) - 1);
            strncpy(registry[student_count].full_name, name, sizeof(registry[student_count].full_name) - 1);
            strncpy(registry[student_count].course_code, course, sizeof(registry[student_count].course_code) - 1);
            student_count++;
        }
    }

    fclose(file);
    printf("SUCCESS: Loaded %d students from the registry.\n", student_count);
    return 1;
}

// --- SEGMENT 2: BLOCKCHAIN DATA STRUCTURE ---

void calculate_hash(Block* block, char* output_hash) {
    char data[1024];
    unsigned char hash[SHA256_DIGEST_LENGTH];

    // Concatenate block data for hashing
    snprintf(data, sizeof(data), "%d%ld%s%s%s%s%s", 
            block->index, 
            block->timestamp, 
            block->student_id, 
            block->full_name, 
            block->course_code, 
            block->status, 
            block->previous_hash);

    // Perform SHA-256 hash using OpenSSL
    SHA256((unsigned char*)data, strlen(data), hash);

    // Convert raw bytes to hexadecimal string
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output_hash + (i * 2), "%02x", hash[i]);
    }
    output_hash[64] = '\0';
}

void init_blockchain() {
    // Allocate memory for Genesis Block
    Block* genesis = (Block*)malloc(sizeof(Block));
    if (!genesis) {
        printf("ERROR: Memory allocation failed for Genesis Block.\n");
        exit(1);
    }

    // Initialize Genesis Block data
    genesis->index = 0;
    genesis->timestamp = time(NULL);
    strcpy(genesis->student_id, "SYSTEM");
    strcpy(genesis->full_name, "Genesis Block");
    strcpy(genesis->course_code, "NONE");
    strcpy(genesis->status, "SYSTEM");
    
    // previous_hash set to 64 zeros (Rubric Requirement)
    memset(genesis->previous_hash, '0', 64);
    genesis->previous_hash[64] = '\0';
    
    // Zero out signature for now
    memset(genesis->signature, 0, 72);

    // Calculate and set hash
    calculate_hash(genesis, genesis->hash);
    
    // Set as head of the linked list
    genesis->next = NULL;
    blockchain_head = genesis;

    printf("SUCCESS: Genesis Block initialized.\n");
    printf("         Hash: %.15s...\n", genesis->hash);
}

// --- SEGMENT 3: CRYPTOGRAPHY & DIGITAL SIGNATURES ---

EVP_PKEY* admin_keypair = NULL;

void generate_keypair() {
    // Create context for Elliptic Curve key generation
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    EVP_PKEY_keygen_init(pctx);
    
    // Use standard secp256r1 curve (P-256), widely used in modern crypto
    EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, NID_X9_62_prime256v1);
    
    // Generate the key
    EVP_PKEY_keygen(pctx, &admin_keypair);
    EVP_PKEY_CTX_free(pctx);
    
    if (admin_keypair != NULL) {
        printf("SUCCESS: Admin Cryptographic Keypair (ECDSA) generated.\n");
    } else {
        printf("ERROR: Failed to generate cryptographic keypair.\n");
        exit(1);
    }
}

void sign_block(Block* block) {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    
    // Initialize signing operations using SHA-256 and the Admin's private key
    EVP_DigestSignInit(mdctx, NULL, EVP_sha256(), NULL, admin_keypair);
    
    // We sign the computed hash of the block to ensure data integrity
    EVP_DigestSignUpdate(mdctx, block->hash, strlen(block->hash));
    
    size_t sig_len = sizeof(block->signature);
    memset(block->signature, 0, sig_len); // Clear out any old garbage memory
    
    // Finalize the signature and store it directly in the block struct
    EVP_DigestSignFinal(mdctx, block->signature, &sig_len);
    EVP_MD_CTX_free(mdctx);
}

int verify_signature(Block* block) {
    // Trick: Find the actual length of the DER encoded signature.
    // DER sequences start with 0x30, followed by the length of the remaining bytes.
    if (block->signature[0] != 0x30) {
        return 0; // Invalid format, likely an empty/tampered signature
    }
    size_t actual_sig_len = block->signature[1] + 2;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    
    // Initialize verification using the Admin's public key
    EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, admin_keypair);
    EVP_DigestVerifyUpdate(mdctx, block->hash, strlen(block->hash));
    
    // Returns 1 if valid, 0 if invalid
    int result = EVP_DigestVerifyFinal(mdctx, block->signature, actual_sig_len);
    EVP_MD_CTX_free(mdctx);
    
    return (result == 1); 
}

// --- SEGMENT 4: MARKING ATTENDANCE ---

void mark_attendance(const char* student_id, const char* course_code, const char* status) {
    // 1. Validate the Student ID against our loaded registry
    int student_found = 0;
    Student* matched_student = NULL;
    
    for (int i = 0; i < student_count; i++) {
        if (strcmp(registry[i].student_id, student_id) == 0) {
            student_found = 1;
            matched_student = &registry[i];
            break;
        }
    }

    // Reject and print error if ID not found
    if (!student_found) {
        printf("ERROR: Student ID '%s' not found in registry. Attendance aborted.\n", student_id);
        return;
    }

    // 2. Traverse the linked list to find the current last block
    Block* current = blockchain_head;
    while (current->next != NULL) {
        current = current->next;
    }

    // 3. Allocate memory for the new attendance block
    Block* new_block = (Block*)malloc(sizeof(Block));
    if (!new_block) {
        printf("ERROR: Memory allocation failed for new block.\n");
        return;
    }

    // 4. Populate the block with data
    new_block->index = current->index + 1;
    new_block->timestamp = time(NULL);
    
    // Safely copy strings using strncpy to prevent buffer overflows
    strncpy(new_block->student_id, matched_student->student_id, sizeof(new_block->student_id) - 1);
    strncpy(new_block->full_name, matched_student->full_name, sizeof(new_block->full_name) - 1);
    strncpy(new_block->course_code, course_code, sizeof(new_block->course_code) - 1);
    strncpy(new_block->status, status, sizeof(new_block->status) - 1);
    
    // Ensure null-termination
    new_block->student_id[sizeof(new_block->student_id) - 1] = '\0';
    new_block->full_name[sizeof(new_block->full_name) - 1] = '\0';
    new_block->course_code[sizeof(new_block->course_code) - 1] = '\0';
    new_block->status[sizeof(new_block->status) - 1] = '\0';

    // 5. Link to the previous block cryptographically
    strncpy(new_block->previous_hash, current->hash, sizeof(new_block->previous_hash));
    
    // Clear out signature array before signing
    memset(new_block->signature, 0, sizeof(new_block->signature));

    // 6. Finalize: Hash the data, sign it, and physically link it to the list
    calculate_hash(new_block, new_block->hash);
    sign_block(new_block);
    
    new_block->next = NULL;
    current->next = new_block; // Append to the end of the chain

    printf("SUCCESS: Attendance marked for %s (%s) - %s\n", new_block->full_name, new_block->student_id, status);
    printf("         Block Hash: %.15s...\n", new_block->hash);
}
