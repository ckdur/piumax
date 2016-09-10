#include "UnlockSys.h"
#include "UnlockRes.h"
#include "game1.h"
#include "Game3.h"
#include "Game4.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"

#include "UnlockLoader.h"

__int64 nCountUnlock = 0;
int nModeUnlock = 0;
CUnlockRes* unlockres;
int nSizeUnlock = 0;
GLuint *uTexUnlock;
BYTE *nParUnlock;
CString *strIDUnlock;
__int64 nTimeSongUnlock;
int nSongUnlock;
int nModeMovUnlock = 0;

CUnlockLoader* unlock;

////////////////////////////////////////////////
// ** Funciones de la interfase basica

#define TIME_START_SELECTOR_UNLOCK 0
#define TIME_TRANSI_SELECTOR_UNLOCK 1000
#define TIME_GLOUTH_SELECTOR_UNLOCK 500

#define TIME_START_START_UNLOCK 1500

#define TIME_TRANSI_SONGBORDER_UNLOCK 500
#define TIME_GLOUTH_SONGBORDER_UNLOCK 500
#define TIME_TRANSI_START_MAIN_UNLOCK 500
#define CHOOSES_UNLOCK 2

#define TIME_SHOW_PREVIEW_UNLOCK 600

int nInterUnlock = 0;
int nSizeSongsUnlock = 0;
int nModTotalSongsUnlock[16000];
BYTE nModSongsUnlock[2][16000];
int nChaTotalSongsUnlock[1000];
int nChaSongsUnlock[1000];
int nRefSongsUnlock[1000];
int nRefExTotalSongsUnlock[2000];
int nRefExSongsUnlock[2000];
int nMilTotalSongsUnlock[16000];
int nMilSongsUnlock[16000];
GLuint nTexCurSongUnlock[9];
TCHAR chTotalSongsUnlock[1000][100];
TCHAR chSongsUnlock[1000][100];

BYTE nSelectedModSongUnlock[2][16];
int nMileageUnlock[2];

int nIndexShowUnlock[9];
double dDCurUnlock[2][5];
int nCurSongUnlock = 0;
int nModeCurSongUnlock[2];
int nBackModeCurSongUnlock[2];
int nPrefModeCurSongUnlock[2];
__int64 nTimeModeCurSongUnlock[2];
__int64 nTimeCurSongUnlock = 0;
__int64 nMaxUnlock = 0;
__int64 nTimeChaUnlock = 0;

int nSizeTotalUnlock = 0;

int nChosesUnlock;
__int64 nTimeChosesUnlock[2];

bool bEnArrowsUnlock[10];
__int64 nTimeEnArrowsUnlock[10];
int nCommandUnlock[2][10];

int nTimeTransiSongBorderUnlock = TIME_TRANSI_SONGBORDER_UNLOCK;
bool bBonusStageUnlock[2];
//bool bSaveUsbUnlock = false;
bool bContinueUsbUnlock = true;

GLuint movie_texUnlock;
CMovieStream2* movie_unlockbg;
CMovieStream2* movie_songpreviewunlock;

bool bLoadMusicCoverUnlock = false;
FMOD_SOUND* music_coverUnlock;

CString strDispUnlock;

CUnlockBasRes* unlockbasres;

void SemiActualizateContextUnlock();

int nSizeSkinUnlock = 0;
int* nRefSkinUnlock;
GLuint* uTexSkinUnlock;
int nSizeCharacterUnlock = 0;
int* nRefCharacterUnlock;
GLuint* uTexCharacterUnlock;

__int64 nTimeUnlockedUnlock;
int nModeUnlockedUnlock;

int FoundPresetUnlock(int x, int nFamily, int nBegin, int nEnd);
void ActualizateIndexShowUnlock();

int nMileageSongMachineUnlock = 0;
int nMileageSongPlayerUnlock[2];

void SetMileageUnlock()
{
	bool bEvalMachine = false;
	nMileageSongMachineUnlock = 0;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		nMileageSongPlayerUnlock[x] = 0;
		for(int y = 0; y < 16; y++)
		{
			if(nModSongsUnlock[x][nCurSongUnlock*16+y] == 1)
			{
				nMileageSongPlayerUnlock[x] += nMilSongsUnlock[nCurSongUnlock*16+y]*(bUsb[x]?1:10);
			}
			if(nModSongsUnlock[x][nCurSongUnlock*16+y] == 0 && !bEvalMachine && bUsb[x])
			{
				nMileageSongMachineUnlock += nMilSongsUnlock[nCurSongUnlock*16+y]*10;
			}
		}
		bEvalMachine = true;
	}
}

int SetChooseModsUnlock(int nPlayer)
{
	if(nModeCurSongUnlock[nPlayer] == 16)
	{
		bool bCons = true;
		for(int a = 0; a < 2; a++) if(np[a])
		{
			if(player_sesion[a]->nMileage < nMileageUnlock[a])
			{
				bCons = false;
			}
		}
		if(bCons)
		{
			for(int a = 0; a < 2; a++) if(np[a])
			{
				player_sesion[a]->nMileage -= nMileageUnlock[a];
				CString str;
				str = ( chSongsUnlock[nCurSongUnlock]);
				bool bRepUnlock = false;
				for(int s = 0; s < nSizeTotalGame1; s++)
				{
					if(str == GetIDTotalSongsGame1(s))
					{
						for(int y = 0; y < 10; y++) if(nSelectedModSongUnlock[a][y])
						{
							if(nModSongsUnlock[a][nCurSongUnlock*16+y] == 0 && bUsb[a])
							{
								machine_sesion->nLockedSongNew[s*10+y] = 0;
								nModSongsUnlock[a][nCurSongUnlock*16+y] = 4;
							}
							else if(nModSongsUnlock[a][nCurSongUnlock*16+y] == 1)
							{
								player_sesion[a]->nLockedSongNew[s*10+y] = 0;
								nModSongsUnlock[a][nCurSongUnlock*16+y] = 0;
							}
							bRepUnlock = true;
						}
					}
				}
				/*for(int s = 0; s < nSizeTotalGame3; s++)
				{
					if(str == GetIDTotalSongsGame3(s))
					{
						for(int y = 0; y < 5; y++) if(nSelectedModSongUnlock[a][y+5])
						{
							if(nModSongsUnlock[a][nCurSongUnlock*16+y+5] == 0 && bUsb[a])
							{
								machine_sesion->nLockedSpecNew[s*5+y] = 0;
								nModSongsUnlock[a][nCurSongUnlock*16+y+5] = 4;
							}
							else if(nModSongsUnlock[a][nCurSongUnlock*16+y+5] == 1)
							{
								player_sesion[a]->nLockedSpecNew[s*5+y] = 0;
								nModSongsUnlock[a][nCurSongUnlock*16+y+5] = 0;
							}
							bRepUnlock = true;
						}
					}
				}*/
				for(int s = 0; s < 5; s++)
				{
					if(nModTotalSongsUnlock[nCurSongUnlock*16+s+10] && nSelectedModSongUnlock[a][s+10])
					{
						if(nModSongsUnlock[a][nCurSongUnlock*16+s+10] == 0 && bUsb[a])
						{
							machine_sesion->nLockedBrainShower[nModTotalSongsUnlock[nCurSongUnlock*16+s+10]-1] = 0;
							nModSongsUnlock[a][nCurSongUnlock*16+s+10] = 4;
						}
						else if(nModSongsUnlock[a][nCurSongUnlock*16+s+10] == 1)
						{
							player_sesion[a]->nLockedBrainShower[nModTotalSongsUnlock[nCurSongUnlock*16+s+10]-1] = 0;
							nModSongsUnlock[a][nCurSongUnlock*16+s+10] = 0;
						}
						bRepUnlock = true;
					}
				}
				if(nSelectedModSongUnlock[a][15])
				{
					if(str.Find(_T("skin")) != -1)
					{
						CString strscan = str.Mid(4, str.GetLength());
						int nRef = 0;
						if(_sscanf(strscan, _T("%d"), &nRef) == 1)
						{
							if(nModSongsUnlock[a][nCurSongUnlock*16+15] == 0 && bUsb[a])
							{
								machine_sesion->nLockedAsp[nRef] = 0;
								nModSongsUnlock[a][nCurSongUnlock*16+15] = 4;
							}
							else if(nModSongsUnlock[a][nCurSongUnlock*16+15] == 1)
							{
								player_sesion[a]->nLockedAsp[nRef] = 0;
								nModSongsUnlock[a][nCurSongUnlock*16+15] = 0;
							}
							bRepUnlock = true;
						}
					}
					else if(str.Find(_T("character")) != -1)
					{
						CString strscan = str.Mid(9, str.GetLength());
						int nRef = 0;
						if(_sscanf(strscan, _T("%d"), &nRef) == 1)
						{
							if(nModSongsUnlock[a][nCurSongUnlock*16+15] == 0 && bUsb[a])
							{
								machine_sesion->nLockedAsp[MAX_SKINS+nRef] = 0;
								nModSongsUnlock[a][nCurSongUnlock*16+15] = 4;
							}
							else if(nModSongsUnlock[a][nCurSongUnlock*16+15] == 1)
							{
								player_sesion[a]->nLockedAsp[MAX_SKINS+nRef] = 0;
								nModSongsUnlock[a][nCurSongUnlock*16+15] = 0;
							}
							bRepUnlock = true;
						}
					}
				}
				if(bRepUnlock)
				{
					nTimeUnlockedUnlock = nCountUnlock;
					nModeUnlockedUnlock = 1;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)15, sound_lockoff, 0, &channels[15]);
					SetMileageUnlock();
				}
				return 1;
			}
		}
		else
		{
			nTimeUnlockedUnlock = nCountUnlock;
			nModeUnlockedUnlock = 2;
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)14, sound_lock, 0, &channels[14]);
		}
	}
	else
	{
		if(nModSongsUnlock[nPlayer][nCurSongUnlock*16+nModeCurSongUnlock[nPlayer]] == 0 || (!bUsb[nPlayer] && nModSongsUnlock[nPlayer][nCurSongUnlock*16+nModeCurSongUnlock[nPlayer]] == 1))
		{
			if(nSelectedModSongUnlock[nPlayer][nModeCurSongUnlock[nPlayer]])
			{
				nMileageUnlock[nPlayer] -= nMilSongsUnlock[nCurSongUnlock*16+nModeCurSongUnlock[nPlayer]]*10;
				nSelectedModSongUnlock[nPlayer][nModeCurSongUnlock[nPlayer]] = 0;
			}
			else
			{
				nMileageUnlock[nPlayer] += nMilSongsUnlock[nCurSongUnlock*16+nModeCurSongUnlock[nPlayer]]*10;
				nSelectedModSongUnlock[nPlayer][nModeCurSongUnlock[nPlayer]] = 1;
			}
		}
		else if(nModSongsUnlock[nPlayer][nCurSongUnlock*16+nModeCurSongUnlock[nPlayer]] == 1)
		{
			if(nSelectedModSongUnlock[nPlayer][nModeCurSongUnlock[nPlayer]])
			{
				nMileageUnlock[nPlayer] -= nMilSongsUnlock[nCurSongUnlock*16+nModeCurSongUnlock[nPlayer]];
				nSelectedModSongUnlock[nPlayer][nModeCurSongUnlock[nPlayer]] = 0;
			}
			else
			{
				nMileageUnlock[nPlayer] += nMilSongsUnlock[nCurSongUnlock*16+nModeCurSongUnlock[nPlayer]];
				nSelectedModSongUnlock[nPlayer][nModeCurSongUnlock[nPlayer]] = 1;
			}
		}
	}
	return 0;
}

void MoveModsUnlock(bool bIzq, int nPlayer)
{
	if(bIzq)
	{
		nModeCurSongUnlock[nPlayer]--;
		if(nModeCurSongUnlock[nPlayer] < 0) nModeCurSongUnlock[nPlayer] = 16;
		nTimeModeCurSongUnlock[nPlayer] = nCountUnlock;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
	}
	else
	{
		nModeCurSongUnlock[nPlayer]++;
		if(nModeCurSongUnlock[nPlayer] > 16) nModeCurSongUnlock[nPlayer] = 0;
		nTimeModeCurSongUnlock[nPlayer] = nCountUnlock;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
	}
}

