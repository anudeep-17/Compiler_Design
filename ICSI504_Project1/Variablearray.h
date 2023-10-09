//header of a 2d array Variablearray.c
#ifndef Variablearray
#define Variablearray

#define Maxsize 500 //Max size of a 2d array

struct Stringarray{
	char data[Maxsize][30];
	int currentsize;
};

void initializeStrings(struct Stringarray* array);
int isEmptyStrings(struct Stringarray* array);
int isFullStrings(struct Stringarray* array);
void append(struct Stringarray* array, const char* data);
char* atindex(struct Stringarray* array, int location);
char* addressofdata(struct Stringarray* array, const char* data);
void printarray(struct Stringarray* array);
#endif
