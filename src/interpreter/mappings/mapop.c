/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --output-file=./src//interpreter/mappings/mapop.c ./mapop.gperf  */
/* Computed positions: -k'1-2' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
#error "gperf generated tables don't work with this execution character set."
#endif

#line 8 "./mapop.gperf"

    // Manually include header header file
    #include "mapop.h"

    /*
     ! These have to be manually replaced if used as prefix:
        
        INC -> PIN
        DEC -> PDE
        ADD -> POS
        SUB -> NEG
    */
#line 23 "./mapop.gperf"
struct s_op;

#define TOTAL_KEYWORDS 25
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 2
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 62
/* maximum key range = 62, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63,  8, 63, 63, 63, 31, 30, 63,
      63, 63, 26, 25,  6,  5, 63, 28, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 23, 18,
       3,  0, 15, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 13, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 20, 63, 10, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

struct s_op *
lookup (register const char *str, register size_t len)
{
  static struct s_op wordlist[] =
    {
      {""},
#line 46 "./mapop.gperf"
      {"=",    OP_ASS,     INFIX,   RTL, 3     /*  Assign                    */},
#line 39 "./mapop.gperf"
      {"==",   OP_EQU,     INFIX,   LTR, 9     /*  Equals                    */},
      {""},
#line 35 "./mapop.gperf"
      {"<",    OP_LET,     INFIX,   LTR, 10    /*  Less Than                 */},
#line 36 "./mapop.gperf"
      {"<=",   OP_LEQ,     INFIX,   LTR, 10    /*  Less than or equal to     */},
#line 34 "./mapop.gperf"
      {"-",    OP_SUB,     INFIX,   LTR, 11    /*  Subtract                  */},
#line 49 "./mapop.gperf"
      {",",    OP_LIST,    INFIX,   LTR, 2     /*  List                      */},
      {""},
#line 28 "./mapop.gperf"
      {"!",    OP_LNOT,    PREFIX,  RTL, 13    /*  Logical NOT               */},
#line 40 "./mapop.gperf"
      {"!=",   OP_NEQU,    INFIX,   LTR, 9     /*  Not Equals                */},
#line 29 "./mapop.gperf"
      {"~",    OP_BNOT,    PREFIX,  RTL, 13    /*  Bitwise NOT               */},
#line 27 "./mapop.gperf"
      {"--",   OP_DECR,    POSTFIX, LTR, 14    /*  Decrement                 */},
      {""},
#line 42 "./mapop.gperf"
      {"^",    OP_BXOR,    INFIX,   LTR, 7     /*  Bitwise XOR               */},
      {""},
#line 37 "./mapop.gperf"
      {">",    OP_GRT,     INFIX,   LTR, 10    /*  Greater than              */},
#line 38 "./mapop.gperf"
      {">=",   OP_GEQ,     INFIX,   LTR, 10    /*  Greater than or equal to  */},
      {""},
#line 50 "./mapop.gperf"
      {";",    OP_END,     POSTFIX, RTL, 1     /*  Semicolon; end expression */},
      {""},
#line 43 "./mapop.gperf"
      {"|",    OP_BOR,     INFIX,   LTR, 6     /*  Bitwise OR                */},
#line 48 "./mapop.gperf"
      {"->",   OP_LAMBDA,  INFIX,   RTL, 3     /*  Lambda declaration        */},
      {""},
#line 47 "./mapop.gperf"
      {":",    OP_HARDSET, INFIX,   RTL, 3     /*  Set in current scope      */},
      {""},
#line 33 "./mapop.gperf"
      {"+",    OP_ADD,     INFIX,   LTR, 11    /*  Add                       */},
#line 30 "./mapop.gperf"
      {"*",    OP_MUL,     INFIX,   LTR, 12    /*  Multiply                  */},
      {""},
#line 31 "./mapop.gperf"
      {"/",    OP_DIV,     INFIX,   LTR, 12    /*  Divide                    */},
      {""},
#line 41 "./mapop.gperf"
      {"&",    OP_BAND,    INFIX,   LTR, 8     /*  Bitwise AND               */},
#line 32 "./mapop.gperf"
      {"%",  OP_MOD,     INFIX,   LTR, 12    /*  Modulo                    */},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 45 "./mapop.gperf"
      {"||",   OP_LOR,     INFIX,   LTR, 4     /*  Logical OR                */},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 26 "./mapop.gperf"
      {"++",   OP_INCR,    POSTFIX, LTR, 14    /*  Increment                 */},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 44 "./mapop.gperf"
      {"&&",   OP_LAND,    INFIX,   LTR, 5     /*  Logical AND               */}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 51 "./mapop.gperf"


operator *mapop(char *expr) {
    return lookup(expr, strlen(expr));
}

operator *mappreop(char *expr) {
    static operator virt[] = {
        {"++_",      OP_PREINCR, PREFIX, RTL, 9},
        {"--_",      OP_PREDECR, PREFIX, RTL, 9},
        {"+_",       OP_POS,     PREFIX, RTL, 9},
        {"-_",       OP_NEG,     PREFIX, RTL, 9},
    };

    operator *ret = lookup(expr, strlen(expr));

    if(ret != 0) {
        switch(ret->opcode) {
            case OP_INCR:
                ret = &virt[0];
                break;
            case OP_DECR:
                ret = &virt[1];
                break;
            case OP_ADD:
                ret = &virt[2];
                break;
            case OP_SUB:
                ret = &virt[3];
            default: break;
        }
    }
    return ret;
}
