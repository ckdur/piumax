#include "Game4.h"
#include "stepeval.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "Game4Res.h"
#include "USBsys.h"
#include "UnlockSys.h"

#define TIME_START_SELECTOR_GAME4 0
#define TIME_TRANSI_SELECTOR_GAME4 1000
#define TIME_GLOUTH_SELECTOR_GAME4 500

#define TIME_START_START_GAME4 1500

#define TIME_TRANSI_SONGBORDER_GAME4 500
#define TIME_GLOUTH_SONGBORDER_GAME4 500
#define TIME_TRANSI_START_MAIN_GAME4 500
#define CHOOSES_GAME4 2
#define MAX_COMMANDS_GAME4 12

#define TIME_SHOW_PREVIEW_GAME4 600

int nComandGame4[2][MAX_COMMANDS_GAME4];
int nInterGame4 = 0;
int nSizeSongsGame4 = 0;
int nChaTotalSongsGame4[1000];
int nChaSongsGame4[1000];
int nRefSongsGame4[1000];
GLuint nTexCurSongGame4[9];
TCHAR chTotalSongsGame4[1000][100];
TCHAR chSongsGame4[1000][100];
bool bIsSingleSongGame4[1000];
bool bIsSingleTotalSongGame4[1000];

int nIndexShowGame4[9];
int nLevelCurGame4[2];
double dDCurGame4[2][5];
int nCurSongGame4 = 0;
/*int nModeCurSongGame4[2];
int nBackModeCurSongGame4[2];
int nPrefModeCurSongGame4[2];
__int64 nTimeModeCurSongGame4[2];*/
__int64 nTimeCurSongGame4 = 0;
__int64 nCountGame4 = 0;
__int64 nMaxGame4 = 0;
__int64 nTimeChaGame4 = 0;

__int64 nCaretCountGame4;

__int64 nTimeLastPresetGame4[2];
int nIndLastPresetGame4[2];
int nIndPresetGame4[2][30];
int nIndSizePresetGame4[2][6];
int nSkinGame4[2];
int nCharacterGame4[2];

int nChosesGame4;
__int64 nTimeChosesGame4[2];
int nChangeRetroGame4;
__int64 nTimeChangeRetroGame4;
int nRetroActiveGame4;

bool bEnArrowsGame4[10];
__int64 nTimeEnArrowsGame4[10];
int nCommandGame4[2][10];

int nTimeTransiSongBorderGame4 = TIME_TRANSI_SONGBORDER_GAME4;
int nModeGame4 = 0;
bool bBonusStageGame4[2];
//bool bSaveUsbGame4 = false;
bool bContinueUsbGame4 = true;

GLuint movie_texGame4;
CMovieStream2* movie_brainbg;
CMovieStream2* movie_songpreviewgame4;

bool bLoadMusicCoverGame4 = false;
FMOD_SOUND* music_coverGame4;

CString strDispGame4;

CGame4Res* ga4res;

CString strBestScoreMachineGame4;
int nBestScoreMachineGame4;
int nBestScorePlayerGame4[2];
int nBestScoreMachineGame4Ex;
int nBestScorePlayerGame4Ex[2];

int FoundPresetGame4(int x, int nFamily, int nBegin, int nEnd);
void SemiActualizateContextGame4();

int nSizeSkinGame4 = 0;
int* nRefSkinGame4;
GLuint* uTexSkinGame4;
int nSizeCharacterGame4 = 0;
int* nRefCharacterGame4;
GLuint* uTexCharacterGame4;

#define MAX_SKINS_GAME4 20
#define MAX_CHARACTERS_GAME4 12

__int64 nTimeSkinGame4[2];
bool bSkinGame4[2];
__int64 nTimeTurnSkinGame4[2];
int nDirTurnSkinGame4[2];
int nCurSkinGame4[2];
__int64 nTimeCharacterGame4[2];
bool bCharacterGame4[2];
__int64 nTimeTurnCharacterGame4[2];
int nDirTurnCharacterGame4[2];
int nCurCharacterGame4[2];

bool bIsAvalibleUnlockerGame4;

int FoundPresetGame4(int x, int nFamily, int nBegin, int nEnd);

void MakeChoiceSkinGame4(int x)
{
	bSkinGame4[x] = false;
	int nPut = nRefSkinGame4[nCurSkinGame4[x]-1]+25;
	int nFamily = 2;
	int nFound = FoundPresetGame4(x, nFamily, 24, 45);
	if(nFound == -1 && nCurSkinGame4[x])
	{
		nFound = nIndSizePresetGame4[x][nFamily];
		nIndSizePresetGame4[x][nFamily]++;
		nIndPresetGame4[x][nFamily*5+nFound] = nPut;
		nIndLastPresetGame4[x] = nFamily*5+nFound;
		nTimeLastPresetGame4[x] = nCountGame4;
	}
	if(nFound != 1)
	{
		if(!nCurSkinGame4[x])	// Desactivar
		{
			nIndPresetGame4[x][nFamily*5+nFound] = 0;
			for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
				nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
			nIndSizePresetGame4[x][nFamily]--;
		}
		else	// Actualizar
		{
			nIndPresetGame4[x][nFamily*5+nFound] = nPut;
			nIndLastPresetGame4[x] = nFamily*5+nFound;
			nTimeLastPresetGame4[x] = nCountGame4;
		}
	}
}

void MakeChoiceCharacterGame4(int x)
{
	bCharacterGame4[x] = false;
	int nPut = nRefCharacterGame4[nCurCharacterGame4[x]-1]+47;
	int nFamily = 5;
	int nFound = FoundPresetGame4(x, nFamily, 46, 59);
	if(nFound == -1 && nCurCharacterGame4[x])
	{
		nFound = nIndSizePresetGame4[x][nFamily];
		nIndSizePresetGame4[x][nFamily]++;
		nIndPresetGame4[x][nFamily*5+nFound] = nPut;
		nIndLastPresetGame4[x] = nFamily*5+nFound;
		nTimeLastPresetGame4[x] = nCountGame4;
	}
	if(nFound != 1)
	{
		if(!nCurCharacterGame4[x])	// Desactivar
		{
			nIndPresetGame4[x][nFamily*5+nFound] = 0;
			for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
				nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
			nIndSizePresetGame4[x][nFamily]--;
		}
		else	// Actualizar
		{
			nIndPresetGame4[x][nFamily*5+nFound] = nPut;
			nIndLastPresetGame4[x] = nFamily*5+nFound;
			nTimeLastPresetGame4[x] = nCountGame4;
		}
	}
}


void TurnSkinsGame4(bool bIzq, int x)
{
	if(bIzq)
	{
		nCurSkinGame4[x]--;
		if(nCurSkinGame4[x] < 0) nCurSkinGame4[x] = nSizeSkinGame4;
	}
	else
	{
		nCurSkinGame4[x]++;
		if(nCurSkinGame4[x] >= (nSizeSkinGame4+1)) nCurSkinGame4[x] = 0;
	}
	nDirTurnSkinGame4[x] = bIzq?1:2;
	nTimeTurnSkinGame4[x] = nCountGame4;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
}

void TurnCharactersGame4(bool bIzq, int x)
{
	if(bIzq)
	{
		nCurCharacterGame4[x]--;
		if(nCurCharacterGame4[x] < 0) nCurCharacterGame4[x] = nSizeCharacterGame4;
	}
	else
	{
		nCurCharacterGame4[x]++;
		if(nCurCharacterGame4[x] >= (nSizeCharacterGame4+1)) nCurCharacterGame4[x] = 0;
	}
	nDirTurnCharacterGame4[x] = bIzq?1:2;
	nTimeTurnCharacterGame4[x] = nCountGame4;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
}

