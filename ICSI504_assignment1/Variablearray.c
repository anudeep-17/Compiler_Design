#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Variablearray.h"
#include <stdbool.h>

void initializeStrings(struct Stringarray* array)
{
	array-> currentsize = 0;
}

int isEmptyStrings(struct Stringarray* array)
{
	return array->currentsize == 0;
}

int isFullStrings(struct Stringarray* array)
{
	return array->currentsize == Maxsize;
}

void append(struct Stringarray* array, const char* data)
{
	if(!isFullStrings(array))
	{
		strcpy(array->data[array->currentsize], data);
		array->currentsize++;
	}
	else
	{
		printf("String is full");
	}
}

char* addressofdata(struct Stringarray* array, const char* data)
{
	static char address[20];

	for(int i = 0; i<Maxsize; i++)
	{
		if(strcmp(array->data[i], data) == 0)
		{
			snprintf(address, sizeof(address), "%p", (void*)&(array->data[i]));
			return address;
		}
	}
	return NULL;
}

void printarray(struct Stringarray* array)
{
    for (int i = 0; i < array->currentsize; i++)
    {
        printf("Element %d: %s\n", i, array->data[i]);
    }
}
