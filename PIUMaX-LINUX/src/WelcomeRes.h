#pragma once
#include "cmnpiu.h"
#define SIZE_WELRES 18

class CWelcomeRes
{
public:
	CWelcomeRes(void);
	~CWelcomeRes(void);
	COGLSprite* skin[SIZE_WELRES];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};