void MoveGroupModsUnlock(bool bIzq, int nPlayer)
{
	if(bIzq)
	{
		if(nModeCurSongUnlock[nPlayer] >= 0 && nModeCurSongUnlock[nPlayer] <= 4)
			nModeCurSongUnlock[nPlayer] = 16;
		else if(nModeCurSongUnlock[nPlayer] == 16)
			nModeCurSongUnlock[nPlayer] = 15;
		else nModeCurSongUnlock[nPlayer]-=5;
		if(nModeCurSongUnlock[nPlayer] < 0) nModeCurSongUnlock[nPlayer] = 16;
		nTimeModeCurSongUnlock[nPlayer] = nCountUnlock;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
	}
	else
	{
		if(nModeCurSongUnlock[nPlayer] >= 10 && nModeCurSongUnlock[nPlayer] <= 14)
			nModeCurSongUnlock[nPlayer] = 15;
		else if(nModeCurSongUnlock[nPlayer] = 15)
			nModeCurSongUnlock[nPlayer] = 16;
		else nModeCurSongUnlock[nPlayer]+=5;
		if(nModeCurSongUnlock[nPlayer] > 16) nModeCurSongUnlock[nPlayer] = 0;
		nTimeModeCurSongUnlock[nPlayer] = nCountUnlock;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
	}
}

void SetChannelUnlock(bool bIzq)
{
	if(bIzq)
	{
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
		bool bLoopLook = true;
		int nIndexLook = nCurSongUnlock;
		while(bLoopLook)
		{
			nIndexLook--;
			if(nIndexLook < 0) nIndexLook = nSizeSongsUnlock-1;
			if(nChaSongsUnlock[nIndexLook] != nChaSongsUnlock[nCurSongUnlock] || nIndexLook == nCurSongUnlock)
				bLoopLook = false;
		}
		nCurSongUnlock = nIndexLook;
		nTimeChaUnlock = nCountUnlock;
		glDeleteTextures(9, nTexCurSongUnlock);
		ActualizateIndexShowUnlock();
		for(int x = 0; x < 9; x++)
		{
			nTexCurSongUnlock[x] = 0;
			TCHAR chFile[260];
			_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsUnlock[nIndexShowUnlock[x]]);
			nTexCurSongUnlock[x] = GenTexImage(chFile);
		}
		FMOD_Channel_Stop(channels[9]);
	}
	else
	{
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
		bool bLoopLook = true;
		int nIndexLook = nCurSongUnlock;
		while(bLoopLook)
		{
			nIndexLook++;
			if(nIndexLook >= nSizeSongsUnlock) nIndexLook = 0;
			if(nChaSongsUnlock[nIndexLook] != nChaSongsUnlock[nCurSongUnlock] || nIndexLook == nCurSongUnlock)
				bLoopLook = false;
		}
		nCurSongUnlock = nIndexLook;
		nTimeChaUnlock = nCountUnlock;
		glDeleteTextures(9, nTexCurSongUnlock);
		ActualizateIndexShowUnlock();
		for(int x = 0; x < 9; x++)
		{
			nTexCurSongUnlock[x] = 0;
			TCHAR chFile[260];
			_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsUnlock[nIndexShowUnlock[x]]);
			nTexCurSongUnlock[x] = GenTexImage(chFile);
		}
		FMOD_Channel_Stop(channels[9]);
	}
}

CString GetIDTotalSongsUnlock(int nIndex)
{
	if(nIndex >= nSizeTotalUnlock) return CString(_T(""));
	CString str;
	str = ( chTotalSongsUnlock[nIndex]);
	return str;
}

CString GetNameTotalSongsUnlock(int nIndex)
{
	if(nIndex >= nSizeTotalUnlock) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsUnlock[nIndex], chTotalSongsUnlock[nIndex], chLang);
	FILE* hFile = _fopen(chFile, _T("r"));
	if(hFile == NULL) return CString(_T("File not found"));
	int nCaret = 0;
	TCHAR ch[260];
	CString str;
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

CString GetArtistTotalSongsUnlock(int nIndex)
{
	if(nIndex >= nSizeTotalUnlock) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsUnlock[nIndex], chTotalSongsUnlock[nIndex], chLang);
	FILE* hFile = _fopen(chFile, _T("r"));
	if(hFile == NULL) return CString(_T("File not found"));
	int nCaret = 0;
	TCHAR ch[260];
	CString str;
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

CString GetBPMTotalSongsUnlock(int nIndex)
{
	if(nIndex >= nSizeTotalUnlock) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsUnlock[nIndex], chTotalSongsUnlock[nIndex], chLang);
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

CString GetNameSongsUnlock(int nIndex)
{
	if(nIndex >= nSizeSongsUnlock) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsUnlock[nIndex], chSongsUnlock[nIndex], chLang);
	FILE* hFile = _fopen(chFile, _T("r"));
	if(hFile == NULL) return CString(_T("File not found"));
	int nCaret = 0;
	TCHAR ch[260];
	CString str;
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

CString GetArtistSongsUnlock(int nIndex)
{
	if(nIndex >= nSizeSongsUnlock) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsUnlock[nIndex], chSongsUnlock[nIndex], chLang);
	FILE* hFile = _fopen(chFile, _T("r"));
	if(hFile == NULL) return CString(_T("File not found"));
	int nCaret = 0;
	TCHAR ch[260];
	CString str;
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

CString GetBPMSongsUnlock(int nIndex)
{
	if(nIndex >= nSizeSongsUnlock) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsUnlock[nIndex], chSongsUnlock[nIndex], chLang);
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

int EnumTotalSongsUnlock()
{
	FILE* hFile = NULL;
	hFile = _fopen(_T("songUnlockNew.acc"), _T("r"));
	if(hFile == NULL) return 0;
	TCHAR ch[500];
	int nChannel = 0;
	nSizeTotalUnlock = 0;
	memset(nModTotalSongsUnlock, 0, 16000*sizeof(BYTE));
	while(_fgets(ch, 500, hFile) != NULL)
	{
		CString str;
		str = ( ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		if(str.Find(_T('*')) == -1)	// No es un comentario
		{
			if(str.Find(_T('[')) != -1)	// Channel
			{
				if(_sscanf(str, _T("[%d]"), &nChannel) != 1) {fclose(hFile); return 0;}
			}
			if(str.Find(_T(':')) != -1)	// Song
			{
				nSizeTotalUnlock++;
				int nCode = str.Find(_T(':'));
				_strcpy(chTotalSongsUnlock[nSizeTotalUnlock-1], str.Left(nCode));
				nCode = str.GetLength() - nCode - 1;
				CString stra = str.Right(nCode);
				int n[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
				if(_sscanf(stra, _T("\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d"),
					&n[0], &n[1], &n[2], &n[3], &n[4], &n[5], &n[6], &n[7], &n[8], &n[9], &n[10], &n[11], &n[12], &n[13], &n[14], &n[15]) != 16) {fclose(hFile); return 0;}
				for(int x = 0; x < 16; x++) nMilTotalSongsUnlock[(nSizeTotalUnlock-1)*16+x] = n[x];
				nChaTotalSongsUnlock[nSizeTotalUnlock-1] = nChannel==-1?0:nChannel;
			}
		}
	}
	for(int s = 0; s < nSizeTotalUnlock; s++)
	{
		CString str;
		str = GetIDTotalSongsUnlock(s);
		if(str.Find(_T("skin")) != -1)
		{
			nModTotalSongsUnlock[s*16+15] = 1;
		}
		else if(str.Find(_T("character")) != -1)
		{
			nModTotalSongsUnlock[s*16+15] = 1;
		}
		else
		{
			for(int x = 0; x < nSizeTotalGame1; x++)
			{
				if(str == GetIDTotalSongsGame1(x))
				{
					nRefExTotalSongsUnlock[s*2] = x;
					for(int y = 0; y < 10; y++)
					{
						nModTotalSongsUnlock[s*16+y] = int(IsAvalibleModeTotal(x, y));
					}
				}
			}
			/*for(int x = 0; x < nSizeTotalGame3; x++)
			{
				if(str == GetIDTotalSongsGame3(x))
				{
					nRefExTotalSongsUnlock[s*2+1] = x;
					for(int y = 0; y < 5; y++)
					{
						nModTotalSongsUnlock[s*16+y+5] = int(IsAvalibleModeTotalGame3(x, y));
					}
				}
			}*/
			for(int x = 0; x < nSizeTotalGame4; x++)
			{
				if(str == GetIDTotalSongsGame4(x))
				{
					nModTotalSongsUnlock[s*16+min(nChaTotalSongsGame4[x], 4)+10] = x+1;
				}
			}
		}
	}
	fclose(hFile);
	return 1;
}

int EnumSongsUnlock()
{
	for(int y = 0; y < 2; y++)
	{
		memset(nModSongsUnlock[y], 0, 16000*sizeof(BYTE));
		if(np[y]) for(int s = 0; s < nSizeTotalUnlock; s++)
		{
			_strcpy(chSongsUnlock[s], chTotalSongsUnlock[s]);
			for(int x = 0; x < 16; x++)
				nMilSongsUnlock[s*16+x] = nMilTotalSongsUnlock[s*16+x];
			nRefSongsUnlock[s] = s;	// Absurdo
			nRefExSongsUnlock[s*2] = nRefExTotalSongsUnlock[s*2];
			nRefExSongsUnlock[s*2+1] = nRefExTotalSongsUnlock[s*2+1];
			for(int x = 0; x < 5; x++)
			{
				if(nModTotalSongsUnlock[16*s+x])
				{
					if(!machine_sesion->nLockedSongNew[nRefExSongsUnlock[s*2]*10+x]) nModSongsUnlock[y][16*s+x] = 4;
					else nModSongsUnlock[y][16*s+x] = player_sesion[y]->nLockedSongNew[nRefExSongsUnlock[s*2]*10+x];
				}
				else
				{
					nModSongsUnlock[y][16*s+x] = 3;
				}
				if(nModTotalSongsUnlock[16*s+x+5])
				{
					if(!machine_sesion->nLockedSongNew[nRefExSongsUnlock[s*2+1]*10+x+5]) nModSongsUnlock[y][16*s+x+5] = 4;
					else nModSongsUnlock[y][16*s+x+5] = player_sesion[y]->nLockedSongNew[nRefExSongsUnlock[s*2+1]*10+x+5];
				}
				else
				{
					nModSongsUnlock[y][16*s+x+5] = 3;
				}
				if(nModTotalSongsUnlock[16*s+x+10])
				{
					if(!machine_sesion->nLockedBrainShower[nModTotalSongsUnlock[16*s+x+10]-1]) nModSongsUnlock[y][16*s+x+10] = 4;
					else nModSongsUnlock[y][16*s+x+10] = player_sesion[y]->nLockedBrainShower[nModTotalSongsUnlock[16*s+x+10]-1];
				}
				else
				{
					nModSongsUnlock[y][16*s+x+10] = 3;
				}
			}
			if(nModTotalSongsUnlock[16*s+15])
			{
				CString str;
				str = ( chSongsUnlock[s]);
				if(str.Find(_T("skin")) != -1)
				{
					CString strscan = str.Mid(4, str.GetLength());
					int nRef = 0;
					if(_sscanf(strscan, _T("%d"), &nRef) == 1)
					{
						if(!machine_sesion->nLockedAsp[nRef]) nModSongsUnlock[y][16*s+15] = 4;
						else nModSongsUnlock[y][16*s+15] = player_sesion[y]->nLockedAsp[nRef];
					}
				}
				else if(str.Find(_T("character")) != -1)
				{
					CString strscan = str.Mid(9, str.GetLength());
					int nRef = 0;
					if(_sscanf(strscan, _T("%d"), &nRef) == 1)
					{
						if(!machine_sesion->nLockedAsp[MAX_SKINS+nRef]) nModSongsUnlock[y][16*s+15] = 4;
						else nModSongsUnlock[y][16*s+15] = player_sesion[y]->nLockedAsp[MAX_SKINS+nRef];
					}
				}
				else nModSongsUnlock[y][16*s+15] = 3;
			}
			else
			{
				nModSongsUnlock[y][16*s+15] = 3;
			}
		}
		nSizeSongsUnlock = nSizeTotalUnlock;
	}
	return 1;
}

int GetAvalibleModeUnlock(int nIndexSong, int nModeUnlockSong)
{
	return 0;
}

#define MAX_TARGET_X 60.0
double AlphaFuncForXUnlock(double dx)
{
	if(abs(dx) >= MAX_TARGET_X) return 0.0;
	if(abs(dx) <= (MAX_TARGET_X/4*3)) return 1.0;
	return (MAX_TARGET_X - abs(dx)) / (MAX_TARGET_X/4);
}

double ThetaFuncForXUnlock(double dx)
{
	if(abs(dx) >= MAX_TARGET_X/4) return -19.0*dx/abs(dx);
	/*if(abs(dx) <= (MAX_TARGET_X/4))*/ return -19.0*dx*4/MAX_TARGET_X;
	//return (MAX_TARGET_X - abs(dx)) / (MAX_TARGET_X/4);
}


void ActualizateIndexShowUnlock()
{
	for(int x = 0; x < 9; x++)
	{
		nIndexShowUnlock[x] = nCurSongUnlock+x-4;
		bool bAsegureLoop = true;
		while(bAsegureLoop)
		{
			bAsegureLoop = false;
			if(nIndexShowUnlock[x] < 0)
			{
				nIndexShowUnlock[x] = nSizeSongsUnlock+nIndexShowUnlock[x];
				bAsegureLoop = true;
			}
			if(nIndexShowUnlock[x] >= nSizeSongsUnlock)
			{
				nIndexShowUnlock[x] = nIndexShowUnlock[x]-nSizeSongsUnlock;
				bAsegureLoop = true;
			}
		}
	}
}

void MoveSongsUnlock(bool bIsIzq)
{
	int nb = nCurSongUnlock;
	if(bIsIzq)
	{
		nCurSongUnlock--;
		if(nCurSongUnlock < 0) nCurSongUnlock = nSizeSongsUnlock-1;
		if(nChaSongsUnlock[nb] != nChaSongsUnlock[nCurSongUnlock]) nTimeChaUnlock = nCountUnlock;
		nModeMovUnlock = -1;
		nChosesUnlock = 0;
		/*Borrar textura sobrante*/
		if(nTexCurSongUnlock[8] != m_uNullTex)
			glDeleteTextures(1, &nTexCurSongUnlock[8]);
		ActualizateIndexShowUnlock();
		//Mover a la derecha los items
		for(int x = 7; x >= 0; x--)
		{
			nTexCurSongUnlock[x+1] = nTexCurSongUnlock[x];
		}
		// Finalmente cargar la nueva imagen
		TCHAR chFile[260];
		_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsUnlock[nIndexShowUnlock[0]]);
		nTexCurSongUnlock[0] = GenTexImage(chFile);
		nTimeCurSongUnlock = nCountUnlock;
	}
	else
	{
		nCurSongUnlock++;
		if(nCurSongUnlock >= nSizeSongsUnlock) nCurSongUnlock = 0;
		if(nChaSongsUnlock[nb] != nChaSongsUnlock[nCurSongUnlock]) nTimeChaUnlock = nCountUnlock;
		nModeMovUnlock = 1;
		nChosesUnlock = 0;
		/*Borrar textura sobrante*/
		if(nTexCurSongUnlock[0] != m_uNullTex) glDeleteTextures(1, &nTexCurSongUnlock[0]);
		ActualizateIndexShowUnlock();
		//Mover a la izquierda los items
		for(int x = 0; x < 8; x++)
		{
			nTexCurSongUnlock[x] = nTexCurSongUnlock[x+1];
		}
		// Finalmente cargar la nueva imagen
		TCHAR chFile[260];
		_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsUnlock[nIndexShowUnlock[8]]);
		nTexCurSongUnlock[8] = GenTexImage(chFile);
		nTimeCurSongUnlock = nCountUnlock;
	}
}
#define TIME_FAST_CHANGE_UNLOCK 1000
#define TIME_CHANGE_LOW_UNLOCK 350
#define TIME_CHANGE_HIGH_UNLOCK 100

