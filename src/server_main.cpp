#include <iostream>
#include "Server.h"
#include <winsock2.h>
#include <assert.h>

int main()
{
	int receivedLength;
	int bufferLength = 255;
	char *buffer = (char *)malloc(sizeof(char) * bufferLength);
	Server *server = Server::getServer();
	server->acceptConnection();
	printf("accepted.\n");

	do
	{
		receivedLength = recv(server->getISock(), buffer, bufferLength,0);
		if(receivedLength > 0)
		{
			buffer[receivedLength] = '\0';
			printf("%s",buffer);
		}
		else if(receivedLength < 0)
			printf("recv failed: %d\n", WSAGetLastError());
	}
	while(receivedLength > 0);

	return 0;
}