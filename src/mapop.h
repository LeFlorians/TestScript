#include <string.h>

typedef struct s_op operator;

struct s_op {
    char *name;
    
    enum {
        INC, DEC, PIN, PDE, POS, NEG, NOT,
        MUL, DIV, MOD, ADD, SUB, LET, LEQ, 
        GRT, GEQ, EQU, NEQ, AND, ORO, CON,
        ASS, PEQ, MEQ, TEQ, DEQ, REQ, LIS, 
    } opcode;

    enum {
        INFIX=0b00, POSTFIX=0b01, PREFIX=0b10,
    } position;

    enum {
        LTR=0, RTL=1,
    } associativity;

    unsigned char precedence;
};

// Function to map infix/postfix operators
operator *mapop(char *expr);

// Function to map expecially prefix operators
// This replaces ++,--,+,- with the correct values if necessary
operator *mappreop(char *expr);