// Owen Brahms - 3739927
// Michaela Green - 4198343
// COP 3402, Spring 2020
// Project 3

#include <stdlib.h>
#include <string.h>
#include "pCompiler.h"

// Method to handle parsing and code generation
instruction* parse(lexeme** tokens, int numTokens, FILE* opr, int printParse, int* numInstructions) {
	symbol** symbolTabe;
	int numSymbols = 0;
	int curToken = 0;
	static instruction* code;
	if (program(&code, &symbolTabe, &numSymbols, tokens, numTokens, numInstructions, &curToken) == 1) {
		return NULL;
	} else {
		return code;
	}

}

// Every non-terminal in the Language Grammar is represented by a function.
int program(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

 	if (block(code, symbolTabe, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 		return 1;
 	} else if(tokens[*curToken]->token != periodsym) {
 		fprintf(stderr, "Parsing Error 0%d at Line (%d): Period Expected.\n",
 		 missingPeriodError, tokens[*curToken]->lineNum);
 		return 1;
 	} else {
 		return addInstruction(code, SIO, 0, 0, 3, numInstructions);
 	}

	
}

int block(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

 	if (tokens[*curToken]->token == constsym) {
 		if (constdeclaration(code, symbolTabe, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 			return 1;
 		}
 	}
 	if (tokens[*curToken]->token == varsym) {
 		*curToken = *curToken + 1;
 		if (vardeclaration(code, symbolTabe, numSymbols, tokens, numTokens, numInstructions, curToken) == 1) {
 			return 1;
 		}
 	}

	return statement(code, symbolTabe, numSymbols, tokens, numTokens, numInstructions, curToken);
}

int constdeclaration(instruction** code, symbol*** symbolTabe, int* numSymbols,
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
		added = addSymbol(symbolTabe, numSymbols, 1, curIdent, curNum, -1, -1);
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

int vardeclaration(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int statement(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int condition(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int relop(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int expression(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int term(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int factor(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}

// Method used to add symbols to the symbol table.
int addSymbol(symbol*** symbolTabe, int* numSymbols, int kind, char* name, char* val, int level, int address) {
	for (int i = *numSymbols; i>= 0; i++) {
		if ((strcmp(symbolTabe[0][i]->name, name) == 0)) {
			if (symbolTabe[0][i]->kind == 1) {

				return identifierAlreadyConstError;

			} else if (symbolTabe[0][i]->level == level) {
				fprintf(stderr, "Parsing Error 0%d: Variable \"%s\" already exists.\n", varAlreadyExistsError, name);
				return varAlreadyExistsError;
			}
		}
	}
	*numSymbols = *numSymbols + 1;
	symbolTabe[0] = realloc(symbolTabe[0], (*numSymbols)*sizeof(symbol*));
	symbolTabe[0][*numSymbols - 1] = malloc(sizeof(symbol));
	symbolTabe[0][*numSymbols - 1]->kind = kind;
	symbolTabe[0][*numSymbols - 1]->name = malloc(strlen(name) * sizeof(char));
	strcpy(symbolTabe[0][*numSymbols - 1]->name, name);
	symbolTabe[0][*numSymbols - 1]->val = convertToInt(val);
	symbolTabe[0][*numSymbols - 1]->level = level;
	symbolTabe[0][*numSymbols - 1]->address = address;
	symbolTabe[0][*numSymbols - 1]->mark = 0;
	return 0;
}

// Method used for symbol lookup.
symbol* findSymbol(symbol** symbolTabe, char* name, int numSymbols) {
	for (int i = numSymbols - 1; i >= 0; i--) {
		if ((strcmp(symbolTabe[i]->name, name) == 0) && symbolTabe[i]->mark == 0) {
			return symbolTabe[i];
		}
	}
	fprintf(stderr, "Parsing Error 0%d: Identifier \"%s\" Does Not Exist.\n", identifierDoesntExistError, name);
	return NULL;

}

// Method used to simplify symbol deactivation.
int deleteSymbol(symbol** symbolTabe, char* name, int numSymbols) {
	symbol* removing = findSymbol(symbolTabe, name, numSymbols);
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