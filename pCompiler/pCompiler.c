// Owen Brahms - 3739927
// Michaela Green - 4198343
// COP 3402, Spring 2020
// Project 3

#include <stdlib.h>
#include <string.h>
#include "pCompiler.h"

int main(int argc, char* argv[]) {
	// Input File should be last argument in command
	char* inFile = argv[argc-1];
	int printLex = 0, printParse = 0, printMachine = 0;

	for (int i = 1; i < argc - 1; i++) {
		if (strcmp("-l", argv[i]) == 0) { // if the -l command is in the command line, print the Lexer Output
			printLex = 1;
		} else if (strcmp("-a", argv[i]) == 0) { // if the -a command is in the command line, print the Parser Output
			printParse = 1;
		} else if (strcmp("-v", argv[i]) == 0) { // if the -v command is in the command line, print the Virtual Machine Output
			printMachine = 1;
		} else {
			fprintf(stderr, "Unrecognized command: %s\n",argv[i]);
			return 0;
		}
	}

	FILE* ipr = fopen(inFile, "r");
	if(ipr == NULL) {
		fprintf(stderr, "Could not find valid file by name: %s \n", inFile);
		return 0;
	}
	// Will always print all output to output.txt regardless of user command
	FILE* opr = fopen("output.txt", "w");
	if(ipr == NULL) {
		fprintf(stderr, "Could not find valid file by name: %s \n", inFile);
		return 0;
	}

	int numTokens = 0;
	lexeme** tokens = lex(ipr, opr, printLex, &numTokens);
	if (tokens == NULL) {
		fprintf(stderr, "Error in Lexicographical Analyzer. Program Failed to Compile\n");
		return 0;
	}

	int numInstructions = 0;
	instruction* code = parse(tokens, numTokens, opr, printParse, &numInstructions);
	if (code == NULL) {
		fprintf(stderr, "Error in Parser. Program Failed to Compile\n");
		return 0;
	}
	
	return 1;
}
