#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stack.h"
#include <stdbool.h>


void initialize(struct CharStack* stack)
{
	stack->top = -1;
}

int isEmpty(struct CharStack* stack)
{
	return stack->top == -1;
}

int IsFull(struct CharStack* stack)
{
	return stack->top == StackSize-1;
}

void PushIntoStack(struct CharStack* stack, const char* item)
{
	if(IsFull(stack))
	{
		printf("Stack is full.....\n ");
		return;
	}
	stack->stackitems[++stack->top] = strdup(item);
}

char* PopStack(struct CharStack* stack)
{
	if(isEmpty(stack))
	{
		printf("stack is empty... nothing to pop \n");
		return '\0';
	}
	char* popitem = stack->stackitems[stack->top];
	stack->top--;
	return popitem;
}

char* PeekStack(struct CharStack* stack)
{
	 if(isEmpty(stack))
        {
                printf("stack is empty... nothing to pop \n");
                return '\0';
        }
	return stack->stackitems[stack->top];
}

void PrintStack(struct CharStack* stack)
{
	if(isEmpty(stack))
	{
		printf("Stack is empty....\n");
		return;
	}
	printf("Stack contents: ");
	for(int i = 0; i<= stack->top; i++)
	{
		printf("%s, ", stack->stackitems[i]);
	}
	printf(" -- done \n");
}

bool FindIfInStack(struct CharStack* stack, const char* address)
{
	for(int i = 0; i<= stack->top; i++)
	{
		if(stack->stackitems[i] == address)
		{
			return true;
		}
	}
	return false;
}
