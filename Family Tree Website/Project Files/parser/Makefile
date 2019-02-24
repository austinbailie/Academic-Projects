CC=gcc
CFLAGS= -Wall -g -std=c11

SRC = ./src/
INC = ./include/
DIR = ../

#UNAME Shell Variable
UNAME_S := $(shell uname -s)

# OS X Dependant Variables
ifeq ($(UNAME_S), Darwin)
    LIB := sharedLib.dylib
# GNU/Linux Depedant Variables
else ifeq ($(UNAME_S), Linux)
    LIB := sharedLib.so
endif

$(LIB): sharedLib.o
	$(CC) -shared -o $(LIB) GEDCOMparser.o LinkedListAPI.o GEDCOMutilities.o -o $(DIR)$(LIB)

sharedLib.o: $(SRC)GEDCOMparser.c $(SRC)LinkedListAPI.c $(SRC)GEDCOMutilities.c
	$(CC) $(CFLAGS) -c -fpic $(SRC)GEDCOMparser.c $(SRC)LinkedListAPI.c $(SRC)GEDCOMutilities.c -Iinclude

clean:
	rm *.o $(DIR)$(LIB)