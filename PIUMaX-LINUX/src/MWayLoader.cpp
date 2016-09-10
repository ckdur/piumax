#include "cmnpiu.h"
#include "MWayLoader.h"

CMWayLoader::CMWayLoader(void)
{
}

CMWayLoader::~CMWayLoader(void)
{
	delete m_uLink;
	delete m_bDraw;
	for (int x = 0; x < m_nSizeWay; x++)
	{
		if (m_uSizeMUnlock[x]) delete m_uMUnlock[x];
		if (m_uSizeMSucess[x]) delete m_uMSucess[x];
	}
	delete m_uSizeMUnlock;
	delete[] m_uMUnlock;
	delete m_uSizeMSucess;
	delete[] m_uMSucess;
}

int CMWayLoader::LoadBinary(LPCTSTR strFile)
{
	FILE* hFile = _fopen(strFile, _T("rb"));
	if(hFile == NULL) return 0;
	fread(&m_nSizeWay, sizeof(int), 1, hFile);
	m_uLink = new unsigned int[m_nSizeWay*2];
	m_bDraw = new BYTE[m_nSizeWay];
	m_uSizeMUnlock = new unsigned int[m_nSizeWay];
	m_uMUnlock = new unsigned int*[m_nSizeWay];
	m_uSizeMSucess = new unsigned int[m_nSizeWay];
	m_uMSucess = new unsigned int*[m_nSizeWay];
	fread(m_uLink, m_nSizeWay*2*sizeof(unsigned int), 1, hFile);
	fread(m_bDraw, m_nSizeWay*sizeof(BYTE), 1, hFile);
	fread(m_uSizeMUnlock, m_nSizeWay*sizeof(unsigned int), 1, hFile);
	fread(m_uSizeMSucess, m_nSizeWay*sizeof(unsigned int), 1, hFile);
	for(int x = 0; x < m_nSizeWay; x++)
	{
		if(m_uSizeMUnlock[x])
		{
			m_uMUnlock[x] = new unsigned int[m_uSizeMUnlock[x]];
			fread(m_uMUnlock[x], m_uSizeMUnlock[x]*sizeof(unsigned int), 1, hFile);
		}
		if(m_uSizeMSucess[x])
		{
			m_uMSucess[x] = new unsigned int[m_uSizeMSucess[x]];
			fread(m_uMSucess[x], m_uSizeMSucess[x]*sizeof(unsigned int), 1, hFile);
		}
	}
	fclose(hFile);
	return 1;
}

