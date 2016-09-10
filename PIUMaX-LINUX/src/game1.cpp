#include "game1.h"
#include "stepeval.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "Game1Res.h"
#include "USBsys.h"
#include "UnlockSys.h"

#define TIME_START_SELECTOR_GAME1 0
#define TIME_TRANSI_SELECTOR_GAME1 1000
#define TIME_GLOUTH_SELECTOR_GAME1 500

#define TIME_START_START_GAME1 1500

#define TIME_TRANSI_SONGBORDER_GAME1 500
#define TIME_GLOUTH_SONGBORDER_GAME1 500
#define TIME_TRANSI_START_MAIN_GAME1 500
#define CHOOSES_GAME1 2
#define MAX_COMMANDS_GAME1 12

#define TIME_SHOW_PREVIEW_GAME1 600

bool bShow = false;

bool bCh[] = {false, false, false, false, false,
									false, false, false, false, false};

int nComandGame1[2][MAX_COMMANDS_GAME1];
int nInterGame1 = 0;
int nSizeSongsGame1 = 0;
int nModTotalSongsGame1[10000];
int nModSongsGame1[10000];
int nChaTotalSongsGame1[1000];
int nChaSongsGame1[1000];
int nNOLTotalSongsGame1[1000];
int nRefSongsGame1[1000];
int nStageTotalSongsGame1[1000];
int nStageSongsGame1[1000];
int nExModSongsGame1[1000];
GLuint nTexCurSongGame1[9];
TCHAR chTotalSongsGame1[1000][100];
TCHAR chSongsGame1[1000][100];

int nIndexShowGame1[9];
int nLevelCurGame1[2];
double dDCurGame1[2][5];
int nCurSongGame1 = 0;
int nModeCurSongGame1[2];
int nBackModeCurSongGame1[2];
int nPrefModeCurSongGame1[2];
__int64 nTimeModeCurSongGame1[2];
__int64 nTimeCurSongGame1 = 0;
__int64 nCountGame1 = 0;
__int64 nMaxGame1 = 0;
__int64 nTimeChaGame1 = 0;
__int64 nCaretCountGame1 = 0;

__int64 nTimeLastPresetGame1[2];
int nIndLastPresetGame1[2];
int nIndPresetGame1[2][30];
int nIndSizePresetGame1[2][6];
int nSkinGame1[2];
int nCharacterGame1[2];

int nChosesGame1;
__int64 nTimeChosesGame1[2];
int nChangeRetroGame1;
__int64 nTimeChangeRetroGame1;
int nRetroActiveGame1;

bool bEnArrowsGame1[10];
__int64 nTimeEnArrowsGame1[10];
int nCommand[2][10];

int nTimeTransiSongBorderGame1 = TIME_TRANSI_SONGBORDER_GAME1;
int nModeGame1 = 0;
bool bBonusStageGame1[2];
//bool bSaveUsbGame1 = false;
bool bContinueUsb = true;

GLuint movie_texGame1;
CMovieStream2* movie_arcadebg = NULL;
CMovieStream2* movie_songpreview = NULL;

bool bLoadMusicCoverGame1 = false;
FMOD_SOUND* music_coverGame1;

CString strDispGame1;

CGame1Res* ga1res;

CString strBestScoreMachineGame1;
int nBestScoreMachineGame1;
int nBestScorePlayerGame1[2];

int FoundPresetGame1(int x, int nFamily, int nBegin, int nEnd);
void SemiActualizateContextGame1();
void SemiClearContextGame1();

int nSizeSkinGame1 = 0;
int* nRefSkinGame1;
GLuint* uTexSkinGame1;
int nSizeCharacterGame1 = 0;
int* nRefCharacterGame1;
GLuint* uTexCharacterGame1;

#define MAX_SKINS_GAME1 20
#define MAX_CHARACTERS_GAME1 12

__int64 nTimeSkinGame1[2];
bool bSkinGame1[2];
__int64 nTimeTurnSkinGame1[2];
int nDirTurnSkinGame1[2];
int nCurSkinGame1[2];
__int64 nTimeCharacterGame1[2];
bool bCharacterGame1[2];
__int64 nTimeTurnCharacterGame1[2];
int nDirTurnCharacterGame1[2];
int nCurCharacterGame1[2];

bool bIsAvalibleUnlockerGame1;

int FoundPresetGame1(int x, int nFamily, int nBegin, int nEnd);

void MakeChoiceSkinGame1(int x)
{
	bSkinGame1[x] = false;
	int nPut = nRefSkinGame1[nCurSkinGame1[x]-1]+25;
	int nFamily = 2;
	int nFound = FoundPresetGame1(x, nFamily, 24, 45);
	if(nFound == -1 && nCurSkinGame1[x])
	{
		nFound = nIndSizePresetGame1[x][nFamily];
		nIndSizePresetGame1[x][nFamily]++;
		nIndPresetGame1[x][nFamily*5+nFound] = nPut;
		nIndLastPresetGame1[x] = nFamily*5+nFound;
		nTimeLastPresetGame1[x] = nCountGame1;
	}
	if(nFound != 1)
	{
		if(!nCurSkinGame1[x])	// Desactivar
		{
			nIndPresetGame1[x][nFamily*5+nFound] = 0;
			for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
				nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
			nIndSizePresetGame1[x][nFamily]--;
		}
		else	// Actualizar
		{
			nIndPresetGame1[x][nFamily*5+nFound] = nPut;
			nIndLastPresetGame1[x] = nFamily*5+nFound;
			nTimeLastPresetGame1[x] = nCountGame1;
		}
	}
}

void MakeChoiceCharacterGame1(int x)
{
	bCharacterGame1[x] = false;
	int nPut = nRefCharacterGame1[nCurCharacterGame1[x]-1]+47;
	int nFamily = 5;
	int nFound = FoundPresetGame1(x, nFamily, 46, 59);
	if(nFound == -1 && nCurCharacterGame1[x])
	{
		nFound = nIndSizePresetGame1[x][nFamily];
		nIndSizePresetGame1[x][nFamily]++;
		nIndPresetGame1[x][nFamily*5+nFound] = nPut;
		nIndLastPresetGame1[x] = nFamily*5+nFound;
		nTimeLastPresetGame1[x] = nCountGame1;
	}
	if(nFound != 1)
	{
		if(!nCurCharacterGame1[x])	// Desactivar
		{
			nIndPresetGame1[x][nFamily*5+nFound] = 0;
			for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
				nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
			nIndSizePresetGame1[x][nFamily]--;
		}
		else	// Actualizar
		{
			nIndPresetGame1[x][nFamily*5+nFound] = nPut;
			nIndLastPresetGame1[x] = nFamily*5+nFound;
			nTimeLastPresetGame1[x] = nCountGame1;
		}
	}
}


void TurnSkinsGame1(bool bIzq, int x)
{
	if(bIzq)
	{
		nCurSkinGame1[x]--;
		if(nCurSkinGame1[x] < 0) nCurSkinGame1[x] = nSizeSkinGame1;
	}
	else
	{
		nCurSkinGame1[x]++;
		if(nCurSkinGame1[x] >= (nSizeSkinGame1+1)) nCurSkinGame1[x] = 0;
	}
	nDirTurnSkinGame1[x] = bIzq?1:2;
	nTimeTurnSkinGame1[x] = nCountGame1;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
}

void TurnCharactersGame1(bool bIzq, int x)
{
	if(bIzq)
	{
		nCurCharacterGame1[x]--;
		if(nCurCharacterGame1[x] < 0) nCurCharacterGame1[x] = nSizeCharacterGame1;
	}
	else
	{
		nCurCharacterGame1[x]++;
		if(nCurCharacterGame1[x] >= (nSizeCharacterGame1+1)) nCurCharacterGame1[x] = 0;
	}
	nDirTurnCharacterGame1[x] = bIzq?1:2;
	nTimeTurnCharacterGame1[x] = nCountGame1;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
}

