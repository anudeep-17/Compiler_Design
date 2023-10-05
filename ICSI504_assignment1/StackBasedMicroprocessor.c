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

	// struct Stringarray variables;
	// initializeStrings(&variables);
	//
	// struct Map variablevaluemap;
	// initializeMap(&variablevaluemap);
	//
	FILE* abmFile = abmFileReader("abmfiles/foo.abm");

	abminstructionrunner(abmFile, &stack, &container);


	// insertIntoContainer(&container, "variables1", 1);
	// printcontainers(&container);
	// printf("%d\n", FindInContainer(&container, "variables1"));
	//
	//
	// NewScope(&container);
	//
	// insertIntoContainer(&container, "variables1", 3);
	// printcontainers(&container);
	// printf("%d\n", FindInContainer(&container, "variables1"));
	//
	// NewScope(&container);
	// insertIntoContainer(&container, "variables1", 3);
	// insertIntoContainer(&container, "variables2", 3);
	// printcontainers(&container);
	// printf("%d\n", FindInContainer(&container, "variables1"));
	//
	// DeleteScope(&container);
	// DeleteScope(&container);
	//
	// printf("after deleting scope 1\n");
	//
	// printcontainers(&container);
	return 0;
}
