// Master Header File Used Across all areas of the program

#include <stdio.h>

/*
============================================USED IN VIRTUAL MACHINE======================================
*/



// enum to keep track of op-codes for instructions
typedef enum op_code {
	LIT = 1, RET, LOD, STO, CAL, INC, JMP, JPC, SIO, NEG, ADD,
	SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
}op_code;

// struct used to organize instructions
typedef struct{
    op_code op;
    int r;
    int l;
    int m;
}instruction;

// Constants used in the Virtual Machine
#define MAX_DATASTACK_HEIGHT  40
#define MAX_CODE_LENGTH  200
#define MAX_LEXI_LEVELS  3
#define NUM_REGISTERS 8
// Potential Error Messages
#define STACK_OVERFLOW fprintf(stderr, "Runtime Error: Stack Overflow.\n");
#define OUT_OF_BOUNDS fprintf(stderr, "Runtime Error: Invalid Memory Location.\n");
#define BAD_OPERATION fprintf(stderr, "Runtime Error: Invalid Operation.\n");

// Method used to print current state of the machine
void printState(int* stack, int curLoc, instruction ir, int pc, int bp, int sp, int* regFile, int l, FILE* output, char** oper);
// Method used for formatting in the event a value requires 2 digits.
void makeBuffer(char *str, int num, int maxSize);
// Method used to redefine base
int base(int* stack, int l, int base);
// Method used to export the Virtual Machine running to the compiler
int execute(instruction* code, FILE* opr);



/*
============================================USED IN LEXER================================================
*/



#define MAX_VAR_LENGTH 11
#define MAX_NUM_LENGTH 5

// enum used to keep track of the various token types in the lexer
typedef enum token_type { 
comment=0, nulsym=1, identsym, numbersym, plussym, minussym,
multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym, 
whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
readsym , elsesym} token_type;

// enum used to keep track of the type of error encountered
typedef enum {
	numLengthError = 1, varLengthError, invalidSymbolError,
	invalidIdentifierError, openCommentError
} error_type;

// struct used to store errors to be printed at a later time
typedef struct {
	int lineNum;
	error_type type;
	char* value;
} error;

// struct used to store information related to tokens
typedef struct {
	char* value;
	token_type token;
} lexeme;

// Method used to add to Token and Lexeme Lists
void addLexeme(lexeme*** lexemes, char* lex, token_type token);
// Method used to deal with spacing when printing the Lexeme Table
void makeBuf(char buffer[], char* str, int bufLen);
// Method used to keep track of Lexical Errors
void addError(char* str, error_type type, int lineNum);
// Method used to export the lexer to the compiler
lexeme** lex(FILE* ipr, FILE* opr, int toConsole, int* tokenNum);



/*
============================================USED IN PARSER===============================================
*/



// struct used to represent an element of the symbol table in the parser
typedef struct{
	int kind;	// const = 1, var = 2
	char* name;	// name of identifier
	int val;	// number value of symbol
	int level;	// lexicographical level of symbol
	int address;// memory address of symbol
	int mark;	// keeps track of lifetime of symbol. 0 if in use, 1 if no longer in use.
}symbol;

// Method to handle parsing and code generation
instruction* parse(lexeme** tokens, int numTokens, FILE* opr, int printParse, int* numInstructions);

// Every non-terminal in the Language Grammar is represented by a function.
int program(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int block(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int constdeclaration(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int vardeclaration(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int statement(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int condition(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int relop(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int expression(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int term(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int factor(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int number(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int identifier(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int digit(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);
int letter(instruction** code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken);

// Method used to add symbols to the symbol table.
int addSymbol(symbol*** symbolTabe, int* numSymbols, int kind, char* name, int val, int level, int address);

// Method used for symbol lookup.
symbol* findSymbol(symbol** symbolTabe, char* name, int numSymbols);

// Method used to simplify symbol deactivation.
int deleteSymbol(symbol** symbolTabe, char* name, int numSymbols);

// Method used for adding instructions during code generation
int addInstruction(instruction** code, op_code op, int r, int l, int m, int* numInstructions);