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

#define TOTAL_KEYWORDS 23
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 2
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 52
/* maximum key range = 52, duplicates = 0 */

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
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 18, 53, 53, 53, 13, 25, 53,
      53, 53,  8, 15, 10,  5, 53,  3, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      25,  0, 20, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 20, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53
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
_lookup (register const char *str, register size_t len)
{
  static struct s_op wordlist[] =
    {
      {""},
#line 42 "mapop.gperf"
      {"=",      ASS,    INFIX,      RTL,    10},
#line 38 "mapop.gperf"
      {"==",     EQU,    INFIX,      LTR,    6},
      {""},
#line 30 "mapop.gperf"
      {"/",      DIV,    INFIX,      LTR,    3},
#line 46 "mapop.gperf"
      {"/=",     DEQ,    INFIX,      RTL,    10},
#line 33 "mapop.gperf"
      {"-",      SUB,    INFIX,      LTR,    4},
#line 44 "mapop.gperf"
      {"-=",     MEQ,    INFIX,      RTL,    10},
      {""},
#line 29 "mapop.gperf"
      {"*",      MUL,    INFIX,      LTR,    3},
#line 45 "mapop.gperf"
      {"*=",     TEQ,    INFIX,      RTL,    10},
#line 48 "mapop.gperf"
      {",",      LIS,    INFIX,      RTL,    11},
#line 27 "mapop.gperf"
      {"--",     DEC,    POSTFIX,    LTR,    1},
      {""},
#line 31 "mapop.gperf"
      {"%",    MOD,    INFIX,      LTR,    3},
#line 47 "mapop.gperf"
      {"%=",   REQ,    INFIX,      RTL,    10},
#line 32 "mapop.gperf"
      {"+",      ADD,    INFIX,      LTR,    4},
#line 43 "mapop.gperf"
      {"+=",     PEQ,    INFIX,      RTL,    10},
      {""},
#line 28 "mapop.gperf"
      {"!",      NOT,    PREFIX,     RTL,    2},
#line 39 "mapop.gperf"
      {"!=",     NEQ,    INFIX,      LTR,    6},
#line 36 "mapop.gperf"
      {">",      GRT,    INFIX,      LTR,    5},
#line 37 "mapop.gperf"
      {">=",     GEQ,    INFIX,      LTR,    5},
      {""}, {""}, {""},
#line 34 "mapop.gperf"
      {"<",      LET,    INFIX,      LTR,    5},
#line 35 "mapop.gperf"
      {"<=",     LEQ,    INFIX,      LTR,    5},
      {""}, {""}, {""}, {""},
#line 26 "mapop.gperf"
      {"++",     INC,    POSTFIX,    LTR,    1},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 41 "mapop.gperf"
      {"||",     ORO,    INFIX,      LTR,    8},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 40 "mapop.gperf"
      {"&&",     AND,    INFIX,      LTR,    7}
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
