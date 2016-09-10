#include "stdafx.h"
#include "UsbRes.h"

CUsbRes::CUsbRes(void)
{
	for(int x = 0; x < SIZE_USBRES; x++)
		skin[x] = new COGLSprite();
}

CUsbRes::~CUsbRes(void)
{
	for(int x = 0; x < SIZE_USBRES; x++)
		delete skin[x];
}

int CUsbRes::LoadSkin(void)
{
	int nDur[SIZE_USBRES], nFrames[SIZE_USBRES];
	FILE* hFile;
	TCHAR str[260];
	TCHAR chArchives[][260] =
	{
		_T("USB-UpAndDown"),
		_T("ProfileWindow"),
		_T("ProfileBackWindow"),
		_T("ProfileName"),
		_T("ProfileSplash"),
		_T("ProfileNonUsb"),
		_T("SesionEnum"),
		_T("SesionContSelect1"),
		_T("SesionContSelect2"),
		_T("ProfileBackGlouth"),
		_T("ProfileBackWindowGlouth"),
		_T("ProfileNameGlouth"),
		_T("SesionTray"),
		_T("DataInstruction"),
		_T("DataLoading"),
		_T("DataError"),
		_T("SesionMenuMulti"),
		_T("SesionConnector"),
		_T("FowardGaming"),
		_T("ExpBar"),
		_T("Item1-1"),
		_T("Item1-2"),
		_T("Item2-1"),
		_T("Item2-2"),
		_T("Item3-1"),
		_T("Item3-2"),
		_T("Item4-1"),
		_T("Item4-2"),
		_T("Item5-1"),
		_T("Item5-2"),
		_T("Item6-1"),
		_T("Item6-2"),
		_T("Item7-1"),
		_T("Item7-2")
	};
	TCHAR chDir[260] = _T("USB/");
	_sprintf(str, _T("%lsconfig.acc"), chDir);
	hFile = _fopen(str, _T("r"));
	if(hFile == NULL)
	{
		return 0;
	}
	int nLang[SIZE_USBRES];
	TCHAR ch[100];
	for(int x = 0; x < SIZE_USBRES; x++)
	{
		if(_fgets(ch, 100, hFile) == NULL) {fclose(hFile); return 0;}
		if(_sscanf(ch, _T("%d\t%d\t%d"), &nFrames[x], &nDur[x], &nLang[x]) != 3) {fclose(hFile); return 0;}
	}
	fclose(hFile);
	for(int x = 0; x < SIZE_USBRES; x++)
	{
		if(nLang[x]) _sprintf(str, _T("%ls%ls_%ls.png"), chDir, chArchives[x], chLang);
		else _sprintf(str, _T("%ls%ls.png"), chDir, chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	return 1;
}

void CUsbRes::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SIZE_USBRES; x++)
		skin[x]->Update(dwMiliseconds);
}

void CUsbRes::Delete()
{
	for(int x = 0; x < SIZE_USBRES; x++)
		skin[x]->DeleteImage();
}
