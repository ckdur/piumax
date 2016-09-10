#include "libc_max_common.h"
#include <iconv.h>

iconv_t g_icWC;
iconv_t g_icCW;

iconv_t g_icW16W32;
iconv_t g_icW32W16;

int UnicodeToAnsi(LPCWSTR pszW, LPSTR pszA)
{
    unsigned long cCharacters;
    if (NULL == pszA || NULL == pszW)
    {
        return 0;
    }
    cCharacters = (unsigned long)wcslen(pszW);
    if (0 >= wcstombs(pszA, pszW, cCharacters))
    {
        return 0;
    }
    *(pszA+cCharacters) = 0;
    return 1;
}

int AnsiToUnicode(LPCSTR pszA, LPWSTR pszW)
{
    unsigned long cCharacters;

    if (NULL == pszA || NULL == pszW)
    {
        return 0;
    }

    cCharacters = (unsigned long)strlen(pszA);

    // Covert to Unicode.
    if (0 >= mbstowcs(pszW, pszA, cCharacters))
    {
        return 0;
    }

	*(pszW+cCharacters) = 0;

    return 1;
}

int UnicodeToUtf8(LPCWSTR pszW, LPSTR pszA, size_t nCount)
{
    size_t cCharacters, nlength = nCount;
	memset(pszA, 0, nCount*sizeof(char));

    if (NULL == pszA || NULL == pszW || nCount <= 0)
    {
        return 0;
    }

    cCharacters = (unsigned long)wcslen(pszW);

    // Covert to Unicode.
#ifdef _WIN32
    cCharacters *= 2;
    size_t nRet = iconv(g_icCW, (const char**)&pszW, &cCharacters, (char**)&pszA, &nlength);
#else
    cCharacters *= 4;
    size_t nRet = iconv(g_icCW, (char**)&pszW, &cCharacters, (char**)&pszA, &nlength);
#endif

	//*(pszA+nCount-nlength) = 0;
    return 1;
}

int Utf8ToUnicode(LPCSTR pszA, LPWSTR pszW, size_t nCount)
{
    size_t cCharacters, nlength = nCount;
	memset(pszW, 0, nCount*sizeof(wchar_t));

    if (NULL == pszA || NULL == pszW || nCount <= 0)
    {
        return 0;
    }

    cCharacters = (size_t)strlen(pszA);

    // Covert to Unicode.
#ifdef _WIN32
    nlength *= 2;
    size_t nRet = iconv(g_icWC, (const char**)&pszA, &cCharacters, (char**)&pszW, &nlength);
#else
    nlength *= 4;
    size_t nRet = iconv(g_icWC, (char**)&pszA, &cCharacters, (char**)&pszW, &nlength);
#endif

	//*(pszW+nCount-nlength+1);

    return 1;
}

int Utf16ToUtf32(const char16_t* pszA, wchar_t* pszW, size_t nCount)
{
#if 1
    size_t cCharacters, nlength = nCount*4;
	memset(pszW, 0, nCount*sizeof(wchar_t));

    if (NULL == pszA || NULL == pszW || nCount <= 0)
    {
        return 0;
    }

    cCharacters = (size_t)char_traits<char16_t>::length(pszA);
    cCharacters *= 2;

    // Covert
#ifdef _WIN32
    size_t nRet = iconv(g_icW32W16, (const char**)&pszA, &cCharacters, (char**)&pszW, &nlength);
#else
	size_t nRet = iconv(g_icW32W16, (char**)&pszA, &cCharacters, (char**)&pszW, &nlength);
#endif

    return 1;
#else
    wcscpy(pszW, pszA);
    return 1;
#endif
}

int Utf32ToUtf16(const wchar_t* pszA, char16_t* pszW, size_t nCount)
{
#if 1
    size_t cCharacters, nlength = nCount*2;
	memset(pszW, 0, nCount*sizeof(char16_t));

    if (NULL == pszA || NULL == pszW || nCount <= 0)
    {
        return 0;
    }

    cCharacters = (size_t)char_traits<wchar_t>::length(pszA);
    cCharacters *= 4;

    // Covert
#ifdef _WIN32
	size_t nRet = iconv(g_icW16W32, (const char**)&pszA, &cCharacters, (char**)&pszW, &nlength);
#else
    size_t nRet = iconv(g_icW16W32, (char**)&pszA, &cCharacters, (char**)&pszW, &nlength);
#endif

    return 1;
#else
    wcscpy(pszW, pszA);
    return 1;
#endif
}

