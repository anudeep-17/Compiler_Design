//Import statements for libraries
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
//Import statements for all the header files that are required.
#include "ABMMethods.h"
#include "IndexKeywordCommandPair.h"
#include "Stack.h"
#include "Variablearray.h"
#include "addresstovaluedict.h"
#include "VariableManager.h"


//status for global variables:
/*
Notes on how they act:
  1. every read is S
  2. every write is m
  3. every new or invalid variable is I
  they do change on conditions which are handled in the code.
*/
const char* Initialize_Invalid = "Initialize_Invalid";
const char* Shared = "Shared";
const char* Mine = "Mine";

//===========================================================================================================================================================


//is used to send messages to the bus regarding 3 case
/*
case 1 : when global variables are found.
case 2 : when a read on global variable happens.
case 3 : when a write on global variable happens.
*/
void SendToBus(const int clientsocketaddress, const char* message, const char* label)
{
  size_t length = strlen(message) + strlen(label)+1;
  char* combinedmessage = (char*)malloc(length);

  if(combinedmessage != NULL)
  {
    strcpy(combinedmessage, label);
    strcat(combinedmessage, message);

    const char* messagetoserver = combinedmessage;
    size_t message_length = strlen(combinedmessage);
    int bytes_sent = send(clientsocketaddress, messagetoserver, message_length, 0);
    printf("bytes send %d \n\n",bytes_sent);

    free(combinedmessage);
  }
}

