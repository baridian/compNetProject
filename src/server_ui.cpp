#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrap_string.h"


int main()
{
	char *buffer = (char *)malloc(sizeof(char) * 255);
	char *commandBuffer = (char *)malloc(sizeof(char) * 255);

	FILE *outgoing;
	FILE *incoming;
	FILE *source;

	printf("type exit to leave and send for file transfer\n");

	do
	{
		scanf("%[^\n]s", buffer);
		getchar();

		if(strcmp(buffer, "exit") != 0)
		{
			if(strcmp(buffer, "send") == 0)
			{
				printf("enter path: ");
				scanf("%s",buffer);
				getchar();
				source = fopen(buffer,"r");
				if(source == NULL)
					printf("warning: invalid file\n");
				else
				{
					outgoing = fopen("..\\server_out.txt","a");
					fprintf(outgoing,"file\n%s\n", strstr(buffer, ".") + 1);
					fclose(outgoing);
					sprintf(commandBuffer,"type %s >> ..\\server_out.txt", buffer);
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

			incoming = fopen("..\\server_in.txt", "r");
			while (fscanf(incoming, "%[^\n]s", buffer) == 1)
			{
				fgetc(incoming);
				unwrap(buffer);
				printf(">: %s\n", buffer);
			}
			fclose(incoming);

			incoming = fopen("..\\server_in.txt","w");
			fclose(incoming);
		}

	} while(strcmp(buffer, "exit") != 0);

	return 0;
}