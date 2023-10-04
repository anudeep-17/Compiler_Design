#include <stdbool.h>
#ifndef Stack
#define Stack

#define StackSize 100

struct CharStack
{
        int top;
        char* stackitems[StackSize];
};

void initialize(struct CharStack* stack);
int isEmpty(struct CharStack* stack);
int IsFull(struct CharStack* stack);
void PushIntoStack(struct CharStack* stack, const char* item);
char* PopStack(struct CharStack* stack);
char* PeekStack(struct CharStack* stack);
void PrintStack(struct CharStack* stack);
bool FindIfInStack(struct CharStack* stack, const char* address);
#endif
