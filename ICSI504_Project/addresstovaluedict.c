#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "addresstovaluedict.h"
#include <stdbool.h>
#include <time.h>

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
									clock_gettime(CLOCK_REALTIME, &(map->Mapitems[i].TimeStamp));
									return;
          }
  }

	if(map->currentsize < MaxMapsize) //if key is not encountered previously
	{
		strcpy(map->Mapitems[map->currentsize].address, key);
		map->Mapitems[map->currentsize].value = value;
		map->Mapitems[map->currentsize].syncedaddress[0] = '\0'; //null terminated as there isnt any given address for now.
		map->Mapitems[map->currentsize].status[0] = '\0'; // null terminates status for now.
		clock_gettime(CLOCK_REALTIME, &(map->Mapitems[map->currentsize].TimeStamp));  // adds current timestamp to the
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
									clock_gettime(CLOCK_REALTIME, &(map->Mapitems[i].TimeStamp));
									return;
          }
  }
}

/*
InsertStatus: takes a status and adds the status to the variable, it is only done to global variables and rest all have null
*/
void InsertStatus(struct Map* map, const char* key, const char* status)
{
	// printf("in addresstovaluedict: key: %s and syncadd : %s \n\n\n", key, status);
	for(int i = 0; i<MaxMapsize; i++)
	{
					if(strcmp(map->Mapitems[i].address, key) == 0) // a instance where key is already in the map then we update the value only.
					{
									strcpy(map->Mapitems[i].status, status); //copy the given status to map
									clock_gettime(CLOCK_REALTIME, &(map->Mapitems[i].TimeStamp));
									return;
					}
	}
}

void InsertTimeStamp(struct Map* map, const char* key)
{
	for(int i = 0; i<MaxMapsize; i++)
	{
					if(strcmp(map->Mapitems[i].address, key) == 0) // a instance where key is already in the map then we update the value only.
					{
						clock_gettime(CLOCK_REALTIME, &(map->Mapitems[i].TimeStamp));
						return;
					}
	}
}

void InsertIndexBased(struct Map* map, const char* key, const int value, const int index)
{
		if(index > -1)
		{
			strcpy(map->Mapitems[index].address, key); // replaces the key with the new key
			map->Mapitems[index].value = value; // adds the value
			map->Mapitems[map->currentsize].syncedaddress[0] = '\0'; //null terminated as there isnt any given address for now.

			clock_gettime(CLOCK_REALTIME, &(map->Mapitems[index].TimeStamp)); // adds current timestamp to the
		}
}


int IndextoUpdateCache(struct Map* map)
{
	//to findout index with the oldes indexes.
	 struct timespec oldestTimestamp = {.tv_sec = LONG_MAX, .tv_nsec = 0};
	 int indexofoldest = -1;

	 for (int i = 0; i < map->currentsize; i++)
	 {
			 if (map->Mapitems[i].TimeStamp.tv_sec < oldestTimestamp.tv_sec ||
					(map->Mapitems[i].TimeStamp.tv_sec == oldestTimestamp.tv_sec && map->Mapitems[i].TimeStamp.tv_nsec < oldestTimestamp.tv_nsec))
					 {
							 oldestTimestamp = map->Mapitems[i].TimeStamp;
							 indexofoldest = i;
					 }
	 }
	 return indexofoldest;
}

struct addresstoval getFields_AtIndex(struct Map* map, const int index)
{
	struct addresstoval result;

	if(index < map->currentsize)
	{
		result = map->Mapitems[index];
	}
	return result;
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
				clock_gettime(CLOCK_REALTIME, &(map->Mapitems[i].TimeStamp));

			}
		}
	}
	return INT_MIN; //else returns INT_MIN
}

