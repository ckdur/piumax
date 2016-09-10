#include "stdafx.h"
#include "OperStr.h"

COperStr::COperStr(void)
{
	m_nSizeVar = 0;
}

COperStr::~COperStr(void)
{
	DeleteVar();
}

void CheckSigns(CString& strArg)
{
	while(strArg.Find(_T("++")) != -1) strArg.Replace(_T("++"), _T("+"));
	while(strArg.Find(_T("--")) != -1) strArg.Replace(_T("--"), _T("+"));
	while(strArg.Find(_T("+-")) != -1) strArg.Replace(_T("+-"), _T("-"));
	while(strArg.Find(_T("-+")) != -1) strArg.Replace(_T("-+"), _T("-"));
	while(strArg.Find(_T("*+")) != -1) strArg.Replace(_T("*+"), _T("*"));
	while(strArg.Find(_T("/+")) != -1) strArg.Replace(_T("/+"), _T("/"));
	while(strArg.Find(_T("==")) != -1) strArg.Replace(_T("=="), _T("="));
	while(strArg.Find(_T("&&")) != -1) strArg.Replace(_T("&&"), _T("&"));
	while(strArg.Find(_T("||")) != -1) strArg.Replace(_T("||"), _T("|"));
	if(strArg.GetAt(0) == _T('+')) strArg.Delete(0);
}

