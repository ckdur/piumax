/* 
Compilation standard definition PIUMaX
*/
#ifndef PIUMAX_DEFCOMP
#define PIUMAX_DEFCOMP

// OpenGL definitions compatible with Win32, UNIX, Apple Macintosh
//#if defined(_WIN32) || defined(__WIN32__) || defined(UNIX) || defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
#define PIUMAX_OGL
//#endif // defined(_WIN32) || defined(__WIN32__) || defined(UNIX) || defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))

// DirectX definitions compatible with Win32 & XBOX
#if defined(_WIN32) || defined(__WIN32__) || defined(_XBOX)
//#define PIUMAX_DX
#ifdef PIUMAX_DX
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#endif
#endif // defined(_WIN32) || defined(__WIN32__)

// FMOD definitions
#define PIUMAX_FMOD

// ** Windows no-warnings compilations
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif


#endif // PIUMAX_DEFCOMP
