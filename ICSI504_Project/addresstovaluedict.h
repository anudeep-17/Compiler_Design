//header of a Map addresstoval.c
#ifndef addresstovaluedict
#define addresstovaluedict

#define MaxMapsize 500 //Max map elements

#include <stdbool.h>
#include <time.h>

struct addresstoval
{
	char address[30];
	int value;
	char syncedaddress[30];
	char status[30];
  struct timespec lastUsedTime;
};

struct Map
{
	struct addresstoval Mapitems[MaxMapsize];
	int currentsize;
};

//method declarations
void initializeMap(struct Map* map);
//============================Insert Methods=======================================
void insert(struct Map* map, const char* key, int value);
void InsertSyncedAddress(struct Map* map, const char* key, const char* syncaddress);
void InsertStatus(struct Map* map, const char* key, const char* status);
void InsertBasingOnIndex(struct Map* map, const char* key, const int value, const int index);
struct addresstoval atIndexInMap(struct Map* map, const int Index);
//=========================find methods===========================================
int find(struct Map* map, const char* key);
char* findSyncedWith(struct Map* map, const char* key);
char* findStatus(struct Map* map, const char* key);
const char** findinstancesthatsynced(struct Map* map, const char* key, int* countofaddresses);
//===============================for status methods ===============================
bool ifstatus(struct Map* map, const char* key, const char* status);
char* StructureStringForSignal(struct Map* map, const int forclient);
//==================================time based methods ============================
void InsertTimeStamp(struct Map* map, const char* key);
int leastRecentlyUsedIndex(struct Map* map);
//==================================test based methods ============================
void printMap(struct Map *map);
bool isthere_aconcecutive(struct Map* map, const char* varname, int offset);
#endif
