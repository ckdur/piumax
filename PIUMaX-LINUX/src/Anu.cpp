#include "Anu.h"

CAnu::CAnu(void)
{
	anu = new ANUMAX();
	anu->nDur = 0;
	anu->nSizeObj = 0;
	anu->nSizeAnu = 0;
	m_nNext = 0;
	m_bLoop = false;
	m_bLoad = false;
}

CAnu::~CAnu(void)
{
	Destroy();
	delete anu;
}

int CAnu::Open(LPCTSTR lpszPathName)
{
	TCHAR chDir[260] = _T("Game2/anu/");
	TCHAR strDrive[260];
	TCHAR strDir[260];
	TCHAR strFile[260];
	TCHAR strAcc[260];
	TCHAR* ch;
	_tsplitpath(lpszPathName, strDrive, strDir, strFile, strAcc);
	_sprintf(strAcc, _T("%ls%ls%ls.acc"), strDrive, strDir, strFile);
	FILE* hFile = _fopen(lpszPathName, _T("rb"));
	FILE* hFileAcc = _fopen(strAcc, _T("r"));
	if(hFile == NULL)
	{
        _printf(_T("Cannot open Anu file: %ls\n"), lpszPathName);
        return 0;
	}
	fread(&anu->nDur, sizeof(__int64), 1, hFile);
	fread(&anu->nSizeObj, sizeof(int), 1, hFile);
	anu->strFile = new CString[anu->nSizeObj];
	anu->dSizeX = new double[anu->nSizeObj];
	anu->dSizeY = new double[anu->nSizeObj];
	anu->skin = new COGLSprite*[anu->nSizeObj];

	fread(anu->dSizeX, anu->nSizeObj*sizeof(double), 1, hFile);
	fread(anu->dSizeY, anu->nSizeObj*sizeof(double), 1, hFile);
	for(int x = 0; x < anu->nSizeObj; x++)
	{
		int nSize = 0;
		fread(&nSize, sizeof(int), 1, hFile);
		if(nSize)
		{
            char16_t* cha = new char16_t[nSize+1];
			fread(cha, nSize*sizeof(char16_t), 1, hFile);
			cha[nSize] = 0;
			ch = new TCHAR[nSize+1];
			memset(ch, 0, (nSize+1)*sizeof(TCHAR));
			Utf16ToUtf32(cha, (wchar_t*)ch, nSize);
			anu->strFile[x] = ch;
			delete ch;
			delete cha;
		}
		int nFrames = 1, nDur = 0, nLang = 0;
		if(hFileAcc != NULL)
		{
			ch = new TCHAR[260];
			if(_fgets(ch, 260, hFileAcc) == NULL)
			{
				fclose(hFileAcc);
				hFileAcc = NULL;
			}
			else
			{
				if(_sscanf(ch, _T("%d\t%d\t%d"), &nFrames, &nDur, &nLang) != 3)
				{nFrames = 1; nDur = 0; nLang = 0;}
			}
			delete ch;
		}
		else {nFrames = 1; nDur = 0; nLang = 0;}
		CString str;
		if(nLang)
		{
			int nIndex = 0;
			if((nIndex = anu->strFile[x].Find(_T('.'))) == -1)
				nIndex = anu->strFile[x].GetLength();
			str.Format(_T("%ls%ls_%ls%ls"), chDir, anu->strFile[x].Left(nIndex), chLang, anu->strFile[x].Right(anu->strFile[x].GetLength()-nIndex));
		}
		else str.Format(_T("%ls%ls"), chDir, anu->strFile[x]);
		anu->skin[x] = new COGLSprite();
		anu->skin[x]->InitImage(str, nFrames);
		anu->skin[x]->SetDuration(nDur);
	}
	fread(&anu->nSizeAnu, sizeof(int32_t), 1, hFile);
	anu->nObjRes = new int32_t[anu->nSizeAnu];
	anu->nSizeChk = new int32_t[anu->nSizeAnu];
	anu->dUbiX = new double*[anu->nSizeAnu];
	anu->dUbiY = new double*[anu->nSizeAnu];
	anu->dScaleX = new double*[anu->nSizeAnu];
	anu->dScaleY = new double*[anu->nSizeAnu];
	anu->dAngle = new double*[anu->nSizeAnu];
	anu->dRGB = new DWORD*[anu->nSizeAnu];
	anu->bDraw = new BYTE*[anu->nSizeAnu];
	anu->nBegAnu = new __int64*[anu->nSizeAnu];
	anu->nDurAnu = new __int64*[anu->nSizeAnu];
	anu->dExpTUbiX = new double*[anu->nSizeAnu];
	anu->dExpTUbiY = new double*[anu->nSizeAnu];
	anu->dExpTScaleX = new double*[anu->nSizeAnu];
	anu->dExpTScaleY = new double*[anu->nSizeAnu];
	anu->dExpTAngle = new double*[anu->nSizeAnu];
	anu->dExpTRGB = new double*[anu->nSizeAnu];
	fread(anu->nObjRes, anu->nSizeAnu*sizeof(int32_t), 1, hFile);
	fread(anu->nSizeChk, anu->nSizeAnu*sizeof(int32_t), 1, hFile);
	for(int x = 0; x < anu->nSizeAnu; x++) if(anu->nSizeChk[x] > 1)
	{
		anu->dUbiX[x] = new double[anu->nSizeChk[x]];
		anu->dUbiY[x] = new double[anu->nSizeChk[x]];
		anu->dScaleX[x] = new double[anu->nSizeChk[x]];
		anu->dScaleY[x] = new double[anu->nSizeChk[x]];
		anu->dAngle[x] = new double[anu->nSizeChk[x]];
		anu->dRGB[x] = new DWORD[anu->nSizeChk[x]];
		anu->bDraw[x] = new BYTE[anu->nSizeChk[x]-1];
		anu->nBegAnu[x] = new __int64[anu->nSizeChk[x]-1];
		anu->nDurAnu[x] = new __int64[anu->nSizeChk[x]-1];
		anu->dExpTUbiX[x] = new double[anu->nSizeChk[x]-1];
		anu->dExpTUbiY[x] = new double[anu->nSizeChk[x]-1];
		anu->dExpTScaleX[x] = new double[anu->nSizeChk[x]-1];
		anu->dExpTScaleY[x] = new double[anu->nSizeChk[x]-1];
		anu->dExpTAngle[x] = new double[anu->nSizeChk[x]-1];
		anu->dExpTRGB[x] = new double[anu->nSizeChk[x]-1];

		fread(anu->dUbiX[x], anu->nSizeChk[x]*sizeof(double), 1, hFile);
		fread(anu->dUbiY[x], anu->nSizeChk[x]*sizeof(double), 1, hFile);
		fread(anu->dScaleX[x], anu->nSizeChk[x]*sizeof(double), 1, hFile);
		fread(anu->dScaleY[x], anu->nSizeChk[x]*sizeof(double), 1, hFile);
		fread(anu->dAngle[x], anu->nSizeChk[x]*sizeof(double), 1, hFile);
		fread(anu->dRGB[x], anu->nSizeChk[x]*sizeof(DWORD), 1, hFile);

		fread(anu->bDraw[x], (anu->nSizeChk[x]-1)*sizeof(BYTE), 1, hFile);
		fread(anu->nBegAnu[x], (anu->nSizeChk[x]-1)*sizeof(__int64), 1, hFile);
		fread(anu->nDurAnu[x], (anu->nSizeChk[x]-1)*sizeof(__int64), 1, hFile);
		fread(anu->dExpTUbiX[x], (anu->nSizeChk[x]-1)*sizeof(double), 1, hFile);
		fread(anu->dExpTUbiY[x], (anu->nSizeChk[x]-1)*sizeof(double), 1, hFile);
		fread(anu->dExpTScaleX[x], (anu->nSizeChk[x]-1)*sizeof(double), 1, hFile);
		fread(anu->dExpTScaleY[x], (anu->nSizeChk[x]-1)*sizeof(double), 1, hFile);
		fread(anu->dExpTAngle[x], (anu->nSizeChk[x]-1)*sizeof(double), 1, hFile);
		fread(anu->dExpTRGB[x], (anu->nSizeChk[x]-1)*sizeof(double), 1, hFile);
	}
	fclose(hFile);
	m_bLoad = true;
	return 1;
}

