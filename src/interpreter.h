#include <stdio.h>

// TODO: define an interpreter environment
typedef struct {

} environment;

// define interpreter methods
void f_interpret(environment *memory, FILE *stream);
void interpret(environment *memory, char *text);