#include "stdafx.h"
#include "GLFont.h"

CGLFont::CGLFont(void)
: m_nDivX(0)
, m_nDivY(0)
{
}

CGLFont::~CGLFont(void)
{
	if(m_nDivX != 0) glDeleteTextures(1, &texture);
}

int CGLFont::GetPower(int nSize)	// Devuelve el numero elevado al 2 mas cercano al numero especificado
{
	int nTest = 0, power = 0;
	while(nTest < nSize)
	{
		power++;
		nTest = (int)pow((double)2, power);
	}
	return nTest;
}

int CGLFont::LoadFont(TCHAR* chFile, int nDivX, int nDivY)
{
	if(nDivX != 0) glDeleteTextures(1, &texture);
	m_nDivX = nDivX;
	m_nDivY = nDivY;
	texture = GenTexImage(chFile);
	return 1;
}

void CGLFont::glTextOut(double x, double y, double cx, double cy, CString strMsg)
{
}

void CGLFont::glDrawText(double t, double l, double b, double r, double x, double y, double dz, double cx, double cy, CString strMsg, DWORD dwFlags)
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
			//glPushMatrix();
			//glTranslated(l, t, 0.0);
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
					glBindTexture(GL_TEXTURE_2D, texture);
					glBegin(GL_QUADS);
					glTexCoord2d(drx1+dSizeX*dxt1, dry1+dSizeY*dyt1); glVertex3d(dx1, dy1, dz);
					glTexCoord2d(drx1+dSizeX*dxt2, dry1+dSizeY*dyt1); glVertex3d(dx2, dy1, dz);
					glTexCoord2d(drx1+dSizeX*dxt2, dry1+dSizeY*dyt2); glVertex3d(dx2, dy2, dz);
					glTexCoord2d(drx1+dSizeX*dxt1, dry1+dSizeY*dyt2); glVertex3d(dx1, dy2, dz);
					glEnd();
				}
			}
			//glPopMatrix();
		}
		///////////////////
		nBackCode = nCode+1;
	}
}

void CGLFont::Delete()
{
	if(m_nDivX != 0) glDeleteTextures(1, &texture);
}