float COperStr::Solve(CString strArg)
{
	if(!ConvertString(strArg)) return 0.0;
	if(strArg.Find(_T("+*")) != -1) return 0.0;
	if(strArg.Find(_T("+/")) != -1) return 0.0;
	if(strArg.Find(_T("-*")) != -1) return 0.0;
	if(strArg.Find(_T("-/")) != -1) return 0.0;
	if(strArg.Find(_T("**")) != -1) return 0.0;
	if(strArg.Find(_T("*/")) != -1) return 0.0;
	if(strArg.Find(_T("/*")) != -1) return 0.0;
	if(strArg.Find(_T("//")) != -1) return 0.0;
	// Comenzar con los (
	int nCode = -1;
	int nBackCode = 0;
	CheckSigns(strArg);
	while((nCode = strArg.Find(_T('('), nBackCode)) != -1)
	{
		// Encontrar el respectivo )
		int nFound = -1;
		int nCaret = 1;
		for(int x = nCode+1; x < strArg.GetLength(); x++)
		{
			TCHAR ch = strArg.GetAt(x);
			if(ch == '(') nCaret++;
			if(ch == ')') nCaret--;
			if(nCaret == 0)
			{
				nFound = x;
				break;
			}
		}
		if(nFound == -1) return 0.0;	// Mala sintaxis por no encontrar )
		nBackCode = nFound;
		CString subStr = strArg.Mid(nCode+1, nBackCode-nCode-1);
		if(subStr.IsEmpty()) return 0.0;	// Mala sintaxis por no haber operacion entre los ()
		float nResult = Solve(subStr);
		CString strRes;
		strRes.Format(_T("%g"), nResult);
		strArg.Delete(nCode, nBackCode-nCode+1);
		strArg.Insert(nCode, strRes);
		CheckSigns(strArg);
		nBackCode = nCode;
	}
	// Buscar los *, los / y los % (2da prioridad)
	nCode = -1;
	nBackCode = 0;
	do
	{
		int nOper = -1;	// -1 no hay oper, 0 mult, 1 div, 2 res
		nCode = strArg.Find(_T('*'), nBackCode);
		if(nCode != -1) nOper = 0;
		else
		{
			nCode = strArg.Find(_T('/'), nBackCode);
			if(nCode != -1) nOper = 1;
			else
			{
				nCode = strArg.Find(_T('%'), nBackCode);
				if(nCode != -1) nOper = 2;
			}
		}
		if(nCode != -1)
		{
			int nBegin = 0, nEnd = 0;
			// Encontrar hacia atras la operacion anterior
			for(int x = nCode-1; x >= 0; x--)
			{
				TCHAR ch = strArg.GetAt(x);
				nBegin = x+1;
				if(ch == _T('+') || ch == _T('-')
					|| ch == _T('*') || ch == _T('/') || ch == _T('%')
					|| ch == _T('<') || ch == _T('=') || ch == _T('>')
					|| ch == _T('&') || ch == _T('|') || ch == _T('!') || ch == _T('^'))
				{
					break;
				}
				nBegin = x;
			}
			// Encontrar hacia adelante la siguiente operacion
			for(int x = nCode+1; x < strArg.GetLength(); x++)
			{
				TCHAR ch = strArg.GetAt(x);
				nEnd = x;
				if((x != (nCode+1)) && (ch == _T('+') || ch == _T('-')
					|| ch == _T('*') || ch == _T('/') || ch == _T('%')
					|| ch == _T('<') || ch == _T('=') || ch == _T('>')
					|| ch == _T('&') || ch == _T('|') || ch == _T('!') || ch == _T('^')))
				{
					break;
				}
				nEnd = x+1;
			}
			CString strN1 = strArg.Mid(nBegin, nCode-nBegin);
			CString strN2 = strArg.Mid(nCode+1, nEnd-nCode-1);
			float n1 = 0, n2 = 0;
			if(_sscanf(strN1, _T("%g"), &n1) != 1) return 0.0;	// Fallo en lectura?
			if(_sscanf(strN2, _T("%g"), &n2) != 1) return 0.0;	// Fallo en lectura?
			float nRes = 0;
			switch(nOper)
			{
			case 0:
				nRes = n1*n2;
				break;
			case 1:
				nRes = n1/n2;
				break;
			case 2:
				nRes = float(int(n1)%int(n2));
				break;
			}
			CString strRes;
			strRes.Format(_T("%g"), nRes);
			strArg.Delete(nBegin, nEnd-nBegin);
			strArg.Insert(nBegin, strRes);
			CheckSigns(strArg);
			nBackCode = nBegin;
		}
	} while(nCode != -1);
	// Buscar los + y - (3da prioridad)
	nCode = -1;
	nBackCode = 1;	// Se busca 1 posicion desues del comienzo (por si hay - operario)
	do
	{
		int nOper = -1;	// -1 no hay oper, 0 sum, 1 res
		nCode = strArg.Find(_T('+'), nBackCode);
		if(nCode != -1) nOper = 0;
		else
		{
			nCode = strArg.Find(_T('-'), nBackCode);
			if(nCode != -1) nOper = 1;
		}
		if(nCode != -1)
		{
			int nBegin = 0, nEnd = 0;
			// Encontrar hacia atras la operacion anterior
			for(int x = nCode-1; x >= 0; x--)
			{
				TCHAR ch = strArg.GetAt(x);
				nBegin = x+1;
				if(ch == _T('+') || ch == _T('-')
					|| ch == _T('<') || ch == _T('=') || ch == _T('>')
					|| ch == _T('&') || ch == _T('|') || ch == _T('!') || ch == _T('^'))
				{
					if(ch == _T('-')) nBegin = x;
					break;
				}
				nBegin = x;
			}
			// Encontrar hacia adelante la siguiente operacion
			for(int x = nCode+1; x < strArg.GetLength(); x++)
			{
				TCHAR ch = strArg.GetAt(x);
				nEnd = x;
				if((x != (nCode+1)) && (ch == _T('+') || ch == _T('-')
					|| ch == _T('<') || ch == _T('=') || ch == _T('>')
					|| ch == _T('&') || ch == _T('|') || ch == _T('!') || ch == _T('^')))
				{
					break;
				}
				nEnd = x+1;
			}
			CString strN1 = strArg.Mid(nBegin, nCode-nBegin);
			CString strN2 = strArg.Mid(nCode+1, nEnd-nCode-1);
			float n1 = 0, n2 = 0;
			if(_sscanf(strN1, _T("%g"), &n1) != 1) return 0.0;	// Fallo en lectura?
			if(_sscanf(strN2, _T("%g"), &n2) != 1) return 0.0;	// Fallo en lectura?
			float nRes = 0;
			switch(nOper)
			{
			case 0:
				nRes = n1+n2;
				break;
			case 1:
				nRes = n1-n2;
				break;
			}
			CString strRes;
			strRes.Format(_T("%g"), nRes);
			strArg.Delete(nBegin, nEnd-nBegin);
			strArg.Insert(nBegin, strRes);
			CheckSigns(strArg);
			nBackCode = max(1, nBegin);
		}
	} while(nCode != -1);
	// Buscar los comparadores (<, > o =) (3da prioridad)
	nCode = -1;
	nBackCode = 0;
	do
	{
		int nOper = -1;	// -1 no hay oper
		nCode = strArg.Find(_T('<'), nBackCode);
		if(nCode != -1)
		{
			if((nCode+1) < strArg.GetLength()) {if(strArg.GetAt(nCode+1) == _T('=')) nOper = 3;
			else nOper = 0;}
		}
		else
		{
			nCode = strArg.Find(_T('>'), nBackCode);
			if(nCode != -1)
			{
				if((nCode+1) < strArg.GetLength()) {if(strArg.GetAt(nCode+1) == _T('=')) nOper = 4;
				else nOper = 2;}
			}
			else
			{
				nCode = strArg.Find(_T("!="), nBackCode);
				if(nCode != -1)
					nOper = 5;
				else
				{
					nCode = strArg.Find(_T('='), nBackCode);
					if(nCode != -1) nOper = 1;
				}
			}
		}
		if(nCode != -1)
		{
			int nBegin = 0, nEnd = 0;
			// Encontrar hacia atras la operacion anterior
			for(int x = nCode-1; x >= 0; x--)
			{
				TCHAR ch = strArg.GetAt(x);
				nBegin = x+1;
				if(ch == _T('<') || ch == _T('=') || ch == _T('>')
					|| ch == _T('&') || ch == _T('|') || ch == _T('!') || ch == _T('^'))
				{
					break;
				}
				nBegin = x;
			}
			// Encontrar hacia adelante la siguiente operacion
			for(int x = nCode+1; x < strArg.GetLength(); x++)
			{
				TCHAR ch = strArg.GetAt(x);
				nEnd = x;
				if((x != (nCode+1)) && (ch == _T('<') || ch == _T('=') || ch == _T('>')
					|| ch == _T('&') || ch == _T('|') || ch == _T('!') || ch == _T('^')))
				{
					break;
				}
				nEnd = x+1;
			}
			CString strN1 = strArg.Mid(nBegin, nCode-nBegin);
			CString strN2 = strArg.Mid(nOper>2?(nCode+2):(nCode+1), nEnd-nCode-(nOper>2?2:1));
			float n1 = 0, n2 = 0;
			if(_sscanf(strN1, _T("%g"), &n1) != 1) return 0.0;	// Fallo en lectura?
			if(_sscanf(strN2, _T("%g"), &n2) != 1) return 0.0;	// Fallo en lectura?
			int nRes = 0;
			switch(nOper)
			{
			case 0:
				nRes = n1<n2;
				break;
			case 1:
				nRes = n1==n2;
				break;
			case 2:
				nRes = n1>n2;
				break;
			case 3:
				nRes = n1<=n2;
				break;
			case 4:
				nRes = n1>=n2;
				break;
			case 5:
				nRes = n1!=n2;
				break;
			}
			CString strRes;
			strRes.Format(_T("%d"), nRes);
			strArg.Delete(nBegin, nEnd-nBegin);
			strArg.Insert(nBegin, strRes);
			CheckSigns(strArg);
			nBackCode = nBegin;
		}
	} while(nCode != -1);
	// Encontrar operadores logicos (&, |, !) (5ta prioridad)
	nCode = -1;
	nBackCode = 0;
	do
	{
		int nOper = -1;	// -1 no hay oper, 0 and, 1 or, 2 xor, 3 not
		nCode = strArg.Find(_T('&'), nBackCode);
		if(nCode != -1) nOper = 0;
		else
		{
			nCode = strArg.Find(_T('|'), nBackCode);
			if(nCode != -1) nOper = 1;
			else
			{
				nCode = strArg.Find(_T('^'), nBackCode);
				if(nCode != -1) nOper = 2;
				else
				{
					nCode = strArg.Find(_T('!'), nBackCode);
					if(nCode != -1) nOper = 3;
				}
			}
		}
		if(nCode != -1)
		{
			int nBegin = 0, nEnd = 0;
			// Encontrar hacia atras la operacion anterior (Excepto para !)
			if(nOper != 3) {for(int x = nCode-1; x >= 0; x--)
			{
				TCHAR ch = strArg.GetAt(x);
				nBegin = x+1;
				if(ch == _T('&') || ch == _T('|') || ch == _T('!') || ch == _T('^'))
				{
					break;
				}
				nBegin = x;
			}}
			else nBegin = nCode;
			// Encontrar hacia adelante la siguiente operacion
			for(int x = nCode+1; x < strArg.GetLength(); x++)
			{
				TCHAR ch = strArg.GetAt(x);
				nEnd = x;
				if(ch == _T('&') || ch == _T('|') || ch == _T('!') || ch == _T('^'))
				{
					break;
				}
				nEnd = x+1;
			}
			CString strN1 = strArg.Mid(nBegin, nCode-nBegin);
			CString strN2 = strArg.Mid((nCode+1), nEnd-nCode-1);
			int n1 = 0, n2 = 0;
			if(_sscanf(strN1, _T("%d"), &n1) != 1 && nOper != 3) 
				return 0.0;	// Fallo en lectura?
			if(_sscanf(strN2, _T("%d"), &n2) != 1) 
				return 0.0;	// Fallo en lectura?
			int nRes = 0;
			switch(nOper)
			{
			case 0:
				nRes = n1&&n2;
				break;
			case 1:
				nRes = n1||n2;
				break;
			case 2:
				nRes = (n1&&(!n2))||((!n1)&&n2);
				break;
			case 3:
				nRes = !n2;
				break;
			}
			CString strRes;
			strRes.Format(_T("%d"), nRes);
			strArg.Delete(nBegin, nEnd-nBegin);
			strArg.Insert(nBegin, strRes);
			CheckSigns(strArg);
			nBackCode = nBegin;
		}
	} while(nCode != -1);
	float nResult = 0;
	if(_sscanf(strArg, _T("%g"), &nResult) != 1) return 0.0;	// Fallo en lectura?
	return nResult;
}

