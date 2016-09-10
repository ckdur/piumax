#pragma once
#include "cmnpiu.h"
//#include "OGLSprite.h"

class CSkin
{
public:
	CSkin(void);
	~CSkin(void);
	COGLSprite* skin[35];
	int LoadSkin(int nSkin);
	void Update(DWORD dwMiliseconds);
	void Delete();
	double dScale;
};
