// Owen Brahms - 3739927
// Michaela Green - 4198343
// COP 3402, Spring 2020
// Project 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "lexer.h"

int *tokens, numTokens;
char **lexemes;

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
	char *buffer = calloc((MAX_VAR_LENGTH) + 1, sizeof(char));
	
	// Used to to store individual chars as a string for
	// pattern matching purposes
	char *charAsString = calloc(2, sizeof(char));

	enum token_type state = nulsym;

	fprintf(opr, "\n");

	int ungot = 0;
	int varLength = 0;
	numTokens = 0;
	do {

		tmpC = fgetc(ipr);
		if (EOF == tmpC)
			break;

		ungot = 0;
		switch (state) {
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
						buffer[varLength++] = '/';
						break;
					case '=':
						addLexeme("=", eqsym);
						break;
					case '<':
						state = lessym;
						buffer[varLength++] = '<';
						break;
					case '>':
						state = gtrsym;
						buffer[varLength++] = '>';
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
						buffer[varLength++] = ':';
						break;
					case ' ':
					case '\t':
					case '\n':
						state = nulsym;
						break;

						// If the character is not a recognized special character
						// Checks if it is a letter or a digit
						// If neither, print remaining code, throws an error, and halts the program
					default:
						charAsString[0] = tmpC;
						if (regexec(&letter, charAsString, 0, NULL, 0) == 0) {
							state = identsym;
							buffer[varLength++] = tmpC;
						} else if (regexec(&digit, charAsString, 0, NULL, 0) == 0) {
							state = numbersym;
							buffer[varLength++] = tmpC;
						} else {
							printRest(ipr, opr);
							fprintf(opr, "Error: Unrecognized Token - \"%s\"\n", charAsString);
							return 0;
						}
						break;

				}
				break;

			case identsym :
				if (varLength >= 11) {
					printRest(ipr, opr);
					fprintf(opr, "Error: Cannot have Identifier Longer than 11 characters - \"%s\"\n", buffer);
					return 0;
				}

				charAsString[0] = tmpC;
				if ((regexec(&letter, charAsString, 0, NULL, 0) == 0) ||
				 regexec(&digit, charAsString, 0, NULL, 0) == 0) {
					buffer[varLength++] = tmpC;
				} else {
					ungetc(tmpC, ipr);
					ungot = 1;
					state = nulsym;
					if (strcmp(buffer,  "odd") == 0) {
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
					memset(buffer, 0, strlen(buffer));
					varLength = 0;

				}
				break;

		}

		if (ungot == 0) {
			fprintf(opr, "%c", tmpC);
		}

	} while(1);
	fprintf(opr, "\n\n\n");
	fprintf(opr, "Lexeme Table:\nlexeme           token type\n");

	char buf[17];

	// Prints out Lexeme Table. Lexeme followed by spaces followed by the associate token
	for(int i = 0; i < numTokens; i++) {
		makeBuf(buf, lexemes[i], 17);
		fprintf(opr, "%s%s%d\n", lexemes[i], buf, tokens[i]);
	}
	fprintf(opr, "\n\nLexeme List:\n");

	//Prints out Lexeme List. Tokens, with Lexeme following if identifier or number
	for (int i = 0; i < numTokens; i++) {
		if (tokens[i] == 2 || tokens[i] == 3) {
			fprintf(opr, "%d|%s|", tokens[i], lexemes[i]);
		} else {
			fprintf(opr, "%d|", tokens[i]);
		}
	}
	fprintf(opr, "\n");


	fclose(ipr);
	if (strcmp("display",argv[1]) != 0)
		fclose(opr);

	return 0;
}

// Method used to add to Token and Lexeme Lists
void addLexeme (char* lex, int token) {
	// Increases number of tokens, increases the size of the tokens array and
	// lexemes array, adds the new token and new lexeme.
	numTokens++;
	tokens = realloc(tokens, numTokens * sizeof(int));
	tokens[numTokens-1] = token;
	lexemes = realloc(lexemes, numTokens * sizeof(char*));
	lexemes[numTokens-1] = malloc((strlen(lex)) * sizeof(char));
	strcpy(lexemes[numTokens-1], lex);
}

// Method used to print remainder of input file in the event of an error
void printRest(FILE *input, FILE *output) {

}

void makeBuf(char buffer[], char* str, int bufLen) {
	if (bufLen >= strlen(str)) {
		for (int i = 0; i < (bufLen - strlen(str)); i++) {
			buffer[i] = ' ';
		}
		buffer[bufLen-strlen(str)] = '\0';
	}
}