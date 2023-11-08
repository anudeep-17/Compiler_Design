//erver that handles number of files running will be used fro the allocation of bus also.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <limits.h>
#include <time.h>

#include "VariableManager.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier; // Declare the barrier

int numberofactiveterminals = 0;
char filenames[2][256];
int filenamesRecieved = 0;
int UseOnlyOneFile = 0;
struct VariableContainer MemoryBus; // VariableContainer to store variables basing on scope while running abmfile
struct Map lastedited;
int clients[2];
//=================================================================Memory BUS ======================================================================================



//=================================================================server===========================================================================================
void handleTimeout()
{
    printf("\n\n Timeout: after first client connection, second client havent established under 15 secs, moving forward with 1 file \n\n");
    UseOnlyOneFile = 1;
}

void* ConnectionHandler(void* args)
{
  //handles multiple connections to the server and thread each connection.
  int clientsocketsfromeachterminal = *((int*)args);
  char buffer[256];

  recv(clientsocketsfromeachterminal, buffer, sizeof(buffer), 0);

  if(filenamesRecieved < 2)
  {
    strcpy(filenames[filenamesRecieved], buffer);
    filenamesRecieved++;
  }

  pthread_mutex_lock(&mutex);
  numberofactiveterminals++;

  if(numberofactiveterminals == 2)
  {
     pthread_barrier_wait(&barrier); // Use the barrier to synchronize
  }

  pthread_mutex_unlock(&mutex);

  return NULL;
}

void* StartExcecution_SignalReceiver(void *args)
{
  int clientsocket = *((int*)args);
  int startexecutionflag = 1;
  int bytes_received;

  send(clientsocket, &startexecutionflag, sizeof(int), 0);
  while(1)
  {
    // loops for ever listening to the client and waits until it ends.
    char buffer[1024];
    bytes_received = recv(clientsocket, buffer, sizeof(buffer), 0);
    if(bytes_received > 0)
    {
      printf("recieved from client %d : %s and lenof buffer %ld \n\n", clientsocket, buffer, strlen(buffer));
      char* token = strtok(buffer, ":");

      //locks the task on MemoryBus here.
      if(token !=NULL)
      {
        if(strcmp(token, "GlobalVars") == 0)
        {
          token = strtok(NULL," ");
          while(token != NULL)
          {

            // printf("var: %s \n", token);
            pthread_mutex_lock(&mutex);
            if(FindInContainer(&MemoryBus, token) == INT_MIN)
            {
              // printf("added var: %s \n", token);
              insertIntoContainer(&MemoryBus, token, 0);
            }
            pthread_mutex_unlock(&mutex);
            // printcontainers(&MemoryBus);


            token = strtok(NULL, " ");
          }
        }
        else if(strcmp(token, "Write") == 0)
        {
          // writes the value recieved from core to the variable container while locking it for establishing concurrency
          char* nameofvariable;
          int value;
          token = strtok(NULL,",");
          token++;
          //name of variable
          nameofvariable = token;
          // printf("\n\n %s\n\n", token);
          token = strtok(NULL, ",");
          value = atoi(token);
          // printf("\n\n %s\n\n", token);

          pthread_mutex_lock(&mutex);
          if(FindInContainer(&MemoryBus, nameofvariable) != INT_MIN)
          {
            updateGlobalContainerbyaddress(&MemoryBus, getaddressfromGlobalContainer(&MemoryBus, nameofvariable), value);
            pthread_mutex_unlock(&mutex);

            pthread_mutex_lock(&mutex);
            insert(&lastedited, nameofvariable, clientsocket);
            InsertStatus(&lastedited, nameofvariable, "Initialize_Invalid");
            pthread_mutex_unlock(&mutex);
          }
          else
          {
            pthread_mutex_unlock(&mutex);
          }

          // printcontainers(&MemoryBus);
        }
        else if(strcmp(token, "Read") == 0)
        {
            // reads the value recieved from core to the variable container while locking it for establishing concurrency and sends it to the core
            char* nameofvariable;
            int value;

            token = strtok(NULL, " ");
            // printf("\n\n %s \n\n", token);
            nameofvariable = token;
            //locks the lastedited and unlocks once works is done
            pthread_mutex_lock(&mutex);
            insert(&lastedited, nameofvariable, clientsocket);
            InsertStatus(&lastedited, nameofvariable, "Shared");
            pthread_mutex_unlock(&mutex);

            int timeout = 0.4;
            time_t starttime = time(NULL);
            while(time(NULL) - starttime < timeout)
            {
                //locks the variable  and unlocks once works is done
              pthread_mutex_lock(&mutex);
              if(find(&lastedited, nameofvariable) != clientsocket)
              {
                pthread_mutex_unlock(&mutex);
                break;
              }
              else
              {
                  pthread_mutex_unlock(&mutex);
              }
            }

            pthread_mutex_lock(&mutex);
            value = FindInContainer(&MemoryBus, nameofvariable);
            pthread_mutex_unlock(&mutex);


            // send to the core the value it want.
            char charval[10];
            sprintf(charval, "%d", value);
            send(clientsocket, charval, 10, 0);

            pthread_mutex_lock(&mutex);
            insert(&lastedited, nameofvariable, clientsocket);
            InsertStatus(&lastedited, nameofvariable, "Shared");
            pthread_mutex_unlock(&mutex);

            // printf("%s : %d \n\n", nameofvariable, value);
            // pthread_mutex_unlock(&mutex);
        }
        else if(strcmp(token, "Changes?") == 0)
        {
          // send the changes if any
            printMap(&lastedited);
            char sendtocore[1024];
            strcpy(sendtocore, StructureStringForSignal(&lastedited, clientsocket));
            send(clientsocket, sendtocore, 1024, 0);
        }
        else if(strcmp(token, "Sync") == 0)
        {
          // setup the sync between recieved variables
         char variablethatwillsync[50];
         char variableforsync[50];

          token = strtok(NULL, " ");
          printf("token 2:%s %s\n", token, getaddressfromGlobalContainer(&MemoryBus, token));
          strcpy(variablethatwillsync, getaddressfromGlobalContainer(&MemoryBus, token));

          token = strtok(NULL, " ");
          printf("token 2:%s %s\n", token, getaddressfromGlobalContainer(&MemoryBus, token));
          strcpy(variableforsync, getaddressfromGlobalContainer(&MemoryBus, token));

          pthread_mutex_lock(&mutex);
          printf("[%s], [%s] \n\n", variablethatwillsync, variableforsync);
          setSyncBetween(&MemoryBus, variablethatwillsync, variableforsync);
          printcontainers(&MemoryBus);
          pthread_mutex_unlock(&mutex);
        }
        else if(strcmp(token, "END") == 0)
        {
          break;
        }
      }


      // pthread_mutex_unlock(&mutex);
    }
  }
}

