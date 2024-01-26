// Example usage of vars.h
#include "vars.h"
#include "stdio.h"

int main() {
    example_integer_variable = 111; //=======================================// The vars are already allocated in vars.c //

    printf("\nBefore loading config file: %d\n", example_integer_variable);
    configure("data/config_file.txt"); //====================================// Set vars from a config file. //
    printf("After loading config file: %d\n\n", example_integer_variable);

    printf("example_float_var   =  %f\n", example_float_variable); //========// You can floats and strings too! //
    printf("example_string_var  =  '%s'\n", example_string_variable);

    print_vars(); //=========================================================// Print a summary of the vars at any time. //

    return 0;
}
