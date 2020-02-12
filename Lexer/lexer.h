#define MAX_VAR_LENGTH 11
#define MAX_NUM_LENGTH 5



typedef enum token_type { 
nulsym =1, identsym, numbersym, plussym, minussym,
multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym, 
whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
readsym , elsesym} token_type;

// Method used to add to Token and Lexeme Lists
void addLexeme(char *lex, int token);
// Method used to print remainder of input file in the event of an error
void printRest(FILE *input, FILE *output);
