// Master Header File Used Across all areas of the program

#include <stdio.h>

/*
============================================USED IN VIRTUAL MACHINE======================================
*/



// enum to keep track of op-codes for instructions
typedef enum op_code {
	LIT = 1, RET, LOD, STO, CAL, INC, JMP, JPC, SIO, NEG, ADD,
	SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
} op_code;

// struct used to organize instructions
typedef struct{
    op_code op;
    int r;
    int l;
    int m;
} instruction;

// Constants used in the Virtual Machine
#define MAX_DATASTACK_HEIGHT  40
#define MAX_CODE_LENGTH  200
#define MAX_LEXI_LEVELS  3
#define NUM_REGISTERS 8
// Potential Error Messages
#define STACK_OVERFLOW fprintf(stderr, "Runtime Error: Stack Overflow.\n");
#define INTEGER_OVERFLOW fprintf(stderr, "Runtime Error: Integer Overflow, number too large.\n");
#define OUT_OF_BOUNDS fprintf(stderr, "Runtime Error: Invalid Memory Location.\n");
#define BAD_OPERATION fprintf(stderr, "Runtime Error: Invalid Operation.\n");

// Method used to print current state of the machine
void printState(int* stack, int curLoc, instruction ir, int pc, int bp, int sp, int* regFile, int l, FILE* output, char** oper, int printMachine);
// Method used for formatting in the event a value requires 2 digits.
void makeBuffer(char *str, int num, int maxSize);
// Method used to redefine base
int base(int* stack, int l, int base);
// Method used to redefine base in terms of requested lexicographical level by dynamic link
// Used only for Printing Purposes
int dynamicBase(int* stack, int l, int base);
// Method used to export the Virtual Machine running to the compiler
void execute(instruction* code, FILE* opr, int printMachine, int printParse, int numInstructions);


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
	readsym , elsesym
} token_type;

// enum used to keep track of the type of error encountered
typedef enum {
	numLengthError = 1, varLengthError, invalidSymbolError,
	invalidIdentifierError, openCommentError, missingPeriodError,
	identifierAlreadyConstError, identifierAlreadyVarError,
	identifierDoesntExistError, identifierExpectedError,
	semicolonExpectedError, eqlExpectedError, numberExpectedError,
	constAlreadyExists, endExpectedError, thenExpectedError,
	doExpectedError, relopExpectedError, constReassignError, 
	becomesExpectedError, invalidExpressionError, incompleteProgramError,
	procedureExpectedConstError, procedureExpectedVarError,
	procInExpressionError, procReassignError,
	procedureDoesntExistError, variableDoesntExistError,
	procedureAlreadyExistsError, commaExpectedError
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
	int lineNum;
} lexeme;

// Method used to add to Token and Lexeme Lists
void addLexeme(lexeme*** lexemes, char* lex, token_type token, int lineNum);
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
	int kind;	// const = 1, var = 2, procedure = 3
	char* name;	// name of identifier
	int val;	// number value of symbol
	int level;	// lexicographical level of symbol
	int address;// memory address of symbol
	int mark;	// keeps track of lifetime of symbol. 0 if in use, 1 if no longer in use.
} symbol;

// Method to handle parsing and code generation
instruction* parse(lexeme** tokens, int numTokens, FILE* opr, int* numInstructions);

// Every non-terminal in the Language Grammar is represented by a function.
int program(instruction* code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, FILE* opr);
int block(instruction* code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, FILE* opr,
  int curLexLevel, symbol* curProc);
int constdeclaration(instruction* code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, FILE* opr, int curLexLevel);
int vardeclaration(instruction* code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, FILE* opr, int* numVars, int curLexLevel);
int procdeclaration(instruction* code, symbol*** symbolTable, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, FILE* opr, int curLexLevel);
int statement(instruction* code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, int* curReg, FILE* opr, int curLexLevel);
int condition(instruction* code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, int* curReg, FILE* opr, int curLexLevel);
int expression(instruction* code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, int* curReg, FILE* opr, int curLexLevel);
int term(instruction* code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, int* curReg, FILE* opr, int curLexLevel);
int factor(instruction* code, symbol*** symbolTabe, int* numSymbols,
 lexeme** tokens, int numTokens, int* numInstructions, int* curToken, int* curReg, FILE* opr, int curLexLevel);

// Method used to add symbols to the symbol table
int addSymbol(symbol*** symbolTabe, int* numSymbols, int kind, char* name, char* val, int level, int address);

// Method used for symbol lookup
symbol* findSymbol(symbol** symbolTabe, char* name, int kind, int numSymbols, FILE* opr);

// Method used to simplify symbol deactivation
void deleteSymbols(symbol** symbolTable, int numSymbols, int curLexLevel, FILE* opr);

// Method used for adding instructions during code generation
int addInstruction(instruction* code, op_code op, int r, int l, int m, int* numInstructions, FILE* opr);

// Method used to get the next token for use by the parser
int getToken(int *curToken, int numTokens, lexeme **tokens, FILE* opr);