void COperStr::AddVar(CString strName, CString strVal)
{
	ReallocVar(m_nSizeVar+1);
	strVarName[m_nSizeVar-1] = strName;
	strVarValue[m_nSizeVar-1] = strVal;
}

void COperStr::ReallocVar(int nNewSize)
{
	if(nNewSize < 0) return;
	if(nNewSize == 0)
	{
		DeleteVar();
		return;
	}
	int nSize = m_nSizeVar;
	CString* strBackVarName;
	CString* strBackVarValue;
	if(nSize > 0)
	{
		strBackVarName = new CString[nSize];
		strBackVarValue = new CString[nSize];
	}
	for(int x = 0; x < nSize; x++)
	{
		strBackVarName[x] = strVarName[x];
		strBackVarValue[x] = strVarValue[x];
	}
	DeleteVar();
	strVarName = new CString[nNewSize];
	strVarValue = new CString[nNewSize];
	for(int x = 0; x < nNewSize; x++)
	{
		/*strVarName[x] = new CString();
		strVarValue[x] = new CString();*/
		if(x < nSize)
		{
			strVarName[x] = strBackVarName[x];
			strVarValue[x] = strBackVarValue[x];
		}
	}
	if(nSize > 0)
	{
		delete[] strBackVarName;
		delete[] strBackVarValue;
	}
	m_nSizeVar=nNewSize;
}

