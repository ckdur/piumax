#pragma once
#include "cmnpiu.h"
#define SIZE_STEPEVAL 38

class CStepEvalRes
{
public:
	CStepEvalRes(void);
	~CStepEvalRes(void);
	COGLSprite* skin[SIZE_STEPEVAL];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};