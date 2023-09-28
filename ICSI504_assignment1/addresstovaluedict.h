#ifndef addresstovaluedict
#define addresstovaluedict

#define Maxsize 1024

struct addresstoval
{
	char address[1024];
	int value;
};

struct Map
{
	struct addresstoval Mapitems[Maxsize];
	int currentsize;
};

void initializeMap(struct Map* map);
void insert(struct Map* map, const char* key, int value);
int find(struct Map* map, const char* key);
#endif

