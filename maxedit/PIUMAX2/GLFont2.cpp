#include "stdafx.h"
#include "GLFont2.h"
#include "basutil.h"
#include "cmnpiu.h"

CFont2::CFont2(void)
{
	m_nSizeTex = 0;
	m_nSizeFace = 0;
	texture = NULL;
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

CFont2::~CFont2(void)
{
	if(m_nSizeTex!=0)
	{
		m_nSizeTex = 0;
		delete[] texture;
	}
	m_nSizeFace = 0;
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

int CFont2::LoadFont(LPCTSTR chFile)
{
	if(m_nSizeTex!=0) Delete();
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
	FILE* hFile = _tfopen(chFile, _T("r+"));
	if(hFile == NULL)
	{
	    _printf(_T("Cannot open file: ") STROP _T("\n"), chFile);
	    return 0;
	 }
	TCHAR ch[260];
	CString str;
	while(_fgets(ch, 260, hFile) != NULL)
	{
		str.Format(STROP, ch);
		str.Remove(_T('\n'));
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
				texture = new CGraphTexture*[m_nSizeTex];
				m_nSizeTexx = (int*)max_realloc(m_nSizeTexx, m_nSizeTex*sizeof(int));
				m_nSizeTexy = (int*)max_realloc(m_nSizeTexy, m_nSizeTex*sizeof(int));
				for(int x = 0; x < m_nSizeTex; x++)
				{
					if(_fgets(ch, 260, hFile) == NULL)
					{
						fclose(hFile);
						return 0;
					}
					str.Format(STROP, ch);
					str.Remove(_T('\n'));

					texture[x] = GenTexImage(str);

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
				m_nPage = (int*)max_realloc(m_nPage, m_nSizeFace*sizeof(int));
				m_dCoordx = (double*)max_realloc(m_dCoordx, m_nSizeFace*sizeof(double));
				m_dCoordy = (double*)max_realloc(m_dCoordy, m_nSizeFace*sizeof(double));
				m_dSizex = (double*)max_realloc(m_dSizex, m_nSizeFace*sizeof(double));
				m_dSizey = (double*)max_realloc(m_dSizey, m_nSizeFace*sizeof(double));
				m_dOffsetx = (double*)max_realloc(m_dOffsetx, m_nSizeFace*sizeof(double));
				m_dOffsety = (double*)max_realloc(m_dOffsety, m_nSizeFace*sizeof(double));
				m_dAdvx = (double*)max_realloc(m_dAdvx, m_nSizeFace*sizeof(double));
				for(int x = 0; x < 65535; x++)
					m_nCharSet[x] = 0;
				for(int x = 0; x < m_nSizeFace; x++)
				{
					if(_fgets(ch, 260, hFile) == NULL)
					{
						fclose(hFile);
						return 0;
					}
					str.Format(STROP, ch);
					str.Remove(_T('\n'));
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

int CFont2::GetIndex(int nChar)
{
	if(m_nCharSet[nChar] < 0 || m_nCharSet[nChar] >= m_nSizeFace) return 0;
	return m_nCharSet[nChar];
}

void CFont2::glTextOut(double x, double y, double z, double cx, double cy, CString strMsg)
{
	glDrawText(30.0, -40.0, -30.0, 40.0, x, y, z, cx, cy, strMsg);
}

void CFont2::glDrawText(double t, double l, double b, double r,
						double x, double y, double dz, double cx, double cy, CString strMsg, DWORD dwFlags)
{
	// Width and height about viewport
	//double dWidth = fabs(l-r);
	//double dHeight = fabs(t-b);

	// Proceced dimmentions about viewport
	double dt = max(t, b);
	double db = min(t, b);
	double dr = max(l, r);
	double dl = min(l, r);

	// nCntLin -> Number of lines
	int nCntLin = 1;
	int nCode = strMsg.Find(_T('\n'), 0);
	while(nCode != -1)
	{
		nCntLin++;
		nCode = strMsg.Find(_T('\n'), nCode+1);
	}

	// Start Y in?
	double dStartY = 0.0;
	if(dwFlags & DT_VCENTER) dStartY = y+cy/2*(double)(nCntLin);
	if(dwFlags & DT_BOTTOM) dStartY = y+cy*(double)(nCntLin);
	else dStartY = y;

	// Proceed with lines first
	int nBackCode = 0;	// Auxiliar for extract lines without \n
	nCode = -1;
	for(int z = 0; z < nCntLin; z++)
	{
		// Extract lines without \n (strDisp)
		nCode = strMsg.Find(_T('\n'), nBackCode);
		if(nCode == -1) nCode = strMsg.GetLength();
		CString strDisp = strMsg.Mid(nBackCode, nCode-nBackCode);

		// Start X in?
		double dStartX = 0.0;
		if(dwFlags & DT_CENTER || dwFlags & DT_RIGHT)
		{
			double dSizeStrDisp = 0.0;
			for(int a = 0; a < strDisp.GetLength(); a++)
			{
				dSizeStrDisp += m_dAdvx[GetIndex(strDisp.GetAt(a))]/m_dSizeFont*cy*cx;
			}
			if(dwFlags & DT_CENTER) dStartX = x-dSizeStrDisp/2;
			if(dwFlags & DT_RIGHT) dStartX = x-dSizeStrDisp;
		}
		else dStartX = x;

		// Proceed with characters
		for(int k = 0; k < strDisp.GetLength(); k++)
		{
			// dy1 and dy2 -> Bottom and top coordinates about character
			double dy1 = dStartY-cy*double(z)-m_dOffsety[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy;
			double dy2 = dy1-m_dSizey[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy;

			// dyt1 and dyt2 -> Bottom and top texture coordinates about character (if absolute)
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
				// dy1 and dy2 -> Bottom and top coordinates about character
				double dx1 = dStartX+m_dOffsetx[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy*cx;
				double dx2 = dx1+m_dSizex[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy*cx;

				// dxt1 and dxt2 -> Bottom and top texture coordinates about character (if absolute)
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
					//TCHAR ch = strDisp.GetAt(k);
					double drx1 = m_dCoordx[GetIndex(strDisp.GetAt(k))];
					double dry1 = 1.0-m_dCoordy[GetIndex(strDisp.GetAt(k))];
					double dSizeX = m_dSizex[GetIndex(strDisp.GetAt(k))];
					double dSizeY = m_dSizey[GetIndex(strDisp.GetAt(k))];
					g_gmMain->BindTexture(texture[m_nPage[GetIndex(strDisp.GetAt(k))]]);
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->TexCoord2d(drx1+dSizeX*dxt1, dry1-dSizeY*dyt1); g_gmMain->Vertex3d(dx1, dy1, dz);
					g_gmMain->TexCoord2d(drx1+dSizeX*dxt2, dry1-dSizeY*dyt1); g_gmMain->Vertex3d(dx2, dy1, dz);
					g_gmMain->TexCoord2d(drx1+dSizeX*dxt2, dry1-dSizeY*dyt2); g_gmMain->Vertex3d(dx2, dy2, dz);
					g_gmMain->TexCoord2d(drx1+dSizeX*dxt1, dry1-dSizeY*dyt2); g_gmMain->Vertex3d(dx1, dy2, dz);
					g_gmMain->End();
				}
			}
		}
		///////////////////
		nBackCode = nCode+1;
	}
}

void CFont2::glDrawTextTol(double t, double l, double b, double r,
						   double tt, double tl, double tb, double tr,
						   double x, double y, double dz, double cx, double cy, double da, CString strMsg, DWORD dwFlags)
{
	// This function has the same functionality than glDrawText
	// But draws extra-edges if some-text outs about limits

	// Width and height about viewport
	//double dWidth = fabs(l-r);
	//double dHeight = fabs(t-b);

	// Proceced dimmentions about viewport
	double dt = max(t, b);
	double db = min(t, b);
	double dr = max(l, r);
	double dl = min(l, r);

	// Proceced dimmentions about extended viewport
	double det = dt+fabs(tt);
	double deb = db-fabs(tb);
	double der = dr+fabs(tr);
	double del = dl-fabs(tl);

	// nCntLin -> Number of lines
	int nCntLin = 1;
	int nCode = strMsg.Find(_T('\n'), 0);
	while(nCode != -1)
	{
		nCntLin++;
		nCode = strMsg.Find(_T('\n'), nCode+1);
	}

	// Start Y in?
	double dStartY = 0.0;
	if(dwFlags & DT_VCENTER) dStartY = y+cy/2*(double)(nCntLin);
	if(dwFlags & DT_BOTTOM) dStartY = y+cy*(double)(nCntLin);
	else dStartY = y;

	// Proceed with lines first
	int nBackCode = 0;	// Auxiliar for extract lines without \n
	nCode = -1;
	for(int z = 0; z < nCntLin; z++)
	{
		// Extract lines without \n (strDisp)
		nCode = strMsg.Find(_T('\n'), nBackCode);
		if(nCode == -1) nCode = strMsg.GetLength();
		CString strDisp = strMsg.Mid(nBackCode, nCode-nBackCode);

		// Start X in?
		double dStartX = 0.0;
		if(dwFlags & DT_CENTER || dwFlags & DT_RIGHT)
		{
			double dSizeStrDisp = 0.0;
			for(int a = 0; a < strDisp.GetLength(); a++)
			{
				dSizeStrDisp += m_dAdvx[GetIndex(strDisp.GetAt(a))]/m_dSizeFont*cy*cx;
			}
			if(dwFlags & DT_CENTER) dStartX = x-dSizeStrDisp/2;
			if(dwFlags & DT_RIGHT) dStartX = x-dSizeStrDisp;
		}
		else dStartX = x;

		// Proceed with characters
		for(int k = 0; k < strDisp.GetLength(); k++)
		{
			double dSizeX = m_dSizex[GetIndex(strDisp.GetAt(k))];
			double dSizeY = m_dSizey[GetIndex(strDisp.GetAt(k))];
			double dSizeDX = m_dSizex[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy*cx;
			double dSizeDY = m_dSizey[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy;

			// dy1 and dy2 -> Bottom and top coordinates about character
			// dx1 and dx2 -> Left and right coordinates about character
			double dy1 = dStartY-cy*double(z)-m_dOffsety[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy;
			double dy2 = dy1-dSizeDY;
			double dx1 = dStartX+m_dOffsetx[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy*cx;
			double dx2 = dx1+dSizeDX;
			dStartX += m_dAdvx[GetIndex(strDisp.GetAt(k))]/m_dSizeFont*cy*cx;

			// Proceced coordinates
			double pdy1[5]; double pdy2[5]; double pdx1[5]; double pdx2[5];		// Spacial coordinates
			double pdyt1[5]; double pdyt2[5]; double pdxt1[5]; double pdxt2[5];	// Relative Texture coordinates
			double pdya1[5]; double pdya2[5]; double pdxa1[5]; double pdxa2[5];	// Alpha for each

			// Clamp coordinates to limits of drawing
			pdy1[0]=clamp(dy1,db,dt); pdy2[0]=clamp(dy2,db,dt); pdx1[0]=clamp(dx1,dl,dr); pdx2[0]=clamp(dx1,dl,dr);
			pdy1[1]=clamp(dy1,dt,det); pdy2[1]=clamp(dy2,dt,det); pdx1[1]=clamp(dx1,dl,dr); pdx2[1]=clamp(dx1,dl,dr);
			pdy1[2]=clamp(dy1,deb,db); pdy2[2]=clamp(dy2,deb,db); pdx1[2]=clamp(dx1,dl,dr); pdx2[2]=clamp(dx1,dl,dr);
			pdy1[3]=clamp(dy1,db,dt); pdy2[3]=clamp(dy2,db,dt); pdx1[3]=clamp(dx1,del,dl); pdx2[3]=clamp(dx1,del,dl);
			pdy1[4]=clamp(dy1,db,dt); pdy2[4]=clamp(dy2,db,dt); pdx1[4]=clamp(dx1,db,deb); pdx2[4]=clamp(dx1,db,deb);

			for(int i = 0; i < 5; i++)
			{
				// Re-clamp coordinates to original coordinates
				pdy1[i]=clamp(pdy1[i],dy2,dy1); pdy2[i]=clamp(pdy2[i],dy2,dy1);
				pdx1[i]=clamp(pdx1[i],dx1,dx2); pdx2[i]=clamp(pdx2[i],dx1,dx2);
				// Calculate relative texture coordinates
				pdyt1[i]=fabs(pdy1[i]-dy2)/dSizeDY; pdyt2[i]=fabs(pdy2[i]-dy2)/dSizeDY;
				pdxt1[i]=fabs(pdx1[i]-dx1)/dSizeDX; pdxt2[i]=fabs(pdx2[i]-dx1)/dSizeDX;
				// Calculate Alpha
				if(db <= pdy1[i] && pdy1[i] <= dt) pdya1[i] = da;
				else if(deb <= pdy1[i] && pdy1[i] <= db) pdya1[i] = (pdy1[i]-deb)/(db-deb)*da;
				else if(dt <= pdy1[i] && pdy1[i] <= det) pdya1[i] = (pdy1[i]-dt)/(det-dt)*da;
				if(db <= pdy2[i] && pdy2[i] <= dt) pdya2[i] = da;
				else if(deb <= pdy2[i] && pdy2[i] <= db) pdya2[i] = (pdy2[i]-deb)/(db-deb)*da;
				else if(dt <= pdy2[i] && pdy2[i] <= det) pdya2[i] = (pdy2[i]-dt)/(det-dt)*da;
				if(dl <= pdx1[i] && pdx1[i] <= dr) pdxa1[i] = da;
				else if(del <= pdx1[i] && pdx1[i] <= dl) pdxa1[i] = (pdx1[i]-del)/(dl-del)*da;
				else if(dr <= pdx1[i] && pdx1[i] <= der) pdxa1[i] = (pdx1[i]-dr)/(der-dr)*da;
				if(dl <= pdx2[i] && pdx2[i] <= dr) pdxa2[i] = da;
				else if(del <= pdx2[i] && pdx2[i] <= dl) pdxa2[i] = (pdx2[i]-del)/(dl-del)*da;
				else if(dr <= pdx2[i] && pdx2[i] <= der) pdxa2[i] = (pdx2[i]-dr)/(der-dr)*da;

				// Now, draw it! (if possible)
				double drx1 = m_dCoordx[GetIndex(strDisp.GetAt(k))];
				double dry1 = 1.0-m_dCoordy[GetIndex(strDisp.GetAt(k))];
				if(pdy1[i] <= pdy2[i] || pdx1[i] >= pdx2[i])
				{
					g_gmMain->BindTexture(texture[m_nPage[GetIndex(strDisp.GetAt(k))]]);
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->TexCoord2d(drx1+dSizeX*pdxt1[i], dry1-dSizeY*pdyt1[i]); g_gmMain->Vertex3d(pdx1[i], pdy1[i], dz);
					g_gmMain->TexCoord2d(drx1+dSizeX*pdxt2[i], dry1-dSizeY*pdyt1[i]); g_gmMain->Vertex3d(pdx2[i], pdy1[i], dz);
					g_gmMain->TexCoord2d(drx1+dSizeX*pdxt2[i], dry1-dSizeY*pdyt2[i]); g_gmMain->Vertex3d(pdx2[i], pdy2[i], dz);
					g_gmMain->TexCoord2d(drx1+dSizeX*pdxt1[i], dry1-dSizeY*pdyt2[i]); g_gmMain->Vertex3d(pdx1[i], pdy2[i], dz);
					g_gmMain->End();
				}
			}
		}
		///////////////////
		nBackCode = nCode+1;
	}
}

void CFont2::Delete()
{
	if(m_nSizeTex!=0)
	{
		for(int i = 0; i < m_nSizeTex; i++)
		{
			texture[i]->Release();
			delete texture[i];
		}
		m_nSizeTex = 0;
		m_nSizeFace = 0;
		
		delete texture;
		m_nPage = (int*)max_realloc(m_nPage, m_nSizeFace*sizeof(int));
		m_dCoordx = (double*)max_realloc(m_dCoordx, m_nSizeFace*sizeof(double));
		m_dCoordy = (double*)max_realloc(m_dCoordy, m_nSizeFace*sizeof(double));
		m_dSizex = (double*)max_realloc(m_dSizex, m_nSizeFace*sizeof(double));
		m_dSizey = (double*)max_realloc(m_dSizey, m_nSizeFace*sizeof(double));
		m_dOffsetx = (double*)max_realloc(m_dOffsetx, m_nSizeFace*sizeof(double));
		m_dOffsety = (double*)max_realloc(m_dOffsety, m_nSizeFace*sizeof(double));
		m_dAdvx = (double*)max_realloc(m_dAdvx, m_nSizeFace*sizeof(double));
	}
}

double CFont2::CalcSingleLine(CString strMsg,  double cy, int nStart /*=0*/, double cx /*=1.0*/)
{
	double dSizeStrDispMax = 0.0;
	int nBegin = nStart;
	int nEnd = -1;
	while(nEnd != strMsg.GetLength())
	{
		nEnd = strMsg.Find(_T('\n'), nBegin);
		if(nEnd == -1) nEnd = strMsg.GetLength();
		double dSizeStrDisp = 0.0;
		for(int a = nBegin; a < nEnd; a++)
			dSizeStrDisp += m_dAdvx[GetIndex(strMsg.GetAt(a))]/m_dSizeFont*cy*cx;
		nBegin = nEnd+1;
		if(dSizeStrDisp > dSizeStrDispMax) dSizeStrDispMax = dSizeStrDisp;
	}
	return dSizeStrDispMax;
}

double CFont2::CalcSingleColumn(CString strMsg,  double cy, int nStart /*=0*/, double cx /*=1.0*/)
{
	int nCntLin = 1;
	int nCode = strMsg.Find(_T('\n'), nStart);
	while(nCode != -1)
	{
		nCntLin++;
		nCode = strMsg.Find(_T('\n'), nCode+1);
	}
	return cy*(double)(nCntLin);
}
