//Owen Brahms - 3739927
//Michaela Green - 
#include "pMachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DISPLAY

// Declaration of Global variables and arrays used throughout the VM
int sp, bp, pc, halt, numLines, *stack, *registerFile;
struct instruction *ir, **code;

// Array used to keep track of the end of any activation records.
// Used exclusively for printing purposes.
#ifdef DISPLAY
int *endOfRecord;
char *oper[] = {"lit","ret","lod","sto","cal","inc","jmp","jpc","sio", "sio",
    "sio", "neg", "add", "sub", "mul", "div", "odd", "mod", "eql", "neq", "lss",
    "leq", "gtr", "geq"};
#endif


int main(int argc, char *argv[]) {

   

    // Instantiation of Global variables
    sp = 0;
    bp = 1;
    pc = 0;
    stack = calloc(MAX_DATASTACK_HEIGHT, sizeof(int));
    registerFile = calloc(NUM_REGISTERS, sizeof(int));
    halt = 1;
    numLines = 0;

    // Instantiation of array for printing purposes
    #ifdef DISPLAY
    endOfRecord = calloc(MAX_DATASTACK_HEIGHT, sizeof(int));
    #endif  

	FILE *file = fopen(argv[1], "r");
	if(file == 0) {
		printf("Could not find valid file by name: %s \n", argv[1]);
		return 0;
	}

	if(readInstructions(file) == 1) {
		printf("Program too long for machine to run.");
		return 0;
	}
    // Will only print the instructions and stack trace while DISPLAY the VM
    // Avoids messiness for actual use of the machine.
    #ifdef DISPLAY
	printInstructions();
	printf("\n\n                          pc    bp    sp    registers\n");
	printf("Initial values:           %d     %d     %d    0  0  0  0  0  0  0\n\n",pc,bp,sp);
    #endif

    // Temp variable used to store pc value before execution
    int tmpPc = pc;

	while(halt == 1) {
		// Fetch
		ir = code[pc];
        tmpPc = pc;
		// Execute
        // sysOp is repeated intentionally to comply with specs given in the HW file.
		switch (ir->op) {
			case 1:
				literal(ir->r, ir->m);
                break;
			case 2:
				ret();
                break;
			case 3:
				load(ir->r, ir->l, ir->m);
                break;
			case 4:
				store(ir->r, ir->l, ir->m);
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
				jmpIfZero(ir->r, ir->m);
                break;
			case 9:
				sysOp(ir->r, ir->m);
                break;
            case 10:
                sysOp(ir->r, ir->m);
                break;
            case 11:
                sysOp(ir->r, ir->m);
                break;
            case 12:
                negate(ir->r, ir->m);
                break;
            case 13:
                add(ir->r, ir->l, ir->m);
                break;
            case 14:
                sub(ir->r, ir->l, ir->m);
                break;
            case 15:
                multiply(ir->r, ir->l, ir->m);
                break;
            case 16:
                divide(ir->r, ir->l, ir->m);
                break;
            case 17:
                odd(ir->r, ir->m);
                break;
            case 18:
                mod(ir->r, ir->l, ir->m);
                break;
            case 19:
                equal(ir->r, ir->l, ir->m);
                break;
            case 20:
                notEqual(ir->r, ir->l, ir->m);
                break;
            case 21:
                less(ir->r, ir->l, ir->m);
                break;
            case 22:
                lessOrEqual(ir->r, ir->l, ir->m);             
                break;
                case 23:
                greater(ir->r, ir->l, ir->m);
                break;
            case 24:
                greaterOrEqual(ir->r, ir->l, ir->m);
                break;
            default:
                printf("Error: Invalid Operation.\n");
                halt = 0;
                break;
		}

        // Increments the Program Counter
        pc++;
        
        // Only prints the stack trace while the VM is in DISPLAY mode
        #ifdef DISPLAY
		printState(tmpPc);
        #endif
	}

    // Frees all remaining pointers on the heap
    freeAll(file);
    
    return 0;
}

