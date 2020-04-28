#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrap_string.h"


int main()
{
	char *buffer = (char *)malloc(sizeof(char) * 255);
	FILE *outgoing;
	FILE *incoming;

	printf("send exit to leave\n");

	do
	{
		scanf("%[^\n]s", buffer);
		getchar();

		if(strcmp(buffer, "exit") != 0)
		{
			wrap(buffer);
			outgoing = fopen("..\\server_out.txt", "a");
			fprintf(outgoing, "%s\n", buffer);
			fclose(outgoing);

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