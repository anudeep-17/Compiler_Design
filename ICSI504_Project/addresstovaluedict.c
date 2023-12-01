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
									clock_gettime(CLOCK_MONOTONIC, &map->Mapitems[i].lastUsedTime); // updates the usage time.
									// printf("%s: %ld seconds, %ld nanoseconds\n", key, map->Mapitems[i].lastUsedTime.tv_sec, map->Mapitems[i].lastUsedTime.tv_nsec);
									return;
          }
  }

	if(map->currentsize < MaxMapsize) //if key is not encountered previously
	{
		strcpy(map->Mapitems[map->currentsize].address, key);
		map->Mapitems[map->currentsize].value = value;
		map->Mapitems[map->currentsize].syncedaddress[0] = '\0'; //null terminated as there isnt any given address for now.
		map->Mapitems[map->currentsize].status[0] = '\0'; // null terminates status for now.
		clock_gettime(CLOCK_MONOTONIC, &map->Mapitems[map->currentsize].lastUsedTime); // inserts time to the variable
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
									clock_gettime(CLOCK_MONOTONIC, &map->Mapitems[i].lastUsedTime);// updates the usage time.
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
									clock_gettime(CLOCK_MONOTONIC, &map->Mapitems[i].lastUsedTime); // updates the usage time.
									return;
					}
	}
}


/*
InsertTimeStamp: takes map and key in the map and if the key is found we update the timetakes map and key in the map and if the key is found we update the timev
*/
void InsertTimeStamp(struct Map* map, const char* key)
{
	for(int i = 0; i<MaxMapsize; i++) // searches the map
	{
					if(strcmp(map->Mapitems[i].address, key) == 0) // a instance where key is already in the map then we update the value only.
					{
								clock_gettime(CLOCK_MONOTONIC, &map->Mapitems[i].lastUsedTime); // updates the usage time.
								return;
					}
	}
}

/*
InsertBasingOnIndex: so this is for cache memeory specially as in cache we have to replace the least reused item with the new item so we use this to repalce the index of least reused with the new item.
parameters: map, key, value, index.
*/
void InsertBasingOnIndex(struct Map* map, const char* key, const int value, const int index)
{
	if(index > -1) // if index is not -1.
		{
			strcpy(map->Mapitems[index].address, key); // replaces the key with the new key
			map->Mapitems[index].value = value; // adds the value
			map->Mapitems[index].syncedaddress[0] = '\0'; //null terminated as there isnt any given address for now.
			map->Mapitems[index].status[0] = '\0'; // null terminates status for now.
			clock_gettime(CLOCK_MONOTONIC, &(map->Mapitems[index].lastUsedTime)); // adds current timestamp to the
		}
}

// atIndexInMap: at teh provided index we return the attributes we have in that index.
struct addresstoval atIndexInMap(struct Map* map, const int index)
{
	return map->Mapitems[index]; //  we return all the components that belong to this index.
}

// compareTimes: used to compare two timespec times of two variables to findout which one is the oldes -- least reasused component time comparision function
int compareTimes(struct timespec time1, struct timespec time2) {
    if (time1.tv_sec < time2.tv_sec) {
        return -1;  // time1 is earlier
    } else if (time1.tv_sec > time2.tv_sec) {
        return 1;   // time2 is earlier
    } else {
        // seconds are equal, compare nanoseconds
        if (time1.tv_nsec < time2.tv_nsec) {
            return -1;  // time1 is earlier
        } else if (time1.tv_nsec > time2.tv_nsec) {
            return 1;   // time2 is earlier
        } else {
            return 0;   // both times are equal
        }
    }
}

