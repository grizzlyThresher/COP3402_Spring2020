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

	return 1;
}
int block(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int constdeclaration(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

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
int number(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int identifier(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int digit(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}
int letter(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken) {

	return 0;
}

// Method used to add symbols to the symbol table.
int addSymbol(symbol*** symbolTabe, int* numSymbols, int kind, char* name, int val, int level, int address) {
	for (int i = *numSymbols; i>= 0; i++) {
		if ((strcmp(symbolTabe[0][i]->name, name) == 0)) {
			if (symbolTabe[0][i]->kind == 1) {
				fprintf(stderr, "Error: Identifier \"%s\" already exists as a constant.\n",name);
				return 1;
			} else if (symbolTabe[0][i]->level == level) {
				fprintf(stderr, "Error: Variable \"%s\" already exists.\n",name);
				return 1;
			}
		}
	}
	*numSymbols = *numSymbols + 1;
	symbolTabe[0] = realloc(symbolTabe[0], (*numSymbols)*sizeof(symbol*));
	symbolTabe[0][*numSymbols - 1] = malloc(sizeof(symbol));
	symbolTabe[0][*numSymbols - 1]->kind = kind;
	symbolTabe[0][*numSymbols - 1]->name = malloc(strlen(name) * sizeof(char));
	strcpy(symbolTabe[0][*numSymbols - 1]->name, name);
	symbolTabe[0][*numSymbols - 1]->val = val;
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
	fprintf(stderr, "Error: Identifier \"%s\" Does Not Exist.\n",name);
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