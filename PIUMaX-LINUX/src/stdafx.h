// stdafx.h: archivo de inclusión para archivos de inclusión estándar del sistema,
// o archivos de inclusión específicos del proyecto utilizados frecuentemente,
// pero cambiados rara vez

#ifndef PIUMAX_STDAFX
#define PIUMAX_STDAFX

#ifndef UNICODE
#define UNICODE
#endif // UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif // UNICODE

#ifndef NO_DECLARE_PIUEXTENDED
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <time.h>
#include <cstdint>
#include <vector>
using std::vector;

#ifdef __GNUC__
#include <sys/stat.h>
#define _mkdir(a) mkdir(a, S_IREAD|S_IWRITE)
#endif

/*#ifndef _WIN32
#define NOMINMAX
#endif*/

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#ifndef _WIN32
#include <GL/glx.h>
#endif
#endif

#include <fmod/fmod.h>
#include <fmod/fmod_errors.h>

#include "png.h"
#define PNGSIGSIZE 8

#ifndef __GNUC__
//#pragma comment (lib,"libpng.lib")
//#pragma comment (lib,"zlibstat.lib")
//#pragma comment (lib,"Opengl32.lib")
//#pragma comment (lib,"glu32.lib")
//#pragma comment (lib,"sdl.lib")
//#pragma comment (lib,"fmodex_vc.lib")
//#pragma comment (lib,"pthreadVC2.lib")
#endif

#endif //NO_DECLARE_PIUEXTENDED

#include <SDL/SDL_thread.h>
#include <SDL/SDL_mutex.h>

#ifdef _WIN32
#include <tchar.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif
#include <pthread.h>
#ifdef __cplusplus
}
#endif

#define ARGNOTE_NOMISS 0x00000001
#define ARGNOTE_NOCOUNT 0x00000002

#ifdef UNICODE

#define _fopen _wfopen

#define _fprintf fwprintf
#define _fscanf fwscanf
#define _fgets fgetws
#define _fputs fputws
#ifdef _WIN32
#define _sprintf swprintf
#else
#define _sprintf(a,...) swprintf(a, sizeof(a)/sizeof(wchar_t),__VA_ARGS__)
#endif
#define _sscanf swscanf
#define _strcpy wcscpy
#define _printf wprintf
#define _tmkdir _wmkdir
#define _tchdir _wchdir
#else
#ifdef WIN32
#define _tfopen_k fopen_s
#else
#define _tfopen_k fopen
#endif // WIN32
#define _fprintf fprintf
#define _fscanf fscanf
#define _fgets fgets
#define _fputs fputs
#define _sprintf sprintf
#define _sscanf sscanf
#define _strcpy strcpy
#define _printf printf
#define _tmkdir _mkdir
#define _tchdir _chdir
#endif



#ifndef PIU_NOINC_STRING
#include "StdString.h"	// MFC Standard CString Class
#endif

#include "defbas.h"		// Definiciones basicas del Windows (FUCK!)
//using namespace std;

#include "MovieStream2.h"
#include "OGLSprite.h"
#include "intdef.h"
#include "resdef.h"
#include "eventdef.h"


#ifndef _WIN32

void _tsplitpath(LPCTSTR lpszPathName, LPTSTR strDrive, LPTSTR strDir, LPTSTR strFile, LPTSTR strExt);
// Splits a Path into 4 parts (Directory, Drive, Filename, Extention).  Supports UNC path names.
/* If Path is a directory (eg. c:\games\stepmania"), append a slash so the last
 * element will end up in Dir, not FName: "c:\games\stepmania\". */
void splitpath( const CStdStringA &Path, CStdStringA& Dir, CStdStringA& Filename, CStdStringA& Ext );

class Regex {
	void *reg;
	unsigned backrefs;
    CStdStringA pattern;
    void Compile();
    void Release();
public:
	Regex(const CStdStringA &pat = "");
	Regex(const Regex &rhs);
	Regex &operator=(const Regex &rhs);
	~Regex();
	void Set(const CStdStringA &str);
	bool Compare(const CStdStringA &str);
	bool Compare(const CStdStringA &str, vector<CStdStringA> &matches);
};

FILE* _wfopen(LPCWSTR strFile, LPCWSTR strMode);

#endif

#endif // ONCE
