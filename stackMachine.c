//Owen Brahms - 3739927
//Michaela Green - 
#include "stackMachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TESTING

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
    // Will only print the instructions and stack trace while testing the VM
    // Avoids messiness for actual use of the machine.
    #ifdef TESTING
	printInstructions();
	printf("\n\n                      gp    pc    bp    sp    data               stack\n");
	printf("Initial values        %d    %d     %d     %d    \n\n",gp,pc,bp,sp);
    #endif
    int tmpPc = pc;

	while(halt==1) {
		// Fetch
		ir = &code[pc];
        tmpPc = pc;
		// Execute
		switch (ir->op) {
			case 1:
				literal(ir->m);
                break;
			case 2:
				operation(ir->m);
                break;
			case 3:
				load(ir->l, ir->m);
                break;
			case 4:
				store(ir->l, ir->m);
                break;
			case 5:
				call(ir->l, ir->m);
                break;
			case 6:
				inc(ir->m);
                break;
			case 7:
				jump(ir->m);
                break;
			case 8:
				jmpIfZero(ir->m);
                break;
			case 9:
				sysOp(ir->m);
                break;
            default:
                printf("Error: Invalid Operation.\n");
                halt = 0;
                break;
		}

        // Increments the Program Counter
        pc++;
        
        // Only prints the stack trace while testing the VM
        #ifdef TESTING
		printState(tmpPc);
        #endif
	}

    return 0;
}

// Methods for each ISA input
void literal(int val) {
    if (base(0,bp) == 0) {
        gp++;
        stack[gp] = val;
    } else {
        if (sp - 1 == gp) {
            printf("Error: Stack Overflow.\n");
            halt = 0;
        }
        sp--;
        stack[sp] = val;
    }
    
}
void operation(int op) {
    switch (op) {
        case 0:
            ret();
            break;
        case 1:
            negate();
            break;
        case 2:
            add();
            break;
        case 3:
            sub();
            break;
        case 4:
            multiply();
            break;
        case 5:
            divide();
            break;
        case 6:
            odd();
            break;
        case 7:
            mod();
            break;
        case 8:
            equal();
            break;
        case 9:
            notEqual();
            break;
        case 10:
            less();
            break;
        case 11:
            lessOrEqual();
            break;
        case 12:
            greater();
            break;
        case 13:
            greaterOrEqual();
            break;
        default:
            printf("Error: Invalid Operation.\n");
            halt = 0;
            break;
    }
    
}
void load(int lex, int offset) {

    
}
void store(int lex, int offset) {

    
}
void call(int lex, int loc) {
    if (sp - 4 <= gp) {
        printf("Error: Stack Overflow.\n");
    } else {
        stack[sp - 1] = 0;
        stack[sp - 2] = base(lex, bp);
        stack[sp - 3] = bp;
        stack[sp - 4] = pc;
        bp = sp - 1;
        // Subtracts 1 to offset global pc increment
        pc = loc - 1;
    }

}
void inc(int numLocals) {
    if (bp == 0) {
        gp += numLocals;
    } else {
        if (sp - numLocals <= gp) {
            printf("Error: Stack Overflow.\n");
            halt = 0;
        } else {
            sp -= numLocals;
        }
    }
    
}
void jump(int loc) {
    // Subtracts 1 to offset global pc increment
    pc = loc - 1;
}
void jmpIfZero(int loc) {
    if(stack[sp] == 0) {
        // Subtracts 1 to offset global pc increment
        pc = loc - 1;
    }
    sp++;
}
void sysOp(int op) {
	switch (op) {
		case 1:
			write();
            break;
		case 2:
			read();
            break;
		case 3:
			end();
            break;
        default:
            printf("Error: Invalid System Operation.\n");
            halt = 0;
            break;
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

#ifdef TESTING
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

    char buffer[6] = {};
    // Prints out the current state with spacing determined by the number of digits
    // in the associated values.
    makeBuffer(buffer, curLoc, 3);
    printf("%d%s", curLoc, buffer);
    makeBuffer(buffer, gp, 6);
    printf("%s  %d  %d          %d%s", oper[(ir->op)-1], ir->l, ir->m, gp, buffer);
    makeBuffer(buffer, pc, 6);
    printf("%d%s", pc, buffer);
    makeBuffer(buffer, bp, 6);
    printf("%d%s", bp, buffer);
    makeBuffer(buffer, sp, 6);
    printf("%d%s", sp, buffer);

    // Prints out the current state of the data-stack
	for(int i = 0; i <= gp; i++) {
		printf("%d", stack[i]);
	}
    if (sp < MAX_DATASTACK_HEIGHT) {

        printf("|");

        for(int i = gp+1; i < sp; i++) {
            printf(" ");
        }
        for(int i = sp; i < MAX_DATASTACK_HEIGHT; i++) {
            printf("%d", stack[i]);
        }
    }
    printf("\n");

}

// Method used for formatting in the event a value requires 2 digits.
// maxSize should always be <= max size of str.
void makeBuffer(char *str, int num, int maxSize) {

    int offset;
    if (num / 10 > 0 || num < 0) {
        offset = maxSize - 2;   
    } else {
        offset = maxSize - 1;
    }

    for(int i = 0; i < offset; i++) {
            str[i] = ' ';
        }
        str[offset] = '\0';
}

#endif

// Method used to redefine base in terms of requested lexicographical level
int base(int l, int base) {
	int b1 = base;
	while (l-- > 0) {
		b1 = stack[b1 - 1];
	}
	return b1;
}