void GetMaxCurrentDirectory(TCHAR* ch)
{
	char strDirect[MAX_PATH];

	getcwd(strDirect, sizeof(TCHAR)*MAX_PATH);
	Max_strcpy(ch, strDirect);
}

void SetMaxCurrentDirectory(LPCTSTR ch)
{
	char strDirect[MAX_PATH];

	Max_strcpy(strDirect, ch);
	chdir(strDirect);
}

void GetMaxProcessDirectory(TCHAR* ch)
{
#ifdef _WIN32
	TCHAR strApp[MAX_PATH];
	TCHAR strDrive[MAX_PATH];
	TCHAR strDir[MAX_PATH];
	TCHAR strFile[MAX_PATH];
	TCHAR strExt[MAX_PATH];
	GetModuleFileName(NULL, strApp, MAX_PATH);
	_tsplitpath(strApp, strDrive, strDir, strFile, strExt);
	_sprintf(ch, _T("%s%s"), strDrive, strDir);
#endif // WIN32

#if defined (UNIX) || defined(__linux__)
    char buf[MAX_PATH + 1];
    if (readlink("/proc/self/exe", buf, sizeof(buf) - 1) == -1)
	{
        throw std::string("readlink() failed");
		return;
	}
    std::string str(buf);
    std::string stra; stra = str.substr(0, str.rfind('/')+1);
	Max_strcpy(ch, stra.c_str());
#endif
}

void GetMaxUserDirectory(TCHAR* ch)
{
#ifdef _WIN32
	if(!SUCCEEDED(SHGetFolderPath(NULL,
								 CSIDL_PERSONAL|CSIDL_FLAG_CREATE,
								 NULL,
								 0,
								 ch)))
	{
		throw std::string("SHGetFolderPath() failed");
		return;
	}
#endif

#if defined (UNIX) || defined(__linux__)
	// struct passwd *pw = getpwuid(getuid());
	const char *homedir =  getenv("HOME"); // pw->pw_dir;
	Max_strcpy(ch, homedir);
#endif
}

char* Max_strcpy(char* dst, const wchar_t* src)
{
	UnicodeToAnsi(src, dst);
	return dst;
}

wchar_t* Max_strcpy(wchar_t* dst, const char* src)
{
	AnsiToUnicode(src, dst);
	return dst;
}

char* Max_strcpy(char* dst, const char* src)
{
	return strcpy(dst, src);
}

wchar_t* Max_strcpy(wchar_t* dst, const wchar_t* src)
{
	return wcscpy(dst, src);
}

int Max_init_iconv(void)
{
	g_icWC = iconv_open("WCHAR_T", "UTF-8");
    if ((iconv_t) -1 == g_icWC) {
        perror("iconv_open WCHAR_T UTF-8");
        return 0;
    }

	g_icCW = iconv_open("UTF-8", "WCHAR_T");
    if ((iconv_t) -1 == g_icWC) {
        perror("iconv_open UTF-8 WCHAR_T");
        return 0;
    }

#if 1
    g_icW16W32 = iconv_open("UTF-16", "WCHAR_T");
    if ((iconv_t) -1 == g_icWC) {
        perror("iconv_open UTF-16 UTF-32");
        return 0;
    }

    g_icW32W16 = iconv_open("WCHAR_T", "UTF-16");
    if ((iconv_t) -1 == g_icWC) {
        perror("iconv_open UTF-32 UTF-16");
        return 0;
    }
#endif
	return 1;
}

void Max_delete_iconv(void)
{
	iconv_close (g_icWC);
	iconv_close (g_icCW);
#ifndef _WIN32
	iconv_close (g_icW16W32);
	iconv_close (g_icW32W16);
#endif
}

char* Max_strcpy_utf8(char* dst, size_t nCount, const wchar_t* src)
{
	UnicodeToUtf8(src, dst, nCount);
	return dst;
}

wchar_t* Max_strcpy_utf8(wchar_t* dst, size_t nCount, const char* src)
{
	Utf8ToUnicode(src, dst, nCount);
	return dst;
}

char* Max_strcpy_utf8(char* dst, size_t nCount, const char* src, int nDirect)
{
	wchar_t* aux = new wchar_t[nCount];
	switch(nDirect)
	{
	case 0:		// Utf8 to char
		Utf8ToUnicode(src, aux, nCount);
		UnicodeToAnsi(aux, dst);
		break;
	default:
		AnsiToUnicode(src, aux);
		UnicodeToUtf8(aux, dst, nCount);
		break;
	}
	delete aux;
	return dst;
}
