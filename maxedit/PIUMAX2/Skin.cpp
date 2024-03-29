#include "stdafx.h"
#include "Skin.h"

CSkin::CSkin(void)
{
	dScale = 1.0;
	for(int x = 0; x < SKIN_SIZE; x++)
		skin[x] = new CSprite();
}

CSkin::~CSkin(void)
{
	for(int x = 0; x < SKIN_SIZE; x++)
		delete skin[x];
}

int CSkin::LoadSkin(int nSkin)
{
	dScale = 1.0;
	int nDur[SKIN_SIZE], nFrames[SKIN_SIZE];
	FILE* hFile;
	TCHAR str[260];
	TCHAR* chArchives[] =
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
		_T("EBdownLeft.png"),
		_T("EBupLeft.png"),
		_T("EBcenter.png"),
		_T("EBupRight.png"),
		_T("EBdownRight.png"),
		_T("GdownLeft.png"),
		_T("GupLeft.png"),
		_T("Gcenter.png"),
		_T("GupRight.png"),
		_T("GdownRight.png")
	};
	_sprintf(str, _T("skins/%d/config.acc"), nSkin);
	hFile = _tfopen(str, _T("r+"));
	if(hFile == NULL)
	{
		return -1;
	}
	for(int x = 0; x < SKIN_SIZE; x++)
	{
		_fscanf(hFile, _T("%d\t%d\n"), &nFrames[x], &nDur[x]);
	}
	fclose(hFile);

	for(int x = 0; x < SKIN_SIZE; x++)
	{
		_sprintf(str, _T("skins/%d/%s"), nSkin, chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}

	_sprintf(str, _T("skins/%d/scale.acc"), nSkin);
	hFile = _tfopen(str, _T("r+"));
	if(hFile != NULL)
	{
		float fScale = 0.0;
		char ch[260];
		_fgets(ch, 260, hFile);
		if(_sscanf(ch, _T("%f\r\n"), &fScale) == 1)
			dScale = (double)fScale;
		fclose(hFile);
	}
	return 0;
}

void CSkin::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SKIN_SIZE; x++)
		skin[x]->Update(dwMiliseconds);
}

void CSkin::Delete()
{
	for(int x = 0; x < SKIN_SIZE; x++)
		skin[x]->DeleteImage();
}
