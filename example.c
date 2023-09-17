// Example usage of vars.h
#include "vars.h"
#include "stdio.h"

int main() {
    // You can use all vars in vars.h by name.
    example_integer_variable = 111;
    printf("\nBefore loading config file: %d\n", example_integer_variable);
    // You can set any of these vars with a config file.
    // (only values set in the config file get updated)
    configure("data/config_file.txt");
    printf("After loading config file: %d\n\n", example_integer_variable);
    // You can print a summary of the vars at any time.
    print_vars();

    // Here are some other variables set by data/config_file.txt
    printf("example_float_var   =  %f\n", example_float_variable);
    printf("example_string_var  =  '%s'\n", example_string_variable);
    printf("lima_bean = '%s'\n", lima_bean);
    return 0;
}
