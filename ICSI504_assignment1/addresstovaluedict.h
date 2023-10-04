#ifndef addresstovaluedict
#define addresstovaluedict

#define MaxMapsize 500

struct addresstoval
{
	char address[30];
	int value;
};

struct Map
{
	struct addresstoval Mapitems[MaxMapsize];
	int currentsize;
};

void initializeMap(struct Map* map);
void insert(struct Map* map, const char* key, int value);
int find(struct Map* map, const char* key);
void printMap(struct Map *map);

#endif

