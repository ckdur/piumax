
#include "GraphTexture.h"

CGraphTexture::CGraphTexture(void)
{
	m_nGestor = 0;
	m_bFlipY = false;
#ifdef PIUMAX_OGL
	m_uId = 0;
#endif // PIUMAX_OGL
#ifdef PIUMAX_DX
	m_tex = NULL;
#endif // PIUMAX_DX
}

CGraphTexture::~CGraphTexture(void)
{
	Release();
}

int CGraphTexture::TexImage2D(int nMipMapLevels, int nFormat, int nType, int nWidth, int nHeight, void* pData)
{
	if(min(nWidth, nHeight) <=0) return 0;
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
        int nMipMaps = 0;
		int nSugMipMaps = 0;
		while(max(nWidth,nHeight)/(1<<nSugMipMaps)) nSugMipMaps++;
		if(nMipMapLevels <= 0)
			nMipMaps = nSugMipMaps;
		else nMipMaps = nMipMapLevels;
		GLenum format = GL_RGBA;
		GLint int_format = 3;
		switch(nFormat)
		{
			case PMT_RGBA: format = GL_RGBA; int_format = 4; break;
			case PMT_RGBX: format = GL_RGBA; int_format = 4; break;
			case PMT_RGB: format = GL_RGB; int_format = 3; break;
			case PMT_BGRA: format = GL_BGRA_EXT; int_format = 4; break;
			case PMT_BGRX: format = GL_BGRA_EXT; int_format = 4; break;
			case PMT_BGR: format = GL_BGR_EXT; int_format = 3; break;
			default: return 0; break;
		}
		if(nSugMipMaps == nMipMaps) {
			gluBuild2DMipmaps(GL_TEXTURE_2D, int_format, nWidth, nHeight, format, GL_UNSIGNED_BYTE, pData);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else 
		{
			for(int x=0; x < nMipMaps; x++)
			{
				GLsizei width = max(1,nWidth/(1<<x));
				GLsizei height = max(1,nHeight/(1<<x));
				if(width == nWidth && height == nHeight) 
				{
					glTexImage2D(GL_TEXTURE_2D, x, format, width, height, 0, format, GL_UNSIGNED_BYTE, pData);
				}
				else 
				{
					char* rdata = NULL;
					rdata = new char[width*height*int_format];
					gluScaleImage(format, nWidth, nHeight, GL_UNSIGNED_BYTE, pData, width, height, GL_UNSIGNED_BYTE, rdata);
					glTexImage2D(GL_TEXTURE_2D, x, format, width, height, 0, format, GL_UNSIGNED_BYTE, rdata);
					delete rdata;
					rdata = NULL;
				}
			}
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		
		/*if(nWidth==1) 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		else 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		if(nHeight==1) 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		else 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);*/
		// ** Format is ignored, must pass UCHAR's
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		// nMipMapLevels Are ignored, use it in CGraphMain::CreateTexture
		D3DLOCKED_RECT *pRect = 0;
		GLint int_format = 3;
		pRect = new D3DLOCKED_RECT();
		switch(nFormat)
		{
			case PMT_RGBA: int_format = 4; break;
			case PMT_RGBX: int_format = 4; break;
			case PMT_RGB: int_format = 3; break;
			case PMT_BGRA: int_format = 4; break;
			case PMT_BGRX: int_format = 4; break;
			case PMT_BGR: int_format = 3; break;
			default: return 0; break;
		}
		m_tex->LockRect(0, pRect, NULL, 0);
		unsigned char* data = (unsigned char*)pRect->pBits;
		unsigned char* sdata = (unsigned char*)pData;
		memcpy(data, sdata, nWidth*nHeight*int_format*sizeof(unsigned char));
		m_tex->UnlockRect(0);
		m_tex->GenerateMipSubLevels();
		delete pRect; pRect = NULL;
#endif // PIUMAX_DX
	}
	return 1;
}

void CGraphTexture::Release(void)
{
	m_nGestor = 0;
#ifdef PIUMAX_OGL
	glDeleteTextures(1, &m_uId);
	m_uId = 0;
#endif // PIUMAX_OGL
#ifdef PIUMAX_DX
	if(m_tex) m_tex->Release();
	m_tex=NULL;
#endif // PIUMAX_DX
}
