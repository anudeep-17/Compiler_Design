#include <stdio.h>
#include "ABMMethods.h"
#include <string.h>

void abminstructionrunner(FILE* abminstructionfile)
{
	
	if (abminstructionfile == NULL) 
	{
        	perror("Error opening file");
        	return;
    	}

	char Eachline[1024];
 	char *firstspace;

	while (fgets(Eachline, sizeof(Eachline), abminstructionfile) != NULL) 
	{
		firstspace = strchr(Eachline, ' ');
		
		if(firstspace != NULL)
		{
			*firstspace = '\0';
			printf("before space %s\n", Eachline);
               		printf("after space %s\n", firstspace + 1);
		}
		else 
		{
			printf("before space %s\n", Eachline);
		}	
	}
}

void abmkeywordhelper(char* keyword, char* command)
{
	
}
