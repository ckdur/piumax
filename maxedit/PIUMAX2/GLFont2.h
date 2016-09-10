#ifndef GL_FONT2
#define GL_FONT2
#include "basutil.h"

class CFont2
{
public:
	CFont2(void);
	~CFont2(void);
	int m_nSizeTex;
	CGraphTexture** texture;
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
	int LoadFont(LPCTSTR chFile);
	int GetIndex(int nChar);
	void glTextOut(double x, double y, double z, double cx, double cy, CString strMsg);
	void glDrawText(
		double t, double l, double b, double r, 
		double x, double y, double dz, double cx, double cy, CString strMsg, DWORD dwFlags = DT_TOP|DT_LEFT);
	void glDrawTextTol(
		double t, double l, double b, double r, 
		double tt, double tl, double tb, double tr, 
		double x, double y, double dz, double cx, double cy, double da, CString strMsg, DWORD dwFlags = DT_TOP|DT_LEFT);
	double CalcSingleLine(CString strMsg, double cy, int nStart = 0, double cx = 1.0);
	double CalcSingleColumn(CString strMsg, double cy, int nStart = 0, double cx = 1.0);
	void Delete();
};

#endif // GL_FONT2
