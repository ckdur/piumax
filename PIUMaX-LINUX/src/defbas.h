#ifndef PIU_DEFBAS
#define PIU_DEFBAS

#ifndef NOMINMAX
//#ifdef max
//#undef max
//#endif

//#ifdef min
//#undef min
//#endif

#define max(a,b)   (((a) > (b)) ? (a) : (b))
#define min(a,b)   (((a) < (b)) ? (a) : (b))
#endif  /* NOMINMAX */

#ifndef _WIN32
inline double abs(double X){ return fabs(X);}
#endif

#ifdef UNICODE
#define STROP L"%ls"
#else   // UNICODE
#define STROP "%ls"
#endif

#ifndef _MSC_VER
typedef void                                   *LPVOID;
typedef const void                             *LPCVOID;
typedef int16_t             BOOL,       *PBOOL,    *LPBOOL;
typedef unsigned char   BYTE,       *PBYTE,    *LPBYTE;
typedef unsigned char   UCHAR,      *PUCHAR;
typedef unsigned short  WORD,       *PWORD,    *LPWORD;
typedef unsigned short  USHORT,     *PUSHORT;
typedef int             INT,        *PINT,     *LPINT;
typedef unsigned int    UINT,       *PUINT;
typedef float           FLOAT,      *PFLOAT;
typedef char                        *PSZ;
//#ifdef _MSC_VER
typedef int32_t                                   LONG;
typedef int32_t                                   *LPLONG;
typedef uint32_t   DWORD,      *PDWORD,   *LPDWORD;
typedef uint32_t   ULONG,      *PULONG;
typedef uint32_t   DWORD_PTR;
/*#else
typedef int                                    *LPLONG;
typedef unsigned int    DWORD,      *PDWORD,   *LPDWORD;
typedef unsigned int    ULONG,      *PULONG;
#endif*/

#ifndef _WIN32

typedef int64_t   __int64;

#define CALLBACK    __stdcall
#define WINAPI

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

inline int64_t _abs64 (int64_t n) { return llabs(n);}

#endif // WIN32

#define LOBYTE(w)              ((BYTE)((DWORD_PTR)(w) & 0xFF))
#define HIBYTE(w)              ((BYTE)((DWORD_PTR)(w) >> 8))

#define LOWORD(l)              ((WORD)((DWORD_PTR)(l) & 0xFFFF))
#define HIWORD(l)              ((WORD)((DWORD_PTR)(l) >> 16))

#define MAKEWORD(low,high)     ((WORD)(((BYTE)((DWORD_PTR)(low) & 0xFF)) | ((WORD)((BYTE)((DWORD_PTR)(high) & 0xFF))) << 8))
#define MAKELONG(low,high)     ((LONG)(((WORD)((DWORD_PTR)(low) & 0xFFFF)) | ((DWORD)((WORD)((DWORD_PTR)(high) & 0xFFFF))) << 16))

#ifdef MAX_PATH /* Work-around for Mingw */
#undef MAX_PATH
#endif /* MAX_PATH */

#define MAX_PATH        260
#define HFILE_ERROR     ((HFILE)-1)

#ifndef _T
#ifdef UNICODE
#define _T(x) L##x
#else
#define _T(x) x
#endif // UNICODE
#endif //_T

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

#else //UNICODE

typedef char TCHAR, _TCHAR;
typedef char *PTCHAR;
typedef unsigned char TBYTE;
typedef unsigned char *PTBYTE;

typedef LPCH LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;

typedef const char *LPCOLESTR;

#endif //UNICODE

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

typedef /* [string] */ const WCHAR *LPCOLESTR;

#endif //_MSC_VER

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


#endif // PIU_DEFBAS

