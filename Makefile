all : exo11.o exo10.o exo9.o exo8.o exo7.o exo6.o exo5.o exo4.o exo3.o exo2.o exo1.o main.o myGit.o
	gcc -g exo11.o exo10.o exo9.o exo8.o exo7.o exo6.o exo5.o exo4.o exo3.o exo2.o exo1.o main.o -o main 
	gcc -g exo11.o exo10.o exo9.o exo8.o exo7.o exo6.o exo5.o exo4.o exo3.o exo2.o exo1.o myGit.o -o myGit

myGit.o : myGit.c exo11.h exo10.h exo9.h exo8.h exo7.h exo6.h exo5.h exo4.h exo3.h exo2.h exo1.h
	gcc -g -c  myGit.c -o myGit.o

main.o :  main.c exo11.h exo10.h exo9.h exo8.h exo7.h exo6.h exo5.h exo4.h exo3.h exo2.h exo1.h
	gcc -g -c main.c -o main.o

exo1.o : exo1.c exo1.h
	gcc -g -c exo1.c -o exo1.o
	
exo2.o : exo2.c exo2.h exo1.h
	gcc -g -c exo2.c -o exo2.o

exo3.o : exo3.c exo1.h exo2.h exo3.h
	gcc -g -c exo3.c -o exo3.o

exo4.o : exo4.c exo1.h exo2.h exo3.h exo4.h
	gcc -g -c exo4.c -o exo4.o

exo5.o : exo5.c exo1.h exo2.h exo3.h exo4.h exo5.h
	gcc -g -c exo5.c -o exo5.o

exo6.o : exo6.c exo1.h exo2.h exo3.h exo4.h exo5.h exo6.h
	gcc -g -c exo6.c -o exo6.o

exo7.o : exo7.c exo1.h exo2.h exo3.h exo4.h exo5.h exo6.h exo7.h
	gcc -g -c exo7.c -o exo7.o

exo8.o : exo8.c exo1.h exo2.h exo3.h exo4.h exo5.h exo6.h exo7.h exo8.h
	gcc -g -c exo8.c -o exo8.o

exo9.o : exo9.c exo1.h exo2.h exo3.h exo4.h exo5.h exo6.h exo7.h exo8.h exo9.h
	gcc -g -c exo9.c -o exo9.o

exo10.o : exo10.c exo1.h exo2.h exo3.h exo4.h exo5.h exo6.h exo7.h exo8.h exo9.h exo10.h
	gcc -g -c exo10.c -o exo10.o

exo11.o : exo11.c exo1.h exo2.h exo3.h exo4.h exo5.h exo6.h exo7.h exo8.h exo9.h exo10.h exo11.h
	gcc -g -c exo11.c -o exo11.o

clean :
	rm -f *.o all ; rm main ; rm myGit

