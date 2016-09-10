#pragma once
#include "cmnpiu.h"
#define SIZE_USBRES 34

class CUsbRes
{
public:
	CUsbRes(void);
	~CUsbRes(void);
	COGLSprite* skin[SIZE_USBRES];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};
