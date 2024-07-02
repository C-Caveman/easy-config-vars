# A config file system.

# An example of how to use the config system.
example_use: vars.o
	gcc example.c vars.o -o example

# The config loading system.
vars.o: vars.c vars.h
	gcc -c vars.c -o vars.o




clean:
	rm *.o example

test: example_use		# Verify the example program's output is the same.
	bash ./testing/test.sh
