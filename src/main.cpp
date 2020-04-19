#include <iostream>
#include "Server.h"
#include <winsock.h>
#include <assert.h>

//how to build:

//add make.exe to PATH environment variable
//right click on build.bat, then click on show in explorer
//build the project by double clicking the build batch file


int main()
{
	Server *server = Server::getServer();

	assert(connect(server->getListenSocket(), server->getSaServer(), sizeof(*server->getSaServer())) == 0);
}