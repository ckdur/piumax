
#include "stdafx.h"
#include "libc_max_common.h"

void* max_realloc(void* prev, size_t nSize)
{
	void* nPtr;
	if(nSize == 0)
	{
		
		free(prev);
		nPtr = calloc(1, nSize);
		return nPtr;
	}
	else 
	{
		nPtr = realloc(prev, nSize);
		if(nPtr == NULL) return prev;
		return nPtr;
	}
}

/*FILE* _wfopen(LPCWSTR chFile, LPCWSTR chMode)
{
    char cFile [MAX_PATH];
    char cMode [MAX_PATH];
    Max_strcpy(cFile, chFile);
    Max_strcpy(cMode, chMode);
    return fopen(cFile, cMode);
}*/
