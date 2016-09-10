#include "stdafx.h"
#include "SkinCommon.h"

CSkinCommon::CSkinCommon(void)
{
	for(int x = 0; x < SIZE_SPRITE; x++)
		skin[x] = new COGLSprite();
}

CSkinCommon::~CSkinCommon(void)
{
	for(int x = 0; x < SIZE_SPRITE; x++)
		delete skin[x];
}

int CSkinCommon::LoadSkin(void)
{
	int nDur[SIZE_SPRITE], nFrames[SIZE_SPRITE];
	FILE* hFile;
	TCHAR str[260];
	TCHAR chArchives[][260] =
	{
		_T("mine.png"),
		_T("potion.png"),
		_T("heart.png"),
		_T("flash.png"),
		_T("BackToNormal.png"),
		_T("VelocityX1.png"),
		_T("VelocityX2.png"),
		_T("VelocityX3.png"),
		_T("VelocityX4.png"),
		_T("VelocityX5.png"),
		_T("VelocityX8.png"),
		_T("drop.png"),
		_T("UnderAttack.png"),
		_T("LeftAttack.png"),
		_T("RightAttack.png"),
		_T("snake.png"),
		_T("EVA1.png"),
		_T("EVA2.png"),
		_T("EVA3.png"),
		_T("EVA4.png"),
		_T("EVA5.png"),
		_T("font0.png"),
		_T("font1.png"),
		_T("font2.png"),
		_T("font3.png"),
		_T("font4.png"),
		_T("font5.png"),
		_T("font6.png"),
		_T("font7.png"),
		_T("font8.png"),
		_T("font9.png"),
		_T("BarSkin.png"),
		_T("BarActive.png"),
		_T("Combo.png"),
		_T("Combomiss.png")
	};
	hFile = _fopen(_T("skins/Common/config.acc\0"), _T("r"));
	if(hFile == NULL)
	{
        _printf(_T("File not found: %ls\n"), str);
		return -1;
	}
	for(int x = 0; x < SIZE_SPRITE; x++)
		_fscanf(hFile, _T("%d\t%d\n"), &nFrames[x], &nDur[x]);
	fclose(hFile);
	for(int x = 0; x < SIZE_SPRITE; x++)
	{
		_sprintf(str, _T("skins/Common/%ls"), chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	return 0;
}

void CSkinCommon::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SIZE_SPRITE; x++)
		skin[x]->Update(dwMiliseconds);
}

void CSkinCommon::Delete()
{
	for(int x = 0; x < SIZE_SPRITE; x++)
		skin[x]->DeleteImage();
}
