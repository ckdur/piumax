#include "game2.h"
#include "stepeval.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "Game2Res.h"
#include "USBsys.h"
#include "MissionLoader.h"
#include "MWayLoader.h"
#include "Anu.h"
#include "UnlockSys.h"
#include "History.h"

#define CHOOSES_GAME2 2
#define SIZE_MISION_SELECTOR 2.5
#define MAX_COMMANDS_GAME2 12
#define TIME_START_LR_GAME2 0
#define TIME_TRANSI_LR_GAME2 500
#define TIME_GLOUTH_LR_GAME2 500

int nInterGame2 = 0;
int nMap = 0;
GLuint texbg_game2;
CMissionLoader* mission;
CMWayLoader* way;
CBasicGame2Res* ga2res;
CMapGame2Res* ga2mapres;
CPluGame2Res* ga2iconres;
bool bActiveSongGame2 = false;

bool bRepAnu;
__int64 nRepAnu;
__int64 nDurRepAnu;
int nInRepAnu;

bool bLinkOther = false;
int nLinkOther = 0;

int nAPGame2 = 0;	// Active player
bool bAd[4];
double dAngAd[4];

int nSizeAnuMission = 0;
CAnu** anuMission;
int* nRefAnuMission;
FMOD_SOUND** soundAnuMission;
bool* bIsSoundAnuMission;

FMOD_SOUND* sound_wmove;
FMOD_SOUND* sound_wchoose;
FMOD_SOUND* sound_wstop;

bool bMusicCurMap = false;
FMOD_SOUND* music_curMap;

CString strCurMap;

__int64 nCountGame2 = 0;
__int64 nTimeChangeGame2 = 0;
double dPosX;
double dPosY;
double dBackPosX;
double dBackPosY;
double dx = 0, dy = 0;
int nCurMissionGame2 = 0;
__int64 nTimeWayGame2 = 0;
bool bContinueUsbGame2;

double* dEsfWayPar;	// Parametros esfericos para los caminos

int nComandGame2[2][MAX_COMMANDS_GAME2];
bool bEnArrowsGame2[5];
__int64 nTimeEnArrowsGame2[5];

int nChosesGame2 = 0;
__int64 nTimeChosesGame2[CHOOSES_GAME2];

__int64 nMaxGame2 = 0;

__int64 nTimeLastPresetGame2;
int nIndLastPresetGame2;
int nIndPresetGame2[30];
int nIndSizePresetGame2[6];

int nCurModEditGame2 = 0;
int nDirModEditGame2 = 0;
int nCurStateModEditGame2[7];
__int64 nTimeChangeModGame2 = 0;
__int64 nTimeModEditGame2 = 0;

int nReqMileage = 0;

__int64 nTimeUnlockedGame2;
int nModeUnlockedGame2;

CString strCurNameSongGame2 = _T("");
CString strCurArtistSongGame2 = _T("");

int FoundPresetGame2(int nFamily, int nBegin, int nEnd);

