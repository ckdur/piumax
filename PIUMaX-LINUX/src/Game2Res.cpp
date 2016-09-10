#include "stdafx.h"
#include "Game2Res.h"

CBasicGame2Res::CBasicGame2Res(void)
{
	for(int x = 0; x < SIZE_GAME2RES; x++)
		skin[x] = new COGLSprite();
}

CBasicGame2Res::~CBasicGame2Res(void)
{
	for(int x = 0; x < SIZE_GAME2RES; x++)
		delete skin[x];
}

int CBasicGame2Res::LoadSkin(void)
{
	int nDur[SIZE_GAME2RES], nFrames[SIZE_GAME2RES];
	FILE* hFile;
	TCHAR str[260];
	TCHAR chArchives[][260] =
	{
		_T("LatBar"),
		_T("MileageBord"),
		_T("Selector"),
		_T("SelectorGlow"),
		_T("Line"),
		_T("MissionIcon"),
		_T("MissionIconSucess"),
		_T("MissionCond"),
		_T("MissionIconSpecial"),
		_T("ArrowDown"),
		_T("ArrowUp"),
		_T("LeftCha"),
		_T("RightCha"),
		_T("EggDificulty"),
		_T("StarDificulty"),
		_T("SkullDificulty"),
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
		_T("ModEditor"),
		_T("WMGoMod"),
		_T("WMLifeMod"),
		_T("WMSpeedMod"),
		_T("WMVanishMod"),
		_T("WMSkinMod"),
		_T("WMArrowChangeMod"),
		_T("WMArrowViewMod"),
		_T("BannerMileage"),
		_T("LifeLV0"),
		_T("LifeLV1"),
		_T("LifeLV2"),
		_T("LifeLV3"),
		_T("LifeLV4"),
		_T("ArrowMini"),
		_T("Life0"),
		_T("Life1"),
		_T("Life2"),
		_T("Life3"),
		_T("Life4"),
		_T("Life5"),
		_T("Life6"),
		_T("Life7"),
		_T("Life8"),
		_T("NoMileage"),
		_T("TimeTray")
	};
	TCHAR chDir[260] = _T("Game2/");
	_sprintf(str, _T("%lsconfig.acc\0"), chDir);
	hFile = _fopen(str, _T("r"));
	if(hFile == NULL)
	{
        _printf(_T("File not found: %ls\n"), str);
		return 0;
	}
	int nLang[SIZE_GAME2RES];
	TCHAR ch[100];
	for(int x = 0; x < SIZE_GAME2RES; x++)
	{
		if(_fgets(ch, 100, hFile) == NULL)
		{
			fclose(hFile);
			return 0;
		}
		if(_sscanf(ch, _T("%d\t%d\t%d"), &nFrames[x], &nDur[x], &nLang[x]) != 3)
		{
			fclose(hFile);
			return 0;
		}
	}
	fclose(hFile);
	for(int x = 0; x < SIZE_GAME2RES; x++)
	{
		if(nLang[x]) _sprintf(str, _T("%ls%ls_%ls.png"), chDir, chArchives[x], chLang);
		else _sprintf(str, _T("%ls%ls.png"), chDir, chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	return 1;
}

void CBasicGame2Res::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SIZE_GAME2RES; x++)
		skin[x]->Update(dwMiliseconds);
}

void CBasicGame2Res::Delete()
{
	for(int x = 0; x < SIZE_GAME2RES; x++)
		skin[x]->DeleteImage();
}

///////////////////////////////////////////////

CPluGame2Res::CPluGame2Res()
{
	nSize = 0;
}

CPluGame2Res::~CPluGame2Res()
{
	Delete();
}

int CPluGame2Res::LoadSkin(LPCTSTR chFile)
{
	TCHAR chDir[260];
	FILE* hFile;
	hFile = _fopen(chFile, _T("r"));
	if(hFile == NULL)
	{
		return 0;
	}
	TCHAR ch[260];
	CString str;
	while(_fgets(ch, 260, hFile) != NULL)
	{
		str = ( ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		if(str.Find(_T("#DIR:")) != -1)
		{
			str.Remove(_T('\t'));
			str.Delete(0, 5);	// Erase #DIR:
			str += _T("/");
			_strcpy(chDir, str);
		}
		if(str.Find(_T("#SIZE:")) != -1)
		{
			str.Remove(_T(' '));
			if(_sscanf(str, _T("#SIZE:%d"), &nSize) != 1)
			{
				fclose(hFile);
				return 0;
			}
			skin = new COGLSprite*[nSize];
			for(int x = 0; x < nSize; x++)
			{
				skin[x] = new COGLSprite();
				if(_fgets(ch, 260, hFile) == NULL)
				{
					fclose(hFile);
					return 0;
				}
				str = ( ch);
				str.Remove(_T('\n')); str.Remove(_T('\r'));
				int nCode = str.Find(_T(":"));
				CString strFile = str.Mid(nCode+1, str.GetLength()-nCode-1);
				int nFrames = 0, nDur = 0, nLang = 0;
				if(_sscanf(str, _T("%d\t%d\t%d"), &nFrames, &nDur, &nLang) != 3)
				{
					fclose(hFile);
					return 0;
				}
				if(nLang) strFile.Format(_T("%ls%ls_%ls.png"), chDir, strFile, chLang);
				else strFile.Format(_T("%ls%ls.png"), chDir, strFile, chLang);
				skin[x]->InitImage(strFile, nFrames);
				skin[x]->SetDuration(nDur);
			}
		}
	}
	fclose(hFile);
	return 1;
}

void CPluGame2Res::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < nSize; x++)
		skin[x]->Update(dwMiliseconds);
}

