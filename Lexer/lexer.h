#define MAX_VAR_LENGTH 11
#define MAX_NUM_LENGTH 5



typedef enum token_type { 
comment=0, nulsym=1, identsym, numbersym, plussym, minussym,
multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym, 
whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
readsym , elsesym} token_type;

typedef enum {
	numLengthError = 1, varLengthError, invalidSymbolError,
	openCommentError
} error_type;

typedef struct {
	int lineNum;
	error_type type;
	char* value;
} error;

typedef struct {
	char* value;
	token_type token;
} lexeme;

// Method used to add to Token and Lexeme Lists
void addLexeme(char* lex, token_type token);
// Method used to deal with spacing when printing the Lexeme Table
void makeBuf(char buffer[], char* str, int bufLen);
// Method used to keep track of Lexical Errors
void addError(char* str, error_type type, int lineNum);