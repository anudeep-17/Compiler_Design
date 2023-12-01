#include <stdio.h>
#include <string.h>
#include "VariableManager.h"
#include <stdbool.h>
#include <limits.h>
#include "Variablearray.h"
#include "addresstovaluedict.h"

/*
VariableManager: it layerd over 2D array(Stringarray from Variablearray.h), and a Map (Map from addresstovaluedict) to store variables and manages its scope.
								 Variablenames -> 2D array and
								 Variablevalues -> Map

*/

//initializes new container and asigns its number
void initializeContainer(struct VariableContainer* container)
{
	 container->containernumber = 0;
	 initializeStrings(&(container->variablenames[0]));
	 initializeMap(&(container->variablevalues[0]));
}

// ----------------------------------------------------------------INSERT METHODS ----------------------------------------------------------------------------

//insertIntoContainer: Inserts given variablename and its value into container.
void insertIntoContainer(struct VariableContainer* container, const char* variablename, int variablevalue)
{
	// printf("check for global, variablename : %s, and variablevalue: %d \n\n", variablename, variablevalue);
	if(container->containernumber < Maxarraysize)
	{
		int index = container->containernumber; // takes current contianer number
		append(&(container->variablenames[index]), variablename); //inserts the variable in variable array
		insert(&(container->variablevalues[index]), addressofdata(&(container->variablenames[index]), variablename), variablevalue); //inserts variable address from 2d array and its value in Map
	}
	else
	{
		printf("it is full");
	}
}

// updateContainerbyaddress: takes a variable address and its value and updates the value in the map
void updateContainerbyaddress(struct VariableContainer* container, const char* variableaddress, int variablevalue)
{
		int index = container->containernumber;
		if(FindInContainerbyaddress(container, variableaddress) != -1)
		{
			insert(&(container->variablevalues[index]), variableaddress, variablevalue); // Map function insert handles this.
		}
}



// ----------------------------------------------------------------FIND METHODS ----------------------------------------------------------------------------

//FindInAboveScope: takes a variable name and search for this above scope and returns its value,
// ******************this is used when passing parameters.
int FindInAboveScope(struct VariableContainer* container, const char* variablename)
{
	int index = (container->containernumber > 0)? container->containernumber - 1 : -1;
	if(index != -1)
	{
		return find(&(container->variablevalues[index]), addressofdata(&(container->variablenames[index]), variablename)); // finds the address in previous container and returns the value
	}
	else
	{
		return INT_MIN; //else returns INT_MIN
	}
}


/*
FindInContainer: find the given variable name in current container.
*/
int FindInContainer(struct VariableContainer* container, const char* variablename)
{
	int index = container->containernumber;
	return find(&(container->variablevalues[index]), addressofdata(&(container->variablenames[index]), variablename)); // returns the value if it is in current container.
}

/*
FindInContainerbyaddress: find the give variable address in given container and return its value
*/
int FindInContainerbyaddress(struct VariableContainer* container, const char* variableaddress)
{
	int index = container->containernumber;
	return find(&(container->variablevalues[index]), variableaddress);
}


//-----------------------------------------------------------------------ADDRESS RETURNING METHODS------------------------------------------------
/*
getaddressfromContainer: gets the variable and returns address of the variable name from container
*/
char* getaddressfromContainer(struct VariableContainer* container, const char* variablename)
{
	int index = container->containernumber;
	return addressofdata(&(container->variablenames[index]), variablename);
}

//----------------------------------------------------------------------SCOPE MANAGEMENT METHODS----------------------------------------------------------
/*
NewScope: if it is new scope  we create a new container now.
*/
void NewScope(struct VariableContainer* container)
{
		container->containernumber++;
}

/*
DeleteScope: delete the currentscope, used when we have are done with a scope to free space.
*/
void DeleteScope(struct VariableContainer* container)
{
	int index = container->containernumber;
	//initializes all of them making them 0 length and terminates.
	initializeStrings(&(container->variablenames[index]));
	initializeMap(&(container->variablevalues[index]));
	container->containernumber--;
}

//----------------------------------------------------------------HandleReturnableMethods---------------------------------------------------------------------
/*
MakeReturnablesAccesible: takes number of returnable variables in the scope and appends those in the above scope making them accesible to the scope above.
*/
void MakeReturnablesAccesible(struct VariableContainer* container, int numofreturnables)
{
	int currentindex = container->containernumber; //index we currently are in
	int indextoappendin = container->containernumber - 1; //index we want to append returnable variables

	for(int i = container->variablenames[currentindex].currentsize-1; i >= container->variablenames[currentindex].currentsize-numofreturnables; i--) // loops through the container from end
	{
		if(addressofdata(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]) != NULL ) // if the variable already exist in above scope just update
		{
				insert(&(container->variablevalues[indextoappendin]), addressofdata(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]), find(&(container->variablevalues[currentindex]),addressofdata(&(container->variablenames[currentindex]), container->variablenames[currentindex].data[i])));
		}
		else // if it doesnt exist make a new variable and insert the value.
		{
			append(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]);
			insert(&(container->variablevalues[indextoappendin]), addressofdata(&(container->variablenames[indextoappendin]), container->variablenames[currentindex].data[i]), find(&(container->variablevalues[currentindex]),addressofdata(&(container->variablenames[currentindex]), container->variablenames[currentindex].data[i])));
		}
	}
}

//---------------------------------------------------------------Global scope methods------------------------------------------------------------

// finds in global scope and returns values
int FindInGlobalScope(struct VariableContainer* container, const char* variablename)
{
	int index = 0; // the global and shared scope.
	return find(&(container->variablevalues[index]), addressofdata(&(container->variablenames[index]), variablename)); // finds the address in previous container and returns the value
}

