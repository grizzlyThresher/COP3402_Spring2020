// Owen Brahms - 3739927
// Michaela Green - 4198343
// COP 3402, Spring 2020
// Project 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "lexer.h"

int numTokens, numErrors, numLines;
lexeme **lexemes;
error **errorList;

int main(int argc, char *argv[]) {

	// Used for Input and Output files
    FILE* ipr;
    // FILE must be static because otherwise it can't be assigned to stdout
    static FILE* opr;

    // Regular Expressions used to match Letters and/or Digits
    regex_t letter, digit;
    int result;
    result = regcomp(&letter, "[A-Za-z]", 0);
    if (result != 0) {
    	printf("Could not compile regular expression");
    	return 0;
    }
    result = regcomp(&digit, "[0-9]", 0);
    if (result != 0) {
    	printf("Could not compile regular expression");
    	return 0;
    }


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

	char tmpC;
	
	// Used to to store individual chars as a string for
	// pattern matching purposes
	char *charAsString = calloc(2, sizeof(char));
	

	token_type state = nulsym;

	fprintf(opr, "Source Program:\n");

	int ungot = 0;
	int varLength = 0;
	int almostEnding = 0;
	numTokens = 0;
	numLines = 1;
	numErrors = 0;
	char *buffer = malloc(varLength * sizeof(char));
	do {

		tmpC = fgetc(ipr);
		if (EOF == tmpC)
			break;

		ungot = 0;
		switch (state) {
			case comment :
				if (tmpC == '*') {
					almostEnding = 1;
					state = comment;
				} else if ((tmpC == '/') && (almostEnding == 1)) {
					state = nulsym;
				} else {
					almostEnding = 0;
					state = comment;
				}
				break;

			case nulsym : // Handles all transitions from the start state.
				switch (tmpC) {
					case '+':
						addLexeme("+", plussym);
						break;
					case '-':
						addLexeme("-", minussym);
						break;
					case '*':
						addLexeme("*", multsym);
						break;
					case '/':
						state = slashsym;
						break;
					case '=':
						addLexeme("=", eqsym);
						break;
					case '<':
						state = lessym;
						break;
					case '>':
						state = gtrsym;
						break;
					case '(':
						addLexeme("(", lparentsym);
						break;
					case ')':
						addLexeme(")", rparentsym);
						break;
					case ',':
						addLexeme(",", commasym);
						break;
					case ';':
						addLexeme(";", semicolonsym);
						break;
					case '.':
						addLexeme(".", periodsym);
						break;
					case ':':
						state = becomessym;
						break;
					case '\n':
						numLines++;
					case ' ':
					case '\t':
						state = nulsym;
						break;

						// If the character is not a recognized special character
						// Checks if it is a letter or a digit
						// If neither, print remaining code, throws an error, and halts the program
					default:
						charAsString[0] = tmpC;
						if (regexec(&letter, charAsString, 0, NULL, 0) == 0) {
							state = identsym;
							varLength++;
							buffer = realloc(buffer, varLength * sizeof(char));
							buffer[varLength - 1] = tmpC;
						} else if (regexec(&digit, charAsString, 0, NULL, 0) == 0) {
							state = numbersym;
							varLength++;
							buffer = realloc(buffer, varLength * sizeof(char));
							buffer[varLength - 1] = tmpC;
						} else {
							addError(charAsString, invalidSymbolError, numLines);
							state = nulsym;
						}
						break;

				}
				break;

			case identsym : // Handles all transitions for potential identifiers and reserved words.
				charAsString[0] = tmpC;
				if ((regexec(&letter, charAsString, 0, NULL, 0) == 0) ||
				 regexec(&digit, charAsString, 0, NULL, 0) == 0) {
					varLength++;
					buffer = realloc(buffer, varLength * sizeof(char));
					buffer[varLength - 1] = tmpC;
				} else {
					ungetc(tmpC, ipr);
					ungot = 1;
					state = nulsym;
					buffer = realloc(buffer, (varLength + 1) * sizeof(char));
					buffer[varLength] = '\0';
					if (varLength > MAX_VAR_LENGTH){
						addError(buffer, varLengthError, numLines);
					} else if (strcmp(buffer,  "odd") == 0) {
						addLexeme(buffer, oddsym);
					} else if (strcmp(buffer, "begin") == 0) {
						addLexeme(buffer, beginsym);
					} else if (strcmp(buffer, "end") == 0) {
						addLexeme(buffer, endsym);
					} else if (strcmp(buffer, "if") == 0) {
						addLexeme(buffer, ifsym);
					} else if (strcmp(buffer, "then") == 0) {
						addLexeme(buffer, thensym);
					} else if (strcmp(buffer, "while") == 0) {
						addLexeme(buffer, whilesym);
					} else if (strcmp(buffer, "do") == 0) {
						addLexeme(buffer, dosym);
					} else if (strcmp(buffer, "call") == 0) {
						addLexeme(buffer, callsym);
					} else if (strcmp(buffer, "const") == 0) {
						addLexeme(buffer, constsym);
					} else if (strcmp(buffer, "var") == 0) {
						addLexeme(buffer, varsym);
					} else if (strcmp(buffer, "procedure") == 0) {
						addLexeme(buffer, procsym);
					} else if (strcmp(buffer, "write") == 0) {
						addLexeme(buffer, writesym);
					} else if (strcmp(buffer, "read") == 0) {
						addLexeme(buffer, readsym);
					} else if (strcmp(buffer, "else") == 0) {
						addLexeme(buffer, elsesym);
					} else {
						addLexeme(buffer, identsym);
					}

					// Resets buffer back to empty in preparation for next pass through the lexer.
					varLength = 0;
					buffer = realloc(buffer, varLength * sizeof(char));

				}
            break;

            case slashsym :
            	if (tmpC == '*')
            	{
       				almostEnding = 0;
            		state = comment;
            	} else {
            		addLexeme("/", slashsym);
            		ungetc(tmpC, ipr);
            		ungot = 1;
            		state = nulsym;
            	}
            	break;

			case lessym : 
				if (tmpC == '=') {
					addLexeme("<=", leqsym);
				} else if (tmpC == '>') {
					addLexeme("<>", neqsym);
				} else {
					addLexeme("<", lessym);
					ungetc(tmpC, ipr);
					ungot = 1;
				}

				state = nulsym;
				break;

			case gtrsym : 
				if (tmpC == '=') {
					addLexeme(">=", geqsym);
				} else {
					addLexeme(">", gtrsym);
					ungetc(tmpC, ipr);
					ungot = 1;
				}

				state = nulsym;
				break;

			case becomessym :
				if (tmpC == '=') {
					addLexeme(":=", becomessym);
				} else {
					addError(":", invalidSymbolError, numLines);
					ungetc(tmpC, ipr);
					ungot = 1;
				}

				state = nulsym;
				break;
			}
		if (ungot == 0) {
			fprintf(opr, "%c", tmpC);
		}
	} while(1);

	if (state == comment)
		addError("", openCommentError, numLines);

	fprintf(opr, "\n\n\n");
	if (numErrors >= 1) {
		for (int i = 0; i < numErrors; i++) {
			fprintf(opr, "Error at Line (%d) : ", errorList[i]->lineNum);
			switch (errorList[i]->type) {
				case numLengthError :
				fprintf(opr, "Number Longer Than %d Digits - %s\n", MAX_NUM_LENGTH, errorList[i]->value);
				break;
				case varLengthError :
				fprintf(opr, "Variable Longer Than %d Characters - \"%s\"\n", MAX_VAR_LENGTH, errorList[i]->value);
				break;
				case invalidSymbolError :
				fprintf(opr, "Unidentified Symbol - \"%s\"\n",errorList[i]->value);
				break;
				case openCommentError :
				fprintf(opr, "Open Comment Never Closed\n");
				break;
			}
		}
	} else {
		fprintf(opr, "Lexeme Table:\nlexeme           token type\n");

		char buf[17];

		// Prints out Lexeme Table. Lexeme followed by spaces followed by the associate token
		for(int i = 0; i < numTokens; i++) {
			makeBuf(buf, lexemes[i]->value, 17);
			fprintf(opr, "%s%s%d\n", lexemes[i]->value, buf, lexemes[i]->token);
		}
		fprintf(opr, "\n\nLexeme List:\n");

		//Prints out Lexeme List. Tokens, with Lexeme following if identifier or number
		for (int i = 0; i < numTokens; i++) {
			if (lexemes[i]->token == identsym || lexemes[i]->token == numbersym) {
				fprintf(opr, "%d|%s|", lexemes[i]->token, lexemes[i]->value);
			} else {
				fprintf(opr, "%d|", lexemes[i]->token);
			}
		}
		fprintf(opr, "\n");
	}

	fclose(ipr);
	if (strcmp("display",argv[1]) != 0)
		fclose(opr);

	return 0;
}

