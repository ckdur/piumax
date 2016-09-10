#include "cmnpiu.h"
#include "UnlockLoader.h"

CUnlockLoader::CUnlockLoader(void)
{
	m_nSize = 0;
}

CUnlockLoader::~CUnlockLoader(void)
{
	Destroy();
}

int CUnlockLoader::LoadBinary(LPCTSTR chFile)
{
	FILE* hFile = _fopen(chFile, _T("rb"));
	if(hFile == NULL) return 0;
	fread(&m_nSize, sizeof(int), 1, hFile);
	m_dReqGame1 = new double[m_nSize];
	m_dReqGame2 = new double[m_nSize];
	m_dReqGame3 = new double[m_nSize];
	m_dReqGame4 = new double[m_nSize];
	m_dReqGame5 = new double[m_nSize];
	m_nReqExp = new int[m_nSize];
	m_nReqLvl = new int[m_nSize];
	m_nSizeMission = new int[m_nSize];
	m_uIDReqMission = new unsigned int*[m_nSize];
	m_nSizeCondUnlock = new int[m_nSize];
	m_uCondUnlock = new unsigned int*[m_nSize];
	m_dwCondUnlockModes = new DWORD*[m_nSize];
	m_nSizeCondAspUnlock = new int[m_nSize];
	m_uCondAspUnlock = new unsigned int*[m_nSize];

	fread(m_dReqGame1, m_nSize*sizeof(double), 1, hFile);
	fread(m_dReqGame2, m_nSize*sizeof(double), 1, hFile);
	fread(m_dReqGame3, m_nSize*sizeof(double), 1, hFile);
	fread(m_dReqGame4, m_nSize*sizeof(double), 1, hFile);
	fread(m_dReqGame5, m_nSize*sizeof(double), 1, hFile);
	fread(m_nReqExp, m_nSize*sizeof(int), 1, hFile);
	fread(m_nReqLvl, m_nSize*sizeof(int), 1, hFile);
	fread(m_nSizeMission, m_nSize*sizeof(int), 1, hFile);
	fread(m_nSizeCondUnlock, m_nSize*sizeof(int), 1, hFile);
	fread(m_nSizeCondAspUnlock, m_nSize*sizeof(int), 1, hFile);
	for(int x = 0; x < m_nSize; x++)
	{
		if(m_nSizeMission[x])
		{
			m_uIDReqMission[x] = new unsigned int[m_nSizeMission[x]];
			fread(m_uIDReqMission[x], m_nSizeMission[x]*sizeof(unsigned int), 1, hFile);
		}
		if(m_nSizeCondUnlock[x])
		{
			m_uCondUnlock[x] = new unsigned int[m_nSizeCondUnlock[x]];
			m_dwCondUnlockModes[x] = new DWORD[m_nSizeCondUnlock[x]];
			fread(m_uCondUnlock[x], m_nSizeCondUnlock[x]*sizeof(unsigned int), 1, hFile);
			fread(m_dwCondUnlockModes[x], m_nSizeCondUnlock[x]*sizeof(DWORD), 1, hFile);
		}
		if(m_nSizeCondAspUnlock[x])
		{
			m_uCondAspUnlock[x] = new unsigned int[m_nSizeCondAspUnlock[x]];
			fread(m_uCondAspUnlock[x], m_nSizeCondAspUnlock[x]*sizeof(unsigned int), 1, hFile);
		}
	}
	return 1;
}

