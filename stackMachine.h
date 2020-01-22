#define MAX_DATASTACK_HEIGHT  30
#define MAX_CODE_LENGTH  500
#define MAX_LEXI_LEVELS  3
//struct used to organize instructions
struct{
    int op;
    int l;
    int m;
}instruction;
//Functions for each ISA input
void literal(int val);
void operation(int op);
void load(int lex, int offset);
void store(int lex, int offset);
void call(int lex, int index);
void inc(int numLocals);
void jump(int loc);
void jmpIfZero(int loc);
void write();
void read();
void end();
//Functions for each Op code for operation()
void ret();
void negate();
void add();
void sub();
void mult();
void div();
void odd();
void mod();
void equal();
void notEqual();
void less();
void lessOrEqual();
void greater();
void greaterOrEqual();