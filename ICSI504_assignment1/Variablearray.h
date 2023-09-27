#ifndef variablearray
#define variablearray

#define Maxsize 1024

struct Stringarray{
	char Stringarray[Maxsize][Maxsize];
};

int isEmpty(struct Stringarray array);
int isFull(struct Stringarray array);
void append(struct Stringarray array, const char* data);
char* atindex(struct Stringarray array, int location);
char addressofdata(struct Stringarray array, const char* data);
#endif
