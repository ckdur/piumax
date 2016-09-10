//#define DRAWDIB_INCLUDE_STRETCHDIB
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _AFXDLL
#include <windows.h>
#endif

#include <tchar.h>
#include <vfw.h>
#pragma comment (lib,"vfw32.lib") 

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>	// Header File For The OpenGL32 Library
#include <OpenGL/glu.h>	// Header File For The GLu32 Library
#else
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>

#pragma once

class CMovieStream
{
public:
	CMovieStream(void);
	bool OpenAVI(LPCTSTR szFile);
	void GrabAVIFrame();
	int Update(DWORD milliseconds);
public:
	virtual ~CMovieStream(void);
public:
	char 				*data;
	PAVISTREAM pavi;
	AVISTREAMINFO		psi;
	PGETFRAME			pgf;
	long				lastframe;
	int					width;
	int					height;
	int					nwidth;
	int					nheight;
	int					mpf;
	DWORD next;
	DWORD frame;
	HBITMAP hBitmap;
	void SetZeroCount(void);
	void CloseAVI(void);
	bool bLoad;
	bool bLoop;
	bool bResize;
	int GetPowerOf2(int nSize);
	void SetResize(void);
	void ClearResize(void);
};
