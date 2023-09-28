#ifndef variablearray
#define variablearray

#define Maxsize 1024

struct Stringarray{
	char data[Maxsize][Maxsize];
	int currentsize;
};

void initializeStrings(struct Stringarray* array);
int isEmptyStrings(struct Stringarray* array);
int isFullStrings(struct Stringarray* array);
void append(struct Stringarray* array, const char* data);
char* atindex(struct Stringarray* array, int location);
char* addressofdata(struct Stringarray* array, const char* data);

#endif
