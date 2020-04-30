#include <stdio.h>
#include <stdlib.h>
#include "Server.h"
#include <winsock2.h>
#include <assert.h>
#include "process_binary.h"



//function to send the server input file
//client calls sendData with read
//each line is requested with next
//end of file token is last line sent
void sendData(SOCKET socket, const char *file)
{
	FILE *input = fopen(file, "r");
	char *buffer = (char *)malloc(sizeof(char) * 255);

	while(fscanf(input,"%[^\n]s",buffer) == 1)
	{
		if(strcmp(buffer, "file") == 0) //handle file transfer request
			sendBinaryFile(socket, input, file);
		else //regular text message
		{
			fgetc(input);
			send(socket, buffer, (int) strlen(buffer) + 1, 0);
			printf("sent line\n");
			recv(socket, buffer, 255, 0);
			if(strcmp(buffer, "next") != 0) //confirm request for next line
			{
				printf("received request for next\n");
				fprintf(stderr, "unexpected token \"%s\"\n", buffer);
				exit(1);
			}
		}
	}
	printf("\nend of file reached\n"); //send eof marker
	sprintf(buffer, "end of file");
	send(socket, buffer, (int)strlen(buffer) + 1, 0);

	printf("cleaning up\n");
	//cleanup
	free(buffer);
	fclose(input);
	input = fopen(file, "w"); //wipe input
	fclose(input);
}


int main()
{
	int receivedLength;
	int bufferLength = 255;
	char *buffer = (char *)malloc(sizeof(char) * 255);
	Server *server = Server::getServer();
	FILE *output;
	server->acceptConnection();

	do //while connection open
	{
		receivedLength = server->receive(buffer, bufferLength);
		printf("received\n");
		if(strcmp(buffer, "read") == 0) //if special read command received
		{
			printf("attempting read\n");
			sendData(server->getISock(), "..\\server_out.txt");
		}
		else //message sent to server
		{
			send(server->getISock(),"ack",(int)strlen("ack") + 1,0); //reply with ack
			printf("sent ack\n");

			output = fopen("..\\server_in.txt", "a"); //append to daemon incoming file
			if(output == NULL)
			{
				fprintf(stderr, "could not open output file\n");
				exit(1);
			}

			buffer[receivedLength] = '\0';
			printf("received \"%s\"\n", buffer);
			fprintf(output, "%s", buffer);
			fclose(output);
			printf("wrote file\n");
		}
	}
	while(receivedLength > 0);


	free(buffer);
	return 0;
}