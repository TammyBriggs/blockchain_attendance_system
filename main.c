#include <stdio.h>
#include "blockchain.h"

int main() {
    printf("--- Blockchain Attendance System Initialization ---\n");

    // 1. Load the Student Registry
    if (!load_students("students.txt")) {
        printf("System halted due to missing/empty registry.\n");
        return 1; 
    }

    // 2. Generate Cryptographic Keys for the System Admin
    generate_keypair();

    // 3. Initialize the Blockchain (Create Genesis Block)
    init_blockchain();

    // 4. Sign the Genesis Block to test our ECDSA logic
    sign_block(blockchain_head);

    // 5. Verify the signature
    int is_valid = verify_signature(blockchain_head);

    printf("\n--- Cryptography Verification ---\n");
    printf("Genesis Block Signature Generated.\n");
    printf("Cryptographic Verification Check: [%s]\n", is_valid ? "VALID AND AUTHENTIC" : "TAMPERED/INVALID");

    return 0;
}
