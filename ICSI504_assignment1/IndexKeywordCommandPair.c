
#include <stdio.h>
#include <string.h>
#include "IndexKeywordCommandPair.h"


void initializePair(struct Pair* pair)
{
	pair->currentpairsize = 0;
}

void addPair(struct Pair* pair, int linenumber, const char* keyword, const char* command)
{
    	if (pair->currentpairsize < MaxPairSize)
	{
        	int index = pair->currentpairsize;
        	pair->Key_CommandPair[index].linenumber = linenumber;
        	strncpy(pair->Key_CommandPair[index].keyword, keyword, sizeof(pair->Key_CommandPair[index].keyword));
        	if(command == NULL)
		{
			strcpy(pair->Key_CommandPair[index].command , "\0");
		}
		else
		{
			strncpy(pair->Key_CommandPair[index].command, command, sizeof(pair->Key_CommandPair[index].command));
        	}
		pair->currentpairsize++;
    	}
	else
	{
        	printf("Error: Pair is full, cannot add more pairs.\n");
    	}

}

int getlinenumber(struct Pair* pair, const char* keyword, const char* command) {
    	for (int i = 0; i < pair->currentpairsize; i++)
	{
        	if (strcmp(pair->Key_CommandPair[i].keyword, keyword) == 0 && strcmp(pair->Key_CommandPair[i].command, command) == 0)
		{
            		return pair->Key_CommandPair[i].linenumber;
        	}
  	}
    return -1; // Keyword and command not found
}

void printPair(struct Pair *pair) {
    for (int i = 0; i < pair->currentpairsize; i++)
		{
			printf("Line Number: %d, Keyword: %s, Command: %s\n",pair->Key_CommandPair[i].linenumber,pair->Key_CommandPair[i].keyword,pair->Key_CommandPair[i].command);
    }
}
