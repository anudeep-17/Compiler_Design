//will have cache memeory stuff here
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#include "addresstovaluedict.h"
#include "CacheMemory.h"
#include "ABMMethods.h"

// initializeCache: initialize the cache, with size of 3
void initializeCache(struct Cache* cacheMem)
{
  cacheMem->currentCachesize = 0;
  initializeMap(&(cacheMem->CacheValues));
}

// InsertInCache: this is the method that is responsible for inserting new elements at right spots in cache
/*
This at step 1 looks at the cache and check if it is already available in cache, if it is available then it will just reinsert with changes and updates the lastUsedTime for this variable.
while it reedits this, it will also for synced variables to resolve their values,
In Step 2 if step 1 fails, it will no check if the size of cache < 3
if so then
  it will go ahead and insert the new element
else
  it will now call the IndextoUpdate method to findout the index with LRU and replace it using the new values and creates a new time stamp of lastUsedTime
  while this is the case if the element leaving cache is in M it will sent to bus
*/
void InsertInCache(struct Cache* cacheMem, const char* varname, const int value, const char* state, const int clientsocketaddress)
{
  // printf("varname that came here : %s -- with value : %d -- state : %s\n\n\n\n", varname, value, state);
  if(find(&(cacheMem->CacheValues), varname) != INT_MIN)
  {
    // it is in cache
    insert(&(cacheMem->CacheValues), varname, value);
    InsertStatus(&(cacheMem->CacheValues), varname, state);

    //to if there are variables synced with this variables.
    if(strcmp(state, "Mine") == 0)
    {
      // if a write happens..............
      // Checks for the variable that are synced with each other, and using the data of syncing it will update those values also. s
      int countforrootvariable = 0;
      const char** syncedaddressforroot = findinstancesthatsynced(&(cacheMem->CacheValues),varname, &countforrootvariable); // gets all the variabls that have the given varname as synced address

      if(strcmp(findSyncedWith(&(cacheMem->CacheValues), varname), "NO addr") != 0 && (find(&(cacheMem->CacheValues), findSyncedWith(&(cacheMem->CacheValues), varname)) != value))
      {
        // if the given varname has a synced address then update it.
          InsertInCache(cacheMem,findSyncedWith(&(cacheMem->CacheValues), varname), value, "Mine",clientsocketaddress);
          InsertStatus(&(cacheMem->CacheValues), findSyncedWith(&(cacheMem->CacheValues), varname), "Mine");
      }
      else if(countforrootvariable > 0)
      {
        // if the given varname is synced address of any variables then update all those variables
         for( int i = 0; i<countforrootvariable; i++)
         {
           if(find(&(cacheMem->CacheValues),syncedaddressforroot[i]) != INT_MIN)
           {
             InsertInCache(cacheMem, syncedaddressforroot[i], value, "Mine" ,clientsocketaddress);
             InsertStatus(&(cacheMem->CacheValues), syncedaddressforroot[i], "Mine");
           }
         }
      }
    }
  }
  else
  {
    // not in cache then
    if(cacheMem->currentCachesize < Maxcachesize)
     {
       insert(&(cacheMem->CacheValues), varname, value);
       InsertStatus(&(cacheMem->CacheValues), varname, state);
       cacheMem->currentCachesize++;
     }
    else
    {
      // need replacement in Cache
      printf("recieved index to change due to cache size being full %d \n",leastRecentlyUsedIndex(&(cacheMem->CacheValues)));
      // if removing index state is M then we send it bus else we just remove it.
      if(leastRecentlyUsedIndex(&(cacheMem->CacheValues)) >= 0)
      {
          // if we have a replacement index nad that index is in status M we then send to bus to write and then repalce
          struct addresstoval result = atIndexInMap(&(cacheMem->CacheValues), leastRecentlyUsedIndex(&(cacheMem->CacheValues)));
          if(strcmp(result.status, "Mine") == 0)
          {
            if(strcmp(FindInCache_SyncedWith(cacheMem, result.address), varname) == 0)
            {
              // if we are removing a element that is synced with the current element to update the BUS in right manner we send the new expected value of the removing element.
              // so if a incoming variable is synced with leaving variable since the new value update need to be reflected we will send the new value else we move with old value
              printf("varname: %s value: %d, state: %s \n",result.address, result.value, result.status);
              char charvalue[10];
              sprintf(charvalue, "%d", value);
              SendToBus(clientsocketaddress, result.address, charvalue, "Write:");
            }
            else
            {
              printf("varname: %s value: %d, state: %s \n",result.address, result.value, result.status);
              char charvalue[10];
              sprintf(charvalue, "%d", result.value);
              SendToBus(clientsocketaddress, result.address, charvalue, "Write:");
            }
          }
      }

      InsertBasingOnIndex(&(cacheMem->CacheValues), varname, value, leastRecentlyUsedIndex(&(cacheMem->CacheValues)));
      if(strcmp(state, "Mine") == 0)
      {
        //for synced variables.
        InsertInCache(cacheMem, varname, value, state,clientsocketaddress);
      }
      InsertStatus(&(cacheMem->CacheValues), varname, state);
    }
  }
}

// FindInCache: finds in the Cache and returns the value of variable
int FindInCache(struct Cache* cacheMem, const char* varname)
{
  return find(&(cacheMem->CacheValues), varname);
}

//FindInCache_Status: find status of the given varname in cache.
char* FindInCache_Status(struct Cache* cacheMem, const char* varname)
{
  return findStatus(&(cacheMem->CacheValues), varname);
}
//FindInCache_SyncedWith: returns the index it is synced with
char* FindInCache_SyncedWith(struct Cache* cacheMem, const char* varname)
{
  return findSyncedWith(&(cacheMem->CacheValues), varname);
}

// updateInCacheState: used to update the state of a varname in cache,
void updateInCacheState(struct Cache* cacheMem, const char* varname, const char* status)
{
  InsertStatus(&(cacheMem->CacheValues), varname, status);
}

// updateInCacheTimeStamp: updates the lastUsedTime of varname.
void updateInCacheTimeStamp(struct Cache* cacheMem, const char* varname)
{
  InsertTimeStamp(&(cacheMem->CacheValues), varname);
}

// InsertSyncedAddress_InCache: used to update the synced of a variable when :& happens on symboltable.
void InsertSyncedAddress_InCache(struct Cache* cacheMem, const char* leftname, const char* rightname)
{
  InsertSyncedAddress(&(cacheMem->CacheValues), leftname, rightname);
}

//=========================================================================================================================================================================================

void printCache(struct Cache* cacheMem)
{
  printMap(&(cacheMem->CacheValues));
}
