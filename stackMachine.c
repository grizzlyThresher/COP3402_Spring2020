//Owen Brahms - 3739927
//Michaela Green - 
#include "stackMachine.h"
#include <stdio.h>
#include <stdlib.h>

int sp = MAX_DATASTACK_HEIGHT;
int bp = 0;
int pc = 0;
int ir = 0;
int gp = -1;
int[] stack = calloc(MAX_DATASTACK_HEIGHT, sizeof(int));
instruction* text = malloc(MAX_CODE_LENGTH * sizeof(instruction));


int main() {

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
void mult() {

}
void div() {

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