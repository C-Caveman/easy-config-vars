// Load values from a config file.
// See LICENSE file for copyright and license details.
#include "vars.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//=================================================================// ALLOCATE ALL VARS //
#define expand(x) int x = 0; //--------------- Allocate integers.
INT_VARS_LIST
#undef expand
#define expand(x) float x = 0; //------------- Allocate floats.
FLOAT_VARS_LIST
#undef expand
#define expand(x) char x[STRING_VAR_SIZE]; //- Allocate strings.
STRING_VARS_LIST
#undef expand
//================================================// EXPAND VAR NAMES AS STRING ARRAYS //
#define expand(x) #x, 
char INT_VAR_NAMES[NUM_INT_VARS][VAR_NAME_SIZE] = { INT_VARS_LIST };
char FLOAT_VAR_NAMES[NUM_FLOAT_VARS][VAR_NAME_SIZE] = { FLOAT_VARS_LIST };
char STRING_VAR_NAMES[NUM_STRING_VARS][VAR_NAME_SIZE] = { STRING_VARS_LIST };
#undef expand
//=======================================// Temporary storage for variable values loaded from file. //
int INT_VARS[NUM_INT_VARS];
float FLOAT_VARS[NUM_FLOAT_VARS];
char STRING_VARS[NUM_STRING_VARS][STRING_VAR_SIZE];
// Mark which vars have been changed.
char INT_VARS_CHANGED[NUM_INT_VARS];
char FLOAT_VARS_CHANGED[NUM_FLOAT_VARS];
char STRING_VARS_CHANGED[NUM_STRING_VARS];
//=============================================================================// GET VAR INDEX BY NAME //
int get_var_index(char* name, char names[][VAR_NAME_SIZE], int num_names) { //--- (returns -1 on fail)
    int index = -1;
    for (int i=0; i<num_names; i++) { if (strncmp(name, names[i], VAR_NAME_SIZE) == 0) { index = i; break; } }
    return index;
}
char c = 0; //============================================// Current line. //
char line[MAX_CONFIG_LINE_LEN];
char var_name[STRING_VAR_SIZE];
char var_value[STRING_VAR_SIZE];
int var_name_len = 0;
int var_value_len = 0;
//========================================================// Digest a line from the config file. //
void read_line(FILE* fp) {
    int len = 0;
    int pos = 0;
    var_name_len = 0;
    var_value_len = 0;
    memset(line, 0, MAX_CONFIG_LINE_LEN*sizeof(char));
    memset(var_name, 0, STRING_VAR_SIZE*sizeof(char));
    memset(var_value, 0, STRING_VAR_SIZE*sizeof(char));
    if (feof(fp)) { return; }
    while (len<MAX_CONFIG_LINE_LEN) { //---------------------------- Load the line.
        c = fgetc(fp);
        if (feof(fp) || c == '\n') { break; }
        line[len] = c;
        len++;
    }
    if (line[0] == '#') { return; } // Skip the comment.
    while (pos<len) { //-------------------------------------------- Trim whitespace.
        if (!isspace(line[pos])) { break; }
        pos++;
    }
    while (pos<len && var_name_len < STRING_VAR_SIZE) { //---------- Get the var_name.
        if (line[pos] == '=' || isspace(line[pos]) || line[pos] == '#') { break; }
        var_name[var_name_len] = line[pos];
        var_name_len += 1;
        pos++;
    }
    while (pos<len) { //-------------------------------------------- Trim whitespace.
        if (!isspace(line[pos]) && line[pos] != '=') { break; }
        pos++;
    }
    while (pos<len && var_value_len < STRING_VAR_SIZE) { //--------- Get the var_value.
        if (line[pos] == '#') { break; }
        var_value[var_value_len] = line[pos];
        var_value_len += 1;
        pos++;
    }
}
void configure(char* fname) { //===============================================// Load config variables from file. //
    FILE* fp = fopen(fname, "r");
    if (fp == 0) { printf("File %s not found!\n", fname); exit(-1); }
    memset(INT_VARS_CHANGED, 0, NUM_INT_VARS*sizeof(char)); //---------- Mark all variables as unchanged by the config file.
    memset(FLOAT_VARS_CHANGED, 0, NUM_FLOAT_VARS*sizeof(char));
    memset(STRING_VARS_CHANGED, 0, NUM_STRING_VARS*sizeof(char));
    int index = -1; //--------------------------------------------------------------- Iterate through each line.
    while (1) {
        if (feof(fp)) { break; }
        read_line(fp); //------------------------------------------------------------ Get first word of line.
        if (var_name_len == 0) { continue; } // Invalid line.
        if (DEBUG) printf("\nvar_name:  '%s'\nvar_value: '%s'\n", var_name, var_value);
        enum match_types {NO_MATCH, INT, FLOAT, STRING}; //-------------------------- Find the variable by name.
        int match_type = NO_MATCH;
        index = get_var_index(var_name, INT_VAR_NAMES, NUM_INT_VARS);
        if (index != -1) { match_type = INT; goto GOT_MATCH; }
        index = get_var_index(var_name, FLOAT_VAR_NAMES, NUM_FLOAT_VARS);
        if (index != -1) { match_type = FLOAT; goto GOT_MATCH; }
        index = get_var_index(var_name, STRING_VAR_NAMES, NUM_STRING_VARS);
        if (index != -1) { match_type = STRING; goto GOT_MATCH; }
        if (DEBUG) { printf("[var_name not found]\n"); }
        continue;
        GOT_MATCH: //------------------------------------- GOT_MATCH
        switch(match_type) { //=======================================// STORE THE VALUE IN THE ARRAY //
        case INT:
            INT_VARS[index] = atoi(var_value); //------------------------- Store int.
            INT_VARS_CHANGED[index] = 1;
            if (DEBUG) { printf("[int] %d\n", INT_VARS[index]); }
            break;
        case FLOAT:
            FLOAT_VARS[index] = atof(var_value); //----------------------- Store float.
            FLOAT_VARS_CHANGED[index] = 1;
            if (DEBUG) { printf("[float]     %f\n", FLOAT_VARS[index]); }
            break;
        case STRING:
            memcpy(STRING_VARS[index], var_value, STRING_VAR_SIZE); //---- Store string.
            STRING_VARS_CHANGED[index] = 1;
            if (DEBUG) { printf("[string]    %s\n", STRING_VARS[index]); }
            break;
        }
        if (feof(fp)) { break; }
    }
    fclose(fp); //===================================================================// SET THE VARS THAT CHANGED //
    #undef expand
    #define expand(x) if (INT_VARS_CHANGED[enum_##x]) x = INT_VARS[enum_##x]; //------ Set ints from the ints array.
    INT_VARS_LIST
    #undef expand
    #define expand(x) if (FLOAT_VARS_CHANGED[enum_##x]) x = FLOAT_VARS[enum_##x]; //-- Set floats from the floats array.
    FLOAT_VARS_LIST
    #undef expand //------------------------------------------------------------------ Set strings from the strings array.
    #define expand(x) if (STRING_VARS_CHANGED[enum_##x]) strncpy(x, STRING_VARS[enum_##x], STRING_VAR_SIZE);
    STRING_VARS_LIST
}
void print_vars() { //=======================================// Print the names of each variable. //
    printf("o=====================================================o\n");
    printf("| Summary of Vars:                                    |\n");
    printf("o-----------------------------------------------------o\n");
    printf("| Integers                                            |\n");
    printf("o=====================================================o\n");
    #undef expand
    #define expand(x) printf("| %-32s = %-16d |\n", #x, x);
    INT_VARS_LIST
    printf("o-----------------------------------------------------o\n");
    printf("| Floats                                              |\n");
    printf("o=====================================================o\n");
    #undef expand
    #define expand(x) printf("| %-32s = %-16f |\n", #x, x);
    FLOAT_VARS_LIST
    printf("o-----------------------------------------------------o\n");
    printf("| Strings          (first 16 chars)                   |\n");
    printf("o=====================================================o\n");
    #undef expand
    #define expand(x) printf("| %-32s = %-16.*s |\n", #x, 16, x);
    STRING_VARS_LIST
    printf("o=====================================================o\n\n");
}
