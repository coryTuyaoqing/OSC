# Makefile for debug_challenge
# author: TD

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -g

all: debugchallenge
	./debugchallenge.o

clean:
	rm -f debugchallenge.o

debugchallenge: cstring.c
	$(CC) $(CFLAGS) -o debugchallenge.o cstring.c
	#./debugchallenge.o

memtest: debugchallenge
	valgrind --leak-check=yes ./debugchallenge.o
	#./debugchallenge.o

# launch with CLION target executable configured or uncomment line
