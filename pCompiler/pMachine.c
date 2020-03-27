// Owen Brahms - 3739927
// Michaela Green - 4198343
// COP 3402, Spring 2020
// Project 1

#include <stdlib.h>
#include <string.h>
#include "pCompiler.h"


void execute(instruction* code, FILE* opr, int printMachine, int printParse, int numInstructions) {   

    // Instantiation of Global variables
    int sp = 0;
    int bp = 1;
    int pc = 0;
    instruction ir;
    char buffer[8] = {};
    int* stack = calloc(MAX_DATASTACK_HEIGHT, sizeof(int));
    int* registerFile = calloc(NUM_REGISTERS, sizeof(int));
    int halt = 0;

    // Keeps track of legicographical level for printing purposes
    int curLex = 0;

    // List of all possible operations the Virtual Machines can perform
    char *operations[] = {"lit","ret","lod","sto","cal","inc","jmp","jpc","sio",
    "neg", "add", "sub", "mul", "div", "odd", "mod", "eql", "neq",
    "lss", "leq", "gtr", "geq"};

    // Prints instructions to output file
    for(int i = 0; i < numInstructions; i++) {
        makeBuffer(buffer, i, 7);
        fprintf(opr, "%d%s%s    %d   %d    %d \n", i, buffer,
            operations[(code[i].op) - 1], code[i].r, code[i].l, code[i].m);
    }

    // Prints instructions to console IF printParse is active
    if (printParse == 1) {
        for(int i = 0; i < numInstructions; i++) {
            makeBuffer(buffer, i, 7);
            fprintf(stdout, "%d%s%s    %d   %d    %d \n", i, buffer,
                operations[(code[i].op) - 1], code[i].r, code[i].l, code[i].m);
        }
    }

	fprintf(opr, "\n\n                          pc    bp    sp    registers\n");
    if(printMachine == 1) fprintf(stdout, "\n\n                          pc    bp    sp    registers\n");
	fprintf(opr, "Initial values:           %d     %d     %d  ", pc,bp,sp);
    if(printMachine == 1) fprintf(stdout, "Initial values:           %d     %d     %d  ", pc,bp,sp);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        fprintf(opr, "   0");
        if(printMachine == 1) fprintf(stdout, "   0");
    }
    fprintf(opr, "\n\n");
    if(printMachine == 1) fprintf(stdout, "\n\n");

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
			case LIT: // LIT
                registerFile[ir.r] = ir.m;
                break;
			case RET: // RET
				if (bp != 1) {
                    sp = bp - 1;
                    bp = stack[sp + 3];
                    pc = stack[sp + 4];
                    curLex--;
                }
                break;
			case LOD: // LOD
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
			case STO: // STO
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
			case CAL: // CAL
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
			case INC: // INC
				if (sp + ir.m >= MAX_DATASTACK_HEIGHT) {
                    STACK_OVERFLOW
                    halt = 1;
                } else {
                    sp += ir.m;
                }
                break;
			case JMP: // JMP
                pc = ir.m;
                break;
			case JPC: // JPC
                if(registerFile[ir.r] == 0) {
                    pc = ir.m;
                }
                break;
			case SIO: 
                switch (ir.m) {
                    case 1: // SIO WRITE
                        printf("%d\n", registerFile[ir.r]);
                        break;
                    case 2: // SIO READ
	                    printf("Please input a value: ");
	                    scanf("%d", &registerFile[ir.r]);
                        fprintf(opr, "user input was: %d\n", registerFile[ir.r]);
                        break;
                    case 3: // SIO HALT
                        halt = 1;
                        break;
                }
                break;
            case NEG: // NEG
            	registerFile[ir.r] = -1 * registerFile[ir.r];
                break;
            case ADD: // ADD
                registerFile[ir.r] = registerFile[ir.l] + registerFile[ir.m];
                break;
            case SUB: // SUB
            	registerFile[ir.r] = registerFile[ir.l] - registerFile[ir.m];
                break;
            case MUL: // MUL
            	registerFile[ir.r] = registerFile[ir.l] * registerFile[ir.m];
                break;	
            case DIV: // DIV
            	registerFile[ir.r] = registerFile[ir.l] / registerFile[ir.m];
                break;
            case ODD: // ODD
            	registerFile[ir.r] = registerFile[ir.m] % 2;
                break;
            case MOD: // MOD
            	registerFile[ir.r] = registerFile[ir.l] % registerFile[ir.m];
                break;
            case EQL: // EQL
            	registerFile[ir.r] = (registerFile[ir.l] == registerFile[ir.m]);
                break;
            case NEQ: // NEQ
            	registerFile[ir.r] = (registerFile[ir.l] != registerFile[ir.m]);
                break;
            case LSS: // LSS
            	registerFile[ir.r] = (registerFile[ir.l] < registerFile[ir.m]);
                break;
            case LEQ: // LEQ
                registerFile[ir.r] = (registerFile[ir.l] <= registerFile[ir.m]);           
                break;
            case GTR: // GTR
            	registerFile[ir.r] = (registerFile[ir.l] > registerFile[ir.m]);
                break;
            case GEQ: // GEQ
            	registerFile[ir.r] = (registerFile[ir.l] >= registerFile[ir.m]);
                break;
            default:
                BAD_OPERATION
                halt = 1;
                return;
                break;
		}
        // Prints out current state of the machine
		printState(stack, tmpPc, ir, pc, bp, sp, registerFile, curLex, opr, operations, printMachine);
	}

    // Frees all remaining pointers on the heap
    free(stack);
    free(registerFile);
}

