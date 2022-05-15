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

// For internal use
struct s_op *_lookup(register const char *str, register size_t len);

// Function to map infix/postfix operators
operator *mapop(char *expr) {
    return _lookup(expr, strlen(expr));
}

// Function to map expecially prefix operators
// This replaces ++,--,+,- with the correct values if necessary
operator *mappreop(char *expr) {
    operator *ret = _lookup(expr, strlen(expr));
    switch(ret->opcode) {
        case INC:
            ret->opcode = PIN;
            ret->associativity = RTL;
            ret->position = PREFIX;
            ret->precedence = 2;
            break;
        case DEC:
            ret->opcode = PDE;
            ret->associativity = RTL;
            ret->position = PREFIX;
            ret->precedence = 2;
            break;
        case ADD:
            ret->opcode = POS;
            ret->associativity = RTL;
            ret->position = PREFIX;
            ret->precedence = 2;
            break;
        case SUB:
            ret->opcode = NEG;
            ret->associativity = RTL;
            ret->position = PREFIX;
            ret->precedence = 2;
        default: break;
    }
    return ret;
}


