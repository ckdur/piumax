#pragma once
#include "cmnpiu.h"
#define SIZE_UNLOCKRES 22

class CUnlockRes
{
public:
	CUnlockRes(void);
	~CUnlockRes(void);
	COGLSprite* skin[SIZE_UNLOCKRES];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};

#define SIZE_UNLOCKBASRES 57

class CUnlockBasRes
{
public:
	CUnlockBasRes(void);
	~CUnlockBasRes(void);
	COGLSprite* skin[SIZE_UNLOCKBASRES];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};
