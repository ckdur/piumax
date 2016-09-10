#ifndef PIUMAX_GRAPHTEXTURE
#define PIUMAX_GRAPHTEXTURE

#include "stdafx.h"
#include "defcomp.h"

class CGraphTexture
{
public:
	CGraphTexture(void);
	~CGraphTexture(void);
	int m_nGestor;
	bool m_bFlipY;
#ifdef PIUMAX_OGL
	GLuint m_uId;
#endif // PIUMAX_OGL
#ifdef PIUMAX_DX
	LPDIRECT3DTEXTURE9 m_tex;
#endif // PIUMAX_DX
	int TexImage2D(int nMipMapLevels, int nFormat, int nType, int nWidth, int nHeight, void* pData);
	void Release(void);
};

enum PMT_FORMATS_TEXTURE
{
	PMT_NO_FORMAT,
	PMT_RGBA,
	PMT_RGBX,
	PMT_RGB,
	PMT_BGRA,
	PMT_BGRX,
	PMT_BGR
};

#endif