// Owen Brahms - 3739927
// Michaela Green - 4198343
// COP 3402, Spring 2020
// Project 1

#include "pMachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {   

    // Instantiation of Global variables
    int sp = 0;
    int bp = 1;
    int pc = 0;
    struct instruction ir, *code;
    int* stack = calloc(MAX_DATASTACK_HEIGHT, sizeof(int));
    int* registerFile = calloc(NUM_REGISTERS, sizeof(int));
    int halt = 0;
    int numLines = 0;

    // Keeps track of legicographical level for printing purposes
    int curLex = 0;

    // Used for Input and Output files
    FILE* ipr;
    // FILE must be static because otherwise it can't be assigned to stdout
    static FILE* opr;

    // If display is entered before the file to be read when running the program, stack trace is printed to console
    // Otherwise, file is read normally and stack trace is printed to output.txt
    char *inFile;
    if (argv[1] == NULL) {
        fprintf(stderr, "System requires either command or file name.\n");
        return 0;
    } else if (strcmp("display",argv[1]) == 0) {
        opr = stdout;
        inFile = argv[2];
    } else {
        inFile = argv[1];
        char* outFile;
        if (argv[2] == NULL) {
            outFile = "output.txt";
        } else {
            outFile = argv[2];
        }
        opr = fopen(outFile, "w");
        if(opr == NULL) {
            fprintf(stderr, "Could not find valid file by name: %s \n", outFile);
            return 0;
        }
    }

	ipr = fopen(inFile, "r");
	if(ipr == NULL) {
		fprintf(stderr, "Could not find valid file by name: %s \n", inFile);
		return 0;
	}

    // List of all possible operations the Virtual Machines can perform
    char *operations[] = {"lit","ret","lod","sto","cal","inc","jmp","jpc","sio",
    "neg", "add", "sub", "mul", "div", "odd", "mod", "eql", "neq", "lss",
    "leq", "gtr", "geq"};

    fprintf(opr, "Line    Op    R   L    M \n");
	if(readInstructions(&code, ipr, opr, operations, &numLines) == 1) {
		fprintf(stderr, "Program too long for machine to run.");
		return 0;
	}
    fclose(ipr);

	fprintf(opr, "\n\n                          pc    bp    sp    registers\n");
	fprintf(opr, "Initial values:           %d     %d     %d  ", pc,bp,sp);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        fprintf(opr, "   0");
    }
    fprintf(opr, "\n\n");


    // Temp variable used to store pc value before execution
    int tmpPc = pc;

	while(halt == 0) {
		// Fetch
		ir = code[pc];
        tmpPc = pc;
        pc++;
		// Execute
        // sysOp is repeated intentionally to comply with specs given in the HW file.
		switch (ir.op) {
			case 1: // LIT
                registerFile[ir.r] = ir.m;
                break;
			case 2: // RET
				if (bp != 1) {
                    sp = bp - 1;
                    bp = stack[sp + 3];
                    pc = stack[sp + 4];
                    curLex--;
                }
                break;
			case 3: // LOD
                if (base(stack, ir.l, bp) + ir.m >= MAX_DATASTACK_HEIGHT) {
                    OUT_OF_BOUNDS
                    halt = 1;
                } else if (base(stack, ir.l, bp) + ir.m > 0 && base(stack, ir.l, bp) + ir.m <= sp) {

                    registerFile[ir.r] = stack[base(stack, ir.l, bp) + ir.m];

                } else {
                    OUT_OF_BOUNDS
                    halt = 1;
                }
                break;
			case 4: // STO
                if (base(stack, ir.l, bp) + ir.m >= MAX_DATASTACK_HEIGHT) {
                    STACK_OVERFLOW
                    halt = 1;
                } else if (base(stack, ir.l, bp) + ir.m > 0 && base(stack, ir.l, bp) + ir.m <= sp) {

                    stack[base(stack, ir.l, bp) + ir.m] = registerFile[ir.r];

                } else {
                    OUT_OF_BOUNDS
                    halt = 1;
                }
                break;
			case 5: // CAL
				if (sp + 4 >= MAX_DATASTACK_HEIGHT) {
                    STACK_OVERFLOW
                    halt = 1;
                } else {
                    stack[sp + 1] = 0;
                    stack[sp + 2] = base(stack, ir.l, bp);
                    stack[sp + 3] = bp;
                    stack[sp + 4] = pc;
                    bp = sp + 1;

                    pc = ir.m;
                    curLex++;
                }
                break;
			case 6: // INC
				if (sp + ir.m >= MAX_DATASTACK_HEIGHT) {
                    STACK_OVERFLOW
                    halt = 1;
                } else {
                    sp += ir.m;
                }
                break;
			case 7: // JMP
                pc = ir.m;
                break;
			case 8: // JPC
                if(registerFile[ir.r] == 0) {
                    pc = ir.m;
                }
                break;
			case 9: // SIO
				switch (ir.m) {
                    case 1: // WRITE
                    	printf("%d\n", registerFile[ir.r]);
                        break;
                    case 2: // READ
	                    printf("Please input a value: ");
	                    scanf("%d", &registerFile[ir.r]);
                        fprintf(opr, "user input was: %d\n", registerFile[ir.r]);
                        break;
                    case 3: // halt = 1;
                        halt = 1;
                        break;
                    default:
                        BAD_OPERATION
                        return 0;
                    break;
                }
                break;
            case 10: // NEG
            	registerFile[ir.r] = -1 * registerFile[ir.m];
                break;
            case 11: // ADD
                registerFile[ir.r] = registerFile[ir.l] + registerFile[ir.m];
                break;
            case 12: // SUB
            	registerFile[ir.r] = registerFile[ir.l] - registerFile[ir.m];
                break;
            case 13: // MUL
            	registerFile[ir.r] = registerFile[ir.l] * registerFile[ir.m];
                break;	
            case 14: // DIV
            	registerFile[ir.r] = registerFile[ir.l] / registerFile[ir.m];
                break;
            case 15: // ODD
            	registerFile[ir.r] = registerFile[ir.m] % 2;
                break;
            case 16: // MOD
            	registerFile[ir.r] = registerFile[ir.l] % registerFile[ir.m];
                break;
            case 17: // EQL
            	registerFile[ir.r] = (registerFile[ir.l] == registerFile[ir.m]);
                break;
            case 18: // NEQ
            	registerFile[ir.r] = (registerFile[ir.l] != registerFile[ir.m]);
                break;
            case 19: // LSS
            	registerFile[ir.r] = (registerFile[ir.l] < registerFile[ir.m]);
                break;
            case 20: // LEQ
                registerFile[ir.r] = (registerFile[ir.l] <= registerFile[ir.m]);           
                break;
            case 21: // GTR
            	registerFile[ir.r] = (registerFile[ir.l] > registerFile[ir.m]);
                break;
            case 22: // GEQ
            	registerFile[ir.r] = (registerFile[ir.l] >= registerFile[ir.m]);
                break;
            default:
                BAD_OPERATION
                return 0;
                break;
		}
        // Prints out current state of the machine
		printState(stack, tmpPc, ir, pc, bp, sp, registerFile, curLex, opr, operations);
	}

    // Frees all remaining pointers on the heap
    free(code);
    free(stack);
    free(registerFile);
    if(strcmp("display",argv[1])) {
        fclose(opr);
    }
    
    return 0;
}

