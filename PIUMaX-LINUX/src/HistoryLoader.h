#pragma once
#include "Anu.h"

class CHistoryLoader
{
public:
	CHistoryLoader(void);
	~CHistoryLoader(void);
	int m_nSizeAnu;			// Size Unlock
	CAnu** m_anu;
	int m_nSizeStops;
	int* m_nWStops;
	int* m_nStops;
	int m_nSizeWaitCenter;
	int* m_nWaitCenter;
	int m_nSizeChangeTPL;
	int* m_nWChangeTPL;
	int* m_nChangeTPL;
	int m_nSizeShowAnu;
	int* m_nWShowAnu;
	int* m_nShowAnu;
	int m_nSizeSounds;
	int m_nSizePlaySound;
	int* m_nWPlaySound;
	int* m_nPlaySound;
	int m_nSizePlayLoopSound;
	int* m_nWPlayLoopSound;
	int* m_nPlayLoopSound;
	FMOD_SOUND** m_soundSounds;
	int m_nSizeLoopSounds;
	FMOD_SOUND** m_soundLoopSounds;
	__int64 m_nNext;
	CString m_strText;
	int LoadText(LPCTSTR  chName);
	void Destroy(void);
	int Draw(void);
	void Update(DWORD dwMilliseconds);
	int m_nPressCenter;
	int m_nTPL;
	void SetZeroCount(void);
	int TimeToTPL(__int64 m_nMillisec);
	void PressCenter(void);
	__int64 TPLToTime(int nTPL);
	int m_nPlaying;
	int m_nPlayingLoop;
	GLuint m_uTexText;
};
