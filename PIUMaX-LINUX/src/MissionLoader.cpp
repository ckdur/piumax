#include "cmnpiu.h"
#include "MissionLoader.h"

CMissionLoader::CMissionLoader(void)
{
	m_nSize = 0;
}

CMissionLoader::~CMissionLoader(void)
{
	Destroy();
}

int CMissionLoader::LoadBinary(LPCTSTR chFile)
{
	FILE* hFile = _fopen(chFile, _T("rb"));
	if(hFile == NULL) return 0;
	char16_t* ch;
	fread(&m_nSize, sizeof(int), 1, hFile);
	m_uID = new unsigned int[m_nSize];
	m_uStage = new unsigned int[m_nSize];
	m_uPath = new unsigned int[m_nSize*4];
	m_uLvl = new unsigned int[m_nSize];
	m_uPlayer = new unsigned int[m_nSize];
	m_uMileage = new unsigned int[m_nSize];
	m_uIcon = new unsigned int[m_nSize];
	m_uIconSucess = new unsigned int[m_nSize];
	m_uMap = new unsigned int[m_nSize];
	m_uGo = new unsigned int[m_nSize];
	m_strName = new CString[m_nSize];
	m_nSizeSong = new int[m_nSize];
	m_strSong = new CString*[m_nSize];
	m_nSizeStep = new int[m_nSize];
	m_strStep = new CString*[m_nSize];
	m_strHis = new CString[m_nSize];
	m_strShowCond = new CString[m_nSize];
	m_strComment = new CString[m_nSize];
	m_strGo = new CString[m_nSize];
	m_strLand = new CString[m_nSize];
	m_bBreak = new BYTE[m_nSize];
	m_bLink = new BYTE[m_nSize];
	m_bActUMod = new BYTE[m_nSize];
	m_dUbiX = new double[m_nSize];
	m_dUbiY = new double[m_nSize];
	m_dInitialBar = new double[m_nSize];
	m_dIncrBar = new double[m_nSize];
	m_dDecrBar = new double[m_nSize];
	m_bDouble = new BYTE[m_nSize];
	m_uVersus = new unsigned int[m_nSize];
	m_uLvlVersus = new unsigned int[m_nSize];

	m_nSizeMod = new int[m_nSize];
	m_nMod = new int*[m_nSize];

	m_nSizeCond = new int[m_nSize];
	m_strCond = new CString*[m_nSize];
	m_strHPass = new CString*[m_nSize];
	m_strHNoPass = new CString*[m_nSize];
	m_bShowSucess = new BYTE*[m_nSize];

	m_nSizeCondOpen = new int*[m_nSize];
	m_uCondOpen = new unsigned int**[m_nSize];

	m_nSizeCondUnlock = new int*[m_nSize];
	m_uCondUnlock = new unsigned int**[m_nSize];
	m_dwCondUnlockModes = new DWORD**[m_nSize];

	m_nSizeCondAspUnlock = new int*[m_nSize];
	m_uCondAspUnlock = new unsigned int**[m_nSize];

	fread(m_uID, m_nSize*sizeof(unsigned int), 1, hFile);
	fread(m_uStage, m_nSize*sizeof(unsigned int), 1, hFile);
	fread(m_uPath, m_nSize*4*sizeof(unsigned int), 1, hFile);
	fread(m_uLvl, m_nSize*sizeof(unsigned int), 1, hFile);
	fread(m_uPlayer, m_nSize*sizeof(unsigned int), 1, hFile);
	fread(m_uMileage, m_nSize*sizeof(unsigned int), 1, hFile);
	fread(m_uIcon, m_nSize*sizeof(unsigned int), 1, hFile);
	fread(m_uIconSucess, m_nSize*sizeof(unsigned int), 1, hFile);
	fread(m_uMap, m_nSize*sizeof(unsigned int), 1, hFile);
	fread(m_uGo, m_nSize*sizeof(unsigned int), 1, hFile);
	for(int x = 0; x < m_nSize; x++)
	{
		int nSize = 0;
		fread(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
			ch = new char16_t[nSize+1];
			fread(ch, nSize*sizeof(char16_t), 1, hFile);
			ch[nSize] = 0;
			TCHAR* cha = new TCHAR[nSize+1];
			memset(cha, 0, (nSize+1)*sizeof(TCHAR));
			Utf16ToUtf32(ch, (wchar_t*)cha, nSize);
			m_strName[x] = cha;
			delete ch;
			delete cha;
		}
		nSize = m_strName[x].GetLength();

		fread(&m_nSizeSong[x], sizeof(int), 1, hFile);
		if(m_nSizeSong[x]) m_strSong[x] = new CString[m_nSizeSong[x]];
		for(int y = 0; y < m_nSizeSong[x]; y++)
		{
			fread(&nSize, sizeof(int), 1, hFile);
			if(nSize)
			{
                ch = new char16_t[nSize+1];
                fread(ch, nSize*sizeof(char16_t), 1, hFile);
                ch[nSize] = 0;
                TCHAR* cha = new TCHAR[nSize+1];
                Utf16ToUtf32(ch, (wchar_t*)cha, nSize+1);
                m_strSong[x][y] = cha;
                delete ch;
                delete cha;
			}
		}

		fread(&m_nSizeStep[x], sizeof(int), 1, hFile);
		if(m_nSizeStep[x]) m_strStep[x] = new CString[m_nSizeStep[x]];
		for(int y = 0; y < m_nSizeStep[x]; y++)
		{
			fread(&nSize, sizeof(int), 1, hFile);
			if(nSize)
			{
                ch = new char16_t[nSize+1];
                fread(ch, nSize*sizeof(char16_t), 1, hFile);
                ch[nSize] = 0;
                TCHAR* cha = new TCHAR[nSize+1];
                Utf16ToUtf32(ch, (wchar_t*)cha, nSize+1);
                m_strStep[x][y] = cha;
                delete ch;
                delete cha;
			}
		}

		fread(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new char16_t[nSize+1];
			fread(ch, nSize*sizeof(char16_t), 1, hFile);
			ch[nSize] = 0;
			TCHAR* cha = new TCHAR[nSize+1];
			Utf16ToUtf32(ch, (wchar_t*)cha, nSize+1);
			m_strHis[x] = cha;
			delete ch;
			delete cha;
		}

		fread(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new char16_t[nSize+1];
			fread(ch, nSize*sizeof(char16_t), 1, hFile);
			ch[nSize] = 0;
			TCHAR* cha = new TCHAR[nSize+1];
			Utf16ToUtf32(ch, (wchar_t*)cha, nSize+1);
			m_strShowCond[x] = cha;
			delete ch;
			delete cha;
		}

		fread(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new char16_t[nSize+1];
			fread(ch, nSize*sizeof(char16_t), 1, hFile);
			ch[nSize] = 0;
			TCHAR* cha = new TCHAR[nSize+1];
			Utf16ToUtf32(ch, (wchar_t*)cha, nSize+1);
			m_strComment[x] = cha;
			delete ch;
			delete cha;
		}

		fread(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new char16_t[nSize+1];
			fread(ch, nSize*sizeof(char16_t), 1, hFile);
			ch[nSize] = 0;
			TCHAR* cha = new TCHAR[nSize+1];
			Utf16ToUtf32(ch, (wchar_t*)cha, nSize+1);
			m_strGo[x] = cha;
			delete ch;
			delete cha;
		}

		fread(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new char16_t[nSize+1];
			fread(ch, nSize*sizeof(char16_t), 1, hFile);
			ch[nSize] = 0;
			TCHAR* cha = new TCHAR[nSize+1];
			Utf16ToUtf32(ch, (wchar_t*)cha, nSize+1);
			m_strLand[x] = cha;
			delete ch;
			delete cha;
		}
	}
	fread(m_bBreak, m_nSize*sizeof(BYTE), 1, hFile);
	fread(m_bLink, m_nSize*sizeof(BYTE), 1, hFile);
	fread(m_bActUMod, m_nSize*sizeof(BYTE), 1, hFile);
	fread(m_dUbiX, m_nSize*sizeof(double), 1, hFile);
	fread(m_dUbiY, m_nSize*sizeof(double), 1, hFile);
	fread(m_dInitialBar, m_nSize*sizeof(double), 1, hFile);
	fread(m_dIncrBar, m_nSize*sizeof(double), 1, hFile);
	fread(m_dDecrBar, m_nSize*sizeof(double), 1, hFile);
	fread(m_bDouble, m_nSize*sizeof(BYTE), 1, hFile);
	fread(m_uVersus, m_nSize*sizeof(unsigned int), 1, hFile);
	fread(m_uLvlVersus, m_nSize*sizeof(unsigned int), 1, hFile);

	fread(m_nSizeMod, m_nSize*sizeof(int), 1, hFile);
	for(int x = 0; x < m_nSize; x++)
	{
		m_nMod[x] = new int[m_nSizeMod[x]];
		fread(m_nMod[x], m_nSizeMod[x]*sizeof(int), 1, hFile);
	}

	fread(m_nSizeCond, m_nSize*sizeof(int), 1, hFile);
	for(int x = 0; x < m_nSize; x++)
	{
		if(m_nSizeCond[x])
		{
			m_strCond[x] = new CString[m_nSizeCond[x]];
			m_strHPass[x] = new CString[m_nSizeCond[x]];
			m_strHNoPass[x] = new CString[m_nSizeCond[x]];
			m_bShowSucess[x] = new BYTE[m_nSizeCond[x]];

			m_nSizeCondOpen[x] = new int[m_nSizeCond[x]];
			m_uCondOpen[x] = new unsigned int*[m_nSizeCond[x]];

			m_nSizeCondUnlock[x] = new int[m_nSizeCond[x]];
			m_uCondUnlock[x] = new unsigned int*[m_nSizeCond[x]];
			m_dwCondUnlockModes[x] = new DWORD*[m_nSizeCond[x]];

			m_nSizeCondAspUnlock[x] = new int[m_nSizeCond[x]];
			m_uCondAspUnlock[x] = new unsigned int*[m_nSizeCond[x]];
		}
		for(int y = 0; y < m_nSizeCond[x]; y++)
		{
			int nSize;
			fread(&nSize, sizeof(int), 1, hFile);
			if(nSize)
			{
                ch = new char16_t[nSize+1];
                fread(ch, nSize*sizeof(char16_t), 1, hFile);
                ch[nSize] = 0;
                TCHAR* cha = new TCHAR[nSize+1];
                Utf16ToUtf32(ch, (wchar_t*)cha, nSize+1);
                m_strCond[x][y] = cha;
                delete ch;
                delete cha;
			}

			fread(&nSize, sizeof(int), 1, hFile);
			if(nSize)
			{
                ch = new char16_t[nSize+1];
                fread(ch, nSize*sizeof(char16_t), 1, hFile);
                ch[nSize] = 0;
                TCHAR* cha = new TCHAR[nSize+1];
                Utf16ToUtf32(ch, (wchar_t*)cha, nSize+1);
                m_strHPass[x][y] = cha;
                delete ch;
                delete cha;
			}

			fread(&nSize, sizeof(int), 1, hFile);
			if(nSize)
			{
                ch = new char16_t[nSize+1];
                fread(ch, nSize*sizeof(char16_t), 1, hFile);
                ch[nSize] = 0;
                TCHAR* cha = new TCHAR[nSize+1];
                Utf16ToUtf32(ch, (wchar_t*)cha, nSize+1);
                m_strHNoPass[x][y] = cha;
                delete ch;
                delete cha;
			}
		}
		fread(m_bShowSucess[x], m_nSizeCond[x]*sizeof(BYTE), 1, hFile);

		fread(m_nSizeCondOpen[x], m_nSizeCond[x]*sizeof(int), 1, hFile);
		for(int y = 0; y < m_nSizeCond[x]; y++)
		{
			if(m_nSizeCondOpen[x][y]) m_uCondOpen[x][y] = new unsigned int[m_nSizeCondOpen[x][y]];
			fread(m_uCondOpen[x][y], m_nSizeCondOpen[x][y]*sizeof(unsigned int), 1, hFile);
		}

		fread(m_nSizeCondUnlock[x], m_nSizeCond[x]*sizeof(int), 1, hFile);
		for(int y = 0; y < m_nSizeCond[x]; y++)
		{
			if(m_nSizeCondUnlock[x][y])
			{
				m_uCondUnlock[x][y] = new unsigned int[m_nSizeCondUnlock[x][y]];
				m_dwCondUnlockModes[x][y] = new DWORD[m_nSizeCondUnlock[x][y]];
			}
			fread(m_uCondUnlock[x][y], m_nSizeCondUnlock[x][y]*sizeof(unsigned int), 1, hFile);
			fread(m_dwCondUnlockModes[x][y], m_nSizeCondUnlock[x][y]*sizeof(DWORD), 1, hFile);
		}

		fread(m_nSizeCondAspUnlock[x], m_nSizeCond[x]*sizeof(int), 1, hFile);
		for(int y = 0; y < m_nSizeCond[x]; y++)
		{
			if(m_nSizeCondAspUnlock[x][y]) m_uCondAspUnlock[x][y] = new unsigned int[m_nSizeCondAspUnlock[x][y]];
			fread(m_uCondAspUnlock[x][y], m_nSizeCondAspUnlock[x][y]*sizeof(unsigned int), 1, hFile);
		}
	}
	fclose(hFile);
	return 1;
}

