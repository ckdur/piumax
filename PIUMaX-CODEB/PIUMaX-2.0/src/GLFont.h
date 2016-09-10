#ifndef GL_FONT
#define GL_FONT
#include "GraphTexture.h"

class CFont
{
public:
	CFont(void);
	~CFont(void);
	CGraphTexture* texture;
	int LoadFont(TCHAR* chFile, int nDivX, int nDivY);
	int GetPower(int nSize);
	void glTextOut(double x, double y, double cx, double cy, CString strMsg);
	void glDrawText(double t, double l, double b, double r, double x, double y, double dz, double cx, double cy, CString strMsg, DWORD dwFlags = DT_TOP|DT_LEFT);
	void Delete();
	int m_nDivX;
	int m_nDivY;
};

#endif // GL_FONT2