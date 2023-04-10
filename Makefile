all : exo7.o exo6.o exo5.o exo4.o exo3.o exo2.o exo1.o main.o myGit.o
	gcc exo7.o exo6.o exo5.o exo4.o exo3.o exo2.o exo1.o main.o -o main 
	gcc exo7.o exo6.o exo5.o exo4.o exo3.o exo2.o exo1.o myGit.o -o myGit

myGit.o :  myGit.c exo7.h exo6.h exo5.h exo4.h exo3.h exo2.h exo1.h
	gcc -c myGit.c -o myGit.o

main.o :  main.c exo7.h exo6.h exo5.h exo4.h exo3.h exo2.h exo1.h
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
exo7.o : exo7.c exo1.h exo2.h exo3.h exo4.h exo5.h exo6.h exo7.h
	gcc -c exo7.c -o exo7.o
clean :
	rm -f *.o all ; rm main ; rm myGit

