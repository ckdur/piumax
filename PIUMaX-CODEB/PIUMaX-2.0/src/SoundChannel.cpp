#include "SoundChannel.h"
#include "basutil.h"

CSoundChannel::CSoundChannel(void)
{
#ifdef PIUMAX_FMOD
	m_channel = NULL;
#endif // PIUMAX_FMOD
}

CSoundChannel::~CSoundChannel(void)
{
}

// THIS FUNCTIONS DOESN'T WORK YET
int CSoundChannel::GetCurrentSound(CSoundStream* stream)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_GetCurrentSound(m_channel, &stream->m_stream);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::GetIndex(int* index)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_GetIndex(m_channel, index);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::GetLoopCount(int* loopcount)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_GetLoopCount(m_channel, loopcount);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::GetLoopPoints(unsigned int* loopstart, unsigned int* loopend, DWORD dwFlags)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_GetLoopPoints(m_channel, loopstart, FMOD_TIMEUNIT_MS, loopend, FMOD_TIMEUNIT_MS);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::SetLoopCount(int loopcount)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_SetLoopCount(m_channel, loopcount);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::SetLoopPoints(unsigned int loopstart, unsigned int loopend, DWORD dwFlags)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_SetLoopPoints(m_channel, loopstart, FMOD_TIMEUNIT_MS, loopend, FMOD_TIMEUNIT_MS);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::GetFrequency(float* frec)
{
#ifdef PIUMAX_FMOD
	
	FMOD_RESULT result = FMOD_Channel_GetFrequency(m_channel, frec);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD

	return 1;
}


int CSoundChannel::SetFrequency(float frec)
{
#ifdef PIUMAX_FMOD
	
	FMOD_RESULT result = FMOD_Channel_SetFrequency(m_channel, frec);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD

	return 1;
}

int CSoundChannel::GetMute(bool* mute)
{
#ifdef PIUMAX_FMOD
	FMOD_BOOL bMute;
	FMOD_RESULT result = FMOD_Channel_GetMute(m_channel, &bMute);
	if(!ERRCHECK(result)) return 0;
	(*mute)=bMute?true:false;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::SetMute(bool mute)
{
#ifdef PIUMAX_FMOD
	FMOD_BOOL bMute = mute?1:0;
	FMOD_RESULT result = FMOD_Channel_SetMute(m_channel, bMute);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::GetPan(float* pan)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_GetPan(m_channel, pan);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::SetPan(float pan)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_SetPan(m_channel, pan);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::GetPaused(bool* paused)
{
#ifdef PIUMAX_FMOD
	FMOD_BOOL bPaused;
	FMOD_RESULT result = FMOD_Channel_GetPaused(m_channel, &bPaused);
	if(!ERRCHECK(result)) return 0;
	(*paused)=bPaused?true:false;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::SetPaused(bool paused)
{
#ifdef PIUMAX_FMOD
	FMOD_BOOL bPaused = paused?1:0;
	FMOD_RESULT result = FMOD_Channel_SetPaused(m_channel, bPaused);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::GetPosition(unsigned int* position, DWORD dwFlags)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_GetPosition(m_channel, position, FMOD_TIMEUNIT_MS);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::SetPosition(unsigned int position, DWORD dwFlags)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_SetPosition(m_channel, position, FMOD_TIMEUNIT_MS);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::GetPriority(int* priority)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_GetPriority(m_channel, priority);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::SetPriority(int priority)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_SetPriority(m_channel, priority);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::GetVolume(float* volume)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_GetVolume(m_channel, volume);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::SetVolume(float volume)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_SetVolume(m_channel, volume);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::IsPlaying(bool* isplaying)
{
#ifdef PIUMAX_FMOD
	FMOD_BOOL bPlaying;
	FMOD_RESULT result = FMOD_Channel_IsPlaying(m_channel, &bPlaying);
	if(!ERRCHECK(result)) return 0;
	(*isplaying)=bPlaying?true:false;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundChannel::Stop(void)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_Channel_Stop(m_channel);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}
