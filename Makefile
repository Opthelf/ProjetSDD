all : exo3.o exo2.o main.o exo1.o
	gcc exo3.o exo2.o main.o exo1.o -o main
	
main.o :  main.c exo3.h exo2.h exo1.h
	gcc -c main.c -o main.o

exo1.o : exo1.c exo1.h
	gcc -c exo1.c -o exo1.o
	
exo2.o : exo2.c exo2.h exo1.h
	gcc -c exo2.c -o exo2.o

exo3.o : exo3.c exo1.h exo2.h exo3.h
	gcc -c exo3.c -o exo3.o

clean :
	rm -f *.o all ; rm main ; rm -f *.txt all

