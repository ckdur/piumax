#pragma once
#include "cmnpiu.h"
#define SIZE_GA3RES 182

class CGame3Res
{
public:
	CGame3Res(void);
	~CGame3Res(void);
	COGLSprite* skin[SIZE_GA3RES];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};