void CloseConnections(const int clientsockets[], const int serverSocket, int numberofconnections)
{
  if(numberofconnections == 1)
  {
    //end both sockets
    close(clientsockets[0]);
    close(serverSocket);
    return;
  }
  // Close the server socket and client sockets
  close(clientsockets[0]);
  close(clientsockets[1]);
  close(serverSocket);
}

int main()
{
  int serverSocket, clientsockets[2];
  struct sockaddr_in serverAddress, clientAddress;

  socklen_t clientAddressLen = sizeof(clientAddress);

  serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  // reusability of the server address to make a quick connection nonconflictable.
  int reusableopt = 1;
  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reusableopt, sizeof(int)) == -1)
  {
       perror("setsockopt");
       exit(1);
  }

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
  serverAddress.sin_port = htons(59911);

  bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
  pthread_barrier_init(&barrier, NULL, 2); // Initialize the barrier for 2 clients

  //testing purpose ========================================================
  char serverIP[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(serverAddress.sin_addr), serverIP, INET_ADDRSTRLEN);

  printf("Started the server, with address %s\n", serverIP);
  //end testing purpose ========================================================


  listen(serverSocket, 2);

  pthread_t threads[2];

  for(int i = 0; i<2; i++)
  {
    if(i == 0)
    {
      clientsockets[i] = accept(serverSocket,(struct sockaddr*) &clientAddress, &clientAddressLen);
      pthread_create(&threads[i] ,NULL, ConnectionHandler, &clientsockets[i]);
      printf("\n Recieved CLIENT 1 -- Will be waiting for second client for 15 secs \n");
    }
    else
    {
      // after first client connectiosn waits for 15 secs and if no connection made will move on.
      struct timeval timeout;
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
      fd_set readfds;
      FD_ZERO(&readfds);
      FD_SET(serverSocket, &readfds);

      int secondconnection = select(serverSocket+1, &readfds, NULL, NULL, &timeout);

      if(secondconnection == 0)
      {
        handleTimeout();
      }
      else if (secondconnection > 0 && FD_ISSET(serverSocket, &readfds))
      {
        // accepts second client and makes its thread.
        clientsockets[i] = accept(serverSocket,(struct sockaddr*) &clientAddress, &clientAddressLen);
        pthread_create(&threads[i] ,NULL, ConnectionHandler, &clientsockets[i]);
      }
    }

  }

  //initializes Bus for the server work.
  initializeContainer(&MemoryBus);
  initializeMap(&lastedited);

  if(UseOnlyOneFile == 0)
  {
    pthread_barrier_wait(&barrier); // Wait for both clients to reach this point

    printf("Both terminals are ready. Starting execution.\n");
    printf("From client %d filename Recieved %s \n",clientsockets[0], filenames[0]);
    printf("From client %d filename Recieved %s \n",clientsockets[1], filenames[1]);
    //a global copy of clients.
    clients[0] = clientsockets[0];
    clients[1] = clientsockets[0];
    pthread_create(&threads[0], NULL,StartExcecution_SignalReceiver,&clientsockets[0]);
    pthread_create(&threads[1], NULL,StartExcecution_SignalReceiver,&clientsockets[1]);

    /*
    lets both the threads finish and only then ends the server.
    They will run concurrently and independently, processing data from their respective client sockets.
    There is no guarantee of the order in which they start or complete their execution, as thread scheduling is managed by the operating system and can vary.

    */
    for (int i = 0; i < 2; i++)
    {
       pthread_join(threads[i], NULL);
    }

    CloseConnections(clientsockets, serverSocket, 2);
  }
  else
  {
    //only one file is encountered and we run it.
    printf("From client %d filename Recieved %s \n",1, filenames[0]);
    //a thread is not required as it will run together anyway.
    StartExcecution_SignalReceiver(&clientsockets[0]);
    CloseConnections(clientsockets, serverSocket, 1);
  }



   return 0;
}