void PlaySoundTransiAboutLimitUnlock(int nLimit)
{
	if(nLimit >= TIME_CHANGE_LOW_UNLOCK)
	{
		nTimeTransiSongBorderUnlock = TIME_TRANSI_SONGBORDER_UNLOCK;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
	}
	else
	{
		nTimeTransiSongBorderUnlock = TIME_CHANGE_HIGH_UNLOCK;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)6, sound_change2, 0, &channels[6]);
	}
}

////////////////////////////////////////////////
// ** Funciones principales

void CheckForUnlock()
{
	for(int k = 0; k < 2; k++) if(np[k])
	{
		int nPlayedGame1 = 0;
		int nPlayedGame2 = 0;
		int nPlayedGame3 = 0;
		int nPlayedGame4 = 0;
		int nPlayedGame5 = 0;
		for(int i = 0; i < (nSizeTotalGame1*10); i++)
		{
			if(player_sesion[k]->nArcadeRecords[i][0]) nPlayedGame1++;
		}
		for(int i = 0; i < (nSizeTotalGame2); i++)
		{
			if(player_sesion[k]->nMission[i]) nPlayedGame2++;
		}
		for(int i = 0; i < (nSizeTotalGame3*5); i++)
		{
			if(player_sesion[k]->nSpecialRecords[i][0]) nPlayedGame3++;
		}
		for(int i = 0; i < (nSizeTotalGame4); i++)
		{
			if(player_sesion[k]->nBrainShowerRecords[i][0]) nPlayedGame4++;
		}
		for(int i = 0; i < (nSizeTotalGame5); i++)
		{
			if(player_sesion[k]->nAprovedTraining[i]) nPlayedGame5++;
		}

		for(int x = 0; x < unlock->m_nSize; x++)
		{
			bool bUnlock = true;
			if((double(nPlayedGame1)/double(nSizeTotalGame1)) < unlock->m_dReqGame1[x] && bUnlock)
			{
				bUnlock = false;
			}
			if((double(nPlayedGame2)/double(nSizeTotalGame2)) < unlock->m_dReqGame2[x] && bUnlock)
			{
				bUnlock = false;
			}
			if((double(nPlayedGame3)/double(nSizeTotalGame3)) < unlock->m_dReqGame3[x] && bUnlock)
			{
				bUnlock = false;
			}
			if((double(nPlayedGame4)/double(nSizeTotalGame4)) < unlock->m_dReqGame4[x] && bUnlock)
			{
				bUnlock = false;
			}
			if((double(nPlayedGame5)/double(nSizeTotalGame5)) < unlock->m_dReqGame5[x] && bUnlock)
			{
				bUnlock = false;
			}
			if(player_sesion[k]->nExp < unlock->m_nReqExp[x] && bUnlock)
			{
				bUnlock = false;
			}
			if(player_sesion[k]->nLevel < unlock->m_nReqLvl[x] && bUnlock)
			{
				bUnlock = false;
			}
			for(int y = 0; y < unlock->m_nSizeMission[x]; y++)
			{
				if(!player_sesion[k]->nMission[mission->SearchID(unlock->m_uIDReqMission[x][y])] && bUnlock)
				{
					bUnlock = false;
					break;
				}
			}
			if(bUnlock)
			{
				for(int y = 0; y < unlock->m_nSizeCondUnlock[x]; y++)
				{
					int nID = unlock->m_uCondUnlock[x][y];
					int nSet = unlock->m_dwCondUnlockModes[x][y]&0xF;
					bool bSet[5];
					bSet[0] = ((unlock->m_dwCondUnlockModes[x][y]>>4)&0xF)?true:false;
					bSet[1] = ((unlock->m_dwCondUnlockModes[x][y]>>8)&0xF)?true:false;
					bSet[2] = ((unlock->m_dwCondUnlockModes[x][y]>>12)&0xF)?true:false;
					bSet[3] = ((unlock->m_dwCondUnlockModes[x][y]>>16)&0xF)?true:false;
					bSet[4] = ((unlock->m_dwCondUnlockModes[x][y]>>20)&0xF)?true:false;
					if(nSet == 0)
					{
						bool bNotify = false;
						BYTE *nPar;
						nPar = new BYTE[16];
						for(int z = 0; z < 16; z++) nPar[z] = 0;
						for(int z = 0; z < 5; z++) if(player_sesion[k]->nLockedSongNew[nID*10+z]==2 && bSet[z])
						{
							player_sesion[k]->nLockedSongNew[nID*10+z] = 1;
							nPar[z] = 1;
							bNotify = true;
						}
						if(bNotify) RegisterUnlock(0, nID, nPar);
						delete nPar;
					}
					if(nSet == 1)
					{
						bool bNotify = false;
						BYTE *nPar;
						nPar = new BYTE[16];
						for(int z = 0; z < 16; z++) nPar[z] = 0;
						for(int z = 0; z < 5; z++) if(player_sesion[k]->nLockedSongNew[nID*10+z+5]==2 && bSet[z])
						{
							player_sesion[k]->nLockedSongNew[nID*10+z+5] = 1;
							nPar[z+5] = 1;
							bNotify = true;
						}
						if(bNotify) RegisterUnlock(1, nID, nPar);
						delete nPar;
					}
					if(nSet == 2)
					{
						bool bNotify = false;
						BYTE *nPar;
						nPar = new BYTE[16];
						for(int z = 0; z < 16; z++) nPar[z] = 0;
						if(player_sesion[k]->nLockedBrainShower[nID] == 2 && bSet[0])
						{
							player_sesion[k]->nLockedBrainShower[nID] = 1;
							bNotify = true;
						}
						nPar[10+nChaTotalSongsGame4[nID]] = 1;
						if(bNotify) RegisterUnlock(2, nID, nPar);
						delete nPar;
					}
				}
				for(int y = 0; y < unlock->m_nSizeCondAspUnlock[x]; y++)
				{
					if(player_sesion[k]->nLockedAsp[unlock->m_uCondAspUnlock[x][y]] == 2)
						player_sesion[k]->nLockedAsp[unlock->m_uCondAspUnlock[x][y]] = 1;
				}
			}
		}
	}
}

void RegisterUnlock(int nRef, int nID, BYTE* nPar)
{
	CString strID;
	int nSup = 0, nInf = 0;
	switch(nRef)
	{
	case 0:
		strID = GetIDTotalSongsGame1(nID);
		nSup = 0; nInf = 5;
		break;
	case 1:
		strID = GetIDTotalSongsGame1(nID);
		nSup = 5; nInf = 10;
		break;
	case 2:
		strID = GetIDTotalSongsGame4(nID);
		nSup = 10; nInf = 15;
		break;
	default:
		strID = _T("\0");
		nSup = 0; nInf = 0;
		break;
	}
	int nIndex = -1;
	for(int x = 0; x < nSizeUnlock; x++)
	{
		if(strIDUnlock[x] == strID)
		{
			nIndex = x;
			break;
		}
	}
	if(nIndex == -1)
	{
		nIndex = nSizeUnlock;
		CString* strIDs = NULL;
		if(nSizeUnlock)
		{
			strIDs = new CString[nSizeUnlock];
			for(int x = 0; x < nSizeUnlock; x++)
			{
				strIDs[x] = strIDUnlock[x];
			}
			delete[] strIDUnlock;
		}
		nSizeUnlock++;
		uTexUnlock = (GLuint*)realloc(uTexUnlock, nSizeUnlock*sizeof(int));
		nParUnlock = (BYTE*)realloc(nParUnlock, nSizeUnlock*sizeof(BYTE)*16);
		strIDUnlock = new CString[nSizeUnlock];
		if(nSizeUnlock > 1)
		{
			for(int x = 0; x < nIndex; x++)
			{
				if(strIDs) strIDs[x] = strIDUnlock[x];
			}
			delete[] strIDs;
		}
		uTexUnlock[nIndex] = 0;
		for(int x = 0; x < 16; x++) nParUnlock[nIndex*16+x] = 0;
		strIDUnlock[nIndex] = _T("\0");
	}
	CString str;
	str.Format(_T("song/preview/%ls.PNG"), strID);
	uTexUnlock[nIndex] = GenTexImage(str);
	for(int x = nSup; x < nInf; x++)
	{
		if(!nParUnlock[nIndex*16+x] && nPar[x])
			nParUnlock[nIndex*16+x] = 1;
	}
	strIDUnlock[nIndex] = strID;
}

void SwitchSongSplash(bool bIsIzq)
{
	if(bIsIzq)
	{
		nSongUnlock--;
		if(nSongUnlock < 0) nSongUnlock = nSizeUnlock-1;
	}
	else
	{
		nSongUnlock++;
		if(nSongUnlock >= nSizeUnlock) nSongUnlock = 0;
	}
	nTimeSongUnlock = nCountUnlock;
}

