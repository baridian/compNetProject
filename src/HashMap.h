#ifndef GARBAGE_HASHMAP
#define GARBAGE_HASHMAP

#include <cstdlib>

typedef struct hashNode
{
	void *key;
	void *data;
	unsigned isOccupied : 1;
} hashNode;

class HashMap
{
public:
	HashMap(size_t keySize, size_t valueSize, bool stringKey = false);

	~HashMap();

	void *read(void *key);

	void write(void *key, void *value);

	void erase(void *key);

	bool contains(void *key);

private:
	inline unsigned hashString(void *);

	inline unsigned hash(void *);

	void rescaleHash();

	void *lastKey;
	int lastIndex;
	bool stringKey;
	bool rescaled;
	size_t keySize;
	size_t valueSize;
	int allocated;
	int indexesUsed;
	hashNode *table;
};

#endif