CString GetNameSongsGame2(int nIndex)
{
	if(nIndex >= mission->m_nSize) return CString(_T("No song"));
	if(mission->m_nSizeSong[nIndex] <= 0) return CString(_T(""));
	CString str;
	str.Format(_T("song/%ls/%ls_%ls.acc"), mission->m_strSong[nIndex][0], mission->m_strSong[nIndex][0], chLang);
	FILE* hFile = _fopen(str, _T("r"));
	if(hFile == NULL) return CString(_T("File not found"));
	int nCaret = 0;
	TCHAR ch[260];
	while(_fgets(ch, 260, hFile) != NULL && nCaret < 1)
	{
		str = ( ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		if(!str.IsEmpty()) nCaret++;
	}
	fclose(hFile);
	if(nCaret != 1) return CString(_T("Format error"));
	return str;
}

CString GetArtistSongsGame2(int nIndex)
{
	if(nIndex >= mission->m_nSize) return CString(_T("No song"));
	if(mission->m_nSizeSong[nIndex] <= 0) return CString(_T(""));
	CString str;
	str.Format(_T("song/%ls/%ls_%ls.acc"), mission->m_strSong[nIndex][0], mission->m_strSong[nIndex][0], chLang);
	FILE* hFile = _fopen(str, _T("r"));
	if(hFile == NULL) return CString(_T("File not found"));
	int nCaret = 0;
	TCHAR ch[260];
	while(_fgets(ch, 260, hFile) != NULL && nCaret < 2)
	{
		str = ( ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		if(!str.IsEmpty()) nCaret++;
	}
	fclose(hFile);
	if(nCaret != 2) return CString(_T("Format error"));
	return str;
}

CString GetBPMSongsGame2(int nIndex)
{
	if(nIndex >= mission->m_nSize) return CString(_T("No song"));
	if(mission->m_nSizeSong[nIndex] <= 0) return CString(_T(""));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), mission->m_strSong[nIndex][0].c_str(), mission->m_strSong[nIndex][0].c_str(), chLang);
	FILE* hFile = _fopen(chFile, _T("r"));
	if(hFile == NULL) return CString(_T("File not found"));
	int nCaret = 0;
	TCHAR ch[260];
	CString str;
	while(_fgets(ch, 260, hFile) != NULL && nCaret < 3)
	{
		str = ( ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		if(!str.IsEmpty()) nCaret++;
	}
	fclose(hFile);
	if(nCaret != 3) return CString(_T("Format error"));
	return str;
}

int LoadSongGame2()
{
	HideSesionTrayMileage(nAPGame2);
	HideSesionTrayExp(nAPGame2);
	// ** Cargar datos basicos
	ARGSTEP arg;
	int nSong = rand()%mission->m_nSizeSong[nCurMissionGame2], nStep = rand()%mission->m_nSizeStep[nCurMissionGame2];
	arg.strMusic.Format(_T("song/%ls/%ls.mp3"), mission->m_strSong[nCurMissionGame2][nSong], mission->m_strSong[nCurMissionGame2][nSong]);
	arg.strLights.Format(_T("song/%ls/%ls.lgt"), mission->m_strSong[nCurMissionGame2][nSong], mission->m_strSong[nCurMissionGame2][nSong]);
	arg.strBGA.Format(_T("song/%ls/%ls.avi"), mission->m_strSong[nCurMissionGame2][nSong], mission->m_strSong[nCurMissionGame2][nSong]);
	CString str;
	TCHAR ch[260];
	str.Format(_T("song/%ls/%ls.acc"), mission->m_strSong[nCurMissionGame2][nSong], mission->m_strSong[nCurMissionGame2][nSong]);
	FILE* hFile = _fopen(str, _T("r"));
	if(hFile != NULL)
	{
		if( _fgets(ch, 260, hFile) != NULL)
		{
			arg.strBGA.Format(_T("song/%ls/%ls"), mission->m_strSong[nCurMissionGame2][nSong], ch);
			if(arg.strBGA.Find(_T('[')) != -1)
				arg.strBGA.Format(_T("song/GNRBGA/%ls"), ch);
			arg.strBGA.Remove(_T('\n')); arg.strBGA.Remove(_T('\r'));
		}
		else arg.strBGA.Format(_T("song/%ls/%ls.avi"), mission->m_strSong[nCurMissionGame2][nSong], mission->m_strSong[nCurMissionGame2][nSong]);
		fclose(hFile);
	}
	else arg.strBGA.Format(_T("song/%ls/%ls.avi"), mission->m_strSong[nCurMissionGame2][nSong], mission->m_strSong[nCurMissionGame2][nSong]);
	if(player_sesion[nAPGame2]->nLockedAsp[ITEM_BGAOFF])
	{
		arg.strBGA = _T("");
	}
	arg.strSplash.Format(_T("song/%ls/%ls.png"), mission->m_strSong[nCurMissionGame2][nSong], mission->m_strSong[nCurMissionGame2][nSong]);
	arg.bAutoPlay = false;
	if(!LoadSong(arg)) return 0;
	arg.strFile.Format(_T("song/%ls/%ls_%ls.sksf"), mission->m_strSong[nCurMissionGame2][nSong], mission->m_strSong[nCurMissionGame2][nSong], mission->m_strStep[nCurMissionGame2][nStep]);
	arg.dUserVel = 1.0;
	arg.bIsSingle = !mission->m_bDouble[nCurMissionGame2];
	arg.bAutoPlay = false;
	arg.nSkin = -1;
	arg.bLoadBS = false;
	arg.bTrainingMode = false;
	arg.bXMode = false;
	arg.bNXMode = false;
	arg.bHJ = false;
	arg.bRS = false;
	arg.bMirror = false;
	arg.bFreedom = false;
	arg.bRV = false;
	arg.bRandomSkin = false;
	arg.nVanish = -1;
	arg.bDC = false;
	arg.bAC = false;
	arg.bEW = false;
	arg.bFlash = false;
	arg.bUnderAttack = false;
	arg.bLeftAttack = false;
	arg.bRightAttack = false;
	arg.bSnake = false;
	arg.nCharacter = 0;
	arg.bCharacter = false;
	arg.nLevelCharacer = 0;
	arg.dIncrBar = mission->m_dIncrBar[nCurMissionGame2]/(nCurStateModEditGame2[1]==0?1.0:double(nCurStateModEditGame2[1]));
	arg.dDecrBar = mission->m_dDecrBar[nCurMissionGame2]/(nCurStateModEditGame2[1]==0?1.0:double(nCurStateModEditGame2[1]));
	switch(piuconfig.nLevelMach)
	{
	case 0:
		arg.dPowTimer = POW_TIMER_EASY;
		break;
	case 1:
		arg.dPowTimer = POW_TIMER_NORMAL;
		break;
	case 2:
		arg.dPowTimer = POW_TIMER_HARD;
		break;
	default:
		arg.dPowTimer = POW_TIMER_EASY;
		break;
	}
	arg.nLevelMach = TIMING_HARD;
	arg.bDrop = false;
	int nCode = 0;
	nCode = FoundPresetGame2(0, 1, 7);
	if(nCode == -1)
		arg.dUserVel = 1.0;
	else
	{
		switch(nIndPresetGame2[nCode])	// 0*5+nCode
		{
		case 1:
			arg.dUserVel = 2.0;
			break;
		case 2:
			arg.dUserVel = 3.0;
			break;
		case 3:
			arg.dUserVel = 4.0;
			break;
		case 4:
			arg.dUserVel = 5.0;
			break;
		case 5:
			arg.dUserVel = 8.0;
			break;
		case 6:
			arg.bEW = true;
			break;
		case 7:
			arg.bRV = true;
			break;
		}
	}
	nCode = FoundPresetGame2(0, 8, 9);
	if(nCode != -1)
	{
		switch(nIndPresetGame2[nCode])	// 0*5+nCode
		{
		case 8:
			arg.bAC = true;
			break;
		case 9:
			arg.bDC = true;
			break;
		}
	}
	// Familia 1
	nCode = FoundPresetGame2(1, 10, 11);
	if(nCode != -1)
	{
		switch(nIndPresetGame2[5+nCode])	// 0*5+nCode
		{
		case 10:
			arg.nVanish = 5;
			break;
		case 11:
			arg.nVanish = 11;
			break;
		}
	}
	if(FoundPresetGame2(1, 12, 12) != -1) arg.bFlash = true;
	if(FoundPresetGame2(1, 13, 13) != -1) arg.bFreedom = true;
	// Familia 2
	nCode = FoundPresetGame2(2, 24, 45);
	if(nCode != -1)
	{
		switch(nIndPresetGame2[10+nCode])
		{
		case 24:
			arg.bRandomSkin = true;
			break;
		default:
			arg.nSkin = nIndPresetGame2[10+nCode]-24;
			break;
		}
	}
	// Familia 5
	nCode = FoundPresetGame2(5, 46, 59);
	if(nCode != -1)
	{
		switch(nIndPresetGame2[25+nCode])
		{
		case 46:
			arg.bCharacter = true;
			arg.nCharacter = (rand()%12)+1;
			arg.nLevelCharacer = 20; //????
			break;
		default:
			arg.bCharacter = true;
			arg.nCharacter = nIndPresetGame2[25+nCode]-46;
			arg.nLevelCharacer = 20; //????
			break;
		}
	}
	// Familia 3
	nCode = FoundPresetGame2(3, 14, 15);
	if(nCode != -1)
	{
		switch(nIndPresetGame2[15+nCode])	// 0*5+nCode
		{
		case 14:
			arg.bRS = true;
			break;
		case 15:
			arg.bMirror = true;
			break;
		}
	}
	//if(FoundPresetGame2(3, 16, 16) != -1);
	if(FoundPresetGame2(3, 17, 17) != -1) arg.bTrainingMode = true;
	if(FoundPresetGame2(3, 18, 18) != -1) arg.dPowTimer = POW_TIMER_HARD;
	// Familia 4
	if(FoundPresetGame2(4, 19, 19) != -1) arg.bSnake = true;
	if(FoundPresetGame2(4, 20, 20) != -1) arg.bNXMode = true;
	if(FoundPresetGame2(4, 21, 21) != -1) arg.bXMode = true;
	nCode = FoundPresetGame2(4, 22, 23);
	if(nCode != -1)
	{
		switch(nIndPresetGame2[20+nCode])	// 0*5+nCode
		{
		case 22:
			arg.bUnderAttack = true;
			break;
		case 23:
			arg.bDrop= true;
			break;
		}
	}
	if(!LoadSongCompile(arg, nAPGame2+1)) return 0;
	if(!nAPGame2)
	{
		viewp1->status.dLife = min(mission->m_dInitialBar[nCurMissionGame2]+double(nCurStateModEditGame2[1])/10.0+(player_sesion[nAPGame2]->nLockedAsp[ITEM_LIFE]?0.2:0.0), 1.0);
	}
	else
	{
		viewp2->status.dLife = min(mission->m_dInitialBar[nCurMissionGame2]+double(nCurStateModEditGame2[1])/10.0+(player_sesion[nAPGame2]->nLockedAsp[ITEM_LIFE]?0.2:0.0), 1.0);
	}
	if(mission->m_uVersus[nCurMissionGame2] && !mission->m_bDouble[nCurMissionGame2])
	{
		arg.bCharacter = true;
		arg.nCharacter = mission->m_uVersus[nCurMissionGame2];
		arg.nLevelCharacer = mission->m_uLvlVersus[nCurMissionGame2];
		arg.bAutoPlay = true;
		if(!LoadSongCompile(arg, int(!nAPGame2)+1)) return 0;
		if(nAPGame2)
		{
			viewp1->status.dLife = mission->m_dInitialBar[nCurMissionGame2];
		}
		else
		{
			viewp2->status.dLife = mission->m_dInitialBar[nCurMissionGame2];
		}
	}
	nSizeCondSucess = mission->m_nSizeCond[nCurMissionGame2];
	if(nSizeCondSucess)
	{
		strCondSucess = new CString[nSizeCondSucess];
		for(int x = 0; x < nSizeCondSucess; x++)
		{
			strCondSucess[x] = mission->m_strCond[nCurMissionGame2][x];
		}
	}
	bActiveSongGame2 = true;
	return 1;
}

void DeleteSongGame2()
{
	if(bActiveSongGame2)
	{
		DeleteSongCompile(nAPGame2+1);
		if(g_bPlayer[int(!nAPGame2)]) DeleteSongCompile(int(!nAPGame2)+1);
		DeleteSong();
		bActiveSongGame2 = false;
		if(nSizeCondSucess)
		{
			delete[] strCondSucess;
			nSizeCondSucess = 0;
		}
	}
}

int FoundPresetGame2(int nFamily, int nBegin, int nEnd)
{
	int nFound = -1;
	for(int k = 0; k < nIndSizePresetGame2[nFamily]; k++)
	{
		if(nIndPresetGame2[nFamily*5+k] >= nBegin && nIndPresetGame2[nFamily*5+k] <= nEnd)
		{
			nFound = k;
			break;
		}
	}
	return nFound;
}

void ToogleMovMod(bool bIsIzq)
{
	if(bIsIzq)
	{
		if(nCurModEditGame2<=0) return;
		nCurModEditGame2--;
		nTimeModEditGame2 = nCountGame2;
		nDirModEditGame2 = 1;
	}
	else
	{
		if(nCurModEditGame2>=6) return;
		nCurModEditGame2++;
		nTimeModEditGame2 = nCountGame2;
		nDirModEditGame2 = 2;
	}
	nTimeChangeModGame2 = nCountGame2;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
}

#define VALUE_LIFE 50
#define VALUE_SPEED 45
#define VALUE_MOD 60

void ToogleChangeMod()
{
	nCurStateModEditGame2[nCurModEditGame2]++;
	switch(nCurModEditGame2)
	{
	case 1:	// Life
		nReqMileage += VALUE_LIFE;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 5)
		{
			nCurStateModEditGame2[nCurModEditGame2] = 0;
			nReqMileage-=VALUE_LIFE*5;
		}
		break;
	case 2:
		if(nCurStateModEditGame2[nCurModEditGame2] == 1) nReqMileage+=VALUE_SPEED;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 7) nReqMileage-=VALUE_SPEED;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 7)
			nCurStateModEditGame2[nCurModEditGame2] = 0;
		if(!nCurStateModEditGame2[nCurModEditGame2])
		{
			for(int x = 0; x < mission->m_nSizeMod[nCurMissionGame2]; x++)
			{
				int nMod = mission->m_nMod[nCurMissionGame2][x];
				int nFamily = 0;
				int nInf = 0, nSup = 0;
				if(nMod >= 1 && nMod <= 7) {nInf = 1; nSup = 7; nFamily = 0;}
				if(nMod >= 8 && nMod <= 9) {nInf = 8; nSup = 9; nFamily = 0;}
				int nFound = FoundPresetGame2(nFamily, nInf, nSup);
				if(nFound == -1 && nInf != 0)
				{
					nFound = nIndSizePresetGame2[nFamily];
					nIndSizePresetGame2[nFamily]++;
				}
				if(nInf != 0)
				{
					nIndPresetGame2[nFamily*5+nFound] = nMod;
					nIndLastPresetGame2 = nFamily*5+nFound;
					nTimeLastPresetGame2 = nCountGame2;
				}
			}
		}
		else
		{
			int nFound, nFamily = 0;
			for(nFound = 0; nFound < 5; nFound++)
			{
				nIndPresetGame2[nFamily*5+nFound] = 0;
			}
			if(nCurStateModEditGame2[nCurModEditGame2] == 1) nIndSizePresetGame2[nFamily] = 0;
			else
			{
				nIndSizePresetGame2[nFamily] = 1;
				nFound = 0;
				nIndPresetGame2[nFamily*5+nFound] = nCurStateModEditGame2[nCurModEditGame2]-1;
				nIndLastPresetGame2 = nFamily*5+nFound;
				nTimeLastPresetGame2 = nCountGame2;
			}
		}
		break;
	case 3:
		if(nCurStateModEditGame2[nCurModEditGame2] == 1) nReqMileage+=VALUE_MOD;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 2) nReqMileage-=VALUE_MOD;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 2)
			nCurStateModEditGame2[nCurModEditGame2] = 0;
		if(!nCurStateModEditGame2[nCurModEditGame2])
		{
			for(int x = 0; x < mission->m_nSizeMod[nCurMissionGame2]; x++)
			{
				int nMod = mission->m_nMod[nCurMissionGame2][x];
				int nFamily = 0;
				int nInf = 0, nSup = 0;
				if(nMod >= 10 && nMod <= 11) {nInf = 10; nSup = 11; nFamily = 1;}
				if(nMod == 12) {nInf = 12; nSup = 12; nFamily = 1;}
				if(nMod == 13) {nInf = 13; nSup = 13; nFamily = 1;}
				int nFound = FoundPresetGame2(nFamily, nInf, nSup);
				if(nFound == -1 && nInf != 0)
				{
					nFound = nIndSizePresetGame2[nFamily];
					nIndSizePresetGame2[nFamily]++;
				}
				if(nInf != 0)
				{
					nIndPresetGame2[nFamily*5+nFound] = nMod;
					nIndLastPresetGame2 = nFamily*5+nFound;
					nTimeLastPresetGame2 = nCountGame2;
				}
			}
		}
		else
		{
			int nFound, nFamily = 1;
			for(nFound = 0; nFound < 5; nFound++)
			{
				nIndPresetGame2[nFamily*5+nFound] = 0;
			}
			nIndSizePresetGame2[nFamily] = 0;
		}
		break;
	/*case 4:
		if(nCurStateModEditGame2[nCurModEditGame2] == 1) nReqMileage+=VALUE_MOD;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 2) nReqMileage-=VALUE_MOD;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 2)
			nCurStateModEditGame2[nCurModEditGame2] = 0;
		if(!nCurStateModEditGame2[nCurModEditGame2])
		{
			for(int x = 0; x < mission->m_nSizeMod[nCurMissionGame2]; x++)
			{
				int nMod = mission->m_nMod[nCurMissionGame2][x];
				int nFamily = 0;
				int nInf = 0, nSup = 0;
				if(nMod >= 10 && nMod <= 11) {nInf = 10; nSup = 11; nFamily = 1;}
				if(nMod == 12) {nInf = 12; nSup = 12; nFamily = 1;}
				if(nMod == 13) {nInf = 13; nSup = 13; nFamily = 1;}
				int nFound = FoundPresetGame2(nFamily, nInf, nSup);
				if(nFound == -1 && nInf != 0)
				{
					nFound = nIndSizePresetGame2[nFamily];
					nIndSizePresetGame2[nFamily]++;
				}
				if(nInf != 0)
				{
					nIndPresetGame2[nFamily*5+nFound] = nMod;
					nIndLastPresetGame2 = nFamily*5+nFound;
					nTimeLastPresetGame2 = nCountGame2;
				}
			}
		else
		{
			int nFound, nFamily = 1;
			for(nFound = 0; nFound < 5; nFound++)
			{
				nIndPresetGame2[nFamily*5+nFound] = 0;
			}
			nIndSizePresetGame2[nFamily] = 0;
		}
		break;*/
	case 5:
		if(nCurStateModEditGame2[nCurModEditGame2] == 1) nReqMileage+=VALUE_MOD;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 2) nReqMileage-=VALUE_MOD;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 2)
			nCurStateModEditGame2[nCurModEditGame2] = 0;
		if(!nCurStateModEditGame2[nCurModEditGame2])
		{
			for(int x = 0; x < mission->m_nSizeMod[nCurMissionGame2]; x++)
			{
				int nMod = mission->m_nMod[nCurMissionGame2][x];
				int nFamily = 0;
				int nInf = 0, nSup = 0;
				if(nMod >= 14 && nMod <= 15) {nInf = 14; nSup = 15; nFamily = 3;}
				if(nMod == 16) {nInf = 16; nSup = 16; nFamily = 3;}
				if(nMod == 17) {nInf = 17; nSup = 17; nFamily = 3;}
				if(nMod == 18) {nInf = 18; nSup = 18; nFamily = 3;}
				int nFound = FoundPresetGame2(nFamily, nInf, nSup);
				if(nFound == -1 && nInf != 0)
				{
					nFound = nIndSizePresetGame2[nFamily];
					nIndSizePresetGame2[nFamily]++;
				}
				if(nInf != 0)
				{
					nIndPresetGame2[nFamily*5+nFound] = nMod;
					nIndLastPresetGame2 = nFamily*5+nFound;
					nTimeLastPresetGame2 = nCountGame2;
				}
			}
		}
		else
		{
			int nFound, nFamily = 3;
			for(nFound = 0; nFound < 5; nFound++)
			{
				nIndPresetGame2[nFamily*5+nFound] = 0;
			}
			nIndSizePresetGame2[nFamily] = 0;
		}
		break;
	case 6:
		if(nCurStateModEditGame2[nCurModEditGame2] == 1) nReqMileage+=VALUE_MOD;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 2) nReqMileage-=VALUE_MOD;
		if(nCurStateModEditGame2[nCurModEditGame2] >= 2)
			nCurStateModEditGame2[nCurModEditGame2] = 0;
		if(!nCurStateModEditGame2[nCurModEditGame2])
		{
			for(int x = 0; x < mission->m_nSizeMod[nCurMissionGame2]; x++)
			{
				int nMod = mission->m_nMod[nCurMissionGame2][x];
				int nFamily = 0;
				int nInf = 0, nSup = 0;
				if(nMod == 19) {nInf = 19; nSup = 19; nFamily = 4;}
				if(nMod == 20) {nInf = 20; nSup = 20; nFamily = 4;}
				if(nMod == 21) {nInf = 21; nSup = 21; nFamily = 4;}
				if(nMod >= 22 && nMod <= 23) {nInf = 22; nSup = 23; nFamily = 4;}
				int nFound = FoundPresetGame2(nFamily, nInf, nSup);
				if(nFound == -1 && nInf != 0)
				{
					nFound = nIndSizePresetGame2[nFamily];
					nIndSizePresetGame2[nFamily]++;
				}
				if(nInf != 0)
				{
					nIndPresetGame2[nFamily*5+nFound] = nMod;
					nIndLastPresetGame2 = nFamily*5+nFound;
					nTimeLastPresetGame2 = nCountGame2;
				}
			}
		}
		else
		{
			int nFound, nFamily = 4;
			for(nFound = 0; nFound < 5; nFound++)
			{
				nIndPresetGame2[nFamily*5+nFound] = 0;
			}
			nIndSizePresetGame2[nFamily] = 0;
		}
		break;
	}
	nTimeChangeModGame2 = nCountGame2;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
}

