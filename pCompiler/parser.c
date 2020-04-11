// Owen Brahms - 3739927
// Michaela Green - 4198343
// COP 3402, Spring 2020
// Project 3

#include <stdlib.h>
#include <string.h>
#include "pCompiler.h"

// Method to handle parsing and code generation
instruction* parse(lexeme** tokens, int numTokens, FILE* opr, int* numInstructions) {
	symbol** symbolTable = NULL;
	static instruction code[MAX_CODE_LENGTH];
	int numSymbols = 0;
	int curToken = 0;

	// If the parse ran successfully, return the generated code
	if (program(code, &symbolTable, &numSymbols, tokens, numTokens, numInstructions, &curToken, opr) == 0) {
		return code;
	} else {

	// Otherwise, return NULL
		return NULL;
	}
}

// Every non-terminal in the Language Grammar is represented by a function.
int program(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, FILE* opr) {

	int firstInstruction;
	int curLexLevel = 0;

	// New thingy u said to add
	addInstruction(code, JMP, 0, 0, 0, numInstructions, opr);

	// If block returns 1, end
 	if (block(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, opr, curLexLevel, NULL) == 1) {
 		return 1;
 	}

 	

 	 // If token is a . ...
 	else if(tokens[*curToken]->token != periodsym) {

 		// Prints an error to output file and console if a period was expected
 		fprintf(stderr, "Parsing Error 0%d at Line (%d): Period Expected.\n",
 		 missingPeriodError, tokens[*curToken]->lineNum);
 		fprintf(opr, "Parsing Error 0%d at Line (%d): Period Expected.\n",
 		 missingPeriodError, tokens[*curToken]->lineNum);
 		return 1;
 	} else {

 		// Adds the HALT instruction
 		return addInstruction(code, SIO, 0, 0, 3, numInstructions, opr);
 	}
}

int block(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, FILE* opr,
  int curLexLevel, symbol* curProc) {

	// If the token is a constant...
 	if (tokens[*curToken]->token == constsym) {
 		// If constdeclaration encounters an error, end
 		if (constdeclaration(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, opr, curLexLevel) == 1) {
 			return 1;
 		}
 	}
 	int numVars = 0;
 	// If the token is a variable..
 	if (tokens[*curToken]->token == varsym) {
 		// If vardeclaration encounters an error, end
 		if (vardeclaration(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, opr, &numVars, curLexLevel) == 1) {
 			return 1;
 		}
 	}

 	if (tokens[*curToken]->token == procsym) {
 		if (procdeclaration(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, opr, curLexLevel) == 1) {
 			return 1;
 		}
 	}

 	// Tells Program or Procdeclaration where the program/procedure properly starts
 	if (curProc == NULL) {
 		// Tells the program where to Jump on Start
 		code[0].m = *numInstructions;
 	} else {
 		// edits the symbol in the associated Procedure to have the proper address
 		curProc->address = *numInstructions;
 	}

 	// Adds variables just before starting to add the statement code. If in main only increments enough for variables
 	// If in a procedure, increments for variables plus Activation Record.
 	addInstruction(code, INC, 0, 0, numVars, numInstructions, opr);
 	

 	int curReg = 0;

	return statement(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, &curReg, opr, curLexLevel);
}

