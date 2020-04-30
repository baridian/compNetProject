#include <stdio.h>
#include <stdlib.h>
#include "Server.h"
#include <winsock2.h>
#include <assert.h>




//function to send the server input file
//client calls sendFile with read
//each line is requested with next
//end of file token is last line sent
void sendFile(SOCKET socket, const char *file)
{
	FILE *input = fopen(file, "r");
	char *buffer = (char *)malloc(sizeof(char) * 255);
	long long i;
	long long oldPosition;
	long long fileLength;
	int ch;
	int messageLength;
	int lines = 0;

	while(fscanf(input,"%[^\n]s",buffer) == 1)
	{


		if(strcmp(buffer, "file") == 0) //handle file transfer request
		{
			fgetc(input); //chomp \n

			send(socket, "file",(int)strlen("file") + 1, 0); //sent to initiate file read mode

			recv(socket, buffer, 255, 0); //confirm client entered file read mode
			if(strcmp(buffer, "file") != 0)
			{
				fprintf(stderr,"ERROR: unexpected token \"%s\"\n",buffer);
				exit(1);
			}

			fscanf(input, "%[^\n]s",buffer); //send file extension to client
			fgetc(input);
			oldPosition = ftell(input);
			send(socket, buffer, 255, 0);
			recv(socket, buffer, 255, 0);


			fseek(input, 0, SEEK_END); //get file length
			i = ftell(input);
			printf("f len = %d\n", i);
			fileLength = i;
			fclose(input);
			input = fopen(file,"rb");
			fseek(input, oldPosition, SEEK_SET);
			i = oldPosition;

			fseek(input, oldPosition, SEEK_SET);
			fileLength -= lines;

			printf("start position = %d\n", i);

			while(i < fileLength) //copy file
			{
				fread(buffer, sizeof(char), 255, input);
				/*for(lines = 0; lines < 255; lines++)
				{
					ch = fgetc(input);
					buffer[lines] = (char)ch;
					if(feof(input)) //jpg files use the EOF character for data. must ignore it and use the
						//given end position
					{
						buffer[lines] = (char)EOF;
						clearerr(input);
					}
				}*/
				i += 255;
				//printf("\ni = %d\n",i);
				messageLength = i < fileLength ? 255 : (int)(fileLength - (i - 255));
				/*for(int j=0;j<messageLength; j++)
				{
					putchar(buffer[j]);
				}*/
				send(socket, buffer, messageLength, 0);
				recv(socket, buffer, 255, 0);
				if (strcmp(buffer, "file next") != 0)
				{
					fprintf(stderr, "ERROR: unexpected token \"%s\"\n", buffer);
					exit(1);
				}
			}
			/*send(socket, buffer, i, 0);
			recv(socket, buffer, 255, 0);
			if(strcmp(buffer, "file next") != 0)
			{
				fprintf(stderr,"ERROR: unexpected token \"%s\"\n",buffer);
				exit(1);
			}*/
			fclose(input);
		}
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
	printf("\nend of file reached\n");
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

	do
	{
		receivedLength = server->receive(buffer, bufferLength);
		printf("received\n");
		if(strcmp(buffer, "read") == 0)
		{
			printf("attempting read\n");
			sendFile(server->getISock(), "..\\server_out.txt");
		}
		else
		{
			send(server->getISock(),"ack",(int)strlen("ack") + 1,0);
			printf("sent ack\n");
			output = fopen("..\\server_in.txt", "a");
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