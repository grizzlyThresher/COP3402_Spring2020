// Owen Brahms - 3739927
// Michaela Green - 4198343
// COP 3402, Spring 2020
// Project 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "lexer.h"

// numTokens used as length of our lexeme/token array
// numErrors used as length of our error array
// numLines used to keep track of line number for error handling purposes
int numTokens, numErrors, numLines;
// Our list of lexeme/token pairs
lexeme **lexemes;
// Our list of errors
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


    // If display is entered before the file to be read when running the program, lexer output is printed to console
    // Otherwise, file is read normally and lexer output goes to specified output file. 
    // If no output file is specified, system defaults to output.txt
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

	int ungot = 0; // Used to check if we've had to call ungetc() for that pass of the loop.
	int badVar = 0; // Used to check if the potential identifier starts with a number
	int varLength = 0; // Used to keep track of the length of our buffer
	int almostEnding = 0; // Used while in a comment to know if we should exit when give a '/'
	numTokens = 0; // Initially no tokens in lexeme/token array
	numLines = 1; // Program always starts at line 1
	numErrors = 0; // Initially no errors in error array

	// Used to store potential identifiers and numbers
	char *buffer = malloc(varLength * sizeof(char));

	do {

		tmpC = fgetc(ipr); // Sets tmpC equal to the next char from the input file
		if (EOF == tmpC) // If the next char is the end of file, break out of do-while
			break;

		ungot = 0;
		switch (state) { // Switches state bassed on the character read from the input file

			case comment : // Handles all comments
				if (tmpC == '*') { // If we read in a '*', a '/' will end the comment-- check for this next
					almostEnding = 1;
					state = comment;
				} else if ((tmpC == '/') && (almostEnding == 1)) { // If the comment successfully closes, we remove
					free(errorList[--numErrors]->value); // no close error from the errorList and return to normal
					free(errorList[numErrors]);          // execution of the lexer, ready to read in the next input
					errorList = realloc(errorList, numErrors * sizeof(error*));
					state = nulsym;
				} else if (tmpC == '\n') { // Keeps track of number of lines in the code, even through comments
					numLines++;
					almostEnding = 0;
					state = comment;
				} else { // Still a comment, continue parsing
					almostEnding = 0;
					state = comment;
				} 
				break;

			case nulsym : // Handles all transitions from the start state
				switch (tmpC) {
					case '+': // Character read in is a '+'
						addLexeme("+", plussym);
						break;
					case '-': // Character read in is a '-'
						addLexeme("-", minussym);
						break;
					case '*': // Character read in is a '*'
						addLexeme("*", multsym);
						break;
					case '/': // Character read in is a '/'
						state = slashsym;
						break;
					case '=': // Character read in is a '='
						addLexeme("=", eqsym);
						break;
					case '<': // Character read in is a '<'
						state = lessym; // Can lead to multi-char operator, move to new state
						break;
					case '>': // Character read in is a '>'
						state = gtrsym; // Can lead to multi-char operator, move to new state
						break;
					case '(': // Character read in is a '('
						addLexeme("(", lparentsym);
						break;
					case ')': // Character read in is a ')'
						addLexeme(")", rparentsym);
						break;
					case ',': // Character read in is a ','
						addLexeme(",", commasym);
						break;
					case ';': // Character read in is a ';'
						addLexeme(";", semicolonsym);
						break;
					case '.': // Character read in is a '.'
						addLexeme(".", periodsym);
						break;
					case ':': // Character read in is a ':'
						state = becomessym; // Can lead to multi-char operator, move to new state
						break;
					case '\n': // Character read in is a new line character
						numLines++; // Keeps track of line number for error parsing
					case ' ':  // Ignores any sort of spacing character and moves to the next character
					case '\t': //  |
					case '\a': //  |
					case '\b': //  |
					case '\r': //  |
					case '\f': //  |
					case '\v': //  V
						state = nulsym;
						break;

					default : // The char is not a recognized special character
						badVar = 0;
						charAsString[0] = tmpC;
						if (regexec(&letter, charAsString, 0, NULL, 0) == 0) { // Checks if char is a letter
							state = identsym;
							varLength++;
							buffer = realloc(buffer, varLength * sizeof(char));
							buffer[varLength - 1] = tmpC;
						} else if (regexec(&digit, charAsString, 0, NULL, 0) == 0) { // Checks if char is a digit
							state = numbersym;
							varLength++;
							buffer = realloc(buffer, varLength * sizeof(char));
							buffer[varLength - 1] = tmpC;
						} else { // Char is neither a letter or digit, so print remaining code and throw a new error
							addError(charAsString, invalidSymbolError, numLines);
							state = nulsym;
						}
						break;
				}
				break;

			case identsym : // Handles all transitions for potential identifiers and reserved words
				charAsString[0] = tmpC;
				if ((regexec(&letter, charAsString, 0, NULL, 0) == 0) ||
				 regexec(&digit, charAsString, 0, NULL, 0) == 0) { // Checks if the next char is a letter or digit
					varLength++; // Begins the process of adding the new cahr to the buffer
					buffer = realloc(buffer, varLength * sizeof(char));
					buffer[varLength - 1] = tmpC;
				} else { // Checks if the char is anything other than a letter or digit, indicating that the variable string has ended
					ungetc(tmpC, ipr); // Re-checks the next char in the series by directing it back to nulsym
					ungot = 1;
					state = nulsym;
					buffer = realloc(buffer, (varLength + 1) * sizeof(char)); // Tidies up the buffer to finish the string
					buffer[varLength] = '\0';
					if ((varLength > MAX_VAR_LENGTH) || (badVar == 1)) { // Checks if there's an error with the identifier
						if (varLength > MAX_VAR_LENGTH) { // Throws an error if the identifier is longer than the max specification
							addError(buffer, varLengthError, numLines);
						} else if (badVar == 1) { // Throws an error if the (invalid) identifier begins with a number
							addError(buffer, invalidIdentifierError, numLines);
						}
					} else if (badVar == 1) { 
					} else if (strcmp(buffer,  "odd") == 0) { // String read in is "odd", valid string
						addLexeme(buffer, oddsym);
					} else if (strcmp(buffer, "begin") == 0) { // String read in is "begin", valid string
						addLexeme(buffer, beginsym);
					} else if (strcmp(buffer, "end") == 0) { // String read in is "end", valid string
						addLexeme(buffer, endsym);
					} else if (strcmp(buffer, "if") == 0) { // String read in is "if", valid string
						addLexeme(buffer, ifsym);
					} else if (strcmp(buffer, "then") == 0) { // String read in is "then", valid string
						addLexeme(buffer, thensym);
					} else if (strcmp(buffer, "while") == 0) { // String read in is "while", valid string
						addLexeme(buffer, whilesym);
					} else if (strcmp(buffer, "do") == 0) { // String read in is "do", valid string
						addLexeme(buffer, dosym);
					} else if (strcmp(buffer, "call") == 0) { // String read in is "call", valid string
						addLexeme(buffer, callsym);
					} else if (strcmp(buffer, "const") == 0) { // String read in is "const", valid string
						addLexeme(buffer, constsym);
					} else if (strcmp(buffer, "var") == 0) { // String read in is "var", valid string
						addLexeme(buffer, varsym);
					} else if (strcmp(buffer, "procedure") == 0) { // String read in is "procedure", valid string
						addLexeme(buffer, procsym);
					} else if (strcmp(buffer, "write") == 0) { // String read in is "write", valid string
						addLexeme(buffer, writesym);
					} else if (strcmp(buffer, "read") == 0) { // String read in is "read", valid string
						addLexeme(buffer, readsym);
					} else if (strcmp(buffer, "else") == 0) { // String read in is "else", valid string
						addLexeme(buffer, elsesym);
					} else { // String read in is a new valid variable
						addLexeme(buffer, identsym);
					}
					varLength = 0; // Resets buffer back to empty in preparation for next pass through the lexer
					buffer = realloc(buffer, varLength * sizeof(char));
				}
            break;

            case numbersym : // Handles all transitions for numbers and checks for invalid identifiers
            	charAsString[0] = tmpC;
            	if (regexec(&digit, charAsString, 0, NULL, 0) == 0) { // Checks if the char is a digit
            		varLength++; // Begins the process of adding the char to the buffer
            		buffer = realloc(buffer, varLength * sizeof(char));
            		buffer[varLength - 1] = tmpC;
            		state = numbersym; // Continues checking numbersym to see if the number continues
            	} else if (regexec(&letter, charAsString, 0, NULL, 0) == 0) { // Checks if the char is a letter
            		varLength++; // Begins the process of adding the char to the buffer
            		buffer = realloc(buffer, varLength * sizeof(char));
            		buffer[varLength - 1] = tmpC;
            		badVar = 1; // Indicates that the number series has a letter in it, making it an invalid identifier
            		state = identsym; // Moves the next char check in the series to identsym, as the string is now
            		                  // considered to be an (invalid) identifier
            	} else { // Checks if the char is anything other than a digit or letter, indicating that the number
            		     // string has finished
            		
            		ungetc(tmpC, ipr); // Re-checks the next character in the series by directing it back to nulsym
            		ungot = 1;
            		buffer = realloc(buffer, (varLength+1) * sizeof(char)); // Tidies up the buffer to finish up the string
            		buffer[varLength] = '\0';
            		if (varLength > 5) { // Checks the length of the read in number string and throws an error if it's too long
            			addError(buffer, numLengthError, numLines);
            		} else {// Otehrwise adds the string from the buffer into the lexeme list
            			addLexeme(buffer, numbersym); 
            		}
            		varLength = 0; // Begins the process of reallocating and clearing the buffer
            		buffer = realloc(buffer, (varLength + 1) * sizeof(char));
            		state = nulsym;
            	}
            	break;

            case slashsym : // Checks the next char after '/' is read in
            	if (tmpC == '*') { // String indicates the start of a comment
            		addError("", openCommentError, numLines);
       				almostEnding = 0;
            		state = comment; // Begin comment parsing
            	} else { // Previous char read in is '/', re-check current char
            		addLexeme("/", slashsym);
            		ungetc(tmpC, ipr);
            		ungot = 1;
            		state = nulsym;
            	}
            	break;

			case lessym : // Checks the next char after '<' is read in
				if (tmpC == '=') { // String read in is "<="
					addLexeme("<=", leqsym);
				} else if (tmpC == '>') { // String read in is "<>"
					addLexeme("<>", neqsym);
				} else { // Previous char read in is '<', re-check current char
					addLexeme("<", lessym);
					ungetc(tmpC, ipr);
					ungot = 1;
				}
				state = nulsym;
				break;

			case gtrsym : // Checks the next char after '>' is read in
				if (tmpC == '=') { // String read in is ">="
					addLexeme(">=", geqsym);
				} else { // Previous char read in is '>', re-check current char
					addLexeme(">", gtrsym);
					ungetc(tmpC, ipr);
					ungot = 1;
				}
				state = nulsym;
				break;

			case becomessym : // Checks the next char after ':' is read in
				if (tmpC == '=') { // String read in is ":="
					addLexeme(":=", becomessym);
				} else { // Previous char read in is invalid, throw error and re-check current char
					addError(":", invalidSymbolError, numLines);
					ungetc(tmpC, ipr);
					ungot = 1;
				}
				state = nulsym;
				break;
			}

		if (ungot == 0) { // If the current char has to be checked again, don't print it out yet
			fprintf(opr, "%c", tmpC);
		}
	} while(1);

	fprintf(opr, "\n\n\n");

	if (numErrors >= 1) { // If any errors were encountered in the input file, prints error and all associated info
		for (int i = 0; i < numErrors; i++) {
			fprintf(opr, "Error at (%s:%d): ",inFile, errorList[i]->lineNum);
			switch (errorList[i]->type) {
				case numLengthError : // Number length exceeds maximum allowed length
				fprintf(opr, "Number Longer Than %d Digits (%s).\n", MAX_NUM_LENGTH, errorList[i]->value);
				break;
				case varLengthError : // Variable length exceeds maximum allowed length
				fprintf(opr, "Identifier Longer Than %d Characters (\"%s\").\n", MAX_VAR_LENGTH, errorList[i]->value);
				break;
				case invalidIdentifierError : // Variable is invalid if it begins with a digit
				fprintf(opr, "Identifier Begins With a Digit (\"%s\").\n", errorList[i]->value);
				break;
				case invalidSymbolError : // The read in symbol is not included in the lexeme
				fprintf(opr, "Unidentified Symbol (\'%s\').\n", errorList[i]->value);
				break;
				case openCommentError : // A comment was opened in the program but never closed
				fprintf(opr, "Open Comment Never Closed.\n");
				break;
			}
		}
	} else { // If there were no errors found in the input file, prints out the acquired lexeme table
		fprintf(opr, "Lexeme Table:\nlexeme           token type\n");

		char buf[17];

		// Prints out Lexeme Table; lexeme followed by spaces followed by the associate token
		for(int i = 0; i < numTokens; i++) {
			makeBuf(buf, lexemes[i]->value, 17);
			fprintf(opr, "%s%s%d\n", lexemes[i]->value, buf, lexemes[i]->token);
		}
		fprintf(opr, "\n\nLexeme List:\n");

		// Prints out Lexeme List; tokens, with Lexeme following if identifier or number
		for (int i = 0; i < numTokens; i++) {
			if (lexemes[i]->token == identsym || lexemes[i]->token == numbersym) {
				fprintf(opr, "%d|%s|", lexemes[i]->token, lexemes[i]->value);
			} else {
				fprintf(opr, "%d|", lexemes[i]->token);
			}
		}
		fprintf(opr, "\n");
	}

	// Frees used memory before program finishes
	for (int i = 0; i < numTokens; i++) {
		free(lexemes[i]->value);
		free(lexemes[i]);
	}
	free(lexemes);
	for (int i = 0; i < numErrors; i++) {
		free(errorList[i]->value);
		free(errorList[i]);
	}
	free(errorList);
	free(buffer);
	fclose(ipr);
	free(charAsString);
	// Only closes opr if it isn't set as stdout
	if (strcmp("display",argv[1]) != 0)
		fclose(opr);

	return 0;
}

