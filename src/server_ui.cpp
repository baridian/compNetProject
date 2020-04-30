#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "wrap_string.h"


int main()
{
	char *buffer = (char *)malloc(sizeof(char) * 255);
	char *commandBuffer = (char *)malloc(sizeof(char) * 255);
	char *kbBuffer = (char *)malloc(sizeof(char) * 255);
	int kbChar;
	int bufferIndex = 0;

	FILE *outgoing;
	FILE *incoming;
	FILE *source;

	printf("type exit to leave and send for file transfer\n");

	do
	{
		//scanf("%[^\n]s", buffer);
		//getchar();

		if(_kbhit())
		{
			kbChar = _getch();
			printf("%c",(int)kbChar);
			if(kbChar == '\r')
			{
				printf("\n");
				kbBuffer[bufferIndex++] = '\0';
				bufferIndex = 0;
				strcpy(buffer,kbBuffer);
			}
			else
			{
				kbBuffer[bufferIndex++] = (char)kbChar;
			}
		}

		if(strcmp(buffer, "exit") != 0 && strlen(buffer) > 0)
		{
			if(strcmp(buffer, "send") == 0) //if file transfer requested
			{
				printf("enter path: "); //get path
				scanf("%s",buffer);
				getchar(); //chomp \n

				source = fopen(buffer,"r"); //confirm file exists
				if(source == NULL)
					printf("warning: invalid file\n");
				else //file exists
				{
					outgoing = fopen("..\\server_out.txt","a"); //print header data
					fprintf(outgoing,"file\n%s\n", strstr(buffer, ".") + 1);
					fclose(outgoing);

					sprintf(commandBuffer,"type %s >> ..\\server_out.txt", buffer); //transfer file contents
					system(commandBuffer);
				}
				fclose(source);
			}
			else
			{
				wrap(buffer);
				outgoing = fopen("..\\server_out.txt", "a");
				fprintf(outgoing, "%s\n", buffer);
				fclose(outgoing);
			}

		}
		else if(strcmp(buffer, "exit") == 0)
			break;

		Sleep(100);
		incoming = fopen("..\\server_in.txt", "r"); //display incoming messages
		while (fscanf(incoming, "%[^\n]s", buffer) == 1)
		{
			fgetc(incoming);
			unwrap(buffer);
			printf(">: %s\n", buffer);
		}
		fclose(incoming);

		incoming = fopen("..\\server_in.txt","w"); //clear daemon incoming file
		fclose(incoming);

		buffer[0] = '\0';

	} while(strcmp(buffer, "exit") != 0);

	return 0;
}