#include <stdio.h>
#include "ABMReader.h"
#include "ABMMethods.h"

//Main FILE thats runs all the abm files given to produce output.
int main(int argc, char* argv[])
{
		if(argv[1] != NULL)
		{
			FILE* abmFile = abmFileReader(argv[1]);
			abminstructionrunner(abmFile);
		}
		else
		{
			printf("NO FILE NAME EVER ENTERED\n");
		}

	return 0; //just to exit
}
