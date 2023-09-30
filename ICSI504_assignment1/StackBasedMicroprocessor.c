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

	struct Stringarray variables;
	initializeStrings(&variables);

	struct Map variablevaluemap;
	initializeMap(&variablevaluemap);

	FILE* abmFile = abmFileReader("abmfiles/foo.abm");

	abminstructionrunner(abmFile, &stack, &variables, &variablevaluemap);
	return 0;
}