int CUnlockLoader::SaveBinary(LPCTSTR chName)
{
	FILE* hFile = _fopen(chName, _T("w+b"));
	if(hFile == NULL) return 0;
	fwrite(&m_nSize, sizeof(int), 1, hFile);
	fwrite(m_dReqGame1, m_nSize*sizeof(double), 1, hFile);
	fwrite(m_dReqGame2, m_nSize*sizeof(double), 1, hFile);
	fwrite(m_dReqGame3, m_nSize*sizeof(double), 1, hFile);
	fwrite(m_dReqGame4, m_nSize*sizeof(double), 1, hFile);
	fwrite(m_dReqGame5, m_nSize*sizeof(double), 1, hFile);
	fwrite(m_nReqExp, m_nSize*sizeof(int), 1, hFile);
	fwrite(m_nReqLvl, m_nSize*sizeof(int), 1, hFile);
	fwrite(m_nSizeMission, m_nSize*sizeof(int), 1, hFile);
	fwrite(m_nSizeCondUnlock, m_nSize*sizeof(int), 1, hFile);
	fwrite(m_nSizeCondAspUnlock, m_nSize*sizeof(int), 1, hFile);
	for(int x = 0; x < m_nSize; x++)
	{
		if(m_nSizeMission[x])
		{
			fwrite(m_uIDReqMission[x], m_nSizeMission[x]*sizeof(unsigned int), 1, hFile);
		}
		if(m_nSizeCondUnlock[x])
		{
			fwrite(m_uCondUnlock[x], m_nSizeCondUnlock[x]*sizeof(unsigned int), 1, hFile);
			fwrite(m_dwCondUnlockModes[x], m_nSizeCondUnlock[x]*sizeof(DWORD), 1, hFile);
		}
		if(m_nSizeCondAspUnlock[x])
		{
			fwrite(m_uCondAspUnlock[x], m_nSizeCondAspUnlock[x]*sizeof(unsigned int), 1, hFile);
		}
	}
	fclose(hFile);
	return 1;
}

