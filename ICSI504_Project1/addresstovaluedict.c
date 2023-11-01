#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "addresstovaluedict.h"

/*
Map: a basic key to value map
*/
//initialzes the map by making its size 0
void initializeMap(struct Map* map)
{
	map->currentsize = 0;
}

// insert: takes a key value and int value and checks if key already exists, if exists update it by new value else create a new key and insert value.
void insert(struct Map* map, const char* key, int value)
{
	for(int i = 0; i<MaxMapsize; i++)
  {
          if(strcmp(map->Mapitems[i].address, key) == 0) // a instance where key is already in the map then we update the value only.
          {
                  map->Mapitems[i].value = value;
									return;
          }
  }

	if(map->currentsize < MaxMapsize) //if key is not encountered previously
	{
		strcpy(map->Mapitems[map->currentsize].address, key);
		map->Mapitems[map->currentsize].value = value;
		map->Mapitems[map->currentsize].syncedaddress[0] = '\0'; //null terminated as there isnt any given address for now.
		map->currentsize++;
 	}

}

/*
InsertSyncedAddress: takes a key where it needs to put the address it needs to sync up with, and we update it syncedaddress to the given address
*/
void InsertSyncedAddress(struct Map* map, const char* key, const char* syncaddress)
{
	// printf("in addresstovaluedict: key: %s and syncadd : %s \n\n\n", key, syncaddress);
	for(int i = 0; i<MaxMapsize; i++)
  {
          if(strcmp(map->Mapitems[i].address, key) == 0) // a instance where key is already in the map then we update the value only.
          {
									// printf("\n\n\n\ni was here to set the address \n\n\n\n");
                  strcpy(map->Mapitems[i].syncedaddress, syncaddress); //copy the given syncadd to map
									return;
          }
  }
}


/*
find: finds the given key in map and returns its value.
*/
int find(struct Map* map, const char* key)
{
	if(key != NULL)
	{
		for(int i = 0; i < MaxMapsize; i++)
		{
			if(strcmp(map->Mapitems[i].address, key) == 0) //when the key match found we return its value
			{
				return map->Mapitems[i].value;
			}
		}
	}
	return INT_MIN; //else returns INT_MIN
}

/*
findSyncedWith: for given key address find its synced address to update the address.
*/
char* findSyncedWith(struct Map* map, const char* key)
{
	if(key != NULL)
	{
		for(int i = 0; i < MaxMapsize; i++)
		{
			if(strcmp(map->Mapitems[i].address, key) == 0) //when the key match found we return its value
			{
				if (strlen(map->Mapitems[i].syncedaddress) > 0)
				{
						// printf("\n\n\n\ni was here to get the address \n\n\n\n");
            return map->Mapitems[i].syncedaddress; // Return the synced address.
        }
				else
				{
            // Handle the case where syncedaddress is empty ('\0').
            return "NO addr"; // Or return NULL or another appropriate value.
        }
			}
		}
	}
	return "NO addr"; //else returns NULL
}

//-----------------------------------------------------------------for testing -----------------------------------------------------------------------------------
void printMap(struct Map *map)
{
    for (int i = 0; i < map->currentsize; i++)
		{
        printf("Address: %s, Value: %d\n", map->Mapitems[i].address, map->Mapitems[i].value);
    }
}
