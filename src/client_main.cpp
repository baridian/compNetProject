#include <iostream>
#include <winsock2.h>
#include <assert.h>
#include <unistd.h>
#include <zconf.h>

//how to build: add make to PATH
//make -f client_main.mak
//must be in src directory!!

SOCKET setupSocket(const char *address, int port)
{
	WSADATA wsaData = {0};
	struct sockaddr_in saServer;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Set up the sockaddr structure
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *) *gethostbyname(address)->h_addr_list));
	saServer.sin_port = htons(port);

	assert(connect(mySocket, (SOCKADDR *) &saServer, sizeof(saServer)) == 0);

	return mySocket;
}

int main()
{
	char *input = (char *)malloc(sizeof(char)*255);

	SOCKET mySocket = setupSocket("10.50.120.215",80);

	scanf("%[^\n]s",input);
	send(mySocket,input,(int)strlen(input) + 1, 0);
	shutdown(mySocket, SD_SEND);
	closesocket(mySocket);
	return 0;
}