void DrawSkinsGame1(int x)
{
#define TIME_TRANSI_SKIN_GAME1 500
	__int64 nTimeSkin = nCountGame1 - nTimeSkinGame1[x];
	double dApt;
	int n[5];
	for(int y = 0; y < 5; y++)
	{
		n[y] = (y-2+nCurSkinGame1[x])%(nSizeSkinGame1+1);
		while(n[y] < 0) n[y] += (nSizeSkinGame1+1);
	}
	if(nTimeSkin >= 0 && nTimeSkin < TIME_TRANSI_SKIN_GAME1)
	{
		dApt = double(nTimeSkin)/TIME_TRANSI_SKIN_GAME1;
		glColor4d(1.0, 1.0, 1.0, dApt);
		glPushMatrix();
		glTranslated(0.0, -2.5*(1-dApt)+11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SkinSplash]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SkinBorder]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[2]==0?ga1res->skin[GAME1RES_SkinNo]->GetCurrentTex():uTexSkinGame1[n[2]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(10.0-5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SkinBorder]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[3]==0?ga1res->skin[GAME1RES_SkinNo]->GetCurrentTex():uTexSkinGame1[n[3]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(-10.0+5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SkinBorder]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[1]==0?ga1res->skin[GAME1RES_SkinNo]->GetCurrentTex():uTexSkinGame1[n[1]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glPopMatrix();
		glColor4d(1.0, 1.0, 1.0, 1.0);
	}
	int nOrdDraw[] = {4,3,0,1,2};
	if(nTimeSkin >= TIME_TRANSI_SKIN_GAME1)
	{
#define TIME_TURNSKIN_GAME1 300
		glPushMatrix();
		glTranslated(0.0, 11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SkinSplash]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		__int64 nTimeTurn = nCountGame1 - nTimeTurnSkinGame1[x];
		dApt = 0.0;
		if(nTimeTurn >= 0 && nTimeTurn <= TIME_TURNSKIN_GAME1 && nDirTurnSkinGame1[x])
		{
			dApt = (nDirTurnSkinGame1[x]==1?-1:1)*(1-double(nTimeTurn)/TIME_TURNSKIN_GAME1);
		}
		for(int z = 0; z < 5; z++)
		{
			int u = nOrdDraw[z];
			double dx = double(u)*10.0-20.0+dApt*10.0;
			double ds = (-abs(dx)+20.0)/10.0;
			double da = ds/2.0;
			glColor4d(1.0, 1.0, 1.0, da);
			glPushMatrix();
			glTranslated(dx, 0.0, 0.0);
			glScaled(ds, ds, 1.0);
			glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SkinBorder]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, n[u]==0?ga1res->skin[GAME1RES_SkinNo]->GetCurrentTex():uTexSkinGame1[n[u]-1]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glPopMatrix();
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
	}
}

void DrawCharactersGame1(int x)
{
#define TIME_TRANSI_CHARACTER_GAME1 500
	__int64 nTimeCharacter = nCountGame1 - nTimeCharacterGame1[x];
	double dApt;
	int n[5];
	for(int y = 0; y < 5; y++)
	{
		n[y] = (y-2+nCurCharacterGame1[x])%(nSizeCharacterGame1+1);
		while(n[y] < 0) n[y] += (nSizeCharacterGame1+1);
	}
	if(nTimeCharacter >= 0 && nTimeCharacter < TIME_TRANSI_CHARACTER_GAME1)
	{
		dApt = double(nTimeCharacter)/TIME_TRANSI_CHARACTER_GAME1;
		glColor4d(1.0, 1.0, 1.0, dApt);
		glPushMatrix();
		glTranslated(0.0, -2.5*(1-dApt)+11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_CharacterSplash]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_CharacterBorder]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[2]==0?ga1res->skin[GAME1RES_CharacterNo]->GetCurrentTex():uTexCharacterGame1[n[2]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(10.0-5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_CharacterBorder]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[3]==0?ga1res->skin[GAME1RES_CharacterNo]->GetCurrentTex():uTexCharacterGame1[n[3]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, -5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0,  5.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(-10.0+5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_CharacterBorder]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[1]==0?ga1res->skin[GAME1RES_CharacterNo]->GetCurrentTex():uTexCharacterGame1[n[1]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glPopMatrix();
		glColor4d(1.0, 1.0, 1.0, 1.0);
	}
	int nOrdDraw[] = {4,3,0,1,2};
	if(nTimeCharacter >= TIME_TRANSI_CHARACTER_GAME1)
	{
#define TIME_TURNCHARACTER_GAME1 300
		glPushMatrix();
		glTranslated(0.0, 11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_CharacterSplash]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		__int64 nTimeTurn = nCountGame1 - nTimeTurnCharacterGame1[x];
		dApt = 0.0;
		if(nTimeTurn >= 0 && nTimeTurn <= TIME_TURNCHARACTER_GAME1 && nDirTurnCharacterGame1[x])
		{
			dApt = (nDirTurnCharacterGame1[x]==1?-1:1)*(1-double(nTimeTurn)/TIME_TURNCHARACTER_GAME1);
		}
		for(int z = 0; z < 5; z++)
		{
			int u = nOrdDraw[z];
			double dx = double(u)*10.0-20.0+dApt*10.0;
			double ds = (-abs(dx)+20.0)/10.0;
			double da = ds/2;
			glColor4d(1.0, 1.0, 1.0, da);
			glPushMatrix();
			glTranslated(dx, 0.0, 0.0);
			glScaled(ds, ds, 1.0);
			glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_CharacterBorder]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, n[u]==0?ga1res->skin[GAME1RES_CharacterNo]->GetCurrentTex():uTexCharacterGame1[n[u]-1]);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glPopMatrix();
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
	}
}

void DeleteSkinsGame1();
void DeleteCharactersGame1();

void EnumSkinsGame1()
{
	DeleteSkinsGame1();
	for(int x = 0; x < MAX_SKINS_GAME1; x++)
	{
		if((np[0] && !player_sesion[0]->nLockedAsp[x]) ||
			(np[1] && !player_sesion[1]->nLockedAsp[x]))
		{
			nSizeSkinGame1++;
			nRefSkinGame1 = (int*)realloc(nRefSkinGame1, nSizeSkinGame1*sizeof(int));
			uTexSkinGame1 = (GLuint*)realloc(uTexSkinGame1, nSizeSkinGame1*sizeof(GLuint));
			nRefSkinGame1[nSizeSkinGame1-1] = x;
			CString str;
			str.Format(_T("skins/%d/Preview.png"), x+1);
			uTexSkinGame1[nSizeSkinGame1-1] = GenTexImage(str);
		}
	}
}

void EnumCharactersGame1()
{
	DeleteCharactersGame1();
	for(int x = 0; x < MAX_CHARACTERS_GAME1; x++)
	{
		if((np[0] && !player_sesion[0]->nLockedAsp[20+x]) ||
			(np[1] && !player_sesion[1]->nLockedAsp[20+x]))
		{
			nSizeCharacterGame1++;
			nRefCharacterGame1 = (int*)realloc(nRefCharacterGame1, nSizeCharacterGame1*sizeof(int));
			uTexCharacterGame1 = (GLuint*)realloc(uTexCharacterGame1, nSizeCharacterGame1*sizeof(GLuint));
			nRefCharacterGame1[nSizeCharacterGame1-1] = x;
			CString str;
			str.Format(_T("Characters/%d/Preview.png"), x+1);
			uTexCharacterGame1[nSizeCharacterGame1-1] = GenTexImage(str);
		}
	}
}

void DeleteSkinsGame1()
{
	if(nSizeSkinGame1)
	{
		glDeleteTextures(nSizeSkinGame1, uTexSkinGame1);
		nSizeSkinGame1 = 0;
		nRefSkinGame1 = (int*)realloc(nRefSkinGame1, nSizeSkinGame1*sizeof(int));
		uTexSkinGame1 = (GLuint*)realloc(uTexSkinGame1,nSizeSkinGame1*sizeof(GLuint));
	}
}

void DeleteCharactersGame1()
{
	if(nSizeCharacterGame1)
	{
		glDeleteTextures(nSizeCharacterGame1, uTexCharacterGame1);
		nSizeCharacterGame1 = 0;
		nRefCharacterGame1 = (int*)realloc(nRefCharacterGame1, nSizeCharacterGame1*sizeof(int));
		uTexCharacterGame1 = (GLuint*)realloc(uTexCharacterGame1, nSizeCharacterGame1*sizeof(GLuint));
	}
}

int FoundFirstRetro(int nSet)
{
	for(int x = 0; x < 1000; x++)
	{
		if(nNOLTotalSongsGame1[x] == nSet)
			return x;
	}
	return 0;
}

int LoadSongGame1()
{
	// ** Cargar datos basicos
	ARGSTEP arg;
	arg.strMusic.Format(_T("song/%ls/%ls.mp3"), chSongsGame1[nCurSongGame1], chSongsGame1[nCurSongGame1]);
	arg.strLights.Format(_T("song/%ls/%ls.lgt"), chSongsGame1[nCurSongGame1], chSongsGame1[nCurSongGame1]);
	CString str;
	TCHAR ch[260];
	for(int x = 0; x < 2; x++) if(np[x])
	{
		str.Format(_T("song/%ls/%ls.acc"), chSongsGame1[nCurSongGame1], chSongsGame1[nCurSongGame1]);
		FILE* hFile = _fopen(str, _T("r"));
		if(hFile != NULL)
		{
			if( _fgets(ch, 260, hFile) != NULL)
			{
				arg.strBGA.Format(_T("song/%ls/%ls"), chSongsGame1[nCurSongGame1], ch);
				if(arg.strBGA.Find(_T('[')) != -1)
					arg.strBGA.Format(_T("song/GNRBGA/%ls"), ch);
				arg.strBGA.Remove(_T('\n')); arg.strBGA.Remove(_T('\r'));
			}
			else arg.strBGA.Format(_T("song/%ls/%ls.avi"), chSongsGame1[nCurSongGame1], chSongsGame1[nCurSongGame1]);
			fclose(hFile);
		}
		else arg.strBGA.Format(_T("song/%ls/%ls.avi"), chSongsGame1[nCurSongGame1], chSongsGame1[nCurSongGame1]);
		if(player_sesion[x]->nLockedAsp[ITEM_BGAOFF] || FoundPresetGame1(x, 5, 59, 59) != -1)
		{
			arg.strBGA = _T("");
			break;
		}
		if(player_sesion[x]->nBgaSongNew[nRefSongsGame1[nCurSongGame1]] != 1)
		{
			if(player_sesion[x]->nBgaSongNew[nRefSongsGame1[nCurSongGame1]] == 0)
			{
				arg.strBGA = _T("");
				break;
			}
			hFile = _fopen(str, _T("r"));
			if(hFile != NULL)
			{
				bool bCount = false;
				for(int y = 0; y < (player_sesion[x]->nBgaSongNew[nRefSongsGame1[nCurSongGame1]]); y++)
				{
					if(_fgets(ch, 260, hFile) == NULL) break;
					bCount = true;
					str = ( ch);
					str.Remove(_T('\n')); str.Remove(_T('\r'));
				}
				fclose(hFile);
				if(bCount)
				{
					arg.strBGA.Format(_T("song/%ls/%ls"), chSongsGame1[nCurSongGame1], str);
					break;
				}
			}
		}
	}
	arg.strSplash.Format(_T("song/%ls/%ls.png"), chSongsGame1[nCurSongGame1], chSongsGame1[nCurSongGame1]);
	arg.bAutoPlay = false;
	if(!LoadSong(arg)) return 0;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		HideSesionTrayExp(x);
		arg.strFile.Format(_T("song/%ls/%ls_%ls.sksf"), chSongsGame1[nCurSongGame1], chSongsGame1[nCurSongGame1], chStdModes[nModeCurSongGame1[x]]);
		arg.dUserVel = 1.0;
		if(nModeCurSongGame1[x]<3)
			arg.bIsSingle = true;
		else if(nModeCurSongGame1[x]>=3 && nModeCurSongGame1[x]<5)
			arg.bIsSingle = false;
		else if(nModeCurSongGame1[x]>=5 && nModeCurSongGame1[x]<8)
			arg.bIsSingle = true;
		else if(nModeCurSongGame1[x]>=8 && nModeCurSongGame1[x]<10)
			arg.bIsSingle = false;
		else
			arg.bIsSingle = true;
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
		switch(piuconfig.nBarLevel)
		{
		case 0:
			arg.dIncrBar = BAR_INCR_1;
			arg.dDecrBar = BAR_DECR_1;
			break;
		case 1:
			arg.dIncrBar = BAR_INCR_2;
			arg.dDecrBar = BAR_DECR_2;
			break;
		case 2:
			arg.dIncrBar = BAR_INCR_3;
			arg.dDecrBar = BAR_DECR_3;
			break;
		case 3:
			arg.dIncrBar = BAR_INCR_4;
			arg.dDecrBar = BAR_DECR_4;
			break;
		case 4:
			arg.dIncrBar = BAR_INCR_5;
			arg.dDecrBar = BAR_DECR_5;
			break;
		case 5:
			arg.dIncrBar = BAR_INCR_6;
			arg.dDecrBar = BAR_DECR_6;
			break;
		case 6:
			arg.dIncrBar = BAR_INCR_7;
			arg.dDecrBar = BAR_DECR_7;
			break;
		default:
			arg.dIncrBar = BAR_INCR_1;
			arg.dDecrBar = BAR_DECR_1;
			break;
		}
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
		nCode = FoundPresetGame1(x, 0, 1, 7);
		if(nCode == -1)
			arg.dUserVel = 1.0;
		else
		{
			switch(nIndPresetGame1[x][nCode])	// 0*5+nCode
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
		nCode = FoundPresetGame1(x, 0, 58, 58);
		if(nCode != -1) arg.dUserVel+=0.5;
		nCode = FoundPresetGame1(x, 0, 8, 9);
		if(nCode != -1)
		{
			switch(nIndPresetGame1[x][nCode])	// 0*5+nCode
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
		nCode = FoundPresetGame1(x, 1, 10, 11);
		if(nCode != -1)
		{
			switch(nIndPresetGame1[x][5+nCode])	// 0*5+nCode
			{
			case 10:
				arg.nVanish = 5;
				break;
			case 11:
				arg.nVanish = 11;
				break;
			}
		}
		if(FoundPresetGame1(x, 1, 12, 12) != -1) arg.bFlash = true;
		if(FoundPresetGame1(x, 1, 13, 13) != -1) arg.bFreedom = true;
		// Familia 2
		nCode = FoundPresetGame1(x, 2, 24, 45);
		if(nCode != -1)
		{
			switch(nIndPresetGame1[x][10+nCode])
			{
			case 24:
				arg.bRandomSkin = true;
				break;
			default:
				arg.nSkin = nIndPresetGame1[x][10+nCode]-24;
				break;
			}
		}
		// Familia 5
		nCode = FoundPresetGame1(x, 5, 46, 59);
		if(nCode != -1)
		{
			switch(nIndPresetGame1[x][25+nCode])
			{
			case 46:
				arg.bCharacter = true;
				arg.nCharacter = (rand()%12)+1;
				arg.nLevelCharacer = 20; //????
				break;
			default:
				arg.bCharacter = true;
				arg.nCharacter = nIndPresetGame1[x][25+nCode]-46;
				arg.nLevelCharacer = 20; //????
				break;
			}
		}
		// Familia 3
		nCode = FoundPresetGame1(x, 3, 14, 15);
		if(nCode != -1)
		{
			switch(nIndPresetGame1[x][15+nCode])	// 0*5+nCode
			{
			case 14:
				arg.bRS = true;
				break;
			case 15:
				arg.bMirror = true;
				break;
			}
		}
		//if(FoundPresetGame1(x, 3, 16, 16) != -1);
		if(FoundPresetGame1(x, 3, 17, 17) != -1) arg.bTrainingMode = true;
		if(FoundPresetGame1(x, 3, 18, 18) != -1) arg.dPowTimer = POW_TIMER_HARD;
		// Familia 4
		if(FoundPresetGame1(x, 4, 19, 19) != -1) arg.bSnake = true;
		if(FoundPresetGame1(x, 4, 20, 20) != -1) arg.bNXMode = true;
		if(FoundPresetGame1(x, 4, 21, 21) != -1) arg.bXMode = true;
		nCode = FoundPresetGame1(x, 4, 22, 23);
		if(nCode != -1)
		{
			switch(nIndPresetGame1[x][20+nCode])	// 0*5+nCode
			{
			case 22:
				arg.bUnderAttack = true;
				break;
			case 23:
				arg.bDrop= true;
				break;
			}
		}
		if(!LoadSongCompile(arg, x+1)) return 0;
	}
	return 1;
}

void DeleteSongGame1()
{
	for(int x = 0; x < 2; x++) if(np[x])
	{
		if(player_sesion[x]->nArcadeRecords[nRefSongsGame1[nCurSongGame1]*10+nModeCurSongGame1[x]][0] < (x?viewp2->status.nScore:viewp1->status.nScore))
		{
			for(int y = 19; y >= 0; y--)
			{
				player_sesion[x]->nArcadeRecords[nRefSongsGame1[nCurSongGame1]*10+nModeCurSongGame1[x]][y+1] =
					player_sesion[x]->nArcadeRecords[nRefSongsGame1[nCurSongGame1]*10+nModeCurSongGame1[x]][y];
			}
			player_sesion[x]->nArcadeRecords[nRefSongsGame1[nCurSongGame1]*10+nModeCurSongGame1[x]][0] = (x?viewp2->status.nScore:viewp1->status.nScore);
		}
		if(machine_sesion->nArcadeRecords[nRefSongsGame1[nCurSongGame1]*10+nModeCurSongGame1[x]] < (x?viewp2->status.nScore:viewp1->status.nScore))
		{
			machine_sesion->nArcadeRecords[nRefSongsGame1[nCurSongGame1]*10+nModeCurSongGame1[x]] = (x?viewp2->status.nScore:viewp1->status.nScore);
#ifndef _WIN32
			Utf16ToUtf32(player_sesion[x]->chName, machine_sesion->chArcadeRecords[nRefSongsGame1[nCurSongGame1]*10+nModeCurSongGame1[x]], 12);
#else
			_strcpy(machine_sesion->chArcadeRecords[nRefSongsGame1[nCurSongGame1]*10+nModeCurSongGame1[x]], player_sesion[x]->chName);
#endif
		}
	}
	for(int x = 0; x < 2; x++) if(g_bPlayer[x])
	{
		DeleteSongCompile(x+1);
	}
	DeleteSong();
}

CString GetIDTotalSongsGame1(int nIndex)
{
	if(nIndex >= nSizeTotalGame1) return CString(_T(""));
	CString str;
	str = ( chTotalSongsGame1[nIndex]);
	return str;
}

CString GetNameTotalSongsGame1(int nIndex)
{
	if(nIndex >= nSizeTotalGame1) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame1[nIndex], chTotalSongsGame1[nIndex], chLang);
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

CString GetArtistTotalSongsGame1(int nIndex)
{
	if(nIndex >= nSizeTotalGame1) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame1[nIndex], chTotalSongsGame1[nIndex], chLang);
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

CString GetBPMTotalSongsGame1(int nIndex)
{
	if(nIndex >= nSizeTotalGame1) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame1[nIndex], chTotalSongsGame1[nIndex], chLang);
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

CString GetNameSongsGame1(int nIndex)
{
	if(nIndex >= nSizeSongsGame1) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame1[nIndex], chSongsGame1[nIndex], chLang);
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

CString GetArtistSongsGame1(int nIndex)
{
	if(nIndex >= nSizeSongsGame1) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame1[nIndex], chSongsGame1[nIndex], chLang);
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

CString GetBPMSongsGame1(int nIndex)
{
	if(nIndex >= nSizeSongsGame1) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame1[nIndex], chSongsGame1[nIndex], chLang);
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

void GetLevelSongsGame1(int nIndex, int nMode, int* nLevel, double* d1, double* d2, double* d3, double* d4, double* d5)
{
	if(nIndex >= nSizeSongsGame1)
	{
		(*nLevel) = 1;
		(*d1) = 0.0;
		(*d2) = 0.0;
		(*d3) = 0.0;
		(*d4) = 0.0;
		(*d5) = 0.0;
		return;
	}
	if(nMode >= 10)
	{
		(*nLevel) = 1;
		(*d1) = 0.0;
		(*d2) = 0.0;
		(*d3) = 0.0;
		(*d4) = 0.0;
		(*d5) = 0.0;
		return;
	}
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.sksf"), chSongsGame1[nIndex], chSongsGame1[nIndex], chStdModes[nMode]);
	FILE* hFile = _fopen(chFile, _T("rb"));
	if(hFile == NULL)
	{
		(*nLevel) = 1;
		(*d1) = 0.0;
		(*d2) = 0.0;
		(*d3) = 0.0;
		(*d4) = 0.0;
		(*d5) = 0.0;
		return;
	}
	STEPHEADER hStep;
	fread(&hStep, sizeof(STEPHEADER), 1, hFile);
	(*nLevel) = min(max(hStep.nDificulty, 1), 30);
	(*d1) = hStep.dStrength;
	(*d2) = hStep.dVoltage;
	(*d3) = hStep.dAir;
	(*d4) = hStep.dFreeze;
	(*d5) = hStep.dCaos;
	fclose(hFile);
}

void GetBestScores(int nIndex, int nMode)
{
	nBestScoreMachineGame1 = machine_sesion->nArcadeRecords[nRefSongsGame1[nIndex]*10+nMode];
	for(int x = 0; x < 2; x++)
	{
		if(np[x]) nBestScorePlayerGame1[x] = player_sesion[x]->nArcadeRecords[nRefSongsGame1[nIndex]*10+nMode][0];
		else nBestScorePlayerGame1[x] = 0;
	}
	strBestScoreMachineGame1 = ( machine_sesion->chArcadeRecords[nRefSongsGame1[nIndex]*10+nMode]);
}

int LoadDefaults(int nPlayer)
{
	if(!AdaptMachineProfile(1, nPlayer)) return 0;
	return 1;
}

int EnumTotalSongs()
{
	FILE* hFile = NULL;
	hFile = _fopen(_T("songArcadeNew.acc"), _T("r"));
	if(hFile == NULL) return 0;
	TCHAR ch[500];
	int nChannel = 0;
	int nIsRetro = 0;
	nSizeTotalGame1 = 0;
	while(_fgets(ch, 500, hFile) != NULL)
	{
		CString str;
		str = ( ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		if(str.Find(_T('*')) == -1)	// No es un comentario
		{
			if(str.Find(_T('[')) != -1)	// Channel
			{
				if(_sscanf(str, _T("[%d]"), &nChannel) != 1)
				{
					fclose(hFile);
					return 0;
				}
				if(nChannel == -1)	// Retro Zone
				{
					nIsRetro = 1;
				}
			}
			if(str.Find(_T(':')) != -1)	// Song
			{
				nSizeTotalGame1++;
				int nCode = str.Find(_T(':'));
				_strcpy(chTotalSongsGame1[nSizeTotalGame1-1], str.Left(nCode));
				nCode = str.GetLength() - nCode - 1;
				CString stra = str.Right(nCode);
				int n[] = {0,0,0,0,0,0,0,0,0,0,0};
				if(_sscanf(stra, _T("\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d")
					, &n[0], &n[1], &n[2], &n[3], &n[4], &n[5], &n[6], &n[7], &n[8], &n[9], &n[10]) < 1)
				{
					fclose(hFile);
					return 0;
				}
				for(int x = 0; x < 10; x++) nModTotalSongsGame1[(nSizeTotalGame1-1)*10+x] = n[x];
				nChaTotalSongsGame1[nSizeTotalGame1-1] = nChannel==-1?0:nChannel;
				nNOLTotalSongsGame1[nSizeTotalGame1-1] = nIsRetro;
				nStageTotalSongsGame1[nSizeTotalGame1-1] = min(max(1, n[10]), 8);
			}
		}
	}
	fclose(hFile);
	nSizeTotalGame1Ex = 0;
	for(int x = 0; x < (nSizeTotalGame1*10); x++) if(nModTotalSongsGame1[x]) nSizeTotalGame1Ex++;
	return 1;
}

int EnumSongs(int nSet)
{
	nSizeSongsGame1 = 0;
	int nAutorize[10000];
	for(int k = 0; k < 10000; k++)
	{
		nAutorize[k] = 0;
		nModSongsGame1[k] = 0;
		if(k < 1000)
		{
			nChaSongsGame1[k] = 0;
		}
	}
	int nNp = max(np[0]?np[0]:0, np[1]?np[1]:0);
	for(int d = 0; d < 2; d++) if(np[d])
	{
		for(int s = 0; s < nSizeTotalGame1; s++)
		{
			if(nNp < nStageTotalSongsGame1[s])
			{
				for(int m = 0; m < 10; m++)
				{
					nAutorize[s*10+m] = 0;
				}
			}
			else for(int m = 0; m < 10; m++)
			{
				if(!player_sesion[d]->nLockedSongNew[s*10+m] || !machine_sesion->nLockedSongNew[s*10+m] || player_sesion[d]->nLockedAsp[ITEM_ALLSONG])
					nAutorize[s*10+m] = 1;
			}
		}
	}
	for(int s = 0; s < nSizeTotalGame1; s++)
	{
		bool bAutorize = false;
		for(int m = 0; m < ((np[0]&&np[1])?3:5); m++)
		{
			if(nModTotalSongsGame1[s*10+m] && nAutorize[s*10+m]) bAutorize = true;
			if(nModTotalSongsGame1[s*10+m+5] && nAutorize[s*10+m+5]) bAutorize = true;
		}
		//if(nNOLTotalSongsGame1[s] != nSet) bAutorize = false;
		if(bAutorize)
		{
			for(int m = 0; m < 10; m++)
				if(nModTotalSongsGame1[s*10+m] && nAutorize[s*10+m])
					nModSongsGame1[nSizeSongsGame1*10+m] = 1;
			nChaSongsGame1[nSizeSongsGame1] = nChaTotalSongsGame1[s];
			nStageSongsGame1[nSizeSongsGame1] = nStageTotalSongsGame1[s];
			nRefSongsGame1[nSizeSongsGame1] = s;
			_strcpy(chSongsGame1[nSizeSongsGame1], chTotalSongsGame1[s]);
			nSizeSongsGame1++;
		}
	}
	for(int s = 0; s < nSizeSongsGame1; s++)
	{
		bool b1 = false, b2 = false;
		for(int t = 0; t < 5; t++) if(nModSongsGame1[s*10+t])
		{
			b1 = true;
			break;
		}
		for(int t = 5; t < 10; t++) if(nModSongsGame1[s*10+t])
		{
			b2 = true;
			break;
		}
		if(b1 && b2) nExModSongsGame1[s] = 1;
		else nExModSongsGame1[s] = 0;
	}
	return 1;
}

int GetAvalibleMode(int nIndexSong, int nModeGame1Song)
{
	if(nIndexSong < 0 || nIndexSong >= nSizeSongsGame1) return 0;
	if(nModeGame1Song < 0 || nModeGame1Song >= 10)
	{
		for(int y = 0; y < 10; y++)
		{
			if(nModSongsGame1[nIndexSong*10+y]) return y;
		}
	}
	if(nModSongsGame1[nIndexSong*10+nModeGame1Song])
		return nModeGame1Song;
	else
	{
		for(int y = 0; y < 10; y++)
		{
			if(nModSongsGame1[nIndexSong*10+y]) return y;
		}
	}
	return 0;
}

bool IsAvalibleMode(int nIndexSong, int nModeGame1Song)
{
	if(nIndexSong < 0 || nIndexSong >= nSizeSongsGame1) return false;
	if(nModeGame1Song < 0 || nModeGame1Song >= 10) return false;
	if(np[0] && np[1] && (nModeGame1Song == 3 || nModeGame1Song == 4 || nModeGame1Song == 8 || nModeGame1Song == 9)) return false;
	if(!nModSongsGame1[nIndexSong*10+nModeGame1Song]) return false;
	return true;
}

bool IsAvalibleModeTotal(int nIndexSong, int nModeGame1Song)
{
	if(nIndexSong < 0 || nIndexSong >= nSizeTotalGame1) return false;
	if(nModeGame1Song < 0 || nModeGame1Song >= 10) return false;
	if(!nModTotalSongsGame1[nIndexSong*10+nModeGame1Song]) return false;
	return true;
}

#define MAX_TARGET_X 60.0
double AlphaFuncForX(double dx)
{
	if(abs(dx) >= MAX_TARGET_X) return 0.0;
	if(abs(dx) <= (MAX_TARGET_X/4*3)) return 1.0;
	return (MAX_TARGET_X - abs(dx)) / (MAX_TARGET_X/4);
}

double ThetaFuncForX(double dx)
{
	if(abs(dx) >= MAX_TARGET_X/4) return -19.0*dx/abs(dx);
	/*if(abs(dx) <= (MAX_TARGET_X/4))*/ return -19.0*dx*4/MAX_TARGET_X;
	//return (MAX_TARGET_X - abs(dx)) / (MAX_TARGET_X/4);
}


void ActualizateIndexShowGame1()
{
	for(int x = 0; x < 9; x++)
	{
		nIndexShowGame1[x] = nCurSongGame1+x-4;
		bool bAsegureLoop = true;
		while(bAsegureLoop)
		{
			bAsegureLoop = false;
			if(nIndexShowGame1[x] < 0)
			{
				nIndexShowGame1[x] = nSizeSongsGame1+nIndexShowGame1[x];
				bAsegureLoop = true;
			}
			if(nIndexShowGame1[x] >= nSizeSongsGame1)
			{
				nIndexShowGame1[x] = nIndexShowGame1[x]-nSizeSongsGame1;
				bAsegureLoop = true;
			}
		}
	}
}

void MoveMode(int nPlayer, bool bIsIzq)
{
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
	nChosesGame1 = 0;
	nTimeModeCurSongGame1[nPlayer] = nCountGame1;
	nBackModeCurSongGame1[nPlayer] = nModeCurSongGame1[nPlayer];
	if(bIsIzq)
	{
		do
		{
			nModeCurSongGame1[nPlayer]--;
			if(nModeCurSongGame1[nPlayer] < 0) nModeCurSongGame1[nPlayer] = 9;
		} while(!IsAvalibleMode(nCurSongGame1, nModeCurSongGame1[nPlayer]));
	}
	else
	{
		do
		{
			nModeCurSongGame1[nPlayer]++;
			if(nModeCurSongGame1[nPlayer] > 9) nModeCurSongGame1[nPlayer] = 0;
		} while(!IsAvalibleMode(nCurSongGame1, nModeCurSongGame1[nPlayer]));
	}
	nPrefModeCurSongGame1[nPlayer] = nModeCurSongGame1[nPlayer];
	GetLevelSongsGame1(nCurSongGame1, nModeCurSongGame1[nPlayer], &nLevelCurGame1[nPlayer],
	&dDCurGame1[nPlayer][0],
	&dDCurGame1[nPlayer][1],
	&dDCurGame1[nPlayer][2],
	&dDCurGame1[nPlayer][3],
	&dDCurGame1[nPlayer][4]);
	GetBestScores(nCurSongGame1, nModeCurSongGame1[nPlayer]);
}

void MoveCha(bool bIsIzq)
{
	FMOD_Channel_Stop(channels[9]);
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_change3, 0, &channels[7]);
	nChosesGame1 = 0;
	bool bLoopLook = true;
	int nIndexLook = nCurSongGame1;
	if(bIsIzq)
	{
		while(bLoopLook)
		{
			nIndexLook--;
			if(nIndexLook < 0) nIndexLook = nSizeSongsGame1-1;
			if(nChaSongsGame1[nIndexLook] != nChaSongsGame1[nCurSongGame1] || nIndexLook == nCurSongGame1)
				bLoopLook = false;
		}
	}
	else
	{
		while(bLoopLook)
		{
			nIndexLook++;
			if(nIndexLook >= nSizeSongsGame1) nIndexLook = 0;
			if(nChaSongsGame1[nIndexLook] != nChaSongsGame1[nCurSongGame1] || nIndexLook == nCurSongGame1)
				bLoopLook = false;
		}
	}
	glDeleteTextures(9, nTexCurSongGame1);
	nCurSongGame1 = nIndexLook;
	nTimeChaGame1 = nCountGame1;
	TCHAR str[260];
	ActualizateIndexShowGame1();
	for(int x = 0; x < 9; x++)
	{
		_sprintf(str, _T("song/preview/%ls.PNG"), chSongsGame1[nIndexShowGame1[x]]);
		nTexCurSongGame1[x] = GenTexImage(str);
	}
	for(int y = 0; y < 2; y++) if(np[y])
	{
		for(int k = 0; k < 5; k++) dDCurGame1[y][k] = 0.0;
		nTimeModeCurSongGame1[y] = nCountGame1;
		nBackModeCurSongGame1[y] = nModeCurSongGame1[y];
		nModeCurSongGame1[y] = GetAvalibleMode(nCurSongGame1, nPrefModeCurSongGame1[y]);
		GetLevelSongsGame1(nCurSongGame1, nModeCurSongGame1[y], &nLevelCurGame1[y],
		&dDCurGame1[y][0],
		&dDCurGame1[y][1],
		&dDCurGame1[y][2],
		&dDCurGame1[y][3],
		&dDCurGame1[y][4]);
		GetBestScores(y, nCurSongGame1);
	}
	bShow = false;
}

void MoveSongs(bool bIsIzq)
{
	int nb = nCurSongGame1;
	if(bIsIzq)
	{
		nCurSongGame1--;
		if(nCurSongGame1 < 0) nCurSongGame1 = nSizeSongsGame1-1;
		if(nChaSongsGame1[nb] != nChaSongsGame1[nCurSongGame1]) nTimeChaGame1 = nCountGame1;
		nModeGame1 = -1;
		nChosesGame1 = 0;
		/*Borrar textura sobrante*/
		glDeleteTextures(1, &nTexCurSongGame1[8]);
		nTexCurSongGame1[8] = 0;
		ActualizateIndexShowGame1();
		//Mover a la derecha los items
		for(int x = 7; x >= 0; x--)
		{
			nTexCurSongGame1[x+1] = nTexCurSongGame1[x];
		}

        TCHAR str[260];
		_sprintf(str, _T("song/preview/%ls.PNG"), chSongsGame1[nIndexShowGame1[0]]);

		nTexCurSongGame1[0] = GenTexImage(str);
		nTimeCurSongGame1 = nCountGame1;
	}
	else
	{
		nCurSongGame1++;
		if(nCurSongGame1 >= nSizeSongsGame1) nCurSongGame1 = 0;
		if(nChaSongsGame1[nb] != nChaSongsGame1[nCurSongGame1]) nTimeChaGame1 = nCountGame1;
		nModeGame1 = 1;
		nChosesGame1 = 0;
		/*Borrar textura sobrante*/
		glDeleteTextures(1, &nTexCurSongGame1[0]);
		ActualizateIndexShowGame1();
		//Mover a la izquierda los items
		for(int x = 0; x < 8; x++)
		{
			nTexCurSongGame1[x] = nTexCurSongGame1[x+1];
		}
		// Finalmente cargar la nueva imagen
		TCHAR str[260];
		_sprintf(str, _T("song/preview/%ls.PNG"), chSongsGame1[nIndexShowGame1[8]]);

		nTexCurSongGame1[8] = GenTexImage(str);
		nTimeCurSongGame1 = nCountGame1;
	}
	for(int y = 0; y < 2; y++)
	{
		for(int k = 0; k < 5; k++) dDCurGame1[y][k] = 0.0;
		nTimeModeCurSongGame1[y] = nCountGame1;
		nBackModeCurSongGame1[y] = nModeCurSongGame1[y];
		nModeCurSongGame1[y] = GetAvalibleMode(nCurSongGame1, nPrefModeCurSongGame1[y]);
	}
}

int FoundPresetGame1(int x, int nFamily, int nBegin, int nEnd)
{
	int nFound = -1;
	for(int k = 0; k < nIndSizePresetGame1[x][nFamily]; k++)
	{
		if(nIndPresetGame1[x][nFamily*5+k] >= nBegin && nIndPresetGame1[x][nFamily*5+k] <= nEnd)
		{
			nFound = k;
			break;
		}
	}
	return nFound;
}

void CheckPreset()
{
	for(int x = 0; x < 2; x++) if(np[x])
	{
		bool bPreset = false;
		int nFamily;
		// ** Primer nivel (0)
		nFamily = 0;
		// Random Velocity (RV)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 3 &&
		   nComandGame1[x][2] == 1 &&
		   nComandGame1[x][3] == 3 &&
		   nComandGame1[x][4] == 1 &&
		   nComandGame1[x][5] == 3 &&
		   nComandGame1[x][6] == 1 &&
		   nComandGame1[x][7] == 3 &&
		   nComandGame1[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 1, 7);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 7;
				nIndLastPresetGame1[x] = nFamily*5+nFound;
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 7)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 7;
					nIndLastPresetGame1[x] = nFamily*5+nFound;
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// 0.5 plus (0.5+)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 3 &&
		   nComandGame1[x][2] == 1 &&
		   nComandGame1[x][3] == 3 &&
		   nComandGame1[x][4] == 1 &&
		   nComandGame1[x][5] == 3 &&
		   nComandGame1[x][6] == 1 && !bPreset)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 58, 58);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 58;
				nIndLastPresetGame1[x] = nFamily*5+nFound;
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 58)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 58;
					nIndLastPresetGame1[x] = nFamily*5+nFound;
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Speed Up
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 3 &&
		   nComandGame1[x][2] == 1 &&
		   nComandGame1[x][3] == 3 &&
		   nComandGame1[x][4] == 1 && !bPreset)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 1, 7);
			int nCurVel = 0;
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nCurVel = 0;
			}
			else nCurVel = nIndPresetGame1[x][nFamily*5+nFound];
			nCurVel++;
			if(nCurVel > 5 || nCurVel < 1)
			{
				nIndPresetGame1[x][nFamily*5+nFound] = 0;
				for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
					nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
				nIndSizePresetGame1[x][nFamily]--;
			}
			else
			{
				nIndPresetGame1[x][nFamily*5+nFound] = nCurVel;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
		}
		// Speed Down
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 1 &&
		   nComandGame1[x][2] == 3 &&
		   nComandGame1[x][3] == 1 &&
		   nComandGame1[x][4] == 3 && !bPreset)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 1, 7);
			int nCurVel = 6;
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nCurVel = 6;
			}
			else nCurVel = nIndPresetGame1[x][nFamily*5+nFound];
			nCurVel--;
			if(nCurVel > 5 || nCurVel < 1)
			{
				nIndPresetGame1[x][nFamily*5+nFound] = 0;
				for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
					nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
				nIndSizePresetGame1[x][nFamily]--;
			}
			else
			{
				nIndPresetGame1[x][nFamily*5+nFound] = nCurVel;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
		}
		// EarthWorm (EW)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 1 &&
		   nComandGame1[x][2] == 0 &&
		   nComandGame1[x][3] == 3 &&
		   nComandGame1[x][4] == 4 &&
		   nComandGame1[x][5] == 1 &&
		   nComandGame1[x][6] == 3 &&
		   nComandGame1[x][7] == 0 &&
		   nComandGame1[x][8] == 4)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, 0, 1, 7);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 6;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 6)	// Desactivar
				{
					nIndPresetGame1[x][nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][k] = nIndPresetGame1[x][k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 6;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Aceleration (AC)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 3 &&
		   nComandGame1[x][2] == 3 &&
		   nComandGame1[x][3] == 1 &&
		   nComandGame1[x][4] == 1 &&
		   nComandGame1[x][5] == 4 &&
		   nComandGame1[x][6] == 4 &&
		   nComandGame1[x][7] == 0 &&
		   nComandGame1[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 8, 9);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 8;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 8)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 8;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Desceleration (DC)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 1 &&
		   nComandGame1[x][2] == 1 &&
		   nComandGame1[x][3] == 3 &&
		   nComandGame1[x][4] == 3 &&
		   nComandGame1[x][5] == 0 &&
		   nComandGame1[x][6] == 0 &&
		   nComandGame1[x][7] == 4 &&
		   nComandGame1[x][8] == 4)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 8, 9);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 9;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 9)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 9;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// ** Segundo nivel (1)
		nFamily = 1;
		// Vanish & NonStep (V & NS)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 4 &&
		   nComandGame1[x][2] == 0 &&
		   nComandGame1[x][3] == 3 &&
		   nComandGame1[x][4] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 10, 11);
			int nCur = 0;
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nCur = 9;
			}
			else nCur = nIndPresetGame1[x][nFamily*5+nFound];
			nCur++;
			if(nCur > 11 || nCur < 10)
			{
				nIndPresetGame1[x][nFamily*5+nFound] = 0;
				for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
					nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
				nIndSizePresetGame1[x][nFamily]--;
			}
			else
			{
				nIndPresetGame1[x][nFamily*5+nFound] = nCur;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
		}
		// Flash (FL)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 3 &&
		   nComandGame1[x][2] == 1 &&
		   nComandGame1[x][3] == 4 &&
		   nComandGame1[x][4] == 3 &&
		   nComandGame1[x][5] == 0 &&
		   nComandGame1[x][6] == 4 &&
		   nComandGame1[x][7] == 1 &&
		   nComandGame1[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 12, 12);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 12;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 12)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 12;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Freedom (FD)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 0 &&
		   nComandGame1[x][2] == 3 &&
		   nComandGame1[x][3] == 1 &&
		   nComandGame1[x][4] == 4 &&
		   nComandGame1[x][5] == 4 &&
		   nComandGame1[x][6] == 3 &&
		   nComandGame1[x][7] == 0 &&
		   nComandGame1[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 13, 13);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 13;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 13)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 13;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// ** Tercer nivel (2)
		nFamily = 2;
		// Random Skin (RSK)
		if(nComandGame1[x][0] == 1 &&
		   nComandGame1[x][1] == 1 &&
		   nComandGame1[x][2] == 1 &&
		   nComandGame1[x][3] == 3 &&
		   nComandGame1[x][4] == 3 &&
		   nComandGame1[x][5] == 3 &&
		   nComandGame1[x][6] == 0 &&
		   nComandGame1[x][7] == 0 &&
		   nComandGame1[x][8] == 0 &&
		   nComandGame1[x][9] == 4 &&
		   nComandGame1[x][10] == 4 &&
		   nComandGame1[x][11] == 4)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 24, 45);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 24;
				nIndLastPresetGame1[x] = nFamily*5+nFound;
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 24)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 24;
					nIndLastPresetGame1[x] = nFamily*5+nFound;
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		nFamily = 5;
		// Random Character (RCH)
		if(nComandGame1[x][0] == 4 &&
		   nComandGame1[x][1] == 4 &&
		   nComandGame1[x][2] == 4 &&
		   nComandGame1[x][3] == 0 &&
		   nComandGame1[x][4] == 0 &&
		   nComandGame1[x][5] == 0 &&
		   nComandGame1[x][6] == 3 &&
		   nComandGame1[x][7] == 3 &&
		   nComandGame1[x][8] == 3 &&
		   nComandGame1[x][9] == 1 &&
		   nComandGame1[x][10] == 1 &&
		   nComandGame1[x][11] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 46, 59);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 45;
				nIndLastPresetGame1[x] = nFamily*5+nFound;
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 45)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 45;
					nIndLastPresetGame1[x] = nFamily*5+nFound;
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Skins
		if(nComandGame1[x][0] == 1 &&
		   nComandGame1[x][1] == 2 &&
		   nComandGame1[x][2] == 4 &&
		   nComandGame1[x][3] == 0 &&
		   nComandGame1[x][4] == 2 &&
		   nComandGame1[x][5] == 3)
		{
			bPreset = true;
			bSkinGame1[x] = true;
			nTimeSkinGame1[x] = nCountGame1;
			nCurSkinGame1[x] = 0;
			nDirTurnSkinGame1[x] = 0;
		}
		if(nComandGame1[x][0] == 3 &&
		   nComandGame1[x][1] == 2 &&
		   nComandGame1[x][2] == 0 &&
		   nComandGame1[x][3] == 4 &&
		   nComandGame1[x][4] == 2 &&
		   nComandGame1[x][5] == 1)
		{
			bPreset = true;
			bCharacterGame1[x] = true;
			nTimeCharacterGame1[x] = nCountGame1;
			nCurCharacterGame1[x] = 0;
			nDirTurnCharacterGame1[x] = 0;
		}
		// Characters
		// ** Cuarto Nivel (3)
		nFamily = 3;
		// Random Step (RS)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 4 &&
		   nComandGame1[x][2] == 0 &&
		   nComandGame1[x][3] == 4 &&
		   nComandGame1[x][4] == 0 &&
		   nComandGame1[x][5] == 3 &&
		   nComandGame1[x][6] == 1 &&
		   nComandGame1[x][7] == 3 &&
		   nComandGame1[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 14, 15);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 14;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 14)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 14;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Mirror (M)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 1 &&
		   nComandGame1[x][2] == 3 &&
		   nComandGame1[x][3] == 0 &&
		   nComandGame1[x][4] == 4 &&
		   nComandGame1[x][5] == 1 &&
		   nComandGame1[x][6] == 3 &&
		   nComandGame1[x][7] == 0 &&
		   nComandGame1[x][8] == 4)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 14, 15);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 15;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 15)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 15;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Reverse Grade (RG)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 1 &&
		   nComandGame1[x][2] == 3 &&
		   nComandGame1[x][3] == 2 &&
		   nComandGame1[x][4] == 0 &&
		   nComandGame1[x][5] == 4 &&
		   nComandGame1[x][6] == 4 &&
		   nComandGame1[x][7] == 0 &&
		   nComandGame1[x][8] == 2 &&
		   nComandGame1[x][9] == 3 &&
		   nComandGame1[x][10] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 16, 16);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 16;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 16)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 16;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Training Mode (T)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 4 &&
		   nComandGame1[x][2] == 1 &&
		   nComandGame1[x][3] == 4 &&
		   nComandGame1[x][4] == 1 &&
		   nComandGame1[x][5] == 3 &&
		   nComandGame1[x][6] == 0 &&
		   nComandGame1[x][7] == 3 &&
		   nComandGame1[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 17, 17);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 17;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 17)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 17;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Hard Judgament (HJ)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 4 &&
		   nComandGame1[x][2] == 3 &&
		   nComandGame1[x][3] == 1 &&
		   nComandGame1[x][4] == 0 &&
		   nComandGame1[x][5] == 1 &&
		   nComandGame1[x][6] == 4 &&
		   nComandGame1[x][7] == 2 &&
		   nComandGame1[x][8] == 0 &&
		   nComandGame1[x][9] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 18, 18);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 18;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 18)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 18;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// ** Quinto Nivel (4)
		nFamily = 4;
		// Snake (SN)
		if(nComandGame1[x][0] == 0 &&
		   nComandGame1[x][1] == 4 &&
		   nComandGame1[x][2] == 2 &&
		   nComandGame1[x][3] == 1 &&
		   nComandGame1[x][4] == 3 &&
		   nComandGame1[x][5] == 0 &&
		   nComandGame1[x][6] == 4 &&
		   nComandGame1[x][7] == 2 &&
		   nComandGame1[x][8] == 1 &&
		   nComandGame1[x][9] == 3)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 19, 19);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 19;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 19)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 19;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// NX Mode (NX)
		if(nComandGame1[x][0] == 3 &&
		   nComandGame1[x][1] == 4 &&
		   nComandGame1[x][2] == 2 &&
		   nComandGame1[x][3] == 1 &&
		   nComandGame1[x][4] == 0 &&
		   nComandGame1[x][5] == 3 &&
		   nComandGame1[x][6] == 4 &&
		   nComandGame1[x][7] == 2 &&
		   nComandGame1[x][8] == 1 &&
		   nComandGame1[x][9] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 20, 20);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 20;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 20)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 20;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// X Mode (X)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 1 &&
		   nComandGame1[x][2] == 4 &&
		   nComandGame1[x][3] == 1 &&
		   nComandGame1[x][4] == 4 &&
		   nComandGame1[x][5] == 3 &&
		   nComandGame1[x][6] == 0 &&
		   nComandGame1[x][7] == 3 &&
		   nComandGame1[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 21, 21);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 21;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 21)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 21;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// UnderAttack (UA)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 3 &&
		   nComandGame1[x][2] == 2 &&
		   nComandGame1[x][3] == 0 &&
		   nComandGame1[x][4] == 2 &&
		   nComandGame1[x][5] == 0 &&
		   nComandGame1[x][6] == 2 &&
		   nComandGame1[x][7] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 22, 23);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 22;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 22)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 22;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Drop (DR)
		if(nComandGame1[x][0] == 4 &&
		   nComandGame1[x][1] == 4 &&
		   nComandGame1[x][2] == 3 &&
		   nComandGame1[x][3] == 0 &&
		   nComandGame1[x][4] == 1 &&
		   nComandGame1[x][5] == 4 &&
		   nComandGame1[x][6] == 3 &&
		   nComandGame1[x][7] == 0 &&
		   nComandGame1[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 22, 23);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 23;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 23)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 23;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// ** Sexto Nivel (5)
		nFamily = 5;
		// No BGA (NBGA)
		if(nComandGame1[x][0] == 2 &&
		   nComandGame1[x][1] == 0 &&
		   nComandGame1[x][2] == 3 &&
		   nComandGame1[x][3] == 1 &&
		   nComandGame1[x][4] == 3 &&
		   nComandGame1[x][5] == 1 &&
		   nComandGame1[x][6] == 4)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame1(x, nFamily, 59, 59);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame1[x][nFamily];
				nIndSizePresetGame1[x][nFamily]++;
				nIndPresetGame1[x][nFamily*5+nFound] = 59;
				nIndLastPresetGame1[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame1[x] = nCountGame1;
			}
			else
			{
				if(nIndPresetGame1[x][nFamily*5+nFound] == 59)	// Desactivar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame1[x][nFamily]-1); k++)
						nIndPresetGame1[x][nFamily*5+k] = nIndPresetGame1[x][nFamily*5+k+1];
					nIndSizePresetGame1[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame1[x][nFamily*5+nFound] = 59;
					nIndLastPresetGame1[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame1[x] = nCountGame1;
				}
			}
		}
		// Switch Unlocker (??)
		if(nComandGame1[x][0] == 0 &&
		   nComandGame1[x][1] == 1 &&
		   nComandGame1[x][2] == 2 &&
		   nComandGame1[x][3] == 3 &&
		   nComandGame1[x][4] == 4 &&
		   nComandGame1[x][5] == 3 &&
		   nComandGame1[x][6] == 2 &&
		   nComandGame1[x][7] == 1 &&
		   nComandGame1[x][8] == 0)
		{
			nChangeRetroGame1 = 3;
			nTimeChangeRetroGame1 = nCountGame1;
			bIsAvalibleUnlockerGame1 = false;
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
			for(int y = 0; y < 10; y++)
			{
				bArrows[y] = false;
				bEnArrowsGame1[y] = false;
				bCh[y] = false;
			}
		}
		// Toggle Null
		if(nComandGame1[x][0] == 4 &&
		   nComandGame1[x][1] == 0 &&
		   nComandGame1[x][2] == 4 &&
		   nComandGame1[x][3] == 0 &&
		   nComandGame1[x][4] == 4 &&
		   nComandGame1[x][5] == 0)
		{
			bPreset = true;
			for(int z = 0; z < 30; z++) nIndPresetGame1[x][z] = 0;
			for(int z = 0; z < 6; z++) nIndSizePresetGame1[x][z] = 0;
			nIndSizePresetGame1[x][0] = 0;
			nIndPresetGame1[x][0] = 1;
			nIndLastPresetGame1[x] = 0;
			nTimeLastPresetGame1[x] = nCountGame1;
		}
		if(bPreset)
		{
			nChosesGame1 = 0;
			nComandGame1[x][0] = -1;
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_option, 0, &channels[2]);
		}
	}
}

