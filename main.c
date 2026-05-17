#include <stdio.h>
#include "blockchain.h"

int main() {
    printf("--- Blockchain Attendance System Initialization ---\n");

    if (!load_students("students.txt")) {
        return 1; 
    }

    generate_keypair();
    init_blockchain();
    sign_block(blockchain_head);

    printf("\n--- Marking Attendance ---\n");
    mark_attendance("ALU001", "BLK101", "PRESENT"); // Becomes Block 1
    mark_attendance("ALU002", "BLK101", "LATE");    // Becomes Block 2

    printf("\n--- Segment 5: Validation & Tamper Testing ---\n");
    
    // 1. Validate the pristine, untouched chain
    printf("\n[Test 1] Validating pristine chain...");
    validate_chain();

    // 2. Simulate a hack: Change ALU001's status from PRESENT to ABSENT
    tamper_block(1, "ABSENT");

    // 3. Validate the chain again to ensure our system catches the tampering
    printf("\n[Test 2] Validating chain after tamper event...");
    validate_chain();

    printf("\nReady for Segment 6 (Data Persistence & Viewing)!\n");

    return 0;
}
