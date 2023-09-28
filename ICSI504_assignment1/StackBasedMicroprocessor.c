#include <stdio.h>
#include "Stack.h"
#include "ABMReader.h"
#include "ABMMethods.h"
#include "addresstovaluedict.h"
#include "Variablearray.h"

int main()
{
	struct CharStack stack;
	initialize(&stack);		
	//FILE* abmFile = abmFileReader("abmfiles/operatorsTest.abm");
	//abminstructionrunner(abmFile, &stack);

struct Stringarray array;
initializeStrings(&array);

struct Map map;
initializeMap(&map);
	return 0;
}