// Method used to print current state of the machine
void printState(int* stack, int curLoc, instruction ir, int pc, int bp, int sp, int* regFile, int lex, FILE* output, char** oper, int printMachine) {

//    fprintf(opr, "                          pc    bp    sp    registers\n");
    char *buffer = calloc(11, sizeof(char));
    // Prints out the current state with spacing determined by the number of digits
    // in the associated values.
    makeBuffer(buffer, curLoc, 4);
    fprintf(output, "%d%s", curLoc, buffer);
    if(printMachine == 1) fprintf(stdout, "%d%s", curLoc, buffer);
    makeBuffer(buffer, ir.m, 11);
    fprintf(output, "%s  %d  %d  %d%s%d", oper[(ir.op)-1], ir.r, ir.l, ir.m, buffer, pc);
    if(printMachine == 1) fprintf(stdout, "%s  %d  %d  %d%s%d", oper[(ir.op)-1], ir.r, ir.l, ir.m, buffer, pc);
    makeBuffer(buffer, pc, 6);
    fprintf(output, "%s%d", buffer, bp);
    if(printMachine == 1) fprintf(stdout, "%s%d", buffer, bp);
    makeBuffer(buffer, bp, 6);
    fprintf(output, "%s%d", buffer, sp);
    if(printMachine == 1) fprintf(stdout, "%s%d", buffer, sp);
    makeBuffer(buffer, sp, 6);
    fprintf(output, "%s", buffer);
    if(printMachine == 1) fprintf(stdout, "%s", buffer);
    
    // Prints out the current state of the register file
    for (int i = 0; i < NUM_REGISTERS; i++) {
        makeBuffer(buffer, regFile[i], 4);
        fprintf(output, "%d%s", regFile[i], buffer);
        if(printMachine == 1) fprintf(stdout, "%d%s", regFile[i], buffer);
    }

    // Prints out the current state of the stack
    fprintf(output, "\nStack: ");
    if(printMachine == 1) fprintf(stdout, "\nStack: ");
    int lineCnt = lex;

	for (int i = 1; i <= sp; i++) {
        if (lineCnt > 0 && i == base(stack, lineCnt - 1, bp)) {
            fprintf(output, "| %d ", stack[i]);
            if(printMachine == 1) fprintf(stdout, "| %d ", stack[i]);
            lineCnt--;
        } else {
            fprintf(output, "%d ", stack[i]);
            if(printMachine == 1) fprintf(stdout, "%d ", stack[i]);
        }
    }
    fprintf(output, "\n\n");
    if(printMachine == 1) fprintf(stdout, "\n\n");
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