#include <stdio.h>
#include <string.h>
#include "VariableManager.h"
#include <stdbool.h>
#include <limits.h>
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

void updateContainerbyaddress(struct VariableContainer* container, const char* variableaddress, int variablevalue)
{
		int index = container->containernumber;
		if(FindInContainerbyaddress(container, variableaddress) != -1)
		{
			insert(&(container->variablevalues[index]), variableaddress, variablevalue);
		}
}


int FindInAboveScope(struct VariableContainer* container, const char* variablename)
{
	int index = (container->containernumber > 0)? container->containernumber - 1 : -1;
	if(index != -1)
	{
		return find(&(container->variablevalues[index]), addressofdata(&(container->variablenames[index]), variablename));
	}
	else
	{
		return INT_MIN;
	}
}



int FindInContainer(struct VariableContainer* container, const char* variablename)
{
	int index = container->containernumber;
	return find(&(container->variablevalues[index]), addressofdata(&(container->variablenames[index]), variablename));
}

int FindInContainerbyaddress(struct VariableContainer* container, const char* variableaddress)
{
	int index = container->containernumber;
	return find(&(container->variablevalues[index]), variableaddress);
}

char* getaddressfromContainer(struct VariableContainer* container, const char* variablename)
{
	int index = container->containernumber;
	return addressofdata(&(container->variablenames[index]), variablename);
}

void NewScope(struct VariableContainer* container)
{
		container->containernumber++;
}

void DeleteScope(struct VariableContainer* container)
{
	int index = container->containernumber;
	initializeStrings(&(container->variablenames[index]));
	initializeMap(&(container->variablevalues[index]));
	container->containernumber--;
}

void OnReturnMakeAccesible(struct VariableContainer* container)
{
	int currentindex = container->containernumber;
	int indextoappendin = container->containernumber - 1;

	for(int i = 0; i< container->variablenames[currentindex].currentsize; i++)
	{
		if(addressofdata(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]) !=NULL )
		{
				insert(&(container->variablevalues[indextoappendin]), addressofdata(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]), find(&(container->variablevalues[currentindex]),addressofdata(&(container->variablenames[currentindex]), container->variablenames[currentindex].data[i])));
		}
		else
		{
			append(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]);
			insert(&(container->variablevalues[indextoappendin]), addressofdata(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]), find(&(container->variablevalues[currentindex]),addressofdata(&(container->variablenames[currentindex]), container->variablenames[currentindex].data[i])));
		}
	}
}

void MakeReturnablesAccesible(struct VariableContainer* container, int numofreturnables)
{
	int currentindex = container->containernumber;
	int indextoappendin = container->containernumber - 1;

	for(int i = container->variablenames[currentindex].currentsize; i >= container->variablenames[currentindex].currentsize-numofreturnables; i--)
	{
		if(addressofdata(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]) !=NULL )
		{
				insert(&(container->variablevalues[indextoappendin]), addressofdata(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]), find(&(container->variablevalues[currentindex]),addressofdata(&(container->variablenames[currentindex]), container->variablenames[currentindex].data[i])));
		}
		else
		{
			append(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]);
			insert(&(container->variablevalues[indextoappendin]), addressofdata(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]), find(&(container->variablevalues[currentindex]),addressofdata(&(container->variablenames[currentindex]), container->variablenames[currentindex].data[i])));
		}
	}
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
