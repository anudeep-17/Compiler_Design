#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Variablearray.h"
#include <stdbool.h>

/*
Variablearray: uses a 2D array to store variable names in them.
*/

/*
initializeStrings: initializes the 2d array
*/
void initializeStrings(struct Stringarray* array)
{
	array-> currentsize = 0;
}

/*
isEmptyStrings: check and returns if the 2d array is empty
*/
int isEmptyStrings(struct Stringarray* array)
{
	return array->currentsize == 0;
}

/*
isFullStrings: check if the 2d array is full or not
*/
int isFullStrings(struct Stringarray* array)
{
	return array->currentsize == Maxsize;
}

/*
append: takes data and append it into the 2d array
*/
void append(struct Stringarray* array, const char* data)
{
	if(!isFullStrings(array))
	{
		strcpy(array->data[array->currentsize], data); //adds into array and increases the pointer
		array->currentsize++;
	}
	else
	{
		printf("String is full");
	}
}

/*
atindex: takes a int and returns whats stored in that location.
not used in abminstruction runner

*/
char* atindex(struct Stringarray* array, int location)
{
	if(location>=0 && location < array->currentsize)
	{
		return array->data[location];
	}
	else
	{
		printf("Index Invalid \n");
		return NULL;
	}
}

/*
addressofdata: returns address of data's index to store it in map corresponding ot its value.
*/
char* addressofdata(struct Stringarray* array, const char* data)
{
	static char address[20];

	for(int i = 0; i<Maxsize; i++)
	{
		if(strcmp(array->data[i], data) == 0) // if it match the passed variable it will return address
		{
			snprintf(address, sizeof(address), "%p", (void*)&(array->data[i])); //stores the address and returns it
			return address;
		}
	}
	return NULL;
}

/*
addressofdatabyoffset: moves the pointer, left or right basing on offset and return the address of that index.
*/
char* addressofdatabyoffset(struct Stringarray* array, const char* address, const int offset)
{
    for (int i = 0; i < Maxsize; i++)
    {
        char addr[20];
        snprintf(addr, sizeof(addr), "%p", (void*)&(array->data[i)); // to check if the given address is this

        if (strcmp(addr, address) == 0) // if that is the address and i is not at the last index
        {
					if(i+offset < 0 || i+offset > Maxsize-1)
					{
						printf("over the index requirement\n\n");
						return NULL;
					}
					else
					{
						char next_addr[20];
						snprintf(next_addr, sizeof(next_addr), "%p", (void*)&(array->data[i + offset]));
						return next_addr;
					}
        }
    }
    return NULL; // Return NULL to indicate that the address wasn't found or it was the last element
}


//----------------------------------testing purpose---------------------------------------------------------
void printarray(struct Stringarray* array)
{
    for (int i = 0; i < array->currentsize; i++)
    {
        printf("Element %d: %s\n", i, array->data[i]);
    }
}