int constdeclaration(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, FILE* opr, int curLexLevel) {

	char* curIdent;
	char* curNum;
	int added;
	do {

		// If getToken encounters an error, end
		if (getToken(curToken, numTokens, tokens, opr)) {
			return 1;
		} // An error has been encountered, end
		if (tokens[*curToken]->token != identsym) {
			// Prints an error to output file and console if an identifier was expected
			fprintf(stderr, "Parsing Error 0%d at Line (%d): Identifier Expected\n",
			 identifierExpectedError, tokens[*curToken]->lineNum);
			fprintf(opr, "Parsing Error 0%d at Line (%d): Identifier Expected\n",
			 identifierExpectedError, tokens[*curToken]->lineNum);
			return 1;
		}
		curIdent = tokens[*curToken]->value;

		// If getToken encounters an error, end
		if (getToken(curToken, numTokens, tokens, opr)) {
			return 1;
		}
		if (tokens[*curToken]->token != eqsym) {
			// Prints an error to output file and console if "=" was expected
			fprintf(stderr, "Parsing Error 0%d at Line (%d): \"=\" Expected.\n",
			 eqlExpectedError, tokens[*curToken]->lineNum);
			fprintf(opr, "Parsing Error 0%d at Line (%d): \"=\" Expected.\n",
			 eqlExpectedError, tokens[*curToken]->lineNum);
			return 1;
		}

		// If getToken encounters an error, end
		if (getToken(curToken, numTokens, tokens, opr)) {
			return 1;
		} // An error has been encountered, so end
		if (tokens[*curToken]->token != numbersym) {
			// Prints an error to output file and console if a number was expected
			fprintf(stderr, "Parsing Error 0%d at Line (%d): Number Expected\n",
			 numberExpectedError, tokens[*curToken]->lineNum);
			fprintf(opr, "Parsing Error 0%d at Line (%d): Number Expected\n",
			 numberExpectedError, tokens[*curToken]->lineNum);
			return 1;
		}
		curNum = tokens[*curToken]->value;

		// Adds new const to Symbol Table
		added = addSymbol(symbolTable, numSymbols, 1, curIdent, curNum, curLexLevel, -1);
		if (added == identifierAlreadyConstError) {
			// Prints an error to output file and console if the constant already exists
			fprintf(stderr, "Parsing Error 0%d: const \"%s\" Already Exists.\n",
			 constAlreadyExists, curIdent);
			fprintf(opr, "Parsing Error 0%d: const \"%s\" Already Exists.\n",
			 constAlreadyExists, curIdent);
		}

		// If getToken encounters an error, end
		if (getToken(curToken, numTokens, tokens, opr)) {
			return 1;
		}

	// Continue while the token is a comma
	} while (tokens[*curToken]->token == commasym);

	// An error has been encountered, so end
	if (tokens[*curToken]->token != semicolonsym) {
		// Prints an error to output file and console if a semicolon was expected
		fprintf(stderr, "Parsing Error 0%d at Line (%d): Semicolon Expected\n",
			 semicolonExpectedError, tokens[*curToken]->lineNum);
		fprintf(opr, "Parsing Error 0%d at Line (%d): Semicolon Expected\n",
			 semicolonExpectedError, tokens[*curToken]->lineNum);
		return 1;
	}

	// If getToken encounters an error, end
	if (getToken(curToken, numTokens, tokens, opr)) {
		return 1;
	}

	return 0;
}

int vardeclaration(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, FILE* opr,
  int* numVars, int curLexLevel) {

 	char* curIdent;
	int vars;
	int added;

	// If in a procedure, leave room for the activation record. If in main, just add the variable.
	if (curLexLevel > 0) {
		vars = *numVars + 4;
	} else {
		vars = *numVars;
	}

	do {

		// If getToken encounters an error, end
		if (getToken(curToken, numTokens, tokens, opr)) {
			return 1;
		} // An error has been encountered, so end
		if (tokens[*curToken]->token != identsym) {
			// Prints an error to output file and console if an identifier was expected
			fprintf(stderr, "Parsing Error 0%d at Line (%d): Identifier Expected.\n",
			 identifierExpectedError, tokens[*curToken]->lineNum);
			fprintf(opr, "Parsing Error 0%d at Line (%d): Identifier Expected.\n",
			 identifierExpectedError, tokens[*curToken]->lineNum);
			return 1;
		}

		added = addSymbol(symbolTable, numSymbols, 2, tokens[*curToken]->value, "0", curLexLevel, (vars++));
		// An error has been encountered, so end
		if (added == varAlreadyExistsError) {
			// Prints an error to output file and console if the variable already exists
			fprintf(stderr, "Parsing Error 0%d: Variable \"%s\" already exists.\n",
			 varAlreadyExistsError, tokens[*curToken]->value);
			fprintf(opr, "Parsing Error 0%d: Variable \"%s\" already exists.\n",
			 varAlreadyExistsError, tokens[*curToken]->value);
			return 1;
		}

		// If getToken encounters an error, end
		if (getToken(curToken, numTokens, tokens, opr)) {
			return 1;
		}

	// Continue while the token is a comma
	} while (tokens[*curToken]->token == commasym);

	// An error has been encountered, so end
	if (tokens[*curToken]->token != semicolonsym) {
		// Prints an error to output file and console if a semicolon was expected
		fprintf(stderr, "Parsing Error 0%d at Line (%d): Semicolon Expected\n",
			 semicolonExpectedError, tokens[*curToken]->lineNum);
		fprintf(opr, "Parsing Error 0%d at Line (%d): Semicolon Expected\n",
			 semicolonExpectedError, tokens[*curToken]->lineNum);
		return 1;
	}

	
	*numVars = vars;
	// If getToken encounters an error, end
	if (getToken(curToken, numTokens, tokens, opr)) {
		return 1;
	}

	return 0;
}

