#include <cstdio>
#include "Server.h"
#include <iostream>

int Server::serverExists = 0;

Server Server::serverSingleton = Server();

Server::Server()
{
	if (!serverExists)
	{
		printf("server does not exist\n");
		serverExists = 1;
		// Create a listening socket
		ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		printf("initialized socket\n");
		// Get the local host information
		localHost = gethostbyname("localhost");
		localIP = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);
		printf("got local host info\n");

		// Set up the sockaddr structure
		saServer.sin_family = AF_INET;
		saServer.sin_addr.s_addr = inet_addr(localIP);
		saServer.sin_port = htons(5150);
		saServerLen = sizeof(saServer);
		printf("set up sockaddr structure\n");

		// Bind the listening socket using the
		// information in the sockaddr structure
		bind(ListenSocket, (SOCKADDR *) &saServer, saServerLen);
		printf("bound\n");

		assert(listen(ListenSocket, SOMAXCONN) == 0);
		printf("assert success\n");

		iSock = accept(ListenSocket, (SOCKADDR *) &saServer, &saServerLen);
		printf("accept set up\n");
	}
}

sockaddr *Server::getSaServer()
{
	return (SOCKADDR *) &saServer;
}

Server *Server::getServer()
{
	return &serverSingleton;
}

SOCKET Server::getISock()
{
	return this->iSock;
}

SOCKET Server::getListenSocket()
{
	return this->ListenSocket;
}