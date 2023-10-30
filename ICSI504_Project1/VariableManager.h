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

//method declarations
//---------------------------------INITIALIZING METHODs----------------------------------------------------
void initializeContainer(struct VariableContainer* container);
//---------------------------------INSERT METHODs----------------------------------------------------
void insertIntoContainer(struct VariableContainer* container, const char* variablename, int variablevalue);
void updateContainerbyaddress(struct VariableContainer* container, const char* variableaddress, int variablevalue);
//---------------------------------FIND METHODs----------------------------------------------------
int FindInContainer(struct VariableContainer* container, const char* variablename);
int FindInContainerbyaddress(struct VariableContainer* container, const char* variableaddress);
int FindInAboveScope(struct VariableContainer* container, const char* variablename);
//---------------------------------ADDRESS GETTER METHODs----------------------------------------------------
char* getaddressfromContainer(struct VariableContainer* container, const char* variablename);
char* getVariableaddressByOffset(struct VariableContainer* container, const char* variableaddress, int offset);
//---------------------------------SCOPE HANDLING METHODs----------------------------------------------------
void NewScope(struct VariableContainer* container);
void DeleteScope(struct VariableContainer* container);
//---------------------------------RETURN HANDLING METHODs----------------------------------------------------
void MakeReturnablesAccesible(struct VariableContainer* container, int numofreturnables);
//---------------------------------TEST METHODs----------------------------------------------------
void printcontainers(struct VariableContainer* container);

//---------------------------------GLOBAL METHODs----------------------------------------------------
char* getaddressfromGlobalContainer(struct VariableContainer* container, const char* variablename);
int FindInGlobalScope(struct VariableContainer* container, const char* variablename);
void updateGlobalContainerbyaddress(struct VariableContainer* container, const char* variableaddress, int variablevalue);
int FindInGlobalContainerbyaddress(struct VariableContainer* container, const char* variableaddress);
#endif
