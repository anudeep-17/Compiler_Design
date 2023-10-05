#ifndef VariableManager
#define VariableManager

#include "Variablearray.h"
#include "addresstovaluedict.h"

#define Maxarraysize 100

struct VariableContainer
{
	int containernumber;
	struct Stringarray variablenames[Maxarraysize];
	struct Map variablevalues[Maxarraysize];
};

void initializeContainer(struct VariableContainer* container);

void insertIntoContainer(struct VariableContainer* container, const char* variablename, int variablevalue);

int FindInContainer(struct VariableContainer* container, const char* variablename);
int FindInContainerbyaddress(struct VariableContainer* container, const char* variableaddress);
int FindInAboveScope(struct VariableContainer* container, const char* variablename);

char* getaddressfromContainer(struct VariableContainer* container, const char* variablename);

void updateContainerbyaddress(struct VariableContainer* container, const char* variableaddress, int variablevalue);

void printcontainers(struct VariableContainer* container);
void NewScope(struct VariableContainer* container);
void DeleteScope(struct VariableContainer* container);

void OnReturnMakeAccesible(struct VariableContainer* container);
void MakeReturnablesAccesible(struct VariableContainer* container, int numofreturnables);

#endif
