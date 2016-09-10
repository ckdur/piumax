/*
Archivo de definiciones principales para PIUMaX2 adaptado a MaxEdit

Ck-Dur
*/

#ifndef MAXEDIT_STDAFX
#define MAXEDIT_STDAFX

// stdafx.h: archivo de inclusion para archivos de inclusion estandar del sistema,
// o archivos de inclusian especificos del proyecto utilizados frecuentemente,
// pero cambiados rara vez

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

#include "defcomp.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
#ifdef main
#undef main
#endif

//#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>

#if defined (UNIX) || defined(__linux__)
#include <strings.h>
#include <cassert>

//#define ASSERT assert
#define stricmp strcasecmp

#endif


#include <vector>
#include <map>

/*THIS IS BULLSHIT, WE NEED TO IMPORT ONLEY WHAT WE NEED*/
using namespace std;
/*using std::cin;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::ios;
using std::vector;
using std::map;
using std::wstring;
using std::string;*/

#include <png.h>
#define PNGSIGSIZE 8

#ifndef PIUMAX_2
#include <gtk/gtk.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinNT.h>
#include <Shlobj.h>
#include <tchar.h>
#include <direct.h>
#endif // _WIN32

#if (defined(UNIX) || defined(__linux__))
#include <wchar.h>
#include <unistd.h>
#endif // UNIX

#ifdef MS_ACTIVATEAPI
#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#ifdef __cplusplus
}
#endif
#endif

#ifdef PIUMAX_DX

#include <d3d9.h>
#include <D3dx9math.h>
#include <D3D9Types.h>
#include <DxErr.h>

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#endif // PIUMAX_DX


#ifdef PIUMAX_OGL
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

//#ifdef _WIN32
#include <GL/glew.h>
//#else
//#include <GL/gl.h>
//#include <GL/glu.h>
//#endif

#endif // defined(__APPLE__) && defined(__MACH__)
#endif // PIUMAX_OGL

#ifdef PIUMAX_FMOD
#include <fmodex/fmod.h>
#include <fmodex/fmod_errors.h>
#endif // PIUMAX_FMOD

#include "StdString.h"	// MFC Standard CString Class

#include "defbas.h"		// Definiciones basicas del Windows

#ifndef CStringArray
#define CStringArray vector<CString>
#endif



#ifdef __GNUC__
FILE* _wfopen(LPCWSTR strFile, LPCWSTR strAttr);
#endif // __GNUC__

void* max_realloc(void* prev, size_t nSize);

//#endif // STDAFX_PIUMAX2


#endif
