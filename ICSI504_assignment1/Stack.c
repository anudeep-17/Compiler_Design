#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"
#define Stacksize 1024

char* stack[Stacksize];
int top = -1;

int isEmpty() 
{
	if (top == -1)
    		return 1;
  	else
    		return 0;
}

int IsFull() 
{
	if (top == Stacksize - 1) 
	{
		return 1;
  	} 
	else 
	{
    		return 0;
  	}
}

char* PeekStack() 
{ 
	return stack[top]; 
}

char* PopStack() 
{
  	char* temp;
  	if (!isEmpty()) 
  	{
    		temp = stack[top];
    		top = top - 1;
    		return temp;
  	} 
	else 
	{
    		printf("Stack empty \n");
		return NULL;
  	}
}

void PushIntoStack(char* data) 
{
	if (!IsFull()) 
	{
    		top = top + 1;
    		stack[top] = data;
  	} 
	else 
	{
    		printf("stack is full\n");
  	}
}