// Methods for each ISA input
void literal(int reg, int val) {
    registerFile[reg] = val;

}
void ret() {
    sp = bp - 1;
    bp = stack[sp + 3];
    pc = stack[sp + 4];

}
void load(int reg, int lex, int offset) {

    registerFile[reg] = stack[base(l, bp) + offset];
    
}
void store(int reg, int lex, int offset) {

    stack[base(l, bp) + offset] = registerFile[reg];
    
}
void call(int lex, int loc) {
    if (sp + 4 >= MAX_DATASTACK_HEIGHT) {
        printf("Error: Stack Overflow.\n");
        halt = 1;
    } else {
        stack[sp + 1] = 0;
        stack[sp + 2] = base(lex, bp);
        stack[sp + 3] = bp;
        stack[sp + 4] = pc;
        bp = sp + 1;
        // Subtracts 1 to offset global pc increment
        pc = loc - 1;

        #ifdef DISPLAY
        endOfRecord[sp] = 1;
        #endif
    }

}
void inc(int numLocals) {

    if (sp + numLocals >= MAX_DATASTACK_HEIGHT) {
        printf("Error: Stack Overflow.\n");
        halt = 0;
    } else {
        sp += numLocals;
    }
        
}
void jump(int loc) {
    // Subtracts 1 to offset global pc increment
    pc = loc - 1;
}
void jmpIfZero(int reg, int loc) {
    if(registerFile[reg] == 0) {
        // Subtracts 1 to offset global pc increment
        pc = loc - 1;
    }
    sp++;
}
void sysOp(int reg, int op) {
	switch (op) {
		case 1:
			write(reg);
            break;
		case 2:
			read(reg);
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
void negate(int stoReg, int froReg) {

}
void add(int stoReg, int reg1, int reg2) {

}
void sub(int stoReg, int reg1, int reg2) {

}
void multiply(int stoReg, int reg1, int reg2) {

}
void divide(int stoReg, int reg1, int reg2) {

}
void odd(int stoReg, int froReg) {

}
void mod(int stoReg, int reg1, int reg2) {

}
void equal(int stoReg, int reg1, int reg2) {

}
void notEqual(int stoReg, int reg1, int reg2) {

}
void less(int stoReg, int reg1, int reg2) {

}
void lessOrEqual(int stoReg, int reg1, int reg2) {

}
void greater(int stoReg, int reg1, int reg2) {

}
void greaterOrEqual(int stoReg, int reg1, int reg2) {

}

// Methods for each System Operation
void write(int reg) {

}
void read(int reg) {

}
void end() {
	halt = 0;
}


// Method to separate the reading of the instructions from the rest of main
int readInstructions(FILE* file) {

	int tmpNum = 0;
	int numCnt = 1;
    // Initialize the code array and first instruction
    code = malloc(sizeof(struct instruction*));
    code[0] = malloc(sizeof(struct instruction));
	//reads in from file 1 number at a time while keeping track of position and 
	//line number.
	while(fscanf(file, "%d", &tmpNum)==1) {
		if(numLines >= MAX_CODE_LENGTH) 
			return 1;

		if(numCnt % 4 == 1) {
			code[numLines]->op = tmpNum;
		} else if(numCnt % 4 == 2) {
			code[numLines]->r = tmpNum;
		} else if (numCnt % 4 == 3) {
            code[numLines]->l = tmpNum;
        } else {
			code[numLines]->m = tmpNum;
				numLines++;
				numCnt=0;
                // Re-size code array to fit next instruction
                // Initializes next instruction
            code = realloc(code, (numLines + 1) * sizeof(struct instruction*));
            code[numLines] = malloc(sizeof(struct instruction));
		}
		numCnt++;
	}
    free(code[numLines]);
    code = realloc(code, (numLines) * sizeof(struct instruction*));

	return 0;
}

#ifdef DISPLAY
// Method to make printing of Instructions easier
void printInstructions() {
	//String array to translate from input number to corresponding operation 
	printf("Line    Op    R   L    M \n");
    char buffer[8] = {};
	for(int i = 0; i < numLines; i++) {
        makeBuffer(buffer, i, 7);
		printf("%d%s%s    %d   %d    %d \n", i, buffer, oper[(code[i]->op) - 1], code[i]->r, code[i]->l, code[i]->m);
	}

}
// Method used to print current state of the machine
void printState(int curLoc) {

    printf("                          pc    bp    sp    registers\n");
    char *buffer = calloc(11, sizeof(char));
    // Prints out the current state with spacing determined by the number of digits
    // in the associated values.
    makeBuffer(buffer, curLoc, 4);
    printf("%d%s", curLoc, buffer);
    makeBuffer(buffer, ir->m, 11);
    printf("%s  %d  %d  %d%s%d", oper[(ir->op)-1], ir->r, ir->l, ir->m, buffer, pc);
    makeBuffer(buffer, pc, 6);
    printf("%s%d", buffer, bp);
    makeBuffer(buffer, bp, 6);
    printf("%s%d", buffer, sp);
    makeBuffer(buffer, sp, 6);
    printf("%s", buffer);
    
    // Prints out the current state of the register file
    for (int i = 0; i < NUM_REGISTERS; i++) {
        makeBuffer(buffer, registerFile[i], 3);
        printf("%d%s", registerFile[i], buffer);
    }
    printf("\nStack: ");
    // Prints out the current state of the data-stack
	for (int i = 0; i < sp; i++) {
        if (endOfRecord[i] == 1) {
            printf("| %d ", stack[i]);
        } else {
            printf("%d ", stack[i]);
        }
    }
    printf("\n\n");
    free(buffer);
}

// Method used for formatting in the event a value requires 2 digits.
// maxSize should always be <= max size of str.
void makeBuffer(char *str, int num, int maxSize) {

    int offset = maxSize - 1;
    
    if (num / 10 > 0) {
        offset--;
        if (num / 100 > 0) {
        offset--;
        }
    }
    if (num < 0) {
        offset--;
    }

    for(int i = 0; i < offset; i++) {
            str[i] = ' ';
        }
        str[offset] = '\0';
}

#endif

// Method to free all arrays instantiated on the heap
void freeAll(FILE *file) {

    // numLInes + 1 because code always has one more element than 
    // number of instructions read.
    destroyCode(code, numLines);
    code = NULL;
    free(stack);
    stack = NULL;
    free(registerFile);
    registerFile = NULL;
    free(file);
    #ifdef DISPLAY
    free(endOfRecord);
    endOfRecord = NULL;
    #endif
}
// Method used to free code array once program is complete.
void destroyCode(struct instruction** ptr, int pLen) {
    for (int i = 0; i < pLen; i++) {
        free(ptr[i]);
    }
    free(ptr);
}

// Method used to redefine base in terms of requested lexicographical level
int base(int l, int base) {
	int b1 = base;
	while (l-- > 0) {
		b1 = stack[b1 - 1];
	}
	return b1;
}