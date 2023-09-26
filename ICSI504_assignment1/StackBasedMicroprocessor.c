#include <stdio.h>
#include "Stack.h"
#include "ABMReader.h"
#include "ABMMethods.h"

int main()
{
	FILE* abmFile = abmFileReader("abmfiles/operatorsTest.abm");
	abminstructionrunner(abmFile);
	return 0;
}