#define TIME_FAST_CHANGE_GAME1 1000
#define TIME_CHANGE_LOW_GAME1 350
#define TIME_CHANGE_HIGH_GAME1 100

void PlaySoundTransiAboutLimit(int nLimit)
{
	if(nLimit >= TIME_CHANGE_LOW_GAME1)
	{
		nTimeTransiSongBorderGame1 = TIME_TRANSI_SONGBORDER_GAME1;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
	}
	else
	{
		nTimeTransiSongBorderGame1 = TIME_CHANGE_HIGH_GAME1;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)6, sound_change2, 0, &channels[6]);
	}
}

int DrawInterfaceSelect()
{
	if(!np[0] && !np[1]) return 1;
	nCountGame1 += dwTick-dwLastTick;
	if(movie_arcadebg->bLoad)
	{
		movie_arcadebg->Update(dwWait);
		movie_arcadebg->GrabAVIFrame();
		glBindTexture(GL_TEXTURE_2D, movie_arcadebg->tex);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
		glEnd();
	}

	double dApt = 0.0;
	static __int64 nTimeShow = 0;
	__int64 nTransBorder = nCountGame1 - nTimeCurSongGame1;
	int nOrder[] = {0,1,2,3,8,7,6,5,4};
#define DY_SONGBORDER_HOLD_GAME1 -3.0
	if(nChangeRetroGame1 == 0)
	{
		if(nChosesGame1 < CHOOSES_GAME1)
		{
			for(int i = 0; i < 9; i++)
			{
				int x = nOrder[i];
				double dTargetx = double(x-4)*MAX_TARGET_X/4;
				if(nCountGame1 >= TIME_START_SELECTOR_GAME1 && nCountGame1 < (TIME_START_SELECTOR_GAME1+TIME_TRANSI_SELECTOR_GAME1))
				{
					if(x == 4)
					{
						if(nCountGame1 < (TIME_TRANSI_START_MAIN_GAME1+TIME_START_SELECTOR_GAME1))
						{
							dApt = pow(double(nCountGame1-TIME_START_SELECTOR_GAME1)/TIME_TRANSI_START_MAIN_GAME1, 2);
							glPushMatrix();
							glTranslated(0.0, DY_SONGBORDER_HOLD_GAME1, -60.0);
							glScaled(1.25+(1-dApt), 1.25+(1-dApt), 1.0);
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
						if(nCountGame1 >= (TIME_TRANSI_START_MAIN_GAME1+TIME_START_SELECTOR_GAME1))
						{
							glPushMatrix();
							glTranslated(0.0, DY_SONGBORDER_HOLD_GAME1, -60.0);
							glScaled(1.25, 1.25, 1.0);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
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
						__int64 nStartTransi = TIME_START_SELECTOR_GAME1+TIME_TRANSI_SELECTOR_GAME1/4*(abs(x-4)-1);
						__int64 nLimitTransi = TIME_START_SELECTOR_GAME1+TIME_TRANSI_SELECTOR_GAME1/4*(abs(x-4));
						if(nCountGame1 >= nStartTransi && nCountGame1 < nLimitTransi)
						{
							dApt = double(nCountGame1-nStartTransi)/double(nLimitTransi-nStartTransi);
							double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(1.0-dApt)+dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0+DY_SONGBORDER_HOLD_GAME1;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForX(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForX(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
  							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
						if(nCountGame1 >= nLimitTransi)
						{
							double dx = dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0+DY_SONGBORDER_HOLD_GAME1;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForX(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForX(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
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
				if(nCountGame1 >= (TIME_START_SELECTOR_GAME1+TIME_TRANSI_SELECTOR_GAME1))
				{
					double dx = dTargetx;
					if(nModeGame1!=0)
					{
						if(nTransBorder < nTimeTransiSongBorderGame1)
						{
							dx += double(nModeGame1)*MAX_TARGET_X/4*(1.0-double(nTransBorder)/double(nTimeTransiSongBorderGame1));
						}
					}
					double dy = pow(dx/MAX_TARGET_X,3.0)*50.0+DY_SONGBORDER_HOLD_GAME1;	// 0.75*40.0
					double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
					double dtheta = ThetaFuncForX(dx);
					double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
					double dalpha = AlphaFuncForX(dx);
					glPushMatrix();
					glTranslated(dx, dy, dz-60.0);
					glRotated(dtheta, 0.0, 1.0, 0.0);
					glScaled(dscale, dscale, 1.0);
					glColor4d(1.0, 1.0, 1.0, dalpha);
					if (x == 4 && nTransBorder > TIME_SHOW_PREVIEW_GAME1 && movie_songpreview && bShow  && movie_songpreview->bLoad && movie_songpreview->bResize)
					{
	#define TIME_SHOWIMAGE_SONGBORDER_GAME1 500
						movie_songpreview->Update(dwWait);
						movie_songpreview->GrabAVIFrame();
						__int64 nCaretCover = movie_songpreview->dwNext;
						glBindTexture(GL_TEXTURE_2D, movie_songpreview->tex);

						glColor4d(1.0, 1.0, 1.0, 1.0);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
						glEnd();
						if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME1)
						{
							dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME1;
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
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
						glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
						glEnd();
					}
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					if(x == 4 && nTransBorder > TIME_SHOW_PREVIEW_GAME1)
					{
						#define TIME_SHOWNAME_BESTSCORE_GAME1 500
						if(nTransBorder < (TIME_SHOW_PREVIEW_GAME1+TIME_SHOWNAME_BESTSCORE_GAME1))
						{
							dApt = double(nTransBorder-TIME_SHOW_PREVIEW_GAME1)/TIME_SHOWNAME_BESTSCORE_GAME1;
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_CoverNames]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						if(nTransBorder >= (TIME_SHOW_PREVIEW_GAME1+TIME_SHOWNAME_BESTSCORE_GAME1))
						{
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_CoverNames]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
						}
						__int64 nModeTime = nCountGame1 - max(max(nTimeModeCurSongGame1[0], nTimeModeCurSongGame1[1]), nTransBorder+TIME_SHOW_PREVIEW_GAME1);
						if(nModeTime >= 0 && nModeTime < (TIME_SHOWNAME_BESTSCORE_GAME1) && bShow)
						{
							dApt = double(nModeTime)/TIME_SHOWNAME_BESTSCORE_GAME1;
							glColor4d(1.0, 1.0, 1.0, dApt);
							if(nBestScoreMachineGame1 != 0)
							{
								font_Micro->glDrawText(10.599, -9.0625, 9.05331, 9.0625, 0.0, 10.076/*9.82615*/, 0.0, 1.0, 1.5, strBestScoreMachineGame1, DT_CENTER|DT_VCENTER);
								CString str;
								str.Format(_T("%d"), nBestScoreMachineGame1);
								font_Micro->glDrawText(9.05331, -9.0625, 7.50762, 9.0625, 0.0, 8.53046/*8.28046*/, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
							}
							for(int k = 0; k < 2; k++) if(np[k])
							{
								if(nBestScorePlayerGame1[k] != 0)
								{
									CString str;
									str.Format(_T("%d"), nBestScorePlayerGame1[k]);
									font_Micro->glDrawText(-9.05331, -9.0625+(k?7.25:-7.25), -10.599, 9.0625+(k?7.25:-7.25), (k?7.25:-7.25), -9.57615, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
								}
							}
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						if(nModeTime >= (TIME_SHOWNAME_BESTSCORE_GAME1) && bShow)
						{
							if(nBestScoreMachineGame1 != 0)
							{
								font_Micro->glDrawText(10.599, -9.0625, 9.05331, 9.0625, 0.0, 10.076/*9.82615*/, 0.0, 1.0, 1.5, strBestScoreMachineGame1, DT_CENTER|DT_VCENTER);
								CString str;
								str.Format(_T("%d"), nBestScoreMachineGame1);
								font_Micro->glDrawText(9.05331, -9.0625, 7.50762, 9.0625, 0.0, 8.53046/*8.28046*/, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
							}
							for(int k = 0; k < 2; k++) if(np[k])
							{
								if(nBestScorePlayerGame1[k] != 0)
								{
									CString str;
									str.Format(_T("%d"), nBestScorePlayerGame1[k]);
									font_Micro->glDrawText(-9.05331, -9.0625+(k?7.25:-7.25), -10.599, 9.0625+(k?7.25:-7.25), (k?7.25:-7.25), -9.57615, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
								}
							}
						}
					}
					glPopMatrix();
				}
			}
			if(nTransBorder >= 0 && nTransBorder <= TIME_GLOUTH_SONGBORDER_GAME1 && nChosesGame1 == 0)
			{
				dApt = double(nTransBorder)/TIME_GLOUTH_SONGBORDER_GAME1;
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderBright]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
				glTranslated(0.0, DY_SONGBORDER_HOLD_GAME1, 0.0);
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
			if(nTransBorder > TIME_TRANSI_SONGBORDER_GAME1 && nTransBorder <= (TIME_TRANSI_SONGBORDER_GAME1+TIME_GLOUTH_SONGBORDER_GAME1) && nChosesGame1 == 0)
			{
				dApt = double(nTransBorder-TIME_TRANSI_SONGBORDER_GAME1)/TIME_GLOUTH_SONGBORDER_GAME1;
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderBright]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, dApt);
				glTranslated(0.0, DY_SONGBORDER_HOLD_GAME1, 0.0);
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
	#define TIME_GLOUTH_PER_SONGBORDER_GAME1 1500
			if(nTransBorder > (TIME_TRANSI_SONGBORDER_GAME1+TIME_GLOUTH_SONGBORDER_GAME1) && nChosesGame1 == 0)
			{
				dApt = abs(double((nTransBorder-TIME_TRANSI_SONGBORDER_GAME1-TIME_GLOUTH_SONGBORDER_GAME1)%TIME_GLOUTH_PER_SONGBORDER_GAME1-TIME_GLOUTH_PER_SONGBORDER_GAME1/2))*2/TIME_GLOUTH_PER_SONGBORDER_GAME1;
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderBright]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, dApt);
				glTranslated(0.0, DY_SONGBORDER_HOLD_GAME1, 0.0);
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
			if(nTransBorder > (TIME_TRANSI_SONGBORDER_GAME1+TIME_GLOUTH_SONGBORDER_GAME1) && nChosesGame1 == 1)
			{
				welres->skin[0]->Update(dwWait);
				welres->skin[1]->Update(dwWait);
				welres->skin[3]->Update(dwWait);
				dApt = double((nCountGame1-nTimeChosesGame1[nChosesGame1-1])%TIME_GLOUTH_SONGBORDER_GAME1)/TIME_GLOUTH_SONGBORDER_GAME1;
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderBright]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
				glTranslated(0.0, DY_SONGBORDER_HOLD_GAME1, 0.0);
				glScaled(1.25+0.2*(dApt), 1.25+0.2*(dApt), 1.0);
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
			}
			if(nTransBorder >= TIME_SHOW_PREVIEW_GAME1)
			{
				if (!bShow)
				{
					TCHAR chFile[260];
					_sprintf(chFile, _T("song/%ls/%ls_P.avi"), chSongsGame1[nCurSongGame1], chSongsGame1[nCurSongGame1]);
					if (movie_songpreview)
					{
						if (movie_songpreview->bLoad)
						{
							movie_songpreview->CloseAVI();
						}
						delete movie_songpreview;
						movie_songpreview = NULL;
					}

					if (movie_songpreview == NULL)
					{
						movie_songpreview = new CMovieStream2();
						movie_songpreview->OpenAVI(chFile);
						movie_songpreview->bLoop = true;
					}
					nTimeShow = nCountGame1;
					bShow = true;
					_sprintf(chFile, _T("song/%ls/%ls_P.mp3"), chSongsGame1[nCurSongGame1], chSongsGame1[nCurSongGame1]);
					if(bLoadMusicCoverGame1)
					{
						bLoadMusicCoverGame1 = false;
						FMOD_Sound_Release(music_coverGame1);
					}
					FMOD_RESULT resc;
#ifdef UNICODE
					char chAnsiFile[260];
					UnicodeToAnsi(chFile, chAnsiFile);
					resc = FMOD_System_CreateStream(sys, chAnsiFile, FMOD_HARDWARE | FMOD_2D, 0, &music_coverGame1);
#else
					resc = FMOD_System_CreateStream(sys, chFile, FMOD_HARDWARE | FMOD_2D, 0, &music_coverGame1);
#endif
					if(ERRCHECK(resc))
					{
						bLoadMusicCoverGame1 = true;
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, music_coverGame1, 0, &channels[9]);
					}
					else
					{
						bLoadMusicCoverGame1 = false;
						_printf(_T("No se puede cargar el archivo \"%ls\"\n"), chFile);
					}
					strDispGame1.Format(_T("%ls - %ls - %ls"), GetNameSongsGame1(nCurSongGame1), GetArtistSongsGame1(nCurSongGame1), GetBPMSongsGame1(nCurSongGame1));
					for(int k = 0; k < 2; k++) if(np[k])
					{
						GetLevelSongsGame1(nCurSongGame1, nModeCurSongGame1[k], &nLevelCurGame1[k],
						&dDCurGame1[k][0],
						&dDCurGame1[k][1],
						&dDCurGame1[k][2],
						&dDCurGame1[k][3],
						&dDCurGame1[k][4]);
					}
					GetBestScores(nCurSongGame1, nModeCurSongGame1[np[0]?0:1]);
				}
			}



	#define TIME_START_UPDO_GAME1 0
	#define TIME_TRANSI_UPDO_GAME1 500
	#define TIME_GLOUTH_UPDO_GAME1 500
			if(nCountGame1 >= TIME_START_UPDO_GAME1 && nCountGame1 < (TIME_START_UPDO_GAME1+TIME_TRANSI_UPDO_GAME1))	// Transi Up and Down Game1
			{
				dApt = (double(nCountGame1)-TIME_START_UPDO_GAME1)/(TIME_TRANSI_UPDO_GAME1);
				glPushMatrix();
				glScaled(1.0+0.8*(1.0-dApt), 1.0+0.8*(1.0-dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpAndDownMaskCha1+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpAndDownCha1+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpLeftChoice]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-26.1-6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(-26.1+6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(-26.1+6.4,  18.2-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-26.1-6.4,  18.2-6.4, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpRightChoice]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d( 26.1-6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 26.1+6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 26.1+6.4,  18.2-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( 26.1-6.4,  18.2-6.4, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DownLeftChoice]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-19.5-6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(-19.5+6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(-19.5+6.4, -21.5-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-19.5-6.4, -21.5-6.4, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DownRightChoice]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d( 19.5-6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 19.5+6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 19.5+6.4, -21.5-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( 19.5-6.4, -21.5-6.4, -50.0);
				glEnd();
				/*Dibujo de los modos*/
				for(int k = 0; k < 5; k++)
				{
					bool bBoth = np[0] && np[1] && ((nModeCurSongGame1[0]<5&&nModeCurSongGame1[1]>=5) || (nModeCurSongGame1[0]>=5&&nModeCurSongGame1[1]<5));
					if(bBoth == true) for(int q = 0; q < 2; q++)
					{
						double dx = double(k)*8.4-16.8+0.925*(q?1.0:-1.0);
						int nTex = 0;
						if(nModeCurSongGame1[q]<5)
							nTex = (IsAvalibleMode(nCurSongGame1, k)?GAME1RES_LevelEZ:GAME1RES_LevelEZ_OFF)+k;
						else
							nTex = (IsAvalibleMode(nCurSongGame1, k+5)?GAME1RES_LevelAEZ:GAME1RES_LevelAEZ_OFF)+k;
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[nTex]->GetCurrentTex());
						glBegin(GL_QUADS);
  						glTexCoord2d(0.0, 1.0); glVertex3d( -0.925+dx, 23.1+0.69375, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  0.925+dx, 23.1+0.69375, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  0.925+dx, 23.1-0.69375, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -0.925+dx, 23.1-0.69375, -50.0);
						glEnd();
					}
					else
					{
						double dx = double(k)*8.4-16.8;
						int q = 0;
						for(q = 0; q < 2; q++) if(np[q]) break;
						int nTex = 0;
						if(nModeCurSongGame1[q]<5)
							nTex = (IsAvalibleMode(nCurSongGame1, k)?GAME1RES_LevelEZ:GAME1RES_LevelEZ_OFF)+k;
						else
							nTex = (IsAvalibleMode(nCurSongGame1, k+5)?GAME1RES_LevelAEZ:GAME1RES_LevelAEZ_OFF)+k;
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[nTex]->GetCurrentTex());
						glBegin(GL_QUADS);
  						glTexCoord2d(0.0, 1.0); glVertex3d( -1.85+dx, 23.1+1.3875, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.85+dx, 23.1+1.3875, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.85+dx, 23.1-1.3875, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.85+dx, 23.1-1.3875, -50.0);
						glEnd();
					}
				}
				/*Dibujo de la elección*/
				for(int k = 0; k < 2; k++) if(np[k])
				{
					double dx = double(nModeCurSongGame1[k]%5)*8.4-16.8;
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SelectorModeP1+k]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d( -2.321568+dx, 23.1+2.321568, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  2.321568+dx, 23.1+2.321568, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  2.321568+dx, 23.1-2.321568, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -2.321568+dx, 23.1-2.321568, -50.0);
					glEnd();
					CString str;
					switch(nModeCurSongGame1[k])
					{
					case 0: str = _T("NORMAL"); break;
					case 1: str = _T("HARD"); break;
					case 2: str = _T("CRAZY"); break;
					case 3: str = _T("FREESTYLE"); break;
					case 4: str = _T("NIGHTMARE"); break;
					case 5: str = _T("A-NORMAL"); break;
					case 6: str = _T("A-HARD"); break;
					case 7: str = _T("A-CRAZY"); break;
					case 8: str = _T("A-FREESTYLE"); break;
					case 9: str = _T("A-NIGHTMARE"); break;
					default: str = _T("Unknown"); break;
					}
					font_Car->glDrawText(40.0, -30.0, -40.0, 30.0, k?10.3:-10.3, 28.2, -50.0, 1.0, 1.8, str, DT_CENTER|DT_VCENTER);
				}
				glPopMatrix();
			}
			if(nCountGame1 >= (TIME_START_UPDO_GAME1+TIME_TRANSI_UPDO_GAME1))	// Up and Down Game1
			{
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpAndDownMaskCha1+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				if(nTransBorder > TIME_SHOW_PREVIEW_GAME1 && bShow)
				{
	#define SIZE_TEXT_GAME1 2
	#define VEL_SHOWTEXT_GAME1 0.01
	#define TIME_WAITTEXTNAME_GAME1 1500
					__int64 nTimeName = nTransBorder-TIME_SHOW_PREVIEW_GAME1-TIME_WAITTEXTNAME_GAME1;
					if(nTimeName < 0) nTimeName = 0;
					double dSizeTex = font_Car->CalcSingleLine(strDispGame1, SIZE_TEXT_GAME1);
					DWORD t1 = DWORD(double(48.0/VEL_SHOWTEXT_GAME1));
					DWORD t2 = DWORD(double(dSizeTex/VEL_SHOWTEXT_GAME1));
					DWORD t = t1+t2;
					DWORD nShowText = (DWORD(nTimeName)+t1)%t;
					double dx = -VEL_SHOWTEXT_GAME1*double(nShowText)+39.0;
					font_Car->glDrawText(-24.1, -17.9, -27.6, 17.9, -8.375+dx, -25.9, -50.0, 1.0, SIZE_TEXT_GAME1, strDispGame1, DT_VCENTER);
				}
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpAndDownCha1+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();

				DWORD dwNext = 0;
				DWORD dwFrame = 0;

				dwNext = (DWORD)(nCountGame1-max(nTimeEnArrowsGame1[1],nTimeEnArrowsGame1[6]));
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame1)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga1res->skin[GAME1RES_UpLeftChoice]->m_nSize)/(DWORD)nTimeTransiSongBorderGame1;
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpLeftChoice]->texture[dwFrame]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-26.1-6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(-26.1+6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(-26.1+6.4,  18.2-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-26.1-6.4,  18.2-6.4, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame1-max(nTimeEnArrowsGame1[3],nTimeEnArrowsGame1[8]));
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame1)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga1res->skin[GAME1RES_UpRightChoice]->m_nSize)/(DWORD)nTimeTransiSongBorderGame1;
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpRightChoice]->texture[dwFrame]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d( 26.1-6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 26.1+6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 26.1+6.4,  18.2-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( 26.1-6.4,  18.2-6.4, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame1-max(nTimeEnArrowsGame1[0],nTimeEnArrowsGame1[5]));
				if(bEnArrowsGame1[0] || bEnArrowsGame1[5]) dwNext = dwNext%nTimeTransiSongBorderGame1;
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame1)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga1res->skin[GAME1RES_DownLeftChoice]->m_nSize)/(DWORD)nTimeTransiSongBorderGame1;
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DownLeftChoice]->texture[dwFrame]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-19.5-6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(-19.5+6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(-19.5+6.4, -21.5-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-19.5-6.4, -21.5-6.4, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame1-max(nTimeEnArrowsGame1[4],nTimeEnArrowsGame1[9]));
				if(bEnArrowsGame1[4] || bEnArrowsGame1[9]) dwNext = dwNext%nTimeTransiSongBorderGame1;
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame1)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga1res->skin[GAME1RES_DownRightChoice]->m_nSize)/(DWORD)nTimeTransiSongBorderGame1;
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DownRightChoice]->texture[dwFrame]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d( 19.5-6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 19.5+6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 19.5+6.4, -21.5-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( 19.5-6.4, -21.5-6.4, -50.0);
				glEnd();

				/*Dibujo de los modos*/
				for(int k = 0; k < 5; k++)
				{
					bool bBoth = np[0] && np[1] && ((nModeCurSongGame1[0]<5&&nModeCurSongGame1[1]>=5) || (nModeCurSongGame1[0]>=5&&nModeCurSongGame1[1]<5));
					if(bBoth == true) for(int q = 0; q < 2; q++)
					{
						double dx = double(k)*8.4-16.8+0.925*(q?1.0:-1.0);
						int nTex = 0;
						if(nModeCurSongGame1[q]<5)
							nTex = (IsAvalibleMode(nCurSongGame1, k)?GAME1RES_LevelEZ:GAME1RES_LevelEZ_OFF)+k;
						else
							nTex = (IsAvalibleMode(nCurSongGame1, k+5)?GAME1RES_LevelAEZ:GAME1RES_LevelAEZ_OFF)+k;
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[nTex]->GetCurrentTex());
						glBegin(GL_QUADS);
  						glTexCoord2d(0.0, 1.0); glVertex3d( -0.925+dx, 23.1+0.69375, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  0.925+dx, 23.1+0.69375, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  0.925+dx, 23.1-0.69375, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -0.925+dx, 23.1-0.69375, -50.0);
						glEnd();
					}
					else
					{
						double dx = double(k)*8.4-16.8;
						int q = 0;
						for(q = 0; q < 2; q++) if(np[q]) break;
						int nTex = 0;
						if(nModeCurSongGame1[q]<5)
							nTex = (IsAvalibleMode(nCurSongGame1, k)?GAME1RES_LevelEZ:GAME1RES_LevelEZ_OFF)+k;
						else
							nTex = (IsAvalibleMode(nCurSongGame1, k+5)?GAME1RES_LevelAEZ:GAME1RES_LevelAEZ_OFF)+k;
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[nTex]->GetCurrentTex());
						glBegin(GL_QUADS);
  						glTexCoord2d(0.0, 1.0); glVertex3d( -1.85+dx, 23.1+1.3875, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.85+dx, 23.1+1.3875, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.85+dx, 23.1-1.3875, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.85+dx, 23.1-1.3875, -50.0);
						glEnd();
					}
				}

	#define TIME_TRANSI_SELECTORMODE_GAME1 300
	#define TIME_START_GRAPH_GAME1 100
	#define TIME_TRANSI_GRAPH_GAME1 500
	#define TIME_GLOUTH_GRAPH_GAME1 300
	#define TIME_START_LINEGRAPH_GAME1 100
	#define TIME_TRANSI_LINEGRAPH_GAME1 500
	#define TIME_TRUNC_LINEGRAPH_GAME1 1200
				/*Dibujo de la elección*/
				for(int k = 0; k < 2; k++) if(np[k])
				{
					double dx = double(nModeCurSongGame1[k]%5)*8.4-16.8;
					double dy = abs(double((nCountGame1-(TIME_START_UPDO_GAME1+TIME_TRANSI_UPDO_GAME1)+(k?250:0))%500)/500-0.5);
					__int64 nTransSelectM = nCountGame1 - nTimeModeCurSongGame1[k];
					if(nTransSelectM >= 0 && nTransSelectM < TIME_TRANSI_SELECTORMODE_GAME1)
					{
						dx+=(double(nBackModeCurSongGame1[k]%5)-double(nModeCurSongGame1[k]%5))*3.725*(1.0-pow(double(nTransSelectM)/TIME_TRANSI_SELECTORMODE_GAME1,0.5));
					}
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SelectorModeP1+k]->GetCurrentTex());
					glColor4d(1.0, 1.0, 1.0, dy+0.5);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d( -2.321568+dx, 23.1+2.321568, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  2.321568+dx, 23.1+2.321568, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  2.321568+dx, 23.1-2.321568, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -2.321568+dx, 23.1-2.321568, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);

					CString str;
					switch(nModeCurSongGame1[k])
					{
					case 0: str = _T("NORMAL"); break;
					case 1: str = _T("HARD"); break;
					case 2: str = _T("CRAZY"); break;
					case 3: str = _T("FREESTYLE"); break;
					case 4: str = _T("NIGHTMARE"); break;
					case 5: str = _T("A-NORMAL"); break;
					case 6: str = _T("A-HARD"); break;
					case 7: str = _T("A-CRAZY"); break;
					case 8: str = _T("A-FREESTYLE"); break;
					case 9: str = _T("A-NIGHTMARE"); break;
					default: str = _T("Unknown"); break;
					}
					font_Car->glDrawText(40.0, -30.0, -40.0, 30.0, k?10.3:-10.3, 28.2, -50.0, 1.0, 1.8, str, DT_CENTER|DT_VCENTER);

#define TIME_START_LR_GAME1 0
#define TIME_TRANSI_LR_GAME1 500
#define TIME_GLOUTH_LR_GAME1 500
					__int64 nCountSide = nCountGame1-(TIME_START_UPDO_GAME1+TIME_TRANSI_UPDO_GAME1);
					if(nCountSide >= TIME_START_LR_GAME1 && nCountSide <= (TIME_START_LR_GAME1+TIME_TRANSI_LR_GAME1))
					{
						dApt = pow(double(nCountSide-TIME_START_LR_GAME1)/TIME_TRANSI_LR_GAME1, 0.5);
						glPushMatrix();
						glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(1-dApt), 0.0, 0.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[(k?GAME1RES_RightCha1:GAME1RES_LeftCha1)+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
#define TIME_TRANSI_PRESET_GAME1 500
#define TIME_GLOUTH_PRESET_GAME1 500
#define TIME_SHOW_PRESET_GAME1 1500
					if(nCountSide > (TIME_START_LR_GAME1+TIME_TRANSI_LR_GAME1))
					{
						glPushMatrix();
						glTranslated(k?40.0:-40.0, 0.0, 0.0);
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[(k?GAME1RES_RightCha1:GAME1RES_LeftCha1)+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
						glEnd();
						glPopMatrix();
						__int64 nCountPreset = nCountGame1-nTimeLastPresetGame1[k];
						for(int q = 0; q < 6; q++)
						{
							double dAdv = 0.0;
							for(int w = 0; w < nIndSizePresetGame1[k][q]; w++)
							{
								int nInd = (q*5+w);
								if(nInd == nIndLastPresetGame1[k] || nIndLastPresetGame1[k] == -1)
								{
									__int64 nCountPreset = nCountGame1-nTimeLastPresetGame1[k];
									if(nCountPreset >= 0 && nCountPreset < TIME_TRANSI_PRESET_GAME1)
									{
										glPushMatrix();
										dApt = double(nCountPreset)/TIME_TRANSI_PRESET_GAME1;
										glLoadIdentity();
										glColor4d(1.0, 1.0, 1.0, dApt);
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.5+2.0*(1-dApt), 1.5+2.0*(1-dApt), 1.0);
										glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetX2-1+nIndPresetGame1[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
										glScaled(1.0+2.0*(1-dApt), 1.0, 1.0);
										glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetX2-1+nIndPresetGame1[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
										glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetCommon]->GetCurrentTex());
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
										glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetGlouth2]->GetCurrentTex());
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
									if(nCountPreset >= TIME_TRANSI_PRESET_GAME1 && nCountPreset < (TIME_TRANSI_PRESET_GAME1+TIME_SHOW_PRESET_GAME1))
									{
										glPushMatrix();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.5, 1.5, 1.0);
										glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetX2-1+nIndPresetGame1[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetX2-1+nIndPresetGame1[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetCommon]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										if(nCountPreset < (TIME_TRANSI_PRESET_GAME1+TIME_GLOUTH_PRESET_GAME1))
										{
											dApt = double(nCountPreset - TIME_TRANSI_PRESET_GAME1)/TIME_GLOUTH_PRESET_GAME1;
											glLoadIdentity();
											glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
											glScaled(dApt*4.0, dApt*4.0, 1.0);
											glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
											glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetGlouth1]->GetCurrentTex());
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
									if(nCountPreset >= (TIME_TRANSI_PRESET_GAME1+TIME_SHOW_PRESET_GAME1))
									{
										glPushMatrix();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.5, 1.5, 1.0);
										glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetX2-1+nIndPresetGame1[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME1+TIME_SHOW_PRESET_GAME1))
										{
											dApt = double(nCountPreset-(TIME_TRANSI_PRESET_GAME1+TIME_SHOW_PRESET_GAME1))/TIME_TRANSI_PRESET_GAME1;
											glLoadIdentity();
											glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
											glScaled(1.0+2.0*(dApt), 1.0-dApt, 1.0);
											glColor4d(1.0, 1.0, 1.0, 1-dApt);
											glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetX2-1+nIndPresetGame1[k][nInd]]->GetCurrentTex());
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
										glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetCommon]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glPopMatrix();
										if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME1+TIME_SHOW_PRESET_GAME1)) dAdv += (1-dApt)*9.0;
										dAdv += 4.5;
									}
								}
								else
								{
									glPushMatrix();
									glLoadIdentity();
									glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
									glScaled(1.5, 1.5, 1.0);
									glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetX2-1+nIndPresetGame1[k][nInd]]->GetCurrentTex());
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
									glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
									glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
									glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
									glEnd();
									glLoadIdentity();
									glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetCommon]->GetCurrentTex());
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

					/*Dibujo del Stage*/

