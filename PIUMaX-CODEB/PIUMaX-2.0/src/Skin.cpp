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
		_T("Tdownleft.png"),
		_T("Tupleft.png"),
		_T("Tcenter.png"),
		_T("Tupright.png"),
		_T("Tdownright.png"),
		_T("Fdownleft.png"),
		_T("Fupleft.png"),
		_T("Fcenter.png"),
		_T("Fupright.png"),
		_T("Fdownright.png"),
		_T("LFdownleft.png"),
		_T("LFupleft.png"),
		_T("LFcenter.png"),
		_T("LFupright.png"),
		_T("LFdownright.png"),
		_T("RUdownleft.png"),
		_T("RUUpleft.png"),
		_T("RUCenter.png"),
		_T("RUupright.png"),
		_T("RUdownright.png"),
		_T("RLdownleft.png"),
		_T("RLupleft.png"),
		_T("RLcenter.png"),
		_T("RLupright.png"),
		_T("RLdownright.png"),
		_T("EBdownleft.png"),
		_T("EBUpleft.png"),
		_T("EBCenter.png"),
		_T("EBupright.png"),
		_T("EBdownright.png"),
		_T("Gdownleft.png"),
		_T("Gupleft.png"),
		_T("Gcenter.png"),
		_T("Gupright.png"),
		_T("Gdownright.png")
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
		if(_fscanf(hFile, _T("%f\n"), &fScale) == 1)
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
