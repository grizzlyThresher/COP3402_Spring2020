//Owen Brahms - 3739927
//Michaela Green - 
#include "stackMachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sp = MAX_DATASTACK_HEIGHT;
int bp = 0;
int pc = 0;
struct instruction* ir = 0;
int gp = -1;
int stack[MAX_DATASTACK_HEIGHT] = {};
struct instruction code[MAX_CODE_LENGTH] = {};
int halt = 1;
int numLines = 0;


int main(int argc, char *argv[]) {

	FILE *file = fopen(argv[1], "r");
	if(file == 0) {
		printf("Could not find valid file by name: %s \n", argv[1]);
		return 0;
	}

	if(readInstructions(file) == 1) {
		printf("Program too long for machine to run.");
		return 0;
	}
	printInstructions();

	printf("\n\n                   gp    pc    bp    sp    data                     stack\n");
	printf("Initial values     %d    %d     %d     %d    \n\n",gp,pc,bp,sp);

	while(halt==1) {
		// Fetch
		ir = &code[pc];
		// Execute
		switch (ir->op) {
			case 1:
				literal(ir->m);
			case 2:
				operation(ir->m);
			case 3:
				load(ir->l, ir->m);
			case 4:
				store(ir->l, ir->m);
			case 5:
				call(ir->l, ir->m);
			case 6:
				inc(ir->m);
			case 7:
				jump(ir->m);
			case 8:
				jmpIfZero(ir->m);
			case 9:
				sysOp(ir->m);
		}
		printState(pc++);
	}

    return 0;
}

// Methods for each ISA input
void literal(int val) {

}
void operation(int op) {

}
void load(int lex, int offset) {

}
void store(int lex, int offset) {

}
void call(int lex, int index) {

}
void inc(int numLocals) {

}
void jump(int loc) {

}
void jmpIfZero(int loc) {

}
void sysOp(int op) {
	switch (op) {
		case 1:
			write();
		case 2:
			read();
		case 3:
			end();
	}
}
// Methods for each System Operation
void write() {

}
void read() {

}
void end() {
	halt = 0;
}
// Methods for each Op code for operation()
void ret() {

}
void negate() {

}
void add() {

}
void sub() {

}
void multiply() {

}
void divide() {

}
void odd() {

}
void mod() {

}
void equal() {

}
void notEqual() {

}
void less() {

}
void lessOrEqual() {

}
void greater() {

}
void greaterOrEqual() {

}
// Method to separate the reading of the instructions from the rest of main
int readInstructions(FILE* file) {

	int tmpNum = 0;
	int numCnt = 1;
	//reads in from file 1 at a time while keeping track of position and 
	//line number. 
	while(fscanf(file, "%d", &tmpNum)==1) {
		if(numLines >= MAX_CODE_LENGTH) 
			return 1;
		
		if(numCnt % 3 == 1) {
			code[numLines].op = tmpNum;
		} else if(numCnt % 3 == 2) {
			code[numLines].l = tmpNum;
		} else {
			code[numLines].m = tmpNum;
				numLines++;
				numCnt=0;
		}
		numCnt++;
	}
	return 0;
}

char* oper[] = {"lit","opr","lod","sto","cal","inc","jmp","jpc","sio"};
// Method to make printing of Instructions easier
void printInstructions() {
	//String array to translate from input number to corresponding operation 
	printf("Line    Op    L    M \n");
	for(int i = 0; i < numLines; i++) {
		printf("%d       %s   %d    %d \n", i, oper[code[i].op - 1], code[i].l, code[i].m);
	}

}
// Method used to print current state of the machine
void printState(int curLoc) {
	printf("%d %s %d %d          %d    %d     %d     %d    ",curLoc,oper[(ir->op)-1],ir->l,ir->m,gp,pc,bp,sp);
	for(int i = 0; i <= gp; i++) {
		printf("%d", stack[i]);
	}
    for(int i = gp+1; i < sp; i++) {
        printf(" ");
    }
    for(int i = sp; i < MAX_DATASTACK_HEIGHT; i++) {
        printf("%d", stack[i]);
    }
    printf("\n");

}
// Method used to redefine base in terms of requested lexicographical level
int base(int l, int base) {
	int b1 = base;
	while (l-- > 0) {
		b1 = stack[b1 - 1];
	}
	return b1;
}