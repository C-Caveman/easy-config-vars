// Variables you can set via config file(s).
//
// See LICENSE file for copyright and license details.

#define VAR_NAME_SIZE 32
#define STRING_VAR_SIZE 256
#define MAX_CONFIG_LINE_LEN 512
// Print detailed info about each line of the config file read.
#define DEBUG 0

// Set variables from a file.
void configure(char* fname);
// Print all the variables and their values.
void print_vars();

// List of variables!
#define INT_VARS_LIST \
    expand(example_integer_variable) \
    expand(another_integer) \
    expand(thirty_characters_is_the_maximum)

#define FLOAT_VARS_LIST \
    expand(example_float_variable) \
    expand(floaty_mc_floatpants) \
    expand(floaty_flops)

#define STRING_VARS_LIST \
    expand(example_string_variable) \
    expand(string_bean) \
    expand(billy_jean) \
    expand(lima_bean)

// Make the enums. (each enum listing has 'enum_' in front of it)

#define expand(name) enum_##name,
enum { INT_VARS_LIST NUM_INT_VARS };
enum { FLOAT_VARS_LIST NUM_FLOAT_VARS };
enum { STRING_VARS_LIST NUM_STRING_VARS };
#undef expand

// Expose all global vars with extern (definitions done config_loader.c).
#define expand(name) extern int name;
INT_VARS_LIST
#undef expand

#define expand(name) extern float name;
FLOAT_VARS_LIST
#undef expand

#define expand(x) extern char x [STRING_VAR_SIZE];
STRING_VARS_LIST
#undef expand
