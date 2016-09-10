#pragma once
#include "stdafx.h"
#include "SoundStream.h"

class CSoundChannel
{
public:
	CSoundChannel(void);
	~CSoundChannel(void);
#ifdef PIUMAX_FMOD
	FMOD_CHANNEL* m_channel;
#endif
	int GetCurrentSound(CSoundStream* stream);
	int SetFrequency(float frec);
	int GetFrequency(float* frec);
	int GetIndex(int* index);
	int GetLoopCount(int* loopcount);
	int GetLoopPoints(unsigned int* loopstart, unsigned int* loopend, DWORD dwFlags);
	int SetLoopCount(int loopcount);
	int SetLoopPoints(unsigned int loopstart, unsigned int loopend, DWORD dwFlags);
	int GetMute(bool* mute);
	int SetMute(bool mute);
	int GetPan(float* pan);
	int SetPan(float pan);
	int GetPaused(bool* paused);
	int SetPaused(bool paused);
	int GetPosition(unsigned int* position, DWORD dwFlags);
	int SetPosition(unsigned int position, DWORD dwFlags);
	int GetPriority(int* priority);
	int SetPriority(int priority);
	int GetVolume(float* volume);
	int SetVolume(float volume);
	int IsPlaying(bool* isplaying);
	int Stop(void);
};