bool isthere_aconcecutive(struct Map* map, const char* varname, int offset)
{
	if(varname != NULL)
	{
		for(int i = 0; i < MaxMapsize; i++)
		{
			char *result = strstr(map->Mapitems[i].address, varname);

			if(result != NULL)
			{
				int counter = 0;
				char buffer[strlen(map->Mapitems[i].address)];
				strcpy(buffer, map->Mapitems[i].address);

				char* token = strtok(buffer, " ");
				while(token != NULL)
				{
					if(strcmp(token, varname) == 0)
					{
						counter++;
						break;
					}
					token = strtok(NULL, " ");
					counter++;
				}
				// printf("varname: %s, offset: %d, counter: %d \n\n", varname, offset, counter);
				if(token != NULL && counter+offset <= find(map, map->Mapitems[i].address) && counter+offset > 0)
				{
					return true;
				}
				else
				{
					break;
				}
			}

		}
	}
	return false;
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

const char** findinstancesthatsynced(struct Map* map, const char* key, int* countofaddresses)
{
	if(key != NULL)
	{
		const char** result = NULL;
		int resultCount = 0;

		for(int i = 0; i<MaxMapsize; i++)
		{
			if(strcmp(map->Mapitems[i].syncedaddress, key) == 0)
			{
				result = realloc(result, (resultCount + 1) * sizeof(const char*));
				result[resultCount] = map->Mapitems[i].address;
        resultCount++;
			}
		}

		if(resultCount> 0)
		{
			*countofaddresses = resultCount;
			return result;
		}
		else
		{
			*countofaddresses = 0;
			free(result);
			return NULL;
		}
	}
	else
	{
		*countofaddresses = 0;
		return NULL;
	}
}


/*
findStatus: for given key address find its status and update the status.
*/
char* findStatus(struct Map* map, const char* key)
{
	if(key != NULL)
	{
		for(int i = 0; i<MaxMapsize; i++)
		{
			if(strcmp(map->Mapitems[i].address, key) == 0)
			{
				if(strlen(map->Mapitems[i].status) > 0)
				{
					return map->Mapitems[i].status;
					clock_gettime(CLOCK_REALTIME, &(map->Mapitems[i].TimeStamp));
				}
				else
				{
					return "NOSTATUS";
				}
			}
		}
	}
	return "NOSTATUS";
}


/*
ifstatus :  gets key and boolen and return true or false basing on the idea if status is same as given or not.
*/

bool ifstatus(struct Map* map, const char* key, const char* status)
{
	if(key != NULL)
	{
		for(int i = 0; i<MaxMapsize; i++)
		{
			if(strcmp(map->Mapitems[i].address, key) == 0)
			{
				return strcmp(map->Mapitems[i].status, status) == 0;
			}
		}
	}
	return false;
}

//only for socket based string structuring
char* StructureStringForSignal(struct Map* map, const int forclient)
{
	if(map->currentsize > 0)
	{
		char* returner = (char*)malloc(1);
		// strcpy(returner, " ");

		for(int i = 0; i< map->currentsize; i++)
		{
			if(map->Mapitems[i].value != forclient )
			{
				char* address = map->Mapitems[i].address;
				char* status = map->Mapitems[i].status;

				char buffer[256];
				sprintf(buffer, "%s %s,", address, status);

				returner = (char*)realloc(returner, strlen(returner)+strlen(buffer)+1);
				strcat(returner, buffer);
			}
		}

		if(strlen(returner) < 2)
		{
			returner = "NOCHANGES";
		}
		return returner;
	}
	else
	{
		return "NOCHANGES";
	}
}

//-----------------------------------------------------------------for testing -----------------------------------------------------------------------------------
void printMap(struct Map *map)
{
    for (int i = 0; i < map->currentsize; i++)
		{
        printf("Address: %s, Value: %d, Syncedaddress: %s, Status: %s, TimeStamp: %ld.%09ld seconds \n", map->Mapitems[i].address, map->Mapitems[i].value, map->Mapitems[i].syncedaddress,map->Mapitems[i].status, map->Mapitems[i].TimeStamp.tv_sec, map->Mapitems[i].TimeStamp.tv_nsec);
    }
}
