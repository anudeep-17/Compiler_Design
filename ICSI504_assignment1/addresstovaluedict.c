#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "addresstovaluedict.h"

void initializeMap(struct Map* map)
{
	map->currentsize = 0;
}

void insert(struct Map* map, const char* key, int value)
{
	for(int i = 0; i<MaxMapsize; i++)
        {
                if(strcmp(map->Mapitems[i].address, key) == 0)
                {
                        map->Mapitems[i].value = value;
												return;
                }
        }

	if(map->currentsize < MaxMapsize)
	{
		strcpy(map->Mapitems[map->currentsize].address, key);
		map->Mapitems[map->currentsize].value = value;
		map->currentsize++;
 	}

}

int find(struct Map* map, const char* key)
{
	if(key != NULL)
	{
		for(int i = 0; i < MaxMapsize; i++)
		{
			if(strcmp(map->Mapitems[i].address, key) == 0)
			{
				return map->Mapitems[i].value;
			}
		}
	}
	return INT_MIN;
}

void printMap(struct Map *map)
{
    for (int i = 0; i < map->currentsize; i++)
		{
        printf("Address: %s, Value: %d\n", map->Mapitems[i].address, map->Mapitems[i].value);
    }
}
