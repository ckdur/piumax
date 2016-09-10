#pragma once
#include "stdafx.h"

class CAccReader
{
public:
	CAccReader(void);
	~CAccReader(void);
	int m_nSizeAttrib;
	vector< CString > m_strAttrib;
	vector< int > m_nDependence;
	vector< CString > m_strFormat;
	vector< bool > m_bIsMultiData;
	vector< vector< vector< vector< CString > > > > m_strData;
	int AddAttrib(LPCTSTR strAttrib, LPCTSTR strFormat, bool bIsMultiData, int nDependence = -1);
	int ResizeAttribs(int nNewSize);
	int Read(LPCTSTR strName);
	int AppendData(int nAttrib);
	int FormatIt(int nAttrib, CString strData);
	void FitData(void);
	int ResetData(void);
	int DeleteAttrib(int nIndex);
	int ReadLine(CString* strPar, FILE* hFile);
};

// Basic definitions for formats

#define ACCFORMAT_NOFORMAT _T("")
#define ACCFORMAT_NORMAL _T(" \t:,")
#define ACCFORMAT_COORD _T(" ,")
#define ACCFORMAT_VERSION _T(" .,")

// Helpers
bool tx_double(LPCTSTR str, double* d);
bool tx_float(LPCTSTR str, float* d);
bool tx_int(LPCTSTR str, int* d);
bool tx_int_cat(LPCTSTR str, int* d, TCHAR** cat, size_t scat);
bool tx_uint_cat(LPCTSTR str, unsigned int* d, TCHAR** cat, size_t scat);
bool tx_uint(LPCTSTR str, unsigned int* d);
bool tx_int64(LPCTSTR str, __int64* d);
bool tx_bool(LPCTSTR str, bool* d);