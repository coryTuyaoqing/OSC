all:
	gcc -g sbuffer.c main.c -o main.out -lpthread

alldebug: all
	./main.out

gendebug: config.h file_creator.c
	gcc -g file_creator.c config.h -o gendebug.out -DDEBUG
	./gendebug.out

zip:
	zip milestone3.zip *.c *.h Makefile