void CAnu::Destroy(void)
{
	if(m_bLoad)
	{
		if(anu->nSizeObj)
		{
			delete[] anu->strFile;
			delete[] anu->dSizeX;
			delete[] anu->dSizeY;
			for(int x = 0; x < anu->nSizeObj; x++) delete anu->skin[x];
			delete[] anu->skin;
		}
		if(anu->nSizeAnu)
		{
			delete[] anu->nObjRes;
			delete[] anu->nSizeChk;
			delete[] anu->dUbiX;
			delete[] anu->dUbiY;
			delete[] anu->dScaleX;
			delete[] anu->dScaleY;
			delete[] anu->dAngle;
			delete[] anu->dRGB;
			delete[] anu->bDraw;
			delete[] anu->nBegAnu;
			delete[] anu->nDurAnu;
			delete[] anu->dExpTUbiX;
			delete[] anu->dExpTUbiY;
			delete[] anu->dExpTScaleX;
			delete[] anu->dExpTScaleY;
			delete[] anu->dExpTAngle;
			delete[] anu->dExpTRGB;
		}
		anu->nDur = 0;
		anu->nSizeObj = 0;
		anu->nSizeAnu = 0;
		m_bLoad = false;
	}
}

void CAnu::Update(DWORD dwMilliseconds)
{
	m_nNext += dwMilliseconds;
	if(m_nNext >= anu->nDur)
	{
		if(m_bLoop)
		{
			m_nNext = 0;
		}
		else
		{
			m_nNext = anu->nDur;
		}
	}
	for(int x = 0; x < anu->nSizeObj; x++)
		anu->skin[x]->Update(dwMilliseconds);
}

