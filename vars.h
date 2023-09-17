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
    f(example_integer_variable) \
    f(another_integer) \
    f(yet_another_int) \
    f(and_another_one) \
    f(thirty_characters_is_the_maximum) \
    f(aaaand_another_integer)

#define FLOAT_VARS_LIST \
    f(example_float_variable) \
    f(floaty_mc_floatpants) \
    f(floaty_flops)

#define STRING_VARS_LIST \
    f(example_string_variable) \
    f(string_bean) \
    f(billy_jean) \
    f(lima_bean)

// Make the enums. (each enum listing has 'enum_' in front of it)
#undef f
#define f(x) enum_##x,
enum { INT_VARS_LIST NUM_INT_VARS };
enum { FLOAT_VARS_LIST NUM_FLOAT_VARS };
enum { STRING_VARS_LIST NUM_STRING_VARS };

// Expose all global vars with extern (definitions done config_loader.c).
#undef f
#define f(x) extern int x;
INT_VARS_LIST
#undef f
#define f(x) extern float x;
FLOAT_VARS_LIST
#undef f
#define f(x) extern char x [256];
STRING_VARS_LIST
