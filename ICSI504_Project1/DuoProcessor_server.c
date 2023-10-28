//erver that handles number of files running will be used fro the allocation of bus also.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier; // Declare the barrier

int numberofactiveterminals = 0;
char filenames[2][256];
int filenamesRecieved = 0;


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

int main()
{
  int serverSocket, clientsockets[2];
  struct sockaddr_in serverAddress, clientAddress;

  socklen_t clientAddressLen = sizeof(clientAddress);

  serverSocket = socket(AF_INET, SOCK_STREAM, 0);

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
    clientsockets[i] = accept(serverSocket,(struct sockaddr*) &clientAddress, &clientAddressLen);
    pthread_create(&threads[i] ,NULL, ConnectionHandler, &clientsockets[i]);
  }

  pthread_barrier_wait(&barrier); // Wait for both clients to reach this point

   // Both terminals are ready, you can start execution
   printf("Both terminals are ready. Starting execution.\n");

   int startexecutionflag = 1;

   printf("filenames of the given files by 1 %s \n", filenames[0]);
   printf("filenames of the given files by 1 %s \n", filenames[1]);

   printf("Sending start execution signal to client 0\n");
   send(clientsockets[0], &startexecutionflag, sizeof(int), 0);
   printf("Sending start execution signal to client 1\n");
   send(clientsockets[1], &startexecutionflag, sizeof(int), 0);

   // Close the server socket
   close(clientsockets[0]);
   close(clientsockets[1]);
   close(serverSocket);

   return 0;
}
