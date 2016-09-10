#include "stdafx.h"
#include "SkinCommon.h"

CSkinCommon::CSkinCommon(void)
{
	for(int x = 0; x < SKINCOMMON_SIZE; x++)
		skin[x] = new CSprite();
}

CSkinCommon::~CSkinCommon(void)
{
	for(int x = 0; x < SKINCOMMON_SIZE; x++)
		delete skin[x];
}

int CSkinCommon::LoadSkin(void)
{
	int nDur[SKINCOMMON_SIZE], nFrames[SKINCOMMON_SIZE];
	FILE* hFile;
	TCHAR str[260];
	TCHAR* chArchives[] =
	{
		_T("Mine.png"),
		_T("Potion.png"),
		_T("Heart.png"),
		_T("Flash.png"),
		_T("BackToNormal.png"),
		_T("VelocityX1.png"),
		_T("VelocityX2.png"),
		_T("VelocityX3.png"),
		_T("VelocityX4.png"),
		_T("VelocityX5.png"),
		_T("VelocityX8.png"),
		_T("Drop.png"),
		_T("UnderAttack.png"),
		_T("LeftAttack.png"),
		_T("RightAttack.png"),
		_T("Snake.png"),
		_T("EVA1.png"),
		_T("EVA2.png"),
		_T("EVA3.png"),
		_T("EVA4.png"),
		_T("EVA5.png"),
		_T("Font0.png"),
		_T("Font1.png"),
		_T("Font2.png"),
		_T("Font3.png"),
		_T("Font4.png"),
		_T("Font5.png"),
		_T("Font6.png"),
		_T("Font7.png"),
		_T("Font8.png"),
		_T("Font9.png"),
		_T("BarSkin.png"),
		_T("BarActive.png"),
		_T("Combo.png"),
		_T("ComboMiss.png")
	};
	hFile = _tfopen(_T("skins/common/config.acc\0"), _T("r+"));
	if(hFile == NULL)
	{
		return -1;
	}
	for(int x = 0; x < SKINCOMMON_SIZE; x++)
	{
		_fscanf(hFile, _T("%d\t%d\n"), &nFrames[x], &nDur[x]);
	}
	fclose(hFile);
	for(int x = 0; x < SKINCOMMON_SIZE; x++)
	{
		_sprintf(str, _T("skins/common/%s"), chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	return 0;
}

void CSkinCommon::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SKINCOMMON_SIZE; x++)
		skin[x]->Update(dwMiliseconds);
}

void CSkinCommon::Delete()
{
	for(int x = 0; x < SKINCOMMON_SIZE; x++)
		skin[x]->DeleteImage();
}