int DrawUnlocker()
{
	if(!np[0] && !np[1]) return 1;
	nCountUnlock += dwTick-dwLastTick;
	if(movie_unlockbg->bLoad)
	{
		movie_unlockbg->Update(dwWait);
		movie_unlockbg->GrabAVIFrame();
		glBindTexture(GL_TEXTURE_2D, movie_unlockbg->tex);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
		glEnd();
	}

	double dApt = 0.0;
	static __int64 nTimeShow = 0;
	static bool bShow = false;
	__int64 nTransBorder = nCountUnlock - nTimeCurSongUnlock;
	int nOrder[] = {0,1,2,3,8,7,6,5,4};
	if(nChosesUnlock < CHOOSES_UNLOCK)
	{
		for(int i = 0; i < 9; i++)
		{
			int x = nOrder[i];
			double dTargetx = double(x-4)*MAX_TARGET_X/4;
			if(nCountUnlock >= TIME_START_SELECTOR_UNLOCK && nCountUnlock < (TIME_START_SELECTOR_UNLOCK+TIME_TRANSI_SELECTOR_UNLOCK))
			{
				if(x == 4)
				{
					if(nCountUnlock < (TIME_TRANSI_START_MAIN_UNLOCK+TIME_START_SELECTOR_UNLOCK))
					{
						dApt = pow(double(nCountUnlock-TIME_START_SELECTOR_UNLOCK)/TIME_TRANSI_START_MAIN_UNLOCK, 2);
						glPushMatrix();
						glTranslated(0.0, 0.0, -60.0);
						glScaled(1.25+(1-dApt), 1.25+(1-dApt), 1.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBindTexture(GL_TEXTURE_2D, nTexCurSongUnlock[x]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
						glEnd();
						glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[7+nChaSongsUnlock[nIndexShowUnlock[x]]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nCountUnlock >= (TIME_TRANSI_START_MAIN_UNLOCK+TIME_START_SELECTOR_UNLOCK))
					{
						glPushMatrix();
						glTranslated(0.0, 0.0, -60.0);
						glScaled(1.25, 1.25, 1.0);
						glBindTexture(GL_TEXTURE_2D, nTexCurSongUnlock[x]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
						glEnd();
						glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[7+nChaSongsUnlock[nIndexShowUnlock[x]]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
						glEnd();
						glPopMatrix();
					}
				}
				else
				{
					__int64 nStartTransi = TIME_START_SELECTOR_UNLOCK+TIME_TRANSI_SELECTOR_UNLOCK/4*(abs(x-4)-1);
					__int64 nLimitTransi = TIME_START_SELECTOR_UNLOCK+TIME_TRANSI_SELECTOR_UNLOCK/4*(abs(x-4));
					if(nCountUnlock >= nStartTransi && nCountUnlock < nLimitTransi)
					{
						dApt = double(nCountUnlock-nStartTransi)/double(nLimitTransi-nStartTransi);
						double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(1.0-dApt)+dTargetx;
						double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
						double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
						double dtheta = ThetaFuncForXUnlock(dx);
						double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
						double dalpha = AlphaFuncForXUnlock(dx);
						glPushMatrix();
						glTranslated(dx, dy, dz-60.0);
						glRotated(dtheta, 0.0, 1.0, 0.0);
						glScaled(dscale, dscale, 1.0);
						glColor4d(1.0, 1.0, 1.0, dalpha);
						glBindTexture(GL_TEXTURE_2D, nTexCurSongUnlock[x]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
						glEnd();
						glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[7+nChaSongsUnlock[nIndexShowUnlock[x]]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nCountUnlock >= nLimitTransi)
					{
						double dx = dTargetx;
						double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
						double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
						double dtheta = ThetaFuncForXUnlock(dx);
						double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
						double dalpha = AlphaFuncForXUnlock(dx);
						glPushMatrix();
						glTranslated(dx, dy, dz-60.0);
						glRotated(dtheta, 0.0, 1.0, 0.0);
						glScaled(dscale, dscale, 1.0);
						glColor4d(1.0, 1.0, 1.0, dalpha);
						glBindTexture(GL_TEXTURE_2D, nTexCurSongUnlock[x]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
						glEnd();
						glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[7+nChaSongsUnlock[nIndexShowUnlock[x]]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
				}
			}
			if(nCountUnlock >= (TIME_START_SELECTOR_UNLOCK+TIME_TRANSI_SELECTOR_UNLOCK))
			{
				double dx = dTargetx;
				if(nModeMovUnlock!=0)
				{
					if(nTransBorder < nTimeTransiSongBorderUnlock)
					{
						dx += double(nModeMovUnlock)*MAX_TARGET_X/4*(1.0-double(nTransBorder)/double(nTimeTransiSongBorderUnlock));
					}
				}
				double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
				double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
				double dtheta = ThetaFuncForXUnlock(dx);
				double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
				double dalpha = AlphaFuncForXUnlock(dx);
				glPushMatrix();
				glTranslated(dx, dy, dz-60.0);
				glRotated(dtheta, 0.0, 1.0, 0.0);
				glScaled(dscale, dscale, 1.0);
				glColor4d(1.0, 1.0, 1.0, dalpha);
				if(x == 4 && nTransBorder > TIME_SHOW_PREVIEW_UNLOCK && movie_songpreviewunlock->bLoad && movie_songpreviewunlock->bResize && bShow)
				{
#define TIME_SHOWIMAGE_SONGBORDER_UNLOCK 500
					movie_songpreviewunlock->Update(dwWait);
					movie_songpreviewunlock->GrabAVIFrame();
					__int64 nCaretCover = movie_songpreviewunlock->dwNext;
					glBindTexture(GL_TEXTURE_2D, movie_songpreviewunlock->tex);

					glColor4d(1.0, 1.0, 1.0, 1.0);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
					glEnd();
					if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_UNLOCK)
					{
						dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_UNLOCK;
						glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
						glBindTexture(GL_TEXTURE_2D, nTexCurSongUnlock[x]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
						glEnd();
					}
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, nTexCurSongUnlock[x]);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
					glEnd();
				}
				glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[7+nChaSongsUnlock[nIndexShowUnlock[x]]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				if(x == 4 && nTransBorder > TIME_SHOW_PREVIEW_UNLOCK)
				{
					#define TIME_SHOWNAME_BESTSCORE_UNLOCK 500
					if(nTransBorder < (TIME_SHOW_PREVIEW_UNLOCK+TIME_SHOWNAME_BESTSCORE_UNLOCK))
					{
						dApt = double(nTransBorder-TIME_SHOW_PREVIEW_UNLOCK)/TIME_SHOWNAME_BESTSCORE_UNLOCK;
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[43]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
					}
					if(nTransBorder >= (TIME_SHOW_PREVIEW_UNLOCK+TIME_SHOWNAME_BESTSCORE_UNLOCK))
					{
						glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[43]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
						glEnd();
					}
					__int64 nModeTime = nCountUnlock - nTransBorder - TIME_SHOW_PREVIEW_UNLOCK;
					if(nModeTime >= 0 && nModeTime < (TIME_SHOWNAME_BESTSCORE_UNLOCK) && bShow)
					{
						dApt = double(nModeTime)/TIME_SHOWNAME_BESTSCORE_UNLOCK;
						glColor4d(1.0, 1.0, 1.0, dApt);
						font_Micro->glDrawText(10.599, -9.0625, 9.05331, 9.0625, 0.0, 10.076, 0.0, 1.0, 1.5, CString(_T("Required mileage")), DT_CENTER|DT_VCENTER);
						if(nMileageSongMachineUnlock != 0)
						{
							CString str;
							str.Format(_T("%d"), nMileageSongMachineUnlock);
							font_Micro->glDrawText(9.05331, -9.0625, 7.50762, 9.0625, 0.0, 8.53046, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
						}
						for(int k = 0; k < 2; k++) if(np[k])
						{
							if(nMileageSongPlayerUnlock[k] != 0)
							{
								CString str;
								str.Format(_T("%d"), nMileageSongPlayerUnlock[k]);
								font_Micro->glDrawText(-9.05331, -9.0625+(k?7.25:-7.25), -10.599, 9.0625+(k?7.25:-7.25), (k?7.25:-7.25), -9.57615, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
							}
						}
						glColor4d(1.0, 1.0, 1.0, 1.0);
					}
					if(nModeTime >= (TIME_SHOWNAME_BESTSCORE_UNLOCK) && bShow)
					{
						font_Micro->glDrawText(10.599, -9.0625, 9.05331, 9.0625, 0.0, 10.076, 0.0, 1.0, 1.5, CString(_T("Required mileage")), DT_CENTER|DT_VCENTER);
						if(nMileageSongMachineUnlock != 0)
						{
							CString str;
							str.Format(_T("%d"), nMileageSongMachineUnlock);
							font_Micro->glDrawText(9.05331, -9.0625, 7.50762, 9.0625, 0.0, 8.53046, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
						}
						for(int k = 0; k < 2; k++) if(np[k])
						{
							if(nMileageSongPlayerUnlock[k] != 0)
							{
								CString str;
								str.Format(_T("%d"), nMileageSongPlayerUnlock[k]);
								font_Micro->glDrawText(-9.05331, -9.0625+(k?7.25:-7.25), -10.599, 9.0625+(k?7.25:-7.25), (k?7.25:-7.25), -9.57615, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
							}
						}
					}
				}
				glPopMatrix();
			}
		}
		if(nTransBorder >= 0 && nTransBorder <= TIME_GLOUTH_SONGBORDER_UNLOCK && nChosesUnlock == 0)
		{
			dApt = double(nTransBorder)/TIME_GLOUTH_SONGBORDER_UNLOCK;
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[4]->GetCurrentTex());
			glPushMatrix();
			glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
			glScaled(1.25+0.5*(dApt), 1.25+0.5*(dApt), 1.0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, -60.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, -60.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, -60.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, -60.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glPopMatrix();
		}
		if(nTransBorder > TIME_TRANSI_SONGBORDER_UNLOCK && nTransBorder <= (TIME_TRANSI_SONGBORDER_UNLOCK+TIME_GLOUTH_SONGBORDER_UNLOCK) && nChosesUnlock == 0)
		{
			dApt = double(nTransBorder-TIME_TRANSI_SONGBORDER_UNLOCK)/TIME_GLOUTH_SONGBORDER_UNLOCK;
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[4]->GetCurrentTex());
			glPushMatrix();
			glColor4d(1.0, 1.0, 1.0, dApt);
			glScaled(1.25+0.5*(1-dApt), 1.25+0.5*(1-dApt), 1.0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, -60.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, -60.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, -60.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, -60.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glPopMatrix();
		}
#define TIME_GLOUTH_PER_SONGBORDER_UNLOCK 1500
		if(nTransBorder > (TIME_TRANSI_SONGBORDER_UNLOCK+TIME_GLOUTH_SONGBORDER_UNLOCK))
		{
			dApt = abs(double((nTransBorder-TIME_TRANSI_SONGBORDER_UNLOCK-TIME_GLOUTH_SONGBORDER_UNLOCK)%TIME_GLOUTH_PER_SONGBORDER_UNLOCK-TIME_GLOUTH_PER_SONGBORDER_UNLOCK/2))*2/TIME_GLOUTH_PER_SONGBORDER_UNLOCK;
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[4]->GetCurrentTex());
			glPushMatrix();
			glColor4d(1.0, 1.0, 1.0, dApt);
			glScaled(1.25+0.1*(1-dApt), 1.25+0.1*(1-dApt), 1.0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, -60.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, -60.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, -60.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, -60.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glPopMatrix();
		}
		/*if(nTransBorder > (TIME_TRANSI_SONGBORDER_UNLOCK+TIME_GLOUTH_SONGBORDER_UNLOCK) && nChosesUnlock == 1)
		{
			welres->skin[0]->Update(dwWait);
			welres->skin[1]->Update(dwWait);
			welres->skin[3]->Update(dwWait);
			dApt = double((nCountUnlock-nTimeChosesUnlock[nChosesUnlock-1])%TIME_GLOUTH_SONGBORDER_UNLOCK)/TIME_GLOUTH_SONGBORDER_UNLOCK;
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[4]->GetCurrentTex());
			glPushMatrix();
			glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
			glScaled(1.25+0.4*(dApt), 1.25+0.4*(dApt), 1.0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, -60.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, -60.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, -60.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, -60.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, welres->skin[1]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-12.5, -2.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 12.5, -2.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 12.5,  2.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-12.5,  2.5, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, welres->skin[3]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-7.5, 2.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 12.5, 2.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 12.5, -2.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-7.5, -2.5, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, welres->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-12.5, 2.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(-7.5, 2.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(-7.5, -2.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-12.5, -2.5, -50.0);
			glEnd();
		}*/
		if(nTransBorder >= TIME_SHOW_PREVIEW_UNLOCK)
		{
			if(!bShow)
			{
				TCHAR chFile[260];
				_sprintf(chFile, _T("song/%ls/%ls_P.avi"), chSongsUnlock[nCurSongUnlock], chSongsUnlock[nCurSongUnlock]);
				if(movie_songpreviewunlock->bLoad)
				{
					movie_songpreviewunlock->CloseAVI();
					delete movie_songpreviewunlock;
					movie_songpreviewunlock = new CMovieStream2();
				}
				movie_songpreviewunlock->OpenAVI(chFile);
				movie_songpreviewunlock->bLoop = true;
				nTimeShow = nCountUnlock;
				bShow = true;
				_sprintf(chFile, _T("song/%ls/%ls_P.mp3"), chSongsUnlock[nCurSongUnlock], chSongsUnlock[nCurSongUnlock]);
				if(bLoadMusicCoverUnlock)
				{
					bLoadMusicCoverUnlock = false;
					FMOD_Sound_Release(music_coverUnlock);
				}
				FMOD_RESULT resc;
#ifdef UNICODE
				char chAnsiFile[260];
				UnicodeToAnsi(chFile, chAnsiFile);
				resc = FMOD_System_CreateStream(sys, chAnsiFile, FMOD_HARDWARE | FMOD_2D, 0, &music_coverUnlock);
#else
				resc = FMOD_System_CreateStream(sys, chFile, FMOD_HARDWARE | FMOD_2D, 0, &music_coverUnlock);
#endif
				if(ERRCHECK(resc))
				{
					bLoadMusicCoverUnlock = true;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, music_coverUnlock, 0, &channels[9]);
				}
				else
				{
					bLoadMusicCoverUnlock = false;
					_printf(_T("No se puede cargar el archivo \"%ls\"\n"), chFile);
				}
				strDispUnlock.Format(_T("%ls - %ls - %ls"), GetNameSongsUnlock(nCurSongUnlock), GetArtistSongsUnlock(nCurSongUnlock), GetBPMSongsUnlock(nCurSongUnlock));
				SetMileageUnlock();
			}
		}



#define TIME_START_UPDO_UNLOCK 0
#define TIME_TRANSI_UPDO_UNLOCK 500
#define TIME_GLOUTH_UPDO_UNLOCK 500
		if(nCountUnlock >= TIME_START_UPDO_UNLOCK && nCountUnlock < (TIME_START_UPDO_UNLOCK+TIME_TRANSI_UPDO_UNLOCK))	// Transi Up and Down Unlock
		{
			dApt = (double(nCountUnlock)-TIME_START_UPDO_UNLOCK)/(TIME_TRANSI_UPDO_UNLOCK);
			glPushMatrix();
			glScaled(1.0+0.8*(1.0-dApt), 1.0+0.8*(1.0-dApt), 1.0);
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[34+nChaSongsUnlock[nCurSongUnlock]]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[16+nChaSongsUnlock[nCurSongUnlock]]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[0]->texture[0]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-26.1-6.4,  18.2+6.4, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(-26.1+6.4,  18.2+6.4, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(-26.1+6.4,  18.2-6.4, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-26.1-6.4,  18.2-6.4, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[1]->texture[0]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d( 26.1-6.4,  18.2+6.4, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 26.1+6.4,  18.2+6.4, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 26.1+6.4,  18.2-6.4, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d( 26.1-6.4,  18.2-6.4, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[2]->texture[0]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-19.5-6.4, -21.5+6.4, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(-19.5+6.4, -21.5+6.4, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(-19.5+6.4, -21.5-6.4, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-19.5-6.4, -21.5-6.4, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[3]->texture[0]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d( 19.5-6.4, -21.5+6.4, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 19.5+6.4, -21.5+6.4, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 19.5+6.4, -21.5-6.4, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d( 19.5-6.4, -21.5-6.4, -50.0);
			glEnd();
			/*Dibujo de los modos*/
			/*Dibujo de la elección*/
			glPopMatrix();
		}
		if(nCountUnlock >= (TIME_START_UPDO_UNLOCK+TIME_TRANSI_UPDO_UNLOCK))	// Up and Down Unlock
		{
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[34+nChaSongsUnlock[nCurSongUnlock]]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
			glEnd();
			if(nTransBorder > TIME_SHOW_PREVIEW_UNLOCK && bShow)
			{
#define SIZE_TEXT_UNLOCK 1.5
#define VEL_SHOWTEXT_UNLOCK 0.01
#define TIME_WAITTEXTNAME_UNLOCK 1500
				__int64 nTimeName = nTransBorder-TIME_SHOW_PREVIEW_UNLOCK-TIME_WAITTEXTNAME_UNLOCK;
				if(nTimeName < 0) nTimeName = 0;
				double dSizeTex = font_Car->CalcSingleLine(strDispUnlock, SIZE_TEXT_UNLOCK);
				DWORD t1 = DWORD(double(48.0/VEL_SHOWTEXT_UNLOCK));
				DWORD t2 = DWORD(double(dSizeTex/VEL_SHOWTEXT_UNLOCK));
				DWORD t = t1+t2;
				DWORD nShowText = (DWORD(nTimeName)+t1)%t;
				double dx = -VEL_SHOWTEXT_UNLOCK*double(nShowText)+39.0;
				font_Car->glDrawText(-24.1, -17.9, -27.6, 17.9, -8.375+dx, -25.9, -50.0, 1.0, SIZE_TEXT_UNLOCK, strDispUnlock, DT_VCENTER);
			}
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[16+nChaSongsUnlock[nCurSongUnlock]]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
			glEnd();

			DWORD dwNext = 0;
			DWORD dwFrame = 0;

			dwNext = (DWORD)(nCountUnlock-max(nTimeEnArrowsUnlock[1],nTimeEnArrowsUnlock[6]));
			if(dwNext>=(DWORD)(nTimeTransiSongBorderUnlock)) dwFrame = 0;
			else dwFrame = (dwNext*(DWORD)unlockbasres->skin[0]->m_nSize)/(DWORD)nTimeTransiSongBorderUnlock;
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[0]->texture[dwFrame]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-26.1-6.4,  18.2+6.4, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(-26.1+6.4,  18.2+6.4, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(-26.1+6.4,  18.2-6.4, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-26.1-6.4,  18.2-6.4, -50.0);
			glEnd();

			dwNext = (DWORD)(nCountUnlock-max(nTimeEnArrowsUnlock[3],nTimeEnArrowsUnlock[8]));
			if(dwNext>=(DWORD)(nTimeTransiSongBorderUnlock)) dwFrame = 0;
			else dwFrame = (dwNext*(DWORD)unlockbasres->skin[1]->m_nSize)/(DWORD)nTimeTransiSongBorderUnlock;
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[1]->texture[dwFrame]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d( 26.1-6.4,  18.2+6.4, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 26.1+6.4,  18.2+6.4, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 26.1+6.4,  18.2-6.4, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d( 26.1-6.4,  18.2-6.4, -50.0);
			glEnd();

			dwNext = (DWORD)(nCountUnlock-max(nTimeEnArrowsUnlock[0],nTimeEnArrowsUnlock[5]));
			if(bEnArrowsUnlock[0] || bEnArrowsUnlock[5]) dwNext = dwNext%nTimeTransiSongBorderUnlock;
			if(dwNext>=(DWORD)(nTimeTransiSongBorderUnlock)) dwFrame = 0;
			else dwFrame = (dwNext*(DWORD)unlockbasres->skin[2]->m_nSize)/(DWORD)nTimeTransiSongBorderUnlock;
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[2]->texture[dwFrame]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-19.5-6.4, -21.5+6.4, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(-19.5+6.4, -21.5+6.4, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(-19.5+6.4, -21.5-6.4, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-19.5-6.4, -21.5-6.4, -50.0);
			glEnd();

			dwNext = (DWORD)(nCountUnlock-max(nTimeEnArrowsUnlock[4],nTimeEnArrowsUnlock[9]));
			if(bEnArrowsUnlock[4] || bEnArrowsUnlock[9]) dwNext = dwNext%nTimeTransiSongBorderUnlock;
			if(dwNext>=(DWORD)(nTimeTransiSongBorderUnlock)) dwFrame = 0;
			else dwFrame = (dwNext*(DWORD)unlockbasres->skin[3]->m_nSize)/(DWORD)nTimeTransiSongBorderUnlock;
			glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[3]->texture[dwFrame]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d( 19.5-6.4, -21.5+6.4, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 19.5+6.4, -21.5+6.4, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 19.5+6.4, -21.5-6.4, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d( 19.5-6.4, -21.5-6.4, -50.0);
			glEnd();

			/*Dibujo de los modos*/

#define TIME_TRANSI_SELECTORMODE_UNLOCK 300
#define TIME_START_GRAPH_UNLOCK 100
#define TIME_TRANSI_GRAPH_UNLOCK 500
#define TIME_GLOUTH_GRAPH_UNLOCK 300
#define TIME_START_LINEGRAPH_UNLOCK 100
#define TIME_TRANSI_LINEGRAPH_UNLOCK 500
#define TIME_TRUNC_LINEGRAPH_UNLOCK 1200
			/*Dibujo de la elección*/
			for(int k = 0; k < 2; k++) if(np[k])
			{
#define TIME_START_SHOWMODES_UNLOCK TIME_TRANSI_SONGBORDER_UNLOCK
#define TIME_TRANSI_SHOWMODES_UNLOCK 300
#define TIME_GLOUTH_SHOWMODES_UNLOCK 200
				if(nTransBorder >= TIME_START_SHOWMODES_UNLOCK && nTransBorder < (TIME_START_SHOWMODES_UNLOCK+TIME_TRANSI_SHOWMODES_UNLOCK))
				{
					dApt = double(nTransBorder-TIME_START_SHOWMODES_UNLOCK)/TIME_TRANSI_SHOWMODES_UNLOCK;
					glColor4d(1.0, 1.0, 1.0, dApt);
					glPushMatrix();
					glTranslated(k?20.0:-20.0+(k?5.0:-5.0)*(1-dApt), 0.0, 0.0);
					glScaled(0.5+0.5*dApt, 0.5+0.5*dApt, 1.0);
					glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[44]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -10.0,  7.5, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  10.0,  7.5, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  10.0, -7.5, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -10.0, -7.5, -50.0);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
				if(nTransBorder >= (TIME_START_SHOWMODES_UNLOCK+TIME_TRANSI_SHOWMODES_UNLOCK))
				{
					glPushMatrix();
					glTranslated(k?20.0:-20.0, 0.0, 0.0);
					glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[44]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -10.0,  7.5, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  10.0,  7.5, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  10.0, -7.5, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -10.0, -7.5, -50.0);
					glEnd();
					for(int a = 0; a < 16; a++) if(nModSongsUnlock[k][nCurSongUnlock*16+a] != 3)
					{
						glPushMatrix();
						switch(a)
						{
						case 15:
							glTranslated(7.5, -0.9375, 0.0);
							break;
						default:
							glTranslated(-7.5+5.0*double(a/5), 2.8125-1.875*double(a%5), 0.0);
							break;
						}
						glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[nModSongsUnlock[k][nCurSongUnlock*16+a]==4?49:(46+nModSongsUnlock[k][nCurSongUnlock*16+a])]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -0.59375,  0.59375, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  0.59375,  0.59375, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  0.59375, -0.59375, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -0.59375, -0.59375, -50.0);
						glEnd();
						glPopMatrix();
					}
#define TIME_TRANSILOOP_SELECTOR_UNLOCK 800
					if(nChosesUnlock == 1)
					{
						dApt = abs(double((nCountUnlock-nTimeChosesUnlock[0])%TIME_TRANSILOOP_SELECTOR_UNLOCK)/TIME_TRANSILOOP_SELECTOR_UNLOCK-0.5)*2.0;
						glPushMatrix();
						if(nModeCurSongUnlock[k] == 16)
						{
							glScaled(1.0+0.2*dApt, 1.0+0.2*dApt, 1.0);
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[44]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -10.0,  7.5, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  10.0,  7.5, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  10.0, -7.5, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -10.0, -7.5, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						else
						{
							switch(nModeCurSongUnlock[k])
							{
							case 15:
								glTranslated(7.5, -0.9375, 0.0);
								break;
							default:
								glTranslated(-7.5+5.0*double(nModeCurSongUnlock[k]/5), 2.8125-1.875*double(nModeCurSongUnlock[k]%5), 0.0);
								break;
							}
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[50+k]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -0.59375,  0.59375, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  0.59375,  0.59375, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  0.59375, -0.59375, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -0.59375, -0.59375, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						glPopMatrix();
						for(int a = 0; a < 16; a++) if(nSelectedModSongUnlock[k][a])
						{
							dApt = abs(double((nCountUnlock-nTimeChosesUnlock[0]+50*a)%TIME_TRANSILOOP_SELECTOR_UNLOCK)/TIME_TRANSILOOP_SELECTOR_UNLOCK-0.5)*2.0;
							glPushMatrix();
							switch(a)
							{
							case 15:
								glTranslated(7.5, -0.9375, 0.0);
								break;
							default:
								glTranslated(-7.5+5.0*double(a/5), 2.8125-1.875*double(a%5), 0.0);
								break;
							}
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[52+k]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -0.59375,  0.59375, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  0.59375,  0.59375, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  0.59375, -0.59375, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -0.59375, -0.59375, -50.0);
							glEnd();
							glPopMatrix();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						// Dibujo del Mileage en gasto
						__int64 nTimeMileageShow = nCountUnlock - nTimeChosesUnlock[0];
#define TIME_TRANSI_MILEAGESHOW_UNLOCK 300
						if(nTimeMileageShow >= 0 && nTimeMileageShow <= TIME_TRANSI_MILEAGESHOW_UNLOCK)
						{
							dApt = double(nTimeMileageShow)/TIME_TRANSI_MILEAGESHOW_UNLOCK;
							glPushMatrix();
							glTranslated(0.0, -10.0, 0.0);
							glScaled(1.0, dApt, 1.0);
							glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[54]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -15.0,  1.875, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  15.0,  1.875, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  15.0, -1.875, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -15.0, -1.875, -50.0);
							glEnd();
							glPopMatrix();
						}
						if(nTimeMileageShow > TIME_TRANSI_MILEAGESHOW_UNLOCK)
						{
							glPushMatrix();
							glTranslated(0.0, -10.0, 0.0);
							glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[54]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -15.0,  1.875, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  15.0,  1.875, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  15.0, -1.875, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -15.0, -1.875, -50.0);
							glEnd();
							CString str;
							str.Format(_T("%.8d"), player_sesion[k]->nMileage);
							glPushMatrix();
							glTranslated(-0.6683, 0.0, 0.0);
							font_Micro->glDrawText(1.875, -15.0, -1.875, 15.0, 0.0, 0.0, -50.0, 1.0, 1.7, str, DT_CENTER|DT_VCENTER);
							glPopMatrix();
							str.Format(_T("%.4d"), nMileageUnlock[k]);
							glPushMatrix();
							glTranslated(9.8019, 0.0, 0.0);
							glColor4d(1.0, 0.0, 0.0, 1.0);
							font_Micro->glDrawText(1.875, -15.0, -1.875, 15.0, 0.0, 0.0, -50.0, 1.0, 1.7, str, DT_CENTER|DT_VCENTER);
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
							glPopMatrix();
						}
					}
					if(nTransBorder < (TIME_START_SHOWMODES_UNLOCK+TIME_TRANSI_SHOWMODES_UNLOCK+TIME_GLOUTH_SHOWMODES_UNLOCK))
					{
						dApt = double(nTransBorder-TIME_START_SHOWMODES_UNLOCK-TIME_TRANSI_SHOWMODES_UNLOCK)/TIME_GLOUTH_SHOWMODES_UNLOCK;
						glPushMatrix();
						glScaled(1.0+0.2*dApt, 1.0+0.2*dApt, 1.0);
						glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[45]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -10.0,  7.5, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  10.0,  7.5, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  10.0, -7.5, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -10.0, -7.5, -50.0);
						glEnd();
						glPopMatrix();
					}
					glPopMatrix();
				}
			}
			if(nModeUnlockedUnlock != 0)
			{
#define TIME_TRANSI_UNLOCKED_UNLOCK 300
#define TIME_DUR_UNLOCKED_UNLOCK 500
				__int64 nTransUnlocker = nCountUnlock - nTimeUnlockedUnlock;
				if(nTransUnlocker >= 0 && nTransUnlocker < TIME_TRANSI_UNLOCKED_UNLOCK)
				{
					dApt = double(nTransUnlocker)/TIME_TRANSI_UNLOCKED_UNLOCK;
					glPushMatrix();
					glScaled(1.0, dApt, 1.0);
					glColor4d(1.0, 1.0, 1.0, dApt);
					glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[54+nModeUnlockedUnlock]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -30.0,  3.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  30.0,  3.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  30.0, -3.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -30.0, -3.75, -50.0);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
				if(nTransUnlocker >= TIME_TRANSI_UNLOCKED_UNLOCK && nTransUnlocker < (TIME_TRANSI_UNLOCKED_UNLOCK+TIME_DUR_UNLOCKED_UNLOCK))
				{
					glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[54+nModeUnlockedUnlock]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -30.0,  3.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  30.0,  3.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  30.0, -3.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -30.0, -3.75, -50.0);
					glEnd();
				}
				if(nTransUnlocker >= (TIME_TRANSI_UNLOCKED_UNLOCK+TIME_DUR_UNLOCKED_UNLOCK) && nTransUnlocker < (TIME_TRANSI_UNLOCKED_UNLOCK+TIME_DUR_UNLOCKED_UNLOCK+TIME_TRANSI_UNLOCKED_UNLOCK))
				{
					dApt = 1.0-double(nTransUnlocker-TIME_TRANSI_UNLOCKED_UNLOCK-TIME_DUR_UNLOCKED_UNLOCK)/TIME_TRANSI_UNLOCKED_UNLOCK;
					glPushMatrix();
					glScaled(1.0, dApt, 1.0);
					glColor4d(1.0, 1.0, 1.0, dApt);
					glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[54+nModeUnlockedUnlock]->GetCurrentTex());
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

			__int64 nTrans1 = nCountUnlock - nTimeChaUnlock;
			if(nTrans1 <= TIME_GLOUTH_UPDO_UNLOCK)
			{
				dApt = (double(nTrans1))/(TIME_GLOUTH_UPDO_UNLOCK);
				glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[25+nChaSongsUnlock[nCurSongUnlock]]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
				glScaled(1.0+0.05*(dApt), 1.0+0.05*(dApt), 1.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}

			CString strDispTime;
			strDispTime.Format(_T("%.2d"), int((nMaxUnlock - nCountUnlock)/1000));
			font_Micro->glDrawText(30.0, -40.0, -30.0, 40.0, 0, 17.75, -50.0, 1.0, 1.5, strDispTime, DT_VCENTER|DT_CENTER);
		}
		if(nCountUnlock >= TIME_START_START_UNLOCK)	// Reconocimiento de comandos
		{
			bool bNewCommand = false;
			static bool bCh[] = {false, false, false, false, false,
								false, false, false, false, false};
			for(int x = 0; x < 2; x++)
			{
				if(np[x] > 1)
				{
					if(x == 0)
					{
						for(int y = 0; y < 5; y++)
						{
							if(bArrows[y] && !bEnArrowsUnlock[y])
							{
								bEnArrowsUnlock[y] = true;
								nTimeEnArrowsUnlock[y] = nCountUnlock;
							}
							if(!bArrows[y] && bEnArrowsUnlock[y])
							{
								bEnArrowsUnlock[y] = false;
								bCh[y] = false;
								if(y == 0 || y == 4)
								{
									nTimeTransiSongBorderUnlock = TIME_TRANSI_SONGBORDER_UNLOCK;
								}
							}
						}
					}
					if(x == 1)
					{
						for(int y = 5; y < 10; y++)
						{
							if(bArrows[y] && !bEnArrowsUnlock[y])
							{
								bEnArrowsUnlock[y] = true;
								nTimeEnArrowsUnlock[y] = nCountUnlock;
							}
							if(!bArrows[y] && bEnArrowsUnlock[y])
							{
								bEnArrowsUnlock[y] = false;
								bCh[y] = false;
								if(y == 5 || y == 9)
								{
									nTimeTransiSongBorderUnlock = TIME_TRANSI_SONGBORDER_UNLOCK;
								}
							}
						}
					}
				}
			}
			if(bEnArrowsUnlock[0])
			{
				if(nChosesUnlock == 1 && !bCh[0])
				{
					MoveModsUnlock(true, 0);
					bCh[0] = true;
				}
				else
				{
					__int64 nCaret = nCountUnlock - nTimeEnArrowsUnlock[0];
					int nLimit = min(nCaret,nCountUnlock-nTimeChaUnlock)>TIME_FAST_CHANGE_UNLOCK?TIME_CHANGE_HIGH_UNLOCK:TIME_CHANGE_LOW_UNLOCK;
					nCaret = nCaret % nLimit;
					if(nCaret < (nLimit/2) && !bCh[0])
					{
						bCh[0] = true;
						MoveSongsUnlock(true);
						PlaySoundTransiAboutLimitUnlock(nLimit);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
					if(nCaret >= (nLimit/2))
					{
						bCh[0] = false;
					}
				}
			}
			if(bEnArrowsUnlock[4])
			{
				if(nChosesUnlock == 1 && !bCh[4])
				{
					MoveModsUnlock(false, 0);
					bCh[4] = true;
				}
				else
				{
					__int64 nCaret = nCountUnlock - nTimeEnArrowsUnlock[4];
					int nLimit = min(nCaret,nCountUnlock-nTimeChaUnlock)>TIME_FAST_CHANGE_UNLOCK?TIME_CHANGE_HIGH_UNLOCK:TIME_CHANGE_LOW_UNLOCK;
					nCaret = nCaret % nLimit;
					if(nCaret < (nLimit/2) && !bCh[4])
					{
						bCh[4] = true;
						MoveSongsUnlock(false);
						PlaySoundTransiAboutLimitUnlock(nLimit);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
					if(nCaret >= (nLimit/2))
					{
						bCh[4] = false;
					}
				}
			}
			if(bEnArrowsUnlock[1] && !bCh[1])
			{
				if(nChosesUnlock == 0)
				{
					SetChannelUnlock(true);
					bShow = false;
					bCh[1] = true;
				}
				if(nChosesUnlock == 1)
				{
					MoveGroupModsUnlock(true, 0);
					bCh[1] = true;
				}
			}
			if(bEnArrowsUnlock[3] && !bCh[3])
			{
				if(nChosesUnlock == 0)
				{
					SetChannelUnlock(false);
					bCh[3] = true;
					bShow = false;
				}
				if(nChosesUnlock == 1)
				{
					MoveGroupModsUnlock(false, 0);
					bCh[3] = true;
				}
			}
			if(bEnArrowsUnlock[2] && !bCh[2])
			{
				bCh[2] = true;
				if(nChosesUnlock == 1)
				{
					if(SetChooseModsUnlock(0)) nChosesUnlock = 0;
				}
				else if(nChosesUnlock == 0)
				{
					nChosesUnlock = 1;
					nModeCurSongUnlock[0] = 0;
					nModeCurSongUnlock[1] = 0;
					memset(nSelectedModSongUnlock[0], 0, 16*sizeof(BYTE));
					nMileageUnlock[0] = 0;
					memset(nSelectedModSongUnlock[1], 0, 16*sizeof(BYTE));
					nMileageUnlock[1] = 0;
				}
				nTimeChosesUnlock[nChosesUnlock-1] = nCountUnlock;
				if(nChosesUnlock > 0 && nChosesUnlock < CHOOSES_UNLOCK) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
				if(nChosesUnlock >= CHOOSES_UNLOCK)
				{
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int x = 0; x < 10; x++) bCh[x] = false;
				}
			}
#define TIME_TOL_SWITCHRETRO_UNLOCK 50
			if((_abs64(nTimeEnArrowsUnlock[0] - nTimeEnArrowsUnlock[4]) < TIME_TOL_SWITCHRETRO_UNLOCK && (bEnArrowsUnlock[0] || bEnArrowsUnlock[4])) ||
				(_abs64(nTimeEnArrowsUnlock[5] - nTimeEnArrowsUnlock[9]) < TIME_TOL_SWITCHRETRO_UNLOCK && (bEnArrowsUnlock[5] || bEnArrowsUnlock[9])))
			{
				nChosesUnlock =	CHOOSES_UNLOCK;
				nTimeChosesUnlock[nChosesUnlock-1] = nCountUnlock;
				if(nChosesUnlock > 0 && nChosesUnlock < CHOOSES_UNLOCK) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
				if(nChosesUnlock >= CHOOSES_UNLOCK)
				{
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int x = 0; x < 10; x++) bCh[x] = false;
				}
				for(int y = 0; y < 10; y++)
				{
					bArrows[y] = false;
					bEnArrowsUnlock[y] = false;
					bCh[y] = false;
				}
			}
#define TIME_TOL_SWITCHDONE_UNLOCK 50
			if((_abs64(nTimeEnArrowsUnlock[1] - nTimeEnArrowsUnlock[3]) < TIME_TOL_SWITCHDONE_UNLOCK && (bEnArrowsUnlock[1] || bEnArrowsUnlock[3])) ||
				(_abs64(nTimeEnArrowsUnlock[6] - nTimeEnArrowsUnlock[8]) < TIME_TOL_SWITCHDONE_UNLOCK && (bEnArrowsUnlock[6] || bEnArrowsUnlock[8])))
			{
				nChosesUnlock = 2;
				nTimeChosesUnlock[nChosesUnlock-1] = nCountUnlock;
				FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
				for(int x = 0; x < 10; x++) bCh[x] = false;
			}

			// 2
			if(bEnArrowsUnlock[5])
			{
				if(nChosesUnlock == 1 && !bCh[5])
				{
					MoveModsUnlock(true, 1);
					bCh[5] = true;
				}
				else
				{
					__int64 nCaret = nCountUnlock - nTimeEnArrowsUnlock[5];
					int nLimit = min(nCaret,nCountUnlock-nTimeChaUnlock)>TIME_FAST_CHANGE_UNLOCK?TIME_CHANGE_HIGH_UNLOCK:TIME_CHANGE_LOW_UNLOCK;
					nCaret = nCaret % nLimit;
					if(nCaret < (nLimit/2) && !bCh[5])
					{
						bCh[5] = true;
						MoveSongsUnlock(true);
						PlaySoundTransiAboutLimitUnlock(nLimit);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
					if(nCaret >= (nLimit/2))
					{
						bCh[5] = false;
					}
				}
			}
			if(bEnArrowsUnlock[9])
			{
				if(nChosesUnlock == 1 && !bCh[9])
				{
					MoveModsUnlock(false, 1);
					bCh[9] = true;
				}
				else
				{
					__int64 nCaret = nCountUnlock - nTimeEnArrowsUnlock[9];
					int nLimit = min(nCaret,nCountUnlock-nTimeChaUnlock)>TIME_FAST_CHANGE_UNLOCK?TIME_CHANGE_HIGH_UNLOCK:TIME_CHANGE_LOW_UNLOCK;
					nCaret = nCaret % nLimit;
					if(nCaret < (nLimit/2) && !bCh[9])
					{
						bCh[9] = true;
						MoveSongsUnlock(false);
						PlaySoundTransiAboutLimitUnlock(nLimit);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
					if(nCaret >= (nLimit/2))
					{
						bCh[9] = false;
					}
				}
			}
			if(bEnArrowsUnlock[6] && !bCh[6])
			{
				if(nChosesUnlock == 0)
				{
					SetChannelUnlock(true);
					bShow = false;
					bCh[6] = true;
				}
				if(nChosesUnlock == 1)
				{
					MoveGroupModsUnlock(true, 1);
					bCh[6] = true;
				}
			}
			if(bEnArrowsUnlock[8] && !bCh[8])
			{
				if(nChosesUnlock == 0)
				{
					SetChannelUnlock(false);
					bShow = false;
					bCh[8] = true;
				}
				if(nChosesUnlock == 1)
				{
					MoveGroupModsUnlock(false, 1);
					bCh[8] = true;
				}
			}
			if(bEnArrowsUnlock[7] && !bCh[7])
			{
				bCh[7] = true;
				if(nChosesUnlock == 1)
				{
					if(SetChooseModsUnlock(1)) nChosesUnlock = 0;
				}
				else if(nChosesUnlock == 0)
				{
					nChosesUnlock = 1;
					nModeCurSongUnlock[0] = 0;
					nModeCurSongUnlock[1] = 0;
					memset(nSelectedModSongUnlock[0], 0, 16*sizeof(BYTE));
					nMileageUnlock[0] = 0;
					memset(nSelectedModSongUnlock[1], 0, 16*sizeof(BYTE));
					nMileageUnlock[1] = 0;
				}
				nTimeChosesUnlock[nChosesUnlock-1] = nCountUnlock;
				if(nChosesUnlock > 0 && nChosesUnlock < CHOOSES_UNLOCK) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
				if(nChosesUnlock >= CHOOSES_UNLOCK)
				{
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int x = 0; x < 10; x++) bCh[x] = false;
				}
			}
			if(nCountUnlock >= nMaxUnlock)
			{
				nChosesUnlock = 2;
				nTimeChosesUnlock[nChosesUnlock-1] = nCountUnlock;
				FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
				for(int x = 0; x < 10; x++) bCh[x] = false;
			}
		}
	}
#define TIME_EXIT_UNLOCK 1000
	if(nChosesUnlock >= CHOOSES_UNLOCK)
	{
		__int64 nTransEnd = nCountUnlock - nTimeChosesUnlock[CHOOSES_UNLOCK-1];
		if(nTransEnd < TIME_EXIT_UNLOCK)
		{
			if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_UNLOCK))
			{
				glBindTexture(GL_TEXTURE_2D, m_uNullTex);
				glColor4d(0.0, 0.0, 0.0, 1.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
				glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
				glEnd();
			}
			for(int i = 0; i < 9; i++)
			{
				int x = nOrder[i];
				double dTargetx = double(x-4)*MAX_TARGET_X/4;
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_UNLOCK))
				{
					if(x == 4)
					{
						if(nTransEnd < (TIME_TRANSI_SELECTOR_UNLOCK))
						{
							dApt = //pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_UNLOCK, 2);
							pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_UNLOCK - 0.25, 2)/0.125 + 0.5;
							glPushMatrix();
							glTranslated(0.0, 0.0, -60.0);
							glScaled(dApt*1.25, dApt*1.25, 1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_UNLOCK);
							if(/*x == 4 && */nTransBorder > TIME_SHOW_PREVIEW_UNLOCK && movie_songpreviewunlock->bLoad && movie_songpreviewunlock->bResize && bShow)
							{
								movie_songpreviewunlock->Update(dwWait);
								movie_songpreviewunlock->GrabAVIFrame();
								__int64 nCaretCover = movie_songpreviewunlock->dwNext;
								glBindTexture(GL_TEXTURE_2D, movie_songpreviewunlock->tex);

								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
								glEnd();
								if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_UNLOCK)
								{
									dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_UNLOCK;
									glColor4d(1.0, 1.0, 1.0, min(1.0-dApt, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_UNLOCK));
									glBindTexture(GL_TEXTURE_2D, nTexCurSongUnlock[x]);
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
									glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
									glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
									glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
									glEnd();
								}
							}
							else
							{
								glBindTexture(GL_TEXTURE_2D, nTexCurSongUnlock[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
							}
							glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[7+nChaSongsUnlock[nIndexShowUnlock[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
					}
					else
					{
						__int64 nStartTransi = TIME_TRANSI_SELECTOR_UNLOCK/4*(-abs(x-4)+4-1);
						__int64 nLimitTransi = TIME_TRANSI_SELECTOR_UNLOCK/4*(-abs(x-4)+4);
						if(nTransEnd >= nStartTransi && nTransEnd < nLimitTransi)
						{
							dApt = double(nTransEnd-nStartTransi)/double(nLimitTransi-nStartTransi);
							double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(dApt)+dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXUnlock(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForXUnlock(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, min(dalpha, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_UNLOCK));
							glBindTexture(GL_TEXTURE_2D, nTexCurSongUnlock[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[7+nChaSongsUnlock[nIndexShowUnlock[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
						if(nTransEnd < nStartTransi)
						{
							double dx = dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXUnlock(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForXUnlock(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongUnlock[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[7+nChaSongsUnlock[nIndexShowUnlock[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
					}
				}
			}
			if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_UPDO_UNLOCK))
			{
				dApt = (double(nTransEnd))/(TIME_TRANSI_UPDO_UNLOCK);
				glPushMatrix();
				glScaled(1.0+0.8*(dApt), 1.0+0.8*(dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[16+nChaSongsUnlock[nCurSongUnlock]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[23+nChaSongsUnlock[nCurSongUnlock]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[0]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[1]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[2]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, unlockbasres->skin[3]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625, -29.75, -50.0);
				glEnd();
			}
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_uNullTex);
			glColor4d(0.0, 0.0, 0.0, 1.0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
			FMOD_Channel_Stop(channels[9]);
			if(movie_songpreviewunlock->bLoad)
			{
				movie_songpreviewunlock->CloseAVI();
				delete movie_songpreviewunlock;
			}
			glDeleteTextures(9, nTexCurSongUnlock);
			for(int x = 0; x < 9; x++) nTexCurSongUnlock[x] = 0;
			bShow = false;
			return 2;
		}
	}

	return 0;
}

int DrawSplash()
{
	if(!nSizeUnlock) return 1;
	nCountUnlock += dwTick - dwLastTick;
#define TIME_EXITSPLASH_UNLOCK 10000
#define TIME_START_MAIN_UNLOCK 0
#define TIME_TRANSI_MAIN_UNLOCK 300
	double dApt;
	if(nCountUnlock < TIME_EXITSPLASH_UNLOCK)
	{
		if(nCountUnlock >= TIME_START_MAIN_UNLOCK && nCountUnlock <= (TIME_START_MAIN_UNLOCK+TIME_TRANSI_MAIN_UNLOCK))
		{
			dApt = double(nCountUnlock-TIME_START_MAIN_UNLOCK)/TIME_TRANSI_MAIN_UNLOCK;
			glPushMatrix();
			glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
			glColor4d(1.0, 1.0, 1.0, dApt);
			glBindTexture(GL_TEXTURE_2D, unlockres->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-30.0,  15.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 30.0,  15.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 30.0, -15.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-30.0, -15.0, -50.0);
			glEnd();
			glPopMatrix();
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		if(nCountUnlock > (TIME_START_MAIN_UNLOCK+TIME_TRANSI_MAIN_UNLOCK))
		{
			glBindTexture(GL_TEXTURE_2D, unlockres->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-30.0,  15.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 30.0,  15.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 30.0, -15.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-30.0, -15.0, -50.0);
			glEnd();
		}
	#define TIME_START_SONG_UNLOCK 2000
	#define TIME_TRANSI_SONG_UNLOCK 300
		__int64 nTimeSong = nCountUnlock-nTimeSongUnlock;
		if(nTimeSong >= 0
			&& nTimeSong <= (TIME_TRANSI_SONG_UNLOCK))
		{
			dApt = double(nTimeSong)/TIME_TRANSI_SONG_UNLOCK;
			glPushMatrix();
			glTranslated(-16.275, 0.0, 0.0);
			glColor4d(1.0, 1.0, 1.0, dApt);
			glBindTexture(GL_TEXTURE_2D, uTexUnlock[nSongUnlock]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-10.5,  7.875, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 10.5,  7.875, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 10.5, -7.875, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-10.5, -7.875, -50.0);
			glEnd();
			glPopMatrix();
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		if(nTimeSong > (TIME_TRANSI_SONG_UNLOCK))
		{
			glPushMatrix();
			glTranslated(-16.275, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, uTexUnlock[nSongUnlock]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-10.5,  7.875, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 10.5,  7.875, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 10.5, -7.875, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-10.5, -7.875, -50.0);
			glEnd();
			glPopMatrix();
		}
	#define TIME_START_SONGPARENT_UNLOCK 100
	#define TIME_TRANSI_SONGPARENT_UNLOCK 500
	#define TIME_GLOUTH_SONGPARENT_UNLOCK 500
	#define TIME_DUR_SONGPARENT_UNLOCK 1000
		if(nCountUnlock >= TIME_START_SONGPARENT_UNLOCK
			&& nCountUnlock <= (TIME_START_SONGPARENT_UNLOCK+TIME_TRANSI_SONGPARENT_UNLOCK))
		{
			dApt = double(nCountUnlock-TIME_START_SONGPARENT_UNLOCK)/TIME_TRANSI_SONGPARENT_UNLOCK;
			glPushMatrix();
			glTranslated(-16.275, 0.0, 0.0);
			glRotated(180*(1-dApt), 0.0, 0.0, 1.0);
			glScaled(1.0+(1-dApt), 1.0+(1-dApt), 1.0);
			glColor4d(1.0, 1.0, 1.0, dApt);
			glBindTexture(GL_TEXTURE_2D, unlockres->skin[21]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-6.75,  6.75, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 6.75,  6.75, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 6.75, -6.75, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-6.75, -6.75, -50.0);
			glEnd();
			glPopMatrix();
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		if(nCountUnlock > (TIME_START_SONGPARENT_UNLOCK+TIME_TRANSI_SONGPARENT_UNLOCK) &&
			nCountUnlock <= (TIME_START_SONGPARENT_UNLOCK+TIME_TRANSI_SONGPARENT_UNLOCK+TIME_DUR_SONGPARENT_UNLOCK))
		{
			glPushMatrix();
			glTranslated(-16.275, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, unlockres->skin[21]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-6.75,  6.75, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 6.75,  6.75, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 6.75, -6.75, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-6.75, -6.75, -50.0);
			glEnd();
			if(nCountUnlock <= (TIME_START_SONGPARENT_UNLOCK+TIME_TRANSI_SONGPARENT_UNLOCK+TIME_GLOUTH_SONGPARENT_UNLOCK))
			{
				dApt = double(nCountUnlock-TIME_START_SONGPARENT_UNLOCK-TIME_GLOUTH_SONGPARENT_UNLOCK)/TIME_TRANSI_SONGPARENT_UNLOCK;
				glPushMatrix();
				glScaled(1.0+(dApt), 1.0+(dApt), 1.0);
				glColor4d(1.0, 1.0, 1.0, 1-dApt);
				glBindTexture(GL_TEXTURE_2D, unlockres->skin[21]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-6.75,  6.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 6.75,  6.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 6.75, -6.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-6.75, -6.75, -50.0);
				glEnd();
				glPopMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0);
			}
			glPopMatrix();
		}
		if(nCountUnlock > (TIME_START_SONGPARENT_UNLOCK+TIME_TRANSI_SONGPARENT_UNLOCK+TIME_DUR_SONGPARENT_UNLOCK)
			&& nCountUnlock <= (TIME_START_SONGPARENT_UNLOCK+TIME_TRANSI_SONGPARENT_UNLOCK+TIME_DUR_SONGPARENT_UNLOCK+TIME_TRANSI_SONGPARENT_UNLOCK))
		{
			dApt = double(nCountUnlock-TIME_START_SONGPARENT_UNLOCK-TIME_TRANSI_SONGPARENT_UNLOCK-TIME_DUR_SONGPARENT_UNLOCK)/TIME_TRANSI_SONGPARENT_UNLOCK;
			glPushMatrix();
			glTranslated(-16.275, 0.0, 0.0);
			glScaled(1.0+(dApt), 1.0+(dApt), 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApt);
			glBindTexture(GL_TEXTURE_2D, unlockres->skin[21]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-6.75,  6.75, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 6.75,  6.75, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 6.75, -6.75, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-6.75, -6.75, -50.0);
			glEnd();
			glPopMatrix();
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
	#define TIME_START_PAR_UNLOCK 300
	#define TIME_TRANSI_PAR_UNLOCK 300
	#define TIME_GLOUTH_PAR_UNLOCK 300
		for(int x = 0; x < 16; x++) if(nParUnlock[nSongUnlock*16+x])
		{
			if(nTimeSong >= TIME_START_PAR_UNLOCK
				&& nTimeSong <= (TIME_START_PAR_UNLOCK+TIME_TRANSI_PAR_UNLOCK))
			{
				dApt = double(nTimeSong-TIME_START_PAR_UNLOCK)/TIME_TRANSI_PAR_UNLOCK;
				glPushMatrix();
				switch(x)
				{
				case 15:
					glTranslated(22.425, 0.0, 0.0);
					break;
				default:
					glTranslated(-1.2+7.873*double(x/5), 7.875-3.9375*double(x%5), 0.0);
					break;
				}
				glColor4d(1.0, 1.0, 1.0, dApt);
				glBindTexture(GL_TEXTURE_2D, unlockres->skin[1+x]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-1.5,  1.5, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 1.5,  1.5, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 1.5, -1.5, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-1.5, -1.5, -50.0);
				glEnd();
				glPopMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0);
			}
			if(nTimeSong > (TIME_START_PAR_UNLOCK+TIME_TRANSI_PAR_UNLOCK))
			{
				glPushMatrix();
				switch(x)
				{
				case 15:
					glTranslated(22.425, 0.0, 0.0);
					break;
				default:
					glTranslated(-1.2+7.873*double(x/5), 7.875-3.9375*double(x%5), 0.0);
					break;
				}
				glBindTexture(GL_TEXTURE_2D, unlockres->skin[1+x]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-1.5,  1.5, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 1.5,  1.5, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 1.5, -1.5, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-1.5, -1.5, -50.0);
				glEnd();
				if(nTimeSong <= (TIME_START_PAR_UNLOCK+TIME_TRANSI_PAR_UNLOCK+TIME_GLOUTH_PAR_UNLOCK))
				{
					dApt = double(nTimeSong-TIME_START_PAR_UNLOCK-TIME_TRANSI_PAR_UNLOCK)/TIME_GLOUTH_PAR_UNLOCK;
					glPushMatrix();
					glScaled(1.0+0.5*(dApt), 1.0+0.5*(dApt), 1.0);
					glColor4d(1.0, 1.0, 1.0, 1-dApt);
					glBindTexture(GL_TEXTURE_2D, unlockres->skin[18]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-1.5,  1.5, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 1.5,  1.5, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 1.5, -1.5, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-1.5, -1.5, -50.0);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
				glPopMatrix();
			}
		}
		static bool bCh[] = {false, false, false, false, false};
		if(nCountUnlock >= TIME_START_SONG_UNLOCK) for(int x = 0; x < 2; x++) if(np[x])
		{
			if(x == 0)
			{
				for(int y = 0; y < 5; y++)
				{
					if(bArrows[y] && !bEnArrowsUnlock[y])
					{
						bEnArrowsUnlock[y] = true;
					}
					if(!bArrows[y] && bEnArrowsUnlock[y])
					{
						bEnArrowsUnlock[y] = false;
						bCh[y] = false;
					}
				}
			}
			if(x == 1)
			{
				for(int y = 5; y < 10; y++)
				{
					if(bArrows[y] && !bEnArrowsUnlock[y-5])
					{
						bEnArrowsUnlock[y-5] = true;
					}
					if(!bArrows[y] && bEnArrowsUnlock[y-5])
					{
						bEnArrowsUnlock[y-5] = false;
						bCh[y-5] = false;
					}
				}
			}
			if(bEnArrowsUnlock[0] && !bCh[0])
			{
				SwitchSongSplash(true);
				bCh[0] = true;
			}
			if(bEnArrowsUnlock[4] && !bCh[4])
			{
				SwitchSongSplash(false);
				bCh[4] = true;
			}
			if(bEnArrowsUnlock[1] && !bCh[1])
			{
				SwitchSongSplash(true);
				bCh[1] = true;
			}
			if(bEnArrowsUnlock[3] && !bCh[3])
			{
				SwitchSongSplash(false);
				bCh[3] = true;
			}
			if(bEnArrowsUnlock[2] && !bCh[2])
			{
				nCountUnlock = TIME_EXITSPLASH_UNLOCK;
				bCh[2] = true;
			}
		}
	}
	else
	{
		__int64 nTimeExit = nCountUnlock - TIME_EXITSPLASH_UNLOCK;
		if( nTimeExit <= TIME_TRANSI_MAIN_UNLOCK)
		{
			dApt = double(nTimeExit-TIME_START_MAIN_UNLOCK)/TIME_TRANSI_MAIN_UNLOCK;
			glPushMatrix();
			glScaled(1.0-0.2*(dApt), 1.0-0.2*(dApt), 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApt);
			glBindTexture(GL_TEXTURE_2D, unlockres->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-30.0,  15.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 30.0,  15.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 30.0, -15.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-30.0, -15.0, -50.0);
			glEnd();
			glLoadIdentity();
			glScaled(1.0+0.2*(dApt), 1.0+0.2*(dApt), 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApt);
			glBindTexture(GL_TEXTURE_2D, unlockres->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-30.0,  15.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 30.0,  15.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 30.0, -15.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-30.0, -15.0, -50.0);
			glEnd();
			glPopMatrix();
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		else return 1;
	}
	return 0;
}

int CreateContextUnlock()
{
	unlock = new CUnlockLoader();
	if(!unlock->LoadBinary(_T("Unlock/Unlock.dat")))
	{
		if(!unlock->LoadText(_T("Unlock/Unlock.acc"))) return 0;
		if(!unlock->SaveBinary(_T("Unlock/Unlock.dat"))) return 0;
	}
	unlockres = new CUnlockRes();
	if(!unlockres->LoadSkin()) return 0;
	uTexUnlock = (GLuint*)calloc(0, sizeof(int));
	nParUnlock = (BYTE*)calloc(0, sizeof(BYTE));
	if(!EnumTotalSongsUnlock()) return 0;
	return 1;
}

void DestroyContextUnlock()
{
	delete unlock;
	delete unlockres;
	delete uTexUnlock;
	delete nParUnlock;
}

void ActualizateContextUnlock()
{
	nCountUnlock = 0;
	if(nModeUnlock)
	{
		movie_unlockbg = new CMovieStream2();
		movie_songpreviewunlock = new CMovieStream2();
		movie_unlockbg->OpenAVI(_T("Unlock/UnlockBG.avi"));
		movie_unlockbg->bLoop = true;

		unlockbasres = new CUnlockBasRes();
		unlockbasres->LoadSkin();

		nMaxUnlock = 60000+TIME_TRANSI_UPDO_UNLOCK+TIME_START_UPDO_UNLOCK;
		nInterUnlock = 0;
		int nPlayerFocus = 0;
		nModeMovUnlock = 0;
		nCurSongUnlock = 0;
		nChosesUnlock = 0;
		nCountUnlock = 0;
		nModeUnlockedUnlock = 0;
		nTimeUnlockedUnlock = 0;
		nTimeCurSongUnlock = TIME_START_SELECTOR_UNLOCK+TIME_TRANSI_SELECTOR_UNLOCK+TIME_GLOUTH_SELECTOR_UNLOCK;
		bLoadMusicCoverUnlock = false;
		bContinueUsbUnlock = true;
		for(int x = 0; x < 2; x++)
		{
			nModeCurSongUnlock[x] = 0;
			nBackModeCurSongUnlock[x] = 0;
			nTimeModeCurSongUnlock[x] = TIME_START_SELECTOR_UNLOCK+TIME_TRANSI_SELECTOR_UNLOCK+TIME_GLOUTH_SELECTOR_UNLOCK;
			nPrefModeCurSongUnlock[x] = 0;
			memset(nSelectedModSongUnlock[x], 0, 16*sizeof(BYTE));
			nMileageUnlock[x] = 0;
			if(np[x]) ShowSesionTrayMileage(x);
		}
		for(int x = 0; x < 10; x++) bEnArrowsUnlock[x] = false;
		for(int x = 0; x < 2; x++) if(np[x])
		{
			EnumSongsUnlock();
			nPlayerFocus = x;
			break;
		}
		bool bFoundLast = false;
		nCurSongUnlock = 0;
		nTimeChaUnlock = TIME_START_UPDO_UNLOCK+TIME_TRANSI_UPDO_UNLOCK;

		ActualizateIndexShowUnlock();
		for(int x = 0; x < 9; x++)
		{
			TCHAR chFile[260];
			_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsUnlock[nIndexShowUnlock[x]]);
			nTexCurSongUnlock[x] = GenTexImage(chFile);
		}
	}
	else
	{
		if (!unlockres) unlockres = new CUnlockRes();
		unlockres->LoadSkin();
		for(int x = 0; x < 5; x++) bEnArrowsUnlock[x] = false;
		nTimeSongUnlock = TIME_START_SONG_UNLOCK;
		nSongUnlock = 0;
	}
	SetMileageUnlock();
	dwTick = dwLastTick = SDL_GetTicks();
}

void ClearContextUnlock()
{
	for(int x = 0; x < 2; x++) if(np[x]) HideSesionTrayMileage(x);
	if(nModeUnlock)
	{
		if(movie_unlockbg->bLoad) movie_unlockbg->CloseAVI();
		delete movie_unlockbg;
		delete unlockbasres;
	}
	else
	{
		if(unlockres) delete unlockres;
		unlockres = NULL;
		if(nSizeUnlock)
		{
			nSizeUnlock = 0;
			delete[] strIDUnlock;
			uTexUnlock = (GLuint*)realloc(uTexUnlock, nSizeUnlock*sizeof(int));
			nParUnlock = (BYTE*)realloc(nParUnlock, nSizeUnlock*sizeof(BYTE)*16);
		}
	}
}

int FrameUnlock()
{
	unlockres->Update(dwTick - dwLastTick);
	switch(nModeUnlock)
	{
	case 0:
		if(DrawSplash())
		{
			ClearContextUnlock();
			return 0;
		}
		break;
	case 1:
		if(DrawUnlocker())
		{
			ClearContextUnlock();
			return 0;
		}
		break;
	}
	return INT_UNLOCK;
}

int EventUnlock(int nEvent, int nKey)
{
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
	return 1;
}
