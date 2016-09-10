#ifndef PIU_DEFBAS
#define PIU_DEFBAS

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#endif

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

#ifdef min
#undef min
#endif
#define min(X,Y) ((X) < (Y) ? (X) : (Y))

#ifdef max
#undef max
#endif
#define max(X,Y) ((X) > (Y) ? (X) : (Y))

#ifndef _WIN32
typedef int64_t   __int64;
#define __stdcall

#else // WIN32

#endif // WIN32

#ifndef clamp
#define clamp(N,DOWN,UP) (max(DOWN, min(UP, N)))
#endif



typedef void                                   *LPVOID;
typedef const void                             *LPCVOID;
typedef int             BOOL,       *PBOOL,    *LPBOOL;
typedef unsigned char   BYTE,       *PBYTE,    *LPBYTE;
typedef unsigned char   UCHAR,      *PUCHAR;
typedef unsigned short  WORD,       *PWORD,    *LPWORD;
typedef unsigned short  USHORT,     *PUSHORT;
typedef int             INT,        *PINT,     *LPINT;
typedef unsigned int    UINT,       *PUINT;
typedef float           FLOAT,      *PFLOAT;
typedef char                        *PSZ;

typedef long            LONG,                  *LPLONG;
typedef unsigned long   DWORD,      *PDWORD,   *LPDWORD;
typedef unsigned long   ULONG,      *PULONG;
typedef unsigned long   DWORD_PTR;

// ** Text auxiliars
#ifndef _WIN32

#define CALLBACK    __stdcall

#define DT_TOP                      0x00000000
#define DT_LEFT                     0x00000000
#define DT_CENTER                   0x00000001
#define DT_RIGHT                    0x00000002
#define DT_VCENTER                  0x00000004
#define DT_BOTTOM                   0x00000008
#define DT_WORDBREAK                0x00000010
#define DT_SINGLELINE               0x00000020
#define DT_EXPANDTABS               0x00000040
#define DT_TABSTOP                  0x00000080
#define DT_NOCLIP                   0x00000100
#define DT_EXTERNALLEADING          0x00000200
#define DT_CALCRECT                 0x00000400
#define DT_NOPREFIX                 0x00000800
#define DT_INTERNAL                 0x00001000

#define _abs64(x) abs((long long)(x))

#endif // WIN32

// *******************************************

#ifndef MAKEWORD
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#endif

#ifndef MAKELONG
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#endif

#ifndef LOWORD
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#endif

#ifndef HIWORD
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#endif

#ifndef LOBYTE
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#endif

#ifndef HIBYTE
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#endif

#define HFILE_ERROR     ((HFILE)-1)

#ifndef _T
#ifdef UNICODE
#define _T(x) L##x
#else
#define _T(x) x
#endif // UNICODE
#endif //_T

#ifdef UNICODE
#define STROP L"%ls"
#define SLS "%ls"
#else
#define STROP "%s"
#define SLS "%s"
#endif

#ifndef _WIN32

typedef wchar_t WCHAR;

typedef char* LPCH;
typedef char* LPSTR;
typedef const char* LPCSTR;

typedef wchar_t* LPWCH;
typedef wchar_t* LPWSTR;
typedef const wchar_t* LPCWSTR;

#ifdef UNICODE
typedef wchar_t TCHAR, _TCHAR;
typedef wchar_t *PTCHAR;
typedef wchar_t TBYTE;
typedef wchar_t *PTBYTE;

typedef LPWCH LPTCH, PTCH;
typedef LPWSTR PTSTR, LPTSTR;
typedef LPCWSTR PCTSTR, LPCTSTR;
typedef LPWSTR LP;

typedef const wchar_t *LPCOLESTR;

#else

typedef char TCHAR, _TCHAR;
typedef char *PTCHAR;
typedef unsigned char TBYTE;
typedef unsigned char *PTBYTE;

typedef LPCH LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;
#endif // UNICODE

#ifdef UNICODE
#define __TEXT(quote) L##quote
#else   // UNICODE
#define __TEXT(quote) quote
#endif

//typedef const char *LPCOLESTR;

#define TEXT(quote) __TEXT(quote)

#endif // _WIN32

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

//#endif //_MSC_VER

#define GetAValue(rgb)      (LOBYTE((rgb)>>24))


#define MAX_SKINS 20
#define MAX_CHARACTERS 12

// Definiciones para los indices items especiales
#define ITEM_SEARCHLINES (MAX_SKINS+MAX_CHARACTERS)
#define ITEM_LIFE (MAX_SKINS+MAX_CHARACTERS+1)
#define ITEM_LUCK (MAX_SKINS+MAX_CHARACTERS+2)
#define ITEM_BGAOFF (MAX_SKINS+MAX_CHARACTERS+3)
#define ITEM_SUPRIMELVL (MAX_SKINS+MAX_CHARACTERS+4)
#define ITEM_MOREEXP (MAX_SKINS+MAX_CHARACTERS+5)
#define ITEM_ALLSONG (MAX_SKINS+MAX_CHARACTERS+6)

#define TIMING_EASY 400 // NOT USED
#define TIMING_NORMAL 300 // NOT USED
#define TIMING_HARD 200 // ATENTION: THIS I'LL BE MS TIMING

#define BAR_DECR_1 0.05
#define BAR_DECR_2 0.06
#define BAR_DECR_3 0.07
#define BAR_DECR_4 0.08
#define BAR_DECR_5 0.09
#define BAR_DECR_6 0.1
#define BAR_DECR_7 0.11
#define BAR_INCR_1 0.01
#define BAR_INCR_2 0.005
#define BAR_INCR_3 0.005
#define BAR_INCR_4 0.004
#define BAR_INCR_5 0.003
#define BAR_INCR_6 0.002
#define BAR_INCR_7 0.001

#define POW_TIMER_EASY 0.4
#define POW_TIMER_NORMAL 0.75 // Lineal timing
#define POW_TIMER_HARD 1.0

#define ARGNOTE_NOMISS 0x00000001
#define ARGNOTE_NOCOUNT 0x00000002

#ifdef UNICODE

#ifndef WIN32
#define _tcscmp wcscmp
#define _tcslen wcslen
#define _tfopen _wfopen
#endif // WIN32

#define _fopen _wfopen

#define _fprintf fwprintf
#define _fscanf fwscanf
#define _fgets fgetws
#define _fputs fputws
#define _sprintf swprintf
#define _sscanf swscanf
#define _strcpy wcscpy
#define _printf wprintf
#define _tmkdir _wmkdir
#define _tchdir _wchdir
#else	// UNICODE

#ifndef _WIN32
#define _tcscmp strcmp
#define _tcslen strlen
#define _tfopen fopen
#endif // WIN32

#ifdef _WIN32
#define _tfopen_k fopen_s
#else	// WIN32
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
#endif // UNICODE

#define MAIN_CRASH(message, code) \
_printf(STROP _T("(code = 0x%x)"), message, code); \
exit(code);

#define MAIN_TITLE "PIU MaX XeneX (2.0) Build 0001"


#endif // PIU_DEFBAS

