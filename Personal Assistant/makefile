CC = gcc
CFLAGS = -Wall -g -std=c99

BIN = ./bin/
SRC = ./src/

program:
	$(CC) $(CFLAGS) $(SRC)main.c $(SRC)binarySearchTree.c $(SRC)rules.c -Iinclude -o $(BIN)program

test:
	$(CC) $(CFLAGS) $(SRC)testMain.c $(SRC)binarySearchTree.c $(SRC)rules.c -Iinclude -o $(BIN)test
	
cls:
	clear

clean:
	rm ./bin/*
