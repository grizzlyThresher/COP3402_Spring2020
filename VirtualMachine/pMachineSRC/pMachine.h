#include <stdio.h>
// Constants used in the Virtual Machine
#define MAX_DATASTACK_HEIGHT  40
#define MAX_CODE_LENGTH  200
#define MAX_LEXI_LEVELS  3
#define NUM_REGISTERS 8
// Used to halt the fetch->execute cycle
#define HALT halt = 0;
// Potential Error Messages
#define STACK_OVERFLOW fprintf(stderr, "ERROR: Stack Overflow.\n");
#define OUT_OF_BOUNDS fprintf(stderr, "ERROR: Invalid Memory Location.\n");
#define INVALID_REGISTER fprintf(stderr, "ERROR: Invalid Register.\n");
#define BAD_INSTRUCTION fprintf(stderr, "ERROR: Invalid Instruction Location.\n");
#define BAD_OPERATION fprintf(stderr, "ERROR: Invalid Operation.\n");
// struct used to organize instructions
struct instruction{
    int op;
    int r;
    int l;
    int m;
};
// Method to separate the reading of the instructions from the rest of main
int readInstructions(struct instruction** code, FILE* input, FILE* output, char** oper, int* lines);
// Method used to print current state of the machine
void printState(int* stack, int curLoc, struct instruction ir, int pc, int bp, int sp, int* regFile, int l, FILE* output, char** oper);
// Method used for formatting in the event a value requires 2 digits.
void makeBuffer(char *str, int num, int maxSize);
// Method used to redefine base
int base(int* stack, int l, int base);