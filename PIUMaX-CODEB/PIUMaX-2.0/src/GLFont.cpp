#include "stdafx.h"
#include "GLFont.h"
#include "basutil.h"
#include "cmnpiu.h"

CFont::CFont(void)
: m_nDivX(0)
, m_nDivY(0)
{
	texture = NULL;
}

CFont::~CFont(void)
{
	Delete();
}

int CFont::LoadFont(TCHAR* chFile, int nDivX, int nDivY)
{
	if(nDivX <0) return 0;
	if(nDivX != 0) delete texture;
	m_nDivX = nDivX;
	m_nDivY = nDivY;
	texture = new CGraphTexture();
	texture = GenTexImage(chFile);
	return 1;
}

void CFont::glTextOut(double x, double y, double cx, double cy, CString strMsg)
{
}

void CFont::glDrawText(double t, double l, double b, double r, double x, double y, double dz, double cx, double cy, CString strMsg, DWORD dwFlags)
{
	if(m_nDivX < 0) return;
	//double dWidth = fabs(l-r);
	//double dHeight = fabs(t-b);
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
		// Código de dibujo
		// ** Por ahora se hará totalmente en ANSI (rango 0 - 255),
		// ** Si sale un caracter que sea mayor que 256, se asimilará
		// ** Como pregunta o simplemente no se mostrará
		double dStartX = 0.0;
		if(dwFlags & DT_CENTER) dStartX = x-cx/2*(double)(strDisp.GetLength());
		else dStartX = x;
		double dy1 = dStartY-cy*(double)(z);
		double dy2 = dStartY-cy*(double)(z+1);
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
			//g_gmMain->PushMatrix();
			//g_gmMain->Translate(l, t, 0.0);
			for(int k = 0; k < strDisp.GetLength(); k++)
			{
				double dx1 = dStartX+cx*(double)(k);
				double dx2 = dStartX+cx*(double)(k+1);
				double dxt1 = 0.0;
				double dxt2 = 1.0;
				if(dx1 < dl && dx2 > dl)
				{
					dxt1 = 1-(dl-dx1)/cx;
					dx1 = dl;
				}
				if(dx1 < dr && dx2 > dr)
				{
					dxt2 = (dx2-dr)/cx;
					dx2 = dr;
				}
				if(dx1 >= dl && dx2 <= dr)
				{
					TCHAR ch = strDisp.GetAt(k);
					double drx1 = ((double)(ch%m_nDivX))/(double)m_nDivX;
					double dry1 = ((double)(ch/m_nDivY))/(double)m_nDivY;
					/*double drx2 = ((double)(ch%m_nDivX+1))/(double)m_nDivX;
					double dry2 = ((double)(ch/m_nDivY+1))/(double)m_nDivY;*/
					double dSizeX = 1/(double)m_nDivX;
					double dSizeY = 1/(double)m_nDivY;
					g_gmMain->BindTexture(texture);
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->TexCoord2d(drx1+dSizeX*dxt1, dry1+dSizeY*dyt1); g_gmMain->Vertex3d(dx1, dy1, dz);
					g_gmMain->TexCoord2d(drx1+dSizeX*dxt2, dry1+dSizeY*dyt1); g_gmMain->Vertex3d(dx2, dy1, dz);
					g_gmMain->TexCoord2d(drx1+dSizeX*dxt2, dry1+dSizeY*dyt2); g_gmMain->Vertex3d(dx2, dy2, dz);
					g_gmMain->TexCoord2d(drx1+dSizeX*dxt1, dry1+dSizeY*dyt2); g_gmMain->Vertex3d(dx1, dy2, dz);
					g_gmMain->End();
				}
			}
			//g_gmMain->PopMatrix();
		}
		///////////////////
		nBackCode = nCode+1;
	}
}

void CFont::Delete()
{
	if(m_nDivX != 0)
	{
		delete texture;
		texture = NULL;
	}
}
