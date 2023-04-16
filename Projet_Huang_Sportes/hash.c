#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdbool.h>
#include "List.h"
#include "WorkTree.h"
#include "branch.h"
#include "Commit.h"
#include "Gestion_file.h"
#include "hash.h"
#include "merge.h"

//Ecrit le hash du fichier source dans le fichier destination
int hashFile(char * source, char * dest){
	char buff[1000];

    //On copie le hash du fichier dans un fichier intermédiaire
	sprintf(buff,"cat %s | sha256sum > resultat.txt",source);	
	system(buff);

    //On ouvre ce fichier en mode lecture
	FILE * temp = fopen("resultat.txt","r");

    //On récupère le chaine de caractère
	fgets(buff,100,temp);

    //On enlève le tiret et le \n de la chaine
	buff[strlen(buff)-4] = '\0';

    //On ouvre le fichier destination en mode écriture
	FILE * d = fopen(dest,"w");

    //On met la chaine dedans
	fputs(buff,d);
	system("rm resultat.txt");

    //On ferme les fichiers
	fclose(temp);
	fclose(d);
	return 0;
}

//Renvoie le hash du fichier 
char * sha256file(char * file){

	FILE * ftest;

    //Cas où le fichier n'existe pas
	if ( ( ftest = fopen(file,"r")) == NULL){
		printf("Le fichier n'existe pas !(sha256file)\n");
		exit(EXIT_FAILURE);
	}
	else{
		fclose(ftest);
	}

	
	//Fichier temporaire
	char static fname[1000];
	sprintf(fname,"%sXXXXXX",file); //sans le /tmp/ à la maison et avec /tmp/ au début de la chaine à la fac
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

    //On ferme le fichier
	fclose(f);
	
	return hash;
}

//Renvoie le hash sous forme de path (un / entre le deuxième et le troisème caractère)
char * hashToPath(char *hash){

    //On vérifie que le hash n'est pas NULL, auquel cas il y a eu un souci avant donc on exit complètement
    if (hash == NULL){
        printf("Le hash est NULL -> hashToPath\n");
        exit(EXIT_FAILURE);
    }

	int l = strlen(hash);
	char * path = malloc(sizeof(char)*(l+10));
    strcpy(path,"");
	path[0] = hash[0];
	path[1] = hash[1];
	path[2] = '/';
	
	int i;
	for( i = 3;i<=l;i++){
		path[i] = hash[i-1];
	}
	path[i]='\0';

    //On récupère le path du hash
	return path;
}

//Crée le dossier avec les deux premiers caractères du hash, puis retourne le path
char * hashToFile(char *hash){
    char * ch2 = strdup(hash);

    //On dit que le chaine s'arrête là
    ch2[2]='\0';
    struct stat st = {0};

    //Si le dossier n'existe pas déjà, on le crée avec tout les droits d'utilisateurs et aucuns pour les autres
    if(stat(ch2,&st) == -1){
        mkdir(ch2,0700);
    }

    //On libère la mémoire allouée
    free(ch2);

    //On retourne le path du hash
    return hashToPath(hash);
}

//Transforme le hash d'un commit en path vers son instantané
char * hashToPathCommit(char * hash){

    //On transforme le hash en path
    char * buff = hashToPath(hash);
    buff = realloc(buff, sizeof(char) * (strlen(buff)+3) );

    //On ajoute le .c des instantanés de commit
    strcat(buff,".c");
    strcat(buff,"\0");
    return buff;
}

//Transforme un hash en path
char * hashToPathWorkTree(char * hash){

    //On transforme le hash en path
    char * buff = hashToPath(hash);
    buff = realloc(buff, sizeof(char) * (strlen(buff)+3) );

    //On ajoute le .t des instantanés de WorkTree
    strcat(buff,".t");
    strcat(buff,"\0");
    return buff;
}

//Hash le contenu
unsigned long hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}