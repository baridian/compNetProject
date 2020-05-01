#include "process_binary.h"

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

//sends a binary file
void sendBinaryFile(SOCKET socket, FILE *input, const char *file)
{
	char *buffer = (char *)malloc(sizeof(char) * 255);
	long long oldPosition;
	long long i;
	long long fileLength;
	int messageLength;

	fclose(input);
	input = fopen(file,"rb");
	fgetc(input); //chomp \n
	//printf("start = %d\n",(int)ftell(input));

	send(socket, "file",(int)strlen("file") + 1, 0); //sent to initiate file read mode

	recv(socket, buffer, 255, 0); //confirm client entered file read mode
	if(strcmp(buffer, "file") != 0)
	{
		fprintf(stderr,"ERROR: unexpected token \"%s\"\n",buffer);
		exit(1);
	}

	fscanf(input, "%s",buffer); //send file extension to client
	fgetc(input); //chomp \r
	fgetc(input); //chomp \n
	oldPosition = ftell(input);
	//printf("extension = %s\n",buffer);
	send(socket, buffer, 255, 0);
	recv(socket, buffer, 255, 0);


	fseek(input, 0, SEEK_END); //get file length
	i = ftell(input);
	//printf("f len = %d\n", i);
	fileLength = i;

	fclose(input); //return to old position
	input = fopen(file,"rb");
	fseek(input, (long)oldPosition, SEEK_SET);
	i = oldPosition;

	//printf("start position = %d\n", i);

	while(i < fileLength) //copy file
	{
		fread(buffer, sizeof(char), 255, input);
		i += 255;
		messageLength = i < fileLength ? 255 : (int)(fileLength - (i - 255));
		send(socket, buffer, messageLength, 0);
		recv(socket, buffer, 255, 0);
		if (strcmp(buffer, "file next") != 0)
		{
			fprintf(stderr, "ERROR: unexpected token \"%s\"\n", buffer);
			exit(1);
		}
	}

	fclose(input);
	free(buffer);
}