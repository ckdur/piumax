#include "SoundStream.h"
#include "basutil.h"

CSoundStream::CSoundStream(void)
{
#ifdef PIUMAX_FMOD
	m_stream = NULL;
#endif // PIUMAX_FMOD
}

CSoundStream::~CSoundStream(void)
{
	Release();
}

int CSoundStream::GetDefaults(float* frecuency, float* volume, float* pan, int* priority)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Sound_GetDefaults(m_stream, frecuency, volume, pan, priority);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundStream::GetLength(unsigned int* length, DWORD dwFlags)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Sound_GetLength(m_stream, length, FMOD_TIMEUNIT_MS);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundStream::GetLoopCount(int* loopcount)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Sound_GetLoopCount(m_stream, loopcount);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundStream::GetLoopPoints(unsigned int* loopstart, unsigned int* loopend, DWORD dwFlags)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Sound_GetLoopPoints(m_stream, loopstart, FMOD_TIMEUNIT_MS, loopend, FMOD_TIMEUNIT_MS);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundStream::Release(void)
{
#ifdef PIUMAX_FMOD
	if(m_stream != NULL)
	{
		FMOD_RESULT result = FMOD_Sound_Release(m_stream);
		if(!ERRCHECK(result)) return 0;
		m_stream = NULL;
	}
	
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundStream::SetDefaults(float frecuency, float volume, float pan, int priority)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Sound_SetDefaults(m_stream, frecuency, volume, pan, priority);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundStream::SetLoopCount(int loopcount)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Sound_SetLoopCount(m_stream, loopcount);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundStream::SetLoopPoints(unsigned int loopstart, unsigned int loopend, DWORD dwFlags)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Sound_SetLoopPoints(m_stream, loopstart, FMOD_TIMEUNIT_MS, loopend, FMOD_TIMEUNIT_MS);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

bool CSoundStream::operator ==(CSoundStream a)
{
#ifdef PIUMAX_FMOD
	return a.m_stream==m_stream;
#endif // PIUMAX_FMOD
}

bool CSoundStream::operator !=(CSoundStream a)
{
#ifdef PIUMAX_FMOD
	return a.m_stream!=m_stream;
#endif // PIUMAX_FMOD
}