#pragma once
#include "StdString.h"

class COperStr
{
public:
	COperStr(void);
	~COperStr(void);
	int m_nSizeVar;
	CString* strVarName;
	CString* strVarValue;
	float Solve(CString strArg);
	void AddVar(CString strName, CString strVal);
private:
	void ReallocVar(int nNewSize);
public:
	void DeleteVar(void);
	int ConvertString(CString& strArg);
};
