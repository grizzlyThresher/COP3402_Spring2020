// Owen Brahms - 3739927
// Michaela Green - 4198343
// COP 3402, Spring 2020
// Project 3

#include <stdlib.h>
#include <string.h>
#include "pCompiler.h"

// Method to handle parsing and code generation
instruction* parse(lexeme** tokens, int numTokens, FILE* opr, int printParse, int* numInstructions) {
	symbol** symbolTable;
	int numSymbols = 0;
	int curToken = 0;
	static instruction* code;
	if (program(&code, &symbolTable, &numSymbols, tokens, numTokens, numInstructions, &curToken) == 1) {
		return NULL;
	} else {
		return code;
	}

}

// Every non-terminal in the Language Grammar is represented by a function.
int program(instruction** code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

 	if (block(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 		return 1;
 	} else if(tokens[*curToken]->token != periodsym) {
 		fprintf(stderr, "Parsing Error 0%d at Line (%d): Period Expected.\n",
 		 missingPeriodError, tokens[*curToken]->lineNum);
 		return 1;
 	} else {
 		return addInstruction(code, SIO, 0, 0, 3, numInstructions);
 	}

	
}

int block(instruction** code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

 	if (tokens[*curToken]->token == constsym) {
 		if (constdeclaration(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 			return 1;
 		}
 	}
 	if (tokens[*curToken]->token == varsym) {
 		*curToken = *curToken + 1;
 		if (vardeclaration(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 			return 1;
 		}
 	}

	return statement(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken);
}

int constdeclaration(instruction** code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	char* curIdent;
	char* curNum;
	int added;
	do {

		*curToken = *curToken + 1;
		if (tokens[*curToken]->token != identsym) {
			fprintf(stderr, "Parsing Error 0%d at Line (%d): Identifier Expected\n",
			 identifierExpectedError, tokens[*curToken]->lineNum);
			return 1;
		}
		curIdent = tokens[*curToken]->value;

		*curToken = *curToken + 1;
		if (tokens[*curToken]->token != eqsym) {
			fprintf(stderr, "Parsing Error 0%d at Line (%d): \"=\" Expected.\n",
			 eqlExpectedError, tokens[*curToken]->lineNum);
			return 1;
		}

		*curToken = *curToken + 1;
		if (tokens[*curToken]->token != numbersym) {
			fprintf(stderr, "Parsing Error 0%d at Line (%d): Number Expected\n",
			 numberExpectedError, tokens[*curToken]->lineNum);
			return 1;
		}
		curNum = tokens[*curToken]->value;

		// Adds new const to Symbol Table
		added = addSymbol(symbolTable, numSymbols, 1, curIdent, curNum, -1, -1);
		if (added == identifierAlreadyConstError) {
			fprintf(stderr, "Parsing Error 0%d: const \"%s\" Already Exists.\n",
			 constAlreadyExists, curIdent);
		}

		*curToken = *curToken + 1;

	} while (tokens[*curToken]->token == commasym);

	if (tokens[*curToken]->token != semicolonsym) {
		fprintf(stderr, "Parsing Error 0%d at Line (%d): Semicolon Expected\n",
			 semicolonExpectedError, tokens[*curToken]->lineNum);
		return 1;
	}
	*curToken = *curToken + 1;
	return 0;
}

int vardeclaration(instruction** code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

 	char* curIdent;
	int numVars = 0;
	int added;

	do {

		*curToken = *curToken + 1;
		if (tokens[*curToken]->token != identsym) {
			fprintf(stderr, "Parsing Error 0%d at Line (%d): Identifier Expected\n",
			 identifierExpectedError, tokens[*curToken]->lineNum);
			return 1;
		}
		curIdent = tokens[*curToken]->value;

		added = addSymbol(symbolTable, numSymbols, 2, curIdent, "0", 0, numVars++);
		if (added == varAlreadyExistsError) {
			fprintf(stderr, "Parsing Error 0%d: Variable \"%s\" already exists.\n",
			 varAlreadyExistsError, curIdent);
			return 1;
		}

		addInstruction(code, INC, 0, 0, 1, numInstructions);

		*curToken = *curToken + 1;
		
	} while (tokens[*curToken]->token == commasym);

	if (tokens[*curToken]->token != semicolonsym) {
		fprintf(stderr, "Parsing Error 0%d at Line (%d): Semicolon Expected\n",
			 semicolonExpectedError, tokens[*curToken]->lineNum);
		return 1;
	}
	*curToken = *curToken + 1;

	return 0;
}
int statement(instruction** code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	int tmpInstruction, jmpInstruction;
	symbol *curSym;

 	switch (tokens[*curToken]->token) {
 		case identsym:

 			curSym = findSymbol(*symbolTable, tokens[*curToken]->value, *numSymbols);
 			if (curSym == NULL) {
 				return 1;
 			} else if (curSym->kind == 1) {
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): Cannot Reassign Value to Constant %s.\n",
			 constReassignError, tokens[*curToken]->lineNum, curSym->name);
 				return 1;
 			}

 			*curToken = *curToken + 1;
 			if (tokens[*curToken]->token != becomessym) {
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): \":=\" Expected\n",
			 becomesExpectedError, tokens[*curToken]->lineNum);
 				return 1;
 			}

 			*curToken = *curToken + 1;
 			if (expression(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 				return 1;
 			}

 			// Makes sure last operation in expression ends up in Register 0
 			code[0][(*numInstructions) - 1].r = 0;
 			addInstruction(code, STO, 0, 0, curSym->address, numInstructions);
 			break;
 		case beginsym:
 			*curToken = *curToken + 1;
 			do {

 				if (statement(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 					return 1;
 				}
 				*curToken = *curToken + 1;
 			} while (tokens[*curToken]->token == semicolonsym);

 			if (tokens[*curToken]->token != endsym) {
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): \"end\" Expected\n",
			 endExpectedError, tokens[*curToken]->lineNum);
 				return 1;
 			}

 			*curToken = *curToken + 1;
 			break;

 		case ifsym:
 			*curToken = *curToken + 1;
 			if (condition(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 				return 1;
 			}
 			if (tokens[*curToken]->token != thensym) {
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): \"then\" Expected\n",
			 thenExpectedError, tokens[*curToken]->lineNum);
 				return 1;
 			}
 			*curToken = *curToken + 1;
 			tmpInstruction = *numInstructions;
 			addInstruction(code, JPC, 0, 0, 0, numInstructions);

 			if (statement(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 				return 1;
 			}

 			code[0][tmpInstruction].m = (*numInstructions);
 			break;

 		case whilesym:
 			*curToken = *curToken + 1;
 			jmpInstruction = *numInstructions;
 			if (condition(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 				return 1;
 			}
 			if (tokens[*curToken]->token != dosym) {
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): \"do\" Expected\n",
			 doExpectedError, tokens[*curToken]->lineNum);
 				return 1;
 			}

 			*curToken = *curToken + 1;
 			tmpInstruction = *numInstructions;
 			addInstruction(code, JPC, 0, 0, 0, numInstructions);

 			if (statement(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 				return 1;
 			}

 			addInstruction(code, JMP, 0, 0, jmpInstruction, numInstructions);
 			code[0][tmpInstruction].m = (*numInstructions);
 			break;

 		case readsym:

 			*curToken = *curToken + 1;
 			curSym = findSymbol(*symbolTable, tokens[*curToken]->value, *numSymbols);
 			if (curSym == NULL) {
 				return 1;
 			} else if (curSym->kind == 1) {
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): Cannot Reassign Value to Constant %s.\n",
			 constReassignError, tokens[*curToken]->lineNum, curSym->name);
 				return 1;
 			}
 			addInstruction(code, SIO, 0, 0, 2, numInstructions);
 			addInstruction(code, STO, 0, 0, curSym->address, numInstructions);
 			*curToken = *curToken + 1;
 			break;

 		case writesym:

 			*curToken = *curToken + 1;
 			curSym = findSymbol(*symbolTable, tokens[*curToken]->value, *numSymbols);
 			if (curSym == NULL) {
 				return 1;
 			}
 			addInstruction(code, LOD, 0, 0, curSym->address, numInstructions);
 			addInstruction(code, SIO, 0, 0, 1, numInstructions);
 			*curToken = *curToken + 1;

 			break;

 		default:
 			return 0;
 			break;
 	}

	return 0;
}
int condition(instruction** code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {
 	if (tokens[*curToken]->token == oddsym) {
 		*curToken = *curToken + 1;
 		expression(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken);

 		code[0][(*numInstructions) - 1].r = 0;
 		addInstruction(code, ODD, 0, 0, 0, numInstructions);
 	} else {
 		expression(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken);

 		code[0][(*numInstructions) - 1].r = 0;

 		op_code op;
 		switch(tokens[*curToken]->token) {
 			case eqsym:
 				op = EQL;
 				break;
 			case neqsym:
 				op = NEQ;
 				break;
 			case lessym:
 				op = LSS;
 				break;
 			case leqsym:
 				op = LEQ;
 				break;
 			case gtrsym:
 				op = GTR;
 				break;
 			case geqsym:
 				op = GEQ;
 				break;
 			default:
  				fprintf(stderr, "Parsing Error 0%d at Line (%d): Relational Operator Expected\n",
			 		relopExpectedError, tokens[*curToken]->lineNum);
 				return 1;
 		}

 		*curToken = *curToken + 1;
 		expression(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken);

 		code[0][(*numInstructions) - 1].r = 1;
 		addInstruction(code, op, 0, 0, 1, numInstructions);
 	}

	return 0;
}
int expression(instruction** code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int term(instruction** code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int factor(instruction** code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}

// Method used to add symbols to the symbol table.
int addSymbol(symbol*** symbolTable, int* numSymbols, int kind, char* name, char* val, int level, int address) {
	for (int i = *numSymbols; i>= 0; i++) {
		if ((strcmp(symbolTable[0][i]->name, name) == 0)) {
			if (symbolTable[0][i]->kind == 1) {

				return identifierAlreadyConstError;

			} else if (symbolTable[0][i]->level == level) {
				
				return varAlreadyExistsError;
			}
		}
	}
	*numSymbols = *numSymbols + 1;
	symbolTable[0] = realloc(symbolTable[0], (*numSymbols)*sizeof(symbol*));
	symbolTable[0][*numSymbols - 1] = malloc(sizeof(symbol));
	symbolTable[0][*numSymbols - 1]->kind = kind;
	symbolTable[0][*numSymbols - 1]->name = malloc(strlen(name) * sizeof(char));
	strcpy(symbolTable[0][*numSymbols - 1]->name, name);
	symbolTable[0][*numSymbols - 1]->val = convertToInt(val);
	symbolTable[0][*numSymbols - 1]->level = level;
	symbolTable[0][*numSymbols - 1]->address = address;
	symbolTable[0][*numSymbols - 1]->mark = 0;
	return 0;
}

// Method used for symbol lookup.
symbol* findSymbol(symbol** symbolTable, char* name, int numSymbols) {
	for (int i = numSymbols - 1; i >= 0; i--) {
		if ((strcmp(symbolTable[i]->name, name) == 0) && symbolTable[i]->mark == 0) {
			return symbolTable[i];
		}
	}
	fprintf(stderr, "Parsing Error 0%d: Identifier \"%s\" Does Not Exist.\n", identifierDoesntExistError, name);
	return NULL;

}

// Method used to simplify symbol deactivation.
int deleteSymbol(symbol** symbolTable, char* name, int numSymbols) {
	symbol* removing = findSymbol(symbolTable, name, numSymbols);
	if (removing == NULL) 
		return 1;
	// Sets Symbol as no longer being used for code generation
	removing->mark = 1;
	return 0;
}

// Adds new instruction to the given code array.
int addInstruction(instruction** code, op_code op, int r, int l, int m, int* numInstructions) {
	*numInstructions = *numInstructions + 1;
	if (*numInstructions > MAX_CODE_LENGTH) {
		fprintf(stderr, "Error: Generated Assembly Code Too Long.\n");
		return 1;
	}
	code[0] = realloc(code[0], (*numInstructions * sizeof(instruction)));
	code[0][*numInstructions - 1].op = op;
	code[0][*numInstructions - 1].r = r;
	code[0][*numInstructions - 1].l = l;
	code[0][*numInstructions - 1].m = m;

	return 0;

}

int convertToInt(char* num) {
	int value = 0;
	int numLen = strlen(num);
	for (int i = 0; i < numLen; i++) {
		// Conversion from ASCII value to integer value, multiplied by appropriate power of 10
		value += (num[numLen] - '0') * power10(numLen - (i + 1));
	}
	return value;
}


int power10(int place) {
	int value = 1;
	for (int i = 1; i <= place; i++) {
		value *= 10;
	}
	return value;
}