#ifndef LIBC_MAX_COMMON
#define LIBC_MAX_COMMON

#include "stdafx.h"

int UnicodeToAnsi(const wchar_t* pszW, char* pszA);

int AnsiToUnicode(char* pszA, wchar_t* pszW);

void GetMaxCurrentDirectory(TCHAR* ch);

void SetMaxCurrentDirectory(LPCTSTR ch);

void GetMaxProcessDirectory(TCHAR* ch);

void GetMaxUserDirectory(TCHAR* ch);

char* Max_strcpy(char* dst, const wchar_t* src);

wchar_t* Max_strcpy(wchar_t* dst, const char* src);

char* Max_strcpy(char* dst, const char* src);

wchar_t* Max_strcpy(wchar_t* dst, const wchar_t* src);

char* Max_strcpy_utf8(char* dst, size_t nCount, const wchar_t* src);

wchar_t* Max_strcpy_utf8(wchar_t* dst, size_t nCount, const char* src);

char* Max_strcpy_utf8(char* dst, size_t nCount, const char* src, int nDirect);

int Max_init_iconv(void);

void Max_delete_iconv(void);

#ifdef _WIN32
int Utf16ToUtf32(const wchar_t* pszA, wchar_t* pszW, size_t nCount);
int Utf32ToUtf16(const wchar_t* pszA, wchar_t* pszW, size_t nCount);
#endif

int Utf16ToUtf32(const char16_t* pszA, wchar_t* pszW, size_t nCount);
int Utf32ToUtf16(const wchar_t* pszA, char16_t* pszW, size_t nCount);

#endif // LIBC_MAX_COMMON
