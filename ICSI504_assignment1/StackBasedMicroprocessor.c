#include <stdio.h>
#include "Stack.h"
#include "ABMReader.h"
#include "ABMMethods.h"

int main()
{
	struct CharStack stack;
	initialize(&stack);		
	FILE* abmFile = abmFileReader("abmfiles/operatorsTest.abm");
	abminstructionrunner(abmFile, &stack);

	return 0;
}
