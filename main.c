#include <stdio.h>
#include "blockchain.h"

int main() {
    printf("--- Blockchain Attendance System Initialization ---\n");

    // 1. Attempt to load the Student Registry
    if (!load_students("students.txt")) {
        printf("System halted due to missing/empty registry.\n");
        return 1; 
    }

    // 2. Initialize the Blockchain (Create Genesis Block)
    init_blockchain();

    // Debug output to verify state
    printf("\n--- System State Verification ---\n");
    printf("Total Students Loaded: %d\n", student_count);
    printf("Genesis Block Index: %d\n", blockchain_head->index);
    printf("Genesis Block Name: %s\n", blockchain_head->full_name);
    printf("Genesis Previous Hash: %s\n", blockchain_head->previous_hash);
    printf("Genesis Current Hash: %s\n", blockchain_head->hash);

    return 0;
}
