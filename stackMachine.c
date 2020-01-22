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


int main(int argc, char *argv[]) {

    return 1;
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