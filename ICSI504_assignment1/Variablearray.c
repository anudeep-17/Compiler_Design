#include <stdio.h>
#include <string.h>
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

