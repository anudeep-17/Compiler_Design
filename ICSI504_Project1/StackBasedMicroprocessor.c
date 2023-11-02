#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ABMReader.h"
#include "ABMMethods.h"

//Main FILE thats runs all the abm files given to produce output.
// this should behave as client to the server where we check for duocore processors.
//gcc Stack.c ABMMethods.c StackBasedMicroprocessor.c ABMReader.c Variablearray.c addresstovaluedict.c IndexKeywordCommandPair.c VariableManager.c -o Project1

void ExcecutionHelper(int clientSocket, char* filename)
{
	// uses the given abm files and execute
	FILE* abmFile = abmFileReader(filename);
	abminstructionrunner(clientSocket, abmFile);
}

int main(int argc, char* argv[])
{
		if(argv[1] != NULL)
		{
			//===================================== server - client connection establishment ===============================================================
			int clientSocket;
			struct sockaddr_in serverAddress;
			clientSocket = socket(AF_INET, SOCK_STREAM, 0);

			//error handling
			if (clientSocket == -1)
			{
				perror("Socket creation failed");
				return 1;
			}

			serverAddress.sin_family = AF_INET;
			serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
			serverAddress.sin_port = htons(59911);

			if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
			{
				//if connection failed
        perror("Connection failed");
        close(clientSocket);
        return 1;
    	}

			send(clientSocket, argv[1], strlen(argv[1]), 0);

			// ===================================================================== server - client connection established, filename also sent ==========================

			// ==================================================== gets signal from server to start excecuting abm file =================================================
			int signal;
	 		int bytes_received;

			while (1)
			{
				bytes_received = recv(clientSocket, &signal, sizeof(int), 0);
        if (bytes_received == sizeof(int))
					{
						// got signal from server to start excecution.
            printf("Received signal to start excecution\n\n\n");

						// calls the ExcecutionHelper to evoke the abm file runner.

						ExcecutionHelper(clientSocket, argv[1]);

            break; // Exit the loop when the desired data is received
        	}
    	}

			// ==========================================================end the client socket and end===============================================================
			close(clientSocket);

		}
		else
		{
			printf("NO FILE NAME EVER ENTERED\n");
		}

	return 0; //just to exit
}
