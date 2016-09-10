#include "stdafx.h"
#include "GLFont2.h"
#include "cmnpiu.h"

CGLFont2::CGLFont2(void)
{
	m_nSizeTex = 0;
	m_nSizeFace = 0;
	texture = (GLuint*)calloc(m_nSizeTex, sizeof(GLuint));
	m_nSizeTexx = (int*)calloc(m_nSizeTex, sizeof(int));
	m_nSizeTexy = (int*)calloc(m_nSizeTex, sizeof(int));
	m_nPage = (int*)calloc(m_nSizeFace, sizeof(int));
	m_dCoordx = (double*)calloc(m_nSizeFace, sizeof(double));
	m_dCoordy = (double*)calloc(m_nSizeFace, sizeof(double));
	m_dSizex = (double*)calloc(m_nSizeFace, sizeof(double));
	m_dSizey = (double*)calloc(m_nSizeFace, sizeof(double));
	m_dOffsetx = (double*)calloc(m_nSizeFace, sizeof(double));
	m_dOffsety = (double*)calloc(m_nSizeFace, sizeof(double));
	m_dAdvx = (double*)calloc(m_nSizeFace, sizeof(double));
	memset(m_nCharSet, 0, sizeof(int)*65535);
}

CGLFont2::~CGLFont2(void)
{
	if(m_nSizeTex!=0) glDeleteTextures(m_nSizeTex, texture);
	m_nSizeTex = 0;
	m_nSizeFace = 0;
	delete texture;
	delete m_nPage;
	delete m_nSizeTexx;
	delete m_nSizeTexy;
	delete m_dCoordx;
	delete m_dCoordy;
	delete m_dSizex;
	delete m_dSizey;
	delete m_dOffsetx;
	delete m_dOffsety;
	delete m_dAdvx;
}

int CGLFont2::GetPower(int nSize)	// Devuelve el numero elevado al 2 mas cercano al numero especificado
{
	int nTest = 0, power = 0;
	while(nTest < nSize)
	{
		power++;
		nTest = (int)pow((double)2, power);
	}
	return nTest;
}

