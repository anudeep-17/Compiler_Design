#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stack.h"
#include <stdbool.h>

/*
	Stack: used in basic excecution of keyword-command pair and also when call is evoked it is used to store the last control index.
*/

void initialize(struct CharStack* stack)
{
	stack->top = -1; //initialize the stack
}

int isEmpty(struct CharStack* stack)
{
	return stack->top == -1; // returns if empty or not.
}

int IsFull(struct CharStack* stack)
{
	return stack->top == StackSize-1; // returns if  full or not.
}


/*
PushIntoStack: pushes the passed item into the stack
*/
void PushIntoStack(struct CharStack* stack, const char* item)
{
	if(IsFull(stack))
	{
		printf("Stack is full.....\n ");
		return;
	}
	stack->stackitems[++stack->top] = strdup(item); //if stack is not full then push it.
}

/*
PopStack: pops the top of stack.
*/
char* PopStack(struct CharStack* stack)
{
	if(isEmpty(stack))
	{
		printf("stack is empty... nothing to pop \n");
		return '\0';
	}
	char* popitem = stack->stackitems[stack->top];
	stack->top--; //reduces the size by 1
	return popitem; //returns popped item
}

/*
PeekStack: peeks the top of the stack and returns it.
*/
char* PeekStack(struct CharStack* stack)
{
	 if(isEmpty(stack))
        {
                printf("stack is empty... nothing to pop \n");
                return '\0';
        }
	return stack->stackitems[stack->top];
}

/*
PrintStack: print the stack for testing purposes
*/
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
		printf("%s, ", stack->stackitems[i]); // accesses the element and prints it.
	}
	printf(" -- done \n");
}
