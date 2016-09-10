#include "stdafx.h"
#include "SkinBS.h"

CSkinBS::CSkinBS(void)
{
	for(int x = 0; x < SIZE_SPRITEBS; x++)
		skin[x] = new COGLSprite();
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
	TCHAR chArchives[][260] =
	{
		_T("ST_Item.png"),
		_T("Numbers.png"),
		_T("Characters.png"),
		_T("Forms.png"),
		_T("Arrows.png"),
		_T("Characters_Shadow.png"),
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
	hFile = _fopen(_T("skins/BS/config.acc\0"), _T("r"));
	if(hFile == NULL)
	{
        _printf(_T("File not found: %ls\n"), str);
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
		_sprintf(str, _T("skins/BS/%ls"), chArchives[x]);
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
