#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

int Stacksize = 10;
int stack[10];
int top = -1;

int isEmpty() {
  if (top == -1)
    return 1;
  else
    return 0;
}

int IsFull() {
  if (top == Stacksize) {
    return 1;
  } else {
    return 0;
  }
}

int PeekStack() { return stack[top]; }

int PopStack() {
  int temp;
  if (!isEmpty()) {
    temp = stack[top];
    top = top - 1;
    return temp;
  } else {
    printf("Stack empty \n");
  }
}

int PushIntoStack(int data) {
  if (!IsFull()) {
    top = top + 1;
    stack[top] = data;
  } else {
    printf("stack is full\n");
  }
}
