#include <iostream>
#include <winsock2.h>
#include <assert.h>
#include <unistd.h>
#include <zconf.h>
#include "Buffer.h"
#include "wrap_string.h"

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

//function to perform file read in
void readFile(SOCKET mySocket, char *type)
{
	char *buffer = (char *)malloc(sizeof(char) * 255);
	int length;
	FILE *output = (FILE *)buffer;
	int i;
	int outputFileNum = 0;

	//send request for next line of file
	send(mySocket,"file next",(int)strlen("file next") + 1, 0);

	//printf("\"%s\"\n",type);

	//attempt opening new files until one files to open, meaning the name hasn't been used
	//printf("output = %p\n",output);
	while(output != NULL)
	{
		sprintf(buffer, "out%d.%s", ++outputFileNum, type);
		output = fopen(buffer,"r");
		fclose(output);
	}
	//printf("desired file name = %s",buffer);
	output = fopen(buffer,"wb");//create file with this name
	if(output == NULL)
	{
		fprintf(stderr, "ERROR: could not create output file\n");
		exit(1);
	}

	//each file line is 255 long unless it's the last one, in which case it's shorter.
	do
	{
		length = recv(mySocket,buffer, 255, 0);
		//printf("length = %d\n",length);
		fwrite(buffer,sizeof(char),length,output);
		send(mySocket,"file next",(int)strlen("file next") + 1, 0);
	} while(length == 255);

	//printf("check for eof\n");
	//confirm that end of file is received
	recv(mySocket, buffer, 255, 0);
	if(strcmp(buffer, "end of file") != 0)
	{
		fprintf(stderr, "expected end of file token, received \"%s\"\n",buffer);
		exit(1);
	}
	fclose(output);
	free(buffer);
	printf("file received!\n");
}

//attempt to display messages
void displayMessages(SOCKET mySocket)
{
	char *buffer = (char *)malloc(sizeof(char)*255);
	int bufferLength = 255;
	int length;

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
	char *input = (char *)malloc(sizeof(char)*255);
	SOCKET serverSocket;
	Buffer buffer;

	printf("please enter server address: ");
	scanf("%s",input);
	getchar();
	serverSocket = makeSocket(input,80);

	printf("send exit to abort conversation\n");
	while(strcmp(input,"exit\n") != 0)
	{
		scanf("%[^\n]s", input);
		getchar();
		strcat(input,"\n");
		if(strcmp(input,"exit\n") != 0)
		{
			wrap(input);
			send(serverSocket, input, (int) strlen(input), 0);
			recv(serverSocket, input, 255, 0);
			if (strcmp(input, "ack") != 0)
			{
				fprintf(stderr, "ERROR: received %s, not ack\n", input);
				exit(1);
			}
			//printf("ack'ed\n");
			send(serverSocket, "read", (int) strlen("read") + 1, 0);
			//printf("sent read command\n");
			displayMessages(serverSocket);
		}
	}
	shutdown(serverSocket, SD_SEND);
	closesocket(serverSocket);
	return 0;
}