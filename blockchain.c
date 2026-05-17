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
