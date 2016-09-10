#include "stdafx.h"
#include "cmnpiu.h"
#include "basutil.h"
#include "OGLSprite.h"

CSprite::CSprite(void)
: m_nSize(0)
, m_dwNext(0)
, m_dwDuration(0)
, m_dwFrame(0)
{
	texture = NULL;
}

CSprite::~CSprite(void)
{
	DeleteImage();
}
void CSprite::DeleteImage()
{
	if(m_nSize != 0)
	{
		for(int i = 0; i < m_nSize; i++)
		{
			delete texture[i];
		}
		delete texture;
		texture = NULL;
		m_nSize = 0;
	}
}

int CSprite::InitImage(LPCTSTR chName, int nSize)
{
	return InitImage_mn(chName, nSize, 1);
}

int CSprite::InitImage_mn(LPCTSTR chName, int m, int n)
{
	DeleteImage();
	unsigned char* data = NULL;
	int w = 0, h = 0;
	if(m <= 0 || n <= 0) return 0;
	if(!OpenReadImagePNG(chName, &data, &w, &h))
	{
		data = TexEmpty(64, 64, 0, 0, 0, 255);
		m_nSize=1;
	}
	else m_nSize = m*n;
	m_dwDuration = 0;
	texture = new CGraphTexture*[m_nSize];

	int mode = PMT_RGBA;

	int sw = w/m, sh = h/n;

	int nRx = GetPowerOf2(sw);
	int nRy = GetPowerOf2(sh);
	bool bIsResize = (nRx != (sw) || nRy != (sh));
	bool bIsSection = m_nSize>1;
	unsigned char *pdata, *rdata;
	if(bIsSection) pdata = new unsigned char[sw*sh*4];
	if(bIsResize) rdata = new unsigned char[nRx*nRy*4];
	for(int y = 0; y < n; y++)
	{
		for(int x = 0; x < m; x++)
		{
			if(bIsSection) GetSeccion(data, w, h, 4, x*sw, y*sh, sw, sh, pdata);
			else pdata = data;
			if(bIsResize) ZoomSurface(sw, sh, pdata, nRx, nRy, rdata);
			else rdata = pdata;
			int nInd = min(m_nSize-1, y*m+x);
			texture[nInd] = new CGraphTexture();
			g_gmMain->CreateTexture(-1, mode, nRx, nRy, texture[nInd]);
			g_gmMain->BindTexture(texture[nInd]);
			texture[nInd]->TexImage2D(-1, mode, 0, nRx, nRy, rdata);
		}
	}
	// Ahora vamos a almacenar las variables de acceso a las texturas
	if(bIsSection) delete pdata; 
	if(bIsResize) delete rdata;
	delete data;
	return 1;
}

void CSprite::Update(DWORD dwMiliseconds)
{
	m_dwNext += dwMiliseconds;
	if(m_dwDuration != 0) m_dwFrame = (m_dwNext*(DWORD)m_nSize)/m_dwDuration;
	else m_dwFrame = 0;
	if(m_dwFrame >= m_nSize)
	{
		m_dwFrame = 0;
		m_dwNext = 0;
	}
}

void CSprite::SetDuration(DWORD dwDuration)
{
	m_dwDuration = dwDuration;
}

CGraphTexture* CSprite::GetCurrentTex()
{
	if(m_nSize != 0) return texture[m_dwFrame];
	else return NULL;
}