void CPluGame2Res::Delete()
{
	if(nSize != 0)
	{
		for(int x = 0; x < nSize; x++)
			delete skin[x];
		delete skin;
		nSize = 0;
	}
}

///////////////////////////////////////////////

CMapGame2Res::CMapGame2Res()
{
	m_nSizeMap = 0;
}

CMapGame2Res::~CMapGame2Res()
{
	DeleteMap();
}

int CMapGame2Res::LoadFile(LPCTSTR chFile)
{
	FILE* hFile;
	hFile = _fopen(chFile, _T("r"));
	if(hFile == NULL)
	{
		return 0;
	}
	TCHAR ch[260];
	CString str;
	while(_fgets(ch, 260, hFile) != NULL)
	{
		str = ( ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		if(str.Find(_T("#ARC:")) != -1)
		{
			str.Remove(_T('\t'));
			str.Delete(0, 5);	// Erase #ARC:
			LoadSkin(str);
		}
		if(str.Find(_T("#SIZE:")) != -1)
		{
			str.Remove(_T(' '));
			if(_sscanf(str, _T("#SIZE:%d"), &m_nSizeMap) != 1)
			{
				fclose(hFile);
				return 0;
			}
			if(m_nSizeMap != nSize)
			{
				m_nSizeMap = 0;
				fclose(hFile);
				return 0;
			}
			dSizeX = new double[m_nSizeMap];
			dSizeY = new double[m_nSizeMap];
			for(int x = 0; x < m_nSizeMap; x++)
			{
				if(_fgets(ch, 260, hFile) == NULL)
				{
					fclose(hFile);
					return 0;
				}
				str = ( ch);
				str.Remove(_T('\n')); str.Remove(_T('\r'));
				float f1, f2;
				if(_sscanf(str, _T("%g,%g"), &f1, &f2) !=2)
				{
					fclose(hFile);
					return 0;
				}
				dSizeX[x] = f1;
				dSizeY[x] = f2;
			}
		}
	}
	fclose(hFile);
	return 1;
}

void CMapGame2Res::DeleteMap()
{
	if(m_nSizeMap)
	{
		delete dSizeX;
		delete dSizeY;
	}
}
void CMapGame2Res::DrawAt(double dx, double dy, int nMap)
{
	if(nMap >= m_nSizeMap) return;
	sx1 = 0, sy1 = 0, sx2 = 0, sy2 = 0;
	sx1 = dx - MAP_EXPANSION_X_DIV2;
	sy1 = dy - MAP_EXPANSION_Y_DIV2;
	sx2 = dx + MAP_EXPANSION_X_DIV2;
	sy2 = dy + MAP_EXPANSION_Y_DIV2;
	dPosX = 0;
	dPosY = 0;
	if(sx1 < 0.0)
	{
		dPosX += sx1;
		sx2 -= sx1;
		sx1 = 0;
	}
	if(sy1 < 0.0)
	{
		dPosY += sy1;
		sy2 -= sy1;
		sy1 = 0;
	}
	if(sx2 > dSizeX[nMap])
	{
		dPosX += sx2-dSizeX[nMap];
		sx1 -= sx2-dSizeX[nMap];
		sx2 = dSizeX[nMap];
	}
	if(sy2 > dSizeY[nMap])
	{
		dPosY += sy2-dSizeY[nMap];
		sy1 -= sy2-dSizeY[nMap];
		sy2 = dSizeY[nMap];
	}
	glBindTexture(GL_TEXTURE_2D, skin[nMap]->GetCurrentTex());
	glBegin(GL_QUADS);
	glTexCoord2d(sx1/dSizeX[nMap], /*1.0-*/sy1/dSizeY[nMap]); glVertex3d(-40.0, -30.0, -50.0);
	glTexCoord2d(sx2/dSizeX[nMap], /*1.0-*/sy1/dSizeY[nMap]); glVertex3d( 40.0, -30.0, -50.0);
	glTexCoord2d(sx2/dSizeX[nMap], /*1.0-*/sy2/dSizeY[nMap]); glVertex3d( 40.0,  30.0, -50.0);
	glTexCoord2d(sx1/dSizeX[nMap], /*1.0-*/sy2/dSizeY[nMap]); glVertex3d(-40.0,  30.0, -50.0);
	glEnd();
}
