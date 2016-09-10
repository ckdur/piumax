#pragma once
#include "stdafx.h"
#include "SoundChannel.h"
#include "SoundStream.h"

class CSoundSys
{
public:
	CSoundSys(void);
	~CSoundSys(void);
	int Initialize(void);
#ifdef PIUMAX_FMOD
	FMOD_SYSTEM* sys;
#endif // PIUMAX_FMOD
	void Destroy(void);
	int CreateStream(LPCTSTR strName, DWORD dwFlags, CSoundStream* stream);
	int GetChannel(int nId, CSoundChannel* channel);
	int GetChannelsPlaying(int* channels);
	int PlaySound(int channelid, CSoundStream* sound, bool paused, CSoundChannel* channel);
	int Update(void);
};

#define PMS_CREATESTREAM_DEFAULT 0x0
#define PMS_CREATESTREAM_LOOP 0x1