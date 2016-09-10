#include "stdafx.h"
#include "UnlockRes.h"

CUnlockRes::CUnlockRes(void)
{
	for(int x = 0; x < SIZE_UNLOCKRES; x++)
		skin[x] = new COGLSprite();
}

CUnlockRes::~CUnlockRes(void)
{
	for(int x = 0; x < SIZE_UNLOCKRES; x++)
		delete skin[x];
}

int CUnlockRes::LoadSkin(void)
{
	int nDur[SIZE_UNLOCKRES], nFrames[SIZE_UNLOCKRES];
	FILE* hFile;
	TCHAR str[260];
	TCHAR chArchives[][260] =
	{
		_T("Window"),
		_T("PresetOn1"),
		_T("PresetOn2"),
		_T("PresetOn3"),
		_T("PresetOn4"),
		_T("PresetOn5"),
		_T("PresetOn6"),
		_T("PresetOn7"),
		_T("PresetOn8"),
		_T("PresetOn9"),
		_T("PresetOn10"),
		_T("PresetOn11"),
		_T("PresetOn12"),
		_T("PresetOn13"),
		_T("PresetOn14"),
		_T("PresetOn15"),
		_T("PresetOn16"),
		_T("PresetGlouth1"),
		_T("PresetGlouth2"),
		_T("PresetGlouth3"),
		_T("NameSongBorder"),
		_T("SongParent")
	};
	TCHAR chDir[260] = _T("Unlock/");
	_sprintf(str, _T("%lsconfig.acc"), chDir);
	hFile = _fopen(str, _T("r"));
	if(hFile == NULL)
	{
		return 0;
	}
	int nLang[SIZE_UNLOCKRES];
	TCHAR ch[100];
	for(int x = 0; x < SIZE_UNLOCKRES; x++)
	{
		if(_fgets(ch, 100, hFile) == NULL) {fclose(hFile); return 0;}
		if(_sscanf(ch, _T("%d\t%d\t%d"), &nFrames[x], &nDur[x], &nLang[x]) != 3) {fclose(hFile); return 0;}
	}
	fclose(hFile);
	for(int x = 0; x < SIZE_UNLOCKRES; x++)
	{
		if(nLang[x]) _sprintf(str, _T("%ls%ls_%ls.png"), chDir, chArchives[x], chLang);
		else _sprintf(str, _T("%ls%ls.png"), chDir, chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	return 1;
}

void CUnlockRes::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SIZE_UNLOCKRES; x++)
		skin[x]->Update(dwMiliseconds);
}

void CUnlockRes::Delete()
{
	for(int x = 0; x < SIZE_UNLOCKRES; x++)
		skin[x]->DeleteImage();
}

CUnlockBasRes::CUnlockBasRes(void)
{
	for(int x = 0; x < SIZE_UNLOCKBASRES; x++)
		skin[x] = new COGLSprite();
}

CUnlockBasRes::~CUnlockBasRes(void)
{
	for(int x = 0; x < SIZE_UNLOCKBASRES; x++)
		delete skin[x];
}

int CUnlockBasRes::LoadSkin(void)
{
	int nDur[SIZE_UNLOCKBASRES], nFrames[SIZE_UNLOCKBASRES];
	FILE* hFile;
	TCHAR str[260];
	TCHAR chArchives[][260] =
	{
		_T("UpLeftChoice"),
		_T("UpRightChoice"),
		_T("DownLeftChoice"),
		_T("DownRightChoice"),
		_T("SongBorderBright"),
		_T("SongSkin"),
		_T("SongDiv"),
		_T("SongBorderCha1"),
		_T("SongBorderCha2"),
		_T("SongBorderCha3"),
		_T("SongBorderCha4"),
		_T("SongBorderCha5"),
		_T("SongBorderCha6"),
		_T("SongBorderCha7"),
		_T("SongBorderCha8"),
		_T("SongBorderCha9"),
		_T("UpAndDownCha1"),
		_T("UpAndDownCha2"),
		_T("UpAndDownCha3"),
		_T("UpAndDownCha4"),
		_T("UpAndDownCha5"),
		_T("UpAndDownCha6"),
		_T("UpAndDownCha7"),
		_T("UpAndDownCha8"),
		_T("UpAndDownCha9"),
		_T("UpAndDownBrightCha1"),
		_T("UpAndDownBrightCha2"),
		_T("UpAndDownBrightCha3"),
		_T("UpAndDownBrightCha4"),
		_T("UpAndDownBrightCha5"),
		_T("UpAndDownBrightCha6"),
		_T("UpAndDownBrightCha7"),
		_T("UpAndDownBrightCha8"),
		_T("UpAndDownBrightCha9"),
		_T("UpAndDownMaskCha1"),
		_T("UpAndDownMaskCha2"),
		_T("UpAndDownMaskCha3"),
		_T("UpAndDownMaskCha4"),
		_T("UpAndDownMaskCha5"),
		_T("UpAndDownMaskCha6"),
		_T("UpAndDownMaskCha7"),
		_T("UpAndDownMaskCha8"),
		_T("UpAndDownMaskCha9"),
		_T("CoverNames"),
		_T("UnlockMod"),
		_T("UnlockModGlouth"),
		_T("LockOpen"),
		_T("Lock"),
		_T("SymbolX"),
		_T("SymbolOk"),
		_T("SelectorP1"),
		_T("SelectorP2"),
		_T("SelectedP1"),
		_T("SelectedP2"),
		_T("BannerMillage"),
		_T("Unlocked"),
		_T("NoMileage")
	};
	TCHAR chDir[260] = _T("Unlock/");
	_sprintf(str, _T("%lsconfigbas.acc"), chDir);
	hFile = _fopen(str, _T("r"));
	if(hFile == NULL)
	{
        _printf(_T("Can't open file %ls"), str);
		return 0;
	}
	int nLang[SIZE_UNLOCKBASRES];
	TCHAR ch[100];
	for(int x = 0; x < SIZE_UNLOCKBASRES; x++)
	{
		if(_fgets(ch, 100, hFile) == NULL) {fclose(hFile); return 0;}
		if(_sscanf(ch, _T("%d\t%d\t%d"), &nFrames[x], &nDur[x], &nLang[x]) != 3) {fclose(hFile); return 0;}
	}
	fclose(hFile);
	for(int x = 0; x < SIZE_UNLOCKBASRES; x++)
	{
		if(nLang[x]) _sprintf(str, _T("%ls%ls_%ls.png"), chDir, chArchives[x], chLang);
		else _sprintf(str, _T("%ls%ls.png"), chDir, chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	return 1;
}

void CUnlockBasRes::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SIZE_UNLOCKBASRES; x++)
		skin[x]->Update(dwMiliseconds);
}

void CUnlockBasRes::Delete()
{
	for(int x = 0; x < SIZE_UNLOCKBASRES; x++)
		skin[x]->DeleteImage();
}
