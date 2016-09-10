#pragma once
#include "cmnpiu.h"
#define SIZE_GAME2RES 106
#define MAP_EXPANSION_X_DIV2 80.0
#define MAP_EXPANSION_Y_DIV2 60.0

class CBasicGame2Res
{
public:
	CBasicGame2Res(void);
	~CBasicGame2Res(void);
	COGLSprite* skin[SIZE_GAME2RES];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};

class CPluGame2Res
{
public:
	CPluGame2Res(void);
	~CPluGame2Res(void);
	int nSize;
	COGLSprite** skin;
	int LoadSkin(LPCTSTR chFile);
	void Update(DWORD dwMiliseconds);
	void Delete();
};

class CMapGame2Res : public CPluGame2Res
{
public:
	CMapGame2Res(void);
	~CMapGame2Res(void);
	int m_nSizeMap;
	double* dSizeX;
	double* dSizeY;
	double dPosX;
	double dPosY;
	double sx1, sy1, sx2, sy2;
	int LoadFile(LPCTSTR chFile);
	void DrawAt(double dx, double dy, int nMap);
	void DeleteMap();
};