#define TIME_TRANSI_LIFEALTER_GAME1 500
					int nMinus = nStageSongsGame1[nCurSongGame1];
					__int64 nTimeLife = nCountGame1;
					int nLife = np[k];
					int nLifeG = np[k]-nMinus;
					if(nLifeG < 0)
					{
						nLife = 0;
						nLifeG = 0;
					}
					glPushMatrix();
					glTranslated(k?30.0:-30.0, k?-28.0:28.0, 0.0);
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_Life0+nLifeG]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-4.1666,  0.5, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 4.1666,  0.5, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 4.1666, -0.5, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-4.1666, -0.5, -50.0);
					glEnd();
					dApt = abs(double((nCountGame1)%TIME_TRANSI_LIFEALTER_GAME1)/TIME_TRANSI_LIFEALTER_GAME1-0.5)*2;
					glColor4d(1.0, 1.0, 1.0, dApt);
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_Life0+nLife]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-4.1666,  0.5, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 4.1666,  0.5, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 4.1666, -0.5, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-4.1666, -0.5, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();

					/*Dibujo del Graph*/

					if(nTransSelectM >= TIME_START_GRAPH_GAME1 && nTransSelectM < (TIME_START_GRAPH_GAME1+TIME_TRANSI_GRAPH_GAME1))
					{
						dApt = pow(double(nTransSelectM - TIME_START_GRAPH_GAME1)/TIME_TRANSI_GRAPH_GAME1, 0.5);
						glPushMatrix();
						glTranslated((k?30.0:-30.0)+(k?1:-1)*17.5*(1-dApt), k?-20.0:20.0, 0.0);
						glScaled(0.5+0.5*dApt, 0.5+0.5*dApt, 1.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						for(int l = 0; l < 6; l++)
						{
							double da = 0.0, de = 0.0;
							if(l == 0) da = dDCurGame1[k][0];
							else da = dDCurGame1[k][l-1];
							if(l == 5) de = dDCurGame1[k][4];
							else de = dDCurGame1[k][l];
							double dx = 0.46875+2.34375*double(l)-7.5;
							double dy = -3.75;
							double dy1 = 9.375*da-3.75;
							double dy2 = 9.375*de-3.75;
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_GraphLine]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(double(l)/6, 1.0-da); glVertex3d( dx,  dy1, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0-de); glVertex3d(  dx+2.34375,  dy2, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0); glVertex3d(  dx+2.34375, dy, -50.0);
							glTexCoord2d(double(l)/6, 1.0); glVertex3d( dx, dy, -50.0);
							glEnd();
						}
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_GraphBase]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  7.5, -7.5, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -7.5, -7.5, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nTransSelectM >= (TIME_START_GRAPH_GAME1+TIME_TRANSI_GRAPH_GAME1))
					{
						glPushMatrix();
						glTranslated(k?30.0:-30.0, k?-20.0:20.0, 0.0);
						for(int l = 0; l < 6; l++)
						{
							double da = 0.0, de = 0.0;
							if(l == 0) da = dDCurGame1[k][0];
							else da = dDCurGame1[k][l-1];
							if(l == 5) de = dDCurGame1[k][4];
							else de = dDCurGame1[k][l];
							double dx = 0.46875+2.34375*double(l)-7.5;
							double dy = -3.75;
							double dy1 = 9.375*da-3.75;
							double dy2 = 9.375*de-3.75;
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_GraphLine]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(double(l)/6, 1.0-da); glVertex3d( dx,  dy1, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0-de); glVertex3d(  dx+2.34375,  dy2, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0); glVertex3d(  dx+2.34375, dy, -50.0);
							glTexCoord2d(double(l)/6, 1.0); glVertex3d( dx, dy, -50.0);
							glEnd();
						}
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_GraphBase]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  7.5, -7.5, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -7.5, -7.5, -50.0);
						glEnd();
						__int64 nTransGraphLine = (nTransSelectM - (TIME_START_GRAPH_GAME1+TIME_TRANSI_GRAPH_GAME1))%TIME_TRUNC_LINEGRAPH_GAME1;
						if(nTransGraphLine >= TIME_START_LINEGRAPH_GAME1 && nTransGraphLine < (TIME_START_LINEGRAPH_GAME1+TIME_TRANSI_LINEGRAPH_GAME1))
						{
							double t = double(nTransGraphLine)-TIME_START_LINEGRAPH_GAME1;
							double dx=0.46875+14.0625*t/TIME_TRANSI_LINEGRAPH_GAME1-7.5;
							double dy=0.0;
							double da=0.0;
							if(t<(TIME_TRANSI_LINEGRAPH_GAME1/6))
							{
								da=t*6/TIME_TRANSI_LINEGRAPH_GAME1;
								dy=9.375*dDCurGame1[k][0]-3.75;
							}
							if(t>=(TIME_TRANSI_LINEGRAPH_GAME1/6) && t < (5*TIME_TRANSI_LINEGRAPH_GAME1/6))
							{
								da = 1.0;
								int n1 = int((t-TIME_TRANSI_LINEGRAPH_GAME1/6)*6/TIME_TRANSI_LINEGRAPH_GAME1);
								int n2 = int((t-TIME_TRANSI_LINEGRAPH_GAME1/6)*6/TIME_TRANSI_LINEGRAPH_GAME1)+1;
								double tl = double(n2)*TIME_TRANSI_LINEGRAPH_GAME1/6;
								dy = 9.375*((t-tl)*6/TIME_TRANSI_LINEGRAPH_GAME1*(dDCurGame1[k][n2]-dDCurGame1[k][n1])+dDCurGame1[k][n1])-3.75;
							}
							if(t>(5*TIME_TRANSI_LINEGRAPH_GAME1/6))
							{
								da=1.0-(t-5*TIME_TRANSI_LINEGRAPH_GAME1/6)*6/TIME_TRANSI_LINEGRAPH_GAME1;
								dy=9.375*dDCurGame1[k][4]-3.75;
							}
							glTranslated(dx, dy, 0.0);
							glColor4d(1.0, 1.0, 1.0, da);
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_GraphStar]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -1.0,  1.0, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  1.0,  1.0, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  1.0, -1.0, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -1.0, -1.0, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						glPopMatrix();
						if(nTransSelectM < (TIME_START_GRAPH_GAME1+TIME_TRANSI_GRAPH_GAME1+TIME_GLOUTH_GRAPH_GAME1))
						{
							dApt = pow(double(nTransSelectM-TIME_START_GRAPH_GAME1-TIME_TRANSI_GRAPH_GAME1)/TIME_GLOUTH_GRAPH_GAME1, 0.5);
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_GraphBright]->GetCurrentTex());
							glPushMatrix();
							glTranslated(k?30.0:-30.0, k?-20.0:20.0, 0.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glScaled(1.0+0.1*dApt, 1.0+0.01*dApt, 1.0);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -7.5,  7.5, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  7.5,  7.5, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  7.5, -7.5, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -7.5, -7.5, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
					}

	#define TIME_START_DIFI_GAME1 600
	#define TIME_TRANSI_DIFI_GAME1 500
	#define TIME_GLOUTH_DIFI_GAME1 500
	#define TIME_CONTROT_DIFI_GAME1 1000
					/*Dibujo del nivel*/
					int nFilterLevel = 0;
					int nItemsActive = 0;
					int nIndexActive = 0;
					if(nLevelCurGame1[k] <= 0 || nLevelCurGame1[k] > ga1res->skin[GAME1RES_DificultyNumbers]->m_nSize) nFilterLevel = ga1res->skin[GAME1RES_DificultyNumbers]->m_nSize;
					else nFilterLevel = nLevelCurGame1[k];
					if(nFilterLevel >= 1 && nFilterLevel <= 30)
					{
						nItemsActive = (nFilterLevel-1)%8+1;
						nIndexActive = (nFilterLevel-1)/8;
					}
					if(player_sesion[k]->nLevel <= (nLevelCurGame1[k]-3) && !player_sesion[k]->nLockedAsp[ITEM_SUPRIMELVL]) nIndexActive = 4;
					double dAngleDif = double(nTransSelectM%TIME_CONTROT_DIFI_GAME1)/TIME_CONTROT_DIFI_GAME1*360;
					if(nTransSelectM >= TIME_START_DIFI_GAME1 && nTransSelectM < (TIME_START_DIFI_GAME1+TIME_TRANSI_DIFI_GAME1))
					{
						dApt = double(nTransSelectM - TIME_START_DIFI_GAME1)/TIME_TRANSI_DIFI_GAME1;
						glPushMatrix();
						if(np[k] && !np[int(!k)])
						{
							double dWidth = 18.75;
							double dSizeItem = dWidth/8;
							double dStart = -dSizeItem*4+dSizeItem/2;
							glLoadIdentity();
							glTranslated(0.0,  18.7, 0.0);
							glColor4d(1.0, 1.0, 1.0, dApt);
							if(nIndexActive < 3)
							{
								for(int q = 0; q < 8; q++)
								{
									double dx = dSizeItem*double(q)+dStart;
									glLoadIdentity();
									glTranslated(dx,  18.7, 0.0);
									glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
									glBindTexture(GL_TEXTURE_2D, ga1res->skin[(q>=nItemsActive?GAME1RES_EggDificultyOFF:GAME1RES_EggDificulty)+nIndexActive]->GetCurrentTex());
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d( -dSizeItem/2,  dSizeItem/2, -50.0);
									glTexCoord2d(1.0, 1.0); glVertex3d(  dSizeItem/2,  dSizeItem/2, -50.0);
									glTexCoord2d(1.0, 0.0); glVertex3d(  dSizeItem/2, -dSizeItem/2, -50.0);
									glTexCoord2d(0.0, 0.0); glVertex3d( -dSizeItem/2, -dSizeItem/2, -50.0);
									glEnd();
								}
							}
							if(nIndexActive == 3)
							{
								glLoadIdentity();
								glTranslated(0.0,  18.7, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DificultyRack]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
							if(nIndexActive == 4)
							{
								glLoadIdentity();
								glTranslated(0.0,  18.7, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DificultyUnexpected]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
						}
						else
						{
							double dWidth = 9.0;
							double dSizeItem = dWidth/8;
							double dStart = -dSizeItem*4+dSizeItem/2+(k?4.75:-4.75);
							glLoadIdentity();
							glTranslated(k?4.75:-4.75,  18.7, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetBar]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -4.75,  0.59375, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  4.75,  0.59375, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  4.75, -0.59375, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -4.75, -0.59375, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, dApt);
							if(nIndexActive < 3)
							{
								for(int q = 0; q < 8; q++)
								{
									double dx = dSizeItem*double(q)+dStart;
									glLoadIdentity();
									glTranslated(dx,  18.7, 0.0);
									glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
									glBindTexture(GL_TEXTURE_2D, ga1res->skin[(q>=nItemsActive?GAME1RES_EggDificultyOFF:GAME1RES_EggDificulty)+nIndexActive]->GetCurrentTex());
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d( -dSizeItem/2,  dSizeItem/2, -50.0);
									glTexCoord2d(1.0, 1.0); glVertex3d(  dSizeItem/2,  dSizeItem/2, -50.0);
									glTexCoord2d(1.0, 0.0); glVertex3d(  dSizeItem/2, -dSizeItem/2, -50.0);
									glTexCoord2d(0.0, 0.0); glVertex3d( -dSizeItem/2, -dSizeItem/2, -50.0);
									glEnd();
								}
							}
							if(nIndexActive == 3)
							{
								glLoadIdentity();
								glTranslated(k?4.75:-4.75,  18.7, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DificultyRack]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
							if(nIndexActive == 4)
							{
								glLoadIdentity();
								glTranslated(k?4.75:-4.75,  18.7, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DificultyUnexpected]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
						}
						glLoadIdentity();
						glTranslated(k?10.875:-10.875,  18.7, 0.0);
						glScaled(1.5+2.0*(1-dApt), 1.5+2.0*(1-dApt), 1.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DificultyNumbers]->texture[nFilterLevel-1]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  0.875, -0.875, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -0.875, -0.875, -50.0);
						glEnd();
						glLoadIdentity();
						glTranslated(k?10.875:-10.875,  18.7, 0.0);
						glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
						glRotated(dAngleDif, 0.0, 0.0, 1.0);
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetCommon]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						glLoadIdentity();
						glTranslated(k?10.875:-10.875,  18.7, 0.0);
						glScaled((1-dApt)*4.0, (1-dApt)*4.0, 1.0);
						glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetGlouth2]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nTransSelectM >= (TIME_START_DIFI_GAME1+TIME_TRANSI_DIFI_GAME1))
					{
						glPushMatrix();
						if(np[k] && !np[int(!k)])
						{
							double dWidth = 18.75;
							double dSizeItem = dWidth/8;
							double dStart = -dSizeItem*4+dSizeItem/2;
							glLoadIdentity();
							glTranslated(0.0,  18.7, 0.0);
							/*glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetBar]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -9.75,  1.21875, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  9.75,  1.21875, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  9.75, -1.21875, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -9.75, -1.21875, -50.0);
							glEnd();*/
							if(nIndexActive < 3)
							{
								for(int q = 0; q < 8; q++)
								{
									double dx = dSizeItem*double(q)+dStart;
									glLoadIdentity();
									glTranslated(dx,  18.7, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga1res->skin[(q>=nItemsActive?GAME1RES_EggDificultyOFF:GAME1RES_EggDificulty)+nIndexActive]->GetCurrentTex());
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d( -dSizeItem/2,  dSizeItem/2, -50.0);
									glTexCoord2d(1.0, 1.0); glVertex3d(  dSizeItem/2,  dSizeItem/2, -50.0);
									glTexCoord2d(1.0, 0.0); glVertex3d(  dSizeItem/2, -dSizeItem/2, -50.0);
									glTexCoord2d(0.0, 0.0); glVertex3d( -dSizeItem/2, -dSizeItem/2, -50.0);
									glEnd();
								}
							}
							if(nIndexActive == 3)
							{
								glLoadIdentity();
								glTranslated(0.0,  18.7, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DificultyRack]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
							if(nIndexActive == 4)
							{
								glLoadIdentity();
								glTranslated(0.0,  18.7, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DificultyUnexpected]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
						}
						else
						{
							double dWidth = 9.0;
							double dSizeItem = dWidth/8;
							double dStart = -dSizeItem*4+dSizeItem/2+(k?4.75:-4.75);
							glLoadIdentity();
							glTranslated(k?4.75:-4.75,  18.7, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetBar]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -4.75,  0.59375, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  4.75,  0.59375, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  4.75, -0.59375, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -4.75, -0.59375, -50.0);
							glEnd();
							if(nIndexActive < 3)
							{
								for(int q = 0; q < 8; q++)
								{
									double dx = dSizeItem*double(q)+dStart;
									glLoadIdentity();
									glTranslated(dx,  18.7, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga1res->skin[(q>=nItemsActive?GAME1RES_EggDificultyOFF:GAME1RES_EggDificulty)+nIndexActive]->GetCurrentTex());
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d( -dSizeItem/2,  dSizeItem/2, -50.0);
									glTexCoord2d(1.0, 1.0); glVertex3d(  dSizeItem/2,  dSizeItem/2, -50.0);
									glTexCoord2d(1.0, 0.0); glVertex3d(  dSizeItem/2, -dSizeItem/2, -50.0);
									glTexCoord2d(0.0, 0.0); glVertex3d( -dSizeItem/2, -dSizeItem/2, -50.0);
									glEnd();
								}
							}
							if(nIndexActive == 3)
							{
								glLoadIdentity();
								glTranslated(k?4.75:-4.75,  18.7, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DificultyRack]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
							if(nIndexActive == 4)
							{
								glLoadIdentity();
								glTranslated(k?4.75:-4.75,  18.7, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DificultyUnexpected]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
						}
						glLoadIdentity();
						glTranslated(k?10.875:-10.875,  18.7, 0.0);
						glScaled(1.5, 1.5, 1.0);
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DificultyNumbers]->texture[nFilterLevel-1]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  0.875, -0.875, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -0.875, -0.875, -50.0);
						glEnd();
						glLoadIdentity();
						glTranslated(k?10.875:-10.875,  18.7, 0.0);
						glRotated(dAngleDif, 0.0, 0.0, 1.0);
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetCommon]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						if(nTransSelectM < (TIME_START_DIFI_GAME1+TIME_TRANSI_DIFI_GAME1+TIME_GLOUTH_DIFI_GAME1))
						{
							dApt = double(nTransSelectM - TIME_START_DIFI_GAME1 -TIME_TRANSI_DIFI_GAME1)/TIME_GLOUTH_DIFI_GAME1;
							glLoadIdentity();
							glTranslated(k?10.875:-10.875,  18.7, 0.0);
							glScaled(dApt*4.0, dApt*4.0, 1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_PresetGlouth1]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						glPopMatrix();
					}
					glPushMatrix();
					glTranslated(k?20.0:-20.0, 0.0, 0.0);
					if(bSkinGame1[k]) DrawSkinsGame1(k);
					if(bCharacterGame1[k]) DrawCharactersGame1(k);
					glPopMatrix();
				}

				__int64 nTrans1 = nCountGame1 - nTimeChaGame1;
				if(nTrans1 <= TIME_GLOUTH_UPDO_GAME1)
				{
					dApt = (double(nTrans1))/(TIME_GLOUTH_UPDO_GAME1);
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpAndDownBrightCha1+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
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
				strDispTime.Format(_T("%.2d"), piuconfig.nModeMach==1?99:int((nMaxGame1 - nCountGame1)/1000));
				font_Car->glDrawText(30.0, -40.0, -30.0, 40.0, 0, 15.5, -50.0, 1.0, 2, strDispTime, DT_VCENTER|DT_CENTER);
			}
