Load variables from a config file with ease!
See LICENSE file for copyright and license details.

Put your variable names in vars.h (in the list macros),
then call configure() with the name of the config file.

An example of how to use this system is provided (example.c).

To add this to your own C/C++ project, for each file using the vars:
    > #include vars.h
    
Then link your program with cfg: "gcc example.c cfg -o example"
                                       ▲         ▲
                                       |         |
                                       o----o----o
                                            |
               Linked ----------------------o
                                         

Build using the Makefile:
    $ make

See example.c and vars.h for more in-depth info.

Author:
    Leo Brown
