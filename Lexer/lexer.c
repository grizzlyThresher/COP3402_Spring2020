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
						state = plussym;
						buffer[varLength++] = '+';
						break;
					case '-':
						state = minussym;
						buffer[varLength++] = '-';
						break;
					case '*':
						state = multsym;
						buffer[varLength++] = '*';
						break;
					case '/':
						state = slashsym;
						buffer[varLength++] = '/';
						break;
					case '=':
						state = eqsym;
						buffer[varLength++] = '=';
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
						state = lparentsym;
						buffer[varLength++] = '(';
						break;
					case ')':
						state = rparentsym;
						buffer[varLength++] = ')';
						break;
					case ',':
						state = commasym;
						buffer[varLength++] = ',';
						break;
					case ';':
						state = semicolonsym;
						buffer[varLength++] = ';';
						break;
					case '.':
						state = periodsym;
						buffer[varLength++] = '.';
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
//							state = identsym;
							buffer[varLength++] = tmpC;
							printf("LETTER\n");
						} else if (regexec(&digit, charAsString, 0, NULL, 0) == 0) {
							state = numbersym;
							buffer[varLength++] = tmpC;
							printf("NUMBER\n");
						} else {
							printRest(ipr, opr);
							fprintf(opr, "Error: Unrecognized Token - \"%s\"\n", charAsString);
							return 0;
						}
						break;

				}
				break;

		}

		if (ungot == 0) {
			fprintf(opr, "%c", tmpC);
			printf("%c\n", tmpC);
		}

	} while(1);
	fprintf(opr, "\n");

	fclose(ipr);
	if (strcmp("display",argv[1]) != 0)
		fclose(opr);

	return 0;
}

// Method used to add to Token and Lexeme Lists
void addLexeme (char* lex, int token) {
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