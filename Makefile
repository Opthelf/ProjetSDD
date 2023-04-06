all : exo6.o exo5.o exo4.o exo3.o exo2.o exo1.o main.o
	gcc exo6.o exo5.o exo4.o exo3.o exo2.o exo1.o main.o -o main
	
main.o :  main.c exo5.h exo4.h exo3.h exo2.h exo1.h
	gcc -c main.c -o main.o

exo1.o : exo1.c exo1.h
	gcc -c exo1.c -o exo1.o
	
exo2.o : exo2.c exo2.h exo1.h
	gcc -c exo2.c -o exo2.o

exo3.o : exo3.c exo1.h exo2.h exo3.h
	gcc -c exo3.c -o exo3.o

exo4.o : exo4.c exo1.h exo2.h exo3.h exo4.h
	gcc -c exo4.c -o exo4.o

exo5.o : exo5.c exo1.h exo2.h exo3.h exo4.h exo5.h
	gcc -c exo5.c -o exo5.o

exo6.o : exo6.c exo1.h exo2.h exo3.h exo4.h exo5.h exo6.h
	gcc -c exo6.c -o exo6.o
clean :
	rm -f *.o all ; rm main 