void TraduceModCurMission()
{
	nIndLastPresetGame2 = -1;	// Show all
	nTimeLastPresetGame2 = nCountGame2+(TIME_START_LR_GAME2+TIME_TRANSI_LR_GAME2);
	for(int y = 0; y < 6; y++)
	{
		nIndSizePresetGame2[y] = 0;
		nIndPresetGame2[y*5] = 0;
		nIndPresetGame2[y*5+1] = 0;
		nIndPresetGame2[y*5+2] = 0;
		nIndPresetGame2[y*5+3] = 0;
		nIndPresetGame2[y*5+4] = 0;
	}
	for(int x = 0; x < mission->m_nSizeMod[nCurMissionGame2]; x++)
	{
		int nMod = mission->m_nMod[nCurMissionGame2][x];
		int nFamily = 0;
		int nInf = 0, nSup = 0;
		if(nMod >= 1 && nMod <= 7) {nInf = 1; nSup = 7; nFamily = 0;}
		if(nMod >= 8 && nMod <= 9) {nInf = 8; nSup = 9; nFamily = 0;}
		if(nMod >= 10 && nMod <= 11) {nInf = 10; nSup = 11; nFamily = 1;}
		if(nMod == 12) {nInf = 12; nSup = 12; nFamily = 1;}
		if(nMod == 13) {nInf = 13; nSup = 13; nFamily = 1;}
		if(nMod >= 14 && nMod <= 15) {nInf = 14; nSup = 15; nFamily = 3;}
		if(nMod == 16) {nInf = 16; nSup = 16; nFamily = 3;}
		if(nMod == 17) {nInf = 17; nSup = 17; nFamily = 3;}
		if(nMod == 18) {nInf = 18; nSup = 18; nFamily = 3;}
		if(nMod == 19) {nInf = 19; nSup = 19; nFamily = 4;}
		if(nMod == 20) {nInf = 20; nSup = 20; nFamily = 4;}
		if(nMod == 21) {nInf = 21; nSup = 21; nFamily = 4;}
		if(nMod >= 22 && nMod <= 23) {nInf = 22; nSup = 23; nFamily = 4;}
		int nFound = FoundPresetGame2(nFamily, nInf, nSup);
		if(nFound == -1 && nInf != 0)
		{
			nFound = nIndSizePresetGame2[nFamily];
			nIndSizePresetGame2[nFamily]++;
		}
		if(nInf != 0) nIndPresetGame2[nFamily*5+nFound] = nMod;
	}
}

void DeclareMakeAnu(void)
{
	if(mission->m_strGo[nCurMissionGame2].IsEmpty()) return;
	for(int x = 0; x < nSizeAnuMission; x++)
	{
		if(nRefAnuMission[x] == nCurMissionGame2)
		{
			nInRepAnu = x;
			anuMission[x]->SetZeroCount();
			nDurRepAnu = anuMission[x]->anu->nDur;
			nRepAnu = nCountGame2;
			if(bIsSoundAnuMission[x])
				FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, soundAnuMission[x], 0, &channels[9]);
			bRepAnu = true;
			break;
		}
	}
}

void CheckWayUser(void)
{
	int n1 = nCurMissionGame2;
	for(int x = 0; x < 4; x++)
	{
		int n2 = mission->SearchID(mission->m_uPath[n1*4+x]);
		int nWay = way->FoundWay(mission->m_uID[n1], mission->m_uID[n2]);
		if(n2 != -1 && mission->m_uPath[n1*4+x] != 0 && nWay != -1 && (way->m_bDraw[nWay] || player_sesion[nAPGame2]->nLockedAsp[ITEM_SEARCHLINES]))
		{
			if(player_sesion[nAPGame2]->nUnlockedMission[n2] && player_sesion[nAPGame2]->nUnlockedWay[nWay])
			{
				double dx = (mission->m_dUbiX[n2]-mission->m_dUbiX[n1]);
				double dy = (mission->m_dUbiY[n2]-mission->m_dUbiY[n1]);
				if(dx == 0) dAngAd[x] = dy/abs(dy)*90;
				else dAngAd[x] = (dx<0?180:0)+atan(dy / dx)*180/M_PI;
				bAd[x] = true;
			}
			else
			{
				bAd[x] = false;
				dAngAd[x] = 0.0;
			}
		}
		else
		{
			bAd[x] = false;
			dAngAd[x] = 0.0;
		}
	}
}

void ChangeMission(int nDir)
{
	// ** 0 DL, 1 UL, 2 UR, 3 DR
	if(nDir < 0 || nDir > 3)
	{
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)12, sound_wstop, 0, &channels[12]);
		return;
	}
	unsigned int nLink = mission->m_uPath[nCurMissionGame2*4+nDir];
	int n1 = mission->SearchID(nLink);
	int nIndex = way->FoundWay(nLink, mission->m_uID[nCurMissionGame2]);
	if(nIndex == -1 || n1 == -1)
	{
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)12, sound_wstop, 0, &channels[12]);
		return;
	}
	// Asegurar desbloqueo de la linea
	if(!player_sesion[nAPGame2]->nUnlockedWay[nIndex] || !player_sesion[nAPGame2]->nUnlockedMission[n1])
	{
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)12, sound_wstop, 0, &channels[12]);
		return;
	}
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)10, sound_wmove, 0, &channels[10]);
	// Hacer el camino
	dBackPosX = dx;
	dBackPosY = dy;
	nCurMissionGame2 = n1;
	CheckWayUser();
	dPosX = mission->m_dUbiX[nCurMissionGame2];
	dPosY = mission->m_dUbiY[nCurMissionGame2];
	nMap = mission->m_uMap[nCurMissionGame2];
	strCurNameSongGame2 = GetNameSongsGame2(nCurMissionGame2);
	strCurArtistSongGame2 = GetArtistSongsGame2(nCurMissionGame2);
	nTimeWayGame2 = nCountGame2;
}

