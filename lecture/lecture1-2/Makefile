all: ex1main.o ex1support.o
	gcc -g -o myprogram ex1main.o ex1support.o
	
ex1main.o: ex1main.c
	gcc -g -c ex1main.c -o ex1main.o

ex1support.o: ex1support.c
	gcc -g -c ex1support.c -o ex1support.o
	
clean:
	rm *.o
	rm myprogram


ex2: ex2typesizes.c
	gcc -g -o ex2 ex2typesizes.c

ex3: ex3array.c
	gcc -g -o ex3 ex3array.c

ex4: ex4ptrarithm.c
	gcc -g -o ex4 ex4ptrarithm.c

ex5: ex5ptrnull.c ex5quiza.c ex5quizb.c
	gcc -g -o ex5p ex5ptrnull.c
	gcc -g -o ex5a ex5quiza.c
	gcc -g -o ex5b ex5quizb.c

ex6: ex6mallocfree.c
	gcc -g -o ex6 ex6mallocfree.c

ex7: ex7typedefstruct.c
	gcc -g -o ex7 ex7typedefstruct.c