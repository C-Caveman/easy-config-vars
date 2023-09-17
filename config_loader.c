// Load values from a config file.
// See LICENSE file for copyright and license details.
#include "vars.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Init the global variables.
#undef f
#define f(x) int x = 0;
INT_VARS_LIST
#undef f
#define f(x) float x = 0;
FLOAT_VARS_LIST
#undef f
#define f(x) char x[STRING_VAR_SIZE];
STRING_VARS_LIST

// Get the global variable names.
#undef f
#define f(x) #x, 
char INT_VAR_NAMES[NUM_INT_VARS][VAR_NAME_SIZE] = { INT_VARS_LIST };
char FLOAT_VAR_NAMES[NUM_FLOAT_VARS][VAR_NAME_SIZE] = { FLOAT_VARS_LIST };
char STRING_VAR_NAMES[NUM_STRING_VARS][VAR_NAME_SIZE] = { STRING_VARS_LIST };


// Temporary storage for variable values loaded from file:
int INT_VARS[NUM_INT_VARS];
float FLOAT_VARS[NUM_INT_VARS];
char STRING_VARS[NUM_INT_VARS][STRING_VAR_SIZE];
// Mark which vars have been changed.
char INT_VARS_CHANGED[NUM_INT_VARS];
char FLOAT_VARS_CHANGED[NUM_FLOAT_VARS];
char STRING_VARS_CHANGED[NUM_STRING_VARS];

// Find a variable name in a name array. Return the index of than name. (-1 if not found)
int get_var_index(char* name, char names[][VAR_NAME_SIZE], int num_names) {
    int index = -1;
    for (int i=0; i<num_names; i++) {
        if (strncmp(name, names[i], VAR_NAME_SIZE) == 0)
            return i;
    }
    return index;
}

// Current data from file.
char c = 0;
char line[MAX_CONFIG_LINE_LEN];
char var_name[STRING_VAR_SIZE];
char var_value[STRING_VAR_SIZE];
int var_name_len = 0;
int var_value_len = 0;

// Digest a line from the config file.
void read_line(FILE* fp) {
    int len = 0;
    int pos = 0;
    var_name_len = 0;
    var_value_len = 0;
    memset(line, 0, MAX_CONFIG_LINE_LEN*sizeof(char));
    memset(var_name, 0, STRING_VAR_SIZE*sizeof(char));
    memset(var_value, 0, STRING_VAR_SIZE*sizeof(char));
    if (feof(fp))
        return;
    // Load the line.
    while (len<MAX_CONFIG_LINE_LEN) {
        c = fgetc(fp);
        if (feof(fp) || c == '\n')
            break;
        line[len] = c;
        len++;
    }
    if (line[0] == '#') // Skip the comment.
        return;
    // Trim whitespace:
    while (pos<len) {
        if (!isspace(line[pos]))
            break;
        pos++;
    }
    // Get the var_name:
    while (pos<len && var_name_len < STRING_VAR_SIZE) {
        if (line[pos] == '=' || isspace(line[pos]) || line[pos] == '#')
            break;
        var_name[var_name_len] = line[pos];
        var_name_len += 1;
        pos++;
    }
    // Trim whitespace:
    while (pos<len) {
        if (!isspace(line[pos]) && line[pos] != '=')
            break;
        pos++;
    }
    // Get the var_value:
    while (pos<len && var_value_len < STRING_VAR_SIZE) {
        if (line[pos] == '#')
            break;
        var_value[var_value_len] = line[pos];
        var_value_len += 1;
        pos++;
    }
}

