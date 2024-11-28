
all: main.c datamgr.c datamgr.h config.h lib/dplist.c lib/dplist.h
	gcc -g main.c datamgr.c lib/dplist.c lib/dplist.h -o main.out

gen: config.h file_creator.c 
	gcc -g file_creator.c config.h -o gen.out
	./gen.out

gendebug: config.h file_creator.c
	gcc -g file_creator.c config.h -o gendebug.out -DDEBUG
	./gendebug.out

debug: main.out
	gdb ./main.out

clean:
	rm *.out room_sensor.map sensor_data sensor_data_text
