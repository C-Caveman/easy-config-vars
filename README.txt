Load variables from a config file with ease!
See LICENSE file for copyright and license details.

1: Put your variable names in vars.h
2: Include vars.h in your C program.
3: Apply a config file with: configure("my_config_file.cfg")

An example of how to use this system is provided (example.c).


Generate vars.o in your Makefile with this rule:
vars.o: vars.c vars.h
	gcc -c vars.c -o vars.o

Link your program with vars.o:   "gcc my_program.c vars.o -o example"
                                           ▲         ▲
                                           │         │
                                           ╰────┬────╯
                                                │
               Linked ──────────────────────────╯


Build the example program:
    $ make

See example.c and vars.h for more in-depth info.

Author:
    Leo Brown