int CMWayLoader::LoadText(LPCTSTR strFile)
{
	FILE* hFile = _fopen(strFile, _T("r"));
	if(hFile == NULL) return 0;
	CString strPar;
	CString strArg;
	TCHAR ch[260];
	int nCur = -1;
	while(_fgets(ch, 260, hFile) != NULL)
	{
		strPar = ( ch);
		strPar.Remove(_T('\n'));
		int nCode = 0;
		if((nCode = strPar.Find(_T("//"))) != -1)
		{
			strPar = strPar.Left(nCode);
		}
		if((nCode = strPar.Find(_T("#SIZEWAY"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeWay) != 1) {fclose(hFile); return 0;}
			m_uLink = new unsigned int[m_nSizeWay*2];
			m_bDraw = new BYTE[m_nSizeWay];
			m_uSizeMUnlock = new unsigned int[m_nSizeWay];
			m_uMUnlock = new unsigned int*[m_nSizeWay];
			m_uSizeMSucess = new unsigned int[m_nSizeWay];
			m_uMSucess = new unsigned int*[m_nSizeWay];
			for(int x = 0; x < m_nSizeWay; x++)
			{
				m_uLink[x*2] = 0;
				m_uLink[x*2+1] = 0;
				m_bDraw[x] = 0;
				m_uSizeMUnlock[x] = 0;
				m_uSizeMSucess[x] = 0;
			}
		}
		if((nCode = strPar.Find(_T("#WAY"))) != -1)
		{
			nCur++;
		}
		if((nCode = strPar.Find(_T("#LINK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u,%u"), &m_uLink[nCur*2], &m_uLink[nCur*2+1]) != 2) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#DRAW"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			unsigned int nCaret;
			if(_sscanf(strArg, _T("%u"), &nCaret) != 1) {fclose(hFile); return 0;}
			m_bDraw[nCur] = nCaret?1:0;
		}

		if((nCode = strPar.Find(_T("#SIZEMUNLOCK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uSizeMUnlock[nCur]) != 1) {fclose(hFile); return 0;}
			if(m_uSizeMUnlock[nCur])
			{
				m_uMUnlock[nCur] = new unsigned int[m_uSizeMUnlock[nCur]];
				for(unsigned int x = 0; x < m_uSizeMUnlock[nCur]; x++)
				{
					m_uMUnlock[nCur][x] = 0;
				}
			}
		}
		if((nCode = strPar.Find(_T("#MUNLOCK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			int a = 0, b = 0;
			for(unsigned int x = 0; x < m_uSizeMUnlock[nCur]; x++)
			{
				if((b = strArg.Find(_T(','), a)) == -1) b = strArg.GetLength();
				if(_sscanf(strArg.Mid(a, b), _T("%d"), &m_uMUnlock[nCur][x]) != 1) {fclose(hFile); return 0;}
				a = b+1;
			}
		}

		if((nCode = strPar.Find(_T("#SIZEMSUCESS"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uSizeMSucess[nCur]) != 1) {fclose(hFile); return 0;}
			if(m_uSizeMSucess[nCur])
			{
				m_uMSucess[nCur] = new unsigned int[m_uSizeMSucess[nCur]];
				for(unsigned int x = 0; x < m_uSizeMSucess[nCur]; x++)
				{
					m_uMSucess[nCur][x] = 0;
				}
			}
		}
		if((nCode = strPar.Find(_T("#MSUCESS"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			int a = 0, b = 0;
			for(unsigned int x = 0; x < m_uSizeMSucess[nCur]; x++)
			{
				if((b = strArg.Find(_T(','), a)) == -1) b = strArg.GetLength();
				if(_sscanf(strArg.Mid(a, b), _T("%d"), &m_uMSucess[nCur][x]) != 1) {fclose(hFile); return 0;}
				a = b+1;
			}
		}
	}
	fclose(hFile);
	return 1;
}

int CMWayLoader::SaveBinary(LPCTSTR strFile)
{
	FILE* hFile = _fopen(strFile, _T("w+b"));
	if(hFile == NULL) return 0;
	fwrite(&m_nSizeWay, sizeof(int), 1, hFile);
	fwrite(m_uLink, m_nSizeWay*2*sizeof(unsigned int), 1, hFile);
	fwrite(m_bDraw, m_nSizeWay*sizeof(BYTE), 1, hFile);
	fwrite(m_uSizeMUnlock, m_nSizeWay*sizeof(unsigned int), 1, hFile);
	fwrite(m_uSizeMSucess, m_nSizeWay*sizeof(unsigned int), 1, hFile);
	for(int x = 0; x < m_nSizeWay; x++)
	{
		if(m_uSizeMUnlock[x])
		{
			fwrite(m_uMUnlock[x], m_uSizeMUnlock[x]*sizeof(unsigned int), 1, hFile);
		}
		if(m_uSizeMSucess[x])
		{
			fwrite(m_uMSucess[x], m_uSizeMSucess[x]*sizeof(unsigned int), 1, hFile);
		}
	}
	fclose(hFile);
	return 1;
}

int CMWayLoader::FoundWay(unsigned int n1, unsigned int n2)
{
	int nIndex = -1;
	for(int x = 0; x < m_nSizeWay; x++)
	{
		if((n1 == m_uLink[x*2] && n2 == m_uLink[x*2+1]) ||
			(n2 == m_uLink[x*2] && n1 == m_uLink[x*2+1]))
		{
			nIndex = x;
			break;
		}
	}
	return nIndex;
}
