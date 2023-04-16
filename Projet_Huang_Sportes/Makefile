all : hash.o List.o Gestion_file.o WorkTree.o Commit.o branch.o merge.o main.o myGit.o
	gcc -g hash.o List.o Gestion_file.o WorkTree.o Commit.o branch.o merge.o main.o -o main 
	gcc -g hash.o List.o Gestion_file.o WorkTree.o Commit.o branch.o merge.o myGit.o -o myGit

myGit.o : myGit.c List.h Gestion_file.h WorkTree.h Commit.h branch.h merge.h hash.h
	gcc -g -c  myGit.c -o myGit.o

main.o :  main.c List.h Gestion_file.h WorkTree.h Commit.h branch.h merge.h hash.h
	gcc -g -c main.c -o main.o

List.o : List.c List.h Gestion_file.h WorkTree.h Commit.h branch.h merge.h hash.h
	gcc -g -c List.c -o List.o
	
Gestion_file.o : Gestion_file.c List.h Gestion_file.h WorkTree.h Commit.h branch.h merge.h hash.h
	gcc -g -c Gestion_file.c -o Gestion_file.o

WorkTree.o : WorkTree.c List.h Gestion_file.h WorkTree.h Commit.h branch.h merge.h hash.h
	gcc -g -c WorkTree.c -o WorkTree.o

Commit.o : Commit.c List.h Gestion_file.h WorkTree.h Commit.h branch.h merge.h hash.h
	gcc -g -c Commit.c -o Commit.o

branch.o : branch.c List.h Gestion_file.h WorkTree.h Commit.h branch.h merge.h hash.h
	gcc -g -c branch.c -o branch.o

merge.o : merge.c List.h Gestion_file.h WorkTree.h Commit.h branch.h merge.h hash.h
	gcc -g -c merge.c -o merge.o

hash.o : hash.c List.h Gestion_file.h WorkTree.h Commit.h branch.h merge.h hash.h
	gcc -g -c hash.c -o hash.o

clean :
	rm -f *.o all ; rm main ; rm myGit