int procdeclaration(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, FILE* opr, int curLexLevel) {
	
	do {

		if (getToken(curToken, numTokens, tokens, opr)) {
			return 1;
		}

		if (tokens[*curToken]->token != identsym) {
			fprintf(stderr, "Parsing Error 0%d at Line (%d): Identifier Expected.\n",
			 identifierExpectedError, tokens[*curToken]->lineNum);
			fprintf(opr, "Parsing Error 0%d at Line (%d): Identifier Expected.\n",
			 identifierExpectedError, tokens[*curToken]->lineNum);
			return 1;
		}

		// adds new procedure to the symbol table, initialized with address of 0, to be changed in block.
		addSymbol(symbolTable, numSymbols, 3, tokens[*curToken]->value, "0", curLexLevel, 0);

		// grabs the newly added symbol to be passed into block.
		symbol* curProc = findSymbol(*symbolTable, tokens[*curToken]->value, *numSymbols, opr);

		if (getToken(curToken, numTokens, tokens, opr)) {
			return 1;
		}

		// ensures there's a semicolon after the declaration of the symbol.
		if (tokens[*curToken]->token != semicolonsym) {
			fprintf(stderr, "Parsing Error 0%d at Line (%d): Semicolon Expected\n",
			 semicolonExpectedError, tokens[*curToken]->lineNum);
			fprintf(opr, "Parsing Error 0%d at Line (%d): Semicolon Expected\n",
			 semicolonExpectedError, tokens[*curToken]->lineNum);
			return 1;
		}

		if (getToken(curToken, numTokens, tokens, opr)) {
			return 1;
		}

		if (block(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, opr, curLexLevel + 1, curProc) == 1) {
 			return 1;
 		}

 		if (getToken(curToken, numTokens, tokens, opr)) {
			return 1;
		}

		if (tokens[*curToken]->token != semicolonsym) {
			fprintf(stderr, "Parsing Error 0%d at Line (%d): Semicolon Expected\n",
			 semicolonExpectedError, tokens[*curToken]->lineNum);
			fprintf(opr, "Parsing Error 0%d at Line (%d): Semicolon Expected\n",
			 semicolonExpectedError, tokens[*curToken]->lineNum);
			return 1;

		}

		// Loop backwards through symbolTable and set mark = 1 for every symbol found until you reach the current procedure
		deleteSymbols(*symbolTable, *numSymbols, curLexLevel, opr);

	} while (tokens[*curToken]->token == procsym);

	return 0;
}

