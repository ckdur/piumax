#pragma once

class CMWayLoader
{
public:
	CMWayLoader(void);
	~CMWayLoader(void);
	int m_nSizeWay;
	unsigned int* m_uLink;
	BYTE* m_bDraw;
	unsigned int* m_uSizeMUnlock;
	unsigned int** m_uMUnlock;
	unsigned int* m_uSizeMSucess;
	unsigned int** m_uMSucess;
	int LoadBinary(LPCTSTR strFile);
	int LoadText(LPCTSTR strFile);
	int SaveBinary(LPCTSTR strFile);
	int FoundWay(unsigned int n1, unsigned int n2);
};
