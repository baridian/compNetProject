#include <cstdio>
#include "Server.h"

#define PORT_NUMBER 80
#define HOST_BY_NAME "10.50.120.179"

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
		localHost = gethostbyname(HOST_BY_NAME);
		if(localHost == NULL)
		{
			fprintf(stderr, "ERROR %d: gethostbyname returned NULL when initializing server\n", WSAGetLastError());
			exit(1);
		}
		localIP = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);


		// Set up the sockaddr structure
		saServer.sin_family = AF_INET;
		saServer.sin_addr.s_addr = inet_addr(localIP);
		saServer.sin_port = htons(PORT_NUMBER);
		saServerLen = sizeof(saServer);

		// Bind the listening socket using the
		// information in the sockaddr structure
		if(bind(ListenSocket, (SOCKADDR *) &saServer, saServerLen) != 0)
		{
			fprintf(stderr, "ERROR %d: bind returned NULL when initializing server\n", WSAGetLastError());
			exit(1);
		}

		//start listening
		if(listen(ListenSocket, SOMAXCONN) != 0)
		{
			fprintf(stderr, "ERROR: listen socket failed\n");
			exit(1);
		}
	}
}

int Server::receive(char *buffer, int bufferLength)
{
	int length = recv(getISock(), buffer, bufferLength,0);
	if(length < 0)
	{
		printf("recv failed: %d\n", WSAGetLastError());
		exit(1);
	}
	return length;
}

void Server::acceptConnection()
{
	iSock = accept(ListenSocket, (SOCKADDR *) &saServer, &saServerLen);
	if(iSock == SOCKET_ERROR)
	{
		fprintf(stderr, "ERROR %d: accept failed to set up\n",(int)iSock);
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