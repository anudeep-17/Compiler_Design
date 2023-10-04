#include <stdio.h>
#include <string.h>
#include "VariableManager.h"
#include <stdbool.h>

#include "Variablearray.h"
#include "addresstovaluedict.h"

void initializeContainer(struct VariableContainer* container)
{
	 container->containernumber = 0;
	 initializeStrings(&(container->variablenames[0]));
	 initializeMap(&(container->variablevalues[0]));
}

void insertIntoContainer(struct VariableContainer* container, const char* variablename, int variablevalue)
{
	if(container->containernumber < Maxarraysize)
	{
		int index = container->containernumber;
		append(&(container->variablenames[index]), variablename);
		insert(&(container->variablevalues[index]), addressofdata(&(container->variablenames[index]), variablename), variablevalue);
	}
	else
	{
		printf("it is full");
	}
}

int FindInContainer(struct VariableContainer* container, const char* variablename)
{
	int index = container->containernumber;
	return find(&(container->variablevalues[index]), addressofdata(&(container->variablenames[index]), variablename));
}

void NewScope(struct VariableContainer* container)
{
		container->containernumber++;
}

void printcontainers(struct VariableContainer* container)
{
	for(int i = 0; i <= container-> containernumber; i++)
	{
		printf("current scope: %d\n", i);
		printarray(&(container->variablenames[i]));
		printMap(&(container->variablevalues[i]));
		printf("\n");
	}
}

void DeleteScope(struct VariableContainer* container)
{
	int index = container->containernumber;
	initializeStrings(&(container->variablenames[index]));
	initializeMap(&(container->variablevalues[index]));
	container->containernumber--;
}
