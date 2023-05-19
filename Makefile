all: exec

main.o: ../main.c include/header.h include/macro.h
	gcc -c main.c -o main.o

usual.o: ../usual.c include/header.h include/macro.h
	gcc -c usual.c -o usual.o

playerturn.o: playerturn.c include/header.h include/macro.h
	gcc -c playerturn.c -o playerturn.o

init.o: init.c include/header.h include/macro.h
	gcc -c init.c -o init.o

exec: main.o init.o usual.o playerturn.o
	gcc main.o init.o usual.o playerturn.o -o exec

clean:
	rm -f *.o
	rm exec