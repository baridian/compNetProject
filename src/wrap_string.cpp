#include "wrap_string.h"
#include <string.h>
#include <stdio.h>

//shifts part of a string forward
//for example "string" shifted becomes "sstring"
void shift(char *input)
{
	if(*input != '\0')
		shift(input + 1);
	input[1] = *input;
}

void wrap(char *input)
{
	int i = 0;
	for(; input[i] != '\0'; i++)
	{
		if(input[i] == 'a' || input[i] == 'e')
		{
			//printf("attempting shift\n");
			shift(input + i);
			i++;
		}
	}
}

//shifts a string back
//for example "string" becomes "tring"
void shiftBack(char *input)
{
	int i = 0;
	for(; input[i] != '\0'; i++)
		input[i] = input[i + 1];
}

void unwrap(char *input)
{
	int i = 0;
	for(; input[i] != '\0'; i++)
	{
		if(input[i] == 'a' || input[i] == 'e')
			shiftBack(input + i);
	}
}