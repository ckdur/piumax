#include "stdafx.h"
#include "Game3Res.h"

CGame3Res::CGame3Res(void)
{
	for(int x = 0; x < SIZE_GA3RES; x++)
		skin[x] = new COGLSprite();
}

CGame3Res::~CGame3Res(void)
{
	for(int x = 0; x < SIZE_GA3RES; x++)
		delete skin[x];
}

int CGame3Res::LoadSkin(void)
{
	int nDur[SIZE_GA3RES], nFrames[SIZE_GA3RES];
	FILE* hFile;
	TCHAR str[260];
	TCHAR chArchives[][260] =
	{
		_T("EggDificulty"),
		_T("StarDificulty"),
		_T("SkullDificulty"),
		_T("UpLeftChoice"),
		_T("UpRightChoice"),
		_T("DownLeftChoice"),
		_T("DownRightChoice"),
		_T("GraphBase"),
		_T("GraphBright"),
		_T("GraphStar"),
		_T("GraphLine"),
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
		_T("LeftCha1"),
		_T("LeftCha2"),
		_T("LeftCha3"),
		_T("LeftCha4"),
		_T("LeftCha5"),
		_T("LeftCha6"),
		_T("LeftCha7"),
		_T("LeftCha8"),
		_T("LeftCha9"),
		_T("LeftBrightCha1"),
		_T("LeftBrightCha2"),
		_T("LeftBrightCha3"),
		_T("LeftBrightCha4"),
		_T("LeftBrightCha5"),
		_T("LeftBrightCha6"),
		_T("LeftBrightCha7"),
		_T("LeftBrightCha8"),
		_T("LeftBrightCha9"),
		_T("RightCha1"),
		_T("RightCha2"),
		_T("RightCha3"),
		_T("RightCha4"),
		_T("RightCha5"),
		_T("RightCha6"),
		_T("RightCha7"),
		_T("RightCha8"),
		_T("RightCha9"),
		_T("RightBrightCha1"),
		_T("RightBrightCha2"),
		_T("RightBrightCha3"),
		_T("RightBrightCha4"),
		_T("RightBrightCha5"),
		_T("RightBrightCha6"),
		_T("RightBrightCha7"),
		_T("RightBrightCha8"),
		_T("RightBrightCha9"),
		_T("EggDificultyOFF"),
		_T("StarDificultyOFF"),
		_T("SkullDificultyOFF"),
		_T("LevelEZ"),
		_T("LevelHD"),
		_T("LevelCZ"),
		_T("LevelFS"),
		_T("LevelNM"),
		_T("LevelEZ_OFF"),
		_T("LevelHD_OFF"),
		_T("LevelCZ_OFF"),
		_T("LevelFS_OFF"),
		_T("LevelNM_OFF"),
		_T("SelectorModeP1"),
		_T("SelectorModeP2"),
		_T("DificultyNumbers"),
		_T("PresetBar"),
		_T("PresetGlouth1"),
		_T("PresetGlouth2"),
		_T("PresetGlouth3"),
		_T("PresetCommon"),
		_T("DificultyRack"),
		_T("DificultyUnexpected"),
		_T("PresetX2"),	//1st
		_T("PresetX3"),
		_T("PresetX4"),
		_T("PresetX5"),
		_T("PresetX8"),
		_T("PresetEW"),
		_T("PresetRV"),
		_T("PresetAC"),
		_T("PresetDC"),
		_T("PresetV"),	//2nd
		_T("PresetNS"),
		_T("PresetFL"),
		_T("PresetFD"),
		_T("PresetRS"),	// 4th
		_T("PresetM"),
		_T("PresetRG"),
		_T("PresetT"),
		_T("PresetHJ"),
		_T("PresetSN"),	// 5th
		_T("PresetNXMode"),
		_T("PresetXMode"),
		_T("PresetUA"),
		_T("PresetDR"),
		_T("PresetRSK"),	// 3rd
		_T("PresetSkin01"),
		_T("PresetSkin02"),
		_T("PresetSkin03"),
		_T("PresetSkin04"),
		_T("PresetSkin05"),
		_T("PresetSkin06"),
		_T("PresetSkin07"),
		_T("PresetSkin08"),
		_T("PresetSkin09"),
		_T("PresetSkin10"),
		_T("PresetSkin11"),
		_T("PresetSkin12"),
		_T("PresetSkin13"),
		_T("PresetSkin14"),
		_T("PresetSkin15"),
		_T("PresetSkin16"),
		_T("PresetSkin17"),
		_T("PresetSkin18"),
		_T("PresetSkin19"),
		_T("PresetSkin20"),
		_T("PresetPJR"),	// 6th
		_T("PresetPJ01"),
		_T("PresetPJ02"),
		_T("PresetPJ03"),
		_T("PresetPJ04"),
		_T("PresetPJ05"),
		_T("PresetPJ06"),
		_T("PresetPJ07"),
		_T("PresetPJ08"),
		_T("PresetPJ09"),
		_T("PresetPJ10"),
		_T("PresetPJ11"),
		_T("PresetPJ12"),
		_T("CoverNames"),
		_T("SkinBorder"),
		_T("SkinSplash"),
		_T("SkinNo"),
		_T("CharacterBorder"),
		_T("CharacterSplash"),
		_T("CharacterNo"),
		_T("Life0"),
		_T("Life1"),
		_T("Life2"),
		_T("Life3"),
		_T("Life4"),
		_T("Life5"),
		_T("Life6"),
		_T("Life7"),
		_T("Life8")
	};
	TCHAR chDir[260] = _T("Game3/");
	_sprintf(str, _T("%lsconfig.acc"), chDir);
	hFile = _fopen(str, _T("r"));
	if(hFile == NULL)
	{
        _printf(_T("File not found: %ls\n"), str);
		return 0;
	}
	int nLang[SIZE_GA3RES];
	TCHAR ch[100];
	for(int x = 0; x < SIZE_GA3RES; x++)
	{
		if(_fgets(ch, 100, hFile) == NULL) {fclose(hFile); return 0;}
		if(_sscanf(ch, _T("%d\t%d\t%d"), &nFrames[x], &nDur[x], &nLang[x]) != 3) {fclose(hFile); return 0;}
	}
	fclose(hFile);
	for(int x = 0; x < SIZE_GA3RES; x++)
	{
		if(nLang[x]) _sprintf(str, _T("%ls%ls_%ls.png"), chDir, chArchives[x], chLang);
		else _sprintf(str, _T("%ls%ls.png"), chDir, chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	return 1;
}

void CGame3Res::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SIZE_GA3RES; x++)
		skin[x]->Update(dwMiliseconds);
}

void CGame3Res::Delete()
{
	for(int x = 0; x < SIZE_GA3RES; x++)
		skin[x]->DeleteImage();
}