void DrawSkinsGame4(int x)
{
#define TIME_TRANSI_SKIN_GAME4 500
	__int64 nTimeSkin = nCountGame4 - nTimeSkinGame4[x];
	double dApt;
	int n[5];
	for(int y = 0; y < 5; y++)
	{
		n[y] = (y-2+nCurSkinGame4[x])%(nSizeSkinGame4+1);
		while(n[y] < 0) n[y] += (nSizeSkinGame4+1);
	}
	if(nTimeSkin >= 0 && nTimeSkin < TIME_TRANSI_SKIN_GAME4)
	{
		dApt = double(nTimeSkin)/TIME_TRANSI_SKIN_GAME4;
		glColor4d(1.0, 1.0, 1.0, dApt);
		glPushMatrix();
		glTranslated(0.0, -2.5*(1-dApt)+11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga4res->skin[168]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
		glBindTexture(GL_TEXTURE_2D, ga4res->skin[167]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[2]==0?ga4res->skin[169]->GetCurrentTex():uTexSkinGame4[n[2]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(10.0-5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga4res->skin[167]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[3]==0?ga4res->skin[169]->GetCurrentTex():uTexSkinGame4[n[3]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(-10.0+5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga4res->skin[167]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[1]==0?ga4res->skin[169]->GetCurrentTex():uTexSkinGame4[n[1]-1]);
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
	if(nTimeSkin >= TIME_TRANSI_SKIN_GAME4)
	{
#define TIME_TURNSKIN_GAME4 300
		glPushMatrix();
		glTranslated(0.0, 11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga4res->skin[168]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		__int64 nTimeTurn = nCountGame4 - nTimeTurnSkinGame4[x];
		dApt = 0.0;
		if(nTimeTurn >= 0 && nTimeTurn <= TIME_TURNSKIN_GAME4 && nDirTurnSkinGame4[x])
		{
			dApt = (nDirTurnSkinGame4[x]==1?-1:1)*(1-double(nTimeTurn)/TIME_TURNSKIN_GAME4);
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
			glBindTexture(GL_TEXTURE_2D, ga4res->skin[167]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, n[u]==0?ga4res->skin[169]->GetCurrentTex():uTexSkinGame4[n[u]-1]);
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

void DrawCharactersGame4(int x)
{
#define TIME_TRANSI_CHARACTER_GAME4 500
	__int64 nTimeCharacter = nCountGame4 - nTimeCharacterGame4[x];
	double dApt;
	int n[5];
	for(int y = 0; y < 5; y++)
	{
		n[y] = (y-2+nCurCharacterGame4[x])%(nSizeCharacterGame4+1);
		while(n[y] < 0) n[y] += (nSizeCharacterGame4+1);
	}
	if(nTimeCharacter >= 0 && nTimeCharacter < TIME_TRANSI_CHARACTER_GAME4)
	{
		dApt = double(nTimeCharacter)/TIME_TRANSI_CHARACTER_GAME4;
		glColor4d(1.0, 1.0, 1.0, dApt);
		glPushMatrix();
		glTranslated(0.0, -2.5*(1-dApt)+11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga4res->skin[171]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
		glBindTexture(GL_TEXTURE_2D, ga4res->skin[170]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[2]==0?ga4res->skin[172]->GetCurrentTex():uTexCharacterGame4[n[2]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(10.0-5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga4res->skin[170]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[3]==0?ga4res->skin[172]->GetCurrentTex():uTexCharacterGame4[n[3]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, -5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0,  5.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(-10.0+5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga4res->skin[170]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[1]==0?ga4res->skin[172]->GetCurrentTex():uTexCharacterGame4[n[1]-1]);
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
	if(nTimeCharacter >= TIME_TRANSI_CHARACTER_GAME4)
	{
#define TIME_TURNCHARACTER_GAME4 300
		glPushMatrix();
		glTranslated(0.0, 11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga4res->skin[171]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		__int64 nTimeTurn = nCountGame4 - nTimeTurnCharacterGame4[x];
		dApt = 0.0;
		if(nTimeTurn >= 0 && nTimeTurn <= TIME_TURNCHARACTER_GAME4 && nDirTurnCharacterGame4[x])
		{
			dApt = (nDirTurnCharacterGame4[x]==1?-1:1)*(1-double(nTimeTurn)/TIME_TURNCHARACTER_GAME4);
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
			glBindTexture(GL_TEXTURE_2D, ga4res->skin[170]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, n[u]==0?ga4res->skin[172]->GetCurrentTex():uTexCharacterGame4[n[u]-1]);
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

void DeleteSkinsGame4();
void DeleteCharactersGame4();

void EnumSkinsGame4()
{
	DeleteSkinsGame4();
	for(int x = 0; x < MAX_SKINS_GAME4; x++)
	{
		if((np[0] && !player_sesion[0]->nLockedAsp[x]) ||
			(np[1] && !player_sesion[1]->nLockedAsp[x]))
		{
			nSizeSkinGame4++;
			nRefSkinGame4 = (int*)realloc(nRefSkinGame4, nSizeSkinGame4*sizeof(int));
			uTexSkinGame4 = (GLuint*)realloc(uTexSkinGame4, nSizeSkinGame4*sizeof(GLuint));
			nRefSkinGame4[nSizeSkinGame4-1] = x;
			CString str;
			str.Format(_T("skins/%d/Preview.png"), x+1);
			uTexSkinGame4[nSizeSkinGame4-1] = GenTexImage(str);
		}
	}
}

void EnumCharactersGame4()
{
	DeleteCharactersGame4();
	for(int x = 0; x < MAX_CHARACTERS_GAME4; x++)
	{
		if((np[0] && !player_sesion[0]->nLockedAsp[20+x]) ||
			(np[1] && !player_sesion[1]->nLockedAsp[20+x]))
		{
			nSizeCharacterGame4++;
			nRefCharacterGame4 = (int*)realloc(nRefCharacterGame4, nSizeCharacterGame4*sizeof(int));
			uTexCharacterGame4 = (GLuint*)realloc(uTexCharacterGame4, nSizeCharacterGame4*sizeof(GLuint));
			nRefCharacterGame4[nSizeCharacterGame4-1] = x;
			CString str;
			str.Format(_T("Characters/%d/Preview.png"), x+1);
			uTexCharacterGame4[nSizeCharacterGame4-1] = GenTexImage(str);
		}
	}
}

void DeleteSkinsGame4()
{
	if(nSizeSkinGame4)
	{
		glDeleteTextures(nSizeSkinGame4, uTexSkinGame4);
		nSizeSkinGame4 = 0;
		nRefSkinGame4 = (int*)realloc(nRefSkinGame4, nSizeSkinGame4*sizeof(int));
		uTexSkinGame4 = (GLuint*)realloc(uTexSkinGame4, nSizeSkinGame4*sizeof(GLuint));
	}
}

void DeleteCharactersGame4()
{
	if(nSizeCharacterGame4)
	{
		glDeleteTextures(nSizeCharacterGame4, uTexCharacterGame4);
		nSizeCharacterGame4 = 0;
		nRefCharacterGame4 = (int*)realloc(nRefCharacterGame4, nSizeCharacterGame4*sizeof(int));
		uTexCharacterGame4 = (GLuint*)realloc(uTexCharacterGame4, nSizeCharacterGame4*sizeof(GLuint));
	}
}

int LoadSongGame4()
{
	// ** Cargar datos basicos
	ARGSTEP arg;
	arg.strMusic.Format(_T("song/%ls/%ls.mp3"), chSongsGame4[nCurSongGame4], chSongsGame4[nCurSongGame4]);
	arg.strLights.Format(_T("song/%ls/%ls.lgt"), chSongsGame4[nCurSongGame4], chSongsGame4[nCurSongGame4]);
	CString str;
	TCHAR ch[260];
	for(int x = 0; x < 2; x++) if(np[x])
	{
		str.Format(_T("song/%ls/%ls.acc"), chSongsGame4[nCurSongGame4], chSongsGame4[nCurSongGame4]);
		FILE* hFile = _fopen(str, _T("r"));
		if(hFile != NULL)
		{
			if( _fgets(ch, 260, hFile) != NULL)
			{
				arg.strBGA.Format(_T("song/%ls/%ls"), chSongsGame4[nCurSongGame4], ch);
				if(arg.strBGA.Find(_T('[')) != -1)
					arg.strBGA.Format(_T("song/GNRBGA/%ls"), ch);
				arg.strBGA.Remove(_T('\n')); arg.strBGA.Remove(_T('\r'));
			}
			else arg.strBGA.Format(_T("song/%ls/%ls.avi"), chSongsGame4[nCurSongGame4], chSongsGame4[nCurSongGame4]);
			fclose(hFile);
		}
		else arg.strBGA.Format(_T("song/%ls/%ls.avi"), chSongsGame4[nCurSongGame4], chSongsGame4[nCurSongGame4]);
		if(player_sesion[x]->nLockedAsp[ITEM_BGAOFF])
		{
			arg.strBGA = _T("");
			break;
		}
	}
	arg.strSplash.Format(_T("song/%ls/%ls.png"), chSongsGame4[nCurSongGame4], chSongsGame4[nCurSongGame4]);
	arg.bAutoPlay = false;
	if(!LoadSong(arg)) return 0;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		HideSesionTrayExp(x);
		arg.strFile.Format(_T("song/%ls/%ls_%ls.sksf"), chSongsGame4[nCurSongGame4], chSongsGame4[nCurSongGame4], chBraModes[min(nChaSongsGame4[nCurSongGame4], 4)]);
		arg.dUserVel = 1.0;
		arg.bIsSingle = bIsSingleSongGame4[nCurSongGame4];
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
		nCode = FoundPresetGame4(x, 0, 1, 7);
		if(nCode == -1)
			arg.dUserVel = 1.0;
		else
		{
			switch(nIndPresetGame4[x][nCode])	// 0*5+nCode
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
		nCode = FoundPresetGame4(x, 0, 8, 9);
		if(nCode != -1)
		{
			switch(nIndPresetGame4[x][nCode])	// 0*5+nCode
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
		nCode = FoundPresetGame4(x, 1, 10, 11);
		if(nCode != -1)
		{
			switch(nIndPresetGame4[x][5+nCode])	// 0*5+nCode
			{
			case 10:
				arg.nVanish = 5;
				break;
			case 11:
				arg.nVanish = 11;
				break;
			}
		}
		if(FoundPresetGame4(x, 1, 12, 12) != -1) arg.bFlash = true;
		if(FoundPresetGame4(x, 1, 13, 13) != -1) arg.bFreedom = true;
		// Familia 2
		nCode = FoundPresetGame4(x, 2, 24, 45);
		if(nCode != -1)
		{
			switch(nIndPresetGame4[x][10+nCode])
			{
			case 24:
				arg.bRandomSkin = true;
				break;
			default:
				arg.nSkin = nIndPresetGame4[x][10+nCode]-24;
				break;
			}
		}
		// Familia 5
		nCode = FoundPresetGame4(x, 5, 46, 59);
		if(nCode != -1)
		{
			switch(nIndPresetGame4[x][25+nCode])
			{
			case 46:
				arg.bCharacter = true;
				arg.nCharacter = (rand()%12)+1;
				arg.nLevelCharacer = 20; //????
				break;
			default:
				arg.bCharacter = true;
				arg.nCharacter = nIndPresetGame4[x][25+nCode]-46;
				arg.nLevelCharacer = 20; //????
				break;
			}
		}
		// Familia 3
		nCode = FoundPresetGame4(x, 3, 14, 15);
		if(nCode != -1)
		{
			switch(nIndPresetGame4[x][15+nCode])	// 0*5+nCode
			{
			case 14:
				arg.bRS = true;
				break;
			case 15:
				arg.bMirror = true;
				break;
			}
		}
		//if(FoundPresetGame4(x, 3, 16, 16) != -1);
		if(FoundPresetGame4(x, 3, 17, 17) != -1) arg.bTrainingMode = true;
		if(FoundPresetGame4(x, 3, 18, 18) != -1) arg.dPowTimer = POW_TIMER_HARD;
		// Familia 4
		if(FoundPresetGame4(x, 4, 19, 19) != -1) arg.bSnake = true;
		if(FoundPresetGame4(x, 4, 20, 20) != -1) arg.bNXMode = true;
		if(FoundPresetGame4(x, 4, 21, 21) != -1) arg.bXMode = true;
		nCode = FoundPresetGame4(x, 4, 22, 23);
		if(nCode != -1)
		{
			switch(nIndPresetGame4[x][20+nCode])	// 0*5+nCode
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

void DeleteSongGame4()
{
	for(int x = 0; x < 2; x++) if(np[x])
	{
		if(player_sesion[x]->nBrainShowerRecords[nRefSongsGame4[nCurSongGame4]][0] < (x?viewp2->status.nScore:viewp1->status.nScore))
		{
			for(int y = 19; y >= 0; y--)
			{
				player_sesion[x]->nBrainShowerRecords[nRefSongsGame4[nCurSongGame4]][y+1] =
					player_sesion[x]->nBrainShowerRecords[nRefSongsGame4[nCurSongGame4]][y];
				player_sesion[x]->nBrainShowerRecordsEx[nRefSongsGame4[nCurSongGame4]][y+1] =
					player_sesion[x]->nBrainShowerRecordsEx[nRefSongsGame4[nCurSongGame4]][y];
			}
			player_sesion[x]->nBrainShowerRecords[nRefSongsGame4[nCurSongGame4]][0] = (x?viewp2->status.nScore:viewp1->status.nScore);
			player_sesion[x]->nBrainShowerRecordsEx[nRefSongsGame4[nCurSongGame4]][0] = (x?viewp2->status.nBSCorrect:viewp1->status.nBSCorrect);
		}
		if(machine_sesion->nBrainShowerRecords[nRefSongsGame4[nCurSongGame4]] < (x?viewp2->status.nScore:viewp1->status.nScore))
		{
			machine_sesion->nBrainShowerRecords[nRefSongsGame4[nCurSongGame4]] = (x?viewp2->status.nScore:viewp1->status.nScore);
			machine_sesion->nBrainShowerRecordsEx[nRefSongsGame4[nCurSongGame4]] = (x?viewp2->status.nBSCorrect:viewp1->status.nBSCorrect);
#ifndef _WIN32
			Utf16ToUtf32(player_sesion[x]->chName, machine_sesion->chBrainShowerRecords[nRefSongsGame4[nCurSongGame4]], 12);
#else
			_strcpy(machine_sesion->chBrainShowerRecords[nRefSongsGame4[nCurSongGame4]], player_sesion[x]->chName);
#endif
		}
	}
	for(int x = 0; x < 2; x++) if(g_bPlayer[x])
	{
		DeleteSongCompile(x+1);
	}
	DeleteSong();
}

CString GetIDTotalSongsGame4(int nIndex)
{
	if(nIndex >= nSizeTotalGame4) return CString(_T(""));
	CString str;
	str = ( chTotalSongsGame4[nIndex]);
	return str;
}

CString GetNameTotalSongsGame4(int nIndex)
{
	if(nIndex >= nSizeTotalGame4) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame4[nIndex], chTotalSongsGame4[nIndex], chLang);
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

CString GetArtistTotalSongsGame4(int nIndex)
{
	if(nIndex >= nSizeTotalGame4) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame4[nIndex], chTotalSongsGame4[nIndex], chLang);
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

CString GetBPMTotalSongsGame4(int nIndex)
{
	if(nIndex >= nSizeTotalGame4) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame4[nIndex], chTotalSongsGame4[nIndex], chLang);
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

CString GetNameSongsGame4(int nIndex)
{
	if(nIndex >= nSizeSongsGame4) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame4[nIndex], chSongsGame4[nIndex], chLang);
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

CString GetArtistSongsGame4(int nIndex)
{
	if(nIndex >= nSizeSongsGame4) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame4[nIndex], chSongsGame4[nIndex], chLang);
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

CString GetBPMSongsGame4(int nIndex)
{
	if(nIndex >= nSizeSongsGame4) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame4[nIndex], chSongsGame4[nIndex], chLang);
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

void GetLevelSongsGame4(int nIndex, int* nLevel, double* d1, double* d2, double* d3, double* d4, double* d5)
{
	if(nIndex >= nSizeSongsGame4)
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
	_sprintf(chFile, _T("song/%ls/%ls_%ls.sksf"), chSongsGame4[nIndex], chSongsGame4[nIndex], chBraModes[min(nChaSongsGame4[nIndex], 4)]);
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

void GetBestScoresGame4(int nIndex)
{
	nBestScoreMachineGame4 = machine_sesion->nBrainShowerRecords[nRefSongsGame4[nIndex]];
	nBestScoreMachineGame4Ex = machine_sesion->nBrainShowerRecordsEx[nRefSongsGame4[nIndex]];
	for(int x = 0; x < 2; x++)
	{
		if(np[x])
		{
			nBestScorePlayerGame4[x] = player_sesion[x]->nBrainShowerRecords[nRefSongsGame4[nIndex]][0];
			nBestScorePlayerGame4Ex[x] = player_sesion[x]->nBrainShowerRecordsEx[nRefSongsGame4[nIndex]][0];
		}
		else
		{
			nBestScorePlayerGame4[x] = 0;
			nBestScorePlayerGame4Ex[x] = 0;
		}
	}
	strBestScoreMachineGame4 = ( machine_sesion->chBrainShowerRecords[nRefSongsGame4[nIndex]]);
}

int LoadDefaultsGame4(int nPlayer)
{
	if(!AdaptMachineProfile(1, nPlayer)) return 0;
	return 1;
}

int EnumTotalSongsGame4()
{
	FILE* hFile = NULL;
	hFile = _fopen(_T("songBrainNew.acc"), _T("r"));
	if(hFile == NULL) return 0;
	TCHAR ch[500];
	int nChannel = 0;
	nSizeTotalGame4 = 0;
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
				nSizeTotalGame4++;
				int nCode = str.Find(_T(':'));
				_strcpy(chTotalSongsGame4[nSizeTotalGame4-1], str.Left(nCode));
				nChaTotalSongsGame4[nSizeTotalGame4-1] = nChannel==-1?0:nChannel;
				int n1;
				nCode = str.GetLength() - nCode - 1;
				if(_sscanf(str.Right(nCode), _T("%d"), &n1) != 1) {fclose(hFile); return 0;}
				bIsSingleTotalSongGame4[nSizeTotalGame4-1] = n1?true:false;
			}
		}
	}
	fclose(hFile);
	nSizeTotalGame4Ex = nSizeTotalGame4;
	return 1;
}

int EnumSongsGame4()
{
	bool bAutorizeDoubles = !(np[0]&&np[1]);
	nSizeSongsGame4 = 0;
	int nAutorize[1000];
	for(int k = 0; k < 1000; k++)
	{
		nAutorize[k] = 0;
		nChaSongsGame4[k] = 0;
	}
	for(int d = 0; d < 2; d++) if(np[d])
	{
		for(int s = 0; s < nSizeTotalGame4; s++)
		{
			if((!player_sesion[d]->nLockedBrainShower[s] || !machine_sesion->nLockedBrainShower[s] || player_sesion[d]->nLockedAsp[ITEM_ALLSONG]) && (bAutorizeDoubles || bIsSingleTotalSongGame4[s]))
				nAutorize[s] = 1;
		}
	}
	for(int s = 0; s < nSizeTotalGame4; s++)
	{
		if(nAutorize[s])
		{
			nChaSongsGame4[nSizeSongsGame4] = nChaTotalSongsGame4[s];
			nRefSongsGame4[nSizeSongsGame4] = s;
			_strcpy(chSongsGame4[nSizeSongsGame4], chTotalSongsGame4[s]);
			bIsSingleSongGame4[nSizeSongsGame4] = bIsSingleTotalSongGame4[s];
			nSizeSongsGame4++;
		}
	}
	return 1;
}

#define MAX_TARGET_X 60.0
double AlphaFuncForXGame4(double dx)
{
	if(abs(dx) >= MAX_TARGET_X) return 0.0;
	if(abs(dx) <= (MAX_TARGET_X/4*3)) return 1.0;
	return (MAX_TARGET_X - abs(dx)) / (MAX_TARGET_X/4);
}

double ThetaFuncForXGame4(double dx)
{
	if(abs(dx) >= MAX_TARGET_X/4) return -19.0*dx/abs(dx);
	/*if(abs(dx) <= (MAX_TARGET_X/4))*/ return -19.0*dx*4/MAX_TARGET_X;
	//return (MAX_TARGET_X - abs(dx)) / (MAX_TARGET_X/4);
}


void ActualizateIndexShowGame4()
{
	for(int x = 0; x < 9; x++)
	{
		nIndexShowGame4[x] = nCurSongGame4+x-4;
		bool bAsegureLoop = true;
		while(bAsegureLoop)
		{
			bAsegureLoop = false;
			if(nIndexShowGame4[x] < 0)
			{
				nIndexShowGame4[x] = nSizeSongsGame4+nIndexShowGame4[x];
				bAsegureLoop = true;
			}
			if(nIndexShowGame4[x] >= nSizeSongsGame4)
			{
				nIndexShowGame4[x] = nIndexShowGame4[x]-nSizeSongsGame4;
				bAsegureLoop = true;
			}
		}
	}
}

void MoveSongsGame4(bool bIsIzq)
{
	int nb = nCurSongGame4;
	if(bIsIzq)
	{
		nCurSongGame4--;
		if(nCurSongGame4 < 0) nCurSongGame4 = nSizeSongsGame4-1;
		if(nChaSongsGame4[nb] != nChaSongsGame4[nCurSongGame4]) nTimeChaGame4 = nCountGame4;
		nModeGame4 = -1;
		nChosesGame4 = 0;
		/*Borrar textura sobrante*/
		if(nTexCurSongGame4[8] != m_uNullTex)
			glDeleteTextures(1, &nTexCurSongGame4[8]);
		ActualizateIndexShowGame4();
		//Mover a la derecha los items
		for(int x = 7; x >= 0; x--)
		{
			nTexCurSongGame4[x+1] = nTexCurSongGame4[x];
		}
		// Finalmente cargar la nueva imagen
		TCHAR chFile[260];
		_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsGame4[nIndexShowGame4[0]]);
		nTexCurSongGame4[0] = GenTexImage(chFile);
		nTimeCurSongGame4 = nCountGame4;
	}
	else
	{
		nCurSongGame4++;
		if(nCurSongGame4 >= nSizeSongsGame4) nCurSongGame4 = 0;
		if(nChaSongsGame4[nb] != nChaSongsGame4[nCurSongGame4]) nTimeChaGame4 = nCountGame4;
		nModeGame4 = 1;
		nChosesGame4 = 0;
		/*Borrar textura sobrante*/
		if(nTexCurSongGame4[0] != m_uNullTex) glDeleteTextures(1, &nTexCurSongGame4[0]);
		ActualizateIndexShowGame4();
		//Mover a la izquierda los items
		for(int x = 0; x < 8; x++)
		{
			nTexCurSongGame4[x] = nTexCurSongGame4[x+1];
		}
		// Finalmente cargar la nueva imagen
		TCHAR chFile[260];
		_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsGame4[nIndexShowGame4[8]]);
		nTexCurSongGame4[8] = GenTexImage(chFile);
		nTimeCurSongGame4 = nCountGame4;
	}
}

int FoundPresetGame4(int x, int nFamily, int nBegin, int nEnd)
{
	int nFound = -1;
	for(int k = 0; k < nIndSizePresetGame4[x][nFamily]; k++)
	{
		if(nIndPresetGame4[x][nFamily*5+k] >= nBegin && nIndPresetGame4[x][nFamily*5+k] <= nEnd)
		{
			nFound = k;
			break;
		}
	}
	return nFound;
}

void CheckPresetGame4()
{
	for(int x = 0; x < 2; x++) if(np[x])
	{
		bool bPreset = false;
		int nFamily;
		// ** Primer nivel (0)
		nFamily = 0;
		// ** Segundo nivel (1)
		nFamily = 1;
		// Vanish & NonStep (V & NS)
		if(nComandGame4[x][0] == 2 &&
		   nComandGame4[x][1] == 4 &&
		   nComandGame4[x][2] == 0 &&
		   nComandGame4[x][3] == 3 &&
		   nComandGame4[x][4] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame4(x, nFamily, 10, 11);
			int nCur = 0;
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame4[x][nFamily];
				nIndSizePresetGame4[x][nFamily]++;
				nCur = 9;
			}
			else nCur = nIndPresetGame4[x][nFamily*5+nFound];
			nCur++;
			if(nCur > 11 || nCur < 10)
			{
				nIndPresetGame4[x][nFamily*5+nFound] = 0;
				for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
					nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
				nIndSizePresetGame4[x][nFamily]--;
			}
			else
			{
				nIndPresetGame4[x][nFamily*5+nFound] = nCur;
				nIndLastPresetGame4[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame4[x] = nCountGame4;
			}
		}
		// Flash (FL)
		if(nComandGame4[x][0] == 2 &&
		   nComandGame4[x][1] == 3 &&
		   nComandGame4[x][2] == 1 &&
		   nComandGame4[x][3] == 4 &&
		   nComandGame4[x][4] == 3 &&
		   nComandGame4[x][5] == 0 &&
		   nComandGame4[x][6] == 4 &&
		   nComandGame4[x][7] == 1 &&
		   nComandGame4[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame4(x, nFamily, 12, 12);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame4[x][nFamily];
				nIndSizePresetGame4[x][nFamily]++;
				nIndPresetGame4[x][nFamily*5+nFound] = 12;
				nIndLastPresetGame4[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame4[x] = nCountGame4;
			}
			else
			{
				if(nIndPresetGame4[x][nFamily*5+nFound] == 12)	// Desactivar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
						nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
					nIndSizePresetGame4[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 12;
					nIndLastPresetGame4[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame4[x] = nCountGame4;
				}
			}
		}
		// Freedom (FD)
		if(nComandGame4[x][0] == 2 &&
		   nComandGame4[x][1] == 0 &&
		   nComandGame4[x][2] == 3 &&
		   nComandGame4[x][3] == 1 &&
		   nComandGame4[x][4] == 4 &&
		   nComandGame4[x][5] == 4 &&
		   nComandGame4[x][6] == 3 &&
		   nComandGame4[x][7] == 0 &&
		   nComandGame4[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame4(x, nFamily, 13, 13);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame4[x][nFamily];
				nIndSizePresetGame4[x][nFamily]++;
				nIndPresetGame4[x][nFamily*5+nFound] = 13;
				nIndLastPresetGame4[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame4[x] = nCountGame4;
			}
			else
			{
				if(nIndPresetGame4[x][nFamily*5+nFound] == 13)	// Desactivar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
						nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
					nIndSizePresetGame4[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 13;
					nIndLastPresetGame4[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame4[x] = nCountGame4;
				}
			}
		}
		// Reverse Grade (RG)
		if(nComandGame4[x][0] == 2 &&
		   nComandGame4[x][1] == 1 &&
		   nComandGame4[x][2] == 3 &&
		   nComandGame4[x][3] == 2 &&
		   nComandGame4[x][4] == 0 &&
		   nComandGame4[x][5] == 4 &&
		   nComandGame4[x][6] == 4 &&
		   nComandGame4[x][7] == 0 &&
		   nComandGame4[x][8] == 2 &&
		   nComandGame4[x][9] == 3 &&
		   nComandGame4[x][10] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame4(x, nFamily, 16, 16);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame4[x][nFamily];
				nIndSizePresetGame4[x][nFamily]++;
				nIndPresetGame4[x][nFamily*5+nFound] = 16;
				nIndLastPresetGame4[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame4[x] = nCountGame4;
			}
			else
			{
				if(nIndPresetGame4[x][nFamily*5+nFound] == 16)	// Desactivar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
						nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
					nIndSizePresetGame4[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 16;
					nIndLastPresetGame4[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame4[x] = nCountGame4;
				}
			}
		}
		// ** Quinto Nivel (4)
		nFamily = 4;
		// Snake (SN)
		if(nComandGame4[x][0] == 0 &&
		   nComandGame4[x][1] == 4 &&
		   nComandGame4[x][2] == 2 &&
		   nComandGame4[x][3] == 1 &&
		   nComandGame4[x][4] == 3 &&
		   nComandGame4[x][5] == 0 &&
		   nComandGame4[x][6] == 4 &&
		   nComandGame4[x][7] == 2 &&
		   nComandGame4[x][8] == 1 &&
		   nComandGame4[x][9] == 3)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame4(x, nFamily, 19, 19);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame4[x][nFamily];
				nIndSizePresetGame4[x][nFamily]++;
				nIndPresetGame4[x][nFamily*5+nFound] = 19;
				nIndLastPresetGame4[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame4[x] = nCountGame4;
			}
			else
			{
				if(nIndPresetGame4[x][nFamily*5+nFound] == 19)	// Desactivar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
						nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
					nIndSizePresetGame4[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 19;
					nIndLastPresetGame4[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame4[x] = nCountGame4;
				}
			}
		}
		// NX Mode (NX)
		if(nComandGame4[x][0] == 3 &&
		   nComandGame4[x][1] == 4 &&
		   nComandGame4[x][2] == 2 &&
		   nComandGame4[x][3] == 1 &&
		   nComandGame4[x][4] == 0 &&
		   nComandGame4[x][5] == 3 &&
		   nComandGame4[x][6] == 4 &&
		   nComandGame4[x][7] == 2 &&
		   nComandGame4[x][8] == 1 &&
		   nComandGame4[x][9] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame4(x, nFamily, 20, 20);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame4[x][nFamily];
				nIndSizePresetGame4[x][nFamily]++;
				nIndPresetGame4[x][nFamily*5+nFound] = 20;
				nIndLastPresetGame4[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame4[x] = nCountGame4;
			}
			else
			{
				if(nIndPresetGame4[x][nFamily*5+nFound] == 20)	// Desactivar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
						nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
					nIndSizePresetGame4[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 20;
					nIndLastPresetGame4[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame4[x] = nCountGame4;
				}
			}
		}
		// X Mode (X)
		if(nComandGame4[x][0] == 2 &&
		   nComandGame4[x][1] == 1 &&
		   nComandGame4[x][2] == 4 &&
		   nComandGame4[x][3] == 1 &&
		   nComandGame4[x][4] == 4 &&
		   nComandGame4[x][5] == 3 &&
		   nComandGame4[x][6] == 0 &&
		   nComandGame4[x][7] == 3 &&
		   nComandGame4[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame4(x, nFamily, 21, 21);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame4[x][nFamily];
				nIndSizePresetGame4[x][nFamily]++;
				nIndPresetGame4[x][nFamily*5+nFound] = 21;
				nIndLastPresetGame4[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame4[x] = nCountGame4;
			}
			else
			{
				if(nIndPresetGame4[x][nFamily*5+nFound] == 21)	// Desactivar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
						nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
					nIndSizePresetGame4[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 21;
					nIndLastPresetGame4[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame4[x] = nCountGame4;
				}
			}
		}
		// UnderAttack (UA)
		if(nComandGame4[x][0] == 2 &&
		   nComandGame4[x][1] == 3 &&
		   nComandGame4[x][2] == 2 &&
		   nComandGame4[x][3] == 0 &&
		   nComandGame4[x][4] == 2 &&
		   nComandGame4[x][5] == 0 &&
		   nComandGame4[x][6] == 2 &&
		   nComandGame4[x][7] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame4(x, nFamily, 22, 23);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame4[x][nFamily];
				nIndSizePresetGame4[x][nFamily]++;
				nIndPresetGame4[x][nFamily*5+nFound] = 22;
				nIndLastPresetGame4[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame4[x] = nCountGame4;
			}
			else
			{
				if(nIndPresetGame4[x][nFamily*5+nFound] == 22)	// Desactivar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
						nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
					nIndSizePresetGame4[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 22;
					nIndLastPresetGame4[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame4[x] = nCountGame4;
				}
			}
		}
		// Drop (DR)
		if(nComandGame4[x][0] == 4 &&
		   nComandGame4[x][1] == 4 &&
		   nComandGame4[x][2] == 3 &&
		   nComandGame4[x][3] == 0 &&
		   nComandGame4[x][4] == 1 &&
		   nComandGame4[x][5] == 4 &&
		   nComandGame4[x][6] == 3 &&
		   nComandGame4[x][7] == 0 &&
		   nComandGame4[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame4(x, nFamily, 22, 23);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame4[x][nFamily];
				nIndSizePresetGame4[x][nFamily]++;
				nIndPresetGame4[x][nFamily*5+nFound] = 23;
				nIndLastPresetGame4[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame4[x] = nCountGame4;
			}
			else
			{
				if(nIndPresetGame4[x][nFamily*5+nFound] == 23)	// Desactivar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame4[x][nFamily]-1); k++)
						nIndPresetGame4[x][nFamily*5+k] = nIndPresetGame4[x][nFamily*5+k+1];
					nIndSizePresetGame4[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame4[x][nFamily*5+nFound] = 23;
					nIndLastPresetGame4[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame4[x] = nCountGame4;
				}
			}
		}
		// Toggle Null
		if(nComandGame4[x][0] == 4 &&
		   nComandGame4[x][1] == 0 &&
		   nComandGame4[x][2] == 4 &&
		   nComandGame4[x][3] == 0 &&
		   nComandGame4[x][4] == 4 &&
		   nComandGame4[x][5] == 0)
		{
			bPreset = true;
			for(int z = 0; z < 30; z++) nIndPresetGame4[x][z] = 0;
			for(int z = 0; z < 6; z++) nIndSizePresetGame4[x][z] = 0;
			nIndLastPresetGame4[x] = -1;
			nTimeLastPresetGame4[x] = nCountGame4;
		}
		if(bPreset)
		{
			nChosesGame4 = 0;
			nComandGame4[x][0] = -1;
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_option, 0, &channels[2]);
		}
	}
}

#define TIME_FAST_CHANGE_GAME4 1000
#define TIME_CHANGE_LOW_GAME4 350
#define TIME_CHANGE_HIGH_GAME4 100

void PlaySoundTransiAboutLimitGame4(int nLimit)
{
	if(nLimit >= TIME_CHANGE_LOW_GAME4)
	{
		nTimeTransiSongBorderGame4 = TIME_TRANSI_SONGBORDER_GAME4;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
	}
	else
	{
		nTimeTransiSongBorderGame4 = TIME_CHANGE_HIGH_GAME4;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)6, sound_change2, 0, &channels[6]);
	}
}

int DrawInterfaceSelectGame4()
{
	if(!np[0] && !np[1]) return 1;
	nCountGame4 += dwTick-dwLastTick;
	if(movie_brainbg->bLoad)
	{
		movie_brainbg->Update(dwWait);
		movie_brainbg->GrabAVIFrame();
		glBindTexture(GL_TEXTURE_2D, movie_brainbg->tex);

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
	__int64 nTransBorder = nCountGame4 - nTimeCurSongGame4;
	int nOrder[] = {0,1,2,3,8,7,6,5,4};
	if(nChangeRetroGame4 == 0)
	{
		if(nChosesGame4 < CHOOSES_GAME4)
		{
			for(int i = 0; i < 9; i++)
			{
				int x = nOrder[i];
				double dTargetx = double(x-4)*MAX_TARGET_X/4;
				if(nCountGame4 >= TIME_START_SELECTOR_GAME4 && nCountGame4 < (TIME_START_SELECTOR_GAME4+TIME_TRANSI_SELECTOR_GAME4))
				{
					if(x == 4)
					{
						if(nCountGame4 < (TIME_TRANSI_START_MAIN_GAME4+TIME_START_SELECTOR_GAME4))
						{
							dApt = pow(double(nCountGame4-TIME_START_SELECTOR_GAME4)/TIME_TRANSI_START_MAIN_GAME4, 2);
							glPushMatrix();
							glTranslated(0.0, 0.0, -60.0);
							glScaled(1.25+(1-dApt), 1.25+(1-dApt), 1.0);
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
						if(nCountGame4 >= (TIME_TRANSI_START_MAIN_GAME4+TIME_START_SELECTOR_GAME4))
						{
							glPushMatrix();
							glTranslated(0.0, 0.0, -60.0);
							glScaled(1.25, 1.25, 1.0);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
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
						__int64 nStartTransi = TIME_START_SELECTOR_GAME4+TIME_TRANSI_SELECTOR_GAME4/4*(abs(x-4)-1);
						__int64 nLimitTransi = TIME_START_SELECTOR_GAME4+TIME_TRANSI_SELECTOR_GAME4/4*(abs(x-4));
						if(nCountGame4 >= nStartTransi && nCountGame4 < nLimitTransi)
						{
							dApt = double(nCountGame4-nStartTransi)/double(nLimitTransi-nStartTransi);
							double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(1.0-dApt)+dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXGame4(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForXGame4(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
						if(nCountGame4 >= nLimitTransi)
						{
							double dx = dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXGame4(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForXGame4(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
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
				if(nCountGame4 >= (TIME_START_SELECTOR_GAME4+TIME_TRANSI_SELECTOR_GAME4))
				{
					double dx = dTargetx;
					if(nModeGame4!=0)
					{
						if(nTransBorder < nTimeTransiSongBorderGame4)
						{
							dx += double(nModeGame4)*MAX_TARGET_X/4*(1.0-double(nTransBorder)/double(nTimeTransiSongBorderGame4));
						}
					}
					double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
					double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
					double dtheta = ThetaFuncForXGame4(dx);
					double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
					double dalpha = AlphaFuncForXGame4(dx);
					glPushMatrix();
					glTranslated(dx, dy, dz-60.0);
					glRotated(dtheta, 0.0, 1.0, 0.0);
					glScaled(dscale, dscale, 1.0);
					glColor4d(1.0, 1.0, 1.0, dalpha);
					if(x == 4 && nTransBorder > TIME_SHOW_PREVIEW_GAME4 && movie_songpreviewgame4->bLoad && movie_songpreviewgame4->bResize && bShow)
					{
	#define TIME_SHOWIMAGE_SONGBORDER_GAME4 500
						movie_songpreviewgame4->Update(dwWait);
						movie_songpreviewgame4->GrabAVIFrame();
						__int64 nCaretCover = movie_songpreviewgame4->dwNext;
						glBindTexture(GL_TEXTURE_2D, movie_songpreviewgame4->tex);

						glColor4d(1.0, 1.0, 1.0, 1.0);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
						glEnd();
						if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME4)
						{
							dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME4;
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
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
						glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
						glEnd();
					}
					glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					if(x == 4 && nTransBorder > TIME_SHOW_PREVIEW_GAME4)
					{
						#define TIME_SHOWNAME_BESTSCORE_GAME4 500
						if(nTransBorder < (TIME_SHOW_PREVIEW_GAME4+TIME_SHOWNAME_BESTSCORE_GAME4))
						{
							dApt = double(nTransBorder-TIME_SHOW_PREVIEW_GAME4)/TIME_SHOWNAME_BESTSCORE_GAME4;
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[166]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						if(nTransBorder >= (TIME_SHOW_PREVIEW_GAME4+TIME_SHOWNAME_BESTSCORE_GAME4))
						{
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[166]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
						}
						__int64 nModeTime = nCountGame4 - nTransBorder-TIME_SHOW_PREVIEW_GAME4;
						if(nModeTime >= 0 && nModeTime < (TIME_SHOWNAME_BESTSCORE_GAME4) && bShow)
						{
							dApt = double(nModeTime)/TIME_SHOWNAME_BESTSCORE_GAME4;
							glColor4d(1.0, 1.0, 1.0, dApt);
							if(nBestScoreMachineGame4 != 0)
							{
								font_Micro->glDrawText(10.599, -9.0625, 9.05331, 9.0625, 0.0, 10.076/*9.82615*/, 0.0, 1.0, 1.5, strBestScoreMachineGame4, DT_CENTER|DT_VCENTER);
								CString str;
								str.Format(_T("%d (O: %d)"), nBestScoreMachineGame4, nBestScoreMachineGame4Ex);
								font_Micro->glDrawText(9.05331, -9.0625, 7.50762, 9.0625, 0.0, 8.53046/*8.28046*/, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
							}
							for(int k = 0; k < 2; k++) if(np[k])
							{
								if(nBestScorePlayerGame4[k] != 0)
								{
									CString str;
									str.Format(_T("%d (O: %d)"), nBestScorePlayerGame4[k], nBestScorePlayerGame4Ex[k]);
									font_Micro->glDrawText(-9.05331, -9.0625+(k?7.25:-7.25), -10.599, 9.0625+(k?7.25:-7.25), (k?7.25:-7.25), -9.57615, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
								}
							}
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						if(nModeTime >= (TIME_SHOWNAME_BESTSCORE_GAME4) && bShow)
						{
							if(nBestScoreMachineGame4 != 0)
							{
								font_Micro->glDrawText(10.599, -9.0625, 9.05331, 9.0625, 0.0, 10.076/*9.82615*/, 0.0, 1.0, 1.5, strBestScoreMachineGame4, DT_CENTER|DT_VCENTER);
								CString str;
								str.Format(_T("%d (O: %d)"), nBestScoreMachineGame4, nBestScoreMachineGame4Ex);
								font_Micro->glDrawText(9.05331, -9.0625, 7.50762, 9.0625, 0.0, 8.53046/*8.28046*/, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
							}
							for(int k = 0; k < 2; k++) if(np[k])
							{
								if(nBestScorePlayerGame4[k] != 0)
								{
									CString str;
									str.Format(_T("%d (O: %d)"), nBestScorePlayerGame4[k], nBestScorePlayerGame4Ex[k]);
									font_Micro->glDrawText(-9.05331, -9.0625+(k?7.25:-7.25), -10.599, 9.0625+(k?7.25:-7.25), (k?7.25:-7.25), -9.57615, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
								}
							}
						}
					}
					glPopMatrix();
				}
			}
			if(nTransBorder >= 0 && nTransBorder <= TIME_GLOUTH_SONGBORDER_GAME4 && nChosesGame4 == 0)
			{
				dApt = double(nTransBorder)/TIME_GLOUTH_SONGBORDER_GAME4;
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[11]->GetCurrentTex());
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
			if(nTransBorder > TIME_TRANSI_SONGBORDER_GAME4 && nTransBorder <= (TIME_TRANSI_SONGBORDER_GAME4+TIME_GLOUTH_SONGBORDER_GAME4) && nChosesGame4 == 0)
			{
				dApt = double(nTransBorder-TIME_TRANSI_SONGBORDER_GAME4)/TIME_GLOUTH_SONGBORDER_GAME4;
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[11]->GetCurrentTex());
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
	#define TIME_GLOUTH_PER_SONGBORDER_GAME4 1500
			if(nTransBorder > (TIME_TRANSI_SONGBORDER_GAME4+TIME_GLOUTH_SONGBORDER_GAME4) && nChosesGame4 == 0)
			{
				dApt = abs(double((nTransBorder-TIME_TRANSI_SONGBORDER_GAME4-TIME_GLOUTH_SONGBORDER_GAME4)%TIME_GLOUTH_PER_SONGBORDER_GAME4-TIME_GLOUTH_PER_SONGBORDER_GAME4/2))*2/TIME_GLOUTH_PER_SONGBORDER_GAME4;
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[11]->GetCurrentTex());
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
			if(nTransBorder > (TIME_TRANSI_SONGBORDER_GAME4+TIME_GLOUTH_SONGBORDER_GAME4) && nChosesGame4 == 1)
			{
				welres->skin[0]->Update(dwWait);
				welres->skin[1]->Update(dwWait);
				welres->skin[3]->Update(dwWait);
				dApt = double((nCountGame4-nTimeChosesGame4[nChosesGame4-1])%TIME_GLOUTH_SONGBORDER_GAME4)/TIME_GLOUTH_SONGBORDER_GAME4;
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[11]->GetCurrentTex());
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
			}
			if(nTransBorder >= TIME_SHOW_PREVIEW_GAME4)
			{
				if(!bShow)
				{
					TCHAR chFile[260];
					_sprintf(chFile, _T("song/%ls/%ls_P.avi"), chSongsGame4[nCurSongGame4], chSongsGame4[nCurSongGame4]);
					if(movie_songpreviewgame4->bLoad)
					{
						movie_songpreviewgame4->CloseAVI();
						delete movie_songpreviewgame4;
						movie_songpreviewgame4 = new CMovieStream2();
					}
					movie_songpreviewgame4->OpenAVI(chFile);
					movie_songpreviewgame4->bLoop = true;
					nTimeShow = nCountGame4;
					bShow = true;
					_sprintf(chFile, _T("song/%ls/%ls_P.mp3"), chSongsGame4[nCurSongGame4], chSongsGame4[nCurSongGame4]);
					if(bLoadMusicCoverGame4)
					{
						bLoadMusicCoverGame4 = false;
						FMOD_Sound_Release(music_coverGame4);
					}
					FMOD_RESULT resc;
	#ifdef UNICODE
					char chAnsiFile[260];
					UnicodeToAnsi(chFile, chAnsiFile);
					resc = FMOD_System_CreateStream(sys, chAnsiFile, FMOD_HARDWARE | FMOD_2D, 0, &music_coverGame4);
	#else
					resc = FMOD_System_CreateStream(sys, chFile, FMOD_HARDWARE | FMOD_2D, 0, &music_coverGame4);
	#endif
					if(ERRCHECK(resc))
					{
						bLoadMusicCoverGame4 = true;
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, music_coverGame4, 0, &channels[9]);
					}
					else
					{
						bLoadMusicCoverGame4 = false;
						_printf(_T("No se puede cargar el archivo \"%ls\"\n"), chFile);
					}
					strDispGame4.Format(_T("%ls - %ls - %ls"), GetNameSongsGame4(nCurSongGame4), GetArtistSongsGame4(nCurSongGame4), GetBPMSongsGame4(nCurSongGame4));
					for(int k = 0; k < 2; k++) if(np[k])
					{
						GetLevelSongsGame4(nCurSongGame4, &nLevelCurGame4[k],
						&dDCurGame4[k][0],
						&dDCurGame4[k][1],
						&dDCurGame4[k][2],
						&dDCurGame4[k][3],
						&dDCurGame4[k][4]);
					}
					GetBestScoresGame4(nCurSongGame4);
				}
			}



	#define TIME_START_UPDO_GAME4 0
	#define TIME_TRANSI_UPDO_GAME4 500
	#define TIME_GLOUTH_UPDO_GAME4 500
			if(nCountGame4 >= TIME_START_UPDO_GAME4 && nCountGame4 < (TIME_START_UPDO_GAME4+TIME_TRANSI_UPDO_GAME4))	// Transi Up and Down Game4
			{
				dApt = (double(nCountGame4)-TIME_START_UPDO_GAME4)/(TIME_TRANSI_UPDO_GAME4);
				glPushMatrix();
				glScaled(1.0+0.8*(1.0-dApt), 1.0+0.8*(1.0-dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[41+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[23+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[3]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[4]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[5]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[6]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625, -29.75, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nCountGame4 >= (TIME_START_UPDO_GAME4+TIME_TRANSI_UPDO_GAME4))	// Up and Down Game4
			{
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[41+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				if(nTransBorder > TIME_SHOW_PREVIEW_GAME4 && bShow)
				{
	#define SIZE_TEXT_GAME4 1.0
	#define VEL_SHOWTEXT_GAME4 0.01
	#define TIME_WAITTEXTNAME_GAME4 1500
					__int64 nTimeName = nTransBorder-TIME_SHOW_PREVIEW_GAME4-TIME_WAITTEXTNAME_GAME4;
					if(nTimeName < 0) nTimeName = 0;
					double dSizeTex = font_Car->CalcSingleLine(strDispGame4, SIZE_TEXT_GAME4);
					DWORD t1 = DWORD(double(18.75/VEL_SHOWTEXT_GAME4));
					DWORD t2 = DWORD(double(dSizeTex/VEL_SHOWTEXT_GAME4));
					DWORD t = t1+t2;
					DWORD nShowText = (DWORD(nTimeName)+t1)%t;
					double dx = -VEL_SHOWTEXT_GAME4*double(nShowText)+18.75;
					font_Car->glDrawText(-22.25, -9.375, -23.75, 9.375, -8.375+dx, -23, -50.0, 1.0, SIZE_TEXT_GAME4, strDispGame4, DT_VCENTER);
				}
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[23+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();

				DWORD dwNext = 0;
				DWORD dwFrame = 0;

				dwNext = (DWORD)(nCountGame4-max(nTimeEnArrowsGame4[1],nTimeEnArrowsGame4[6]));
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame4)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga4res->skin[3]->m_nSize)/(DWORD)nTimeTransiSongBorderGame4;
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[3]->texture[dwFrame]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame4-max(nTimeEnArrowsGame4[3],nTimeEnArrowsGame4[8]));
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame4)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga4res->skin[4]->m_nSize)/(DWORD)nTimeTransiSongBorderGame4;
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[4]->texture[dwFrame]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame4-max(nTimeEnArrowsGame4[0],nTimeEnArrowsGame4[5]));
				if(bEnArrowsGame4[0] || bEnArrowsGame4[5]) dwNext = dwNext%nTimeTransiSongBorderGame4;
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame4)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga4res->skin[5]->m_nSize)/(DWORD)nTimeTransiSongBorderGame4;
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[5]->texture[dwFrame]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame4-max(nTimeEnArrowsGame4[4],nTimeEnArrowsGame4[9]));
				if(bEnArrowsGame4[4] || bEnArrowsGame4[9]) dwNext = dwNext%nTimeTransiSongBorderGame4;
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame4)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga4res->skin[6]->m_nSize)/(DWORD)nTimeTransiSongBorderGame4;
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[6]->texture[dwFrame]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625, -29.75, -50.0);
				glEnd();

	#define TIME_TRANSI_SELECTORMODE_GAME4 300
	#define TIME_START_GRAPH_GAME4 100
	#define TIME_TRANSI_GRAPH_GAME4 500
	#define TIME_GLOUTH_GRAPH_GAME4 300
	#define TIME_START_LINEGRAPH_GAME4 100
	#define TIME_TRANSI_LINEGRAPH_GAME4 500
	#define TIME_TRUNC_LINEGRAPH_GAME4 1200
				/*Dibujo de la elección*/
				for(int k = 0; k < 2; k++) if(np[k])
				{
					__int64 nTransSelectM = nTransBorder;
					/*
					double dx = double(nModeCurSongGame4[k])*3.725-7.45+(k?0.465625:-0.465625);
					double dy = abs(double((nCountGame4-(TIME_START_UPDO_GAME4+TIME_TRANSI_UPDO_GAME4)+(k?250:0))%500)/500-0.5)*2;
					if(nTransSelectM >= 0 && nTransSelectM < TIME_TRANSI_SELECTORMODE_GAME4)
					{
						dx+=(double(nBackModeCurSongGame4[k])-double(nModeCurSongGame4[k]))*3.725*(1.0-pow(double(nTransSelectM)/TIME_TRANSI_SELECTORMODE_GAME4,0.5));
					}
					glBindTexture(GL_TEXTURE_2D, ga4res->skin[99+k]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d( -1.396875+dx, 26.69453125+1.04765625+dy, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  1.396875+dx, 26.69453125+1.04765625+dy, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  1.396875+dx, 26.69453125-1.04765625+dy, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -1.396875+dx, 26.69453125-1.04765625+dy, -50.0);
					glEnd();*/

	#define TIME_START_LR_GAME4 0
	#define TIME_TRANSI_LR_GAME4 500
	#define TIME_GLOUTH_LR_GAME4 500
					__int64 nCountSide = nCountGame4-(TIME_START_UPDO_GAME4+TIME_TRANSI_UPDO_GAME4);
					if(nCountSide >= TIME_START_LR_GAME4 && nCountSide <= (TIME_START_LR_GAME4+TIME_TRANSI_LR_GAME4))
					{
						dApt = pow(double(nCountSide-TIME_START_LR_GAME4)/TIME_TRANSI_LR_GAME4, 0.5);
						glPushMatrix();
						glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(1-dApt), 0.0, 0.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBindTexture(GL_TEXTURE_2D, ga4res->skin[(k?68:50)+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
	#define TIME_TRANSI_PRESET_GAME4 500
	#define TIME_GLOUTH_PRESET_GAME4 500
	#define TIME_SHOW_PRESET_GAME4 1500
					if(nCountSide > (TIME_START_LR_GAME4+TIME_TRANSI_LR_GAME4))
					{
						glPushMatrix();
						glTranslated(k?40.0:-40.0, 0.0, 0.0);
						glBindTexture(GL_TEXTURE_2D, ga4res->skin[(k?68:50)+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
						glEnd();
						glPopMatrix();
						__int64 nCountPreset = nCountGame4-nTimeLastPresetGame4[k];
						for(int q = 0; q < 6; q++)
						{
							double dAdv = 0.0;
							for(int w = 0; w < nIndSizePresetGame4[k][q]; w++)
							{
								int nInd = (q*5+w);
								if(nInd == nIndLastPresetGame4[k] || nIndLastPresetGame4[k] == -1)
								{
									__int64 nCountPreset = nCountGame4-nTimeLastPresetGame4[k];
									if(nCountPreset >= 0 && nCountPreset < TIME_TRANSI_PRESET_GAME4)
									{
										glPushMatrix();
										dApt = double(nCountPreset)/TIME_TRANSI_PRESET_GAME4;
										glLoadIdentity();
										glColor4d(1.0, 1.0, 1.0, dApt);
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
										glBindTexture(GL_TEXTURE_2D, ga4res->skin[108+nIndPresetGame4[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
										glScaled(1.0+2.0*(1-dApt), 1.0, 1.0);
										glBindTexture(GL_TEXTURE_2D, ga4res->skin[108+nIndPresetGame4[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
										glBindTexture(GL_TEXTURE_2D, ga4res->skin[106]->GetCurrentTex());
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
										glBindTexture(GL_TEXTURE_2D, ga4res->skin[104]->GetCurrentTex());
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
									if(nCountPreset >= TIME_TRANSI_PRESET_GAME4 && nCountPreset < (TIME_TRANSI_PRESET_GAME4+TIME_SHOW_PRESET_GAME4))
									{
										glPushMatrix();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga4res->skin[108+nIndPresetGame4[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga4res->skin[108+nIndPresetGame4[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga4res->skin[106]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										if(nCountPreset < (TIME_TRANSI_PRESET_GAME4+TIME_GLOUTH_PRESET_GAME4))
										{
											dApt = double(nCountPreset - TIME_TRANSI_PRESET_GAME4)/TIME_GLOUTH_PRESET_GAME4;
											glLoadIdentity();
											glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
											glScaled(dApt*4.0, dApt*4.0, 1.0);
											glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
											glBindTexture(GL_TEXTURE_2D, ga4res->skin[103]->GetCurrentTex());
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
									if(nCountPreset >= (TIME_TRANSI_PRESET_GAME4+TIME_SHOW_PRESET_GAME4))
									{
										glPushMatrix();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga4res->skin[108+nIndPresetGame4[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME4+TIME_SHOW_PRESET_GAME4))
										{
											dApt = double(nCountPreset-(TIME_TRANSI_PRESET_GAME4+TIME_SHOW_PRESET_GAME4))/TIME_TRANSI_PRESET_GAME4;
											glLoadIdentity();
											glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
											glScaled(1.0+2.0*(dApt), 1.0, 1.0);
											glColor4d(1.0, 1.0, 1.0, 1-dApt);
											glBindTexture(GL_TEXTURE_2D, ga4res->skin[108+nIndPresetGame4[k][nInd]]->GetCurrentTex());
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
										glBindTexture(GL_TEXTURE_2D, ga4res->skin[106]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glPopMatrix();
										if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME4+TIME_SHOW_PRESET_GAME4)) dAdv += (1-dApt)*9.0;
										dAdv += 4.5;
									}
								}
								else
								{
									glPushMatrix();
									glLoadIdentity();
									glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga4res->skin[108+nIndPresetGame4[k][nInd]]->GetCurrentTex());
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
									glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
									glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
									glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
									glEnd();
									glLoadIdentity();
									glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga4res->skin[106]->GetCurrentTex());
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

					/*Dibujo del Graph*/

					if(nTransSelectM >= TIME_START_GRAPH_GAME4 && nTransSelectM < (TIME_START_GRAPH_GAME4+TIME_TRANSI_GRAPH_GAME4))
					{
						dApt = pow(double(nTransSelectM - TIME_START_GRAPH_GAME4)/TIME_TRANSI_GRAPH_GAME4, 0.5);
						glPushMatrix();
						glTranslated((k?30.0:-30.0)+(k?1:-1)*17.5*(1-dApt), k?-20.0:20.0, 0.0);
						glScaled(0.5+0.5*dApt, 0.5+0.5*dApt, 1.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						for(int l = 0; l < 6; l++)
						{
							double da = 0.0, de = 0.0;
							if(l == 0) da = dDCurGame4[k][0];
							else da = dDCurGame4[k][l-1];
							if(l == 5) de = dDCurGame4[k][4];
							else de = dDCurGame4[k][l];
							double dx = 0.46875+2.34375*double(l)-7.5;
							double dy = -3.75;
							double dy1 = 9.375*da-3.75;
							double dy2 = 9.375*de-3.75;
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[10]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(double(l)/6, 1.0-da); glVertex3d( dx,  dy1, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0-de); glVertex3d(  dx+2.34375,  dy2, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0); glVertex3d(  dx+2.34375, dy, -50.0);
							glTexCoord2d(double(l)/6, 1.0); glVertex3d( dx, dy, -50.0);
							glEnd();
						}
						glBindTexture(GL_TEXTURE_2D, ga4res->skin[7]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  7.5, -7.5, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -7.5, -7.5, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nTransSelectM >= (TIME_START_GRAPH_GAME4+TIME_TRANSI_GRAPH_GAME4))
					{
						glPushMatrix();
						glTranslated(k?30.0:-30.0, k?-20.0:20.0, 0.0);
						for(int l = 0; l < 6; l++)
						{
							double da = 0.0, de = 0.0;
							if(l == 0) da = dDCurGame4[k][0];
							else da = dDCurGame4[k][l-1];
							if(l == 5) de = dDCurGame4[k][4];
							else de = dDCurGame4[k][l];
							double dx = 0.46875+2.34375*double(l)-7.5;
							double dy = -3.75;
							double dy1 = 9.375*da-3.75;
							double dy2 = 9.375*de-3.75;
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[10]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(double(l)/6, 1.0-da); glVertex3d( dx,  dy1, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0-de); glVertex3d(  dx+2.34375,  dy2, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0); glVertex3d(  dx+2.34375, dy, -50.0);
							glTexCoord2d(double(l)/6, 1.0); glVertex3d( dx, dy, -50.0);
							glEnd();
						}
						glBindTexture(GL_TEXTURE_2D, ga4res->skin[7]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  7.5, -7.5, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -7.5, -7.5, -50.0);
						glEnd();
						__int64 nTransGraphLine = (nTransSelectM - (TIME_START_GRAPH_GAME4+TIME_TRANSI_GRAPH_GAME4))%TIME_TRUNC_LINEGRAPH_GAME4;
						if(nTransGraphLine >= TIME_START_LINEGRAPH_GAME4 && nTransGraphLine < (TIME_START_LINEGRAPH_GAME4+TIME_TRANSI_LINEGRAPH_GAME4))
						{
							double t = double(nTransGraphLine)-TIME_START_LINEGRAPH_GAME4;
							double dx=0.46875+14.0625*t/TIME_TRANSI_LINEGRAPH_GAME4-7.5;
							double dy=0.0;
							double da=0.0;
							if(t<(TIME_TRANSI_LINEGRAPH_GAME4/6))
							{
								da=t*6/TIME_TRANSI_LINEGRAPH_GAME4;
								dy=9.375*dDCurGame4[k][0]-3.75;
							}
							if(t>=(TIME_TRANSI_LINEGRAPH_GAME4/6) && t < (5*TIME_TRANSI_LINEGRAPH_GAME4/6))
							{
								da = 1.0;
								int n1 = int((t-TIME_TRANSI_LINEGRAPH_GAME4/6)*6/TIME_TRANSI_LINEGRAPH_GAME4);
								int n2 = int((t-TIME_TRANSI_LINEGRAPH_GAME4/6)*6/TIME_TRANSI_LINEGRAPH_GAME4)+1;
								double tl = double(n2)*TIME_TRANSI_LINEGRAPH_GAME4/6;
								dy = 9.375*((t-tl)*6/TIME_TRANSI_LINEGRAPH_GAME4*(dDCurGame4[k][n2]-dDCurGame4[k][n1])+dDCurGame4[k][n1])-3.75;
							}
							if(t>(5*TIME_TRANSI_LINEGRAPH_GAME4/6))
							{
								da=1.0-(t-5*TIME_TRANSI_LINEGRAPH_GAME4/6)*6/TIME_TRANSI_LINEGRAPH_GAME4;
								dy=9.375*dDCurGame4[k][4]-3.75;
							}
							glTranslated(dx, dy, 0.0);
							glColor4d(1.0, 1.0, 1.0, da);
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[9]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -1.0,  1.0, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  1.0,  1.0, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  1.0, -1.0, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -1.0, -1.0, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						glPopMatrix();
						if(nTransSelectM < (TIME_START_GRAPH_GAME4+TIME_TRANSI_GRAPH_GAME4+TIME_GLOUTH_GRAPH_GAME4))
						{
							dApt = pow(double(nTransSelectM-TIME_START_GRAPH_GAME4-TIME_TRANSI_GRAPH_GAME4)/TIME_GLOUTH_GRAPH_GAME4, 0.5);
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[8]->GetCurrentTex());
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

	#define TIME_START_DIFI_GAME4 600
	#define TIME_TRANSI_DIFI_GAME4 500
	#define TIME_GLOUTH_DIFI_GAME4 500
	#define TIME_CONTROT_DIFI_GAME4 1000
					/*Dibujo del nivel*/
					int nFilterLevel = 0;
					int nItemsActive = 0;
					int nIndexActive = 0;
					if(nLevelCurGame4[k] <= 0 || nLevelCurGame4[k] > ga4res->skin[101]->m_nSize) nFilterLevel = ga4res->skin[101]->m_nSize;
					else nFilterLevel = nLevelCurGame4[k];
					if(nFilterLevel >= 1 && nFilterLevel <= 30)
					{
						nItemsActive = (nFilterLevel-1)%8+1;
						nIndexActive = (nFilterLevel-1)/8;
					}
					if(player_sesion[k]->nLevel <= (nLevelCurGame4[k]-3) && !player_sesion[k]->nLockedAsp[ITEM_SUPRIMELVL]) nIndexActive = 4;
					double dAngleDif = double(nTransSelectM%TIME_CONTROT_DIFI_GAME4)/TIME_CONTROT_DIFI_GAME4*360;
					if(nTransSelectM >= TIME_START_DIFI_GAME4 && nTransSelectM < (TIME_START_DIFI_GAME4+TIME_TRANSI_DIFI_GAME4))
					{
						dApt = double(nTransSelectM - TIME_START_DIFI_GAME4)/TIME_TRANSI_DIFI_GAME4;
						glPushMatrix();
						if(np[k] && !np[int(!k)])
						{
							double dWidth = 18.75;
							double dSizeItem = dWidth/8;
							double dStart = -dSizeItem*4+dSizeItem/2;
							glLoadIdentity();
							glTranslated(0.0, -26.375, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[102]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -9.75,  1.21875, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  9.75,  1.21875, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  9.75, -1.21875, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -9.75, -1.21875, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, dApt);
							if(nIndexActive < 3)
							{
								for(int q = 0; q < 8; q++)
								{
									double dx = dSizeItem*double(q)+dStart;
									glLoadIdentity();
									glTranslated(dx, -26.375, 0.0);
									glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
									glBindTexture(GL_TEXTURE_2D, ga4res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glTranslated(0.0, -26.375, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[107]->GetCurrentTex());
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
								glTranslated(0.0, -26.375, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[108]->GetCurrentTex());
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
							glTranslated(k?4.75:-4.75, -26.375, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[102]->GetCurrentTex());
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
									glTranslated(dx, -26.375, 0.0);
									glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
									glBindTexture(GL_TEXTURE_2D, ga4res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glTranslated(k?4.75:-4.75, -26.375, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[107]->GetCurrentTex());
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
								glTranslated(k?4.75:-4.75, -26.375, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[108]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
						}
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, -26.375, 0.0);
						glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBindTexture(GL_TEXTURE_2D, ga4res->skin[101]->texture[nFilterLevel-1]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  0.875, -0.875, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -0.875, -0.875, -50.0);
						glEnd();
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, -26.375, 0.0);
						glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
						glRotated(dAngleDif, 0.0, 0.0, 1.0);
						glBindTexture(GL_TEXTURE_2D, ga4res->skin[106]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, -26.375, 0.0);
						glScaled((1-dApt)*4.0, (1-dApt)*4.0, 1.0);
						glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
						glBindTexture(GL_TEXTURE_2D, ga4res->skin[104]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nTransSelectM >= (TIME_START_DIFI_GAME4+TIME_TRANSI_DIFI_GAME4))
					{
						glPushMatrix();
						if(np[k] && !np[int(!k)])
						{
							double dWidth = 18.75;
							double dSizeItem = dWidth/8;
							double dStart = -dSizeItem*4+dSizeItem/2;
							glLoadIdentity();
							glTranslated(0.0, -26.375, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[102]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -9.75,  1.21875, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  9.75,  1.21875, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  9.75, -1.21875, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -9.75, -1.21875, -50.0);
							glEnd();
							if(nIndexActive < 3)
							{
								for(int q = 0; q < 8; q++)
								{
									double dx = dSizeItem*double(q)+dStart;
									glLoadIdentity();
									glTranslated(dx, -26.375, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga4res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glTranslated(0.0, -26.375, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[107]->GetCurrentTex());
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
								glTranslated(0.0, -26.375, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[108]->GetCurrentTex());
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
							glTranslated(k?4.75:-4.75, -26.375, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[102]->GetCurrentTex());
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
									glTranslated(dx, -26.375, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga4res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glTranslated(k?4.75:-4.75, -26.375, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[107]->GetCurrentTex());
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
								glTranslated(k?4.75:-4.75, -26.375, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[108]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
						}
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, -26.375, 0.0);
						glBindTexture(GL_TEXTURE_2D, ga4res->skin[101]->texture[nFilterLevel-1]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  0.875, -0.875, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -0.875, -0.875, -50.0);
						glEnd();
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, -26.375, 0.0);
						glRotated(dAngleDif, 0.0, 0.0, 1.0);
						glBindTexture(GL_TEXTURE_2D, ga4res->skin[106]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						if(nTransSelectM < (TIME_START_DIFI_GAME4+TIME_TRANSI_DIFI_GAME4+TIME_GLOUTH_DIFI_GAME4))
						{
							dApt = double(nTransSelectM - TIME_START_DIFI_GAME4 -TIME_TRANSI_DIFI_GAME4)/TIME_GLOUTH_DIFI_GAME4;
							glLoadIdentity();
							glTranslated(k?10.875:-10.875, -26.375, 0.0);
							glScaled(dApt*4.0, dApt*4.0, 1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[103]->GetCurrentTex());
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
					if(bSkinGame4[k]) DrawSkinsGame4(k);
					if(bCharacterGame4[k]) DrawCharactersGame4(k);
					glPopMatrix();
				}

				__int64 nTrans1 = nCountGame4 - nTimeChaGame4;
				if(nTrans1 <= TIME_GLOUTH_UPDO_GAME4)
				{
					dApt = (double(nTrans1))/(TIME_GLOUTH_UPDO_GAME4);
					glBindTexture(GL_TEXTURE_2D, ga4res->skin[32+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
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
				strDispTime.Format(_T("%.2d"), piuconfig.nModeMach==1?99:int((nMaxGame4 - nCountGame4)/1000));
				font_Micro->glDrawText(30.0, -40.0, -30.0, 40.0, 0, 17.75, -50.0, 1.0, 1.5, strDispTime, DT_VCENTER|DT_CENTER);
			}
			if(nCountGame4 >= TIME_START_START_GAME4)	// Reconocimiento de comandos
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
								if(bArrows[y] && !bEnArrowsGame4[y])
								{
									bEnArrowsGame4[y] = true;
									nTimeEnArrowsGame4[y] = nCountGame4;
									for(int q = (MAX_COMMANDS_GAME4-2); q >= 0; q--)
									{
										nComandGame4[x][q+1] = nComandGame4[x][q];
									}
									if(!bSkinGame4[x] && !bCharacterGame4[x])
									{
										nComandGame4[x][0] = y;
										bNewCommand = true;
									}
								}
								if(!bArrows[y] && bEnArrowsGame4[y])
								{
									bEnArrowsGame4[y] = false;
									bCh[y] = false;
									if(y == 0 || y == 4)
									{
										nTimeTransiSongBorderGame4 = TIME_TRANSI_SONGBORDER_GAME4;
									}
								}
							}
						}
						if(x == 1)
						{
							for(int y = 5; y < 10; y++)
							{
								if(bArrows[y] && !bEnArrowsGame4[y])
								{
									bEnArrowsGame4[y] = true;
									nTimeEnArrowsGame4[y] = nCountGame4;
									for(int q = (MAX_COMMANDS_GAME4-2); q >= 0; q--)
									{
										nComandGame4[x][q+1] = nComandGame4[x][q];
									}
									if(!bSkinGame4[x] && !bCharacterGame4[x])
									{
										nComandGame4[x][0] = y-5;
										bNewCommand = true;
									}
								}
								if(!bArrows[y] && bEnArrowsGame4[y])
								{
									bEnArrowsGame4[y] = false;
									bCh[y] = false;
									if(y == 5 || y == 9)
									{
										nTimeTransiSongBorderGame4 = TIME_TRANSI_SONGBORDER_GAME4;
									}
								}
							}
						}
					}
					else
					{
						if(x == 0 && bArrows[2] && nCredit)
						{
							PlusStagePlayer(0);
							nCredit--;
							nChangeRetroGame4 = 2;
							nTimeChangeRetroGame4 = nCountGame4;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
							for(int y = 0; y < 10; y++)
							{
								bArrows[y] = false;
								bEnArrowsGame4[y] = false;
								bCh[y] = false;
							}
							break;
						}
						if(x == 1 && bArrows[7] && nCredit)
						{
							PlusStagePlayer(1);
							nCredit--;
							nChangeRetroGame4 = 2;
							nTimeChangeRetroGame4 = nCountGame4;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
							for(int y = 0; y < 10; y++)
							{
								bArrows[y] = false;
								bEnArrowsGame4[y] = false;
								bCh[y] = false;
							}
							break;
						}
					}
				}
				if(bEnArrowsGame4[0])
				{
					if(bSkinGame4[0] && !bCh[0])
					{
						TurnSkinsGame4(true, 0);
						bCh[0] = true;
					}
					else if(bCharacterGame4[0] && !bCh[0])
					{
						TurnCharactersGame4(true, 0);
						bCh[0] = true;
					}
					else
					{
						__int64 nCaret = nCountGame4 - nTimeEnArrowsGame4[0];
						int nLimit = min(nCaret,nCountGame4-nTimeChaGame4)>TIME_FAST_CHANGE_GAME4?TIME_CHANGE_HIGH_GAME4:TIME_CHANGE_LOW_GAME4;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[0])
						{
							bCh[0] = true;
							MoveSongsGame4(true);
							PlaySoundTransiAboutLimitGame4(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[0] = false;
						}
					}
				}
				if(bEnArrowsGame4[4])
				{
					if(bSkinGame4[0] && !bCh[4])
					{
						TurnSkinsGame4(false, 0);
						bCh[4] = true;
					}
					else if(bCharacterGame4[0] && !bCh[4])
					{
						TurnCharactersGame4(false, 0);
						bCh[4] = true;
					}
					else
					{
						__int64 nCaret = nCountGame4 - nTimeEnArrowsGame4[4];
						int nLimit = min(nCaret,nCountGame4-nTimeChaGame4)>TIME_FAST_CHANGE_GAME4?TIME_CHANGE_HIGH_GAME4:TIME_CHANGE_LOW_GAME4;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[4])
						{
							bCh[4] = true;
							MoveSongsGame4(false);
							PlaySoundTransiAboutLimitGame4(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[4] = false;
						}
					}
				}
				if(bEnArrowsGame4[1] && !bCh[1])
				{
					if(!bSkinGame4[0] && !bCharacterGame4[0])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame4 = 0;
						bool bLoopLook = true;
						int nIndexLook = nCurSongGame4;
						while(bLoopLook)
						{
							nIndexLook--;
							if(nIndexLook < 0) nIndexLook = nSizeSongsGame4-1;
							if(nChaSongsGame4[nIndexLook] != nChaSongsGame4[nCurSongGame4] || nIndexLook == nCurSongGame4)
								bLoopLook = false;
						}
						nCurSongGame4 = nIndexLook;
						nTimeChaGame4 = nCountGame4;
						glDeleteTextures(9, nTexCurSongGame4);
						ActualizateIndexShowGame4();
						for(int x = 0; x < 9; x++)
						{
							nTexCurSongGame4[x] = 0;
							TCHAR chFile[260];
							_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsGame4[nIndexShowGame4[x]]);
							nTexCurSongGame4[x] = GenTexImage(chFile);
						}
						bCh[1] = true;
						GetLevelSongsGame4(nCurSongGame4, &nLevelCurGame4[0],
						&dDCurGame4[0][0],
						&dDCurGame4[0][1],
						&dDCurGame4[0][2],
						&dDCurGame4[0][3],
						&dDCurGame4[0][4]);
						GetBestScoresGame4(nCurSongGame4);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
				}
				if(bEnArrowsGame4[3] && !bCh[3])
				{
					if(!bSkinGame4[0] && !bCharacterGame4[0])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame4 = 0;
						bool bLoopLook = true;
						int nIndexLook = nCurSongGame4;
						while(bLoopLook)
						{
							nIndexLook++;
							if(nIndexLook >= nSizeSongsGame4) nIndexLook = 0;
							if(nChaSongsGame4[nIndexLook] != nChaSongsGame4[nCurSongGame4] || nIndexLook == nCurSongGame4)
								bLoopLook = false;
						}
						nCurSongGame4 = nIndexLook;
						nTimeChaGame4 = nCountGame4;
						glDeleteTextures(9, nTexCurSongGame4);
						ActualizateIndexShowGame4();
						for(int x = 0; x < 9; x++)
						{
							nTexCurSongGame4[x] = 0;
							TCHAR chFile[260];
							_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsGame4[nIndexShowGame4[x]]);
							nTexCurSongGame4[x] = GenTexImage(chFile);
						}
						bCh[3] = true;
						GetLevelSongsGame4(nCurSongGame4, &nLevelCurGame4[0],
						&dDCurGame4[0][0],
						&dDCurGame4[0][1],
						&dDCurGame4[0][2],
						&dDCurGame4[0][3],
						&dDCurGame4[0][4]);
						GetBestScoresGame4(nCurSongGame4);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
				}
				if(bEnArrowsGame4[2] && !bCh[2])
				{
					bCh[2] = true;
					if(np[0])
					{
						if(bSkinGame4[0])
						{
							MakeChoiceSkinGame4(0);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else if(bCharacterGame4[0])
						{
							MakeChoiceCharacterGame4(0);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else
						{
							nChosesGame4++;
							nTimeChosesGame4[nChosesGame4-1] = nCountGame4;
							if(nChosesGame4 > 0 && nChosesGame4 < CHOOSES_GAME4) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
							if(nChosesGame4 >= CHOOSES_GAME4)
							{
								FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
								for(int x = 0; x < 10; x++) bCh[x] = false;
							}
						}
					}
				}
	/*#define TIME_TOL_SWITCHRETRO_GAME4 30
				if((_abs64(nTimeEnArrowsGame4[0] - nTimeEnArrowsGame4[4]) < TIME_TOL_SWITCHRETRO_GAME4 && (bEnArrowsGame4[0] || bEnArrowsGame4[4])) ||
					(_abs64(nTimeEnArrowsGame4[5] - nTimeEnArrowsGame4[9]) < TIME_TOL_SWITCHRETRO_GAME4 && (bEnArrowsGame4[5] || bEnArrowsGame4[9])))
				{
					nChangeRetroGame4 = 1;
					nTimeChangeRetroGame4 = nCountGame4;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int y = 0; y < 10; y++)
					{
						bArrows[y] = false;
						bEnArrowsGame4[y] = false;
						bCh[y] = false;
					}
				}*/
#define TIME_TOL_SWITCHUNLOCKER_GAME4 30
				if(((_abs64(nTimeEnArrowsGame4[1] - nTimeEnArrowsGame4[3]) < TIME_TOL_SWITCHUNLOCKER_GAME4 && (bEnArrowsGame4[1] && bEnArrowsGame4[3])) ||
					(_abs64(nTimeEnArrowsGame4[6] - nTimeEnArrowsGame4[8]) < TIME_TOL_SWITCHUNLOCKER_GAME4 && (bEnArrowsGame4[6] && bEnArrowsGame4[8]))) && bIsAvalibleUnlockerGame4)
				{
					nChangeRetroGame4 = 3;
					nTimeChangeRetroGame4 = nCountGame4;
					bIsAvalibleUnlockerGame4 = false;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int y = 0; y < 10; y++)
					{
						bArrows[y] = false;
						bEnArrowsGame4[y] = false;
						bCh[y] = false;
					}
				}

				// 2
				if(bEnArrowsGame4[5])
				{
					if(bSkinGame4[1] && !bCh[5])
					{
						TurnSkinsGame4(true, 1);
						bCh[5] = true;
					}
					else if(bCharacterGame4[1] && !bCh[5])
					{
						TurnCharactersGame4(true, 1);
						bCh[5] = true;
					}
					else
					{
						__int64 nCaret = nCountGame4 - nTimeEnArrowsGame4[5];
						int nLimit = min(nCaret,nCountGame4-nTimeChaGame4)>TIME_FAST_CHANGE_GAME4?TIME_CHANGE_HIGH_GAME4:TIME_CHANGE_LOW_GAME4;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[5])
						{
							bCh[5] = true;
							MoveSongsGame4(true);
							PlaySoundTransiAboutLimitGame4(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[5] = false;
						}
					}
				}
				if(bEnArrowsGame4[9])
				{
					if(bSkinGame4[1] && !bCh[9])
					{
						TurnSkinsGame4(false, 1);
						bCh[9] = true;
					}
					else if(bCharacterGame4[1] && !bCh[9])
					{
						TurnCharactersGame4(false, 1);
						bCh[9] = true;
					}
					else
					{
						__int64 nCaret = nCountGame4 - nTimeEnArrowsGame4[9];
						int nLimit = min(nCaret,nCountGame4-nTimeChaGame4)>TIME_FAST_CHANGE_GAME4?TIME_CHANGE_HIGH_GAME4:TIME_CHANGE_LOW_GAME4;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[9])
						{
							bCh[9] = true;
							MoveSongsGame4(false);
							PlaySoundTransiAboutLimitGame4(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[9] = false;
						}
					}
				}
				if(bEnArrowsGame4[6] && !bCh[6])
				{
					if(!bSkinGame4[1] && !bCharacterGame4[1])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame4 = 0;
						bool bLoopLook = true;
						int nIndexLook = nCurSongGame4;
						while(bLoopLook)
						{
							nIndexLook--;
							if(nIndexLook < 0) nIndexLook = nSizeSongsGame4-1;
							if(nChaSongsGame4[nIndexLook] != nChaSongsGame4[nCurSongGame4] || nIndexLook == nCurSongGame4)
								bLoopLook = false;
						}
						nCurSongGame4 = nIndexLook;
						nTimeChaGame4 = nCountGame4;
						glDeleteTextures(9, nTexCurSongGame4);
						ActualizateIndexShowGame4();
						for(int x = 0; x < 9; x++)
						{
							nTexCurSongGame4[x] = 0;
							TCHAR chFile[260];
							_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsGame4[nIndexShowGame4[x]]);
							nTexCurSongGame4[x] = GenTexImage(chFile);
						}
						bCh[6] = true;
						GetLevelSongsGame4(nCurSongGame4, &nLevelCurGame4[1],
						&dDCurGame4[1][0],
						&dDCurGame4[1][1],
						&dDCurGame4[1][2],
						&dDCurGame4[1][3],
						&dDCurGame4[1][4]);
						GetBestScoresGame4(nCurSongGame4);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
				}
				if(bEnArrowsGame4[8] && !bCh[8])
				{
					if(!bSkinGame4[1] && !bCharacterGame4[1])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame4 = 0;
						bool bLoopLook = true;
						int nIndexLook = nCurSongGame4;
						while(bLoopLook)
						{
							nIndexLook++;
							if(nIndexLook >= nSizeSongsGame4) nIndexLook = 0;
							if(nChaSongsGame4[nIndexLook] != nChaSongsGame4[nCurSongGame4] || nIndexLook == nCurSongGame4)
								bLoopLook = false;
						}
						nCurSongGame4 = nIndexLook;
						nTimeChaGame4 = nCountGame4;
						glDeleteTextures(9, nTexCurSongGame4);
						ActualizateIndexShowGame4();
						for(int x = 0; x < 9; x++)
						{
							nTexCurSongGame4[x] = 0;
							TCHAR chFile[260];
							_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsGame4[nIndexShowGame4[x]]);
							nTexCurSongGame4[x] = GenTexImage(chFile);
						}
						bCh[8] = true;
						GetLevelSongsGame4(nCurSongGame4, &nLevelCurGame4[1],
						&dDCurGame4[1][0],
						&dDCurGame4[1][1],
						&dDCurGame4[1][2],
						&dDCurGame4[1][3],
						&dDCurGame4[1][4]);
						GetBestScoresGame4(nCurSongGame4);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
				}
				if(bEnArrowsGame4[7] && !bCh[7])
				{
					bCh[7] = true;
					if(np[1])
					{
						if(bSkinGame4[1])
						{
							MakeChoiceSkinGame4(1);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else if(bCharacterGame4[1])
						{
							MakeChoiceCharacterGame4(1);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else
						{
							nChosesGame4++;
							nTimeChosesGame4[nChosesGame4-1] = nCountGame4;
							if(nChosesGame4 > 0 && nChosesGame4 < CHOOSES_GAME4) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
							if(nChosesGame4 >= CHOOSES_GAME4)
							{
								FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
								for(int x = 0; x < 10; x++) bCh[x] = false;
							}
						}
					}
				}
				if(nCountGame4 >= nMaxGame4 && piuconfig.nModeMach!=1)
				{
					nChosesGame4 = 2;
					nTimeChosesGame4[nChosesGame4-1] = nCountGame4;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int x = 0; x < 10; x++) bCh[x] = false;
				}
				if(bNewCommand) CheckPresetGame4();
			}
		}
	#define TIME_EXIT_GAME4 1000
		if(nChosesGame4 >= CHOOSES_GAME4)
		{
			__int64 nTransEnd = nCountGame4 - nTimeChosesGame4[CHOOSES_GAME4-1];
			if(nTransEnd < TIME_EXIT_GAME4)
			{
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME4))
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
					if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME4))
					{
						if(x == 4)
						{
							if(nTransEnd < (TIME_TRANSI_SELECTOR_GAME4))
							{
								dApt = //pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME4, 2);
								pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME4 - 0.25, 2)/0.125 + 0.5;
								glPushMatrix();
								glTranslated(0.0, 0.0, -60.0);
								glScaled(dApt*1.25, dApt*1.25, 1.0);
								glColor4d(1.0, 1.0, 1.0, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME4);
								if(/*x == 4 && */nTransBorder > TIME_SHOW_PREVIEW_GAME4 && movie_songpreviewgame4->bLoad && movie_songpreviewgame4->bResize && bShow)
								{
									movie_songpreviewgame4->Update(dwWait);
									movie_songpreviewgame4->GrabAVIFrame();
									__int64 nCaretCover = movie_songpreviewgame4->dwNext;
									glBindTexture(GL_TEXTURE_2D, movie_songpreviewgame4->tex);

									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
									glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
									glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
									glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
									glEnd();
									if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME4)
									{
										dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME4;
										glColor4d(1.0, 1.0, 1.0, min(1.0-dApt, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME4));
										glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
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
									glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
									glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
									glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
									glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
									glEnd();
								}
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
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
							__int64 nStartTransi = TIME_TRANSI_SELECTOR_GAME4/4*(-abs(x-4)+4-1);
							__int64 nLimitTransi = TIME_TRANSI_SELECTOR_GAME4/4*(-abs(x-4)+4);
							if(nTransEnd >= nStartTransi && nTransEnd < nLimitTransi)
							{
								dApt = double(nTransEnd-nStartTransi)/double(nLimitTransi-nStartTransi);
								double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(dApt)+dTargetx;
								double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
								double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
								double dtheta = ThetaFuncForXGame4(dx);
								double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
								double dalpha = AlphaFuncForXGame4(dx);
								glPushMatrix();
								glTranslated(dx, dy, dz-60.0);
								glRotated(dtheta, 0.0, 1.0, 0.0);
								glScaled(dscale, dscale, 1.0);
								glColor4d(1.0, 1.0, 1.0, min(dalpha, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME4));
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
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
								double dtheta = ThetaFuncForXGame4(dx);
								double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
								double dalpha = AlphaFuncForXGame4(dx);
								glPushMatrix();
								glTranslated(dx, dy, dz-60.0);
								glRotated(dtheta, 0.0, 1.0, 0.0);
								glScaled(dscale, dscale, 1.0);
								glColor4d(1.0, 1.0, 1.0, dalpha);
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
								glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
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
					if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_LR_GAME4))
					{
						dApt = pow(double(nTransEnd)/TIME_TRANSI_LR_GAME4, 0.5);
						glPushMatrix();
						glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(dApt), 0.0, 0.0);
						glColor4d(1.0, 1.0, 1.0, 1-dApt);
						glBindTexture(GL_TEXTURE_2D, ga4res->skin[(k?68:50)+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
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
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_UPDO_GAME4))
				{
					dApt = (double(nTransEnd))/(TIME_TRANSI_UPDO_GAME4);
					glPushMatrix();
					glScaled(1.0+0.8*(dApt), 1.0+0.8*(dApt), 1.0);
					glBindTexture(GL_TEXTURE_2D, ga4res->skin[41+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga4res->skin[23+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga4res->skin[3]->texture[0]);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga4res->skin[4]->texture[0]);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga4res->skin[5]->texture[0]);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga4res->skin[6]->texture[0]);
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
				for(int k = 0; k < 2; k++) if(np[k])
				{
					player_sesion[k]->nLastBrainShower[0] = nRefSongsGame4[nCurSongGame4];
				}
				if(movie_songpreviewgame4->bLoad)
				{
					movie_songpreviewgame4->CloseAVI();
					delete movie_songpreviewgame4;
					movie_songpreviewgame4 = new CMovieStream2();
				}
				/*for(int x = 0; x < nSizeMovies; x++)
				{
					if(movie_array[x]->bLoad) movie_array[x]->CloseAVI();
					delete movie_array[x];
				}*/
				glDeleteTextures(9, nTexCurSongGame4);
				for(int x = 0; x < 9; x++) nTexCurSongGame4[x] = 0;
				bShow = false;
				LoadSongGame4();
				Go_Step();
				return 2;
			}
		}
	}
	else
	{
#define TIME_EXIT_CHANGERETRO_GAME4 1000
		__int64 nTransEnd = nCountGame4 - nTimeChangeRetroGame4;
		if(nTransEnd < TIME_EXIT_CHANGERETRO_GAME4)
		{
			if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME4))
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
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME4))
				{
					if(x == 4)
					{
						if(nTransEnd < (TIME_TRANSI_SELECTOR_GAME4))
						{
							dApt = //pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME4, 2);
							pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME4 - 0.25, 2)/0.125 + 0.5;
							glPushMatrix();
							glTranslated(0.0, 0.0, -60.0);
							glScaled(dApt*1.25, dApt*1.25, 1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME4);
							if(nTransBorder > TIME_SHOW_PREVIEW_GAME4 && movie_songpreviewgame4->bLoad && movie_songpreviewgame4->bResize && bShow)
							{
								movie_songpreviewgame4->Update(dwWait);
								movie_songpreviewgame4->GrabAVIFrame();
								__int64 nCaretCover = movie_songpreviewgame4->dwNext;
								glBindTexture(GL_TEXTURE_2D, movie_songpreviewgame4->tex);

								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
								glEnd();
								if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME4)
								{
									dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME4;
									glColor4d(1.0, 1.0, 1.0, min(1.0-dApt, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME4));
									glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
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
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
							}
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
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
						__int64 nStartTransi = TIME_TRANSI_SELECTOR_GAME4/4*(-abs(x-4)+4-1);
						__int64 nLimitTransi = TIME_TRANSI_SELECTOR_GAME4/4*(-abs(x-4)+4);
						if(nTransEnd >= nStartTransi && nTransEnd < nLimitTransi)
						{
							dApt = double(nTransEnd-nStartTransi)/double(nLimitTransi-nStartTransi);
							double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(dApt)+dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXGame4(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForXGame4(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, min(dalpha, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME4));
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
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
							double dtheta = ThetaFuncForXGame4(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForXGame4(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame4[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga4res->skin[14+nChaSongsGame4[nIndexShowGame4[x]]]->GetCurrentTex());
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
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_LR_GAME4))
				{
					dApt = pow(double(nTransEnd)/TIME_TRANSI_LR_GAME4, 0.5);
					glPushMatrix();
					glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(dApt), 0.0, 0.0);
					glColor4d(1.0, 1.0, 1.0, 1-dApt);
					glBindTexture(GL_TEXTURE_2D, ga4res->skin[(k?68:50)+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
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
			if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_UPDO_GAME4))
			{
				dApt = (double(nTransEnd))/(TIME_TRANSI_UPDO_GAME4);
				glPushMatrix();
				glScaled(1.0+0.8*(dApt), 1.0+0.8*(dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[41+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[23+nChaSongsGame4[nCurSongGame4]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[3]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[4]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[5]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga4res->skin[6]->texture[0]);
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
			nCurSongGame4 = 0;
			if(movie_songpreviewgame4->bLoad)
			{
				movie_songpreviewgame4->CloseAVI();
				delete movie_songpreviewgame4;
				movie_songpreviewgame4 = new CMovieStream2();
			}
			glDeleteTextures(9, nTexCurSongGame4);
			for(int x = 0; x < 9; x++) nTexCurSongGame4[x] = 0;
			bShow = false;
			if(nChangeRetroGame4 == 2)
			{
				//SemiActualizateContextGame4();
				return 4;
			}
			if(nChangeRetroGame4 == 3)
			{
				nCaretCountGame4 = nCountGame4;
				return 5;
			}
			for(int k = 0; k < 2; k++) if(np[k])
			{
				player_sesion[k]->nLastBrainShower[0] = nRefSongsGame4[nCurSongGame4];
			}
			nCaretCountGame4 = nCountGame4;
			SemiActualizateContextGame4();
			nMaxGame4 = nMaxGame4 - nCaretCountGame4;
		}
	}

	return 0;
}

void SemiActualizateContextGame4()
{
	int nPlayerFocus = 0;
	nChangeRetroGame4 = 0;
	switch(piuconfig.nTimePlay)
	{
	default:
		nMaxGame4 = 120000+TIME_TRANSI_UPDO_GAME4+TIME_START_UPDO_GAME4; break;
	case 1:
		nMaxGame4 = 90000+TIME_TRANSI_UPDO_GAME4+TIME_START_UPDO_GAME4; break;
	case 2:
		nMaxGame4 = 60000+TIME_TRANSI_UPDO_GAME4+TIME_START_UPDO_GAME4; break;
	}
	nTimeChangeRetroGame4 = 0;
	nModeGame4 = 0;
	nCurSongGame4 = 0;
	nChosesGame4 = 0;
	nCountGame4 = 0;
	nRetroActiveGame4 = 0;
	nTimeCurSongGame4 = TIME_START_SELECTOR_GAME4+TIME_TRANSI_SELECTOR_GAME4+TIME_GLOUTH_SELECTOR_GAME4;
	bLoadMusicCoverGame4 = false;
	strBestScoreMachineGame4 = _T("");
	nBestScoreMachineGame4 = 0;
	nBestScoreMachineGame4Ex = 0;
	for(int x = 0; x < 2; x++)
	{
		nTimeSkinGame4[x] = 0;
		bSkinGame4[x] = false;
		nTimeTurnSkinGame4[x] = 0;
		nDirTurnSkinGame4[x] = 0;
		nTimeCharacterGame4[x] = 0;
		bCharacterGame4[x] = false;
		nTimeTurnCharacterGame4[x] = 0;
		nDirTurnCharacterGame4[x] = 0;
		nBestScorePlayerGame4[x] = 0;
		nBestScorePlayerGame4Ex[x] = 0;
		nLevelCurGame4[x] = 1;
		nIndLastPresetGame4[x] = -1;	// Show all
		nTimeLastPresetGame4[x] = (TIME_START_UPDO_GAME4+TIME_TRANSI_UPDO_GAME4)+(TIME_START_LR_GAME4+TIME_TRANSI_LR_GAME4);
		if(np[x]) ShowSesionTrayExp(x);
		for(int y = 0; y < 10; y++) nCommandGame4[x][y] = -1;
		for(int y = 0; y < MAX_COMMANDS_GAME4; y++) nComandGame4[x][y] = -1;
	}
	for(int x = 0; x < 10; x++) bEnArrowsGame4[x] = false;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		EnumSongsGame4();
		nPlayerFocus = x;
		break;
	}
	bool bFoundLast = false;
	for(int x = 0; x < nSizeSongsGame4; x++)
	{
		if(nRefSongsGame4[x] == player_sesion[nPlayerFocus]->nLastBrainShower[0])
		{
			bFoundLast = true;
			nCurSongGame4 = x;
			break;
		}
	}
	if(!bFoundLast) nCurSongGame4 = 0;
	nTimeChaGame4 = TIME_START_UPDO_GAME4+TIME_TRANSI_UPDO_GAME4;

	ActualizateIndexShowGame4();
	for(int x = 0; x < 9; x++)
	{
		TCHAR chFile[260];
		_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsGame4[nIndexShowGame4[x]]);
		nTexCurSongGame4[x] = GenTexImage(chFile);
	}

	for(int x = 0; x < 10; x++) {
        bArrows[x] = false;
    }
	GetBestScoresGame4(nCurSongGame4);
	EnumSkinsGame4();
	EnumCharactersGame4();
	dwLastTick = dwTick = SDL_GetTicks();
}

int StageCheckGame4()
{
	CheckForUnlock();
	bool bIs[] = {false, false};
	for(int x = 0; x < 2; x++) if(np[x])
	{
		int nMileage = 0;
		if(x)
		{
			nMileage = viewp2->status.nScore/10000;
		}
		else
		{
			nMileage = viewp1->status.nScore/10000;
		}
		if(player_sesion[x]->nLockedAsp[ITEM_LUCK] && (rand()%5) == 0) nMileage *= 2;
		nMileage += (5-nRankStepEval[x])*50;
		player_sesion[x]->nMileage += nMileage;
		double dApt = double(5-nRankStepEval[x])/5*(bUsb[0]?1.0:0.5)*(player_sesion[x]->nLockedAsp[ITEM_MOREEXP]?2.0:1.0);;
		PlusExpPlayer(x, nLevelCurGame4[x], dApt);
		if(piuconfig.nModeMach)
		{
			np[x] = 0;
		}
		else
		{
			int nMinus = 2;
			bIs[x] = true;
			np[x]-=nMinus;
			//if(nRankStepEval[x] >= 5) np[x]-=2;
			if(nRankStepEval[x] >= 2 && bBonusStageGame4[x])
			{
				np[x]-=2;
				bBonusStageGame4[x] = false;
			}
			np[x] = max(0, np[x]);
			if(np[x] <= 1) np[x] = 0;
		}
	}
	if((!np[0]) && (!np[1])) return 1;
	else if((!np[0] && bIs[0]) || (!np[1] && bIs[1])) return 2;
	return 0;
}

////////////////////////////////////////////////
// ** Funciones principales

int CreateContextGame4()
{
	if(!EnumTotalSongsGame4()) return 0;
	nRefSkinGame4 = (int*)calloc(0, sizeof(int));
	uTexSkinGame4 = (GLuint*)calloc(0, sizeof(GLuint));
	nRefCharacterGame4 = (int*)calloc(0, sizeof(int));
	uTexCharacterGame4 = (GLuint*)calloc(0, sizeof(GLuint));
	return 1;
}

void DestroyContextGame4()
{
	delete nRefSkinGame4;
	delete uTexSkinGame4;
	delete nRefCharacterGame4;
	delete uTexCharacterGame4;
}

void ClearContextGame4()
{
	if(movie_brainbg->bLoad) movie_brainbg->CloseAVI();
	delete movie_brainbg;
	delete ga4res;
	DeleteSkinsGame4();
	DeleteCharactersGame4();
}

void ActualizateContextGame4()
{
	nCurStage = 1;
	movie_brainbg = new CMovieStream2();
	movie_songpreviewgame4 = new CMovieStream2();
	movie_brainbg->OpenAVI(_T("Game4/ArcadeBG.avi"));
	movie_brainbg->bLoop = true;

	ga4res = new CGame4Res();
	ga4res->LoadSkin();

	switch(piuconfig.nTimePlay)
	{
	default:
		nMaxGame4 = 120000+TIME_TRANSI_UPDO_GAME4+TIME_START_UPDO_GAME4; break;
	case 1:
		nMaxGame4 = 90000+TIME_TRANSI_UPDO_GAME4+TIME_START_UPDO_GAME4; break;
	case 2:
		nMaxGame4 = 60000+TIME_TRANSI_UPDO_GAME4+TIME_START_UPDO_GAME4; break;
	}
	nInterGame4 = 0;
	nRetroActiveGame4 = 0;
	int nPlayerFocus = 0;
	nChangeRetroGame4 = 0;
	nTimeChangeRetroGame4 = 0;
	nModeGame4 = 0;
	nCurSongGame4 = 0;
	nChosesGame4 = 0;
	nCountGame4 = 0;
	nTimeCurSongGame4 = TIME_START_SELECTOR_GAME4+TIME_TRANSI_SELECTOR_GAME4+TIME_GLOUTH_SELECTOR_GAME4;
	bLoadMusicCoverGame4 = false;
	bContinueUsbGame4 = true;
	strBestScoreMachineGame4 = _T("");
	nBestScoreMachineGame4 = 0;
	nBestScoreMachineGame4Ex = 0;
	bIsAvalibleUnlockerGame4 = true;
	for(int x = 0; x < 2; x++)
	{
		nTimeSkinGame4[x] = 0;
		bSkinGame4[x] = false;
		nTimeTurnSkinGame4[x] = 0;
		nDirTurnSkinGame4[x] = 0;
		nCurSkinGame4[x] = 0;
		nTimeCharacterGame4[x] = 0;
		bCharacterGame4[x] = false;
		nTimeTurnCharacterGame4[x] = 0;
		nDirTurnCharacterGame4[x] = 0;
		nCurCharacterGame4[x] = 0;
		nBestScorePlayerGame4[x] = 0;
		nBestScorePlayerGame4Ex[x] = 0;
		bBonusStageGame4[x] = true;
		nLevelCurGame4[x] = 1;
		nIndLastPresetGame4[x] = -1;	// Show all
		nTimeLastPresetGame4[x] = (TIME_START_UPDO_GAME4+TIME_TRANSI_UPDO_GAME4)+(TIME_START_LR_GAME4+TIME_TRANSI_LR_GAME4);
		if(np[x]) ShowSesionTrayExp(x);
		for(int y = 0; y < 6; y++)
		{
			nIndSizePresetGame4[x][y] = 0;
			nIndPresetGame4[x][y*5] = 0;
			nIndPresetGame4[x][y*5+1] = 0;
			nIndPresetGame4[x][y*5+2] = 0;
			nIndPresetGame4[x][y*5+3] = 0;
			nIndPresetGame4[x][y*5+4] = 0;
		}
		dDCurGame4[x][0] = 0.0;
		dDCurGame4[x][1] = 0.0;
		dDCurGame4[x][2] = 0.0;
		dDCurGame4[x][3] = 0.0;
		dDCurGame4[x][4] = 0.0;
		for(int y = 0; y < 10; y++) nCommandGame4[x][y] = -1;
		for(int y = 0; y < MAX_COMMANDS_GAME4; y++) nComandGame4[x][y] = -1;
	}

	for(int x = 0; x < 10; x++) {
        bArrows[x] = false;
    }
	for(int x = 0; x < 10; x++) bEnArrowsGame4[x] = false;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		EnumSongsGame4();
		nPlayerFocus = x;
		break;
	}
	bool bFoundLast = false;
	for(int x = 0; x < nSizeSongsGame4; x++)
	{
		if(nRefSongsGame4[x] == player_sesion[nPlayerFocus]->nLastBrainShower[0])
		{
			bFoundLast = true;
			nCurSongGame4 = x;
			/*for(int y = 0; y < 2; y++)
			{
				if(y == nPlayerFocus)
					nModeCurSongGame4[y] = GetAvalibleModeGame4(x, player_sesion[nPlayerFocus]->nLastSpecial[1]);
				else
					nModeCurSongGame4[y] = GetAvalibleModeGame4(x, 0);
				nBackModeCurSongGame4[y] = nModeCurSongGame4[y];
				nPrefModeCurSongGame4[y] = nModeCurSongGame4[y];
			}*/
			break;
		}
	}
	if(!bFoundLast) nCurSongGame4 = 0;
	nTimeChaGame4 = TIME_START_UPDO_GAME4+TIME_TRANSI_UPDO_GAME4;

	ActualizateIndexShowGame4();
	for(int x = 0; x < 9; x++)
	{
		TCHAR chFile[260];
		_sprintf(chFile, _T("song/preview/%ls.PNG"), chSongsGame4[nIndexShowGame4[x]]);
		nTexCurSongGame4[x] = GenTexImage(chFile);
	}
	GetBestScoresGame4(nCurSongGame4);
	EnumSkinsGame4();
	EnumCharactersGame4();
	dwLastTick = dwTick = SDL_GetTicks();
}

bool bContAfterSplashGame4 = false;

int FrameGame4()
{
	ga4res->Update(dwWait);
	switch(nInterGame4)
	{
	case 0:
		switch(DrawInterfaceSelectGame4())
		{
		case 1:
			ClearContextGame4();
			return INT_WELCOME;
			break;
		case 2:
			nInterGame4 = 1;
			break;
		case 3:
			return INT_ITEMSHOP;
			break;
		case 4:
			//ClearContextGame4();
			ActualizateContextUsb();
			bSaveUsb = false;
			bContinueUsbGame4 = true;
			nInterGame4 = 4;
			for(int x = 0; x < 2; x++) if(np[x])
			{
				bShowSesionTray[x] = false;
				bShowSesionTrayExp[x] = false;
				glDeleteTextures(1, &uTexPlayerSplash[x]);
			}
			break;
		case 5:
			nModeUnlock = 1;
			ActualizateContextUnlock();
			nInterGame4 = 6;
			break;
		}
		break;
	case 1:
		if(!Draw_All())
		{
			nInterGame4 = 2;
			nModeStepEval = 2;	// Brain Shower Mode
			ActualizateContextStepEval();
		}
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
			if(nMC > 50 || (dMB <= 0.0 && piuconfig.nStageBreak!=0 && nCurStage >= piuconfig.nStageBreak))
			{
				nInterGame4 = 5;
				np[0] = 0;
				np[1] = 0;
				DeleteSongGame4();
			}
		}
		break;
	case 2:
		if(!FrameStepEval())
		{
			nInterGame4 = 0;
			{
				int nCode = StageCheckGame4();
				if(nCode == 0)
				{
                    nInterGame4 = 8;
                    ActualizateContextStageStepEval();
                }
				else
				{
					nInterGame4 = 4;
					if(nCode == 1) bContinueUsbGame4 = false;
					if(nCode == 2) bContinueUsbGame4 = true;
					bSaveUsb = true;
					ActualizateContextUsb();
				}
				if(nSizeUnlock)
				{
					nModeUnlock = 0;
					ActualizateContextUnlock();
					nInterGame4 = 7;
					if(nCode != 0) bContAfterSplashGame4 = false;
					else bContAfterSplashGame4 = true;
				}
				DeleteSongGame4();
			}
			SemiActualizateContextGame4();
		}
		break;
	case 3:
		{
			int nCode = StageCheckGame4();
			if(nCode == 0) nInterGame4 = 0;
			else
			{
				nInterGame4 = 4;
				if(nCode == 1) bContinueUsbGame4 = false;
				if(nCode == 2) bContinueUsbGame4 = true;
				bSaveUsb = true;
				ActualizateContextUsb();
			}
			if(nSizeUnlock)
			{
				nModeUnlock = 0;
				ActualizateContextUnlock();
				nInterGame4 = 7;
				if(nCode != 0) bContAfterSplashGame4 = false;
				else bContAfterSplashGame4 = true;
			}
			DeleteSongGame4();
		}
		SemiActualizateContextGame4();
		break;
	case 4:	// Dibujar Sistema USB
		if(!FrameUsb())
		{
			nInterGame4 = 0;
			if(!bContinueUsbGame4)
			{
				ClearContextGame4();
				return INT_WELCOME;
			}
			else SemiActualizateContextGame4();
		}
		break;
	case 5:
		if(!FrameStageBreakStepEval())
		{
			nInterGame4 = 4;
			np[0] = 0;
			np[1] = 0;
			bSaveUsb = true;
			ActualizateContextUsb();
			bContinueUsbGame4 = false;
		}
		break;
	case 6:
		if(!FrameUnlock())
		{
			nInterGame4 = 0;
			SemiActualizateContextGame4();
			nMaxGame4 = nMaxGame4 - nCaretCountGame4;
		}
		break;
	case 7:
		if(FrameUnlock() != INT_UNLOCK)
		{
			nInterGame4 = bContAfterSplashGame4?0:4;
		}
		break;
	case 8:
		if(!FrameStageStepEval())
		{
			nInterGame4 = 0;
			nCurStage++;
		}
		break;
	}
	return INT_GAME_4;
}

int EventGame4(int nEvent, int nKey)
{
	switch(nInterGame4)
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
		EventUnlock(nEvent, nKey);
		break;
	}
	return 1;
}
