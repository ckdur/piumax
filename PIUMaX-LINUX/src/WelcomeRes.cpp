#include "stdafx.h"
#include "WelcomeRes.h"

CWelcomeRes::CWelcomeRes(void)
{
	for(int x = 0; x < SIZE_WELRES; x++)
		skin[x] = new COGLSprite();
}

CWelcomeRes::~CWelcomeRes(void)
{
	for(int x = 0; x < SIZE_WELRES; x++)
		delete skin[x];
}

int CWelcomeRes::LoadSkin(void)
{
	int nDur[SIZE_WELRES], nFrames[SIZE_WELRES];
	FILE* hFile;
	TCHAR str[260];
	TCHAR chArchives[][260] =
	{
		_T("CenterSplash"),
		_T("CenterBorder"),
		_T("Please_insert_coin"),
		_T("Press_center_step"),
		_T("Song_start"),
		_T("Adventure"),
		_T("Advanced_station"),
		_T("Brain_shower"),
		_T("Training"),
		_T("bright_choose"),
		_T("CreditBorder"),
		_T("SesionTray"),
		_T("SesionTrayMileage"),
		_T("SesionTrayExp"),
		_T("SesionTrayExpBar"),
		_T("UpAndDownBest"),
		_T("ScoreBorder"),
		_T("UpAndDownLoop")
	};
	TCHAR chDir[260] = _T("welcome/");
	_sprintf(str, _T("%lsconfig.acc\0"), chDir);
	hFile = _fopen(str, _T("r"));
	if(hFile == NULL)
	{
        _printf(_T("File not found: %ls\n"), str);
		return 0;
	}
	int nLang[SIZE_WELRES];
	TCHAR ch[100];
	for(int x = 0; x < SIZE_WELRES; x++)
	{
		if(_fgets(ch, 100, hFile) == NULL) {fclose(hFile); return 0;}
		if(_sscanf(ch, _T("%d\t%d\t%d"), &nFrames[x], &nDur[x], &nLang[x]) != 3) {fclose(hFile); return 0;}
	}
	fclose(hFile);
	for(int x = 0; x < SIZE_WELRES; x++)
	{
		if(nLang[x]) _sprintf(str, _T("%ls%ls_%ls.png"), chDir, chArchives[x], chLang);
		else _sprintf(str, _T("%ls%ls.png"), chDir, chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	return 1;
}

void CWelcomeRes::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SIZE_WELRES; x++)
		skin[x]->Update(dwMiliseconds);
}

void CWelcomeRes::Delete()
{
	for(int x = 0; x < SIZE_WELRES; x++)
		skin[x]->DeleteImage();
}
