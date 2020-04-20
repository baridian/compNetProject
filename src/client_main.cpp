#include <iostream>
#include <winsock2.h>
#include <assert.h>

//how to build: add make to PATH
//make -f client_main.mak
//must be in src directory!!

int main()
{
	WSADATA wsaData = {0};
	struct sockaddr_in saServer;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Set up the sockaddr structure
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *) *gethostbyname("localhost")->h_addr_list));
	saServer.sin_port = htons(5150);

	assert(connect(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), (SOCKADDR *) &saServer, sizeof(saServer)) == 0);
	printf("connected\n");
	return 0;
}