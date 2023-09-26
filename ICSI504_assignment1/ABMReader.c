#include <stdio.h>
#include "ABMReader.h"

FILE* abmFileReader(const char* filename){
	FILE* file = fopen(filename, "r");
	if(file == NULL)
	{
		perror("error opening the abm file please check and input again.");
		return NULL;
	}
	return file;
}


void PrintabmFile(FILE* abmFile){
	if(abmFile == NULL)
	{
		 perror("error opening the abm file please check and input again.");
                return;
	}
	char buffer[1024];
	
	while(fgets(buffer, sizeof(buffer), abmFile) != NULL)
	{
		printf("%s", buffer);
	}
}
