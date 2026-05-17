#include <stdio.h>
#include "blockchain.h"

int main() {
    printf("--- Blockchain Attendance System Initialization ---\n");

    if (!load_students("students.txt")) {
        return 1; 
    }

    generate_keypair();

    // --- NEW LOGIC: Load from disk, or initialize fresh ---
    if (load_chain()) {
        printf("Blockchain successfully restored from previous session.\n");
    } else {
        printf("No previous blockchain found. Initializing new chain...\n");
        init_blockchain();
        sign_block(blockchain_head);
    }

    printf("\n--- Segment 6: Persistence & Viewing Test ---\n");
    
    // Add a record just to make sure we have data
    mark_attendance("ALU003", "BLK101", "PRESENT");
    
    // Save the chain to disk
    save_chain();

    // View the entire formatted ledger
    view_records();

    return 0;
}
