#pragma once
#include "GraphTexture.h"

enum MAX_SPRITE_LOOP
{
	MAX_SPRITE_LOOP_NORMAL,
	MAX_SPRITE_LOOP_NO,
	MAX_SPRITE_LOOP_NOANDSTART,
	MAX_SPRITE_LOOP_STOP,
	MAX_SPRITE_LOOP_PAUSE,
	MAX_SPRITE_LOOP_SIZE
};

class CSprite
{
public:
	CSprite(void);
	~CSprite(void);
	int InitImage(LPCTSTR chName, int nSize);
	int InitImage_mn(LPCTSTR chName, int m, int n);
	int m_nSize;
	void Update(DWORD dwMiliseconds);
	DWORD m_dwNext;
	DWORD m_dwDuration;
	void SetDuration(DWORD dwDuration);
	void SetLoop(int nLoop);
	void SetTime(DWORD dwTime);
	int m_dwFrame;
	int m_nLoopOpt;
	CGraphTexture* GetCurrentTex();
	CGraphTexture** texture;
	void DeleteImage();
};