int statement(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, int* curReg, FILE* opr, int curLexLevel) {

	int tmpInstruction, jmpInstruction;
	symbol *curSym;

	// Used to keep track of registers being used
	*curReg = 0;

 	switch (tokens[*curToken]->token) {
 		case identsym:

 			curSym = findSymbol(*symbolTable, tokens[*curToken]->value, *numSymbols, opr);
 			// An error has been encountered, so end
 			if (curSym == NULL) {
 				return 1;
 			// An error has been encountered, so end
 			} else if (curSym->kind == 1) {
 				// Prints an error to output file and console if a value was trying to be reassigned to a constant
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): Cannot Reassign Value to Constant %s.\n",
			 constReassignError, tokens[*curToken]->lineNum, curSym->name);
 				fprintf(opr, "Parsing Error 0%d at Line (%d): Cannot Reassign Value to Constant %s.\n",
			 constReassignError, tokens[*curToken]->lineNum, curSym->name);
 				return 1;
 			}

 			// If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			}
 			// An error has been encountered, so end
 			if (tokens[*curToken]->token != becomessym) {
 				// Prints an error to output file and console if ":=" was expected
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): \":=\" Expected\n",
			 becomesExpectedError, tokens[*curToken]->lineNum);
 				fprintf(opr, "Parsing Error 0%d at Line (%d): \":=\" Expected\n",
			 becomesExpectedError, tokens[*curToken]->lineNum);
 				return 1;
 			}
 			// If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			} // If expression encounters an error, end
 			if (expression(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 				return 1;
 			}

 			// Adds new instruction
 			addInstruction(code, STO, *curReg, 0, curSym->address, numInstructions, opr);
 			break;

 		case callsym:

 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			}

 			if (tokens[*curToken]->token != identsym) {
				fprintf(stderr, "Parsing Error 0%d at Line (%d): Identifier Expected.\n",
			 	 identifierExpectedError, tokens[*curToken]->lineNum);
				fprintf(opr, "Parsing Error 0%d at Line (%d): Identifier Expected.\n",
			 	 identifierExpectedError, tokens[*curToken]->lineNum);
				return 1;
			}

			symbol* curProc = findSymbol(*symbolTable, tokens[*curToken]->value, *numSymbols, opr);
			if (curProc->kind == 1) {
				fprintf(stderr, "Parsing Error 0%d at Line (%d): Procedure Expected, Constant Received.\n",
			 	 procedureExpectedConstError, tokens[*curToken]->lineNum);
				fprintf(opr, "Parsing Error 0%d at Line (%d): Procedure Expected, Constant Received.\n",
			 	 procedureExpectedConstError, tokens[*curToken]->lineNum);

				return 1;
			} else if (curProc->kind == 2) {
				fprintf(stderr, "Parsing Error 0%d at Line (%d): Procedure Expected, Variable Received.\n",
			 	 procedureExpectedVarError, tokens[*curToken]->lineNum);
				fprintf(opr, "Parsing Error 0%d at Line (%d): Procedure Expected, Variable Received.\n",
			 	 procedureExpectedVarError, tokens[*curToken]->lineNum);

				return 1;
			}

			addInstruction(code, CAL, 0, (curLexLevel - curProc->level), curProc->address, numInstructions, opr);

			// If getToken encounters an error, end
			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			}

 			break;

 		case beginsym:
 			
 			do {
 				// If getToken encounters an error, end
 				if (getToken(curToken, numTokens, tokens, opr)) {
 					return 1;
 				}

 				// If statement encounters an error, end
 				if (statement(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 					return 1;
 				}
 			
 			} // Continue while token is a semicolon 
 			while (tokens[*curToken]->token == semicolonsym);
 			
 			// An error has been encountered, so end
 			if (tokens[*curToken]->token != endsym) {
 				// Prints an error to output file and console if "end" was expected
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): \"end\" Expected.\n",
			 endExpectedError, tokens[*curToken]->lineNum);
 				fprintf(opr, "Parsing Error 0%d at Line (%d): \"end\" Expected.\n",
			 endExpectedError, tokens[*curToken]->lineNum);
 				return 1;
 			}

 			// If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			}
 			break;

 		case ifsym:
 			// If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			}
 			 // If condition encounters an error, end
 			if (condition(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 				return 1;
 			}
 			 // An error has been encountered, so end
 			if (tokens[*curToken]->token != thensym) {
 				// Prints an error to output file and console if "then" was expected
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): \"then\" Expected\n",
			 thenExpectedError, tokens[*curToken]->lineNum);
 				fprintf(opr, "Parsing Error 0%d at Line (%d): \"then\" Expected\n",
			 thenExpectedError, tokens[*curToken]->lineNum);
 				return 1;
 			} // If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			} 
 			tmpInstruction = *numInstructions;
 			// Adds a new instruction
 			addInstruction(code, JPC, 0, 0, 0, numInstructions, opr);

 			// If statement encounters an error, end
 			if (statement(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 				return 1;
 			}

 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			}

 			if (tokens[*curToken]->token == elsesym) {

 				// if else block present, add a jump instruction to the end of if statement
 				// this allows the program to skip the else block if the condition is true
 				jmpInstruction = *numInstructions;
 				addInstruction(code, JMP, 0, 0, 0, numInstructions, opr);

 				code[tmpInstruction].m = (*numInstructions);

 				if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 				}

 				// If statement encounters an error, end
 				if (statement(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 					return 1;
 				}

 				code[jmpInstruction].m = (*numInstructions);

 			} else {

 				code[tmpInstruction].m = (*numInstructions);
 			}

 			
 			break;

 		case whilesym:
 			// If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			}
 			jmpInstruction = *numInstructions;
 			// If condition encounters an error, end
 			if (condition(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 				return 1;
 			} // An error has been encountered, so end
 			if (tokens[*curToken]->token != dosym) {
 				// Prints an error to output file and console if "do" was expected
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): \"do\" Expected\n",
			 doExpectedError, tokens[*curToken]->lineNum);
 				fprintf(opr, "Parsing Error 0%d at Line (%d): \"do\" Expected\n",
			 doExpectedError, tokens[*curToken]->lineNum);
 				return 1;
 			}

 			// If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			}
 			tmpInstruction = *numInstructions;
 			// Adds new instruction
 			addInstruction(code, JPC, 0, 0, 0, numInstructions, opr);

 			// If statement encounters an error, end
 			if (statement(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 				return 1;
 			}

 			// Adds new instruction
 			addInstruction(code, JMP, 0, 0, jmpInstruction, numInstructions, opr);
 			code[tmpInstruction].m = (*numInstructions);
 			break;

 		case readsym:

 			// If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			} 

 			// An error has been encountered, so end
 			if (tokens[*curToken]->token != identsym) {
 				// Prints an error to output file and console if an identifier was expected
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): Identifier Expected\n",
			 	 identifierExpectedError, tokens[*curToken]->lineNum);
 				fprintf(opr, "Parsing Error 0%d at Line (%d): Identifier Expected\n",
			 	 identifierExpectedError, tokens[*curToken]->lineNum);
				return 1;
 			}

 			curSym = findSymbol(*symbolTable, tokens[*curToken]->value, *numSymbols, opr);
 			// An error has been encountered, so end
 			if (curSym == NULL) {
 				return 1;
 			} // An error has been encountered, so end
 			else if (curSym->kind == 1) {
 				// Prints an error to output file and console if a value was trying to be reassigned to a constant
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): Cannot Reassign Value to Constant %s.\n",
			 constReassignError, tokens[*curToken]->lineNum, curSym->name);
 				fprintf(opr, "Parsing Error 0%d at Line (%d): Cannot Reassign Value to Constant %s.\n",
			 constReassignError, tokens[*curToken]->lineNum, curSym->name);
 				return 1;
 			} else if (curSym->kind == 3) {
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): Cannot Reassign Value to Procedure %s.\n",
			 procReassignError, tokens[*curToken]->lineNum, curSym->name);
 				fprintf(opr, "Parsing Error 0%d at Line (%d): Cannot Reassign Value to Procedure %s.\n",
			 procReassignError, tokens[*curToken]->lineNum, curSym->name);
 			}

 			// Adds new instruction
 			addInstruction(code, SIO, 0, 0, 2, numInstructions, opr);
 			addInstruction(code, STO, 0, (curLexLevel - curSym->level), curSym->address, numInstructions, opr);

 			// If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			}
 			break;

 		case writesym:

 			// If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			} // An error has been encountered, so end
 			
 			// If expression encounters an error, end
 			if (expression(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 				return 1;
 			}

 			addInstruction(code, SIO, *curReg, 0, 1, numInstructions, opr);

 			// If getToken encounters an error, end
 			if (getToken(curToken, numTokens, tokens, opr)) {
 				return 1;
 			}

 			break;

 		default:
 			// Indicates no errors were found
 			return 0;
 			break;
 	}

	return 0;
}

