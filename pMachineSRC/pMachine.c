//Owen Brahms - 3739927
//Michaela Green - 
#include "pMachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaration of Global variables and arrays used throughout the VM
int sp, bp, pc, halt, numLines, *stack, *registerFile, *endOfRecord;
struct instruction *ir, **code;
FILE *opr;

char *oper[] = {"lit","ret","lod","sto","cal","inc","jmp","jpc","sio", "sio",
    "sio", "neg", "add", "sub", "mul", "div", "odd", "mod", "eql", "neq", "lss",
    "leq", "gtr", "geq"};


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
    endOfRecord = calloc(MAX_DATASTACK_HEIGHT, sizeof(int));

    char inFile[] = "test.txt";
	FILE *ipr = fopen(inFile, "r");

	if(ipr == NULL) {
		fprintf(stderr, "Could not find valid file by name: %s \n", inFile);
		return 0;
	}


	if(readInstructions(ipr) == 1) {
		fprintf(stderr, "Program too long for machine to run.");
		return 0;
	}

    #ifndef DISPLAY
    char outFile[] = "output.txt";
    opr = fopen(outFile, "w");
    if(opr == NULL) {
        fprintf(stderr, "Could not find valid file by name: %s \n", outFile);
        return 0;
    }
    #endif
    #ifdef DISPLAY
    opr = stdout;
    #endif
    // Will only print the instructions and stack trace while DISPLAY the VM
    // Avoids messiness for actual use of the machine.

	printInstructions();
	fprintf(opr, "\n\n                          pc    bp    sp    registers\n");
	fprintf(opr, "Initial values:           %d     %d     %d   ", pc,bp,sp);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        fprintf(opr, "  0");
    }
    fprintf(opr, "\n\n");


    // Temp variable used to store pc value before execution
    int tmpPc = pc;

	while(halt != 0) {
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
                BAD_OPERATION
                HALT
                break;
		}

        // Increments the Program Counter
        pc++;
        
		printState(tmpPc);
	}

    // Frees all remaining pointers on the heap
    freeAll(ipr, opr);
    
    return 0;
}

// Methods for each ISA input
void literal(int reg, int val) {
    if (reg < 0 || reg >= NUM_REGISTERS) {
        INVALID_REGISTER
        HALT
    } else {
        registerFile[reg] = val;
    }

}
void ret() {
    if (bp != 1) {
        sp = bp - 1;
        bp = stack[sp + 3];
        pc = stack[sp + 4];
    }

}
void load(int reg, int lex, int offset) {
    int place = base(lex, bp) + offset;

    if (reg < 0 || reg >= NUM_REGISTERS) {
        INVALID_REGISTER
        HALT
    }
    if (place >= MAX_DATASTACK_HEIGHT) {
        OUT_OF_BOUNDS
        HALT
    } else if (place > 0) {
        registerFile[reg] = stack[place];
    } else {
       OUT_OF_BOUNDS
        HALT
    }
    
}
void store(int reg, int lex, int offset) {
    if (reg < 0 || reg >= NUM_REGISTERS) {
        INVALID_REGISTER
        HALT
    }
    int place = base(lex, bp) + offset;
    if (place >= MAX_DATASTACK_HEIGHT) {
        STACK_OVERFLOW
        HALT
    } else if (place > 0) {
        stack[place] = registerFile[reg];
    } else {
        OUT_OF_BOUNDS
        HALT
    }
    
}
void call(int lex, int loc) {
    if (sp + 4 >= MAX_DATASTACK_HEIGHT) {
        STACK_OVERFLOW
        HALT
    } else {
        stack[sp + 1] = 0;
        stack[sp + 2] = base(lex, bp);
        stack[sp + 3] = bp;
        stack[sp + 4] = pc;
        bp = sp + 1;
        // Subtracts 1 to offset global pc increment
        pc = loc - 1;

        endOfRecord[sp] = 1;
    }

}
void inc(int numLocals) {

    if (sp + numLocals >= MAX_DATASTACK_HEIGHT) {
        STACK_OVERFLOW
        HALT
    } else {
        sp += numLocals;
    }
        
}
void jump(int loc) {
    // Throws an error and stops the program if given line of code doesn't exist
    if (loc < 0 || loc >= numLines) {
        BAD_INSTRUCTION
        HALT
    }
    // Subtracts 1 to offset global pc increment
    pc = loc - 1;
}
void jmpIfZero(int reg, int loc) {

    // Throws an error and stops the program if given line of code doesn't exist
    if (loc < 0 || loc >= numLines) {
        BAD_INSTRUCTION
        HALT
    }

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
            BAD_OPERATION
            HALT
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
	HALT
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

// Method to make printing of Instructions easier
void printInstructions() {
	//String array to translate from input number to corresponding operation 
	fprintf(opr, "Line    Op    R   L    M \n");
    char buffer[8] = {};
	for(int i = 0; i < numLines; i++) {
        makeBuffer(buffer, i, 7);
		fprintf(opr, "%d%s%s    %d   %d    %d \n", i, buffer, oper[(code[i]->op) - 1], code[i]->r, code[i]->l, code[i]->m);
	}

}
// Method used to print current state of the machine
void printState(int curLoc) {

//    fprintf(opr, "                          pc    bp    sp    registers\n");
    char *buffer = calloc(11, sizeof(char));
    // Prints out the current state with spacing determined by the number of digits
    // in the associated values.
    makeBuffer(buffer, curLoc, 4);
    fprintf(opr, "%d%s", curLoc, buffer);
    makeBuffer(buffer, ir->m, 11);
    fprintf(opr, "%s  %d  %d  %d%s%d", oper[(ir->op)-1], ir->r, ir->l, ir->m, buffer, pc);
    makeBuffer(buffer, pc, 6);
    fprintf(opr, "%s%d", buffer, bp);
    makeBuffer(buffer, bp, 6);
    fprintf(opr, "%s%d", buffer, sp);
    makeBuffer(buffer, sp, 6);
    fprintf(opr, "%s", buffer);
    
    // Prints out the current state of the register file
    for (int i = 0; i < NUM_REGISTERS; i++) {
        makeBuffer(buffer, registerFile[i], 3);
        fprintf(opr, "%d%s", registerFile[i], buffer);
    }
    fprintf(opr, "\nStack: ");
    // Prints out the current state of the data-stack
	for (int i = 1; i <= sp; i++) {
        if (endOfRecord[i] == 1 && i != sp) {
            fprintf(opr, "%d | ", stack[i]);
        } else {
            fprintf(opr, "%d ", stack[i]);
        }
    }
    fprintf(opr, "\n\n");
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

// Method to free all arrays instantiated on the heap
void freeAll(FILE *input, FILE *output) {

    // numLInes + 1 because code always has one more element than 
    // number of instructions read.
    destroyCode(code, numLines);
    free(stack);
    free(registerFile);
    free(input);
    free(endOfRecord);
    #ifndef DISPLAY
    free(output);
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