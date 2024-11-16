all: hellomain.c hellofunc.c hellofunc.h
	gcc -g -o hello hellomain.c hellofunc.c

stepwise: hellomain.o hellofunc.o hellofunc.h
	gcc -g -o hello hellomain.o hellofunc.o

hellomain.o: hellomain.c
	gcc -g -c hellomain.c

hellofunc.o: hellofunc.c hellofunc.h
	gcc -g -c hellofunc.c

clean:
	rm -rf *.o *.out *.gch hello