// Method to separate the reading of the instructions from the rest of main
int readInstructions(struct instruction** code, FILE* input, FILE* output, char** oper, int* lines) {

	int tmpNum = 0;
	int numCnt = 1;
    int numLines = *lines;
    char buffer[8] = {};

    // Initialize the code array with first instruction
    code[0] = malloc(sizeof(struct instruction));
	//reads in from input file 1 number at a time while keeping track of position and 
	//line number.
	while(fscanf(input, "%d", &tmpNum)==1) {
		if(numLines >= MAX_CODE_LENGTH) 
			return 1;

		if(numCnt % 4 == 1) {
			code[0][numLines].op = tmpNum;
		} else if(numCnt % 4 == 2) {
			code[0][numLines].r = tmpNum;
		} else if (numCnt % 4 == 3) {
            code[0][numLines].l = tmpNum;
        } else {
			code[0][numLines].m = tmpNum;
            // Prints out newly added instruction
            makeBuffer(buffer, numLines, 7);
            fprintf(output, "%d%s%s    %d   %d    %d \n", numLines, buffer,
             oper[(code[0][numLines].op) - 1], code[0][numLines].r, code[0][numLines].l, code[0][numLines].m);

			numLines++;
			numCnt=0;

            // Re-size code array to fit next instruction
            code[0] = realloc(code[0], (numLines + 1) * sizeof(struct instruction));
            
		}
		numCnt++;
	}
    code[0] = realloc(code[0], (numLines) * sizeof(struct instruction));
    *lines = numLines;

	return 0;
}
// Method used to print current state of the machine
void printState(int* stack, int curLoc, struct instruction ir, int pc, int bp, int sp, int* regFile, int lex, FILE* output, char** oper) {

//    fprintf(opr, "                          pc    bp    sp    registers\n");
    char *buffer = calloc(11, sizeof(char));
    // Prints out the current state with spacing determined by the number of digits
    // in the associated values.
    makeBuffer(buffer, curLoc, 4);
    fprintf(output, "%d%s", curLoc, buffer);
    makeBuffer(buffer, ir.m, 11);
    fprintf(output, "%s  %d  %d  %d%s%d", oper[(ir.op)-1], ir.r, ir.l, ir.m, buffer, pc);
    makeBuffer(buffer, pc, 6);
    fprintf(output, "%s%d", buffer, bp);
    makeBuffer(buffer, bp, 6);
    fprintf(output, "%s%d", buffer, sp);
    makeBuffer(buffer, sp, 6);
    fprintf(output, "%s", buffer);
    
    // Prints out the current state of the register file
    for (int i = 0; i < NUM_REGISTERS; i++) {
        makeBuffer(buffer, regFile[i], 4);
        fprintf(output, "%d%s", regFile[i], buffer);
    }

    // Prints out the current state of the stack
    fprintf(output, "\nStack: ");
    int lineCnt = lex;

	for (int i = 1; i <= sp; i++) {
        if (lineCnt > 0 && i == base(stack, lineCnt - 1, bp)) {
            fprintf(output, "| %d ", stack[i]);
            lineCnt--;
        } else {
            fprintf(output, "%d ", stack[i]);
        }
    }
    fprintf(output, "\n\n");
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
// Method used to redefine base in terms of requested lexicographical level
int base(int* stack, int l, int base) {
	int b1 = base;
	while (l-- > 0) {
		b1 = stack[b1 + 1];
	}
	return b1;
}