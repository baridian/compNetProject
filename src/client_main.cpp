#include <iostream>
#include <winsock2.h>
#include <assert.h>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#include "Buffer.h"
#include "wrap_string.h"
#include "process_binary.h"

//how to build: add make to PATH
//make -f client_main.mak
//must be in src directory!!
SOCKET makeSocket(const char *address, int portNumber)
{
	WSADATA wsaData = {0};
	struct sockaddr_in saServer;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Set up the sockaddr structure
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *) *gethostbyname(address)->h_addr_list));
	saServer.sin_port = htons(portNumber);

	if(connect(mySocket, (SOCKADDR *) &saServer, sizeof(saServer)) != 0)
	{
		fprintf(stderr,"ERROR: could not establish connection to server.\n");
		exit(1);
	}
	printf("connection established\n");
	return mySocket;
}

//attempt to display messages
void displayMessages(SOCKET mySocket)
{
	char *buffer = (char *)malloc(sizeof(char)*255);
	int bufferLength = 255;
	int length;

	Sleep(100);

	//while not eof
	while(strcmp(buffer, "end of file") != 0)
	{
		//free(buffer);
		//buffer = (char *)calloc(255,sizeof(char));

		//printf("waiting for message\n");
		length = recv(mySocket, buffer, bufferLength, 0);
		if (length < 0)
		{
			printf("recv failed: %d\n", WSAGetLastError());
			exit(1);
		}

		//if file keyword
		//printf("buffer = %s\n",buffer);
		if (strcmp(buffer, "file") == 0)
		{
			//printf("file detected\n");
			send(mySocket,"file",(int)strlen("file") + 1, 0); //send file request
			recv(mySocket, buffer, 255, 0); //recieve file extension from server
			readFile(mySocket, buffer);
			//printf("exited readFile function\n");
			sprintf(buffer,"end of file"); //prevent continued looping
		}

		//if end of file keyword
		if(strcmp(buffer,"end of file") != 0)
		{
			unwrap(buffer);
			printf(">: %s\n",buffer);
			send(mySocket,"next",(int)strlen("next") + 1,0);
		}

	}
	//printf("received EOF flag\n");
	free(buffer);
}

int main()
{
	SOCKET serverSocket;
	char *input = (char *)malloc(sizeof(char)*255);
	char *buffer = (char *)malloc(sizeof(char)*255);
	char *commandBuffer = (char *)malloc(sizeof(char)*255);
	char *kbBuffer = (char *)malloc(sizeof(char) * 255);
	FILE *source;
	FILE *outgoing;
	int bufferIndex = 0;
	int kbChar;

	printf("please enter server address: ");
	scanf("%s",input);
	getchar();
	serverSocket = makeSocket(input,5150);

	printf("type exit to abort conversation or send to transfer a file\n");
	while(strcmp(input,"exit\n") != 0)
	{
		/*scanf("%[^\n]s", input);
		getchar();
		strcat(input,"\n");*/

		if(_kbhit()) //non-blocking scanf
		{
			kbChar = _getch();
			printf("%c", (int) kbChar);
			if (kbChar == '\r')
			{
				printf("\n");
				kbBuffer[bufferIndex++] = '\n';
				kbBuffer[bufferIndex++] = '\0';
				bufferIndex = 0;
				strcpy(input, kbBuffer);
			}
			else
			{
				kbBuffer[bufferIndex++] = (char) kbChar;
			}
		}

		if(strcmp(input,"exit\n") != 0 && strlen(input) > 0)
		{
			if(strcmp(input, "send\n") == 0) //file transfer handler
			{
				printf("enter path: ");
				scanf("%s",buffer);
				getchar();
				source = fopen(buffer,"r");
				if(source == NULL)
					printf("warning: invalid file\n");
				else
				{
					outgoing = fopen("client_out.txt","a");
					fprintf(outgoing,"\n%s\n", strstr(buffer, ".") + 1);
					fclose(outgoing);
					sprintf(commandBuffer,"type %s >> client_out.txt", buffer);
					system(commandBuffer);
					fclose(source);
					outgoing = fopen("client_out.txt","r");
					sendBinaryFile(serverSocket, outgoing, "client_out.txt");
					system("del client_out.txt");
					send(serverSocket, "end of file", (int)strlen("end of file") + 1, 0);
					recv(serverSocket, buffer, 255, 0);
				}


			}
			else //text message
			{
				//printf("sending \"%s\"\n",input);
				wrap(input);
				send(serverSocket, input, (int) strlen(input), 0);
				recv(serverSocket, input, 255, 0);
				if (strcmp(input, "ack") != 0)
				{
					fprintf(stderr, "ERROR: received %s, not ack\n", input);
					exit(1);
				}
				//printf("ack'ed\n");
				//printf("sent read command\n");
			}
		}
		else if(strcmp(input,"exit\n") == 0)
			break;

		//printf("displaying\n");
		send(serverSocket, "read", (int) strlen("read") + 1, 0); //update
		//printf("sent read\n");
		displayMessages(serverSocket);
		//printf("dispayed\n");

		input[0] = '\0';
	}
	shutdown(serverSocket, SD_SEND);
	closesocket(serverSocket);
	return 0;
}