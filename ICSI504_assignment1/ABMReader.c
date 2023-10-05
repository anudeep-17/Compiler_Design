#include <stdio.h>
#include "ABMReader.h"

//abmFileReader reads a given file and returns FILE*
FILE* abmFileReader(const char* filename){
	FILE* file = fopen(filename, "r"); //opens the given file
	if(file == NULL)
	{
		perror("error opening the abm file please check and input again.");
		return NULL;
	}
	return file; //return file
}


//used to print a file
void PrintabmFile(FILE* abmFile)
{
	if(abmFile == NULL)
	{
		 perror("error opening the abm file please check and input again.");
                return;
	}
	char buffer[1024];//a buffer to print the file

	while(fgets(buffer, sizeof(buffer), abmFile) != NULL)
	{
		printf("%s", buffer); //reads file line by line and prints the values
	}
}
