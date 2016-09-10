#pragma once
#include "cmnpiu.h"
#define SIZE_SPRITE 35

class CSkinCommon
{
public:
	CSkinCommon(void);
	~CSkinCommon(void);
	COGLSprite* skin[SIZE_SPRITE];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};