int CMissionLoader::SaveBinary(LPCTSTR chName)
{
	FILE* hFile = _fopen(chName, _T("w+b"));
	if(hFile == NULL) return 0;
	TCHAR* ch;
	fwrite(&m_nSize, sizeof(int), 1, hFile);
	fwrite(m_uID, m_nSize*sizeof(unsigned int), 1, hFile);
	fwrite(m_uStage, m_nSize*sizeof(unsigned int), 1, hFile);
	fwrite(m_uPath, m_nSize*4*sizeof(unsigned int), 1, hFile);
	fwrite(m_uLvl, m_nSize*sizeof(unsigned int), 1, hFile);
	fwrite(m_uPlayer, m_nSize*sizeof(unsigned int), 1, hFile);
	fwrite(m_uMileage, m_nSize*sizeof(unsigned int), 1, hFile);
	fwrite(m_uIcon, m_nSize*sizeof(unsigned int), 1, hFile);
	fwrite(m_uIconSucess, m_nSize*sizeof(unsigned int), 1, hFile);
	fwrite(m_uMap, m_nSize*sizeof(unsigned int), 1, hFile);
	fwrite(m_uGo, m_nSize*sizeof(unsigned int), 1, hFile);
	for(int x = 0; x < m_nSize; x++)
	{
		int nSize = m_strName[x].GetLength()+1;
		fwrite(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new TCHAR[nSize];
            _strcpy(ch, m_strName[x].c_str());
            char16_t* cha = new char16_t[nSize];
            Utf32ToUtf16(ch, cha, nSize);
			fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
			delete ch;
			delete cha;
		}

		fwrite(&m_nSizeSong[x], sizeof(int), 1, hFile);
		for(int y = 0; y < m_nSizeSong[x]; y++)
		{
			nSize = m_strSong[x][y].GetLength()+1;
			fwrite(&nSize, sizeof(int), 1, hFile);
			if(nSize)
			{
                ch = new TCHAR[nSize];
                _strcpy(ch, m_strSong[x][y]);
                char16_t* cha = new char16_t[nSize];
                Utf32ToUtf16(ch, cha, nSize);
                fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
                delete ch;
                delete cha;
			}
		}

		fwrite(&m_nSizeStep[x], sizeof(int), 1, hFile);
		for(int y = 0; y < m_nSizeStep[x]; y++)
		{
			nSize = m_strStep[x][y].GetLength()+1;
			fwrite(&nSize, sizeof(int), 1, hFile);
			if(nSize)
			{
                ch = new TCHAR[nSize];
                _strcpy(ch, m_strStep[x][y]);
                char16_t* cha = new char16_t[nSize];
                Utf32ToUtf16(ch, cha, nSize);
                fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
                delete ch;
                delete cha;
			}
		}

		nSize = m_strHis[x].GetLength()+1;
		fwrite(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new TCHAR[nSize];
            _strcpy(ch, m_strHis[x]);
            char16_t* cha = new char16_t[nSize];
            Utf32ToUtf16(ch, cha, nSize);
            fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
            delete ch;
            delete cha;
		}

		nSize = m_strShowCond[x].GetLength()+1;
		fwrite(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new TCHAR[nSize];
            _strcpy(ch, m_strShowCond[x]);
            char16_t* cha = new char16_t[nSize];
            Utf32ToUtf16(ch, cha, nSize);
            fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
            delete ch;
            delete cha;
		}

		nSize = m_strComment[x].GetLength()+1;
		fwrite(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new TCHAR[nSize];
            _strcpy(ch, m_strComment[x]);
            char16_t* cha = new char16_t[nSize];
            Utf32ToUtf16(ch, cha, nSize);
            fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
            delete ch;
            delete cha;
		}

		nSize = m_strGo[x].GetLength()+1;
		fwrite(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new TCHAR[nSize];
            _strcpy(ch, m_strGo[x]);
            char16_t* cha = new char16_t[nSize];
            Utf32ToUtf16(ch, cha, nSize);
            fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
            delete ch;
            delete cha;
		}

		nSize = m_strLand[x].GetLength()+1;
		fwrite(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            ch = new TCHAR[nSize];
            _strcpy(ch, m_strLand[x]);
            char16_t* cha = new char16_t[nSize];
            Utf32ToUtf16(ch, cha, nSize);
            fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
            delete ch;
            delete cha;
		}
	}
	fwrite(m_bBreak, m_nSize*sizeof(BYTE), 1, hFile);
	fwrite(m_bLink, m_nSize*sizeof(BYTE), 1, hFile);
	fwrite(m_bActUMod, m_nSize*sizeof(BYTE), 1, hFile);
	fwrite(m_dUbiX, m_nSize*sizeof(double), 1, hFile);
	fwrite(m_dUbiY, m_nSize*sizeof(double), 1, hFile);
	fwrite(m_dInitialBar, m_nSize*sizeof(double), 1, hFile);
	fwrite(m_dIncrBar, m_nSize*sizeof(double), 1, hFile);
	fwrite(m_dDecrBar, m_nSize*sizeof(double), 1, hFile);
	fwrite(m_bDouble, m_nSize*sizeof(BYTE), 1, hFile);
	fwrite(m_uVersus, m_nSize*sizeof(unsigned int), 1, hFile);
	fwrite(m_uLvlVersus, m_nSize*sizeof(unsigned int), 1, hFile);

	fwrite(m_nSizeMod, m_nSize*sizeof(int), 1, hFile);
	for(int x = 0; x < m_nSize; x++)
	{
		fwrite(m_nMod[x], m_nSizeMod[x]*sizeof(int), 1, hFile);
	}

	fwrite(m_nSizeCond, m_nSize*sizeof(int), 1, hFile);
	for(int x = 0; x < m_nSize; x++)
	{
		for(int y = 0; y < m_nSizeCond[x]; y++)
		{
			int nSize = m_strCond[x][y].GetLength()+1;
			fwrite(&nSize, sizeof(int), 1, hFile);
			if(nSize)
			{
                ch = new TCHAR[nSize];
                _strcpy(ch, m_strCond[x][y]);
                char16_t* cha = new char16_t[nSize];
                Utf32ToUtf16(ch, cha, nSize);
                fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
                delete ch;
                delete cha;
			}

			nSize = m_strHPass[x][y].GetLength()+1;
			fwrite(&nSize, sizeof(int), 1, hFile);
			if(nSize)
			{
                ch = new TCHAR[nSize];
                _strcpy(ch, m_strHPass[x][y]);
                char16_t* cha = new char16_t[nSize];
                Utf32ToUtf16(ch, cha, nSize);
                fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
                delete ch;
                delete cha;
			}

			nSize = m_strHNoPass[x][y].GetLength()+1;
			fwrite(&nSize, sizeof(int), 1, hFile);
			if(nSize)
			{
                ch = new TCHAR[nSize];
                _strcpy(ch, m_strHNoPass[x][y]);
                char16_t* cha = new char16_t[nSize];
                Utf32ToUtf16(ch, cha, nSize);
                fwrite(cha, nSize*sizeof(char16_t), 1, hFile);
                delete ch;
                delete cha;
			}
		}
		fwrite(m_bShowSucess[x], m_nSizeCond[x]*sizeof(BYTE), 1, hFile);

		fwrite(m_nSizeCondOpen[x], m_nSizeCond[x]*sizeof(int), 1, hFile);
		for(int y = 0; y < m_nSizeCond[x]; y++)
		{
			fwrite(m_uCondOpen[x][y], m_nSizeCondOpen[x][y]*sizeof(unsigned int), 1, hFile);
		}

		fwrite(m_nSizeCondUnlock[x], m_nSizeCond[x]*sizeof(int), 1, hFile);
		for(int y = 0; y < m_nSizeCond[x]; y++)
		{
			fwrite(m_uCondUnlock[x][y], m_nSizeCondUnlock[x][y]*sizeof(unsigned int), 1, hFile);
			fwrite(m_dwCondUnlockModes[x][y], m_nSizeCondUnlock[x][y]*sizeof(DWORD), 1, hFile);
		}

		fwrite(m_nSizeCondAspUnlock[x], m_nSizeCond[x]*sizeof(int), 1, hFile);
		for(int y = 0; y < m_nSizeCond[x]; y++)
		{
			fwrite(m_uCondAspUnlock[x][y], m_nSizeCondAspUnlock[x][y]*sizeof(unsigned int), 1, hFile);
		}
	}
	fclose(hFile);
	return 1;
}

