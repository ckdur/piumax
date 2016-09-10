#include "stdafx.h"
#include "libc_max_common.h"

#define CStringArray vector<CStdStringA>

#ifndef _WIN32
FILE* _wfopen(LPCWSTR strFile, LPCWSTR strMode)
{
    char chFile[260];
    char chMode[260];
    UnicodeToAnsi(strFile, chFile);
    UnicodeToAnsi(strMode, chMode);
    return fopen(chFile, chMode);
}

void _tsplitpath(LPCTSTR lpszPathName, LPTSTR strDrive, LPTSTR strDir, LPTSTR strFile, LPTSTR strExt)
{
#ifdef UNICODE
    char chPath[MAX_PATH];
    Max_strcpy(chPath, lpszPathName);
    CStdStringA strPath = chPath;
    CStdStringA sDir, sFile, sExt;

    splitpath( lpszPathName, sDir, sFile, sExt );

    Max_strcpy(strDrive, _T(""));
    Max_strcpy(strDir, sDir);
    Max_strcpy(strFile, sFile);
    Max_strcpy(strExt, sExt);
#else
    CStdStringA strPath = lpszPathName;
    CStdStringA sDir, sFile, sExt;

    splitpath( lpszPathName, sDir, sFile, sExt );

    _strcpy(strDrive, _T(""));
    _strcpy(strDir, sDir);
    _strcpy(strFile, sFile);
    _strcpy(strExt, sExt);
#endif // UNICODE
}
/*
 * foo\fum\          -> "foo\fum\", "", ""
 * c:\foo\bar.txt    -> "c:\foo\", "bar", ".txt"
 * /foo\fum         -> "" DIRDIV "foo\", "fum", ""
 */
void splitpath( const CStdStringA &Path, CStdStringA& Dir, CStdStringA& Filename, CStdStringA& Ext )
{
	Dir = Filename = Ext = "";

	CStringArray mat;

	/* One level of escapes for the regex, one for C. Ew.
	 * This is really:
	 * ^(.*[" DIRDIV "/])?(.*)$    */
	static Regex sep("^(.*[\\\\/])?(.*)$");
	bool check = sep.Compare(Path, mat);
	ASSERT(check);

	Dir = mat[0];
	const CStdStringA Base = mat[1];

	/* ^(.*)(\.[^\.]+)$ */
	static Regex SplitExt("^(.*)(\\.[^\\.]+)$");
	if( SplitExt.Compare(Base, mat) )
	{
		Filename = mat[0];
		Ext = mat[1];
	} else
		Filename = Base;
}

#if 1
#if 1
/* PCRE */
#include "pcre.h"
void Regex::Compile()
{
	const char *error;
	int offset;
	reg = pcre_compile(pattern.c_str(), PCRE_CASELESS, &error, &offset, NULL);

    if(reg == NULL)
	{
		printf("Invalid regex: '%s' (%s)", pattern.c_str(), error);
		exit(3);
	}

	int ret = pcre_fullinfo( (pcre *) reg, NULL, PCRE_INFO_CAPTURECOUNT, &backrefs);
	ASSERT(ret >= 0);

	backrefs++;
    ASSERT(backrefs < 128);
}

void Regex::Set(const CStdStringA &str)
{
	Release();
    pattern=str;
	Compile();
}

void Regex::Release()
{
    pcre_free(reg);
	reg = NULL;
	pattern = "";
}

Regex::Regex(const CStdStringA &str)
{
	reg = NULL;
	Set(str);
}

Regex::Regex(const Regex &rhs)
{
	reg = NULL;
    Set(rhs.pattern);
}

Regex &Regex::operator=(const Regex &rhs)
{
	if(this != &rhs) Set(rhs.pattern);
	return *this;
}

Regex::~Regex()
{
    Release();
}

bool Regex::Compare(const CStdStringA &str)
{
    int mat[128*3];
	int ret = pcre_exec( (pcre *) reg, NULL,
		str.data(), str.size(), 0, 0, mat, 128*3);

	if( ret < -1 )
	{
		printf("Unexpected return from pcre_exec('%s'): %i",
			pattern.c_str(), ret);
		exit(3);
	}

	return ret >= 0;
}

bool Regex::Compare(const CStdStringA &str, vector<CStdStringA> &matches)
{
    matches.clear();

    int mat[128*3];
	int ret = pcre_exec( (pcre *) reg, NULL,
		str.data(), str.size(), 0, 0, mat, 128*3);

	if( ret < -1 )
	{
		printf("Unexpected return from pcre_exec('%s'): %i",
			pattern.c_str(), ret);
		exit(3);
	}

	if(ret == -1)
		return false;

    for(unsigned i = 1; i < backrefs; ++i)
    {
		const int start = mat[i*2], end = mat[i*2+1];
        if(start == -1)
            matches.push_back(""); /* no match */
        else
            matches.push_back(str.substr(start, end - start));
    }

    return true;
}
#else
/* GNU regex */
#include "regex.h"
void Regex::Compile()
{
    reg = new regex_t;

    int ret = regcomp((regex_t *) reg, pattern.c_str(), REG_EXTENDED|REG_ICASE);
    if(ret != 0)
		RageException::Throw("Invalid regex: '%ls'", pattern.c_str());

    /* Count the number of backreferences. */
    backrefs = 0;
    for(int i = 0; i < int(pattern.size()); ++i)
        if(pattern[i] == '(') backrefs++;
    ASSERT(backrefs+1 < 128);
}

void Regex::Set(const CStdStringA &str)
{
	Release();
    pattern=str;
	Compile();
}

void Regex::Release()
{
    delete (regex_t *)reg;
	reg = NULL;
	pattern = "";
}

Regex::Regex(const CStdStringA &str)
{
	reg = NULL;
	Set(str);
}

Regex::Regex(const Regex &rhs)
{
	reg = NULL;
    Set(rhs.pattern);
}

Regex &Regex::operator=(const Regex &rhs)
{
	if(this != &rhs) Set(rhs.pattern);
	return *this;
}

Regex::~Regex()
{
    Release();
}

bool Regex::Compare(const CStdStringA &str)
{
    return regexec((regex_t *) reg, str.c_str(), 0, NULL, 0) != REG_NOMATCH;
}

bool Regex::Compare(const CStdStringA &str, vector<CStdStringA> &matches)
{
    matches.clear();

    regmatch_t mat[128];
    int ret = regexec((regex_t *) reg, str.c_str(), 128, mat, 0);

	if(ret == REG_NOMATCH)
        return false;

    for(unsigned i = 1; i < backrefs+1; ++i)
    {
        if(mat[i].rm_so == -1)
            matches.push_back(""); /* no match */
        else
            matches.push_back(str.substr(mat[i].rm_so, mat[i].rm_eo - mat[i].rm_so));
    }

    return true;
}
#endif

#endif


#endif // _WIN32