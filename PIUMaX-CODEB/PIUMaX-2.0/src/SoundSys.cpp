#include "SoundSys.h"
#include "basutil.h"
#include "cmnpiu.h"

CSoundSys::CSoundSys(void)
{
	sys = NULL;
}

CSoundSys::~CSoundSys(void)
{
	Destroy();
}

int CSoundSys::Initialize(void)
{
#ifdef PIUMAX_FMOD
	_printf(_T("Inicializando FMOD para PIU MaX\n"));
	FMOD_RESULT result = FMOD_System_Create(&sys);
	if(!ERRCHECK(result)) return 0;

	unsigned int      version;

	result = FMOD_System_GetVersion(sys, &version);
	if(!ERRCHECK(result)) return 0;

	if (version < FMOD_VERSION)
	{
		_printf(_T("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n"), version, FMOD_VERSION);
		return 0;
	}
#define WAITFORFMODSOUND 10000
	DWORD dwCheck = SDL_GetTicks();
	while(!ERRCHECK(FMOD_System_Init(sys, MAX_CHANNELS, FMOD_INIT_NORMAL, 0))) 
	{
		SDL_Delay(1000);
		if((SDL_GetTicks() - dwCheck) >= WAITFORFMODSOUND) 
		{
			_printf(_T("Tiempo de espera de FMOD alcanzado, PIUMaX termina"));
			return 0;
		}
	}
#endif // PIUMAX_FMOD
	return 1;
}

void CSoundSys::Destroy(void)
{
#ifdef PIUMAX_FMOD
	FMOD_System_Close(sys);
    FMOD_System_Release(sys);
#endif // PIUMAX_FMOD
}

int CSoundSys::CreateStream(LPCTSTR strName, DWORD dwFlags, CSoundStream* stream)
{
#ifdef PIUMAX_FMOD
	char chAnsiFile[MAX_PATH];
	Max_strcpy(chAnsiFile, strName);
	FMOD_MODE mode = FMOD_HARDWARE | FMOD_2D;
	if(dwFlags & PMS_CREATESTREAM_LOOP) mode |= FMOD_LOOP_NORMAL;
	FMOD_RESULT resc = FMOD_System_CreateStream(sys, chAnsiFile, mode, 0, &stream->m_stream);
	
	if(!ERRCHECK(resc)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundSys::GetChannel(int nId, CSoundChannel* channel)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_System_GetChannel(sys, nId, &channel->m_channel);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundSys::GetChannelsPlaying(int* channels)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_System_GetChannelsPlaying(sys, channels);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundSys::PlaySound(int channelid, CSoundStream* sound, bool paused, CSoundChannel* channel)
{
#ifdef PIUMAX_FMOD
	if(sound == NULL) return 0;
	FMOD_RESULT result = FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)channelid, sound->m_stream, paused, &channel->m_channel);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}

int CSoundSys::Update(void)
{
#ifdef PIUMAX_FMOD
	FMOD_RESULT result = FMOD_System_Update(sys);
	if(!ERRCHECK(result)) return 0;
#endif // PIUMAX_FMOD
	return 1;
}
