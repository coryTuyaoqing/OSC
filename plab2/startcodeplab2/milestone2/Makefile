all: main.c sensor_db.h sensor_db.c logger.h logger.c
	gcc -g -Wall main.c sensor_db.c logger.c -o main.out

run: all
	./main.out

zip:
	zip milestone2.zip *.c *.h Makefile

debug: all
	gdb main.out

rerun: all
	rm gateway.log sensor_db.csv
	./main.out
	cat gateway.log sensor_db.csv
