#include <stdio.h>
#include "blockchain.h"

int main() {
    printf("--- Blockchain Attendance System Initialization ---\n");

    if (!load_students("students.txt")) {
        printf("System halted due to missing/empty registry.\n");
        return 1; 
    }

    generate_keypair();
    init_blockchain();
    sign_block(blockchain_head);

    printf("\n--- Segment 4: Testing Attendance Marking ---\n");

    // Test 1: A Valid Student (From your students.txt)
    printf("\n[Test 1] Marking valid student ALU001...\n");
    mark_attendance("ALU001", "BLK101", "PRESENT");

    // Test 2: Another Valid Student
    printf("\n[Test 2] Marking valid student ALU002...\n");
    mark_attendance("ALU002", "BLK101", "LATE");

    // Test 3: An Invalid Student (Should trigger an error and abort)
    printf("\n[Test 3] Marking INVALID student ALU999...\n");
    mark_attendance("ALU999", "BLK101", "PRESENT");

    printf("\nReady for Segment 5 (Validation & Tamper Detection)!\n");

    return 0;
}
