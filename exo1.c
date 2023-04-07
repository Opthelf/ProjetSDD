#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int hashFile(char * source, char * dest){
	char buff[1000];
	sprintf(buff,"cat %s | sha256sum > resultat.txt",source);	
	system(buff);
	FILE * temp = fopen("resultat.txt","r");
	fgets(buff,100,temp);
	buff[strlen(buff)-4] = '\0';
	FILE * d = fopen(dest,"w");
	fputs(buff,d);
	system("rm resultat.txt");
	fclose(temp);
	fclose(d);
	return 0;
}

char * sha256file(char * file){
	//Cas où le fichier n'existe pas
	FILE * ftest;
	if ( ( ftest = fopen(file,"r")) == NULL){
		fclose(ftest);
		printf("Le fichier n'existe pas !(sha256file)\n");
		return NULL;
	}
	fclose(ftest);
	
	//Fichier temporaire
	char static fname[1000];
	sprintf(fname,"%sXXXXXX",file); //sans le /tmp/ à la maison et avec /tmp/ au début de la chaine à la fac j'imagine
	int fd = mkstemp(fname);
	
	//On hashe le fichier et on met ce hashage dans le fichier temporaire
	hashFile(file,fname);
	
	//On récupère le hash
	FILE* f = fopen(fname,"r");
	char * hash = malloc(1000*sizeof(char));
	fgets(hash,1000,f);

	//On supprime le fichier temporaire
	char remove[1000] = "rm ";
	strcat(remove,fname);
	system(remove);
	
	return hash;
}
