#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h> // Required for SHA256

#define MAX_STUDENTS 100

// Student Registry Structure
typedef struct {
    char student_id[20];
    char full_name[50];
    char course_code[10];
} Student;

// Blockchain Node Structure
typedef struct Block {
    int index;
    time_t timestamp;
    char student_id[20];
    char full_name[50];
    char course_code[10];
    char status[10];
    char previous_hash[65];
    unsigned char signature[72]; // ECDSA signature (max 72 bytes)
    char hash[65];
    struct Block* next;          // Pointer to link to the next block
} Block;

// Global Variables
extern Student registry[MAX_STUDENTS];
extern int student_count;
extern Block* blockchain_head;

// Function Prototypes
int load_students(const char* filename);
void calculate_hash(Block* block, char* output_hash);
void init_blockchain();

#endif