/*void DeleteSongGame2()
{
	for(int x = 0; x < 2; x++) if(np[x])
	{
		DeleteSongCompile(x+1);
	}
	DeleteSong();
}*/
#define SIZE_ICON2 4.0
#define SIZE_NICON2 2.0
#define SIZE_LINE2 0.1
void DrawMission()
{
	int nb = nAPGame2;
	for(int x = 0; x < way->m_nSizeWay; x++)
	{
		int n1 = mission->SearchID(way->m_uLink[x*2]);
		int n2 = mission->SearchID(way->m_uLink[x*2+1]);
		if((way->m_bDraw[x] || player_sesion[nAPGame2]->nLockedAsp[ITEM_SEARCHLINES]) &&
			player_sesion[nb]->nUnlockedMission[n1] && mission->m_uMap[n1] == mission->m_uMap[nCurMissionGame2] &&
			player_sesion[nb]->nUnlockedMission[n2] && mission->m_uMap[n2] == mission->m_uMap[nCurMissionGame2] &&
			player_sesion[nb]->nUnlockedWay[x])
		{
			glBindTexture(GL_TEXTURE_2D, ga2res->skin[4]->GetCurrentTex());
			glPushMatrix();
			glTranslated((mission->m_dUbiX[n1]-ga2mapres->sx1-MAP_EXPANSION_X_DIV2)*40.0/MAP_EXPANSION_X_DIV2, (mission->m_dUbiY[n1]-ga2mapres->sy1-MAP_EXPANSION_Y_DIV2)*30.0/MAP_EXPANSION_Y_DIV2, 0.0);
			glRotated(dEsfWayPar[x*2+1], 0.0, 0.0, 1.0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(0,  SIZE_LINE2, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( dEsfWayPar[x*2]*40.0/MAP_EXPANSION_X_DIV2,  SIZE_LINE2, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( dEsfWayPar[x*2]*40.0/MAP_EXPANSION_X_DIV2, -SIZE_LINE2, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(0, -SIZE_LINE2, -50.0);
			glEnd();
			glPopMatrix();
		}
	}
	for(int x = 0; x < mission->m_nSize; x++)
	{
		if(player_sesion[nb]->nUnlockedMission[x] && mission->m_uMap[x] == mission->m_uMap[nCurMissionGame2])
		{
			glPushMatrix();
			glTranslated((mission->m_dUbiX[x]-ga2mapres->sx1-MAP_EXPANSION_X_DIV2)*40.0/MAP_EXPANSION_X_DIV2, (mission->m_dUbiY[x]-ga2mapres->sy1-MAP_EXPANSION_Y_DIV2)*30.0/MAP_EXPANSION_Y_DIV2, 0.0);
			bool bDrawIso = true;
			if(mission->m_uIcon[x])
			{
				if(player_sesion[nb]->nMission[x])
				{
					if(int(mission->m_uIcon[x]) <= ga2iconres->nSize)
					{
						glBindTexture(GL_TEXTURE_2D, ga2iconres->skin[mission->m_uIcon[x]-1]->GetCurrentTex());
						bDrawIso = false;
					}
				}
				else
				{
					if(int(mission->m_uIconSucess[x]) <= ga2iconres->nSize)
					{
						glBindTexture(GL_TEXTURE_2D, ga2iconres->skin[mission->m_uIcon[x]-1]->GetCurrentTex());
						bDrawIso = false;
					}
				}
				if(!bDrawIso)
				{
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-SIZE_ICON2,  SIZE_ICON2, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( SIZE_ICON2,  SIZE_ICON2, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( SIZE_ICON2, -SIZE_ICON2, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-SIZE_ICON2, -SIZE_ICON2, -50.0);
					glEnd();
				}
			}
			if(bDrawIso)
			{
				if(player_sesion[nb]->nMission[x]) glBindTexture(GL_TEXTURE_2D, ga2res->skin[6]->GetCurrentTex());
				else glBindTexture(GL_TEXTURE_2D, ga2res->skin[5]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-SIZE_NICON2,  SIZE_NICON2, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( SIZE_NICON2,  SIZE_NICON2, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( SIZE_NICON2, -SIZE_NICON2, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-SIZE_NICON2, -SIZE_NICON2, -50.0);
				glEnd();
			}
			glPopMatrix();
		}
	}
}

int DrawWorld()
{
	nCountGame2 += dwTick - dwLastTick;
	if(strCurMap != mission->m_strLand[nCurMissionGame2])
	{
		if(bMusicCurMap)
		{
			FMOD_Sound_Release(music_curMap);
			FMOD_Channel_Stop(channels[13]);
		}
		strCurMap = mission->m_strLand[nCurMissionGame2];
		CString str;
		str.Format(_T("Game2/%ls.mp3"), strCurMap);
		FMOD_RESULT res;
#ifdef UNICODE
		char chFile[260];
		UnicodeToAnsi(str, chFile);
		res = FMOD_System_CreateStream(sys, chFile, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &music_curMap);
#else
		res = FMOD_System_CreateStream(sys, str, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &music_curMap);
#endif
		if(res == FMOD_OK)
		{
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)13, music_curMap, 0, &channels[13]);
			bMusicCurMap = true;
		}
		else bMusicCurMap = false;

	}
#define TIME_TRANSI_WAYCHANGE_GAME2	500
#define TIME_TRANSI_ANGLESELECTOR_GAME2 700
	double dApt = 0;
	if((nCountGame2 - nTimeWayGame2) < TIME_TRANSI_WAYCHANGE_GAME2)
	{
		dApt = pow(double(nCountGame2 - nTimeWayGame2)/TIME_TRANSI_WAYCHANGE_GAME2, 0.5);
		dx = (dPosX-dBackPosX)*dApt+dBackPosX;
		dy = (dPosY-dBackPosY)*dApt+dBackPosY;
	}
	else
	{
		dx = dPosX;
		dy = dPosY;
	}
	ga2mapres->DrawAt(dx, dy, nMap);
	DrawMission();
#define DIST_REFARROW_GAME2 4.0
#define SIZE_ARROWREF2_GAME2 1.0
#define TIME_TRANSI_SHOWAD_GAME2 400
#define TIME_BLANKAFTERANU_GAME2 600
	if(bRepAnu)
	{
		if((nCountGame2 - nRepAnu) >= 0 && (nCountGame2 - nRepAnu) <= nDurRepAnu)
		{
			if(anuMission[nInRepAnu]->m_bLoad)
			{
				anuMission[nInRepAnu]->Update(dwTick - dwLastTick);
				anuMission[nInRepAnu]->Draw();
			}
			else bRepAnu = false;
		}
		if((nCountGame2 - nRepAnu) >= nDurRepAnu && (nCountGame2 - nRepAnu) <= (nDurRepAnu+TIME_BLANKAFTERANU_GAME2))
		{
			dApt = double(nCountGame2 - nRepAnu - nDurRepAnu)/TIME_BLANKAFTERANU_GAME2;
			glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		if((nCountGame2 - nRepAnu) > nDurRepAnu && bLinkOther)
		{
			bLinkOther = false;
			nCurMissionGame2 = nLinkOther;
			CheckWayUser();
			dBackPosX = dx = dPosX = mission->m_dUbiX[nCurMissionGame2];
			dBackPosY = dy = dPosY = mission->m_dUbiY[nCurMissionGame2];
			nMap = mission->m_uMap[nCurMissionGame2];
			nTimeWayGame2 = nCountGame2;
		}
	}
	if(!bRepAnu && bLinkOther)
	{
		bLinkOther = false;
		nCurMissionGame2 = nLinkOther;
		CheckWayUser();
		dBackPosX = dx = dPosX = mission->m_dUbiX[nCurMissionGame2];
		dBackPosY = dy = dPosY = mission->m_dUbiY[nCurMissionGame2];
		nMap = mission->m_uMap[nCurMissionGame2];
		nTimeWayGame2 = nCountGame2;
	}
	if((nCountGame2 - nTimeWayGame2) >= TIME_TRANSI_WAYCHANGE_GAME2)
	/*{

	else */if(!bRepAnu || (nCountGame2 - nRepAnu) > nDurRepAnu)
	{
		dApt = double((nCountGame2 - nTimeWayGame2)%TIME_TRANSI_ANGLESELECTOR_GAME2)/TIME_TRANSI_ANGLESELECTOR_GAME2;
		glPushMatrix();
		glTranslated(ga2mapres->dPosX*40.0/MAP_EXPANSION_X_DIV2, ga2mapres->dPosY*30.0/MAP_EXPANSION_Y_DIV2, 0.0);
		glRotated(dApt*360, 0.0, 0.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, ga2res->skin[2]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-SIZE_NICON2,  SIZE_NICON2, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( SIZE_NICON2,  SIZE_NICON2, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( SIZE_NICON2, -SIZE_NICON2, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-SIZE_NICON2, -SIZE_NICON2, -50.0);
		glEnd();
		if((nCountGame2 - nTimeWayGame2) < (TIME_TRANSI_WAYCHANGE_GAME2+TIME_TRANSI_SHOWAD_GAME2))
			dApt = pow(double(nCountGame2 - nTimeWayGame2 - TIME_TRANSI_WAYCHANGE_GAME2)/TIME_TRANSI_SHOWAD_GAME2, 0.5);
		else dApt = 1.0;
		glColor4d(1.0, 1.0, 1.0, dApt);
		for(int x = 0; x < 4; x++) if(bAd[x])
		{
			glLoadIdentity();
			glTranslated(ga2mapres->dPosX*40.0/MAP_EXPANSION_X_DIV2, ga2mapres->dPosY*30.0/MAP_EXPANSION_Y_DIV2, 0.0);
			glRotated(dAngAd[x], 0.0, 0.0, 1.0);
			glTranslated(DIST_REFARROW_GAME2*dApt, 0.0, 0.0);
			glRotated(-90.0, 0.0, 0.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, ga2res->skin[(x==0||x==3)?9:10]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-SIZE_ARROWREF2_GAME2,  SIZE_ARROWREF2_GAME2, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( SIZE_ARROWREF2_GAME2,  SIZE_ARROWREF2_GAME2, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( SIZE_ARROWREF2_GAME2, -SIZE_ARROWREF2_GAME2, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-SIZE_ARROWREF2_GAME2, -SIZE_ARROWREF2_GAME2, -50.0);
			glEnd();
		}
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glPopMatrix();
	}
	if(nChosesGame2 < CHOOSES_GAME2)
	{
#define TIME_START_SIDE_GAME2 0
#define TIME_TRANSI_SIDE_GAME2 500
#define TIME_GLOUTH_SIDE_GAME2 500
#define TIME_START_START_GAME2 500
		bool bFirst = (!nAPGame2);
		glPushMatrix();
		glTranslated((bFirst?-1:1)*38.0, 28.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga2res->skin[105]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-2.0,  2.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 2.0,  2.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 2.0, -2.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-2.0, -2.0, -50.0);
		glEnd();
		CString strDispTime;
		strDispTime.Format(_T("%.2d"), int((nMaxGame2 - nCountGame2)/1000));
		font_Micro->glDrawText(2.0, -2.0, -2.0, 2.0, 0, 0, -50.0, 1.0, 1.5, strDispTime, DT_VCENTER|DT_CENTER);
		glPopMatrix();
		if(nCountGame2 >= TIME_START_SIDE_GAME2 && nCountGame2 < (TIME_START_SIDE_GAME2+TIME_TRANSI_SIDE_GAME2))
		{
			dApt = (double(nCountGame2)-TIME_START_SIDE_GAME2)/(TIME_TRANSI_SIDE_GAME2);
			glPushMatrix();
			glTranslated((bFirst?1:-1)*30+(bFirst?1:-1)*20*(1-dApt), 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, ga2res->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -30.0, -50.0);
			glEnd();
			glPopMatrix();
		}
		if(nCountGame2 >= (TIME_START_SIDE_GAME2+TIME_TRANSI_SIDE_GAME2))
		{
			glPushMatrix();
			glTranslated((bFirst?1:-1)*30, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, ga2res->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -30.0, -50.0);
			glEnd();
			// Dibujar el preview del mapa (y la flecha)
			glLoadIdentity();
			glTranslated((bFirst?1:-1)*30, 22.5, 0.0);
			double rx = ga2mapres->dSizeX[nMap]/ga2mapres->dSizeY[nMap]*5.0;
			glBindTexture(GL_TEXTURE_2D, ga2mapres->skin[nMap]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-rx,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( rx,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( rx, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-rx, -5.0, -50.0);
			glEnd();
#define SIZE_INDICATORARROW_GAME2 2.0
			double ox = dx/ga2mapres->dSizeX[nMap]*rx*2, oy = dy/ga2mapres->dSizeY[nMap]*10.0;
			glTranslated(-rx+ox, SIZE_INDICATORARROW_GAME2-5.0+oy, 0.0);
			glBindTexture(GL_TEXTURE_2D, ga2res->skin[94]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-SIZE_INDICATORARROW_GAME2,  SIZE_INDICATORARROW_GAME2, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( SIZE_INDICATORARROW_GAME2,  SIZE_INDICATORARROW_GAME2, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( SIZE_INDICATORARROW_GAME2, -SIZE_INDICATORARROW_GAME2, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-SIZE_INDICATORARROW_GAME2, -SIZE_INDICATORARROW_GAME2, -50.0);
			glEnd();
			// Dibujar información
			// Land
			glLoadIdentity();
			glTranslated((bFirst?1:-1)*30+2.5, 11.5, 0.0);
			font_Micro->glDrawText(1.25, -5.0, -1.25, 5.0, 0.0, 0.0, -50.0, 1.0, 1.0, mission->m_strLand[nCurMissionGame2], DT_CENTER|DT_VCENTER);
			glLoadIdentity();
			glTranslated((bFirst?1:-1)*30-1.5, 9.0, 0.0);
#define SIZE_TEXT_GAME2 1.0
#define VEL_SHOWTEXT_GAME2 0.005
#define TIME_WAITTEXTNAME_GAME2 1500
			__int64 nTimeTextName = nCountGame2-nTimeWayGame2-TIME_WAITTEXTNAME_GAME2;
			if(nTimeTextName < 0) nTimeTextName = 0;
			double dSizeTex = font_Micro->CalcSingleLine(mission->m_strName[nCurMissionGame2], SIZE_TEXT_GAME2);
			DWORD t1 = DWORD(double(8.6/VEL_SHOWTEXT_GAME2));
			DWORD t2 = DWORD(double(dSizeTex/VEL_SHOWTEXT_GAME2));
			DWORD t = t1+t2;
			DWORD nShowText = (DWORD(nTimeTextName)+t1)%t;
			double kx = -VEL_SHOWTEXT_GAME2*double(nShowText)+8.6;
			font_Micro->glDrawText(1.25, 0, -1.25, 8.6, kx, 0.0, -50.0, 1.0, SIZE_TEXT_GAME2, mission->m_strName[nCurMissionGame2], DT_VCENTER);
			glLoadIdentity();
			glTranslated((bFirst?1:-1)*30-7.5, 7.6, 0.0);
			font_Micro->glDrawText(0, 0, -11.3, 15.0, 0.9, -1.1, -50.0, 1.0, SIZE_TEXT_GAME2, strCurNameSongGame2, 0);
			font_Micro->glDrawText(0, 0, -11.3, 15.0, 0.9, -3.1, -50.0, 1.0, SIZE_TEXT_GAME2, strCurArtistSongGame2, 0);
			font_Micro->glDrawText(0, 0, -11.3, 15.0, 0.9, -5.1, -50.0, 1.0, SIZE_TEXT_GAME2, mission->m_strComment[nCurMissionGame2], 0);
			glLoadIdentity();
			glTranslated((bFirst?1:-1)*30+1.5, -4.9, 0.0);
			bool bLevel = true;
			bool bTrunc = false;
			int nLevel = mission->m_uLvl[nCurMissionGame2];
			if(nLevel <= 0 || nLevel >= 25)
			{
				bLevel = false;
				nLevel = 1;
			}
			if(player_sesion[nAPGame2]->nLevel <= (nLevel-3) && !player_sesion[nAPGame2]->nLockedAsp[ITEM_SUPRIMELVL]) bTrunc = true;
			if(bTrunc)
			{
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[23]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-10.4,  1.3, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 10.4,  1.3, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 10.4, -1.3, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-10.4, -1.3, -50.0);
				glEnd();
			}
			else
			{
				if(bLevel)
				{
					glBindTexture(GL_TEXTURE_2D, ga2res->skin[16]->texture[nLevel-1]);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-2.0,  2.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 2.0,  2.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 2.0, -2.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-2.0, -2.0, -50.0);
					glEnd();
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, ga2res->skin[22]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-10.4,  1.3, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 10.4,  1.3, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 10.4, -1.3, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-10.4, -1.3, -50.0);
					glEnd();
				}
			}
			// Life
#define TIME_TRANSI_LIFEALTER_GAME2 500
			__int64 nTimeLife = nCountGame2-nTimeWayGame2;
			int nLife = np[nAPGame2];
			int nLifeG = np[nAPGame2]-mission->m_uStage[nCurMissionGame2];
			if(nLifeG < 0)
			{
				nLife = 0;
				nLifeG = 0;
			}
			glLoadIdentity();
			glTranslated((bFirst?1:-1)*30+2.2, -26.8, 0.0);
			glBindTexture(GL_TEXTURE_2D, ga2res->skin[95+nLifeG]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-4.1666,  0.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 4.1666,  0.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 4.1666, -0.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-4.1666, -0.5, -50.0);
			glEnd();
			dApt = abs(double((nCountGame2-nTimeWayGame2)%TIME_TRANSI_LIFEALTER_GAME2)/TIME_TRANSI_LIFEALTER_GAME2-0.5)*2;
			glColor4d(1.0, 1.0, 1.0, dApt);
			glBindTexture(GL_TEXTURE_2D, ga2res->skin[95+nLife]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-4.1666,  0.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 4.1666,  0.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 4.1666, -0.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-4.1666, -0.5, -50.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
			if(nCountGame2 < (TIME_START_SIDE_GAME2+TIME_TRANSI_SIDE_GAME2+TIME_GLOUTH_SIDE_GAME2))
			{
				dApt = (double(nCountGame2-TIME_START_SIDE_GAME2-TIME_TRANSI_SIDE_GAME2))/(TIME_GLOUTH_SIDE_GAME2);
				glLoadIdentity();
				glTranslated((bFirst?1:-1)*30, 0.0, 0.0);
				glScaled(1.0+0.2*dApt, 1.0+0.2*dApt, 0.0);
				glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[0]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -30.0, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
			}
			glPopMatrix();
		}
		if(nChosesGame2 == 1 && !mission->m_bLink[nCurMissionGame2])
		{
			__int64 nTimeShowCond = nCountGame2 - nTimeChosesGame2[0];
#define TIME_TRANSI_SHOWCOND_GAME2 500
			// Dibujo del Mileage en gasto
			__int64 nTimeMileageShow = nCountGame2 - nTimeChosesGame2[0];
#define TIME_TRANSI_MILEAGESHOW_UNLOCK 300
			if(nTimeMileageShow >= 0 && nTimeMileageShow <= TIME_TRANSI_MILEAGESHOW_UNLOCK)
			{
				dApt = double(nTimeMileageShow)/TIME_TRANSI_MILEAGESHOW_UNLOCK;
				glPushMatrix();
				glTranslated(0.0, 20.0, 0.0);
				glScaled(1.0, dApt, 1.0);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[88]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d( -30.0,  3.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(  30.0,  3.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(  30.0, -3.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( -30.0, -3.75, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nTimeMileageShow > TIME_TRANSI_MILEAGESHOW_UNLOCK)
			{
				glPushMatrix();
				glTranslated(0.0, 20.0, 0.0);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[88]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d( -30.0,  3.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(  30.0,  3.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(  30.0, -3.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( -30.0, -3.75, -50.0);
				glEnd();
				CString str;
				str.Format(_T("%.8d"), player_sesion[nAPGame2]->nMileage);
				glPushMatrix();
				glTranslated(-1.3366, 0.0, 0.0);
				font_Micro->glDrawText(1.875, -15.0, -1.875, 15.0, 0.0, 0.0, -50.0, 1.0, 1.7, str, DT_CENTER|DT_VCENTER);
				glPopMatrix();
				str.Format(_T("%.4d"), nReqMileage);
				glPushMatrix();
				glTranslated(19.6038, 0.0, 0.0);
				glColor4d(1.0, 0.0, 0.0, 1.0);
				font_Micro->glDrawText(1.875, -15.0, -1.875, 15.0, 0.0, 0.0, -50.0, 1.0, 1.7, str, DT_CENTER|DT_VCENTER);
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
				glPopMatrix();
			}
			if(nTimeShowCond >= 0 && nTimeShowCond < TIME_TRANSI_SHOWCOND_GAME2)
			{
				dApt = (double(nTimeShowCond))/(TIME_TRANSI_SHOWCOND_GAME2);
				glPushMatrix();
				glScaled(1.0-0.2*(1.0-dApt), 1.0-0.2*(1.0-dApt), 1.0);
				glColor4d(1.0, 1.0, 1.0, dApt);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[7]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-35.5625,  18.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 35.5625,  18.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 35.5625, -18.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-35.5625, -18.0, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(nTimeShowCond >= TIME_TRANSI_SHOWCOND_GAME2)
			{
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[7]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-35.5625,  18.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 35.5625,  18.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 35.5625, -18.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-35.5625, -18.0, -50.0);
				glEnd();
				glPushMatrix();
				glTranslated(0.0, 2.0, 0.0);
				font_Micro2->glDrawText(18.0, -35.5, -18.0, 35.5, 0.0, 0.0, -50.0, 1.0, 2.0, mission->m_strShowCond[nCurMissionGame2], DT_CENTER|DT_VCENTER);
				glPopMatrix();
			}
			__int64 nCountSide = nCountGame2 - nTimeChosesGame2[0];
			int k = nAPGame2;
			if(nCountSide >= TIME_START_LR_GAME2 && nCountSide <= (TIME_START_LR_GAME2+TIME_TRANSI_LR_GAME2))
			{
				dApt = pow(double(nCountSide-TIME_START_LR_GAME2)/TIME_TRANSI_LR_GAME2, 0.5);
				glPushMatrix();
				glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(1-dApt), 0.0, 0.0);
				glColor4d(1.0, 1.0, 1.0, dApt);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[(k?12:11)]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
#define TIME_TRANSI_PRESET_GAME2 500
#define TIME_GLOUTH_PRESET_GAME2 500
#define TIME_SHOW_PRESET_GAME2 1500
			if(nCountSide > (TIME_START_LR_GAME2+TIME_TRANSI_LR_GAME2))
			{
				glPushMatrix();
				glTranslated(k?40.0:-40.0, 0.0, 0.0);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[(k?12:11)]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
				glEnd();
				glPopMatrix();
				__int64 nCountPreset = nCountGame2-nTimeLastPresetGame2;
				for(int q = 0; q < 6; q++)
				{
					double dAdv = 0.0;
					for(int w = 0; w < nIndSizePresetGame2[q]; w++)
					{
						int nInd = (q*5+w);
						if(nInd == nIndLastPresetGame2 || nIndLastPresetGame2 == -1)
						{
							__int64 nCountPreset = nCountGame2-nTimeLastPresetGame2;
							if(nCountPreset >= 0 && nCountPreset < TIME_TRANSI_PRESET_GAME2)
							{
								glPushMatrix();
								dApt = double(nCountPreset)/TIME_TRANSI_PRESET_GAME2;
								glLoadIdentity();
								glColor4d(1.0, 1.0, 1.0, dApt);
								glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
								glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
								glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
								glEnd();
								glLoadIdentity();
								glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0, 1.0);
								glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
								glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
								glEnd();
								glLoadIdentity();
								glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga2res->skin[21]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
								glEnd();
								glLoadIdentity();
								glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
								glScaled((1-dApt)*4.0, (1-dApt)*4.0, 1.0);
								glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
								glBindTexture(GL_TEXTURE_2D, ga2res->skin[19]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
								glEnd();
								glColor4d(1.0, 1.0, 1.0, 1.0);
								glPopMatrix();
								dAdv += dApt*9.0+4.5;
							}
							if(nCountPreset >= TIME_TRANSI_PRESET_GAME2 && nCountPreset < (TIME_TRANSI_PRESET_GAME2+TIME_SHOW_PRESET_GAME2))
							{
								glPushMatrix();
								glLoadIdentity();
								glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
								glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
								glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
								glEnd();
								glLoadIdentity();
								glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
								glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
								glEnd();
								glLoadIdentity();
								glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga2res->skin[21]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
								glEnd();
								if(nCountPreset < (TIME_TRANSI_PRESET_GAME2+TIME_GLOUTH_PRESET_GAME2))
								{
									dApt = double(nCountPreset - TIME_TRANSI_PRESET_GAME2)/TIME_GLOUTH_PRESET_GAME2;
									glLoadIdentity();
									glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
									glScaled(dApt*4.0, dApt*4.0, 1.0);
									glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
									glBindTexture(GL_TEXTURE_2D, ga2res->skin[18]->GetCurrentTex());
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
									glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
									glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
									glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
									glEnd();
									glColor4d(1.0, 1.0, 1.0, 1.0);
								}
								glPopMatrix();
								dAdv += 9.0+4.5;
							}
							if(nCountPreset >= (TIME_TRANSI_PRESET_GAME2+TIME_SHOW_PRESET_GAME2))
							{
								glPushMatrix();
								glLoadIdentity();
								glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
								glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
								glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
								glEnd();
								if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME2+TIME_SHOW_PRESET_GAME2))
								{
									dApt = double(nCountPreset-(TIME_TRANSI_PRESET_GAME2+TIME_SHOW_PRESET_GAME2))/TIME_TRANSI_PRESET_GAME2;
									glLoadIdentity();
									glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
									glScaled(1.0+2.0*(dApt), 1.0, 1.0);
									glColor4d(1.0, 1.0, 1.0, 1-dApt);
									glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
									glBegin(GL_QUADS);
									glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
									glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
									glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
									glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
									glEnd();
									glColor4d(1.0, 1.0, 1.0, 1.0);
								}
								glLoadIdentity();
								glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga2res->skin[21]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
								glEnd();
								glPopMatrix();
								if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME2+TIME_SHOW_PRESET_GAME2)) dAdv += (1-dApt)*9.0;
								dAdv += 4.5;
							}
						}
						else
						{
							glPushMatrix();
							glLoadIdentity();
							glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
							glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
							glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
							glEnd();
							glLoadIdentity();
							glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga2res->skin[21]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
							glEnd();
							glPopMatrix();
							dAdv += 4.5;
						}
					}
				}
			}
#define TIME_TRANSI_MODEDITOR_GAME2 300
#define TIME_TRANSI_MODS_GAME2 300
#define TIME_TRANSI_MOVEMODS_GAME2 300
#define ANGLE_MODS_GAME2 10.0
			if(mission->m_bActUMod[nCurMissionGame2])
			{
				if(nTimeShowCond >= 0 && nTimeShowCond <= TIME_TRANSI_MODEDITOR_GAME2)
				{
					dApt = pow(double(nTimeShowCond)/TIME_TRANSI_MODEDITOR_GAME2, 0.5);
					glPushMatrix();
					glTranslated(0.0, -20.0*(1-dApt), 0.0);
					glBindTexture(GL_TEXTURE_2D, ga2res->skin[80]->GetCurrentTex());
					glColor4d(1.0, 1.0, 1.0, dApt);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -40.0, -10.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  40.0, -10.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -40.0, -30.0, -50.0);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
				if(nTimeShowCond > TIME_TRANSI_MODEDITOR_GAME2)
				{
					glBindTexture(GL_TEXTURE_2D, ga2res->skin[80]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -40.0, -10.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  40.0, -10.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -40.0, -30.0, -50.0);
					glEnd();
					if(nTimeShowCond <= (TIME_TRANSI_MODEDITOR_GAME2+TIME_TRANSI_MODS_GAME2))
					{
						dApt = pow(double(nTimeShowCond-TIME_TRANSI_MODEDITOR_GAME2)/TIME_TRANSI_MODS_GAME2, 0.5);
						for(int u = 0; u < 7; u++)
						{
							glPushMatrix();
							double dAng = 180.0*(1-dApt)-double(u)*ANGLE_MODS_GAME2;
							glTranslated(0.0, -50.0, 0.0);
							glRotated(dAng, 0.0, 0.0, 1.0);
							glTranslated(0.0, 37.2, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga2res->skin[81+u]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -2.8,  2.8, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  2.8,  2.8, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  2.8, -2.8, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -2.8, -2.8, -50.0);
							glEnd();
							glPopMatrix();
						}
					}
	#define ANGLE_TOLSCALINGMODS_GAME2 10.0
					if(nTimeShowCond > (TIME_TRANSI_MODEDITOR_GAME2+TIME_TRANSI_MODS_GAME2))
					{
						for(int u = 0; u < 7; u++)
						{
							glPushMatrix();
							double dAng = double(nCurModEditGame2)*ANGLE_MODS_GAME2-double(u)*ANGLE_MODS_GAME2;
							if(nDirModEditGame2 != 0 && (nCountGame2 - nTimeModEditGame2) >= 0 && (nCountGame2 - nTimeModEditGame2) < (TIME_TRANSI_MOVEMODS_GAME2))
							{
								dApt = double(nCountGame2 - nTimeModEditGame2)/TIME_TRANSI_MOVEMODS_GAME2;
								dAng += (nDirModEditGame2 == 1?1.0:-1.0)*(1-dApt)*ANGLE_MODS_GAME2;
							}
							glTranslated(0.0, -50.0, 0.0);
							glRotated(dAng, 0.0, 0.0, 1.0);
							glTranslated(0.0, 37.2, 0.0);
							if(abs(dAng) < ANGLE_TOLSCALINGMODS_GAME2)
							{
								dApt = abs(dAng) / ANGLE_TOLSCALINGMODS_GAME2;
								glScaled(1.0+(1-dApt)*0.3, 1.0+(1-dApt)*0.3, 1.0);
							}
							glBindTexture(GL_TEXTURE_2D, ga2res->skin[81+u]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -2.8,  2.8, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  2.8,  2.8, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  2.8, -2.8, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -2.8, -2.8, -50.0);
							glEnd();
							glPopMatrix();
							if(nCurModEditGame2 == u)
							{
								if(u == 1)
								{
	#define TIME_LVTRANSI_GAME2 300
	#define TRANSLX_PRESETS_GAME2 -20.0
	#define TRANSLY_PRESETS_GAME2 -7.5
	#define TIME_LVGLOUTH_GAME2 300
									__int64 nTimeLv = nCountGame2 - nTimeChangeModGame2;
									if(nTimeLv >= 0 && nTimeLv <= TIME_LVGLOUTH_GAME2)
									{
										dApt = double(nTimeLv)/TIME_LVGLOUTH_GAME2;
										glPushMatrix();
										glColor4d(1.0, 1.0, 1.0, 1-dApt);
										glTranslated(0.0, TRANSLY_PRESETS_GAME2, 0.0);
										glScaled(1.0+dApt, 1.0+dApt, 1.0);
										glBindTexture(GL_TEXTURE_2D, ga2res->skin[19]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glPopMatrix();
										glColor4d(1.0, 1.0, 1.0, 1.0);
									}
									if(nTimeLv >= 0 && nTimeLv <= TIME_LVTRANSI_GAME2)
									{
										dApt = double(nTimeLv)/TIME_LVTRANSI_GAME2;
										glPushMatrix();
										glColor4d(1.0, 1.0, 1.0, dApt);
										glTranslated(0.0, TRANSLY_PRESETS_GAME2, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga2res->skin[89+nCurStateModEditGame2[1]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glPopMatrix();
										glColor4d(1.0, 1.0, 1.0, 1.0);
									}
									if(nTimeLv > TIME_LVTRANSI_GAME2)
									{
										glPushMatrix();
										glTranslated(0.0, TRANSLY_PRESETS_GAME2, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga2res->skin[89+nCurStateModEditGame2[1]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glPopMatrix();
									}
								}
								if(u >= 2)
								{
									__int64 nCountPreset = nCountGame2-nTimeLastPresetGame2;
									int q = u-2;
									double dAdv = 0.0;
									for(int w = 0; w < nIndSizePresetGame2[q]; w++)
									{
										int nInd = (q*5+w);
										if(nInd == nIndLastPresetGame2 || nIndLastPresetGame2 == -1)
										{
											__int64 nCountPreset = nCountGame2-nTimeLastPresetGame2;
											if(nCountPreset >= 0 && nCountPreset < TIME_TRANSI_PRESET_GAME2)
											{
												glPushMatrix();
												dApt = double(nCountPreset)/TIME_TRANSI_PRESET_GAME2;
												glLoadIdentity();
												glColor4d(1.0, 1.0, 1.0, dApt);
												glTranslated(TRANSLX_PRESETS_GAME2+dAdv, TRANSLY_PRESETS_GAME2, 0.0);
												glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
												glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
												glBegin(GL_QUADS);
												glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
												glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
												glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
												glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
												glEnd();
												glLoadIdentity();
												glTranslated(TRANSLX_PRESETS_GAME2+dAdv+4.5, TRANSLY_PRESETS_GAME2, 0.0);
												glScaled(1.0+2.0*(1-dApt), 1.0, 1.0);
												glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
												glBegin(GL_QUADS);
												glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
												glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
												glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
												glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
												glEnd();
												glLoadIdentity();
												glTranslated(TRANSLX_PRESETS_GAME2+dAdv, TRANSLY_PRESETS_GAME2, 0.0);
												glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
												glBindTexture(GL_TEXTURE_2D, ga2res->skin[21]->GetCurrentTex());
												glBegin(GL_QUADS);
												glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
												glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
												glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
												glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
												glEnd();
												glLoadIdentity();
												glTranslated(TRANSLX_PRESETS_GAME2+dAdv, TRANSLY_PRESETS_GAME2, 0.0);
												glScaled((1-dApt)*4.0, (1-dApt)*4.0, 1.0);
												glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
												glBindTexture(GL_TEXTURE_2D, ga2res->skin[19]->GetCurrentTex());
												glBegin(GL_QUADS);
												glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
												glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
												glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
												glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
												glEnd();
												glColor4d(1.0, 1.0, 1.0, 1.0);
												glPopMatrix();
												dAdv += dApt*9.0+4.5;
											}
											if(nCountPreset >= TIME_TRANSI_PRESET_GAME2 && nCountPreset < (TIME_TRANSI_PRESET_GAME2+TIME_SHOW_PRESET_GAME2))
											{
												glPushMatrix();
												glLoadIdentity();
												glTranslated(TRANSLX_PRESETS_GAME2+dAdv, TRANSLY_PRESETS_GAME2, 0.0);
												glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
												glBegin(GL_QUADS);
												glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
												glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
												glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
												glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
												glEnd();
												glLoadIdentity();
												glTranslated(TRANSLX_PRESETS_GAME2+dAdv+(k?-4.5:4.5), TRANSLY_PRESETS_GAME2, 0.0);
												glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
												glBegin(GL_QUADS);
												glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
												glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
												glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
												glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
												glEnd();
												glLoadIdentity();
												glTranslated(TRANSLX_PRESETS_GAME2+dAdv, TRANSLY_PRESETS_GAME2, 0.0);
												glBindTexture(GL_TEXTURE_2D, ga2res->skin[21]->GetCurrentTex());
												glBegin(GL_QUADS);
												glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
												glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
												glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
												glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
												glEnd();
												if(nCountPreset < (TIME_TRANSI_PRESET_GAME2+TIME_GLOUTH_PRESET_GAME2))
												{
													dApt = double(nCountPreset - TIME_TRANSI_PRESET_GAME2)/TIME_GLOUTH_PRESET_GAME2;
													glLoadIdentity();
													glTranslated(TRANSLX_PRESETS_GAME2+dAdv, TRANSLY_PRESETS_GAME2, 0.0);
													glScaled(dApt*4.0, dApt*4.0, 1.0);
													glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
													glBindTexture(GL_TEXTURE_2D, ga2res->skin[18]->GetCurrentTex());
													glBegin(GL_QUADS);
													glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
													glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
													glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
													glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
													glEnd();
													glColor4d(1.0, 1.0, 1.0, 1.0);
												}
												glPopMatrix();
												dAdv += 9.0+4.5;
											}
											if(nCountPreset >= (TIME_TRANSI_PRESET_GAME2+TIME_SHOW_PRESET_GAME2))
											{
												glPushMatrix();
												glLoadIdentity();
												glTranslated(TRANSLX_PRESETS_GAME2+dAdv, TRANSLY_PRESETS_GAME2, 0.0);
												glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
												glBegin(GL_QUADS);
												glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
												glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
												glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
												glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
												glEnd();
												if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME2+TIME_SHOW_PRESET_GAME2))
												{
													dApt = double(nCountPreset-(TIME_TRANSI_PRESET_GAME2+TIME_SHOW_PRESET_GAME2))/TIME_TRANSI_PRESET_GAME2;
													glLoadIdentity();
													glTranslated(TRANSLX_PRESETS_GAME2+dAdv+4.5, TRANSLY_PRESETS_GAME2, 0.0);
													glScaled(1.0+2.0*(dApt), 1.0, 1.0);
													glColor4d(1.0, 1.0, 1.0, 1-dApt);
													glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
													glBegin(GL_QUADS);
													glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
													glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
													glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
													glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
													glEnd();
													glColor4d(1.0, 1.0, 1.0, 1.0);
												}
												glLoadIdentity();
												glTranslated(TRANSLX_PRESETS_GAME2+dAdv, TRANSLY_PRESETS_GAME2, 0.0);
												glBindTexture(GL_TEXTURE_2D, ga2res->skin[21]->GetCurrentTex());
												glBegin(GL_QUADS);
												glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
												glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
												glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
												glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
												glEnd();
												glPopMatrix();
												if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME2+TIME_SHOW_PRESET_GAME2)) dAdv += (1-dApt)*9.0;
												dAdv += 4.5;
											}
										}
										else
										{
											glPushMatrix();
											glLoadIdentity();
											glTranslated(TRANSLX_PRESETS_GAME2+dAdv, TRANSLY_PRESETS_GAME2, 0.0);
											glBindTexture(GL_TEXTURE_2D, ga2res->skin[23+nIndPresetGame2[nInd]]->GetCurrentTex());
											glBegin(GL_QUADS);
											glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
											glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
											glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
											glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
											glEnd();
											glLoadIdentity();
											glTranslated(TRANSLX_PRESETS_GAME2+dAdv, TRANSLY_PRESETS_GAME2, 0.0);
											glBindTexture(GL_TEXTURE_2D, ga2res->skin[21]->GetCurrentTex());
											glBegin(GL_QUADS);
											glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
											glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
											glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
											glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
											glEnd();
											glPopMatrix();
											dAdv += 4.5;
										}
									}
								}
							}
						}
					}
				}
			}
			if(nModeUnlockedGame2 != 0)
			{
#define TIME_TRANSI_GAME2ED_GAME2 300
#define TIME_DUR_GAME2ED_GAME2 500
				__int64 nTransGame2er = nCountGame2 - nTimeUnlockedGame2;
				if(nTransGame2er >= 0 && nTransGame2er < TIME_TRANSI_GAME2ED_GAME2)
				{
					dApt = double(nTransGame2er)/TIME_TRANSI_GAME2ED_GAME2;
					glPushMatrix();
					glScaled(1.0, dApt, 1.0);
					glColor4d(1.0, 1.0, 1.0, dApt);
					glBindTexture(GL_TEXTURE_2D, ga2res->skin[103+nModeUnlockedGame2]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -30.0,  3.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  30.0,  3.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  30.0, -3.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -30.0, -3.75, -50.0);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
				if(nTransGame2er >= TIME_TRANSI_GAME2ED_GAME2 && nTransGame2er < (TIME_TRANSI_GAME2ED_GAME2+TIME_DUR_GAME2ED_GAME2))
				{
					glBindTexture(GL_TEXTURE_2D, ga2res->skin[103+nModeUnlockedGame2]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -30.0,  3.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  30.0,  3.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  30.0, -3.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -30.0, -3.75, -50.0);
					glEnd();
				}
				if(nTransGame2er >= (TIME_TRANSI_GAME2ED_GAME2+TIME_DUR_GAME2ED_GAME2) && nTransGame2er < (TIME_TRANSI_GAME2ED_GAME2+TIME_DUR_GAME2ED_GAME2+TIME_TRANSI_GAME2ED_GAME2))
				{
					dApt = 1.0-double(nTransGame2er-TIME_TRANSI_GAME2ED_GAME2-TIME_DUR_GAME2ED_GAME2)/TIME_TRANSI_GAME2ED_GAME2;
					glPushMatrix();
					glScaled(1.0, dApt, 1.0);
					glColor4d(1.0, 1.0, 1.0, dApt);
					glBindTexture(GL_TEXTURE_2D, ga2res->skin[103+nModeUnlockedGame2]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -30.0,  3.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  30.0,  3.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  30.0, -3.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -30.0, -3.75, -50.0);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
			}
		}
		if(nCountGame2 >= TIME_START_START_GAME2 &&
			(!bRepAnu || (nCountGame2 - nRepAnu) > nDurRepAnu))
		{
			bool bNewCommand = false;
			static bool bCh[] = {false, false, false, false, false};
			int x = nAPGame2;
			if(x == 0)
			{
				for(int y = 0; y < 5; y++)
				{
					if(bArrows[y] && !bEnArrowsGame2[y])
					{
						bEnArrowsGame2[y] = true;
						nTimeEnArrowsGame2[y] = nCountGame2;
						for(int q = (MAX_COMMANDS_GAME2-2); q >= 0; q--)
						{
							nComandGame2[x][q+1] = nComandGame2[x][q];
						}
						nComandGame2[x][0] = y;
						bNewCommand = true;
					}
					if(!bArrows[y] && bEnArrowsGame2[y])
					{
						bEnArrowsGame2[y] = false;
						bCh[y] = false;
					}
				}
			}
			if(x == 1)
			{
				for(int y = 5; y < 10; y++)
				{
					if(bArrows[y] && !bEnArrowsGame2[y-5])
					{
						bEnArrowsGame2[y-5] = true;
						nTimeEnArrowsGame2[y-5] = nCountGame2;
						for(int q = (MAX_COMMANDS_GAME2-2); q >= 0; q--)
						{
							nComandGame2[x][q+1] = nComandGame2[x][q];
						}
						nComandGame2[x][0] = y-5;
						bNewCommand = true;
					}
					if(!bArrows[y] && bEnArrowsGame2[y-5])
					{
						bEnArrowsGame2[y-5] = false;
						bCh[y-5] = false;
					}
				}
			}
			if(bEnArrowsGame2[0] && !bCh[0])
			{
				bCh[0] = true;
				if(nChosesGame2 < 1) ChangeMission(0);
				if(nChosesGame2 == 1 && mission->m_bActUMod[nCurMissionGame2]) ToogleMovMod(true);
			}
			if(bEnArrowsGame2[4] && !bCh[4])
			{
				bCh[4] = true;
				if(nChosesGame2 < 1) ChangeMission(3);
				if(nChosesGame2 == 1 && mission->m_bActUMod[nCurMissionGame2]) ToogleMovMod(false);
			}
			if(bEnArrowsGame2[1] && !bCh[1])
			{
				bCh[1] = true;
				if(nChosesGame2 < 1) ChangeMission(1);
				nChosesGame2 = 0;
			}
			if(bEnArrowsGame2[3] && !bCh[3])
			{
				bCh[3] = true;
				if(nChosesGame2 < 1) ChangeMission(2);
				nChosesGame2 = 0;
			}
			if(bEnArrowsGame2[2] && !bCh[2])
			{
				if(nChosesGame2 == 1 && !mission->m_bLink[nCurMissionGame2] && mission->m_bActUMod[nCurMissionGame2] && nCurModEditGame2 != 0)
				{
					ToogleChangeMod();
				}
				else
				{
					if(nChosesGame2 == 0 && !mission->m_bLink[nCurMissionGame2])
					{
						TraduceModCurMission();
						nCurModEditGame2 = 0;
						nTimeModEditGame2 = 0;
						nReqMileage = 0;
						memset(nCurStateModEditGame2, 0, sizeof(int)*7);
					}
					if(mission->m_bLink[nCurMissionGame2])
					{
						bLinkOther = true;
						nLinkOther = mission->SearchID(mission->m_uGo[nCurMissionGame2]);
						if(!mission->m_strGo[nCurMissionGame2].IsEmpty() && nChosesGame2 == 0)
							DeclareMakeAnu();
					}
					else if(!mission->m_strGo[nCurMissionGame2].IsEmpty() && nChosesGame2 == 1)
					{
						DeclareMakeAnu();
						if(player_sesion[nAPGame2]->nMileage >= nReqMileage)
						{
							nChosesGame2++;
							nTimeChosesGame2[nChosesGame2-1] = nCountGame2;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_wchoose, 0, &channels[11]);
							player_sesion[nAPGame2]->nMileage -= nReqMileage;
						}
						else
						{
							nTimeUnlockedGame2 = nCountGame2;
							nModeUnlockedGame2 = 1;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)14, sound_lock, 0, &channels[14]);
						}
					}
					else
					{
						if(nChosesGame2 == 0 && np[nAPGame2] >= int(mission->m_uStage[nCurMissionGame2]))
						{
							nChosesGame2++;
							nTimeChosesGame2[nChosesGame2-1] = nCountGame2;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_wchoose, 0, &channels[11]);
						}
						else if(nChosesGame2 == 1)
						{
							if(player_sesion[nAPGame2]->nMileage >= nReqMileage && np[nAPGame2] >= int(mission->m_uStage[nCurMissionGame2]))
							{
								nChosesGame2++;
								nTimeChosesGame2[nChosesGame2-1] = nCountGame2;
								FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_wchoose, 0, &channels[11]);
								player_sesion[nAPGame2]->nMileage -= nReqMileage;
							}
							else
							{
								nTimeUnlockedGame2 = nCountGame2;
								nModeUnlockedGame2 = 1;
								FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)14, sound_lock, 0, &channels[14]);
							}
						}
					}
				}
				bCh[2] = true;
			}
			if(nCountGame2 >= nMaxGame2)
			{
				if(!mission->m_bLink[nCurMissionGame2] && np[nAPGame2] >= int(mission->m_uStage[nCurMissionGame2]))
				{
					nChosesGame2++;
					nTimeChosesGame2[nChosesGame2-1] = nCountGame2;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_wchoose, 0, &channels[11]);
					player_sesion[nAPGame2]->nMileage -= nReqMileage;
					for(int x = 0; x < 5; x++) bCh[x] = false;
				}
			}
		}
	}
#define TIME_EXIT_GAME2 1000
	if(nChosesGame2 >= CHOOSES_GAME2)
	{
		__int64 nTransEnd = nCountGame2 - nTimeChosesGame2[CHOOSES_GAME2-1];
		if(nTransEnd < TIME_EXIT_GAME2)
		{
			bool bFirst = (!nAPGame2);
			int k = nAPGame2;
			if(nTransEnd < TIME_START_SIDE_GAME2)
			{
				glPushMatrix();
				glTranslated((bFirst?1:-1)*30, 0.0, 0.0);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[0]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -30.0, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nTransEnd >= TIME_START_SIDE_GAME2 && nTransEnd < (TIME_START_SIDE_GAME2+TIME_TRANSI_SIDE_GAME2))
			{
				dApt = 1-(double(nTransEnd)-TIME_START_SIDE_GAME2)/(TIME_TRANSI_SIDE_GAME2);
				glPushMatrix();
				glTranslated((bFirst?1:-1)*30+(bFirst?1:-1)*20*(1-dApt), 0.0, 0.0);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[0]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -30.0, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nTransEnd >= 0 && nTransEnd < TIME_TRANSI_SHOWCOND_GAME2)
			{
				dApt = 1-(double(nTransEnd))/(TIME_TRANSI_SHOWCOND_GAME2);
				glPushMatrix();
				glScaled(1.0-0.2*(1.0-dApt), 1.0-0.2*(1.0-dApt), 1.0);
				glColor4d(1.0, 1.0, 1.0, dApt);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[7]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-35.5625,  18.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 35.5625,  18.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 35.5625, -18.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-35.5625, -18.0, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(nTransEnd < TIME_START_LR_GAME2)
			{
				glPushMatrix();
				glTranslated(k?40.0:-40.0, 0.0, 0.0);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[(k?12:11)]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nTransEnd >= TIME_START_LR_GAME2 && nTransEnd <= (TIME_START_LR_GAME2+TIME_TRANSI_LR_GAME2))
			{
				dApt = 1-pow(double(nTransEnd-TIME_START_LR_GAME2)/TIME_TRANSI_LR_GAME2, 0.5);
				glPushMatrix();
				glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(1-dApt), 0.0, 0.0);
				glColor4d(1.0, 1.0, 1.0, dApt);
				glBindTexture(GL_TEXTURE_2D, ga2res->skin[(k?12:11)]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(mission->m_bActUMod[nCurMissionGame2])
			{
				if(nTransEnd <= (TIME_TRANSI_MODS_GAME2))
				{
					dApt = pow(double(nTransEnd)/TIME_TRANSI_MODS_GAME2, 0.5);
					for(int u = 0; u < 7; u++)
					{
						glPushMatrix();
						double dAng = 180.0*(dApt)-double(u)*ANGLE_MODS_GAME2;
						glTranslated(0.0, -50.0, 0.0);
						glRotated(dAng, 0.0, 0.0, 1.0);
						glTranslated(0.0, 37.2, 0.0);
						glBindTexture(GL_TEXTURE_2D, ga2res->skin[81+u]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -2.8,  2.8, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  2.8,  2.8, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  2.8, -2.8, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -2.8, -2.8, -50.0);
						glEnd();
						glPopMatrix();
					}
				}
				if(nTransEnd < TIME_TRANSI_MODS_GAME2)
				{
					glBindTexture(GL_TEXTURE_2D, ga2res->skin[80]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -40.0, -10.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  40.0, -10.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -40.0, -30.0, -50.0);
					glEnd();
				}
				if(nTransEnd >= TIME_TRANSI_MODS_GAME2 && nTransEnd <= (TIME_TRANSI_MODS_GAME2+TIME_TRANSI_MODEDITOR_GAME2))
				{
					dApt = 1-pow(double(nTransEnd-TIME_TRANSI_MODS_GAME2)/TIME_TRANSI_MODEDITOR_GAME2, 0.5);
					glPushMatrix();
					glTranslated(0.0, -20.0*(1-dApt), 0.0);
					glBindTexture(GL_TEXTURE_2D, ga2res->skin[80]->GetCurrentTex());
					glColor4d(1.0, 1.0, 1.0, dApt);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -40.0, -10.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  40.0, -10.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -40.0, -30.0, -50.0);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
			}
		}
		else
		{
			FMOD_Channel_Stop(channels[13]);
			player_sesion[nAPGame2]->nLastMission = nCurMissionGame2;
			return 2;
		}
	}
	return 0;
}

int StageCheckGame2()
{
	bool bIs[] = {false, false};
	for(int x = 0; x < 2; x++) if(np[x])
	{
		bIs[x] = true;
		np[x]-=mission->m_uStage[nCurMissionGame2];
		np[x] = max(0, np[x]);
		if(np[x]==1) np[x] = 0;
		double dApt = (bUsb[0]?1.0:0.1)*(bMissionSucess?1.0:0.5)*(player_sesion[x]->nLockedAsp[ITEM_MOREEXP]?2.0:1.0);
		PlusExpPlayer(x, int(mission->m_uLvl[nCurMissionGame2]), dApt);
		if(bMissionSucess)
		{
			player_sesion[x]->nMileage += int(mission->m_uMileage[nCurMissionGame2]);
			if(player_sesion[x]->nLockedAsp[ITEM_LUCK] && (rand()%5) == 0) player_sesion[x]->nMileage += int(mission->m_uMileage[nCurMissionGame2]);
			player_sesion[x]->nMission[nCurMissionGame2] = 1;
			for(int y = 0; y < mission->m_nSizeCondOpen[nCurMissionGame2][bMissionSucess-1]; y++)
			{
				int n1 = mission->SearchID(mission->m_uCondOpen[nCurMissionGame2][bMissionSucess-1][y]);
				if(n1 != -1)
					player_sesion[x]->nUnlockedMission[n1] = 1;
			}
			for(int y = 0; y < way->m_nSizeWay; y++)
			{
				int n1 = mission->SearchID(way->m_uLink[y*2]), n2 = mission->SearchID(way->m_uLink[y*2+1]);
				BYTE bUn1 = player_sesion[x]->nUnlockedMission[n1], bUn2 = player_sesion[x]->nUnlockedMission[n1];
				if(bUn1 && bUn2)
				{
					bool bUnlock = true;
					for(int z = 0; z < int(way->m_uSizeMUnlock[y]); z++)
					{
						if(!player_sesion[x]->nUnlockedMission[mission->SearchID(way->m_uMUnlock[y][z])])
						{
							bUnlock = false;
							break;
						}
					}
					for(int z = 0; z < int(way->m_uSizeMSucess[y]); z++)
					{
						if(!player_sesion[x]->nMission[mission->SearchID(way->m_uMSucess[y][z])])
						{
							bUnlock = false;
							break;
						}
					}
					if(bUnlock)
					{
						player_sesion[x]->nUnlockedWay[y] = 1;
					}
				}
			}
			for(int y = 0; y < mission->m_nSizeCondUnlock[nCurMissionGame2][bMissionSucess-1]; y++)
			{
				int nID = mission->m_uCondUnlock[nCurMissionGame2][bMissionSucess-1][y];
				int nSet = mission->m_dwCondUnlockModes[nCurMissionGame2][bMissionSucess-1][y]&0xF;
				bool bSet[5];
				bSet[0] = ((mission->m_dwCondUnlockModes[nCurMissionGame2][bMissionSucess-1][y]>>4)&0xF)?true:false;
				bSet[1] = ((mission->m_dwCondUnlockModes[nCurMissionGame2][bMissionSucess-1][y]>>8)&0xF)?true:false;
				bSet[2] = ((mission->m_dwCondUnlockModes[nCurMissionGame2][bMissionSucess-1][y]>>12)&0xF)?true:false;
				bSet[3] = ((mission->m_dwCondUnlockModes[nCurMissionGame2][bMissionSucess-1][y]>>16)&0xF)?true:false;
				bSet[4] = ((mission->m_dwCondUnlockModes[nCurMissionGame2][bMissionSucess-1][y]>>20)&0xF)?true:false;
				bool bReg = false;
				if(nSet == 0)
				{
					BYTE *nPar;
					nPar = new BYTE[16];
					for(int z = 0; z < 16; z++) nPar[z] = 0;
					for(int z = 0; z < 5; z++) if(player_sesion[x]->nLockedSongNew[nID*10+z]==2 && bSet[z])
					{
						player_sesion[x]->nLockedSongNew[nID*10+z] = 1;
						nPar[z] = 1;
						bReg = true;
					}
					if(bReg) RegisterUnlock(0, nID, nPar);
					delete nPar;
				}
				if(nSet == 1)
				{
					BYTE *nPar;
					nPar = new BYTE[16];
					for(int z = 0; z < 16; z++) nPar[z] = 0;
					for(int z = 5; z < 10; z++) if(player_sesion[x]->nLockedSongNew[nID*10+z]==2 && bSet[z])
					{
						player_sesion[x]->nLockedSongNew[nID*10+z] = 1;
						nPar[z+5] = 1;
						bReg = true;
					}
					if(bReg) RegisterUnlock(1, nID, nPar);
					delete nPar;
				}
				if(nSet == 2)
				{
					BYTE *nPar;
					nPar = new BYTE[16];
					for(int z = 0; z < 16; z++) nPar[z] = 0;
					if(player_sesion[x]->nLockedBrainShower[nID] == 2 && bSet[0])
					{
						player_sesion[x]->nLockedBrainShower[nID] = 1;
						bReg = true;
					}
					nPar[10+nChaTotalSongsGame4[nID]] = 1;
					if(bReg) RegisterUnlock(2, nID, nPar);
					delete nPar;
				}
			}
			for(int y = 0; y < mission->m_nSizeCondAspUnlock[nCurMissionGame2][bMissionSucess-1]; y++)
			{
				if(player_sesion[x]->nLockedAsp[mission->m_uCondAspUnlock[nCurMissionGame2][bMissionSucess-1][y]] == 2)
					player_sesion[x]->nLockedAsp[mission->m_uCondAspUnlock[nCurMissionGame2][bMissionSucess-1][y]] = 1;
			}
		}
		CheckForUnlock();
	}
	if((!np[0]) && (!np[1])) return 1;
	else if((!np[0] && bIs[0]) || (!np[1] && bIs[1])) return 2;
	return 0;
}

///////////////////////////////////////
// ** Funciones principales

int CreateContextGame2()
{
	nRefAnuMission = (int*)calloc(0, sizeof(int));
	mission = new CMissionLoader();
	way = new CMWayLoader();
	if(!mission->LoadBinary(_T("Game2/Mission.dat")))
	{
		if(!mission->LoadText(_T("Game2/Mission.acc"))) return 0;
		if(!mission->SaveBinary(_T("Game2/Mission.dat"))) return 0;
	}
	if(!way->LoadBinary(_T("Game2/Way.dat")))
	{
		if(!way->LoadText(_T("Game2/Way.acc"))) return 0;
		if(!way->SaveBinary(_T("Game2/Way.dat"))) return 0;
	}
	dEsfWayPar = new double[way->m_nSizeWay*2];
	for(int x = 0; x < way->m_nSizeWay; x++)
	{
		int n1 = mission->SearchID(way->m_uLink[x*2]);
		int n2 = mission->SearchID(way->m_uLink[x*2+1]);
		double dx = (mission->m_dUbiX[n2]-mission->m_dUbiX[n1]);
		double dy = (mission->m_dUbiY[n2]-mission->m_dUbiY[n1]);
		double d = sqrt(pow(dx, 2) + pow(dy, 2));
		double t;
		if(dx == 0) t = dy/abs(dy)*90;
		else t = (dx<0?180:0)+atan(dy / dx)*180/M_PI;
		dEsfWayPar[x*2] = d;
		dEsfWayPar[x*2+1] = t;
	}
	nSizeTotalGame2 = nSizeTotalGame2Ex = mission->m_nSize;
	return 1;
}

void DestroyContextGame2()
{
	delete mission;
	delete way;
	delete dEsfWayPar;
	if(nSizeAnuMission != 0)
	{
		for(int x = 0; x < nSizeAnuMission; x++)
		{
			delete anuMission[x];
			if(bIsSoundAnuMission[x]) FMOD_Sound_Release(soundAnuMission[x]);
		}
		delete[] anuMission;
		delete[] soundAnuMission;
		delete bIsSoundAnuMission;
		nRefAnuMission = (int*)realloc(nRefAnuMission, 0);
	}
	nSizeAnuMission = 0;
	delete nRefAnuMission;
}

void ClearContextGame2()
{
	delete ga2res;
	delete ga2mapres;
	delete ga2iconres;
	if(nSizeAnuMission != 0)
	{
		for(int x = 0; x < nSizeAnuMission; x++)
		{
			delete anuMission[x];
			if(bIsSoundAnuMission[x]) FMOD_Sound_Release(soundAnuMission[x]);
		}
		delete[] anuMission;
		delete[] soundAnuMission;
		delete bIsSoundAnuMission;
		nRefAnuMission = (int*)realloc(nRefAnuMission, 0);
	}
	nSizeAnuMission = 0;
	FMOD_Sound_Release(sound_wmove);
	FMOD_Sound_Release(sound_wchoose);
	FMOD_Sound_Release(sound_wstop);
	piuconfig.nModeMach = 0;
}

void ActualizateContextGame2()
{
	bMusicCurMap = false;
	strCurMap = _T("");
	ga2res = new CBasicGame2Res();
	ga2res->LoadSkin();
	ga2mapres = new CMapGame2Res();
	ga2mapres->LoadFile(_T("Game2/MapSize.acc"));
	ga2iconres = new CPluGame2Res();
	ga2iconres->LoadSkin(_T("Game2/Res.acc"));
	bRepAnu = false;
	nRepAnu = 0;
	nDurRepAnu = 0;
	nCurModEditGame2 = 0;
	nDirModEditGame2 = 0;
	nTimeModEditGame2 = 0;
	nTimeChangeModGame2 = 0;
	nReqMileage = 0;
	nTimeUnlockedGame2 = 0;
	nModeUnlockedGame2 = 0;
	memset(nCurStateModEditGame2, 0, sizeof(int)*7);
	FMOD_System_CreateStream(sys, "Game2/wmove.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_wmove);
	FMOD_System_CreateStream(sys, "Game2/wchoose.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_wchoose);
	FMOD_System_CreateStream(sys, "Game2/wstop.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_wstop);
	switch(piuconfig.nTimePlay)
	{
	default:
		nMaxGame2 = 90000; break;
	case 1:
		nMaxGame2 = 60000; break;
	case 2:
		nMaxGame2 = 30000; break;
	}
	for(int y = 0; y < 2; y++) if(np[y])
	{
		nAPGame2 = y;
	}
	nInterGame2 = 0;
	nCountGame2 = 0;
	bContinueUsbGame2 = true;
	nTimeWayGame2 = 0;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		ShowSesionTrayExp(x);
		ShowSesionTrayMileage(x);
		nCurMissionGame2 = player_sesion[x]->nLastMission;
		if(nCurMissionGame2 == -1) nCurMissionGame2 = mission->SearchID(1);// Ejecutar primera instancia
		if(nCurMissionGame2 != -1)
		{
			dPosX = mission->m_dUbiX[nCurMissionGame2];
			dPosY = mission->m_dUbiY[nCurMissionGame2];
			dBackPosX = 0;
			dBackPosY = 0;
			nMap = mission->m_uMap[nCurMissionGame2];
		}
	}
	nSizeAnuMission = 0;
	for(int x = 0; x < mission->m_nSize; x++)
	{
		if(!mission->m_strGo[x].IsEmpty())
		{
			nSizeAnuMission++;
			nRefAnuMission = (int*)realloc(nRefAnuMission, nSizeAnuMission*sizeof(int));
			nRefAnuMission[nSizeAnuMission-1] = x;
		}
	}
	if(nSizeAnuMission != 0)
	{
		anuMission = new CAnu*[nSizeAnuMission];
		soundAnuMission = new FMOD_SOUND*[nSizeAnuMission];
		bIsSoundAnuMission = new bool[nSizeAnuMission];
		for(int x = 0; x < nSizeAnuMission; x++)
		{
			anuMission[x] = new CAnu();
			CString str;
			str.Format(_T("Game2/anu/%ls"), mission->m_strGo[nRefAnuMission[x]]);
			anuMission[x]->Open(str);
			str.Format(_T("Game2/anu/%ls.wav\0"), mission->m_strGo[nRefAnuMission[x]]);
			FMOD_RESULT res;
#ifdef UNICODE
			char chFile[260];
			UnicodeToAnsi(str, chFile);
			res = FMOD_System_CreateStream(sys, chFile, FMOD_HARDWARE | FMOD_2D, 0, &soundAnuMission[x]);
#else
			res = FMOD_System_CreateStream(sys, str, FMOD_HARDWARE | FMOD_2D, 0, &soundAnuMission[x]);
#endif
			if(res == FMOD_OK) bIsSoundAnuMission[x] = true;
			else bIsSoundAnuMission[x] = false;
		}
	}
	nIndLastPresetGame2 = -1;	// Show all
	nTimeLastPresetGame2 = 0;
	for(int y = 0; y < 5; y++)
	{
		nIndSizePresetGame2[y] = 0;
		nIndPresetGame2[y*5] = 0;
		nIndPresetGame2[y*5+1] = 0;
		nIndPresetGame2[y*5+2] = 0;
		nIndPresetGame2[y*5+3] = 0;
		nIndPresetGame2[y*5+4] = 0;
	}

	for(int x = 0; x < 10; x++) {
        bArrows[x] = false;
    }
	CheckWayUser();
	strCurNameSongGame2 = GetNameSongsGame2(nCurMissionGame2);
	strCurArtistSongGame2 = GetArtistSongsGame2(nCurMissionGame2);
	piuconfig.nModeMach = 1;
	dwTick = dwLastTick = SDL_GetTicks();
}

void SemiActualizateContextGame2()
{
	bMusicCurMap = false;
	strCurMap = _T("");
	nCountGame2 = 0;
	nChosesGame2 = 0;
	nTimeWayGame2 = 0;
	bRepAnu = false;
	nRepAnu = 0;
	nDurRepAnu = 0;
	nCurModEditGame2 = 0;
	nDirModEditGame2 = 0;
	nTimeModEditGame2 = 0;
	nTimeChangeModGame2 = 0;
	nReqMileage = 0;
	nTimeUnlockedGame2 = 0;
	nModeUnlockedGame2 = 0;
	memset(nCurStateModEditGame2, 0, sizeof(int)*7);
	switch(piuconfig.nTimePlay)
	{
	default:
		nMaxGame2 = 90000; break;
	case 1:
		nMaxGame2 = 60000; break;
	case 2:
		nMaxGame2 = 30000; break;
	}
	for(int x = 0; x < 2; x++) if(np[x])
	{
		ShowSesionTrayExp(x);
		ShowSesionTrayMileage(x);
		nCurMissionGame2 = player_sesion[x]->nLastMission;
		if(nCurMissionGame2 == -1) nCurMissionGame2 = mission->SearchID(1);// Ejecutar primera instancia
		if(nCurMissionGame2 != -1)
		{
			dPosX = mission->m_dUbiX[nCurMissionGame2];
			dPosY = mission->m_dUbiY[nCurMissionGame2];
			dBackPosX = 0;
			dBackPosY = 0;
			nMap = mission->m_uMap[nCurMissionGame2];
		}
	}
	nIndLastPresetGame2 = -1;	// Show all
	nTimeLastPresetGame2 = 0;
	for(int y = 0; y < 5; y++)
	{
		nIndSizePresetGame2[y] = 0;
		nIndPresetGame2[y*5] = 0;
		nIndPresetGame2[y*5+1] = 0;
		nIndPresetGame2[y*5+2] = 0;
		nIndPresetGame2[y*5+3] = 0;
		nIndPresetGame2[y*5+4] = 0;
	}

	for(int x = 0; x < 10; x++) {
        bArrows[x] = false;
    }
	CheckWayUser();
	strCurNameSongGame2 = GetNameSongsGame2(nCurMissionGame2);
	strCurArtistSongGame2 = GetArtistSongsGame2(nCurMissionGame2);
	dwTick = dwLastTick = SDL_GetTicks();
}

bool bContAfterSplashGame2 = false;

int FrameGame2()
{
	ga2res->Update(dwTick - dwLastTick);
	ga2iconres->Update(dwTick - dwLastTick);
	ga2mapres->Update(dwTick - dwLastTick);
	switch(nInterGame2)
	{
	case 0:
		switch(DrawWorld())
		{
		case 1:
			ClearContextGame2();
			return INT_WELCOME;
			break;
		case 2:
			if(mission->m_strHis[nCurMissionGame2].IsEmpty())
			{
				LoadSongGame2();
				Go_Step();
				nInterGame2 = 1;
			}
			else
			{
				strHis = mission->m_strHis[nCurMissionGame2];
				if(ActualizateContextHistory())
					nInterGame2 = 7;
				else
				{
					LoadSongGame2();
					Go_Step();
					nInterGame2 = 1;
				}
			}
			break;
		case 3:
			return INT_ITEMSHOP;
		}
		break;
	case 1:
		if(!Draw_All())
		{
			nInterGame2 = 2;
			nModeStepEval = 1;
			ActualizateContextStepEval();
		}
		{
			double dMB = 0.0;
			if(!nAPGame2)
			{
				if(dMB < viewp1->status.dLife) dMB = viewp1->status.dLife;
			}
			else
			{
				if(dMB < viewp2->status.dLife) dMB = viewp2->status.dLife;
			}
			if(dMB <= 0.0 && mission->m_bBreak[nCurMissionGame2])
			{
				nInterGame2 = 5;
				DeleteSongGame2();
			}
		}
		break;
	case 2:
		if(!FrameStepEval())
		{
			nInterGame2 = 3;
		}
		break;
	case 3:
		DeleteSongGame2();
		{
			int nCode = StageCheckGame2();
			if(nCode == 0) nInterGame2 = 0;
			else
			{
				nInterGame2 = 4;
				if(nCode == 1) bContinueUsbGame2 = false;
				if(nCode == 2) bContinueUsbGame2 = true;
				bSaveUsb = true;
				ActualizateContextUsb();
			}
			if(nSizeUnlock)
			{
				nModeUnlock = 0;
				ActualizateContextUnlock();
				nInterGame2 = 6;
				if(nCode != 0) bContAfterSplashGame2 = false;
				else bContAfterSplashGame2 = true;
			}
		}
		SemiActualizateContextGame2();
		break;
	case 4:	// Dibujar Sistema USB
		if(!FrameUsb())
		{
			nInterGame2 = 0;
			if(!bContinueUsbGame2)
			{
				ClearContextGame2();
				return INT_WELCOME;
			}
		}
		break;
	case 5:
		if(!FrameStageBreakStepEval())
		{
			nInterGame2 = 3;
		}
		break;
	case 6:
		if(FrameUnlock() != INT_UNLOCK)
		{
			nInterGame2 = bContAfterSplashGame2?0:4;
		}
		break;
	case 7:
		if(!FrameHistory())
		{
			LoadSongGame2();
			Go_Step();
			nInterGame2 = 1;
		}
		break;
	}
	return INT_GAME_2;
}

int EventGame2(int nEvent, int nKey)
{
	switch(nInterGame2)
	{
	case 0:
		if(nEvent == EV_KEYDOWN)
		{
			if(nKey == EV_P1_ARROW1) bArrows[0] = true;
			if(nKey == EV_P1_ARROW7) bArrows[1] = true;
			if(nKey == EV_P1_CENTER) bArrows[2] = true;
			if(nKey == EV_P1_ARROW9) bArrows[3] = true;
			if(nKey == EV_P1_ARROW3) bArrows[4] = true;
			if(nKey == EV_P2_ARROW1) bArrows[5] = true;
			if(nKey == EV_P2_ARROW7) bArrows[6] = true;
			if(nKey == EV_P2_CENTER) bArrows[7] = true;
			if(nKey == EV_P2_ARROW9) bArrows[8] = true;
			if(nKey == EV_P2_ARROW3) bArrows[9] = true;
		}
		if(nEvent == EV_KEYUP)
		{
			if(nKey == EV_P1_ARROW1) bArrows[0] = false;
			if(nKey == EV_P1_ARROW7) bArrows[1] = false;
			if(nKey == EV_P1_CENTER) bArrows[2] = false;
			if(nKey == EV_P1_ARROW9) bArrows[3] = false;
			if(nKey == EV_P1_ARROW3) bArrows[4] = false;
			if(nKey == EV_P2_ARROW1) bArrows[5] = false;
			if(nKey == EV_P2_ARROW7) bArrows[6] = false;
			if(nKey == EV_P2_CENTER) bArrows[7] = false;
			if(nKey == EV_P2_ARROW9) bArrows[8] = false;
			if(nKey == EV_P2_ARROW3) bArrows[9] = false;
		}
		break;
	case 1:
		Event_Step(nEvent, nKey);
		break;
	case 6:
		EventUnlock(nEvent, nKey);
		break;
	case 7:
		EventHistory(nEvent, nKey);
		break;
	}
	return 1;
}
