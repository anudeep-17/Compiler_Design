#include "CacheMemory.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "addresstovaluedict.h"


void initializeCache(struct Cache* cacheMem)
{
  cacheMem->currentCachesize = 0;
  initializeMap(&(cacheMem->CacheValues));
}

void InsertCache(struct Cache* cacheMem, const char* varname, const int value, const char* state)
{
  if(find(&(cacheMem->CacheValues), varname) != INT_MIN)
  {
    insert(&(cacheMem->CacheValues), varname, value);
    InsertStatus(&(cacheMem->CacheValues), varname, state);

    //for synced variables by :&
    int countforrootvariable = 0;
    const char** syncedaddressforroot = findinstancesthatsynced(&(cacheMem->CacheValues),varname, &countforrootvariable);

    if(strcmp(findSyncedWith(&(cacheMem->CacheValues), varname), "NO addr") != 0 && (find(&(cacheMem->CacheValues), findSyncedWith(&(cacheMem->CacheValues), varname)) != value))
    {
        InsertCache(cacheMem,findSyncedWith(&(cacheMem->CacheValues), varname), value, state);
        InsertStatus(&(cacheMem->CacheValues), findSyncedWith(&(cacheMem->CacheValues), varname), state);
    }
    else if(countforrootvariable > 0)
    {
      for( int i = 0; i<countforrootvariable; i++)
      {
        if(find(&(cacheMem->CacheValues),syncedaddressforroot[i]) != INT_MIN)
        {
          InsertCache(cacheMem, syncedaddressforroot[i], value, state);
          InsertStatus(&(cacheMem->CacheValues), syncedaddressforroot[i], state);
        }
      }
    }

  }
  else
  {
    if(cacheMem->currentCachesize < Maxcachesize)
    {
      insert(&(cacheMem->CacheValues), varname, value);
      InsertStatus(&(cacheMem->CacheValues), varname, state);
      cacheMem->currentCachesize++;
    }
    else
    {
      InsertIndexBased(&(cacheMem->CacheValues), varname, value, IndextoUpdateCache(&(cacheMem->CacheValues)));
      InsertStatus(&(cacheMem->CacheValues), varname, state);
    }
  }
}

int FindInCache(struct Cache* cacheMem, const char* varname)
{
  return find(&(cacheMem->CacheValues), varname);
}

char* FindInCache_Status(struct Cache* cacheMem, const char* varname)
{
  return findStatus(&(cacheMem->CacheValues), varname);
}

void updateInCacheState(struct Cache* cacheMem, const char* varname, const char* status)
{
  InsertStatus(&(cacheMem->CacheValues), varname, status);
}

void updateInCacheTimeStamp(struct Cache* cacheMem, const char* varname)
{
  InsertTimeStamp(&(cacheMem->CacheValues), varname);
}

void InsertSyncedAddress_InCache(struct Cache* cacheMem, const char* leftname, const char* rightname)
{
  InsertSyncedAddress(&(cacheMem->CacheValues), leftname, rightname);
}
void printCache(struct Cache* cacheMem)
{
  printMap(&(cacheMem->CacheValues));
}
