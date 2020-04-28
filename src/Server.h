#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <assert.h>

class Server
{
public:
	static Server *getServer();

	sockaddr *getSaServer();

	SOCKET getISock();

	SOCKET getListenSocket();

	void acceptConnection();

	int receive(char *buffer, int bufferLength);

private:
	Server();

	static Server serverSingleton;
	static int serverExists;
	SOCKET  ListenSocket;
	struct sockaddr_in saServer;
	hostent* localHost;
	char* localIP;
	int saServerLen;
	SOCKET iSock;


};



#endif