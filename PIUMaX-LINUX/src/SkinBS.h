#pragma once
#include "cmnpiu.h"
#define SIZE_SPRITEBS 20

class CSkinBS
{
public:
	CSkinBS(void);
	~CSkinBS(void);
	COGLSprite* skin[SIZE_SPRITEBS];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};
