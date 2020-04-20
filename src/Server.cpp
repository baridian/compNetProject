#include <cstdio>
#include "Server.h"

int Server::serverExists = 0;

Server Server::serverSingleton = Server();

Server::Server()
{
	int iResult = 0;
	WSADATA wsaData = {0};
	if (!serverExists)
	{
		//ensure function can only be called once
		serverExists = 1;

		//start up wsa
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
			fprintf(stderr, "ERROR %d: WSAStartup failed\n", iResult);
			exit(1);
		}

		// Create a listening socket
		if((ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		{
			fprintf(stderr,"ERROR %d: invalid socket created\n",WSAGetLastError());
			exit(1);
		}

		// Get the local host information
		localHost = gethostbyname("localhost");
		if(localHost == NULL)
		{
			fprintf(stderr, "ERROR %d: gethostbyname returned NULL when initializing server\n", WSAGetLastError());
			exit(1);
		}
		localIP = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);


		// Set up the sockaddr structure
		saServer.sin_family = AF_INET;
		saServer.sin_addr.s_addr = inet_addr(localIP);
		saServer.sin_port = htons(5150);
		saServerLen = sizeof(saServer);

		// Bind the listening socket using the
		// information in the sockaddr structure
		bind(ListenSocket, (SOCKADDR *) &saServer, saServerLen);

		//start listening
		if(listen(ListenSocket, SOMAXCONN) != 0)
		{
			fprintf(stderr, "ERROR: listen socket failed\n");
			exit(1);
		}

		//
		iSock = accept(ListenSocket, (SOCKADDR *) &saServer, &saServerLen);
		if(iSock == INVALID_SOCKET)
		{
			fprintf(stderr, "ERROR %d: accept failed to set up\n",(int)iSock);
		}
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