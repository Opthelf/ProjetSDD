#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int hashFile(char * source, char * dest){
	char buff[50];
	sprintf(buff,"cat %s | sha256sum > %s",source,dest);
	system(buff);
	return 0;
}

char * sha256file(char * file){
	char static buff[1000];
	sprintf(buff,"/tmp/%sXXXXXX",file); //sans le /tmp/ à la maison.
	char fname[1000];
	strcpy(fname,buff);
	int fd = mkstemp(fname);
	hashFile(file,fname);
	FILE* f=fopen(fname,"r");
	char * res = malloc(1000*sizeof(char));
	res =fgets(buff,1000,f);
	res[strlen(res)-2] = ' ';
	return (res);
}
