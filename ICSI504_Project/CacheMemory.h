#ifndef CacheMemory
#define CacheMemory

#include "addresstovaluedict.h"
#define Maxcachesize 3

struct Cache
{
  int currentCachesize;
  struct Map CacheValues;
};

//===========================================Initialize the Cache memory here===========================
void initializeCache(struct Cache* cacheMem);

//===========================================INSERT=====================================================
void InsertCache(struct Cache* cacheMem, const char* varname, const int value, const char* state, int clientSocketnumber);
void InsertSyncedAddress_InCache(struct Cache* cacheMem, const char* leftname, const char* rightname);

//===========================================FIND=======================================================
int FindInCache(struct Cache* cacheMem, const char* varname);

char* FindInCache_Status(struct Cache* cacheMen, const char* varname);

//===========================================UPDATE=====================================================
void updateInCacheState(struct Cache* cacheMem, const char* varname, const char* status);
void updateInCacheTimeStamp(struct Cache* cacheMem, const char* varname);
//===========================================TestMethod=================================================
void printCache(struct Cache* cacheMem);

#endif // CacheMemory
