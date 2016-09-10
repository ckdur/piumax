#pragma once
#include "cmnpiu.h"
#define SIZE_GA4RES 173

class CGame4Res
{
public:
	CGame4Res(void);
	~CGame4Res(void);
	COGLSprite* skin[SIZE_GA4RES];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};
