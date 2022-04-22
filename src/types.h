/*
* This file declares all different value types and provides simple type conversions
* v_ prefixes any type defined in this file
*/

// Enum of different types
typedef enum {
    STRING,
    NUMBER,
    OBJECT,
    EMPTY, // also 'null'
    UNDEFINED,
} v_type;

// Define different types
typedef struct {
    int num;
} v_number;

typedef struct {
    char* content;
} v_string;

typedef struct {
    char* name;
    v_type type;
    void *val; // NULL if type is EMPTY or UNDEFINED
} v_field;

typedef struct {
    int n_fields;
    v_field** fields; // pointer array, allocated one at a time
} v_object;