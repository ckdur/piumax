#pragma once
#include "stdafx.h"

class CSoundStream
{
public:
	CSoundStream(void);
	~CSoundStream(void);
#ifdef PIUMAX_FMOD
	FMOD_SOUND* m_stream;
#endif
	int GetDefaults(float* frecuency, float* volume, float* pan, int* priority);
	int SetDefaults(float frecuency, float volume, float pan, int priority);
	int GetLength(unsigned int* length, DWORD dwFlags);
	int GetLoopCount(int* loopcount);
	int GetLoopPoints(unsigned int* loopstart, unsigned int* loopend, DWORD dwFlags);
	int SetLoopCount(int loopcount);
	int SetLoopPoints(unsigned int loopstart, unsigned int loopend, DWORD dwFlags);
	int Release(void);

	bool operator ==(CSoundStream a);
	bool operator !=(CSoundStream a);
};
