# A config file system.

all: config_loader.c vars.h
	gcc config_loader.c -o cfg




clean:
	rm *.o cfg
