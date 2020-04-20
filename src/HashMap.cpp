#include "HashMap.h"

#include <cstring>
#include <cmath>
#include <cstdio>

static const int START_SIZE = 4;

unsigned HashMap::hashString(void *key)
{
	auto keyCopy = (char *)key;
	unsigned toReturn = 0;
	int i = 0;
	for(; keyCopy[i];i++)
		toReturn += keyCopy[i] * pow(31,i);
	return toReturn % allocated;
}

unsigned HashMap::hash(void *key)
{
	return (*(unsigned *)key >> 3u) % allocated;
}

HashMap::HashMap(size_t keySize, size_t valueSize, bool stringKey)
{
	int i = 0;
	allocated = START_SIZE;
	indexesUsed = 0;
	this->keySize = keySize;
	this->valueSize = valueSize;
	this->stringKey = stringKey;
	table = (hashNode *) malloc(sizeof(hashNode) * START_SIZE);
	for (; i < START_SIZE; i++)
		table[i].isOccupied = 0;
	rescaled = false;
}

void *HashMap::read(void *key)
{
	int index;
	int start;
	if(lastKey != nullptr && !stringKey && memcmp(key,lastKey,keySize) == 0 && !rescaled)
		return table[lastIndex].data;
	if(!stringKey)
		index = hash(key);
	else
		index = hashString(key);
	start = index;
	do
	{
		if(table[index].isOccupied)
		{
			if ((!stringKey && memcmp(key, table[index].key, keySize) == 0) ||
				(stringKey && strcmp((char *)table[index].key,(char *)key) == 0))
			{
				if(lastKey == nullptr)
					lastKey = malloc(keySize);
				memcpy(lastKey,key,keySize);
				lastIndex = index;
				rescaled = false;
				return table[index].data;
			}
		}
		index = index == allocated - 1 ? 0 : index + 1;
	} while (table[index].isOccupied && index != start);
	return nullptr;
}

void HashMap::rescaleHash()
{
	hashNode *oldTable;
	int oldLength;
	int i = 0;
	int newIndex;
	if (indexesUsed > allocated * 0.7 || (indexesUsed < allocated * 0.7 / 2 && allocated > START_SIZE))
	{
		oldTable = table;
		oldLength = allocated;
		if(indexesUsed > allocated * 0.7)
			allocated *= 2;
		else
			allocated /= 2;
		table = (hashNode *) malloc(sizeof(hashNode) * allocated);
		for (; i < allocated; i++)
			table[i].isOccupied = 0;
		for (i = 0; i < oldLength; i++)
		{
			if (oldTable[i].isOccupied)
			{
				if(!stringKey)
					newIndex = hash(oldTable[i].key);
				else
					newIndex = hashString(oldTable[i].key);
				while (table[newIndex].isOccupied)
					newIndex = newIndex == allocated - 1 ? 0 : newIndex + 1;
				table[newIndex] = oldTable[i];
			}
		}
		free(oldTable);
		rescaled = true;
	}
}

void HashMap::write(void *key, void *value)
{
	int index;
	int start;
	if(lastKey != nullptr && !stringKey && memcmp(lastKey,key,keySize) == 0 && !rescaled)
	{
		if(!table[lastIndex].isOccupied)
		{
			table[lastIndex].data = malloc(valueSize);
			table[lastIndex].key = malloc(keySize);
			memcpy(table[lastIndex].key,key,keySize);
			indexesUsed++;
		}
		memcpy(table[lastIndex].data,value,valueSize);
		return;
	}
	if(stringKey)
		index = hashString(key);
	else
		index = hash(key);
	start = index;
	while (table[index].isOccupied)
	{
		if(!stringKey && memcmp(table[index].key,key,keySize) == 0)
			break;
		else if(stringKey && strcmp((char *)table[index].key,(char *)key) == 0)
			break;
		index = index == allocated - 1 ? 0 : index + 1;
		if(index == start)
		{
			fprintf(stderr,"ERROR: table is full. This should never happen\n");
			exit(1);
		}
	}
	if(!stringKey)
	{
		if(!table[index].isOccupied)
			table[index].key = malloc(keySize);
		memcpy(table[index].key, key,keySize);
	}
	else
	{
		if(table[index].isOccupied)
			free(table[index].key);
		table[index].key = malloc(strlen((char *)key) + 1);
		strcpy((char *)table[index].key,(char *)key);
	}
	if(!table[index].isOccupied)
	{
		table[index].data = malloc(valueSize);
		indexesUsed++;
	}
	rescaled = false;
	if(lastIndex == NULL)
		lastKey = malloc(keySize);
	memcpy(lastKey,key,keySize);
	lastIndex = index;
	memcpy(table[index].data, value,valueSize);
	table[index].isOccupied = 1;
	rescaleHash();
}

void HashMap::erase(void *key)
{
	int index = !stringKey ? hash(key) : hashString(key);
	int start = index;
	while (table[index].isOccupied)
	{
		if(!stringKey && memcmp(table[index].key,key,keySize) == 0)
			break;
		else if(stringKey && strcmp((char *)table[index].key,(char *)key) == 0)
			break;
		index = index == allocated - 1 ? 0 : index + 1;
		if(index == start)
		{
			fprintf(stderr,"ERROR: table is full. This should never happen\n");
			exit(1);
		}
	}
	if(!table[index].isOccupied)
		return;
	table[index].isOccupied = 0;
	free(table[index].key);
	free(table[index].data);
	indexesUsed--;
	rescaleHash();
}

bool HashMap::contains(void *key)
{
	return read(key) != nullptr;
}

HashMap::~HashMap()
{
	int i = 0;
	for (; i < allocated; i++)
	{
		if (table[i].isOccupied)
		{
			free(table[i].key);
			free(table[i].data);
		}
	}
	free(table);
}