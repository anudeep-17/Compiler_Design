#include <stdio.h>
#include "ABMReader.h"
#include "ABMMethods.h"
#include "Variablearray.h"
//Main FILE thats runs all the abm files given to produce output.
int main()
{
		FILE* abmFile = abmFileReader("abmfiles/demo.abm");
		abminstructionrunner(abmFile);
	return 0; //just to exit
}