int CUnlockLoader::LoadText(LPCTSTR  chName)
{
	FILE* hFile = _fopen(chName, _T("r"));
	if(hFile == NULL) return 0;
	CString strPar;
	CString strArg;
	TCHAR ch[260];
	int nCur = -1;
	int nCurCondUnlock = 0;
	while(_fgets(ch, 260, hFile) != NULL)
	{
		strPar = ( ch);
		strPar.Remove(_T('\n'));
		int nCode = 0;
		if((nCode = strPar.Find(_T("//"))) != -1)
		{
			strPar = strPar.Left(nCode);
		}
		if((nCode = strPar.Find(_T("#SIZEUNLOCK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSize) != 1) {fclose(hFile); return 0;}

			m_dReqGame1 = new double[m_nSize];
			m_dReqGame2 = new double[m_nSize];
			m_dReqGame3 = new double[m_nSize];
			m_dReqGame4 = new double[m_nSize];
			m_dReqGame5 = new double[m_nSize];
			m_nReqExp = new int[m_nSize];
			m_nReqLvl = new int[m_nSize];
			m_nSizeMission = new int[m_nSize];
			m_uIDReqMission = new unsigned int*[m_nSize];
			m_nSizeCondUnlock = new int[m_nSize];
			m_uCondUnlock = new unsigned int*[m_nSize];
			m_dwCondUnlockModes = new DWORD*[m_nSize];
			m_nSizeCondAspUnlock = new int[m_nSize];
			m_uCondAspUnlock = new unsigned int*[m_nSize];
			for(int x = 0; x < m_nSize; x++)
			{
				m_dReqGame1[x] = 0;
				m_dReqGame2[x] = 0;
				m_dReqGame3[x] = 0;
				m_dReqGame4[x] = 0;
				m_dReqGame5[x] = 0;
				m_nReqExp[x] = 0;
				m_nReqLvl[x] = 0;
				m_nSizeMission[x] = 0;
				m_nSizeCondUnlock[x] = 0;
				m_nSizeCondAspUnlock[x] = 0;
			}
		}
		if((nCode = strPar.Find(_T("#EUNLOCK"))) != -1)
		{
			nCur++;
		}
		if((nCode = strPar.Find(_T("#REQGAME1"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%lg"), &m_dReqGame1[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#REQGAME2"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%lg"), &m_dReqGame2[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#REQGAME3"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%lg"), &m_dReqGame3[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#REQGAME4"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%lg"), &m_dReqGame4[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#REQGAME5"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%lg"), &m_dReqGame5[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#REQEXP"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nReqExp[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#REQLVL"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nReqLvl[nCur]) != 1) {fclose(hFile); return 0;}
		}

		if((nCode = strPar.Find(_T("#SIZEMISSION"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeMission[nCur]) != 1) {fclose(hFile); return 0;}
			if(m_nSizeMission[nCur])
			{
				m_uIDReqMission[nCur] = new unsigned int[m_nSizeMission[nCur]];
				for(int x = 0; x < m_nSizeMission[nCur]; x++)
				{
					m_uIDReqMission[nCur][x] = 0;
				}
			}
		}
		if((nCode = strPar.Find(_T("#MISSION"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			int a = 0, b = 0;
			for(int x = 0; x < m_nSizeMission[nCur]; x++)
			{
				if((b = strArg.Find(_T(','), a)) == -1) b = strArg.GetLength();
				if(_sscanf(strArg.Mid(a, b), _T("%u"), &m_uIDReqMission[nCur][x]) != 1) {fclose(hFile); return 0;}
				a = b+1;
			}
		}

		if((nCode = strPar.Find(_T("#SIZESONGUNLOCK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeCondUnlock[nCur]) != 1) {fclose(hFile); return 0;}
			if(m_nSizeCondUnlock[nCur])
			{
				m_uCondUnlock[nCur] = new unsigned int[m_nSizeCondUnlock[nCur]];
				m_dwCondUnlockModes[nCur] = new DWORD[m_nSizeCondUnlock[nCur]];
				for(int x = 0; x < m_nSizeCondUnlock[nCur]; x++)
				{
					m_uCondUnlock[nCur][x] = 0;
					m_dwCondUnlockModes[nCur][x] = 0;
				}
			}
			nCurCondUnlock = -1;
		}
		if((nCode = strPar.Find(_T("#SONGUNLOCK"))) != -1)
		{
			nCurCondUnlock++;
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			int n[7];
			if(_sscanf(strArg, _T("%d\t%d\t%d\t%d\t%d\t%d\t%d"), &n[0], &n[1], &n[2], &n[3], &n[4], &n[5], &n[6]) != 7) {fclose(hFile); return 0;}
			m_uCondUnlock[nCur][nCurCondUnlock] = (unsigned int)max(0, n[0]);
			m_dwCondUnlockModes[nCur][nCurCondUnlock] = (n[1]&0xF)|((n[2]&0xF)<<4)|((n[3]&0xF)<<8)|((n[4]&0xF)<<12)|((n[5]&0xF)<<16)|((n[6]&0xF)<<20);
		}

		if((nCode = strPar.Find(_T("#SIZEASPUNLOCK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeCondAspUnlock[nCur]) != 1) {fclose(hFile); return 0;}
			if(m_nSizeCondAspUnlock[nCur])
			{
				m_uCondAspUnlock[nCur] = new unsigned int[m_nSizeCondAspUnlock[nCur]];
				for(int x = 0; x < m_nSizeCondAspUnlock[nCur]; x++)
				{
					m_uCondAspUnlock[nCur][x] = 0;
				}
			}
		}
		if((nCode = strPar.Find(_T("#ASPUNLOCK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			int a = 0, b = 0;
			for(int x = 0; x < m_nSizeCondAspUnlock[nCur]; x++)
			{
				if((b = strArg.Find(_T(','), a)) == -1) b = strArg.GetLength();
				if(_sscanf(strArg.Mid(a, b), _T("%d"), &m_uCondAspUnlock[nCur][x]) != 1) {fclose(hFile); return 0;}
				a = b+1;
			}
		}
	}
	fclose(hFile);
	return 1;
}

void CUnlockLoader::Destroy(void)
{
	if(m_nSize != 0)
	{
		delete m_dReqGame1;
		delete m_dReqGame2;
		delete m_dReqGame3;
		delete m_dReqGame4;
		delete m_dReqGame5;
		delete m_nReqExp;
		delete m_nReqLvl;
		for (int x = 0; x < m_nSize; x++)
		{
			if (m_nSizeMission[x]) delete m_uIDReqMission[x];
			if (m_nSizeCondUnlock[x])
			{
				delete m_uCondUnlock[x];
				delete m_dwCondUnlockModes[x];
			}
			if (m_nSizeCondAspUnlock[x]) delete m_uCondAspUnlock[x];
		}
		delete m_nSizeMission;
		delete[] m_uIDReqMission;
		delete m_nSizeCondUnlock;	// Size Unlocks
		delete[] m_uCondUnlock;	// ID Unlocks
		delete[] m_dwCondUnlockModes;	// ID Unlocks Modes [Binary]
		delete m_nSizeCondAspUnlock;	// Size Aspect Unlocks
		delete[] m_uCondAspUnlock;	// ID Aspect Unlocks
	}
}
