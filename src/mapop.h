#include <string.h>

typedef struct s_op op;

struct s_op {
    char *name;
    
    enum {
        INC, DEC, PIN, PDE, POS, NEG, NOT,
        MUL, DIV, MOD, ADD, SUB, LET, LEQ, 
        GRT, GEQ, EQU, NEQ, AND, ORO, CON,
        ASS, PEQ, MEQ, TEQ, DEQ, REQ, LIS, 
    } opcode;

    /* 
        Info description:
        char info = 0b aa bb cccc;
        - a: 00=Infix, 01=Postfix, 10=Prefix, 11=Unspecified 
        - b: 00=LTR, 01=RTL, 10,11=Unspecified
        - c: Associativity (see table in parser.h)
    
        ab hex table:


    */    
    char info;
};

// For internal use
struct s_op *_lookup(register const char *str, register size_t len);

// Function to map infix/postfix operators
op *mapop(char *expr) {
    return _lookup(expr, strlen(expr));
}

// Function to map expecially prefix operators
// This replaces ++,--,+,- with the correct values if necessary
op *mappreop(char *expr) {
    op *ret = _lookup(expr, strlen(expr));
    switch(ret->opcode) {
        case INC:
            ret->opcode = PIN;
            ret->info = 0x91;
            break;
        case DEC:
            ret->opcode = PDE;
            ret->info = 0x91;
            break;
        case POS:
            ret->opcode = PDE;
            ret->info = 0x91;
            break;
        case SUB:
            ret->opcode = NEG;
            ret->info = 0x91;
            break;
        default: break;
    }
    return ret;
}