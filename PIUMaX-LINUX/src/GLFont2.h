#pragma once
#include "cmnpiu.h"

class CGLFont2
{
public:
	CGLFont2(void);
	~CGLFont2(void);
	int m_nSizeTex;
	GLuint* texture;
	int* m_nSizeTexx;
	int* m_nSizeTexy;
	int m_nSizeFace;
	int* m_nPage;
	double* m_dCoordx;
	double* m_dCoordy;
	double* m_dSizex;
	double* m_dSizey;
	double* m_dOffsetx;
	double* m_dOffsety;
	double* m_dAdvx;
	double m_dSizeFont;
	int m_nCharSet[65535];
	int LoadFont(TCHAR* chFile);
	int GetPower(int nSize);
	int GetIndex(int nChar);
	void glTextOut(double x, double y, double cx, double cy, CString strMsg);
	void glDrawText(double t, double l, double b, double r, double x, double y, double dz, double cx, double cy, CString strMsg, DWORD dwFlags = DT_TOP|DT_LEFT);
	double CalcSingleLine(CString strMsg, double cy, int nStart = 0, double cx = 1.0);
	void Delete();
};
