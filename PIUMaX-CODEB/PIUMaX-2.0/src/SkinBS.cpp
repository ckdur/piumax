#include "stdafx.h"
#include "SkinBS.h"

CSkinBS::CSkinBS(void)
{
	for(int x = 0; x < SIZE_SPRITEBS; x++)
		skin[x] = new CSprite();
}

CSkinBS::~CSkinBS(void)
{
	for(int x = 0; x < SIZE_SPRITEBS; x++)
		delete skin[x];
}

int CSkinBS::LoadSkin(void)
{
	int nDur[SIZE_SPRITEBS], nFrames[SIZE_SPRITEBS];
	FILE* hFile;
	TCHAR str[260];
	TCHAR* chArchives[] =
	{
		_T("St_item.png"),
		_T("Numbers.png"),
		_T("Characters.png"),
		_T("Forms.png"),
		_T("Arrows.png"),
		_T("Characters_shadow.png"),
		_T("Forms_shadow.png"),
		_T("Arrows_shadow.png"),
		_T("XO.png"),
		_T("Right.png"),
		_T("Wrong.png"),
		_T("PopUpBG.png"),
		_T("CenterBar.png"),
		_T("CenterSpace.png"),
		_T("LateralBar.png"),
		_T("FrontBar.png"),
		_T("Clock.png"),
		_T("Bottom_q.png"),
		_T("QuestionY.png"),
		_T("Circle.png")
	};
	hFile = _tfopen(_T("skins/BS/config.acc\0"), _T("r+"));
	if(hFile == NULL)
	{
		return -1;
	}
	for(int x = 0; x < SIZE_SPRITEBS; x++)
	{
		TCHAR chLect[500];
		_fgets(chLect, 500, hFile);
		_sscanf(chLect, _T("%d\t%d"), &nFrames[x], &nDur[x]);
	}
	fclose(hFile);
	for(int x = 0; x < SIZE_SPRITEBS; x++)
	{
		_sprintf(str, _T("skins/BS/%s"), chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	return 0;
}

void CSkinBS::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SIZE_SPRITEBS; x++)
		skin[x]->Update(dwMiliseconds);
}

void CSkinBS::Delete()
{
	for(int x = 0; x < SIZE_SPRITEBS; x++)
		skin[x]->DeleteImage();
}