#define TIME_TRANSI_EXMOD_GAME1 300
			if(nExModSongsGame1[nCurSongGame1])
			{
				if(nTransBorder >= 0 && nTransBorder < TIME_TRANSI_EXMOD_GAME1)
				{
					dApt = double(nTransBorder)/TIME_TRANSI_EXMOD_GAME1;
					for(int u = 0; u < 2; u++)
					{
						glPushMatrix();
						glTranslated((u?23.0:-23.0)+(u?5.0:-5.0)*(1-dApt), 23.0, 0.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_ExModLeft+u]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-2.25, 2.25, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 2.25, 2.25, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 2.25, -2.25, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-2.25, -2.25, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
				}
				if(nTransBorder >= TIME_TRANSI_EXMOD_GAME1)
				{
					dApt = sin(M_PI*double(nTransBorder-TIME_TRANSI_EXMOD_GAME1)/1000);
					for(int u = 0; u < 2; u++)
					{
						glPushMatrix();
						glTranslated((u?23.0:-23.0)+(u?1.0:-1.0)*dApt, 23.0, 0.0);
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_ExModLeft+u]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-2.25, 2.25, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 2.25, 2.25, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 2.25, -2.25, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-2.25, -2.25, -50.0);
						glEnd();
						glPopMatrix();
					}
				}
			}
			if(nCountGame1 >= TIME_START_START_GAME1)	// Reconocimiento de comandos
			{
				bool bNewCommand = false;
				for(int x = 0; x < 2; x++)
				{
					if(np[x] > 0)
					{
						if(x == 0)
						{
							for(int y = 0; y < 5; y++)
							{
								if(bArrows[y] && !bEnArrowsGame1[y])
								{
									bEnArrowsGame1[y] = true;
									nTimeEnArrowsGame1[y] = nCountGame1;
									for(int q = (MAX_COMMANDS_GAME1-2); q >= 0; q--)
									{
										nComandGame1[x][q+1] = nComandGame1[x][q];
									}
									if(!bSkinGame1[x] && !bCharacterGame1[x])
									{
										nComandGame1[x][0] = y;
										bNewCommand = true;
									}
								}
								if(!bArrows[y] && bEnArrowsGame1[y])
								{
									bEnArrowsGame1[y] = false;
									bCh[y] = false;
									if(y == 0 || y == 4)
									{
										nTimeTransiSongBorderGame1 = TIME_TRANSI_SONGBORDER_GAME1;
									}
								}
							}
						}
						if(x == 1)
						{
							for(int y = 5; y < 10; y++)
							{
								if(bArrows[y] && !bEnArrowsGame1[y])
								{
									bEnArrowsGame1[y] = true;
									nTimeEnArrowsGame1[y] = nCountGame1;
									for(int q = (MAX_COMMANDS_GAME1-2); q >= 0; q--)
									{
										nComandGame1[x][q+1] = nComandGame1[x][q];
									}
									if(!bSkinGame1[x] && !bCharacterGame1[x])
									{
										nComandGame1[x][0] = y-5;
										bNewCommand = true;
									}
								}
								if(!bArrows[y] && bEnArrowsGame1[y])
								{
									bEnArrowsGame1[y] = false;
									bCh[y] = false;
									if(y == 5 || y == 9)
									{
										nTimeTransiSongBorderGame1 = TIME_TRANSI_SONGBORDER_GAME1;
									}
								}
							}
						}
					}
					else
					{
						if(x == 0 && bArrows[2] && nCredit)
						{
							nCurStage = 1;
							PlusStagePlayer(0);
							nCredit--;
							nChangeRetroGame1 = 2;
							nTimeChangeRetroGame1 = nCountGame1;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
							for(int y = 0; y < 10; y++)
							{
								bArrows[y] = false;
								bEnArrowsGame1[y] = false;
								bCh[y] = false;
							}
							break;
						}
						if(x == 1 && bArrows[7] && nCredit)
						{
							nCurStage = 1;
							PlusStagePlayer(1);
							nCredit--;
							nChangeRetroGame1 = 2;
							nTimeChangeRetroGame1 = nCountGame1;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
							for(int y = 0; y < 10; y++)
							{
								bArrows[y] = false;
								bEnArrowsGame1[y] = false;
								bCh[y] = false;
							}
							break;
						}
					}
				}
				if(bEnArrowsGame1[0])
				{
					if(bEnArrowsGame1[1] && !bCh[0])
					{
						MoveCha(true);
						bCh[0] = true;
					}
					else if(bSkinGame1[0] && !bCh[0])
					{
						TurnSkinsGame1(true, 0);
						bCh[0] = true;
					}
					else if(bCharacterGame1[0] && !bCh[0])
					{
						TurnCharactersGame1(true, 0);
						bCh[0] = true;
					}
					else
					{
						__int64 nCaret = nCountGame1 - nTimeEnArrowsGame1[0];
						int nLimit = min(nCaret,nCountGame1-nTimeChaGame1)>TIME_FAST_CHANGE_GAME1?TIME_CHANGE_HIGH_GAME1:TIME_CHANGE_LOW_GAME1;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[0])
						{
							bCh[0] = true;
							MoveSongs(true);
							PlaySoundTransiAboutLimit(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[0] = false;
						}
					}
				}
				if(bEnArrowsGame1[4])
				{
					if(bEnArrowsGame1[3] && !bCh[4])
					{
						MoveCha(false);
						bCh[4] = true;
					}
					else if(bSkinGame1[0] && !bCh[4])
					{
						TurnSkinsGame1(false, 0);
						bCh[4] = true;
					}
					else if(bCharacterGame1[0] && !bCh[4])
					{
						TurnCharactersGame1(false, 0);
						bCh[4] = true;
					}
					else
					{
						__int64 nCaret = nCountGame1 - nTimeEnArrowsGame1[4];
						int nLimit = min(nCaret,nCountGame1-nTimeChaGame1)>TIME_FAST_CHANGE_GAME1?TIME_CHANGE_HIGH_GAME1:TIME_CHANGE_LOW_GAME1;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[4])
						{
							bCh[4] = true;
							MoveSongs(false);
							PlaySoundTransiAboutLimit(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[4] = false;
						}
					}
				}
				if(bEnArrowsGame1[1] && !bCh[1])
				{
					if(!bSkinGame1[0] && !bCharacterGame1[0])
					{
						bCh[1] = true;
						MoveMode(0, true);
					}
				}
				if(bEnArrowsGame1[3] && !bCh[3])
				{
					if(!bSkinGame1[0] && !bCharacterGame1[0])
					{
						bCh[3] = true;
						MoveMode(0, false);
					}
				}
				if(bEnArrowsGame1[2] && !bCh[2])
				{
					bCh[2] = true;
					if(np[0])
					{
						if(bSkinGame1[0])
						{
							MakeChoiceSkinGame1(0);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else if(bCharacterGame1[0])
						{
							MakeChoiceCharacterGame1(0);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else
						{
							nChosesGame1++;
							nTimeChosesGame1[nChosesGame1-1] = nCountGame1;
							if(nChosesGame1 > 0 && nChosesGame1 < CHOOSES_GAME1) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
							if(nChosesGame1 >= CHOOSES_GAME1)
							{
								FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
								for(int x = 0; x < 10; x++) bCh[x] = false;
							}
						}
					}
				}

				// 2
				if(bEnArrowsGame1[5])
				{
					if(bEnArrowsGame1[6] && !bCh[5])
					{
						MoveCha(true);
						bCh[5] = true;
					}
					else if(bSkinGame1[1] && !bCh[5])
					{
						TurnSkinsGame1(true, 1);
						bCh[5] = true;
					}
					else if(bCharacterGame1[1] && !bCh[5])
					{
						TurnCharactersGame1(true, 1);
						bCh[5] = true;
					}
					else
					{
						__int64 nCaret = nCountGame1 - nTimeEnArrowsGame1[5];
						int nLimit = min(nCaret,nCountGame1-nTimeChaGame1)>TIME_FAST_CHANGE_GAME1?TIME_CHANGE_HIGH_GAME1:TIME_CHANGE_LOW_GAME1;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[5])
						{
							bCh[5] = true;
							MoveSongs(true);
							PlaySoundTransiAboutLimit(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[5] = false;
						}
					}
				}
				if(bEnArrowsGame1[9])
				{
					if(bEnArrowsGame1[8] && !bCh[9])
					{
						MoveCha(false);
						bCh[9] = true;
					}
					else if(bSkinGame1[1] && !bCh[9])
					{
						TurnSkinsGame1(false, 1);
						bCh[9] = true;
					}
					else if(bCharacterGame1[1] && !bCh[9])
					{
						TurnCharactersGame1(false, 1);
						bCh[9] = true;
					}
					else
					{
						__int64 nCaret = nCountGame1 - nTimeEnArrowsGame1[9];
						int nLimit = min(nCaret,nCountGame1-nTimeChaGame1)>TIME_FAST_CHANGE_GAME1?TIME_CHANGE_HIGH_GAME1:TIME_CHANGE_LOW_GAME1;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[9])
						{
							bCh[9] = true;
							MoveSongs(false);
							PlaySoundTransiAboutLimit(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[9] = false;
						}
					}
				}
				if(bEnArrowsGame1[6] && !bCh[6])
				{
					if(!bSkinGame1[1] && !bCharacterGame1[1])
					{
						bCh[6] = true;
						MoveMode(1, true);
					}
				}
				if(bEnArrowsGame1[8] && !bCh[8])
				{
					if(!bSkinGame1[1] && !bCharacterGame1[1])
					{
						bCh[8] = true;
						MoveMode(1, false);
					}
				}
				if(bEnArrowsGame1[7] && !bCh[7])
				{
					bCh[7] = true;
					if(np[1])
					{
						if(bSkinGame1[1])
						{
							MakeChoiceSkinGame1(1);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else if(bCharacterGame1[1])
						{
							MakeChoiceCharacterGame1(1);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else
						{
							nChosesGame1++;
							nTimeChosesGame1[nChosesGame1-1] = nCountGame1;
							if(nChosesGame1 > 0 && nChosesGame1 < CHOOSES_GAME1) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
							if(nChosesGame1 >= CHOOSES_GAME1)
							{
								FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
								for(int x = 0; x < 10; x++) bCh[x] = false;
							}
						}
					}
				}

/*#define TIME_TOL_SWITCHRETRO_GAME1 30
				if((_abs64(nTimeEnArrowsGame1[0] - nTimeEnArrowsGame1[4]) < TIME_TOL_SWITCHRETRO_GAME1 && (bEnArrowsGame1[0] && bEnArrowsGame1[4])) ||
					(_abs64(nTimeEnArrowsGame1[5] - nTimeEnArrowsGame1[9]) < TIME_TOL_SWITCHRETRO_GAME1 && (bEnArrowsGame1[5] && bEnArrowsGame1[9])))
				{
					nChangeRetroGame1 = 1;
					nTimeChangeRetroGame1 = nCountGame1;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int y = 0; y < 10; y++)
					{
						bArrows[y] = false;
						bEnArrowsGame1[y] = false;
						bCh[y] = false;
					}
				}*/

/*#define TIME_TOL_SWITCHUNLOCKER_GAME1 30
				if(((_abs64(nTimeEnArrowsGame1[1] - nTimeEnArrowsGame1[3]) < TIME_TOL_SWITCHUNLOCKER_GAME1 && (bEnArrowsGame1[1] && bEnArrowsGame1[3])) ||
					(_abs64(nTimeEnArrowsGame1[6] - nTimeEnArrowsGame1[8]) < TIME_TOL_SWITCHUNLOCKER_GAME1 && (bEnArrowsGame1[6] && bEnArrowsGame1[8]))) && bIsAvalibleUnlockerGame1)
				{
					nChangeRetroGame1 = 3;
					nTimeChangeRetroGame1 = nCountGame1;
					bIsAvalibleUnlockerGame1 = false;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int y = 0; y < 10; y++)
					{
						bArrows[y] = false;
						bEnArrowsGame1[y] = false;
						bCh[y] = false;
					}
				}*/
				if(nCountGame1 >= nMaxGame1 && piuconfig.nModeMach != 1)
				{
					nChosesGame1 = 2;
					nTimeChosesGame1[nChosesGame1-1] = nCountGame1;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int x = 0; x < 10; x++) bCh[x] = false;
				}
				if(bNewCommand) CheckPreset();
			}
		}
	#define TIME_EXIT_GAME1 1000
		if(nChosesGame1 >= CHOOSES_GAME1)
		{
			__int64 nTransEnd = nCountGame1 - nTimeChosesGame1[CHOOSES_GAME1-1];
			if(nTransEnd < TIME_EXIT_GAME1)
			{
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME1))
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
					if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME1))
					{
						if(x == 4)
						{
							if(nTransEnd < (TIME_TRANSI_SELECTOR_GAME1))
							{
								dApt = //pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME1, 2);
								pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME1 - 0.25, 2)/0.125 + 0.5;
								glPushMatrix();
								glTranslated(0.0, DY_SONGBORDER_HOLD_GAME1, -60.0);
								glScaled(dApt*1.25, dApt*1.25, 1.0);
								glColor4d(1.0, 1.0, 1.0, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME1);
								if(/*x == 4 && */nTransBorder > TIME_SHOW_PREVIEW_GAME1 && movie_songpreview && movie_songpreview->bLoad && movie_songpreview->bResize && bShow)
								{
									movie_songpreview->Update(dwWait);
									movie_songpreview->GrabAVIFrame();
									__int64 nCaretCover = movie_songpreview->dwNext;
									glBindTexture(GL_TEXTURE_2D, movie_songpreview->tex);

									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
									glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
									glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
									glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
									glEnd();
									if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME1)
									{
										dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME1;
										glColor4d(1.0, 1.0, 1.0, min(1.0-dApt, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME1));
										glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
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
									glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
									glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
									glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
									glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
									glEnd();
								}
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
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
							__int64 nStartTransi = TIME_TRANSI_SELECTOR_GAME1/4*(-abs(x-4)+4-1);
							__int64 nLimitTransi = TIME_TRANSI_SELECTOR_GAME1/4*(-abs(x-4)+4);
							if(nTransEnd >= nStartTransi && nTransEnd < nLimitTransi)
							{
								dApt = double(nTransEnd-nStartTransi)/double(nLimitTransi-nStartTransi);
								double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(dApt)+dTargetx;
								double dy = pow(dx/MAX_TARGET_X,3.0)*50.0+DY_SONGBORDER_HOLD_GAME1;	// 0.75*40.0
								double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
								double dtheta = ThetaFuncForX(dx);
								double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
								double dalpha = AlphaFuncForX(dx);
								glPushMatrix();
								glTranslated(dx, dy, dz-60.0);
								glRotated(dtheta, 0.0, 1.0, 0.0);
								glScaled(dscale, dscale, 1.0);
								glColor4d(1.0, 1.0, 1.0, min(dalpha, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME1));
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
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
								double dy = pow(dx/MAX_TARGET_X,3.0)*50.0+DY_SONGBORDER_HOLD_GAME1;	// 0.75*40.0
								double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
								double dtheta = ThetaFuncForX(dx);
								double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
								double dalpha = AlphaFuncForX(dx);
								glPushMatrix();
								glTranslated(dx, dy, dz-60.0);
								glRotated(dtheta, 0.0, 1.0, 0.0);
								glScaled(dscale, dscale, 1.0);
								glColor4d(1.0, 1.0, 1.0, dalpha);
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
								glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
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
				for(int k = 0; k < 2; k++) if(np[k])
				{
					if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_LR_GAME1))
					{
						dApt = pow(double(nTransEnd)/TIME_TRANSI_LR_GAME1, 0.5);
						glPushMatrix();
						glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(dApt), 0.0, 0.0);
						glColor4d(1.0, 1.0, 1.0, 1-dApt);
						glBindTexture(GL_TEXTURE_2D, ga1res->skin[(k?GAME1RES_RightCha1:GAME1RES_LeftCha1)+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
				}
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_UPDO_GAME1))
				{
					dApt = (double(nTransEnd))/(TIME_TRANSI_UPDO_GAME1);
					glPushMatrix();
					glScaled(1.0+0.8*(dApt), 1.0+0.8*(dApt), 1.0);
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpAndDownMaskCha1+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpAndDownCha1+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpLeftChoice]->texture[0]);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-26.1-6.4,  18.2+6.4, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(-26.1+6.4,  18.2+6.4, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(-26.1+6.4,  18.2-6.4, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-26.1-6.4,  18.2-6.4, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpRightChoice]->texture[0]);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d( 26.1-6.4,  18.2+6.4, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 26.1+6.4,  18.2+6.4, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 26.1+6.4,  18.2-6.4, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( 26.1-6.4,  18.2-6.4, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DownLeftChoice]->texture[0]);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-19.5-6.4, -21.5+6.4, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(-19.5+6.4, -21.5+6.4, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(-19.5+6.4, -21.5-6.4, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-19.5-6.4, -21.5-6.4, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DownRightChoice]->texture[0]);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d( 19.5-6.4, -21.5+6.4, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 19.5+6.4, -21.5+6.4, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 19.5+6.4, -21.5-6.4, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( 19.5-6.4, -21.5-6.4, -50.0);
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
				for(int k = 0; k < 2; k++) if(np[k])
				{
					player_sesion[k]->nLastArcade[0] = nRefSongsGame1[nCurSongGame1];
					player_sesion[k]->nLastArcade[1] = nModeCurSongGame1[k];
				}
				SemiClearContextGame1();
				bShow = false;
				LoadSongGame1();
				Go_Step();
				return 2;
			}
		}
	}
	else
	{
#define TIME_EXIT_CHANGERETRO_GAME1 1000
		__int64 nTransEnd = nCountGame1 - nTimeChangeRetroGame1;
		if(nTransEnd < TIME_EXIT_CHANGERETRO_GAME1)
		{
			if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME1))
			{
				glBindTexture(GL_TEXTURE_2D, 0);
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
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME1))
				{
					if(x == 4)
					{
						if(nTransEnd < (TIME_TRANSI_SELECTOR_GAME1))
						{
							dApt = //pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME1, 2);
							pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME1 - 0.25, 2)/0.125 + 0.5;
							glPushMatrix();
							glTranslated(0.0, 0.0, -60.0);
							glScaled(dApt*1.25, dApt*1.25, 1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME1);
							if(nTransBorder > TIME_SHOW_PREVIEW_GAME1 && movie_songpreview->bLoad && movie_songpreview->bResize && bShow)
							{
								movie_songpreview->Update(dwWait);
								movie_songpreview->GrabAVIFrame();
								__int64 nCaretCover = movie_songpreview->dwNext;
								glBindTexture(GL_TEXTURE_2D, movie_songpreview->tex);

								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
								glEnd();
								if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME1)
								{
									dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME1;
									glColor4d(1.0, 1.0, 1.0, min(1.0-dApt, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME1));
									glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
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
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
							}
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
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
						__int64 nStartTransi = TIME_TRANSI_SELECTOR_GAME1/4*(-abs(x-4)+4-1);
						__int64 nLimitTransi = TIME_TRANSI_SELECTOR_GAME1/4*(-abs(x-4)+4);
						if(nTransEnd >= nStartTransi && nTransEnd < nLimitTransi)
						{
							dApt = double(nTransEnd-nStartTransi)/double(nLimitTransi-nStartTransi);
							double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(dApt)+dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForX(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForX(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, min(dalpha, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME1));
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
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
							double dtheta = ThetaFuncForX(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForX(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame1[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_SongBorderCha1+nChaSongsGame1[nIndexShowGame1[x]]]->GetCurrentTex());
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
			for(int k = 0; k < 2; k++) if(np[k])
			{
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_LR_GAME1))
				{
					dApt = pow(double(nTransEnd)/TIME_TRANSI_LR_GAME1, 0.5);
					glPushMatrix();
					glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(dApt), 0.0, 0.0);
					glColor4d(1.0, 1.0, 1.0, 1-dApt);
					glBindTexture(GL_TEXTURE_2D, ga1res->skin[(k?GAME1RES_RightCha1:GAME1RES_LeftCha1)+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();
				}
			}
			if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_UPDO_GAME1))
			{
				dApt = (double(nTransEnd))/(TIME_TRANSI_UPDO_GAME1);
				glPushMatrix();
				glScaled(1.0+0.8*(dApt), 1.0+0.8*(dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpAndDownMaskCha1+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpAndDownCha1+nChaSongsGame1[nCurSongGame1]]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpLeftChoice]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-26.1-6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(-26.1+6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(-26.1+6.4,  18.2-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-26.1-6.4,  18.2-6.4, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_UpRightChoice]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d( 26.1-6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 26.1+6.4,  18.2+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 26.1+6.4,  18.2-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( 26.1-6.4,  18.2-6.4, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DownLeftChoice]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-19.5-6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d(-19.5+6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d(-19.5+6.4, -21.5-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-19.5-6.4, -21.5-6.4, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga1res->skin[GAME1RES_DownRightChoice]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d( 19.5-6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 19.5+6.4, -21.5+6.4, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 19.5+6.4, -21.5-6.4, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d( 19.5-6.4, -21.5-6.4, -50.0);
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
			nCurSongGame1 = 0;
			bShow = false;
			if(nChangeRetroGame1 == 2)
			{
				return 4;
			}
			if(nChangeRetroGame1 == 3)
			{
				nCaretCountGame1 = nCountGame1;
				return 5;
			}
			for(int k = 0; k < 2; k++) if(np[k])
			{
				int nCar = FoundFirstRetro(nChangeRetroGame1==3?(nRetroActiveGame1?1:0):(nRetroActiveGame1?0:1));
				player_sesion[k]->nLastArcade[0] = nCar;
				player_sesion[k]->nLastArcade[1] = nModeCurSongGame1[k];
			}
			nCaretCountGame1 = nCountGame1;
			nMaxGame1 = nMaxGame1 - nCaretCountGame1;
			SemiClearContextGame1();
		}
	}

	return 0;
}

void SemiActualizateContextGame1()
{
	SetCurDir();
	int nPlayerFocus = 0;
	nChangeRetroGame1 = 0;

	if (movie_arcadebg == NULL)
	{
		movie_arcadebg = new CMovieStream2();
		movie_arcadebg->OpenAVI(_T("Game1/ArcadeBG.avi"));
		movie_arcadebg->bLoop = true;
	}

	if (ga1res == NULL)
	{
		ga1res = new CGame1Res();
		ga1res->LoadSkin();
	}

	switch(piuconfig.nTimePlay)
	{
	default:
		nMaxGame1 = 120000+TIME_TRANSI_UPDO_GAME1+TIME_START_UPDO_GAME1; break;
	case 1:
		nMaxGame1 = 90000+TIME_TRANSI_UPDO_GAME1+TIME_START_UPDO_GAME1; break;
	case 2:
		nMaxGame1 = 60000+TIME_TRANSI_UPDO_GAME1+TIME_START_UPDO_GAME1; break;
	}
	nTimeChangeRetroGame1 = 0;
	nModeGame1 = 0;
	nCurSongGame1 = 0;
	nChosesGame1 = 0;
	nCountGame1 = 0;
	nRetroActiveGame1 = 0;
	nTimeCurSongGame1 = TIME_START_SELECTOR_GAME1+TIME_TRANSI_SELECTOR_GAME1+TIME_GLOUTH_SELECTOR_GAME1;
	bLoadMusicCoverGame1 = false;
	strBestScoreMachineGame1 = _T("");
	nBestScoreMachineGame1 = 0;
	for(int x = 0; x < 2; x++)
	{
		nTimeSkinGame1[x] = 0;
		bSkinGame1[x] = false;
		nTimeTurnSkinGame1[x] = 0;
		nDirTurnSkinGame1[x] = 0;
		nTimeCharacterGame1[x] = 0;
		bCharacterGame1[x] = false;
		nTimeTurnCharacterGame1[x] = 0;
		nDirTurnCharacterGame1[x] = 0;
		nBestScorePlayerGame1[x] = 0;
		nModeCurSongGame1[x] = 0;
		nBackModeCurSongGame1[x] = 0;
		nTimeModeCurSongGame1[x] = TIME_START_SELECTOR_GAME1+TIME_TRANSI_SELECTOR_GAME1+TIME_GLOUTH_SELECTOR_GAME1;
		nPrefModeCurSongGame1[x] = 0;
		nLevelCurGame1[x] = 1;
		nIndLastPresetGame1[x] = -1;	// Show all
		nTimeLastPresetGame1[x] = (TIME_START_UPDO_GAME1+TIME_TRANSI_UPDO_GAME1)+(TIME_START_LR_GAME1+TIME_TRANSI_LR_GAME1);
		if(np[x]) ShowSesionTrayExp(x);
		for(int y = 0; y < 10; y++) nCommand[x][y] = -1;
		for(int y = 0; y < MAX_COMMANDS_GAME1; y++) nComandGame1[x][y] = -1;
	}

	for(int x = 0; x < 10; x++) {
        bEnArrowsGame1[x] = false;
        bArrows[x] = false;
    }
	for(int x = 0; x < 2; x++) if(np[x])
	{
		nRetroActiveGame1 = nNOLTotalSongsGame1[player_sesion[x]->nLastArcade[0]];
		EnumSongs(nNOLTotalSongsGame1[player_sesion[x]->nLastArcade[0]]);
		nPlayerFocus = x;
		break;
	}
	bool bFoundLast = false;
	for(int x = 0; x < nSizeSongsGame1; x++)
	{
		if(nRefSongsGame1[x] == player_sesion[nPlayerFocus]->nLastArcade[0])
		{
			bFoundLast = true;
			nCurSongGame1 = x;
			break;
		}
	}
	if(!bFoundLast) nCurSongGame1 = 0;
	for(int y = 0; y < 2; y++) if(np[y])
	{
		nModeCurSongGame1[y] = GetAvalibleMode(nCurSongGame1, player_sesion[y]->nLastArcade[1]);
		nBackModeCurSongGame1[y] = nModeCurSongGame1[y];
		nPrefModeCurSongGame1[y] = nModeCurSongGame1[y];
	}
	nTimeChaGame1 = TIME_START_UPDO_GAME1+TIME_TRANSI_UPDO_GAME1;

	ActualizateIndexShowGame1();
    TCHAR chFile[260];
	for(int x = 0; x < 9; x++)
	{
		_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsGame1[nIndexShowGame1[x]]);
		nTexCurSongGame1[x] = GenTexImage(chFile);
	}
	GetBestScores(nCurSongGame1, nModeCurSongGame1[nPlayerFocus]);
	EnumSkinsGame1();
	EnumCharactersGame1();
	dwLastTick = dwTick = SDL_GetTicks();
}

void SemiClearContextGame1()
{
	if (movie_songpreview)
	{
		if( movie_songpreview->bLoad ) movie_songpreview->CloseAVI();
		delete movie_songpreview;
		movie_songpreview = NULL;
	}
	glDeleteTextures(9, nTexCurSongGame1);
	if (movie_arcadebg)
	{
		if (movie_arcadebg->bLoad) movie_arcadebg->CloseAVI();
		delete movie_arcadebg;
		movie_arcadebg = NULL;
	}

	if (ga1res != NULL) delete ga1res;
	ga1res = NULL;
}

int StageCheckGame1()
{
	bool bIs[] = {false, false};
	for(int x = 0; x < 2; x++) if(np[x])
	{
		int nMileage = 0;
		if(x)
		{
			nMileage = viewp2->status.nScore/10000;
			nTotalScorePlayer[x] += viewp2->status.nScore;
		}
		else
		{
			nMileage = viewp1->status.nScore/10000;
			nTotalScorePlayer[x] += viewp1->status.nScore;
		}
		if(player_sesion[x]->nLockedAsp[ITEM_LUCK] && (rand()%5) == 0) nMileage *= 2;
		player_sesion[x]->nMileage += nMileage;
		double dApt = double(5-nRankStepEval[x])/5*(bUsb[0]?1.0:0.5)*(player_sesion[x]->nLockedAsp[ITEM_MOREEXP]?2.0:1.0);
		PlusExpPlayer(x, nLevelCurGame1[x], dApt);
		if(piuconfig.nModeMach)
		{
			np[x] = 0;
		}
		else
		{
			bIs[x] = true;
			np[x]-=nStageTotalSongsGame1[player_sesion[x]->nLastArcade[0]];
			if(nRankStepEval[x] >= 5) np[x]-=1;
			if(nRankStepEval[x] >= 2 && bBonusStageGame1[x])
			{
				np[x]-=1;
				bBonusStageGame1[x] = false;
			}
			np[x] = max(0, np[x]);
		}
	}
	CheckForUnlock();
	if((!np[0]) && (!np[1])) return 1;
	else if((!np[0] && bIs[0]) || (!np[1] && bIs[1])) return 2;
	return 0;
}

////////////////////////////////////////////////
// ** Funciones principales

int CreateContextGame1()
{
	if(!EnumTotalSongs()) return 0;
	nRefSkinGame1 = (int*)calloc(0, sizeof(int));
	uTexSkinGame1 = (GLuint*)calloc(0, sizeof(GLuint));
	nRefCharacterGame1 = (int*)calloc(0, sizeof(int));
	uTexCharacterGame1 = (GLuint*)calloc(0, sizeof(GLuint));
	return 1;
}

void DestroyContextGame1()
{
	free( nRefSkinGame1 );
	free(uTexSkinGame1 );
	free(nRefCharacterGame1 );
	free(uTexCharacterGame1 );
}

void ClearContextGame1()
{
    if(bLoadMusicCoverGame1)
    {
        bLoadMusicCoverGame1 = false;
        FMOD_Sound_Release(music_coverGame1);
    }
	SemiClearContextGame1();
	DeleteSkinsGame1();
	DeleteCharactersGame1();
}

void ActualizateContextGame1()
{
	SetCurDir();
	nCurStage = 1;

	if (movie_arcadebg == NULL)
	{
		movie_arcadebg = new CMovieStream2();
		movie_arcadebg->OpenAVI(_T("Game1/ArcadeBG.avi"));
		movie_arcadebg->bLoop = true;
	}

	if (ga1res == NULL)
	{
		ga1res = new CGame1Res();
		ga1res->LoadSkin();
	}

	switch(piuconfig.nTimePlay)
	{
	default:
		nMaxGame1 = 120000+TIME_TRANSI_UPDO_GAME1+TIME_START_UPDO_GAME1; break;
	case 1:
		nMaxGame1 = 90000+TIME_TRANSI_UPDO_GAME1+TIME_START_UPDO_GAME1; break;
	case 2:
		nMaxGame1 = 60000+TIME_TRANSI_UPDO_GAME1+TIME_START_UPDO_GAME1; break;
	}
	nInterGame1 = 0;
	nRetroActiveGame1 = 0;
	int nPlayerFocus = 0;
	nChangeRetroGame1 = 0;
	nTimeChangeRetroGame1 = 0;
	nModeGame1 = 0;
	nCurSongGame1 = 0;
	nChosesGame1 = 0;
	nCountGame1 = 0;
	nTimeCurSongGame1 = TIME_START_SELECTOR_GAME1+TIME_TRANSI_SELECTOR_GAME1+TIME_GLOUTH_SELECTOR_GAME1;
	bLoadMusicCoverGame1 = false;
	bContinueUsb = true;
	strBestScoreMachineGame1 = _T("");
	nBestScoreMachineGame1 = 0;
	bIsAvalibleUnlockerGame1 = true;
	for(int x = 0; x < 2; x++)
	{
		nTotalScorePlayer[x] = 0;
		nTimeSkinGame1[x] = 0;
		bSkinGame1[x] = false;
		nTimeTurnSkinGame1[x] = 0;
		nDirTurnSkinGame1[x] = 0;
		nCurSkinGame1[x] = 0;
		nTimeCharacterGame1[x] = 0;
		bCharacterGame1[x] = false;
		nTimeTurnCharacterGame1[x] = 0;
		nDirTurnCharacterGame1[x] = 0;
		nCurCharacterGame1[x] = 0;
		nBestScorePlayerGame1[x] = 0;
		bBonusStageGame1[x] = true;
		nModeCurSongGame1[x] = 0;
		nBackModeCurSongGame1[x] = 0;
		nTimeModeCurSongGame1[x] = TIME_START_SELECTOR_GAME1+TIME_TRANSI_SELECTOR_GAME1+TIME_GLOUTH_SELECTOR_GAME1;
		nPrefModeCurSongGame1[x] = 0;
		nLevelCurGame1[x] = 1;
		nIndLastPresetGame1[x] = -1;	// Show all
		nTimeLastPresetGame1[x] = (TIME_START_UPDO_GAME1+TIME_TRANSI_UPDO_GAME1)+(TIME_START_LR_GAME1+TIME_TRANSI_LR_GAME1);
		for(int y = 0; y < 6; y++)
		{
			nIndSizePresetGame1[x][y] = 0;
			nIndPresetGame1[x][y*5] = 0;
			nIndPresetGame1[x][y*5+1] = 0;
			nIndPresetGame1[x][y*5+2] = 0;
			nIndPresetGame1[x][y*5+3] = 0;
			nIndPresetGame1[x][y*5+4] = 0;
		}
		nIndSizePresetGame1[x][0] = 1;
        nIndPresetGame1[x][0] = 1;
		dDCurGame1[x][0] = 0.0;
		dDCurGame1[x][1] = 0.0;
		dDCurGame1[x][2] = 0.0;
		dDCurGame1[x][3] = 0.0;
		dDCurGame1[x][4] = 0.0;
		if(np[x]) ShowSesionTrayExp(x);
		for(int y = 0; y < 10; y++) nCommand[x][y] = -1;
		for(int y = 0; y < MAX_COMMANDS_GAME1; y++) nComandGame1[x][y] = -1;
	}
	for(int x = 0; x < 10; x++) {
        bEnArrowsGame1[x] = false;
        bArrows[x] = false;
    }
	for(int x = 0; x < 2; x++) if(np[x])
	{
		nRetroActiveGame1 = nNOLTotalSongsGame1[player_sesion[x]->nLastArcade[0]];
		EnumSongs(nNOLTotalSongsGame1[player_sesion[x]->nLastArcade[0]]);
		nPlayerFocus = x;
		break;
	}
	bool bFoundLast = false;
	for(int x = 0; x < nSizeSongsGame1; x++)
	{
		if(nRefSongsGame1[x] == player_sesion[nPlayerFocus]->nLastArcade[0])
		{
			bFoundLast = true;
			nCurSongGame1 = x;
			break;
		}
	}
	if(!bFoundLast) nCurSongGame1 = 0;
	for(int y = 0; y < 2; y++) if(np[y])
	{
		if(y == nPlayerFocus)
			nModeCurSongGame1[y] = GetAvalibleMode(nCurSongGame1, player_sesion[nPlayerFocus]->nLastArcade[1]);
		else
			nModeCurSongGame1[y] = GetAvalibleMode(nCurSongGame1, 0);
		nBackModeCurSongGame1[y] = nModeCurSongGame1[y];
		nPrefModeCurSongGame1[y] = nModeCurSongGame1[y];
	}
	nTimeChaGame1 = TIME_START_UPDO_GAME1+TIME_TRANSI_UPDO_GAME1;

	ActualizateIndexShowGame1();
		TCHAR chFile[260];
	for(int x = 0; x < 9; x++)
	{
		_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsGame1[nIndexShowGame1[x]]);
		nTexCurSongGame1[x] = GenTexImage(chFile);
	}
	GetBestScores(nCurSongGame1, nModeCurSongGame1[nPlayerFocus]);
	EnumSkinsGame1();
	EnumCharactersGame1();
	dwLastTick = dwTick = SDL_GetTicks();
}

bool bContAfterSplashGame1 = false;

int FrameGame1()
{
	if(nInterGame1 == 0) ga1res->Update(dwWait);
	switch(nInterGame1)
	{
	case 0:
		switch(DrawInterfaceSelect())
		{
		case 1:
			ClearContextGame1();
			return INT_WELCOME;
			break;
		case 2:
			//SemiClearContextGame1(); // When DrawInterfaceSelect returns 2, already called this function
			/*Draw_All();
			DeleteSongGame1();
			SemiActualizateContextGame1();*/ // TESTING
			nInterGame1 = 1;
			break;
		case 3:
			return INT_ITEMSHOP;
			break;
		case 4:
			ActualizateContextUsb();
			bSaveUsb = false;
			bContinueUsb = true;
			nInterGame1 = 4;
			for(int x = 0; x < 2; x++) if(np[x])
			{
				bShowSesionTray[x] = false;
				bShowSesionTrayExp[x] = false;
				if(uTexPlayerSplash[x] != uTexSplash[0]) glDeleteTextures(1, &uTexPlayerSplash[x]);
			}
			break;
		case 5:
			SemiClearContextGame1();
			nModeUnlock = 1;
			ActualizateContextUnlock();
			nInterGame1 = 6;
			break;
		}
		break;
	case 1:
		if(!Draw_All())
		{
			nInterGame1 = 2;
			nModeStepEval = 0;
			ActualizateContextStepEval();
		}
		else
		{
			int nMC = 51;
			double dMB = 0.0;
			for(int x = 0; x < 2; x++) if(g_bPlayer[x])
			{
				if(!x)
				{
					if(nMC > viewp1->status.nMissCombo[0]) nMC = viewp1->status.nMissCombo[0];
					if(dMB < viewp1->status.dLife) dMB = viewp1->status.dLife;
				}
				else
				{
					if(nMC > viewp2->status.nMissCombo[0]) nMC = viewp2->status.nMissCombo[0];
					if(dMB < viewp2->status.dLife) dMB = viewp2->status.dLife;
				}
			}
			if((nMC > 50 && piuconfig.nModeMach != 1) || (dMB <= 0.0 && piuconfig.nStageBreak!=0 && nCurStage >= piuconfig.nStageBreak))
			{
				nInterGame1 = 5;
				np[0] = 0;
				np[1] = 0;
				DeleteSongGame1();
			}
		}
		break;
	case 2:
		if(!FrameStepEval())
		{
			nInterGame1 = 0;
			bNewHighScore = false;
			int nCode = StageCheckGame1();
			if(nCode == 0)
			{
                ActualizateContextStageStepEval();
                nInterGame1 = 8;
            }
			else
			{
				if (bNewHighScore)
				{
					ActualizateContextHSStepEval();
					nInterGame1 = 9;
				}
				else
				{
					nInterGame1 = 4;
					bSaveUsb = true;
					ActualizateContextUsb();
				}
				if(nCode == 1) bContinueUsb = false;
				if(nCode == 2) bContinueUsb = true;
			}
			if(nSizeUnlock)
			{
				nModeUnlock = 0;
				ActualizateContextUnlock();
				nInterGame1 = 7;
				if(nCode != 0) bContAfterSplashGame1 = false;
				else bContAfterSplashGame1 = true;
			}
			DeleteSongGame1();
			SemiActualizateContextGame1();
		}
		break;
	case 3:
		{
			DeleteSongGame1();
			bNewHighScore = false;
			int nCode = StageCheckGame1();
			if(nCode == 0) nInterGame1 = 0;
			else
			{
				if (bNewHighScore)
				{
					ActualizateContextHSStepEval();
					nInterGame1 = 9;
				}
				else
				{
					nInterGame1 = 4;
					bSaveUsb = true;
					ActualizateContextUsb();
				}
				if(nCode == 1) bContinueUsb = false;
				if(nCode == 2) bContinueUsb = true;
			}
			if(nSizeUnlock)
			{
				nModeUnlock = 0;
				ActualizateContextUnlock();
				nInterGame1 = 7;
				if(nCode != 0) bContAfterSplashGame1 = false;
				else bContAfterSplashGame1 = true;
			}
		}
		SemiActualizateContextGame1();
		break;
	case 4:	// Dibujar Sistema USB
		if(!FrameUsb())
		{
			nInterGame1 = 0;
			if(!bContinueUsb)
			{
				ClearContextGame1();
				return INT_WELCOME;
			}
			else SemiActualizateContextGame1();
		}
		break;
	case 5:
		if(!FrameStageBreakStepEval())
		{
			if (bNewHighScore)
			{
				ActualizateContextHSStepEval();
				nInterGame1 = 9;
			}
			else
			{
				nInterGame1 = 4;
				np[0] = 0;
				np[1] = 0;
				bSaveUsb = true;
				ActualizateContextUsb();
			}
			bContinueUsb = false;
		}
		break;
	case 6:
		if(!FrameUnlock())
		{
			nInterGame1 = 0;
			SemiActualizateContextGame1();
			nMaxGame1 = nMaxGame1 - nCaretCountGame1;
		}
		break;
	case 7:
		if(FrameUnlock() != INT_UNLOCK)
		{
			if(bNewHighScore) nInterGame1 = 9;
			else nInterGame1 = bContAfterSplashGame1?8:4;
		}
		break;
	case 8:
		if(!FrameStageStepEval())
		{
			nInterGame1 = 0;
			nCurStage++;
		}
		break;
	case 9:
		if(!FrameHSStepEval())
		{
			nInterGame1 = 4;
			bSaveUsb = true;
			ActualizateContextUsb();
		}
		break;
	}
	return INT_GAME_1;
}

int EventGame1(int nEvent, int nKey)
{
	switch(nInterGame1)
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
	case 4:
		EventUsb(nEvent, nKey);
		break;
	case 6:
	case 7:
		EventUnlock(nEvent, nKey);
		break;
	case 9:
		EventStepEval(nEvent, nKey);
		break;
	}
	return 1;
}
