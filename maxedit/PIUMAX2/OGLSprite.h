#pragma once
#include "GraphTexture.h"

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
	int m_dwFrame;
	CGraphTexture* GetCurrentTex();
	CGraphTexture** texture;
	void DeleteImage();
};
