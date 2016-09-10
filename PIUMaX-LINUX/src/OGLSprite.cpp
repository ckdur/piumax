#include "stdafx.h"
#include "cmnpiu.h"
#include "OGLSprite.h"

COGLSprite::COGLSprite(void)
: m_nSize(0)
, m_dwNext(0)
, m_dwDuration(0)
, m_dwFrame(0)
{
	texture = (GLuint*)calloc(m_nSize,sizeof(GLuint));
}

COGLSprite::~COGLSprite(void)
{
	if(m_nSize != 0)
	{
		glDeleteTextures(m_nSize, texture);
		delete texture;
		m_nSize = 0;
	}
}
void COGLSprite::DeleteImage()
{
	if(m_nSize != 0)
	{
		glDeleteTextures(m_nSize, texture);
		m_nSize = 0;
		texture = (GLuint*)realloc(texture, m_nSize*sizeof(GLuint));
	}
}

int COGLSprite::InitImage(LPCTSTR chName, int nSize)
{
	if(m_nSize != 0)
	{
		glDeleteTextures(m_nSize, texture);
		m_nSize = 0;
		texture = (GLuint*)realloc(texture, m_nSize*sizeof(GLuint));
	}
	m_nSize = nSize;
	m_dwDuration = nSize;
	texture = (GLuint*)realloc(texture, m_nSize*sizeof(GLuint));
	glGenTextures(m_nSize, &texture[0]);

	png_byte pngsig[PNGSIGSIZE];
    int is_png = 0;
	FILE *hFile = _fopen(chName, _T("rb"));
	if(hFile == NULL)
	{
        _printf(_T("File not found: %ls\n"), chName);
        return 1;
	}
	fread(pngsig, 1, PNGSIGSIZE, hFile);
	is_png = !png_sig_cmp(pngsig, 0, PNGSIGSIZE);
	if(!is_png)
	{
		fclose(hFile);
		return m_uNullTex;
	}

	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr)
	{
		fclose(hFile);
		return 0;
	}
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		fclose(hFile);
		return 0;
	}
	if (setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
		fclose(hFile);
		return 0;
	}

	png_set_read_fn(pngPtr, (voidp)hFile, userReadData);
	png_set_sig_bytes(pngPtr, PNGSIGSIZE);
    png_read_info(pngPtr, infoPtr);

	png_uint_32 imgWidth =  png_get_image_width(pngPtr, infoPtr);
    png_uint_32 imgHeight = png_get_image_height(pngPtr, infoPtr);
    png_uint_32 bitdepth   = png_get_bit_depth(pngPtr, infoPtr);
    png_uint_32 channels   = png_get_channels(pngPtr, infoPtr);
    png_uint_32 color_type = png_get_color_type(pngPtr, infoPtr);

	switch (color_type)
	{
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngPtr);
            break;
        case PNG_COLOR_TYPE_GRAY:
            if (bitdepth < 8)
				png_set_expand_gray_1_2_4_to_8(pngPtr);
            break;
    }
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(pngPtr);
    if (bitdepth == 16)
        png_set_strip_16(pngPtr);

	png_bytep* rowPtrs = new png_bytep[imgHeight];
	const char* data = new char[imgWidth * imgHeight * bitdepth * channels / 8];
    const unsigned int stride = imgWidth * bitdepth * channels / 8;
    for (size_t i = 0; i < imgHeight; i++)
	{
        unsigned int q = (imgHeight - i - 1) * stride;
        rowPtrs[i] = (png_bytep)data + q;
    }

	png_read_image(pngPtr, rowPtrs);
	png_read_end(pngPtr, NULL);
	png_destroy_read_struct(&pngPtr, &infoPtr, NULL); fclose(hFile);
	delete rowPtrs;

	int mode = GL_RGBA;
	if(channels == 3) mode = GL_RGB;
	if(channels != 3 && channels != 4)
	{
		delete data;
		return 0;
	}

	int nRx = GetPowerOf2(imgWidth/nSize);
	int nRy = GetPowerOf2(imgHeight);
	bool bIsResize = (nRx != (imgWidth/nSize) || nRy != imgHeight);
	bool bIsSection = nSize>1;
	char *pdata, *rdata;
	if(bIsSection) pdata = new char[imgWidth/nSize*imgHeight*channels*bitdepth/8];
	if(bIsResize) rdata = new char[nRx*nRy*channels*bitdepth/8];
	for(int x = 0; x < nSize; x++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[x]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		if(bIsSection)
			GetSeccion((void*)data, imgWidth, imgHeight, channels, x*imgWidth/nSize, 0, imgWidth/nSize, imgHeight, pdata);
		if(bIsResize && bIsSection)
			gluScaleImage(mode, imgWidth/nSize, imgHeight, GL_UNSIGNED_BYTE, pdata, nRx, nRy, GL_UNSIGNED_BYTE, rdata);
		if(bIsResize && !bIsSection)
			gluScaleImage(mode, imgWidth/nSize, imgHeight, GL_UNSIGNED_BYTE, data, nRx, nRy, GL_UNSIGNED_BYTE, rdata);
		if(bIsResize)
			gluBuild2DMipmaps(GL_TEXTURE_2D, mode, nRx, nRy, mode,GL_UNSIGNED_BYTE, rdata);
		else if(bIsSection)
			gluBuild2DMipmaps(GL_TEXTURE_2D, mode, nRx, nRy, mode,GL_UNSIGNED_BYTE, pdata);
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D, mode, nRx, nRy, mode,GL_UNSIGNED_BYTE, data);
	}
	// Ahora vamos a almacenar las variables de acceso a las texturas
	if(bIsSection) delete pdata;
	if(bIsResize) delete rdata;
	delete data;
	return 1;
}

void COGLSprite::Update(DWORD dwMiliseconds)
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

void COGLSprite::SetDuration(DWORD dwDuration)
{
	m_dwDuration = dwDuration;
}

GLuint COGLSprite::GetCurrentTex()
{
	if(m_nSize != 0) return texture[m_dwFrame];
	else return 0;
}
