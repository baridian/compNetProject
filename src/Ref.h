#ifndef GARBAGE_REF
#define GARBAGE_REF

#include <stdlib.h>
#include <cstring>
#include "HashMap.h"

template <typename T>
class Ref
{
public:

	Ref(T *ref)
	{
		ptr = ref;
		int oldNum = 0;
		if (allocated.contains(&ptr))
			oldNum = *(int *)allocated.read(&ptr);
		allocated.write(&ptr,&++oldNum);
	}

	void operator=(T *ref)
	{
		int oldNum = *(int *)allocated.read(&ptr);
		if(--oldNum == 0)
		{
			allocated.erase(&ptr);
			delete(ptr);
		}
		else
			allocated.write(&ptr,&oldNum);
		oldNum = 0;
		ptr = ref;
		if (allocated.contains(&ptr))
			oldNum = *(int *)allocated.read(&ptr);
		allocated.write(&ptr,&++oldNum);
	}

	void operator=(Ref<T> ref)
	{
		int oldNum;
		*this=ref.ptr;
		oldNum = *(int *)allocated.read(&ptr);
		allocated.write(&ptr,&++oldNum);
	}

	T *operator->()
	{
		return ptr;
	}

	T *operator!()
	{
		return ptr;
	}

	~Ref()
	{
		int oldNum = *(int *)allocated.read(&ptr);
		if(--oldNum == 0)
		{
			allocated.erase(&ptr);
			if(ptr != NULL)
				delete(ptr);
			return;
		}
		allocated.write(&ptr,&oldNum);
	}

protected:
	T *ptr;
private:
	static HashMap allocated;
};

template <typename T>
HashMap Ref<T>::allocated(sizeof(void *),sizeof(int));

#endif