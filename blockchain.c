#include "blockchain.h"

// Define the global variables
Student registry[MAX_STUDENTS];
int student_count = 0;

int load_students(const char* filename) {
    FILE *file = fopen(filename, "r");
    
    // Print error if file is missing
    if (file == NULL) {
        printf("ERROR: Could not open file '%s'. File may be missing.\n", filename);
        return 0; // Return 0 to indicate failure
    }

    // Print error if file is empty
    fseek(file, 0, SEEK_END); // Jump to the end of the file
    if (ftell(file) == 0) {   // If the end is at position 0, it's empty
        printf("ERROR: '%s' is empty.\n", filename);
        fclose(file);
        return 0;
    }
    rewind(file); // Rewind back to the start of the file for reading

    char line[150];
    // Read line by line
    while (fgets(line, sizeof(line), file)) {
        // Strip out the newline character at the end of the line
        line[strcspn(line, "\n")] = 0;

        // Parse the comma-separated values
        char *id = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *course = strtok(NULL, ",");

        // Ensure we successfully parsed all three fields before saving
        if (id && name && course) {
            strncpy(registry[student_count].student_id, id, sizeof(registry[student_count].student_id) - 1);
            strncpy(registry[student_count].full_name, name, sizeof(registry[student_count].full_name) - 1);
            strncpy(registry[student_count].course_code, course, sizeof(registry[student_count].course_code) - 1);
            student_count++;
        }
    }

    fclose(file);
    printf("SUCCESS: Loaded %d students from the registry.\n", student_count);
    return 1; // Return 1 to indicate success
}