// Load config variables from file.
void configure(char* fname) {
    FILE* fp = fopen(fname, "r");
    if (fp == 0) {
        printf("File %s not found!\n", fname);
        exit(-1);
    }
    // Mark all variables as unchanged by the config file.
    memset(INT_VARS_CHANGED, 0, NUM_INT_VARS);
    memset(FLOAT_VARS_CHANGED, 0, NUM_FLOAT_VARS);
    memset(STRING_VARS_CHANGED, 0, NUM_STRING_VARS);
    //
    // Read each line. Set the appropriate array index with the value:
    //
    // Index of variable associated with 'var_name'.
    int index = -1;
    while (1) {
        if (feof(fp))
            break;
        // Read the current line:
        read_line(fp);
        if (var_name_len == 0 || var_value_len == 0) // Invalid line.
            continue;
        if (DEBUG) printf("\nvar_name:  '%s'\nvar_value: '%s'\n", var_name, var_value);
        // Name of variable:
        // Search until we find a matching var name. Then goto GOT_MATCH.
        enum match_types {NO_MATCH, INT, FLOAT, STRING};
        int match_type = NO_MATCH;
        index = get_var_index(var_name, INT_VAR_NAMES, NUM_INT_VARS);
        if (index != -1) {match_type = INT; goto GOT_MATCH;}
        index = get_var_index(var_name, FLOAT_VAR_NAMES, NUM_FLOAT_VARS);
        if (index != -1) {match_type = FLOAT; goto GOT_MATCH;}
        index = get_var_index(var_name, STRING_VAR_NAMES, NUM_STRING_VARS);
        if (index != -1) {match_type = STRING; goto GOT_MATCH;}
        // If we get here, there was no match.
        if (DEBUG) printf("[var_name not found]\n");
        continue;
        // Got a match!
        GOT_MATCH:
        // Set the var value from the var_value string.
        switch(match_type) {
        case INT:
            INT_VARS[index] = atoi(var_value);
            INT_VARS_CHANGED[index] = 1;
            if (DEBUG) printf("[int] %d\n", INT_VARS[index]);
            break;
        case FLOAT:
            FLOAT_VARS[index] = atof(var_value);
            FLOAT_VARS_CHANGED[index] = 1;
            if (DEBUG) printf("[float]     %f\n", FLOAT_VARS[index]);
            break;
        case STRING:
            strncpy(STRING_VARS[index], var_value, STRING_VAR_SIZE);
            STRING_VARS_CHANGED[index] = 1;
            if (DEBUG) printf("[string]    %s\n", STRING_VARS[index]);
            break;
        }
        if (feof(fp))
            break;
    }
    fclose(fp);
    // Set the vars from the values now stored in the arrays.
    // Only set it if the config file changed it.
    #undef f
    #define f(x) if (INT_VARS_CHANGED[enum_##x]) x = INT_VARS[enum_##x];
    INT_VARS_LIST
    #undef f
    #define f(x) if (FLOAT_VARS_CHANGED[enum_##x]) x = FLOAT_VARS[enum_##x];
    FLOAT_VARS_LIST
    #undef f
    #define f(x) if (STRING_VARS_CHANGED[enum_##x]) strncpy(x, STRING_VARS[enum_##x], STRING_VAR_SIZE);
    STRING_VARS_LIST
}

// Print the names of each global variable.
void print_globals() {
    printf("o=====================================================o\n");
    printf("| Summary of Vars:                                    |\n");
    printf("o-----------------------------------------------------o\n");
    printf("| Integers                                            |\n");
    printf("o=====================================================o\n");
    #undef f
    #define f(x) printf("| %-32s = %-16d |\n", #x, x);
    INT_VARS_LIST
    printf("o-----------------------------------------------------o\n");
    printf("| Floats                                              |\n");
    printf("o=====================================================o\n");
    #undef f
    #define f(x) printf("| %-32s = %-16f |\n", #x, x);
    FLOAT_VARS_LIST
    printf("o-----------------------------------------------------o\n");
    printf("| Strings          (first 16 chars)                   |\n");
    printf("o=====================================================o\n");
    #undef f
    #define f(x) printf("| %-32s = %-16.*s |\n", #x, 16, x);
    STRING_VARS_LIST
    printf("o=====================================================o\n\n");
}

int main() {
    printf("\nBefore config:\n");
    print_globals();
    //
    configure("data/config_file.txt");
    //
    printf("\nAfter config:\n");
    print_globals();
    //
    configure("data/config_file_2.txt");
    //
    printf("\nAfter second config:\n");
    print_globals();
    printf("example_integer_var =  %d\n", example_integer_variable);
    printf("example_float_var   =  %f\n", example_float_variable);
    printf("example_string_var  =  '%s'\n", example_string_variable);
    return 0;
}
