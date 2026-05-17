#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100

// Struct definition
typedef struct {
    char student_id[20];
    char full_name[50];
    char course_code[10];
} Student;

// Global array and counter to hold loaded students in memory
extern Student registry[MAX_STUDENTS];
extern int student_count;

// Function Prototypes
int load_students(const char* filename);

#endif
