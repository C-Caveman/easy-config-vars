# A config file system.

# An example of how to use the config system.
example_use: vars
	gcc example.c cfg -o example

# The config loading system.
vars: config_loader.c vars.h
	gcc -c config_loader.c -o cfg




clean:
	rm *.o cfg example
