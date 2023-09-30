#include <string.h>
#ifndef IndexKeywordCommandPair
#define IndexKeywordCommandPair

#define MaxPairSize 1024

struct IndKeyCommPair{
	int linenumber;
	char keyword[20];
	char command[1024];
};

struct Pair{
	struct IndKeyCommPair Key_CommandPair[MaxPairSize];
	int currentpairsize;	
};

void initializePair(struct Pair* pair);
void addPair(struct Pair* pair, int linenumber, const char* keyword, const char* command);
int getlinenumber(struct Pair* pair, const char* keyword, const char* command);
void printPair(struct Pair* pair);

#endif
