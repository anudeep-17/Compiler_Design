#include <stdio.h>
#include <string.h>
#include "IndexKeywordCommandPair.h"
/*
IndexKeywordCommandPair : A struct that holds a linenumber, a keyword and command. and has a currentpairsize.
this is used to read the file and run it
*/

//initialize pair making the current 0
void initializePair(struct Pair* pair)
{
	pair->currentpairsize = 0;
}

/*
addPair: it takes a linenumber, keyword and command to store in pair.
parameters: linenumber, keyword, command
*/
void addPair(struct Pair* pair, int linenumber, const char* keyword, const char* command)
{
    	if (pair->currentpairsize < MaxPairSize)
	{
        	int index = pair->currentpairsize; //increases the size of keyword.
        	pair->Key_CommandPair[index].linenumber = linenumber; //updates the linenumber,
        	strncpy(pair->Key_CommandPair[index].keyword, keyword, sizeof(pair->Key_CommandPair[index].keyword)); // updates the keyword
					//if command is not null we append a \0 line character, else we add command
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

/*
	getlinenumber: get line number of a given keyword and command when both keyword and command matches the parameter.
*/
int getlinenumber(struct Pair* pair, const char* keyword, const char* command) {
    	for (int i = 0; i < pair->currentpairsize; i++)
			{
        	if (strcmp(pair->Key_CommandPair[i].keyword, keyword) == 0 && strcmp(pair->Key_CommandPair[i].command, command) == 0) //if true we return the linenumber
					{
            		return pair->Key_CommandPair[i].linenumber;
        	}
  		}
    return -1; // Keyword and command not found
}

//PrintPair is used to print the pair for testing pupouse else it has no use with code. 
void printPair(struct Pair *pair) {
    for (int i = 0; i < pair->currentpairsize; i++)
		{
			printf("Line Number: %d, Keyword: %s, Command: %s\n",pair->Key_CommandPair[i].linenumber,pair->Key_CommandPair[i].keyword,pair->Key_CommandPair[i].command);
    }
}
