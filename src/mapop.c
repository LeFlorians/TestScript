/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --output-file=src/mapop.c mapop.gperf  */
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
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 8 "mapop.gperf"

    // Manually include header header file
    #include "mapop.h"

    /*
     ! These have to be manually replaced if used as prefix:
        
        INC -> PIN
        DEC -> PDE
        ADD -> POS
        SUB -> NEG
    */
#line 23 "mapop.gperf"
struct s_op;

#define TOTAL_KEYWORDS 33
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
      63, 63, 63, 22, 63, 63, 63, 17, 25, 63,
      63, 63, 12, 20, 30,  5, 63,  7, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 10, 63,
       2,  0, 55, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 45, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 15, 63, 35, 63, 63, 63,
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
#line 46 "mapop.gperf"
      {"=",    OP_ASS,     INFIX,   RTL, 2     /*  Assign                    */},
#line 39 "mapop.gperf"
      {"==",   OP_EQU,     INFIX,   LTR, 8     /*  Equals                    */},
#line 35 "mapop.gperf"
      {"<",    OP_LET,     INFIX,   LTR, 9    /*  Less Than                 */},
#line 36 "mapop.gperf"
      {"<=",   OP_LEQ,     INFIX,   LTR, 9    /*  Less than or equal to     */},
      {""},
#line 34 "mapop.gperf"
      {"-",    OP_SUB,     INFIX,   LTR, 10    /*  Subtract                  */},
#line 52 "mapop.gperf"
      {"-=",   OP_SUBASS,  INFIX,   RTL, 2     /*  Subtraction Assignment    */},
#line 31 "mapop.gperf"
      {"/",    OP_DIV,     INFIX,   LTR, 11    /*  Divide                    */},
#line 54 "mapop.gperf"
      {"/=",   OP_DIVASS,  INFIX,   RTL, 2     /*  Division Assignment       */},
      {""},
#line 56 "mapop.gperf"
      {":",    OP_DEFAULT, INFIX,   RTL, 2     /*  Default Assignment        */},
#line 27 "mapop.gperf"
      {"--",   OP_DECR,    POSTFIX, LTR, 13    /*  Decrement                 */},
#line 30 "mapop.gperf"
      {"*",    OP_MUL,     INFIX,   LTR, 11    /*  Multiply                  */},
#line 53 "mapop.gperf"
      {"*=",   OP_MULASS,  INFIX,   RTL, 2     /*  Multiplication Assignment */},
      {""},
#line 43 "mapop.gperf"
      {"|",    OP_BOR,     INFIX,   LTR, 5     /*  Bitwise OR                */},
#line 50 "mapop.gperf"
      {"|=",   OP_BORASS,  INFIX,   RTL, 2     /*  Bitwise OR Assignment     */},
#line 32 "mapop.gperf"
      {"%",  OP_MOD,     INFIX,   LTR, 11    /*  Modulo                    */},
#line 55 "mapop.gperf"
      {"%=", OP_MODASS,  INFIX,   RTL, 2     /*  Modulo Assignment         */},
      {""},
#line 33 "mapop.gperf"
      {"+",    OP_ADD,     INFIX,   LTR, 10    /*  Add                       */},
#line 51 "mapop.gperf"
      {"+=",   OP_ADDASS,  INFIX,   RTL, 2     /*  Addition Assignment       */},
#line 28 "mapop.gperf"
      {"!",    OP_LNOT,    PREFIX,  RTL, 12    /*  Logical NOT               */},
#line 40 "mapop.gperf"
      {"!=",   OP_NEQU,    INFIX,   LTR, 8     /*  Not Equals                */},
      {""},
#line 41 "mapop.gperf"
      {"&",    OP_BAND,    INFIX,   LTR, 7     /*  Bitwise AND               */},
#line 49 "mapop.gperf"
      {"&=",   OP_BANDASS, INFIX,   RTL, 2     /*  Bitwise AND Assignment    */},
      {""}, {""}, {""},
#line 58 "mapop.gperf"
      {",",    OP_LIST,    INFIX,   RTL, 1     /*  List                      */},
#line 45 "mapop.gperf"
      {"||",   OP_LOR,     INFIX,   LTR, 3     /*  Logical OR                */},
      {""}, {""}, {""},
#line 29 "mapop.gperf"
      {"~",    OP_BNOT,    PREFIX,  RTL, 12    /*  Bitwise NOT               */},
#line 48 "mapop.gperf"
      {"~=",   OP_BNOTASS, INFIX,   RTL, 2     /*  Bitwise NOT Assignment    */},
      {""}, {""}, {""}, {""},
#line 26 "mapop.gperf"
      {"++",   OP_INCR,    POSTFIX, LTR, 13    /*  Increment                 */},
      {""}, {""}, {""},
#line 42 "mapop.gperf"
      {"^",    OP_BXOR,    INFIX,   LTR, 6     /*  Bitwise XOR               */},
#line 47 "mapop.gperf"
      {"^=",   OP_BXORASS, INFIX,   RTL, 2     /*  Bitwise XOR Assignment    */},
      {""}, {""}, {""}, {""},
#line 44 "mapop.gperf"
      {"&&",   OP_LAND,    INFIX,   LTR, 4     /*  Logical AND               */},
      {""}, {""}, {""},
#line 37 "mapop.gperf"
      {">",    OP_GRT,     INFIX,   LTR, 9    /*  Greater than              */},
#line 38 "mapop.gperf"
      {">=",   OP_GEQ,     INFIX,   LTR, 9    /*  Greater than or equal to  */},
      {""}, {""}, {""}, {""},
#line 57 "mapop.gperf"
      {"->",   OP_LAMBDA,  INFIX,   RTL, 2     /*  Lambda declaration        */}
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
#line 59 "mapop.gperf"


operator *mapop(char *expr) {
    return lookup(expr, strlen(expr));
}

operator *mappreop(char *expr) {
    operator *ret = lookup(expr, strlen(expr));
    if(ret != 0) {
        switch(ret->opcode) {
            case OP_INCR:
                ret->opcode = OP_PREINCR;
                ret->associativity = RTL;
                ret->position = PREFIX;
                ret->precedence = 9;
                break;
            case OP_DECR:
                ret->opcode = OP_PREDECR;
                ret->associativity = RTL;
                ret->position = PREFIX;
                ret->precedence = 9;
                break;
            case OP_ADD:
                ret->opcode = OP_POS;
                ret->associativity = RTL;
                ret->position = PREFIX;
                ret->precedence = 9;
                break;
            case OP_SUB:
                ret->opcode = OP_NEG;
                ret->associativity = RTL;
                ret->position = PREFIX;
                ret->precedence = 9;
            default: break;
        }
    }
    return ret;
}