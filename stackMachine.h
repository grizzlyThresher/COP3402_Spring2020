#include <stdio.h>
#define MAX_DATASTACK_HEIGHT  23
#define MAX_CODE_LENGTH  500
#define MAX_LEXI_LEVELS  3
// struct used to organize instructions
struct instruction{
    int op;
    int l;
    int m;
};
// Methods for each ISA input
void literal(int val);
void operation(int op);
void load(int lex, int offset);
void store(int lex, int offset);
void call(int lex, int index);
void inc(int numLocals);
void jump(int loc);
void jmpIfZero(int loc);
void sysOp(int op);
// Methods for each System Operation
void write();
void read();
void end();
// Methods for each Op code for operation()
void ret();
void negate();
void add();
void sub();
void multiply();
void divide();
void odd();
void mod();
void equal();
void notEqual();
void less();
void lessOrEqual();
void greater();
void greaterOrEqual();
// Method to separate the reading of the instructions from the rest of main
int readInstructions(FILE* file);
// Method to make printing of Instructions easier
void printInstructions();
// Method used to print current state of the machine
void printState(int curLoc);
// Method used to redefine base
int base(int l, int base);