// Method used to add to Token and Lexeme Lists
void addLexeme (char* lex, token_type token) {
	// Increases number of tokens, increases the size of the tokens array and
	// lexemes array, adds the new token and new lexeme.
	numTokens++;
	lexemes = realloc(lexemes, numTokens * sizeof(lexeme*));
	lexemes[numTokens-1] = malloc(sizeof(lexeme));
	lexemes[numTokens-1]->token = token;
	lexemes[numTokens-1]->value = malloc((strlen(lex)) * sizeof(char));
	strcpy(lexemes[numTokens-1]->value, lex);
}

void makeBuf(char buffer[], char* str, int bufLen) {
	if (bufLen >= strlen(str)) {
		for (int i = 0; i < (bufLen - strlen(str)); i++) {
			buffer[i] = ' ';
		}
		buffer[bufLen-strlen(str)] = '\0';
	}
}

void addError(char* str, error_type type, int lineNum) {
	numErrors++;
	errorList = realloc(errorList, numErrors * sizeof(error*));
	errorList[numErrors - 1] = malloc(sizeof(error));
	errorList[numErrors - 1]->type = type;
	errorList[numErrors - 1]->lineNum = lineNum;
	errorList[numErrors - 1]->value = malloc(strlen(str) * sizeof(char));
	strcpy(errorList[numErrors - 1]->value, str);
}