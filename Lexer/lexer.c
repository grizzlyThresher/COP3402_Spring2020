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
char *letter ="[a-zA-Z]";
char *digit = "[0-9]";

int main(int argc, char *argv[]) {

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

	char tmpC;
	char *buffer = calloc((MAX_VAR_LENGTH) + 1, sizeof(char));
	enum token_type state = nulsym;

	fprintf(opr, "\n");

	int ungot = 0;
	numTokens = 0;
	do {

		tmpC = fgetc(ipr);
		if (EOF == tmpC)
			break;

		ungot = 0;
		switch (state) {
			case nulsym :

			break;

		}

		if (ungot == 0) {
			fprintf(opr, "%c", tmpC);
		}

	} while(1);
	fprintf(opr, "\n");

	return 0;
}

void addLexeme (char* lex, int token) {
	numTokens++;
	tokens = realloc(tokens, numTokens * sizeof(int));
	tokens[numTokens-1] = token;
	lexemes = realloc(lexemes, numTokens * sizeof(char*));
	lexemes[numTokens-1] = malloc((strlen(lex)) * sizeof(char));
	strcpy(lexemes[numTokens-1], lex);
}