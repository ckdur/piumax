#include "stdafx.h"
#include "Skin.h"

CSkin::CSkin(void)
{
	dScale = 1.0;
	for(int x = 0; x < 35; x++)
		skin[x] = new COGLSprite();
}

CSkin::~CSkin(void)
{
	for(int x = 0; x < 35; x++)
		delete skin[x];
}

int CSkin::LoadSkin(int nSkin)
{
	dScale = 1.0;
	int nDur[35], nFrames[35];
	FILE* hFile;
	TCHAR str[260];
	TCHAR chArchives[][260] =
	{
		_T("TdownLeft.png"),
		_T("TupLeft.png"),
		_T("Tcenter.png"),
		_T("TupRight.png"),
		_T("TdownRight.png"),
		_T("FdownLeft.png"),
		_T("FupLeft.png"),
		_T("Fcenter.png"),
		_T("FupRight.png"),
		_T("FdownRight.png"),
		_T("LFdownLeft.png"),
		_T("LFupLeft.png"),
		_T("LFcenter.png"),
		_T("LFupRight.png"),
		_T("LFdownRight.png"),
		_T("RUdownLeft.png"),
		_T("RUupLeft.png"),
		_T("RUcenter.png"),
		_T("RUupRight.png"),
		_T("RUdownRight.png"),
		_T("RLdownLeft.png"),
		_T("RLupLeft.png"),
		_T("RLcenter.png"),
		_T("RLupRight.png"),
		_T("RLdownRight.png"),
		_T("EBDownLeft.png"),
		_T("EBUpLeft.png"),
		_T("EBCenter.png"),
		_T("EBUpRight.png"),
		_T("EBDownRight.png"),
		_T("GdownLeft.png"),
		_T("GupLeft.png"),
		_T("GCenter.png"),
		_T("GupRight.png"),
		_T("GdownRight.png")
	};
	_sprintf(str, _T("skins/%d/config.acc"), nSkin);
	hFile = _fopen(str, _T("r"));
	if(hFile == NULL)
	{
        _printf(_T("File not found: %ls\n"), str);
		return -1;
	}
	for(int x = 0; x < 35; x++)
		_fscanf(hFile, _T("%d\t%d\n"), &nFrames[x], &nDur[x]);
	fclose(hFile);
	for(int x = 0; x < 35; x++)
	{
		_sprintf(str, _T("skins/%d/%ls"), nSkin, chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	_sprintf(str, _T("skins/%d/scale.acc"), nSkin);
	hFile = _fopen(str, _T("r"));
	if(hFile != NULL)
	{
		float fScale = 0.0;
		if(_fscanf(hFile, _T("%f\n"), &fScale) == 1)
			dScale = (double)fScale;
		fclose(hFile);
	}
	return 0;
}

void CSkin::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < 35; x++)
		skin[x]->Update(dwMiliseconds);
}

void CSkin::Delete()
{
	for(int x = 0; x < 35; x++)
		skin[x]->DeleteImage();
}
