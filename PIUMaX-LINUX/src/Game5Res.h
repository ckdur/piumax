#pragma once
#include "cmnpiu.h"
#define SIZE_GA5RES 173

class CGame5Res
{
public:
	CGame5Res(void);
	~CGame5Res(void);
	COGLSprite* skin[SIZE_GA5RES];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};
