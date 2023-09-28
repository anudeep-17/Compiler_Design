#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "addresstovaluedict.h"

void initializeMap(struct Map* map)
{
	map->currentsize = 0;
}

void insert(struct Map* map, const char* key, int value)
{
	if(map->currentsize < Maxsize)
	{
		strcpy(map->Mapitems[map->currentsize].address, key);
		map->Mapitems[map->currentsize].value = value;
		map->currentsize++;
 	}
	
}

int find(struct Map* map, const char* key)
{
	for(int i = 0; i<Maxsize; i++)
	{
		if(strcmp(map->Mapitems[i].address, key) == 0)
		{
			return map->Mapitems[i].value; 
		}	
	}
	return -1;
}
