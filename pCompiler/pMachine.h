#include <stdio.h>
// Constants used in the Virtual Machine
#define MAX_DATASTACK_HEIGHT  40
#define MAX_CODE_LENGTH  200
#define MAX_LEXI_LEVELS  3
#define NUM_REGISTERS 8
// Potential Error Messages
#define STACK_OVERFLOW fprintf(stderr, "Runtime Error: Stack Overflow.\n");
#define OUT_OF_BOUNDS fprintf(stderr, "Runtime Error: Invalid Memory Location.\n");
#define BAD_OPERATION fprintf(stderr, "Runtime Error: Invalid Operation.\n");
// struct used to organize instructions
struct instruction{
    int op;
    int r;
    int l;
    int m;
};
// Method used to print current state of the machine
void printState(int* stack, int curLoc, struct instruction ir, int pc, int bp, int sp, int* regFile, int l, FILE* output, char** oper);
// Method used for formatting in the event a value requires 2 digits.
void makeBuffer(char *str, int num, int maxSize);
// Method used to redefine base
int base(int* stack, int l, int base);
// Method used to export the Virtual Machine running to the compiler
int execute(struct instruction* code, FILE* opr);