#include <stdio.h>
#include "Stack.h"
#include "ABMReader.h"
#include "ABMMethods.h"

int main()
{
	//FILE* abmFile = abmFileReader("abmfiles/operatorsTest.abm");
	//abminstructionrunner(abmFile);
	PushIntoStack("Hello");
	PushIntoStack("World");

	printf("Element at top of the stack : %s\n", PeekStack());
	
	char* data = PopStack();
	printf("Element poped : %s \n", data);
	
	char* data1 = PopStack();
	printf("Element poped : %s \n", data1);
	
	printf("is stack Empty: %s\n", isEmpty()?"true":"false");
	return 0;
}
