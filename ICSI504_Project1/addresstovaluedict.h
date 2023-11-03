//header of a Map addresstoval.c
#ifndef addresstovaluedict
#define addresstovaluedict

#define MaxMapsize 500 //Max map elements

#include <stdbool.h>

struct addresstoval
{
	char address[30];
	int value;
	char syncedaddress[30];
	char status[30];
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
//=========================find methods=========================================== 
int find(struct Map* map, const char* key);
char* findSyncedWith(struct Map* map, const char* key);
char* findStatus(struct Map* map, const char* key);
//===============================for status methods ===============================
bool ifstatus(struct Map* map, const char* key, const char* status);
//==================================test based methods ============================
void printMap(struct Map *map);

#endif