void COperStr::DeleteVar(void)
{
	if(m_nSizeVar > 0)
	{
		delete[] strVarName;
		delete[] strVarValue;
		m_nSizeVar = 0;
	}
}

int COperStr::ConvertString(CString& strArg)
{
	strArg.Remove(_T(' '));
	strArg.Remove(_T('\n'));
	strArg.Remove(_T('\t'));
	for(int x = 0; x < m_nSizeVar; x++)
	{
		CString strName = strVarName[x];
		CString strVal = strVarValue[x];
		CString strIns = _T("");
		int nInf = 0, nSup = 0;
		while(nInf < strArg.GetLength())
		{
			for(nSup = nInf; nSup < strArg.GetLength(); nSup++)
			{
				TCHAR ch = strArg.GetAt(nSup);
				if((ch >= _T('<') && ch <= _T('>'))
				|| (ch >= _T('(') && ch <= _T('+')) || ch == _T('-') || ch == _T('/')
				|| ch == _T('&') || ch == _T('|') || ch == _T('%') || ch == _T('.') || ch == _T('!') || ch == _T('^'))
				{
					break;
				}
			}
			if(nInf < nSup)
			{
				strIns = strArg.Mid(nInf, nSup-nInf);
				if(strIns==strName)
				{
					strArg.Delete(nInf, nSup-nInf);
					strArg.Insert(nInf, strVal);
					nInf += strVal.GetLength()+1;
				} 
				else nInf = nSup+1;
			}
			else nInf = nSup+1;
		}
	}
	bool bAccept = true;
	for(int x = 0; x < strArg.GetLength(); x++)
	{
		TCHAR ch = strArg.GetAt(x);
		if(ch < _T('0') && ch > _T('9') && ch < _T('<') && ch > _T('>')
			&& ch < _T('(') && ch > _T('+') && ch != _T('-') && ch != _T('/')
			&& ch != _T('&') && ch != _T('|') && ch != _T('%') && ch != _T('.') && ch != _T('!') && ch != _T('^'))
		{
			bAccept = false;
		}
	}
	return bAccept?1:0;
}
