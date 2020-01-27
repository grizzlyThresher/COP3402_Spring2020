#include <stdio.h>
#define MAX_DATASTACK_HEIGHT  40
#define MAX_CODE_LENGTH  200
#define MAX_LEXI_LEVELS  3
#define NUM_REGISTERS 8
// struct used to organize instructions
struct instruction{
    int op;
    int r;
    int l;
    int m;
};
// Methods for each ISA input
void literal(int reg, int val);
void ret();
void load(int reg, int lex, int offset);
void store(int reg, int lex, int offset);
void call(int lex, int index);
void inc(int numLocals);
void jump(int loc);
void jmpIfZero(int reg, int loc);
void sysOp(int reg, int op);
void negate(int stoReg, int froReg);
void add(int stoReg, int reg1, int reg2);
void sub(int stoReg, int reg1, int reg2);
void multiply(int stoReg, int reg1, int reg2);
void divide(int stoReg, int reg1, int reg2);
void odd(int stoReg, int froReg);
void mod(int stoReg, int reg1, int reg2);
void equal(int stoReg, int reg1, int reg2);
void notEqual(int stoReg, int reg1, int reg2);
void less(int stoReg, int reg1, int reg2);
void lessOrEqual(int stoReg, int reg1, int reg2);
void greater(int stoReg, int reg1, int reg2);
void greaterOrEqual(int stoReg, int reg1, int reg2);
// Methods for each System Operation
void write(int reg);
void read(int reg);
void end();

// Method to separate the reading of the instructions from the rest of main
int readInstructions(FILE* file);
// Method to make printing of Instructions easier
void printInstructions();
// Method used to print current state of the machine
void printState(int curLoc);
// Method used for formatting in the event a value requires 2 digits.
void makeBuffer(char *str, int num, int maxSize);
// Method used to free code array once program is complete.
void destroyCode(struct instruction** ptr, int pLen);
// Method used to redefine base
int base(int l, int base);