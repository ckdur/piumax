#ifndef BAS_UTIL
#define BAS_UTIL

#include "stdafx.h"
//#include "cmnpiu.h"

// ** Definición principal de gestores
#include "GraphMain.h"
#include "MaxLua.h"

#include "MID.h"
#include "libc_max_common.h"

void GetSeccion(void *pSrcData, int nWidth, int nHeight, int nChan, int nx, int ny, int cx, int cy, void *pDstData);
void ZoomSurface(int win, int hin, void* pInData, int wout, int hout, void* pOutData);

int GetPowerOf2(int nSize);

extern int imgWidth;
extern int imgHeight;
void userReadData(png_structp pngPtr, png_bytep data, png_size_t length);
CGraphTexture* GenTexImage(LPCTSTR chName);
CGraphTexture* GenTexImagePNG(LPCTSTR chName);
CGraphTexture* GenTexEmpty(int nx, int ny, unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na);
unsigned char* TexEmpty(int nx, int ny, unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na);
int OpenReadImagePNG(LPCTSTR chName, unsigned char** data, int* nWidth, int* nHeight);

#ifndef clamp
#define clamp(val,low,high)		( max( (low), min((val),(high)) ) )
#endif

#define wrap(val,low,high)		(val - floor((val - low) / (high - low)) * (high - low));
#define mag(x,y,z)				(sqrt(x*x+y*y+z*z))

int wrapi(int val, int low, int high);

#ifdef PIUMAX_FMOD
int ERRCHECK(FMOD_RESULT result);
#endif

void GetDirStr(CString strFile, CString* strDir);

// ** UTILITIES OF LUA

int MLUA_readMID(lua_State* L, int idx, MID* mid);
int MLUA_setMID(lua_State* L, int idx, MID mid);

int MLUA_TraduceArgs(lua_State* L, int nArg, ...);

// Stepmania declarations

// // Move val toward other_val by to_move.
void fapproach( float& val, float other_val, float to_move );

/* Return a positive x mod y. */
float fmodfp(float x, float y);

int power_of_two(int input);
bool IsAnInt( const CString &s );
bool IsHexVal( const CString &s );
float HHMMSSToSeconds( const CString &sHMS );
CString SecondsToHHMMSS( float fSecs );
CString SecondsToMMSSMsMs( float fSecs );
CString SecondsToMMSSMsMsMs( float fSecs );
CString PrettyPercent( float fNumerator, float fDenominator );
inline CString PrettyPercent( int fNumerator, int fDenominator ) { return PrettyPercent( float(fNumerator), float(fDenominator) ); }
CString Commify( int iNum );


struct tm GetLocalTime();

CString ssprintf( const char *fmt, ...) /*PRINTF(1,2)*/;
CString vssprintf( const char *fmt, va_list argList );

// Splits a Path into 4 parts (Directory, Drive, Filename, Extention).  Supports UNC path names.
/* If Path is a directory (eg. c:\games\stepmania"), append a slash so the last
 * element will end up in Dir, not FName: "c:\games\stepmania\". */
void splitpath( const CString &Path, CString &Dir, CString &Filename, CString &Ext );

CString SetExtension( const CString &path, const CString &ext );
CString GetExtension( const CString &sPath );

// Splits a CString into an CStringArray according the Delimitor.
void split( const CString &Source, const CString &Delimitor, CStringArray& AddIt, const bool bIgnoreEmpty = true );
void split( const wstring &Source, const wstring &Delimitor, vector<wstring> &AddIt, const bool bIgnoreEmpty = true );

/* In-place split. */
void split( const CString &Source, const CString &Delimitor, int &begin, int &size, const bool bIgnoreEmpty = true );
void split( const wstring &Source, const wstring &Delimitor, int &begin, int &size, const bool bIgnoreEmpty = true );

/* In-place split of partial string. */
void split( const CString &Source, const CString &Delimitor, int &begin, int &size, int len, const bool bIgnoreEmpty ); /* no default to avoid ambiguity */
void split( const wstring &Source, const wstring &Delimitor, int &begin, int &size, int len, const bool bIgnoreEmpty );

// Joins a CStringArray to create a CString according the Deliminator.
CString join( const CString &Delimitor, const CStringArray& Source );
CString join( const CString &Delimitor, CStringArray::const_iterator begin, CStringArray::const_iterator end );

void TrimLeft(CString &str, const char *s = "\r\n\t ");
void TrimRight(CString &str, const char *s = "\r\n\t ");
void StripCrnl(CString &s);

class Regex {
	void *reg;
	unsigned backrefs;
    CString pattern;
    void Compile();
    void Release();
public:
	Regex(const CString &pat = "");
	Regex(const Regex &rhs);
	Regex &operator=(const Regex &rhs);
	~Regex();
	void Set(const CString &str);
	bool Compare(const CString &str);
	bool Compare(const CString &str, vector<CString> &matches);
};


#endif // BAS_UTIL