int condition(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, int* curReg, FILE* opr, int curLexLevel) {
 	int tmpReg = *curReg;
 	// If the token is "odd"
 	if (tokens[*curToken]->token == oddsym) {
 		// If getToken encounters an error, end
 		if (getToken(curToken, numTokens, tokens, opr)) {
 			return 1;
 		}

 		// Runs expression function using current token
 		expression(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel);

 		// Adds new instruction
 		addInstruction(code, ODD, tmpReg, 0, 0, numInstructions, opr);
 	} else {
 		// Runs expression function using current token
 		expression(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel);

 		op_code op;
 		// Switch case for op-code of token
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
 				// Prints an error to output file and console if a relational operator was expected
  				fprintf(stderr, "Parsing Error 0%d at Line (%d): Relational Operator Expected\n",
			 		relopExpectedError, tokens[*curToken]->lineNum);
  				fprintf(opr, "Parsing Error 0%d at Line (%d): Relational Operator Expected\n",
			 		relopExpectedError, tokens[*curToken]->lineNum);
 				return 1;
 		}

 		// If getToken encounters an error, end
 		if (getToken(curToken, numTokens, tokens, opr)) {
 			return 1;
 		}
 		*curReg = *curReg + 1;

 		// If expression encounters an error, end
 		if (expression(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 			return 1;
 		}

 		// Adds new instruction
 		addInstruction(code, op, tmpReg, tmpReg, *curReg, numInstructions, opr);

 		// Shifts current register
 		*curReg = *curReg - 1;
 	}

	return 0;
}

