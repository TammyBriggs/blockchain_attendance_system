#include <stdio.h>
#include "blockchain.h"

int main() {
    printf("--- Blockchain Attendance System Initialization ---\n");

    // 1. Attempt to load the Student Registry
    if (!load_students("students.txt")) {
        printf("System halted due to missing/empty registry.\n");
        return 1; // Exit with error code
    }

    // Quick debug loop to prove they are in memory
    printf("\n--- Registry Contents ---\n");
    for (int i = 0; i < student_count; i++) {
        printf("ID: %s | Name: %s | Course: %s\n", 
            registry[i].student_id, 
            registry[i].full_name, 
            registry[i].course_code);
    }

    return 0;
}