void CAnu::SetZeroCount(void)
{
	m_nNext = 0;
	for(int x = 0; x < anu->nSizeObj; x++)
	{
		anu->skin[x]->m_dwNext = 0;
		anu->skin[x]->m_dwFrame = 0;
	}
}

void CAnu::Draw(void)
{
	for(int x = 0; x < anu->nSizeAnu; x++)
	{
		for(int y = 0; y < (anu->nSizeChk[x]-1); y++)
		{
			double dUbiX, dUbiY, dScaleX, dScaleY, dAngle, dApt, dAct, dr, dg, db, da;
			if(anu->bDraw[x][y] &&
				m_nNext >= anu->nBegAnu[x][y] &&
				m_nNext <= (anu->nBegAnu[x][y]+anu->nDurAnu[x][y]))
			{
				dAct = double(m_nNext-anu->nBegAnu[x][y])/double(anu->nDurAnu[x][y]);
				if(anu->dExpTUbiX[x][y] < 0.0) dApt = 0.0;
				else dApt = pow(dAct, anu->dExpTUbiX[x][y]);
				dUbiX = anu->dUbiX[x][y]
				+(anu->dUbiX[x][y+1]-anu->dUbiX[x][y])*dApt;

				if(anu->dExpTUbiY[x][y] < 0.0) dApt = 0.0;
				else dApt = pow(dAct, anu->dExpTUbiY[x][y]);
				dUbiY = anu->dUbiY[x][y]
				+(anu->dUbiY[x][y+1]-anu->dUbiY[x][y])*dApt;

				if(anu->dExpTScaleX[x][y] < 0.0) dApt = 0.0;
				else dApt = pow(dAct, anu->dExpTScaleX[x][y]);
				dScaleX = anu->dScaleX[x][y]
				+(anu->dScaleX[x][y+1]-anu->dScaleX[x][y])*dApt;

				if(anu->dExpTScaleY[x][y] < 0.0) dApt = 0.0;
				else dApt = pow(dAct, anu->dExpTScaleY[x][y]);
				dScaleY = anu->dScaleY[x][y]
				+(anu->dScaleY[x][y+1]-anu->dScaleY[x][y])*dApt;

				if(anu->dExpTAngle[x][y] < 0.0) dApt = 0.0;
				else dApt = pow(dAct, anu->dExpTAngle[x][y]);
				dAngle = anu->dAngle[x][y]
				+(anu->dAngle[x][y+1]-anu->dAngle[x][y])*dApt;

				if(anu->dExpTRGB[x][y] < 0.0) dApt = 0.0;
				else dApt = pow(dAct, anu->dExpTRGB[x][y]);
				dr = (double(GetRValue(anu->dRGB[x][y]))
				+double(GetRValue(anu->dRGB[x][y+1])-GetRValue(anu->dRGB[x][y]))*dApt)/255.0;
				dg = (double(GetGValue(anu->dRGB[x][y]))
				+double(GetGValue(anu->dRGB[x][y+1])-GetGValue(anu->dRGB[x][y]))*dApt)/255.0;
				db = (double(GetBValue(anu->dRGB[x][y]))
				+double(GetBValue(anu->dRGB[x][y+1])-GetBValue(anu->dRGB[x][y]))*dApt)/255.0;
				da = (double(GetAValue(anu->dRGB[x][y]))
				+double(GetAValue(anu->dRGB[x][y+1])-GetAValue(anu->dRGB[x][y]))*dApt)/255.0;

				glColor4d(dr, dg, db, da);
				glBindTexture(GL_TEXTURE_2D, anu->skin[anu->nObjRes[x]]->GetCurrentTex());
				glPushMatrix();
				glTranslated(dUbiX, dUbiY, 0.0);
				glRotated(dAngle, 0.0, 0.0, 1.0);
				glScaled(dScaleX, dScaleY, 1.0);
				double dSizeX = anu->dSizeX[anu->nObjRes[x]]/2;
				double dSizeY = anu->dSizeY[anu->nObjRes[x]]/2;
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 0.0); glVertex3d(-dSizeX, -dSizeY, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( dSizeX, -dSizeY, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( dSizeX,  dSizeY, -50.0);
				glTexCoord2d(0.0, 1.0); glVertex3d(-dSizeX,  dSizeY, -50.0);
				glEnd();
				glPopMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0);
			}
		}
	}
}
