#ifndef PROCESS_BINARY_H
#define PROCESS_BINARY_H

#include <winsock2.h>
#include <stdio.h>

void readFile(SOCKET, char *type);

void sendBinaryFile(SOCKET, FILE *input, const char *fileName);

#endif