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
int numLines = 0;


int main(int argc, char *argv[]) {

	FILE *file = fopen(argv[1], "r");
	if(file == 0) {
		printf("Could not find valid file by name: %s \n", argv[1]);
		return 0;
	}

	int tmpNum = 0;
	int numCnt = 1;
	//reads in from file 1 at a time while keeping track of position and 
	//line number. 
	while(fscanf(file, "%d", &tmpNum)==1) {
		if(numLines >= MAX_CODE_LENGTH) {
			printf("Program too long for machine to run.");
			return 0;
		}
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
	printInstructions();

    return 0;
}

//Functions for each ISA input
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
void write() {

}
void read() {

}
void end() {

}
//Functions for each Op code for operation()
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
// method to make printing of Instructions easier
void printInstructions() {
	//String array to translate from input number to corresponding operation 
	char* op[] = {"lit","opr","lod","sto","cal","inc","jmp","jpc","sio","sio","sio"};
	printf("Line    Op    L    M \n");
	for(int i = 0; i < numLines; i++) {
		printf("%d       %s   %d    %d \n", i, op[code[i].op - 1], code[i].l, code[i].m);
	}

}