int CMissionLoader::LoadText(LPCTSTR  chName)
{
	FILE* hFile = _fopen(chName, _T("r"));
	if(hFile == NULL) return 0;
	CString strPar;
	CString strArg;
	TCHAR ch[260];
	int nCur = -1;
	int nCurSong = 0;
	int nCurStep = 0;
	int nCurCond = 0;
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
		if((nCode = strPar.Find(_T("#SIZEMISSION"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSize) != 1) {fclose(hFile); return 0;}

			m_uID = new unsigned int[m_nSize];
			m_uStage = new unsigned int[m_nSize];
			m_uPath = new unsigned int[m_nSize*4];
			m_uLvl = new unsigned int[m_nSize];
			m_uPlayer = new unsigned int[m_nSize];
			m_uMileage = new unsigned int[m_nSize];
			m_uIcon = new unsigned int[m_nSize];
			m_uIconSucess = new unsigned int[m_nSize];
			m_uMap = new unsigned int[m_nSize];
			m_uGo = new unsigned int[m_nSize];
			m_strName = new CString[m_nSize];
			m_nSizeSong = new int[m_nSize];
			m_strSong = new CString*[m_nSize];
			m_nSizeStep = new int[m_nSize];
			m_strStep = new CString*[m_nSize];
			m_strHis = new CString[m_nSize];
			m_strShowCond = new CString[m_nSize];
			m_strComment = new CString[m_nSize];
			m_strGo = new CString[m_nSize];
			m_strLand = new CString[m_nSize];
			m_bBreak = new BYTE[m_nSize];
			m_bLink = new BYTE[m_nSize];
			m_bActUMod = new BYTE[m_nSize];
			m_dUbiX = new double[m_nSize];
			m_dUbiY = new double[m_nSize];
			m_dInitialBar = new double[m_nSize];
			m_dIncrBar = new double[m_nSize];
			m_dDecrBar = new double[m_nSize];
			m_bDouble = new BYTE[m_nSize];
			m_uVersus = new unsigned int[m_nSize];
			m_uLvlVersus = new unsigned int[m_nSize];

			m_nSizeMod = new int[m_nSize];
			m_nMod = new int*[m_nSize];

			m_nSizeCond = new int[m_nSize];
			m_strCond = new CString*[m_nSize];
			m_strHPass = new CString*[m_nSize];
			m_strHNoPass = new CString*[m_nSize];
			m_bShowSucess = new BYTE*[m_nSize];

			m_nSizeCondOpen = new int*[m_nSize];
			m_uCondOpen = new unsigned int**[m_nSize];

			m_nSizeCondUnlock = new int*[m_nSize];
			m_uCondUnlock = new unsigned int**[m_nSize];
			m_dwCondUnlockModes = new DWORD**[m_nSize];

			m_nSizeCondAspUnlock = new int*[m_nSize];
			m_uCondAspUnlock = new unsigned int**[m_nSize];
			for(int x = 0; x < m_nSize; x++)
			{
				m_uID[x] = 0;
				m_uStage[x] = 0;
				for(int y = 0; y < 4; y++) m_uPath[x*4+y] = 0;
				m_nSizeSong[x] = 0;
				m_nSizeStep[x] = 0;
				m_uLvl[x] = 0;
				m_uPlayer[x] = 1;
				m_uMileage[x] = 0;
				m_uIcon[x] = 0;
				m_uIconSucess[x] = 0;
				m_uMap[x] = 0;
				m_uGo[x] = 0;
				m_bBreak[x] = 0;
				m_bLink[x] = 0;
				m_bActUMod[x] = 0;
				m_dUbiX[x] = 0.0;
				m_dUbiY[x] = 0.0;
				m_dInitialBar[x] = 0.5;
				m_dIncrBar[x] = 0.01;
				m_dDecrBar[x] = 0.01;
				m_bDouble[x] = 0;
				m_uVersus[x] = 0;
				m_uLvlVersus[x] = 1;

				m_nSizeMod[x] = 0;
				m_nSizeCond[x] = 0;
			}
		}
		if((nCode = strPar.Find(_T("#MISSION"))) != -1)
		{
			nCur++;
		}
		if((nCode = strPar.Find(_T("#ID"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uID[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#STAGE"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uStage[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#PATH"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u,%u,%u,%u"), &m_uPath[nCur*4], &m_uPath[nCur*4+1], &m_uPath[nCur*4+2], &m_uPath[nCur*4+3]) != 4) {fclose(hFile); {fclose(hFile); return 0;}}
		}
		if((nCode = strPar.Find(_T("#NAME"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); {fclose(hFile); return 0;}}
			m_strName[nCur] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
		}
		if((nCode = strPar.Find(_T("#LAND"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); {fclose(hFile); return 0;}}
			m_strLand[nCur] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
		}
		if((nCode = strPar.Find(_T("#SIZESONG"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeSong[nCur]) != 1) {fclose(hFile); return 0;}
			if(m_nSizeSong[nCur])
			{
				m_strSong[nCur] = new CString[m_nSizeSong[nCur]];
			}
			nCurSong = 0;
		}
		if((nCode = strPar.Find(_T("#SIZESTEP"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeStep[nCur]) != 1) {fclose(hFile); return 0;}
			if(m_nSizeStep[nCur])
			{
				m_strStep[nCur] = new CString[m_nSizeStep[nCur]];
			}
			nCurStep = 0;
		}
		if((nCode = strPar.Find(_T("#SONG"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			m_strSong[nCur][nCurSong] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			nCurSong++;
		}
		if((nCode = strPar.Find(_T("#STEP"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			m_strStep[nCur][nCurStep] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			nCurStep++;
		}
		if((nCode = strPar.Find(_T("#HISTORY"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			m_strHis[nCur] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
		}
		if((nCode = strPar.Find(_T("#SHOWCOND"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			m_strShowCond[nCur] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			m_strShowCond[nCur].Replace(_T('#'), _T('\n'));
		}
		if((nCode = strPar.Find(_T("#DESCRIPTION"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			m_strComment[nCur] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			m_strComment[nCur].Replace(_T('#'), _T('\n'));
		}
		if((nCode = strPar.Find(_T("#GOTOSPRITE"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			m_strGo[nCur] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
		}
		if((nCode = strPar.Find(_T("#LEVEL"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uLvl[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#PLAYER"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uPlayer[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#MILEAGE"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uMileage[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#ICON"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uIcon[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#ICONSUCESS"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uIconSucess[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#MAPID"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uMap[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#BREAK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			unsigned int nCaret;
			if(_sscanf(strArg, _T("%u"), &nCaret) != 1) {fclose(hFile); return 0;}
			m_bBreak[nCur] = nCaret?1:0;
		}
		if((nCode = strPar.Find(_T("#LINK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			unsigned int nCaret;
			if(_sscanf(strArg, _T("%u"), &nCaret) != 1) {fclose(hFile); return 0;}
			m_bLink[nCur] = nCaret?1:0;
		}
		if((nCode = strPar.Find(_T("#GOLINK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uGo[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#MODCHANGE"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			unsigned int nCaret;
			if(_sscanf(strArg, _T("%u"), &nCaret) != 1) {fclose(hFile); return 0;}
			m_bActUMod[nCur] = nCaret?1:0;
		}
		if((nCode = strPar.Find(_T("#LOCATION"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			float f1, f2;
			if(_sscanf(strArg, _T("%g,%g"), &f1, &f2) != 2) {fclose(hFile); return 0;}
			m_dUbiX[nCur] = f1;
			m_dUbiY[nCur] = f2;
		}
		if((nCode = strPar.Find(_T("#INITIALBAR"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			float f1;
			if(_sscanf(strArg, _T("%g"), &f1) != 1) {fclose(hFile); return 0;}
			m_dInitialBar[nCur] = f1;
		}
		if((nCode = strPar.Find(_T("#INCRBAR"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			float f1;
			if(_sscanf(strArg, _T("%g"), &f1) != 1) {fclose(hFile); return 0;}
			m_dIncrBar[nCur] = f1;
		}
		if((nCode = strPar.Find(_T("#DECRBAR"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			float f1;
			if(_sscanf(strArg, _T("%g"), &f1) != 1) {fclose(hFile); return 0;}
			m_dDecrBar[nCur] = f1;
		}
		if((nCode = strPar.Find(_T("#DOUBLE"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			unsigned int nCaret;
			if(_sscanf(strArg, _T("%u"), &nCaret) != 1) {fclose(hFile); return 0;}
			m_bDouble[nCur] = nCaret?1:0;
		}
		if((nCode = strPar.Find(_T("#VERSUS"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uVersus[nCur]) != 1) {fclose(hFile); return 0;}
		}
		if((nCode = strPar.Find(_T("#LVLVERSUS"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%u"), &m_uLvlVersus[nCur]) != 1) {fclose(hFile); return 0;}
		}

		if((nCode = strPar.Find(_T("#SIZEMOD"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeMod[nCur]) != 1) {fclose(hFile); return 0;}
			if(m_nSizeMod[nCur])
			{
				m_nMod[nCur] = new int[m_nSizeMod[nCur]];
				for(int x = 0; x < m_nSizeMod[nCur]; x++)
				{
					m_nMod[nCur][x] = 0;
				}
			}
		}
		if((nCode = strPar.Find(_T("#MODIFIERS"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			int a = 0, b = 0;
			for(int x = 0; x < m_nSizeMod[nCur]; x++)
			{
				if((b = strArg.Find(_T(','), a)) == -1) b = strArg.GetLength();
				if(_sscanf(strArg.Mid(a, b), _T("%d"), &m_nMod[nCur][x]) != 1) {fclose(hFile); return 0;}
				a = b+1;
			}
		}

		if((nCode = strPar.Find(_T("#SIZECOND"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeCond[nCur]) != 1) {fclose(hFile); return 0;}
			if(m_nSizeCond[nCur])
			{
				m_strCond[nCur] = new CString[m_nSizeCond[nCur]];
				m_strHPass[nCur] = new CString[m_nSizeCond[nCur]];
				m_strHNoPass[nCur] = new CString[m_nSizeCond[nCur]];
				m_bShowSucess[nCur] = new BYTE[m_nSizeCond[nCur]];

				m_nSizeCondOpen[nCur] = new int[m_nSizeCond[nCur]];
				m_uCondOpen[nCur] = new unsigned int*[m_nSizeCond[nCur]];

				m_nSizeCondUnlock[nCur] = new int[m_nSizeCond[nCur]];
				m_uCondUnlock[nCur] = new unsigned int*[m_nSizeCond[nCur]];
				m_dwCondUnlockModes[nCur] = new DWORD*[m_nSizeCond[nCur]];

				m_nSizeCondAspUnlock[nCur] = new int[m_nSizeCond[nCur]];
				m_uCondAspUnlock[nCur] = new unsigned int*[m_nSizeCond[nCur]];
				for(int x = 0; x < m_nSizeCond[nCur]; x++)
				{
					m_bShowSucess[nCur][x] = 0;
					m_nSizeCondOpen[nCur][x] = 0;
					m_nSizeCondUnlock[nCur][x] = 0;
					m_nSizeCondAspUnlock[nCur][x] = 0;
				}
			}
			nCurCond = -1;
		}
		if((nCode = strPar.Find(_T("#CONDITION"))) != -1)
		{
			nCurCond++;
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			m_strCond[nCur][nCurCond] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
		}
		if((nCode = strPar.Find(_T("#HISPASS"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			m_strHPass[nCur][nCurCond] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
		}
		if((nCode = strPar.Find(_T("#HISNOPASS"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			m_strHNoPass[nCur][nCurCond] = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
		}
		if((nCode = strPar.Find(_T("#SHOWSUCESS"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			unsigned int nCaret;
			if(_sscanf(strArg, _T("%u"), &nCaret) != 1) {fclose(hFile); return 0;}
			m_bShowSucess[nCur][nCurCond] = nCaret?1:0;
		}

		if((nCode = strPar.Find(_T("#SIZEOPEN"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeCondOpen[nCur][nCurCond]) != 1) {fclose(hFile); return 0;}
			if(m_nSizeCondOpen[nCur][nCurCond])
			{
				m_uCondOpen[nCur][nCurCond] = new unsigned int[m_nSizeCondOpen[nCur][nCurCond]];
				for(int x = 0; x < m_nSizeCondOpen[nCur][nCurCond]; x++)
				{
					m_uCondOpen[nCur][nCurCond][x] = 0;
				}
			}
		}
		if((nCode = strPar.Find(_T("#OPEN"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			int a = 0, b = 0;
			for(int x = 0; x < m_nSizeCondOpen[nCur][nCurCond]; x++)
			{
				if((b = strArg.Find(_T(','), a)) == -1) b = strArg.GetLength();
				if(_sscanf(strArg.Mid(a, b), _T("%d"), &m_uCondOpen[nCur][nCurCond][x]) != 1) {fclose(hFile); return 0;}
				a = b+1;
			}
		}

		if((nCode = strPar.Find(_T("#SIZEUNLOCK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeCondUnlock[nCur][nCurCond]) != 1) {fclose(hFile); return 0;}
			if(m_nSizeCondUnlock[nCur][nCurCond])
			{
				m_uCondUnlock[nCur][nCurCond] = new unsigned int[m_nSizeCondUnlock[nCur][nCurCond]];
				m_dwCondUnlockModes[nCur][nCurCond] = new DWORD[m_nSizeCondUnlock[nCur][nCurCond]];
				for(int x = 0; x < m_nSizeCondUnlock[nCur][nCurCond]; x++)
				{
					m_uCondUnlock[nCur][nCurCond][x] = 0;
					m_dwCondUnlockModes[nCur][nCurCond][x] = 0;
				}
			}
			nCurCondUnlock = -1;
		}
		if((nCode = strPar.Find(_T("#UNLOCK"))) != -1)
		{
			nCurCondUnlock++;
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			int n[7];
			if(_sscanf(strArg, _T("%d\t%d\t%d\t%d\t%d\t%d\t%d"), &n[0], &n[1], &n[2], &n[3], &n[4], &n[5], &n[6]) != 7) {fclose(hFile); return 0;}
			m_uCondUnlock[nCur][nCurCond][nCurCondUnlock] = (unsigned int)max(0, n[0]);
			m_dwCondUnlockModes[nCur][nCurCond][nCurCondUnlock] = (n[1]&0xF)|((n[2]&0xF)<<4)|((n[3]&0xF)<<8)|((n[4]&0xF)<<12)|((n[5]&0xF)<<16)|((n[6]&0xF)<<20);
		}

		if((nCode = strPar.Find(_T("#SIZEASPUNLOCK"))) != -1)
		{
			nCode = strPar.Find(_T(':'));
			if(nCode == -1) {fclose(hFile); return 0;}
			strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
			strArg.Remove(_T(' '));
			strArg.Remove(_T('\t'));
			if(_sscanf(strArg, _T("%d"), &m_nSizeCondAspUnlock[nCur][nCurCond]) != 1) {fclose(hFile); return 0;}
			if(m_nSizeCondAspUnlock[nCur][nCurCond])
			{
				m_uCondAspUnlock[nCur][nCurCond] = new unsigned int[m_nSizeCondAspUnlock[nCur][nCurCond]];
				for(int x = 0; x < m_nSizeCondAspUnlock[nCur][nCurCond]; x++)
				{
					m_uCondAspUnlock[nCur][nCurCond][x] = 0;
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
			for(int x = 0; x < m_nSizeCondAspUnlock[nCur][nCurCond]; x++)
			{
				if((b = strArg.Find(_T(','), a)) == -1) b = strArg.GetLength();
				if(_sscanf(strArg.Mid(a, b), _T("%d"), &m_uCondAspUnlock[nCur][nCurCond][x]) != 1) {fclose(hFile); return 0;}
				a = b+1;
			}
		}
	}
	fclose(hFile);
	return 1;
}

void CMissionLoader::Destroy(void)
{
	if(m_nSize != 0)
	{
		delete m_uID;
		delete m_uPath;
		delete m_uStage;
		delete m_uLvl;
		delete m_uIcon;
		delete m_uMap;
		delete m_uGo;
		delete m_uPlayer;
		delete m_uMileage;
		delete m_uIconSucess;
		for (int x = 0; x < m_nSize; x++)
		{
			if(m_nSizeSong[x]) delete[] m_strSong[x];
			if(m_nSizeStep[x]) delete[] m_strStep[x];

			delete m_nMod[x];

			if(m_nSizeCond[x]) delete[] m_strCond[x];
			if (m_nSizeCond[x]) delete[] m_strHPass[x];
			if (m_nSizeCond[x]) delete[] m_strHNoPass[x];
			if (m_nSizeCond[x]) delete[] m_bShowSucess[x];

			for (int y = 0; y < m_nSizeCond[x]; y++)
			{
				if(m_nSizeCondOpen[x][y]) delete m_uCondOpen[x][y];
				if (m_nSizeCondUnlock[x][y]) delete m_uCondUnlock[x][y];
				if (m_nSizeCondUnlock[x][y]) delete m_dwCondUnlockModes[x][y];
				if (m_nSizeCondAspUnlock[x][y]) delete m_uCondAspUnlock[x][y];
			}

			if (m_nSizeCond[x]) delete m_uCondOpen[x];
			if (m_nSizeCond[x]) delete m_uCondUnlock[x];
			if (m_nSizeCond[x]) delete m_dwCondUnlockModes[x];
			if (m_nSizeCond[x]) delete m_uCondAspUnlock[x];

			if (m_nSizeCond[x]) delete m_nSizeCondOpen[x];

			if (m_nSizeCond[x]) delete m_nSizeCondUnlock[x];

			if (m_nSizeCond[x]) delete m_nSizeCondAspUnlock[x];
		}
		delete[] m_strName;
		delete m_nSizeSong;
		delete[] m_strSong;
		delete m_nSizeStep;
		delete[] m_strStep;
		delete[] m_strHis;
		delete[] m_strShowCond;
		delete[] m_strComment;
		delete[] m_strGo;
		delete[] m_strLand;
		delete m_bBreak;
		delete m_bLink;
		delete m_bActUMod;
		delete m_dUbiX;
		delete m_dUbiY;
		delete m_dInitialBar;
		delete m_dIncrBar;
		delete m_dDecrBar;
		delete m_bDouble;
		delete m_uVersus;
		delete m_uLvlVersus;

		delete m_nSizeMod;
		delete[] m_nMod;

		delete m_nSizeCond;
		delete[] m_strCond;
		delete[] m_strHPass;
		delete[] m_strHNoPass;
		delete[] m_bShowSucess;

		delete[] m_nSizeCondOpen;
		delete[] m_uCondOpen;

		delete[] m_nSizeCondUnlock;
		delete[] m_uCondUnlock;
		delete[] m_dwCondUnlockModes;

		delete[] m_nSizeCondAspUnlock;
		delete[] m_uCondAspUnlock;
	}
}

int CMissionLoader::SearchID(unsigned int nId)
{
	int nIndex = -1;
	for(int x = 0; x < m_nSize; x++)
	{
		if(m_uID[x] == nId)
		{
			nIndex = x;
			break;
		}
	}
	return nIndex;
}