// Method used to add to Lexeme/Token List
void addLexeme (char* lex, token_type token) {
	// Increases number of tokens, increases the size of lexeme/token array
	// adds new lexeme/token pair to the list.
	numTokens++;
	lexemes = realloc(lexemes, numTokens * sizeof(lexeme*));
	lexemes[numTokens-1] = malloc(sizeof(lexeme));
	lexemes[numTokens-1]->token = token;
	lexemes[numTokens-1]->value = malloc((strlen(lex)) * sizeof(char));
	strcpy(lexemes[numTokens-1]->value, lex);
}

// Method used to create a buffer to dynamically create space between
// displayed elements based on size. Keeps displayed info more organized.
void makeBuf(char buffer[], char* str, int bufLen) {
	if (bufLen >= strlen(str)) {
		for (int i = 0; i < (bufLen - strlen(str)); i++) {
			buffer[i] = ' ';
		}
		buffer[bufLen-strlen(str)] = '\0';
	}
}

// Method to add a new error to the error list when an error is encountered
void addError(char* str, error_type type, int lineNum) {
	// Keeps track of the number of errors, the associated buffer, the error
	// type, and what line the error occurred on
	numErrors++;
	errorList = realloc(errorList, numErrors * sizeof(error*));
	errorList[numErrors - 1] = malloc(sizeof(error));
	errorList[numErrors - 1]->type = type;
	errorList[numErrors - 1]->lineNum = lineNum;
	errorList[numErrors - 1]->value = malloc(strlen(str) * sizeof(char));
	strcpy(errorList[numErrors - 1]->value, str);
}