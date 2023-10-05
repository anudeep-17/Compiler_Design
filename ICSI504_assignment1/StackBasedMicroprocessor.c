#include <stdio.h>
#include "Stack.h"
#include "ABMReader.h"
#include "ABMMethods.h"
#include "addresstovaluedict.h"
#include "Variablearray.h"
#include "VariableManager.h"
int main()
{
	struct CharStack stack;
	initialize(&stack);

	struct VariableContainer container;
	initializeContainer(&container);

	FILE* abmFile = abmFileReader("abmfiles/recursiveFactorial.abm");

	abminstructionrunner(abmFile, &stack, &container);

	return 0;
}