//checks if the given char* is a address or not since every address has 0x we expect this is address only if the string has 0x
bool isaddress(char* address)
{
  if(strstr(address, "0x") != NULL)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/*
trimtrailspaces: used to remove forward and backward spaces from a given line from file.
returns: void, manipulates the parameter passed line.
parameter: string line from the file
*/
void trimleadtrailspaces(char *line)
{
    //if the entered line is null, return
    if(line == NULL)
    {
	     return;
    }

    //a pointer to the begining of the line
    char *start = line;

    //runs the loop on characters until a non white space encountered.
    while(isspace(*start))
    {
	     start++; // skips whitespaces at the start
    }

    //if we reached end of the line then return.
    if(*line == '\0')
    {
	     return;
    }

    // a pointer to the end of the line
    char *endchar = line + strlen(line) - 1;

    //iterates from end of the string until a nonwhite space character is encountered
    while(endchar > line && isspace(*endchar))
    {
	     endchar--; //skips white spaces.
    }

    *(endchar + 1) = '\0'; // appends a end of the line char to terminate there.

    if(start != line)
    {
	      memmove(line, start, strlen(start) + 1); // moves the line few character forward to remove spaces.
    }
}
/*==========================================================================Helper functions end ==================================================*/


/*
abmkeywordhelper: used to run through the given instructions read from file and execute accordingly.
parameters:
Pair* pair : contains the file data divided as linenumber, keyword, command pair
CharStack* stack: a empty stack to perform given commands.
VariableContainer* container: contains the a 2d array that stores variablename, a map that stores variablename and its address for each variable basing on scope.
Map* labellocations: contains all label line numbers collected while reading the file.
*/
void abmkeywordhelper(struct Pair* pair, struct CharStack* stack, struct VariableContainer* container, struct Map* labellocations, int clientsocketaddress)
{
    //.data flag
    bool firstlinedata = false;
   //boolean flags to determine encounter of begin, call and return keywords.
    bool Beginevoked = false;
    bool Callevoked = false;
    bool Returnedfromcall = false;

    // count of begin and call encounters to establish scope accordingly
    int numberofbegins = 0;
    int numberofcalls = 0;

    // a charstack that will have the line number pushed when a call is evoked.
    struct CharStack lastcontrol;
    initialize(&lastcontrol); //initializing the stack.

    int returnables = 0; //count of number of returnables in a begin and end that can be accessed from out of its scope.

    //iterates through all the keyword,command pairs that are read from file.
    for (int i = 0; i < pair->currentpairsize; i++)
    {

      char *keyword = pair->Key_CommandPair[i].keyword; // stores the current keyword
      char *command = pair->Key_CommandPair[i].command; // stores current command

      // printf("keyword: %s, command: %s \n", keyword, command);
      // we handle every possible keyword here.
      if(strcmp(keyword, "show") == 0) // show -> prints the argument entered with it.
      {
        printf("%s\n", command);
      }
      else if (strcmp(keyword, "print") == 0) // print -> prints the top of the stack
      {
        //peeks the top element of stack and if it is not null prints it
        char *peek = PeekStack(stack);
        (peek != NULL) ? printf("%s\n", peek)
                       : printf("no element in stack to peek\n");
      }
      else if (strcmp(keyword, "pop") == 0) // pop -> pops element out of stack.
      {
        PopStack(stack);
      }
      else if (strcmp(keyword, "push") == 0) // push -> pushes element into the stack
      {
        PushIntoStack(stack, command);
      }
      else if (*keyword == '-') // - -> pops the first two elements of stack and uses them to perform substraction.
      {
        int right = 0;
        int left  = 0;
        char* rightval = !isEmpty(stack) ? PopStack(stack) : "";
        char* leftval = !isEmpty(stack) ? PopStack(stack) : "";
        right = atoi(rightval);
        if(isaddress(leftval))
        {
          if(FindInGlobalContainerbyaddress(container, leftval) != INT_MIN)
          {
            //if left is address.
            PushIntoStack(stack, getVariableaddressByOffset(container, leftval, -right));
          }
          else
          {
            printf("variable isnt global \n\n\n");
          }
        }
        else
        {
          left = atoi(leftval);
          int result = left - right; //performs substraction
          char resulttochar[10];// a char buffer that is used to convert the int to char* and push it into stack.
          sprintf(resulttochar, "%d", result); // converts result into char
          PushIntoStack(stack, resulttochar); // pushes into stack.
        }
      }
      else if (*keyword == '+')// + -> pops the first two elements of stack and uses them to perform addition.
      {
        int right = 0;
        int left  = 0;
        char* rightval = !isEmpty(stack) ? PopStack(stack) : "";
        char* leftval = !isEmpty(stack) ? PopStack(stack) : "";
        //right should have a number so we can take care of this
        right = atoi(rightval);
        if(isaddress(leftval))
        {
          if(FindInGlobalContainerbyaddress(container, leftval) != INT_MIN)
          {
            //if left is address.
            PushIntoStack(stack, getVariableaddressByOffset(container, leftval, right));
          }
          else
          {
            printf("variable isnt global \n\n\n");
          }
        }
        else
        {
          left = atoi(leftval);
          int result = left + right; //performs substraction
          char resulttochar[10];// a char buffer that is used to convert the int to char* and push it into stack.
          sprintf(resulttochar, "%d", result); // converts result into char
          PushIntoStack(stack, resulttochar); // pushes into stack.
        }

      }
      else if (*keyword == '*')// * -> pops the first two elements of stack and uses them to perform multiplication.
      {
        //pops first two elements on stack.
        int right = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int left = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int result = left * right;//performs multiplication
        char resulttochar[10];// a char buffer that is used to convert the int to char* and push it into stack.
        sprintf(resulttochar, "%d", result);// converts result into char
        PushIntoStack(stack, resulttochar);// pushes into stack.
      }
      else if (*keyword == '/')// / -> pops the first two elements of stack and uses them to perform division
      {
        //pops first two elements on stack.
        int right = !isEmpty(stack) ? atoi(PopStack(stack)) : 1;
        int left = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int result = left / right;//performs division
        char resulttochar[10];
        sprintf(resulttochar, "%d", result);
        PushIntoStack(stack, resulttochar);// pushes into stack.
      }
      else if (strcmp(keyword, "div") == 0) // div -> pops the first two elements of stack and uses them to perform %
      {
        //pops first two elements on stack.
        int right = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int left = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int result = left % right;//performs %
        char resulttochar[10];
        sprintf(resulttochar, "%d", result);
        PushIntoStack(stack, resulttochar);// pushes into stack.
      }
      else if (*keyword == '&') // & -> pops the first two elements of stack and uses them to perform &
      {
        //pops first two elements on stack.
        int right = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int left = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        //computes the boolean result of &
        bool result = ((left == 1) ? true : false) & ((right == 1) ? true : false);
        PushIntoStack(stack, result ? "1" : "0"); // pushes into stack.
	    }
      else if (*keyword == '|')// | -> pops the first two elements of stack and uses them to perform |
      {
        //pops first two elements on stack.
        int right = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int left = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        //computes the boolean result of |
        bool result = ((left == 1) ? true : false) | ((right == 1) ? true : false);
        PushIntoStack(stack, result ? "1" : "0"); // pushes into stack.
      }
      else if (*keyword == '!')// ! -> pops the first two elements of stack and uses them to perform !
      {
        int argument = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        bool result = ((argument == 1) ? false : true);
        PushIntoStack(stack, result ? "1" : "0"); // pushes into stack.
      }
      else if (strcmp(keyword, "<>") == 0)// <> -> pops the first two elements of stack and uses them to perform <>
      {
        int right = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int left = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        bool result = (left != right);
        PushIntoStack(stack, result ? "1" : "0");
      }
      else if (strcmp(keyword, "<=") == 0)// <= -> pops the first two elements of stack and uses them to perform <=
      {
        int right = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int left = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        bool result = left <= right;
        PushIntoStack(stack, result ? "1" : "0");
      }
      else if (strcmp(keyword, ">=") == 0)// >= -> pops the first two elements of stack and uses them to perform >=
     {
        int right = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int left = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        bool result = left >= right;
        PushIntoStack(stack, result ? "1" : "0");
      }
      else if (strcmp(keyword, "<") == 0)// < -> pops the first two elements of stack and uses them to perform <
      {
        int right = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int left = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        bool result = left < right;
        PushIntoStack(stack, result ? "1" : "0");
      }
      else if (strcmp(keyword, ">") == 0)// > -> pops the first two elements of stack and uses them to perform >
      {
        int right = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        int left = !isEmpty(stack) ? atoi(PopStack(stack)) : 0;
        bool result = left > right;
        PushIntoStack(stack, result ? "1" : "0");
      }
      else if (strcmp(keyword, "rvalue") == 0)// rvalue -> gets the value of the given variable name using its address and pushes it into stack.
      {
        /*
          SCOPE NOTES:
            0.global introduced ...... using .data .int ...
            1.if only begin[s] is/are evoked then we can access the above scope and get the value of given variable
            2.if Begin & Call is/are evoked then we should only have access to current scope to find given variable.
            3.Mathamatically everytime ** n(begin) > n(call) **  we can access the scope above current and find a variable else NO
        */
        if(FindInGlobalScope(container, command) != INT_MIN && firstlinedata)
        {
          if(!InGlobalScopeIsItGivenStatus(container, getaddressfromGlobalContainer(container, command), Shared))
          {
            InGlobalScopeSetStatus(container, getaddressfromGlobalContainer(container, command), Shared);
          }
          int value = FindInGlobalScope(container, command);
          char charvalue[10];
          sprintf(charvalue, "%d", value);
          PushIntoStack(stack, charvalue); //push it into stack
          printf("\n\n");
          printcontainers(container);
          printf("\n\n");
        }
        else if((Beginevoked && !Callevoked) || (numberofbegins != numberofcalls)) // if only begin is encountered or n(begins) are more than n(calls)
        {
          //******************* we can access scope above ******************************
          if(FindInAboveScope(container, command) != INT_MIN) // if the scope above has this variable
          {
             int value = FindInAboveScope(container, command); //gets its values
      	     char charvalue[10]; // a buffer to convert it to char
             sprintf(charvalue, "%d", value);
      	     PushIntoStack(stack, charvalue); //push it into stack
          }
          else if(FindInContainer(container, command) != INT_MIN)// if the current scope has this variable
          {
            int value = FindInContainer(container, command);//gets its values
            char charvalue[10];
            sprintf(charvalue, "%d", value);
            PushIntoStack(stack, charvalue);//push it into stack
          }
          else
          {
            //both above and current dont have this
            //make a new variable and push 0 into stack.
            insertIntoContainer(container, command, 0);
          	PushIntoStack(stack, "0");
          }

        }
        else // else if begin never encountered, or being and call are encountered
        {
          // scope above current cannot be accessed.
          if(FindInContainer(container, command) != INT_MIN) // if the current scope has this variable
          {

            int value = FindInContainer(container, command); //gets its values
            char charvalue[10];
            sprintf(charvalue, "%d", value);
            PushIntoStack(stack, charvalue);//push it into stack
          }
          else
          {
            //if current dont have this
            //make a new variable and push 0 into stack.
            insertIntoContainer(container, command, 0);
          	PushIntoStack(stack, "0");
          }

        }

      }
      else if (strcmp(keyword, "lvalue") == 0)// lvalue -> gets the address of the given variable name using and pushes it into stack.
      {

        /*
        SCOPE NOTES:
          1. under any circumstance of begin/call/return when we make a new variable it is always in the current scope
          2. if we have encountered call and then encountered return, in begin/end everything after this is accessible its above scope so we count them and make them accessible.
        */
        if(FindInGlobalScope(container, command) != INT_MIN && firstlinedata)
        {
            PushIntoStack(stack, getaddressfromGlobalContainer(container, command)); //gets its address and push it to the stack
        }
        else if (FindInContainer(container, command) != INT_MIN) // if current container has this variable.
        {
            PushIntoStack(stack, getaddressfromContainer(container, command)); //gets its address and push it to the stack
        }
        else
        {
          //if current dont have this
          //make a new variable and push its address into stack.
          insertIntoContainer(container, command, 0);
          PushIntoStack(stack, getaddressfromContainer(container, command));
        }

        //when we encoutnerd a return while begin is evoked we count them to make them accesible.
        if(Returnedfromcall == true && Beginevoked == true)
        {
          returnables++;
        }

      }
      else if (strcmp(keyword, ":=") == 0)// := -> gets address and a value from stack and puts the value in the address and pops both,
      {
        int value = !isEmpty(stack) ? atoi(PopStack(stack)) : 0; //gets the value
        char *address = !isEmpty(stack) ? PeekStack(stack) : NULL; //gets the address

        if(FindInGlobalContainerbyaddress(container, address) != INT_MIN && firstlinedata)
        {
          //updates the address to M as we write here
          if(!InGlobalScopeIsItGivenStatus(container, address, Mine))
          {
            InGlobalScopeSetStatus(container, address, Mine);
          }
          updateGlobalContainerbyaddress(container, address, value);

          printf("\n\n");
          printcontainers(container);
          printf("\n\n");

          PopStack(stack);
        }
        //if the address is found in the container then we update it and pop it from stack else its error handling
        else if (FindInContainerbyaddress(container, address) != INT_MIN)
        {
          updateContainerbyaddress(container, address, value);
        	PopStack(stack);
        }
        else
        {
          //error handling...................
        	printf("given l value never "
        	       "encountered...\n");
        }

      }
      else if (strcmp(keyword, "copy") == 0)// copy -> copies the top element of stack into stack again.
      {
        PushIntoStack(stack, PeekStack(stack)); //pushes top element of stack into it again.
      }
      else if (strcmp(keyword, "label") == 0) // label -> marks the line to jump
      {
        char key[100]="";
        // since we already have a map with labels and their location we double check if we encountered it before
        if(find(labellocations,strcat(strcat(strcat(key, keyword), " "), command)) != INT_MIN)
        {
          continue;
        }
        else
        {
          //if not then its a error in code,
          printf("label asked is not in given map, failed \n");
          return;
        }

      }
      else if (strcmp(keyword, "call") == 0)// call-> jumps to the given target
      {
        char key[100]= "";
        strcat(strcat(strcat(key, "label"), " "), command);
        int index = find(labellocations, key); //we find the index of desired label

        char numtochar[10];
        sprintf(numtochar, "%d", i);
        PushIntoStack(&lastcontrol, numtochar); // we convert our current location into char and pushes it into lastcontrol stack so that on return from call we come here.

        Callevoked = true; // making the flag true
        numberofcalls++; // updating the number of call
        i = index; // jumping to the given target.
      }
      else if(strcmp(keyword, "return") == 0) // return -> returns from subroutine and gives the control access back
      {
        Returnedfromcall = true; // sets the flag true
        numberofcalls--; // since we finished a call we reduce number of active calls by 1
        //in case number of calls are 0 we make the falg false **useful during recursion.
        if(numberofcalls == 0)
        {
            numberofcalls = false;
        }
        int lastcontrolindex = atoi(PopStack(&lastcontrol)); // get the value on top of the stack from last control to pop and use it.
        i = lastcontrolindex; // jumps back
      }
      else if(strcmp(keyword, "begin") == 0) // begin -> Marks beginig of a parameter passing and call
      {
        Beginevoked = true; // marks Begin flag true
        /*
        as begin starts the new scope for variables we make a new scope in our variable container.
        */
        NewScope(container);
        numberofbegins++; // updates number of begin calls.
      }
      else if(strcmp(keyword, "end") == 0)// end -> Marks ending of a parameter passing and call
      {
        if(returnables > 0) // if there are returnables we make them accesible to the above scope.
        {
          MakeReturnablesAccesible(container, returnables); // every returnable variables permission are altered and shifted to its above scope to make it accessible.
          returnables = 0; // and make the returnables 0
        }
        DeleteScope(container); // delete the scope of begin and end as we returned what we want
        numberofbegins--; // reduce number of begins as we have finished one of them
        if(numberofbegins == 0) //in case number of begins are 0 we make the falg false **useful during recursion.
        {
            Beginevoked = false;
        }

      }
      else if(strcmp(keyword, "goto") == 0)// goto -> Next instruction is taken from statement with label l
      {
        char key[100]= "";
        strcat(strcat(strcat(key, "label"), " "), command);
        int index = find(labellocations, key); // find the index where the given lable is and jumps there.
        i=index; // JUMPS there

      }
      else if(strcmp(keyword, "gofalse") == 0)// gofalse -> Pops top of stack and if it is  0 then jumps to given target.
      {

        char* temp = PopStack(stack); // pops stack
        if(strcmp(temp, "0") == 0) // if it is 0
        {
          char key[100]= "";
          strcat(strcat(strcat(key, "label"), " "), command);
          int index = find(labellocations, key); // find label index and jump there
          i=index;
        }

      }
      else if(strcmp(keyword, "gotrue") == 0)// gofalse -> Pops top of stack and if it is non 0 then jumps to given target.
      {
        char* temp = PopStack(stack);// pops stack

        if(strcmp(temp, "0") != 0)// if it is 0
        {
          char key[100]= "";
          strcat(strcat(strcat(key, "label"), " "), command);
          int index = find(labellocations, key);// find label index and jump there
          i=index;
        }

      }
      else if(strcmp(keyword, ".data") == 0)
      {
        firstlinedata = true;
      }
      else if(strcmp(keyword, ".int") == 0 && firstlinedata)
      {
        //send global variables to MemoryBus to let the Bus Know
        const char* label = "GlobalVars: ";
        SendToBus(clientsocketaddress, command, label);

        char* token = strtok(command, " ");
        while(token != NULL)
        {
          insertIntoContainer(container, token, 0); //initialize all global
          InGlobalScopeSetStatus(container, getaddressfromGlobalContainer(container, token), Initialize_Invalid);
          token = strtok(NULL, " ");
        }
        printcontainers(container);
      }
      else if(strcmp(keyword, ".text") == 0 && firstlinedata)
      {
        NewScope(container); //Now we aint in global scope => moved a scope into the program segmentation
      }
      else if(strcmp(keyword, "halt") == 0) // halt -> exit the excecution
      {
        exit(0); //safe exit.
        return;
      }
      else if(strcmp(keyword, ":&") == 0)
      {
        //assign one memory address to the other.
        char* rightaddress = PopStack(stack);
        char* leftaddress = PopStack(stack);

        // now we need left address synced with right address.
        setSyncBetween(container, leftaddress, rightaddress);
      }
   }
}

/*
abminstructionfile: takes instruction FILE, reads it into a Pair of linenumber, keyword, command and uses it call abmkeywordhelper to excecute.
parameter: a file with all instructions
*/
void abminstructionrunner(int clientsocketaddress, FILE* abminstructionfile)
{

    if (abminstructionfile == NULL)
    {
	     perror("Error opening file");
	     return;
    }

    //a line that can store 1024 characters.
    char Eachline[1024];
    char *firstspace; // checks for first space to split on
    char *keyword; //keyword storer
    char *command; // command storer.

    //Pair, Labellocations
    struct Pair pair; // to store pair
    struct Map labellocations; // to store label and its linenumber
    struct CharStack stack; // helper stack while running abmfile
    struct VariableContainer container; // VariableContainer to store variables basing on scope while running abmfile

    int linenumber = 0; // line number counter

    initializePair(&pair);
    initializeMap(&labellocations);
  	initialize(&stack);
  	initializeContainer(&container);

    //loops through each line of abminstructionfile
    while (fgets(Eachline, sizeof(Eachline), abminstructionfile) != NULL)
    {
      trimleadtrailspaces(Eachline); //removes leading and trailing space of eachline.
      //gets the first index of a space in current line.
    	firstspace = strchr(Eachline, ' ');

    	if (firstspace != NULL) // if first space is not null then we split the eachline into before space and after space
      {
    	    *firstspace = '\0';
    	    keyword = Eachline; //everything before space
    	    command = firstspace + 1; //everything after space.
    	}
      else
      {
    	    keyword = Eachline; //since no space encoutnered, it is one word hence just keyword.
    	    command = NULL;
    	}

    	trimleadtrailspaces(keyword); //incase of any removes spaces again
      trimleadtrailspaces(command); //incase of any removes spaces again

      if((int)*keyword != 0) // if keyword is not null
      {
        addPair(&pair, linenumber, keyword, command); // add linenumber <-> keyword <-> command pair to pair
        if(strcmp(keyword, "label") == 0) // if keyword is label then
        {
          char key[100] = "";
          strcat(strcat(strcat(key, keyword), " "), command);
          insert(&labellocations,key,linenumber); //insert a label command and its linenumber into map.
        }
        linenumber++; // move to next line.
      }
    }
    //calls abmwordhelp on to run all the keyword, command.
    abmkeywordhelper(&pair, &stack, &container, &labellocations, clientsocketaddress);
}
