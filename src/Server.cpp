#include "Server.h"

int Server::serverExists = 0;

Server::Server()
{
	if (!serverExists)
	{
		serverExists = 1;
		// Create a listening socket
		ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		// Get the local host information
		localHost = gethostbyname("");
		localIP = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);

		// Set up the sockaddr structure
		saServer.sin_family = AF_INET;
		saServer.sin_addr.s_addr = inet_addr(localIP);
		saServer.sin_port = htons(5150);
		saServerLen = sizeof(saServer);

		// Bind the listening socket using the
		// information in the sockaddr structure
		bind(ListenSocket, (SOCKADDR *) &saServer, saServerLen);

		assert(listen(ListenSocket, SOMAXCONN) == 0);

		iSock = accept(ListenSocket, (SOCKADDR *) &saServer, &saServerLen);
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