// leastRecentlyUsedIndex : from the given map we search for the oldes lastUsedTime and return that index to replace next.
int leastRecentlyUsedIndex(struct Map* map)
{
	int lruIndex = 0; // lru index is initially zero such that we assume that the first is oldest,
	 struct timespec lruTime = map->Mapitems[0].lastUsedTime; // we extract first index time.

	 for (int i = 1; i < map->currentsize; i++) {
		 // if the lastUsedTime is less than lruTime then this is the oldest so replace the index and lruTime and check if there are any more oldest.
			 if (compareTimes(map->Mapitems[i].lastUsedTime, lruTime) < 0) {
					 lruTime = map->Mapitems[i].lastUsedTime;
					 lruIndex = i;
			 }
	 }
	 return lruIndex; // return the index so we can replace that.
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
isthere_aconcecutive : this is made to use for concecutive check thats
											 if a variable x is encountered such that x+offset is requested we check if there exists any thing next to x with the given offset
											 this method will verify this and confirm existence of variables in concecution
*/
bool isthere_aconcecutive(struct Map* map, const char* varname, int offset)
{
	if(varname != NULL) // if varname is not null,
	{
		for(int i = 0; i < MaxMapsize; i++) // we loop throug the map
		{
			char *result = strstr(map->Mapitems[i].address, varname); // takes the varname and check if it exist in the current key, if it does then we move ahead

			/*
			we not loop through this index breaking the char* by " " and see what is the position of this varname
			then after retreiving the position number of varname we add/sub the offset and check if it falls under the length of variable or not.
			if it does we return true
			else fasle;
			*/
			if(result != NULL)
			{
				int counter = 0;
				char buffer[strlen(map->Mapitems[i].address)];
				strcpy(buffer, map->Mapitems[i].address);

				char* token = strtok(buffer, " "); // break the string for each space.
				while(token != NULL)
				{
					if(strcmp(token, varname) == 0)
					{
						counter++; // we get the count of the variable in the char* to figure out if tehre exists any thing in the range of offset.
						break;
					}
					token = strtok(NULL, " ");
					counter++;
				}
				// printf("varname: %s, offset: %d, counter: %d \n\n", varname, offset, counter);
				if(token != NULL && counter+offset <= find(map, map->Mapitems[i].address) && counter+offset > 0) // if token != NULL which specifies that its not point to the end of char*, counter+offset < the total available variables and greater than 0
				{
					// if this is the case return true.
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

/*
findinstancesthatsynced: this is will findout if givenm key is in some other variable synced address and return a 2D with variables that have this key as synced address
												 this is used to update the values if somevariables are synced.
*/
const char** findinstancesthatsynced(struct Map* map, const char* key, int* countofaddresses)
{
	if(key != NULL)
	{
		const char** result = NULL;
		int resultCount = 0;

		for(int i = 0; i<MaxMapsize; i++)
		{
			// so we check if the key is in any of the synced address we add it to the dynamic array which expands basing on number of values
			if(strcmp(map->Mapitems[i].syncedaddress, key) == 0)
			{
				result = realloc(result, (resultCount + 1) * sizeof(const char*)); // decides the size
				result[resultCount] = map->Mapitems[i].address; // adds new element.
        resultCount++;
			}
		}

		// if result is greater than 0 we update the passed reference address with count so we can run the loop for that count.
		if(resultCount> 0)
		{
			*countofaddresses = resultCount;
			return result; // and return the result.
		}
		else
		{
			// else return NULL
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
					return map->Mapitems[i].status; // update the status of the given key,

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
				char* address = map->Mapitems[i].address; // collect the address
				char* status = map->Mapitems[i].status; // collect the status.

				char buffer[256]; // we get a compute the buffer,
				sprintf(buffer, "%s %s,", address, status);

				returner = (char*)realloc(returner, strlen(returner)+strlen(buffer)+1); // returner returns the formulated str
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
        printf("Address: %s, Value: %d, Syncedaddress: %s, Status: %s TimeStamp: %ld seconds, %ld nanoseconds \n", map->Mapitems[i].address, map->Mapitems[i].value, map->Mapitems[i].syncedaddress,map->Mapitems[i].status,map->Mapitems[i].lastUsedTime.tv_sec,map->Mapitems[i].lastUsedTime.tv_nsec);
    }
}