int expression(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, int* curReg, FILE* opr, int curLexLevel) {

	// Keeps track of register used at beginning of expresion
	int tmpReg = *curReg;

	// Checks to see if first value is negative
 	token_type addOp = 0;
 	// If the token is +...
 	if (tokens[*curToken]->token == plussym) {
 		addOp = plussym;
 		// If getToken encounters an error, end
 		if (getToken(curToken, numTokens, tokens, opr)) {
 			return 1;
 		}
 	} // If the token is -...
	else if (tokens[*curToken]->token == minussym) {
 		addOp = minussym;
 		// If getToken encounters an error, end
 		if (getToken(curToken, numTokens, tokens, opr)) {
 			return 1;
 		}
 	}

 	// If term encounters an error, end
 	if (term(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 		return 1;
 	}

 	if (addOp == minussym) {
 		// Adds new instruction
 		addInstruction(code, NEG, tmpReg, 0, 0, numInstructions, opr);
 	}

 	// Continue while the token is + or -
 	while (tokens[*curToken]->token == plussym || tokens[*curToken]->token == minussym) {
 		addOp = tokens[*curToken]->token;
 		// If getToken encounters an error, end
 		if (getToken(curToken, numTokens, tokens, opr)) {
 			return 1;
 		}

 		*curReg = *curReg + 1;
 		// If term encounters an error, end
 		if (term(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 			return 1;
 		}

 		if (addOp == plussym) {
 			// Adds new instruction
 			addInstruction(code, ADD, tmpReg, tmpReg, *curReg, numInstructions, opr);
 		} else if (addOp == minussym) {
 			// Adds new instruction
 			addInstruction(code, SUB, tmpReg, tmpReg, *curReg, numInstructions, opr);
 		}
 		// Shifts current register
 		*curReg = *curReg - 1;
 	}

	return 0;
}

int term(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, int* curReg, FILE* opr, int curLexLevel) {

	// Keeps track of register used at beginning of term
	int tmpReg = *curReg;
	// If factor encounters an error, end
	if (factor(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 		return 1;
 	}

 	token_type mulOp = 0;

 	// Continue while token is * or /
 	while (tokens[*curToken]->token == multsym || tokens[*curToken]->token == slashsym) {
 		mulOp = tokens[*curToken]->token;
 		// If getToken encounters an error, end
 		if (getToken(curToken, numTokens, tokens, opr)) {
 			return 1;
 		}

 		// Shifts current register
 		*curReg = *curReg + 1;
 		// If factor encounters an error, end
 		if (factor(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 			return 1;
 		}

 		if (mulOp == multsym) {
 			// Adds new instruction
 			addInstruction(code, MUL, tmpReg, tmpReg, *curReg, numInstructions, opr);
 		} else if (mulOp == slashsym) {
 			// Adds new instruction
 			addInstruction(code, DIV, tmpReg, tmpReg, *curReg, numInstructions, opr);
 		}
 		// Shfits current register
 		*curReg = *curReg - 1;
 	}

	return 0;
}

int factor(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, int* curReg, FILE* opr, int curLexLevel) {

 	symbol* curSym;

 	// Switch case of token type
	switch (tokens[*curToken]->token) {
		// If token is an identifier...
		case identsym:

			curSym = findSymbol(*symbolTable, tokens[*curToken]->value, *numSymbols, opr);
			// An error has been encountered, end
			if (curSym == NULL) {
				return 1;
			}
			if (curSym->kind == 1) {
				// Adds new instruction
				addInstruction(code, LIT, *curReg, 0, curSym->val, numInstructions, opr);
			} else if (curSym->kind == 2){
				// Adds new instruction
				addInstruction(code, LOD, *curReg, (curLexLevel - curSym->level), curSym->address, numInstructions, opr);
			} else {
				fprintf(stderr, "Parsing Error 0%d at Line (%d): Cannot Use A Procedure Within An Expression.\n",
			 	 procInExpressionError, tokens[*curToken]->lineNum);
 				fprintf(opr, "Parsing Error 0%d at Line (%d): Cannot Use A Procedure Within An Expression.\n",
			 	 procInExpressionError, tokens[*curToken]->lineNum);
				return 1;
			}
			break;
		// If token is a number...
		case numbersym:
			// Adds new instruction
			addInstruction(code, LIT, *curReg, 0, atoi(tokens[*curToken]->value), numInstructions, opr);
			break;
		// If token is (...
		case lparentsym:

			// If getToken encounters an error, end
			if (getToken(curToken, numTokens, tokens, opr)) {
				return 1;
			} // If expression encounters an error, end
			if (expression(code, symbolTable, numSymbols, tokens, numTokens, numInstructions, curToken, curReg, opr, curLexLevel) == 1) {
 				return 1;
 			}

 			// An error has been encountered, end
 			if (tokens[*curToken]->token != rparentsym) {
 				// Prints an error to output file and console if closing parentheses were expected
 				fprintf(stderr, "Parsing Error 0%d at Line (%d): Closing Parentheses Expected.\n",
			 	 invalidExpressionError, tokens[*curToken]->lineNum);
 				fprintf(opr, "Parsing Error 0%d at Line (%d): Closing Parentheses Expected.\n",
			 	 invalidExpressionError, tokens[*curToken]->lineNum);
 				return 1;
 			}
			break;
		default:
			// Prints an error to output file and console if the symbol is invalid
			fprintf(stderr, "Parsing Error 0%d at Line (%d): \"%s\" Is Not A Valid Symbol In An Expression.\n",
			 	invalidExpressionError, tokens[*curToken]->lineNum, tokens[*curToken]->value);
			fprintf(opr, "Parsing Error 0%d at Line (%d): \"%s\" Is Not A Valid Symbol In An Expression.\n",
			 	invalidExpressionError, tokens[*curToken]->lineNum, tokens[*curToken]->value);
 			return 1;
 			break;
	} 

	// If getToken encounters an error, end
	if (getToken(curToken, numTokens, tokens, opr)) {
		return 1;
	}

	return 0;
}

// Method used to add symbols to the symbol table
int addSymbol(symbol*** symbolTable, int* numSymbols, int kind, char* name, char* val, int level, int address) {
	
	for (int i = *numSymbols - 1; i>= 0; i--) {
		if ((strcmp(symbolTable[0][i]->name, name) == 0)) {
			if (symbolTable[0][i]->kind == 1) {
				return identifierAlreadyConstError;
			} else if (symbolTable[0][i]->level == level) {
				return varAlreadyExistsError;
			}
		}
	}
	*numSymbols = *numSymbols + 1;
	*symbolTable = realloc(*symbolTable, (*numSymbols) * sizeof(symbol*));
	symbolTable[0][*numSymbols - 1] = malloc(sizeof(symbol));
	symbolTable[0][*numSymbols - 1]->kind = kind;
	symbolTable[0][*numSymbols - 1]->name = malloc(strlen(name) * sizeof(char));
	strcpy(symbolTable[0][*numSymbols - 1]->name, name);
	symbolTable[0][*numSymbols - 1]->val = atoi(val);
	symbolTable[0][*numSymbols - 1]->level = level;
	symbolTable[0][*numSymbols - 1]->address = address;
	symbolTable[0][*numSymbols - 1]->mark = 0;

	return 0;
}

// Method used for symbol lookup
symbol* findSymbol(symbol** symbolTable, char* name, int numSymbols, FILE* opr) {
	for (int i = numSymbols - 1; i >= 0; i--) {
		if ((strcmp(symbolTable[i]->name, name) == 0) && symbolTable[i]->mark == 0) {
			return symbolTable[i];
		}
	}
	// Prints an error to output file and console if the symbol being searched for does not exist
	fprintf(stderr, "Parsing Error 0%d: Identifier \"%s\" Does Not Exist.\n", identifierDoesntExistError, name);
	fprintf(opr, "Parsing Error 0%d: Identifier \"%s\" Does Not Exist.\n", identifierDoesntExistError, name);

	return NULL;
}

void deleteSymbols(symbol** symbolTable, int numSymbols, int curLexLevel, FILE* opr) {

	for (int i = numSymbols - 1; i >= 0; i--) {
		if (symbolTable[i]->level > curLexLevel) {
			symbolTable[i]->mark = 1;
		} else {
			break;
		}
	}
}

// Method used to add new instruction to the given code array
int addInstruction(instruction* code, op_code op, int r, int l, int m, int* numInstructions, FILE* opr) {
	*numInstructions = *numInstructions + 1;
	// Prints an error to output file and console if the number of instructions is longer than the allowable limit
	if (*numInstructions > MAX_CODE_LENGTH) {
		fprintf(stderr, "Error: Generated Assembly Code Too Long.\n");
		fprintf(opr, "Error: Generated Assembly Code Too Long.\n");
		return 1;
	}
	// Fills code with approrpaite op, r, l, m
	code[*numInstructions - 1].op = op;
	code[*numInstructions - 1].r = r;
	code[*numInstructions - 1].l = l;
	code[*numInstructions - 1].m = m;

	return 0;
}

// Method used to get the next token for use by the parser
int getToken(int *curToken, int numTokens, lexeme **tokens, FILE* opr) {
	*curToken = *curToken + 1;
	int failed = *curToken >= numTokens;
	// An error has been encountered
	if (failed) {
		// Prints an error to output file and console if the program ends abruptly
		fprintf(stderr, "Parsing Error 0%d at Line(%d): Incomplete Program Ends Abruptly.\n",
		 incompleteProgramError, tokens[*curToken - 1]->lineNum);
		fprintf(opr, "Parsing Error 0%d at Line(%d): Incomplete Program Ends Abruptly.\n",
		 incompleteProgramError, tokens[*curToken - 1]->lineNum);
	}

	return failed;
}