int CGLFont2::LoadFont(TCHAR* chFile)
{
	if(m_nSizeTex != 0) glDeleteTextures(m_nSizeTex, texture);
	m_nSizeTex = 0;
	m_nSizeFace = 0;
	int nSizeFont = 0;
	int nId = 0;
	int nChn = 0; // Unused
	int dx = 0;
	int dy = 0;
	int dWidth = 0;
	int dHeight = 0;
	int dxo = 0;
	int dyo = 0;
	int dxa = 0;
	int nPage = 0;
	FILE* hFile = _fopen(chFile, _T("r"));
	if(hFile == NULL) return 0;
	TCHAR ch[260];
	CString str;
	while(_fgets(ch, 260, hFile) != NULL)
	{
		str = ( ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		if(str.Find(_T('*')) != -1 || !str.IsEmpty())
		{
			if(str.Find(_T("size=")) != -1)
			{
				if(_sscanf(str, _T("size=%d"), &nSizeFont) != 1)
				{
					fclose(hFile);
					return 0;
				}
			}
			if(str.Find(_T("pages=")) != -1)
			{
				if(_sscanf(str, _T("pages=%d"), &m_nSizeTex) != 1)
				{
					fclose(hFile);
					return 0;
				}
				texture = (GLuint*)realloc(texture, m_nSizeTex*sizeof(GLuint));
				m_nSizeTexx = (int*)realloc(m_nSizeTexx, m_nSizeTex*sizeof(int));
				m_nSizeTexy = (int*)realloc(m_nSizeTexy, m_nSizeTex*sizeof(int));
				glGenTextures(m_nSizeTex, &texture[0]);
				for(int x = 0; x < m_nSizeTex; x++)
				{
					if(_fgets(ch, 260, hFile) == NULL)
					{
						fclose(hFile);
						return 0;
					}
					str = ( ch);
					str.Remove(_T('\n')); str.Remove(_T('\r'));

					texture[x] = GenTexImage(/*path +*/ str);

					m_nSizeTexx[x] = imgWidth;
					m_nSizeTexy[x] = imgHeight;
				}
			}
			if(str.Find(_T("chars count=")) != -1)
			{
				if(_sscanf(str, _T("chars count=%d"), &m_nSizeFace) != 1)
				{
					fclose(hFile);
					return 0;
				}
				m_nPage = (int*)realloc(m_nPage, m_nSizeFace*sizeof(int));
				m_dCoordx = (double*)realloc(m_dCoordx, m_nSizeFace*sizeof(double));
				m_dCoordy = (double*)realloc(m_dCoordy, m_nSizeFace*sizeof(double));
				m_dSizex = (double*)realloc(m_dSizex, m_nSizeFace*sizeof(double));
				m_dSizey = (double*)realloc(m_dSizey, m_nSizeFace*sizeof(double));
				m_dOffsetx = (double*)realloc(m_dOffsetx, m_nSizeFace*sizeof(double));
				m_dOffsety = (double*)realloc(m_dOffsety, m_nSizeFace*sizeof(double));
				m_dAdvx = (double*)realloc(m_dAdvx, m_nSizeFace*sizeof(double));
				for(int x = 0; x < 65535; x++)
					m_nCharSet[x] = 0;
				for(int x = 0; x < m_nSizeFace; x++)
				{
					if(_fgets(ch, 260, hFile) == NULL)
					{
						fclose(hFile);
						return 0;
					}
					str = ( ch);
					str.Remove(_T('\n')); str.Remove(_T('\r'));
					if(_sscanf(str, _T("char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d chnl=%d"),
						&nId,
						&dx,
						&dy,
						&dWidth,
						&dHeight,
						&dxo,
						&dyo,
						&dxa,
						&nPage,
						&nChn) != 10)
					{
						fclose(hFile);
						return 0;
					}
					m_nCharSet[nId] = x;
					m_nPage[x] = nPage;
					m_dCoordx[x] = double(dx)/double(m_nSizeTexx[nPage]);
					m_dCoordy[x] = double(dy)/double(m_nSizeTexy[nPage]);
					m_dSizex[x] = double(dWidth)/double(m_nSizeTexx[nPage]);
					m_dSizey[x] = double(dHeight)/double(m_nSizeTexy[nPage]);
					m_dOffsetx[x] = double(dxo)/double(m_nSizeTexx[nPage]);
					m_dOffsety[x] = double(dyo)/double(m_nSizeTexy[nPage]);
					m_dAdvx[x] = double(dxa)/double(m_nSizeTexx[nPage]);
				}
			}
		}
	}
	m_dSizeFont = double(nSizeFont);
	m_dSizeFont = m_dSizeFont/double(m_nSizeTexx[0]);
	fclose(hFile);
	return 1;
}

int CGLFont2::GetIndex(int nChar)
{
    if(nChar >= 65535) return 0;
	if(m_nCharSet[nChar] < 0 || m_nCharSet[nChar] >= m_nSizeFace) return 0;
	return m_nCharSet[nChar];
}

void CGLFont2::glTextOut(double x, double y, double cx, double cy, CString strMsg)
{
}

void CGLFont2::glDrawText(double t, double l, double b, double r, double x, double y, double dz, double cx, double cy, CString strMsg, DWORD dwFlags)
{
	double dWidth = abs(l-r);
	double dHeight = abs(t-b);
	double dt = max(t, b);
	double db = min(t, b);
	double dr = max(l, r);
	double dl = min(l, r);
	int nCntLin = 1;
	int nCode = strMsg.Find(_T('\n'), 0);
	while(nCode != -1)
	{
		nCntLin++;
		nCode = strMsg.Find(_T('\n'), nCode+1);
	}
	double dStartY = 0.0;
	if(dwFlags & DT_VCENTER) dStartY = y+cy/2*(double)(nCntLin);
	else dStartY = y;
	int nBackCode = 0;
	nCode = -1;
	for(int z = 0; z < nCntLin; z++)
	{
		nCode = strMsg.Find(_T('\n'), nBackCode);
		if(nCode == -1) nCode = strMsg.GetLength();
		CString strDisp = strMsg.Mid(nBackCode, nCode-nBackCode);

		double dStartX = 0.0;
		if(dwFlags & DT_CENTER)
		{
			double dSizeStrDisp = 0.0;
			for(int a = 0; a < strDisp.GetLength(); a++)
			{
				dSizeStrDisp += m_dAdvx[GetIndex(strDisp.GetAt(a))]/m_dSizeFont*cy*cx;
			}
			dStartX = x-dSizeStrDisp/2;
		}
		else dStartX = x;
		for(int k = 0; k < strDisp.GetLength(); k++)
		{
			double dy1 = dStartY-cy*double(z)-m_dOffsety[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy;
			double dy2 = dy1-m_dSizey[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy;
			double dyt1 = 0.0;
			double dyt2 = 1.0;
			if(dy1 > db && dy2 < db)
			{
				dyt2 = 1-(db-dy2)/cy;
				dy2 = db;
			}
			if(dy1 > dt && dy2 < dt)
			{
				dyt1 = (dy1-dt)/cy;
				dy1 = dt;
			}
			if(dy2 >= db && dy1 <= dt)
			{
				double dx1 = dStartX+m_dOffsetx[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy*cx;
				double dx2 = dx1+m_dSizex[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy*cx;
				double dxt1 = 0.0;
				double dxt2 = 1.0;
				dStartX += m_dAdvx[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy*cx;
				if(dx1 < dl && dl < dx2)
				{
					dxt1 = (dl-dx1)/(cx*cy);
					dx1 = dl;
				}
				if(dx1 < dr && dr < dx2)
				{
					dxt2 = 1-(dx2-dr)/(cx*cy);
					dx2 = dr;
				}
				if(dx1 >= dl && dx2 <= dr)
				{
					TCHAR ch = strDisp.GetAt(k);
					double drx1 = m_dCoordx[GetIndex(strDisp.GetAt(k))];
					double dry1 = 1.0-m_dCoordy[GetIndex(strDisp.GetAt(k))];
					double dSizeX = m_dSizex[GetIndex(strDisp.GetAt(k))];
					double dSizeY = m_dSizey[GetIndex(strDisp.GetAt(k))];
					glBindTexture(GL_TEXTURE_2D, texture[m_nPage[GetIndex(strDisp.GetAt(k))]]);
					glBegin(GL_QUADS);
					glTexCoord2d(drx1+dSizeX*dxt1, dry1-dSizeY*dyt1); glVertex3d(dx1, dy1, dz);
					glTexCoord2d(drx1+dSizeX*dxt2, dry1-dSizeY*dyt1); glVertex3d(dx2, dy1, dz);
					glTexCoord2d(drx1+dSizeX*dxt2, dry1-dSizeY*dyt2); glVertex3d(dx2, dy2, dz);
					glTexCoord2d(drx1+dSizeX*dxt1, dry1-dSizeY*dyt2); glVertex3d(dx1, dy2, dz);
					glEnd();
				}
			}
		}
		///////////////////
		nBackCode = nCode+1;
	}
}

void CGLFont2::Delete()
{
	if(m_nSizeTex!=0) glDeleteTextures(m_nSizeTex, texture);
	m_nSizeTex = 0;
	m_nSizeFace = 0;
	texture = (GLuint*)realloc(texture, m_nSizeTex*sizeof(GLuint));
	m_nPage = (int*)realloc(m_nPage, m_nSizeFace*sizeof(int));
	m_dCoordx = (double*)realloc(m_dCoordx, m_nSizeFace*sizeof(double));
	m_dCoordy = (double*)realloc(m_dCoordy, m_nSizeFace*sizeof(double));
	m_dSizex = (double*)realloc(m_dSizex, m_nSizeFace*sizeof(double));
	m_dSizey = (double*)realloc(m_dSizey, m_nSizeFace*sizeof(double));
	m_dOffsetx = (double*)realloc(m_dOffsetx, m_nSizeFace*sizeof(double));
	m_dOffsety = (double*)realloc(m_dOffsety, m_nSizeFace*sizeof(double));
	m_dAdvx = (double*)realloc(m_dAdvx, m_nSizeFace*sizeof(double));
	/*free(texture);
	free(m_nPage);
	free(m_dCoordx);
	free(m_dCoordy);
	free(m_dSizex);
	free(m_dSizey);
	free(m_dOffsetx);
	free(m_dOffsety);
	free(m_dAdvx);*/
}

double CGLFont2::CalcSingleLine(CString strMsg,  double cy, int nStart /*=0*/, double cx /*=1.0*/)
{
	int nEnd = strMsg.Find(_T('\n'), nStart);
	if(nEnd == -1) nEnd = strMsg.GetLength();
	double dSizeStrDisp = 0.0;
	for(int a = nStart; a < nEnd; a++)
	{
		dSizeStrDisp += m_dAdvx[GetIndex(strMsg.GetAt(a))]/m_dSizeFont*cy*cx;
	}
	return dSizeStrDisp;
}
