#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

class Buffer
{
public:
	Buffer()
	{
		size = 255;
		theBuffer = (char *)malloc(sizeof(char) * size);
		wroteTo = 0;
	}

	void write(char *string, SOCKET to)
	{
		while(wroteTo + strlen(string) > size)
		{
			size *= 2;
			theBuffer = (char *)realloc(theBuffer, size * sizeof(char));
		}
		theBuffer[wroteTo] = '\n';
		wroteTo += 1;
		strcpy(theBuffer + wroteTo, string);
		wroteTo += strlen(string);
		send(to, theBuffer, wroteTo, 0);
	}

	void sendMessage(SOCKET to)
	{
		send(to,theBuffer, wroteTo,0);
		size = 255;
		free(theBuffer);
		theBuffer = (char *)malloc(size * sizeof(char));
		wroteTo = 0;
	}

	~Buffer()
	{
		free(theBuffer);
	}

private:
	char *theBuffer;
	int size;
	int wroteTo;
};

#endif