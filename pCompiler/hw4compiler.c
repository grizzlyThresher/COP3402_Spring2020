// Owen Brahms - 3739927
// Michaela Green - 4198343
// COP 3402, Spring 2020
// Project 3

#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "pCompiler.h"

int main(int argc, char* argv[]) {

	regex_t isFile;
	int result;
	result = regcomp(&isFile, "^.*\.txt$", 0);
	if (result != 0) {
		printf("Could not compile regular expression");
    	return 0;
	}
	char *inFile, *outFile;
	// Used to keep track of how far into the arguments to loop while trying to find a recognized command.
	int loopTill = 1;
	// Checks if second to last argument is a text file. If so, it's the input file and the last argument is the output file.
	if (regexec(&isFile, argv[argc-2], 0, NULL, 0) == 0) {
		inFile = argv[argc-2];
		outFile = argv[argc-1];
		loopTill = 2;
	} else {
		inFile = argv[argc-1];
		outFile = "output.txt";
	}
	int printLex = 0, printParse = 0, printMachine = 0;

	for (int i = 1; i < argc - loopTill; i++) {
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

	// Indicates an error in the validity of the input file
	FILE* ipr = fopen(inFile, "r");
	if(ipr == NULL) {
		fprintf(stderr, "Could not find valid file by name: %s \n", inFile);
		return 0;
	}

	// Indicates an error in the validity of the output file
	// Will always print all output to output.txt regardless of user command
	FILE* opr = fopen(outFile, "w");
	if(ipr == NULL) {
		fprintf(stderr, "Could not find valid file by name: %s \n", inFile);
		return 0;
	}

	// Indicates error in lexicographical analyzer
	int numTokens = 0;
	lexeme** tokens = lex(ipr, opr, printLex, &numTokens);
	if (tokens == NULL) {
		fprintf(stderr, "Error in Lexicographical Analyzer. Program Failed to Compile\n");
		return 0;
	}

	// Indicates error in parser
	int numInstructions = 0;
	instruction *code = parse(tokens, numTokens, opr, &numInstructions);
	if ( code == NULL) {
		fprintf(stderr, "Error in Parser. Program Failed to Compile\n");
		return 0;
	}
	fprintf(opr, "No Errors, program is syntactically correct.");

	// Exports the virtual machine running to the compiler
	execute(code, opr, printMachine, printParse, numInstructions);

	return 1;
}