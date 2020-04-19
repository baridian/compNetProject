#include <iostream>
#include <winsock2.h>

int inet_aton (char* addr, struct in_addr* dest)
{
	int a[4];
	if (sscanf(addr, "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]) != 4)
		return 0;
	dest->s_addr = a[3] | a[2]<<8 | a[1]<<16 | a[0]<<24;
	return 1;
}

int main()
{
	int sockfd = (int)socket(AF_INET, SOCK_STREAM, 0);
	in_addr myInAddr;
	inet_aton("192.168.1.17", &myInAddr);
	sockaddr_in myAddr = {AF_INET, htons(3490), myInAddr, {0,0}};
	bind(sockfd, myAddr, myAddrLen);

	
	return 0;
}