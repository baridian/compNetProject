#include <iostream>
#include "Server.h"
#include <winsock2.h>
#include <assert.h>

int main()
{
	printf("started\n");
	Server *server = Server::getServer();
	printf("got server\n");
	server->acceptConnection();
	return 0;
}