/*
FindInGlobalContainerbyaddress: find the give variable address in given container's global level and return its value
*/
int FindInGlobalContainerbyaddress(struct VariableContainer* container, const char* variableaddress)
{
	int index = 0;
	return find(&(container->variablevalues[index]), variableaddress);
}

/*
getaddressfromGlobalContainer: gets the variable and returns address of the variable name from container
*/
char* getaddressfromGlobalContainer(struct VariableContainer* container, const char* variablename)
{
	int index = 0;
	return addressofdata(&(container->variablenames[index]), variablename);
}

/*
getnameof_variable_byaddress_fromGlobalContainer: gets the variable and returns address of the variable name from container
*/
char* getnameof_variable_byaddress_fromGlobalContainer(struct VariableContainer* container, const char* variableaddress)
{
	int index = 0;
	return nameofvariablebyaddress(&(container->variablenames[index]), variableaddress);
}

/*
updateGlobalContainerbyaddress: takes a variable address and its value and updates the value in the map
*/
void updateGlobalContainerbyaddress(struct VariableContainer* container, const char* variableaddress, int variablevalue)
{
		int index = 0;
		int countforrootvariable = 0;
		const char** syncedaddressforroot = findinstancesthatsynced(&(container->variablevalues[index]),variableaddress, &countforrootvariable);

		// printf("obtained variable address: %s, and its syncedaddress %s \n\n\n", variableaddress, findSyncedWith(&(container->variablevalues[index]), variableaddress));
		// printf("else count of synced with: %d and its value at countainer is %d \n\n ", countforrootvariable, find(&(container->variablevalues[index]), findSyncedWith(&(container->variablevalues[index]), variableaddress)));

		if(FindInGlobalContainerbyaddress(container, variableaddress) != -1)
		{
			insert(&(container->variablevalues[index]), variableaddress, variablevalue); // Map function insert handles this.
			//=================================update the attached address on update the given address======================
			if(strcmp(findSyncedWith(&(container->variablevalues[index]), variableaddress), "NO addr") != 0 && (find(&(container->variablevalues[index]), findSyncedWith(&(container->variablevalues[index]), variableaddress)) != INT_MIN && find(&(container->variablevalues[index]), findSyncedWith(&(container->variablevalues[index]), variableaddress)) != variablevalue))
			{
				//if this variable is synced with any other address we update that address too
				if(FindInGlobalContainerbyaddress(container,findSyncedWith(&(container->variablevalues[index]), variableaddress)) != INT_MIN)
				{
					//if in scope of global container
					updateGlobalContainerbyaddress(container, findSyncedWith(&(container->variablevalues[index]), variableaddress), variablevalue);
					// InGlobalScopeSetStatus(container,findSyncedWith(&(container->variablevalues[index]), variableaddress), "Mine");
				}
			}
			else if(countforrootvariable > 0)
			{
				// printf("i did come here to change the synced addresss\n\n");
				for(int i = 0; i< countforrootvariable; i++)
				{
					if(FindInGlobalContainerbyaddress(container,syncedaddressforroot[i]) != INT_MIN)
					{
						// printf("syncedwith: %s\n\n", syncedaddressforroot[i]);
						// insert(&(container->variablevalues[index]), syncedaddressforroot[i], variablevalue);
						updateGlobalContainerbyaddress(container, syncedaddressforroot[i], variablevalue);
						// InGlobalScopeSetStatus(container,findSyncedWith(&(container->variablevalues[index]), variableaddress), "Mine");
					}
				}
			}
		}
}
//gets the variable address by a particular offset
char* getVariableaddressByOffset(struct VariableContainer* container, const char* variableaddress, int offset)
{
	//gets the address that is +/- offset to the left/right of given variable. --> only in global scope expecting it to behave like MIPS else need another method
	// for current container.
	int index = 0;
	return addressofdatabyoffset(&(container->variablenames[index]), variableaddress, offset);
}

//basically works on &x = &y making x a copy of y and everytime x is updated y updates
void setSyncBetween(struct VariableContainer* container, const char* variableaddress, const char* addresstosyncwith)
{
	int index = 0;
	// printf("address obtainer: %s, and address to sync with %s\n\n\n", variableaddress, addresstosyncwith);
	//now use the index to setup the sync
	InsertSyncedAddress(&(container->variablevalues[index]), variableaddress, addresstosyncwith);
	//updates the value of the variable address to sync it with the given
	updateGlobalContainerbyaddress(container, variableaddress, find(&(container->variablevalues[index]), addresstosyncwith));
}

//works on setting status weather it is I, S, M
void InGlobalScopeSetStatus(struct VariableContainer* container, const char* variableaddress, const char* status)
{
	int index = 0;
	InsertStatus(&(container->variablevalues[index]), variableaddress, status);
}

// gives the status of the variable
char* InGlobalScopeFindStatus(struct VariableContainer* container, const char* variableaddress)
{
	int index = 0;
	return findStatus(&(container->variablevalues[index]), variableaddress);
}
// checks the status of the variable
bool InGlobalScopeIsItGivenStatus(struct VariableContainer* container, const char* variableaddress,const char* status)
{
	int index = 0;
	return ifstatus(&(container->variablevalues[index]), variableaddress, status);
}
// gets all the synced variable address
char* InGlobalScopeFindSyncedWith(struct VariableContainer* container, const char* variableaddress)
{
	int index = 0;
	return findSyncedWith(&(container->variablevalues[index]), variableaddress);
}
// gets all the syncedup addresses.
const char** InGlobalsFindSyncedInstances(struct VariableContainer* container, const char* variableaddress, int* count)
{
	int index = 0;
	return findinstancesthatsynced(&(container->variablevalues[index]), variableaddress, count);
}
//================================================================================End of global methods =======================================================



//Prints containers just for  testing purporse.
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
