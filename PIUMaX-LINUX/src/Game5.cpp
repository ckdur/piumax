#include "Game5.h"
#include "stepeval.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "Game5Res.h"
#include "USBsys.h"
#include "UnlockSys.h"
#include "History.h"

#define TIME_START_SELECTOR_GAME5 0
#define TIME_TRANSI_SELECTOR_GAME5 1000
#define TIME_GLOUTH_SELECTOR_GAME5 500

#define TIME_START_START_GAME5 1500

#define TIME_TRANSI_SONGBORDER_GAME5 500
#define TIME_GLOUTH_SONGBORDER_GAME5 500
#define TIME_TRANSI_START_MAIN_GAME5 500
#define CHOOSES_GAME5 2
#define MAX_COMMANDS_GAME5 12

#define TIME_SHOW_PREVIEW_GAME5 600

int nComandGame5[2][MAX_COMMANDS_GAME5];
int nInterGame5 = 0;
int nSizeSongsGame5 = 0;
int nChaTotalSongsGame5[1000];
int nChaSongsGame5[1000];
int nRefSongsGame5[1000];
GLuint* nTexTrainingGame5;
GLuint nTexCurSongGame5[9];
GLuint nTexCurGame5 = 0;
TCHAR chTotalSongsGame5[1000][100];
TCHAR chSongsGame5[1000][100];
bool bIsSingleSongGame5[1000];
bool bIsSingleTotalSongGame5[1000];
CMissionLoader* training;

int nIndexShowGame5[9];
int nLevelCurGame5[2];
int nCurSongGame5 = 0;
/*int nModeCurSongGame5[2];
int nBackModeCurSongGame5[2];
int nPrefModeCurSongGame5[2];
__int64 nTimeModeCurSongGame5[2];*/
__int64 nTimeCurSongGame5 = 0;
__int64 nCountGame5 = 0;
__int64 nMaxGame5 = 0;
__int64 nTimeChaGame5 = 0;

__int64 nCaretCountGame5;

__int64 nTimeLastPresetGame5[2];
int nIndLastPresetGame5[2];
int nIndPresetGame5[2][30];
int nIndSizePresetGame5[2][6];
int nSkinGame5[2];
int nCharacterGame5[2];

int nChosesGame5;
__int64 nTimeChosesGame5[2];
int nChangeRetroGame5;
__int64 nTimeChangeRetroGame5;
int nRetroActiveGame5;

bool bEnArrowsGame5[10];
__int64 nTimeEnArrowsGame5[10];
int nCommandGame5[2][10];

int nTimeTransiSongBorderGame5 = TIME_TRANSI_SONGBORDER_GAME5;
int nModeGame5 = 0;
bool bBonusStageGame5[2];
//bool bSaveUsbGame5 = false;
bool bContinueUsbGame5 = true;

GLuint movie_texGame5;
CMovieStream2* movie_trainingbg;
CMovieStream2* movie_songpreviewgame5;

bool bLoadMusicCoverGame5 = false;
FMOD_SOUND* music_coverGame5;

CString strDispGame5;

CGame5Res* ga5res;

CString strBestScoreMachineGame5;
int nBestScoreMachineGame5;
int nBestScorePlayerGame5[2];
/*int nBestScoreMachineGame5Ex;
int nBestScorePlayerGame5Ex[2];*/

int FoundPresetGame5(int x, int nFamily, int nBegin, int nEnd);
void SemiActualizateContextGame5();

int nSizeSkinGame5 = 0;
int* nRefSkinGame5;
GLuint* uTexSkinGame5;
int nSizeCharacterGame5 = 0;
int* nRefCharacterGame5;
GLuint* uTexCharacterGame5;

#define MAX_SKINS_GAME5 20
#define MAX_CHARACTERS_GAME5 12

__int64 nTimeSkinGame5[2];
bool bSkinGame5[2];
__int64 nTimeTurnSkinGame5[2];
int nDirTurnSkinGame5[2];
int nCurSkinGame5[2];
__int64 nTimeCharacterGame5[2];
bool bCharacterGame5[2];
__int64 nTimeTurnCharacterGame5[2];
int nDirTurnCharacterGame5[2];
int nCurCharacterGame5[2];

bool bIsAvalibleUnlockerGame5;

int FoundPresetGame5(int x, int nFamily, int nBegin, int nEnd);

void MakeChoiceSkinGame5(int x)
{
	bSkinGame5[x] = false;
	int nPut = nRefSkinGame5[nCurSkinGame5[x]-1]+25;
	int nFamily = 2;
	int nFound = FoundPresetGame5(x, nFamily, 24, 45);
	if(nFound == -1 && nCurSkinGame5[x])
	{
		nFound = nIndSizePresetGame5[x][nFamily];
		nIndSizePresetGame5[x][nFamily]++;
		nIndPresetGame5[x][nFamily*5+nFound] = nPut;
		nIndLastPresetGame5[x] = nFamily*5+nFound;
		nTimeLastPresetGame5[x] = nCountGame5;
	}
	if(nFound != 1)
	{
		if(!nCurSkinGame5[x])	// Desactivar
		{
			nIndPresetGame5[x][nFamily*5+nFound] = 0;
			for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
				nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
			nIndSizePresetGame5[x][nFamily]--;
		}
		else	// Actualizar
		{
			nIndPresetGame5[x][nFamily*5+nFound] = nPut;
			nIndLastPresetGame5[x] = nFamily*5+nFound;
			nTimeLastPresetGame5[x] = nCountGame5;
		}
	}
}

void MakeChoiceCharacterGame5(int x)
{
	bCharacterGame5[x] = false;
	int nPut = nRefCharacterGame5[nCurCharacterGame5[x]-1]+47;
	int nFamily = 5;
	int nFound = FoundPresetGame5(x, nFamily, 46, 59);
	if(nFound == -1 && nCurCharacterGame5[x])
	{
		nFound = nIndSizePresetGame5[x][nFamily];
		nIndSizePresetGame5[x][nFamily]++;
		nIndPresetGame5[x][nFamily*5+nFound] = nPut;
		nIndLastPresetGame5[x] = nFamily*5+nFound;
		nTimeLastPresetGame5[x] = nCountGame5;
	}
	if(nFound != 1)
	{
		if(!nCurCharacterGame5[x])	// Desactivar
		{
			nIndPresetGame5[x][nFamily*5+nFound] = 0;
			for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
				nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
			nIndSizePresetGame5[x][nFamily]--;
		}
		else	// Actualizar
		{
			nIndPresetGame5[x][nFamily*5+nFound] = nPut;
			nIndLastPresetGame5[x] = nFamily*5+nFound;
			nTimeLastPresetGame5[x] = nCountGame5;
		}
	}
}


void TurnSkinsGame5(bool bIzq, int x)
{
	if(bIzq)
	{
		nCurSkinGame5[x]--;
		if(nCurSkinGame5[x] < 0) nCurSkinGame5[x] = nSizeSkinGame5;
	}
	else
	{
		nCurSkinGame5[x]++;
		if(nCurSkinGame5[x] >= (nSizeSkinGame5+1)) nCurSkinGame5[x] = 0;
	}
	nDirTurnSkinGame5[x] = bIzq?1:2;
	nTimeTurnSkinGame5[x] = nCountGame5;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
}

void TurnCharactersGame5(bool bIzq, int x)
{
	if(bIzq)
	{
		nCurCharacterGame5[x]--;
		if(nCurCharacterGame5[x] < 0) nCurCharacterGame5[x] = nSizeCharacterGame5;
	}
	else
	{
		nCurCharacterGame5[x]++;
		if(nCurCharacterGame5[x] >= (nSizeCharacterGame5+1)) nCurCharacterGame5[x] = 0;
	}
	nDirTurnCharacterGame5[x] = bIzq?1:2;
	nTimeTurnCharacterGame5[x] = nCountGame5;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
}

void DrawSkinsGame5(int x)
{
#define TIME_TRANSI_SKIN_GAME5 500
	__int64 nTimeSkin = nCountGame5 - nTimeSkinGame5[x];
	double dApt;
	int n[5];
	for(int y = 0; y < 5; y++)
	{
		n[y] = (y-2+nCurSkinGame5[x])%(nSizeSkinGame5+1);
		while(n[y] < 0) n[y] += (nSizeSkinGame5+1);
	}
	if(nTimeSkin >= 0 && nTimeSkin < TIME_TRANSI_SKIN_GAME5)
	{
		dApt = double(nTimeSkin)/TIME_TRANSI_SKIN_GAME5;
		glColor4d(1.0, 1.0, 1.0, dApt);
		glPushMatrix();
		glTranslated(0.0, -2.5*(1-dApt)+11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga5res->skin[168]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
		glBindTexture(GL_TEXTURE_2D, ga5res->skin[167]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[2]==0?ga5res->skin[169]->GetCurrentTex():uTexSkinGame5[n[2]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(10.0-5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga5res->skin[167]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[3]==0?ga5res->skin[169]->GetCurrentTex():uTexSkinGame5[n[3]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(-10.0+5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga5res->skin[167]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[1]==0?ga5res->skin[169]->GetCurrentTex():uTexSkinGame5[n[1]-1]);
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
	if(nTimeSkin >= TIME_TRANSI_SKIN_GAME5)
	{
#define TIME_TURNSKIN_GAME5 300
		glPushMatrix();
		glTranslated(0.0, 11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga5res->skin[168]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		__int64 nTimeTurn = nCountGame5 - nTimeTurnSkinGame5[x];
		dApt = 0.0;
		if(nTimeTurn >= 0 && nTimeTurn <= TIME_TURNSKIN_GAME5 && nDirTurnSkinGame5[x])
		{
			dApt = (nDirTurnSkinGame5[x]==1?-1:1)*(1-double(nTimeTurn)/TIME_TURNSKIN_GAME5);
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
			glBindTexture(GL_TEXTURE_2D, ga5res->skin[167]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, n[u]==0?ga5res->skin[169]->GetCurrentTex():uTexSkinGame5[n[u]-1]);
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

void DrawCharactersGame5(int x)
{
#define TIME_TRANSI_CHARACTER_GAME5 500
	__int64 nTimeCharacter = nCountGame5 - nTimeCharacterGame5[x];
	double dApt;
	int n[5];
	for(int y = 0; y < 5; y++)
	{
		n[y] = (y-2+nCurCharacterGame5[x])%(nSizeCharacterGame5+1);
		while(n[y] < 0) n[y] += (nSizeCharacterGame5+1);
	}
	if(nTimeCharacter >= 0 && nTimeCharacter < TIME_TRANSI_CHARACTER_GAME5)
	{
		dApt = double(nTimeCharacter)/TIME_TRANSI_CHARACTER_GAME5;
		glColor4d(1.0, 1.0, 1.0, dApt);
		glPushMatrix();
		glTranslated(0.0, -2.5*(1-dApt)+11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga5res->skin[171]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
		glBindTexture(GL_TEXTURE_2D, ga5res->skin[170]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[2]==0?ga5res->skin[172]->GetCurrentTex():uTexCharacterGame5[n[2]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(10.0-5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga5res->skin[170]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[3]==0?ga5res->skin[172]->GetCurrentTex():uTexCharacterGame5[n[3]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, -5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0,  5.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(-10.0+5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga5res->skin[170]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[1]==0?ga5res->skin[172]->GetCurrentTex():uTexCharacterGame5[n[1]-1]);
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
	if(nTimeCharacter >= TIME_TRANSI_CHARACTER_GAME5)
	{
#define TIME_TURNCHARACTER_GAME5 300
		glPushMatrix();
		glTranslated(0.0, 11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga5res->skin[171]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		__int64 nTimeTurn = nCountGame5 - nTimeTurnCharacterGame5[x];
		dApt = 0.0;
		if(nTimeTurn >= 0 && nTimeTurn <= TIME_TURNCHARACTER_GAME5 && nDirTurnCharacterGame5[x])
		{
			dApt = (nDirTurnCharacterGame5[x]==1?-1:1)*(1-double(nTimeTurn)/TIME_TURNCHARACTER_GAME5);
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
			glBindTexture(GL_TEXTURE_2D, ga5res->skin[170]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, n[u]==0?ga5res->skin[172]->GetCurrentTex():uTexCharacterGame5[n[u]-1]);
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

void DeleteSkinsGame5();
void DeleteCharactersGame5();

void EnumSkinsGame5()
{
	DeleteSkinsGame5();
	for(int x = 0; x < MAX_SKINS_GAME5; x++)
	{
		if((np[0] && !player_sesion[0]->nLockedAsp[x]) ||
			(np[1] && !player_sesion[1]->nLockedAsp[x]))
		{
			nSizeSkinGame5++;
			nRefSkinGame5 = (int*)realloc(nRefSkinGame5, nSizeSkinGame5*sizeof(int));
			uTexSkinGame5 = (GLuint*)realloc(uTexSkinGame5, nSizeSkinGame5*sizeof(GLuint));
			nRefSkinGame5[nSizeSkinGame5-1] = x;
			CString str;
			str.Format(_T("skins/%d/Preview.png"), x+1);
			uTexSkinGame5[nSizeSkinGame5-1] = GenTexImage(str);
		}
	}
}

void EnumCharactersGame5()
{
	DeleteCharactersGame5();
	for(int x = 0; x < MAX_CHARACTERS_GAME5; x++)
	{
		if((np[0] && !player_sesion[0]->nLockedAsp[20+x]) ||
			(np[1] && !player_sesion[1]->nLockedAsp[20+x]))
		{
			nSizeCharacterGame5++;
			nRefCharacterGame5 = (int*)realloc(nRefCharacterGame5, nSizeCharacterGame5*sizeof(int));
			uTexCharacterGame5 = (GLuint*)realloc(uTexCharacterGame5, nSizeCharacterGame5*sizeof(GLuint));
			nRefCharacterGame5[nSizeCharacterGame5-1] = x;
			CString str;
			str.Format(_T("Characters/%d/Preview.png"), x+1);
			uTexCharacterGame5[nSizeCharacterGame5-1] = GenTexImage(str);
		}
	}
}

void DeleteSkinsGame5()
{
	if(nSizeSkinGame5)
	{
		glDeleteTextures(nSizeSkinGame5, uTexSkinGame5);
		nSizeSkinGame5 = 0;
		nRefSkinGame5 = (int*)realloc(nRefSkinGame5, nSizeSkinGame5*sizeof(int));
		uTexSkinGame5 = (GLuint*)realloc(uTexSkinGame5, nSizeSkinGame5*sizeof(GLuint));
	}
}

void TraduceModCurTraining()
{
	for(int k = 0; k < 2; k++) if(np[k])
	{
		nIndLastPresetGame5[k] = -1;	// Show all
		for(int x = 0; x < training->m_nSizeMod[nRefSongsGame5[nCurSongGame5]]; x++)
		{
			int nMod = training->m_nMod[nRefSongsGame5[nCurSongGame5]][x];
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
			int nFound = FoundPresetGame5(k, nFamily, nInf, nSup);
			if(nFound == -1 && nInf != 0)
			{
				nFound = nIndSizePresetGame5[k][nFamily];
				nIndSizePresetGame5[k][nFamily]++;
			}
			if(nInf != 0) nIndPresetGame5[k][nFamily*5+nFound] = nMod;
		}
	}
}

void DeleteCharactersGame5()
{
	if(nSizeCharacterGame5)
	{
		glDeleteTextures(nSizeCharacterGame5, uTexCharacterGame5);
		nSizeCharacterGame5 = 0;
		nRefCharacterGame5 = (int*)realloc(nRefCharacterGame5, nSizeCharacterGame5*sizeof(int));
		uTexCharacterGame5 = (GLuint*)realloc(uTexCharacterGame5, nSizeCharacterGame5*sizeof(GLuint));
	}
}

int LoadSongGame5()
{
	// ** Cargar datos basicos
	TraduceModCurTraining();
	ARGSTEP arg;
	arg.strMusic.Format(_T("song/%ls/%ls.mp3"), chSongsGame5[nCurSongGame5], chSongsGame5[nCurSongGame5]);
	arg.strLights.Format(_T("song/%ls/%ls.lgt"), chSongsGame5[nCurSongGame5], chSongsGame5[nCurSongGame5]);
	CString str;
	TCHAR ch[260];
	for(int x = 0; x < 2; x++) if(np[x])
	{
		str.Format(_T("song/%ls/%ls.acc"), chSongsGame5[nCurSongGame5], chSongsGame5[nCurSongGame5]);
		FILE* hFile = _fopen(str, _T("r"));
		if(hFile != NULL)
		{
			if( _fgets(ch, 260, hFile) != NULL)
			{
				arg.strBGA.Format(_T("song/%ls/%ls"), chSongsGame5[nCurSongGame5], ch);
				if(arg.strBGA.Find(_T('[')) != -1)
					arg.strBGA.Format(_T("song/GNRBGA/%ls"), ch);
				arg.strBGA.Remove(_T('\n')); arg.strBGA.Remove(_T('\r'));
			}
			else arg.strBGA.Format(_T("song/%ls/%ls.avi"), chSongsGame5[nCurSongGame5], chSongsGame5[nCurSongGame5]);
			fclose(hFile);
		}
		else arg.strBGA.Format(_T("song/%ls/%ls.avi"), chSongsGame5[nCurSongGame5], chSongsGame5[nCurSongGame5]);
		if(player_sesion[x]->nLockedAsp[ITEM_BGAOFF])
		{
			arg.strBGA = _T("");
			break;
		}
	}
	arg.strSplash.Format(_T("song/%ls/%ls.png"), chSongsGame5[nCurSongGame5], chSongsGame5[nCurSongGame5]);
	arg.bAutoPlay = false;
	if(!LoadSong(arg)) return 0;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		HideSesionTrayExp(x);
		arg.strFile.Format(_T("song/%ls/%ls_%ls.sksf"), chSongsGame5[nCurSongGame5], chSongsGame5[nCurSongGame5], training->m_strStep[nRefSongsGame5[nCurSongGame5]][0]);
		arg.dUserVel = 1.0;
		arg.bIsSingle = !training->m_bDouble[nRefSongsGame5[nCurSongGame5]]/*bIsSingleSongGame5[nCurSongGame5]*/;
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
		nCode = FoundPresetGame5(x, 0, 1, 7);
		if(nCode == -1)
			arg.dUserVel = 1.0;
		else
		{
			switch(nIndPresetGame5[x][nCode])	// 0*5+nCode
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
		nCode = FoundPresetGame5(x, 0, 8, 9);
		if(nCode != -1)
		{
			switch(nIndPresetGame5[x][nCode])	// 0*5+nCode
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
		nCode = FoundPresetGame5(x, 1, 10, 11);
		if(nCode != -1)
		{
			switch(nIndPresetGame5[x][5+nCode])	// 0*5+nCode
			{
			case 10:
				arg.nVanish = 5;
				break;
			case 11:
				arg.nVanish = 11;
				break;
			}
		}
		if(FoundPresetGame5(x, 1, 12, 12) != -1) arg.bFlash = true;
		if(FoundPresetGame5(x, 1, 13, 13) != -1) arg.bFreedom = true;
		// Familia 2
		nCode = FoundPresetGame5(x, 2, 24, 45);
		if(nCode != -1)
		{
			switch(nIndPresetGame5[x][10+nCode])
			{
			case 24:
				arg.bRandomSkin = true;
				break;
			default:
				arg.nSkin = nIndPresetGame5[x][10+nCode]-24;
				break;
			}
		}
		// Familia 5
		nCode = FoundPresetGame5(x, 5, 46, 59);
		if(nCode != -1)
		{
			switch(nIndPresetGame5[x][25+nCode])
			{
			case 46:
				arg.bCharacter = true;
				arg.nCharacter = (rand()%12)+1;
				arg.nLevelCharacer = 20; //????
				break;
			default:
				arg.bCharacter = true;
				arg.nCharacter = nIndPresetGame5[x][25+nCode]-46;
				arg.nLevelCharacer = 20; //????
				break;
			}
		}
		// Familia 3
		nCode = FoundPresetGame5(x, 3, 14, 15);
		if(nCode != -1)
		{
			switch(nIndPresetGame5[x][15+nCode])	// 0*5+nCode
			{
			case 14:
				arg.bRS = true;
				break;
			case 15:
				arg.bMirror = true;
				break;
			}
		}
		//if(FoundPresetGame5(x, 3, 16, 16) != -1);
		if(FoundPresetGame5(x, 3, 17, 17) != -1) arg.bTrainingMode = true;
		if(FoundPresetGame5(x, 3, 18, 18) != -1) arg.dPowTimer = POW_TIMER_HARD;
		// Familia 4
		if(FoundPresetGame5(x, 4, 19, 19) != -1) arg.bSnake = true;
		if(FoundPresetGame5(x, 4, 20, 20) != -1) arg.bNXMode = true;
		if(FoundPresetGame5(x, 4, 21, 21) != -1) arg.bXMode = true;
		nCode = FoundPresetGame5(x, 4, 22, 23);
		if(nCode != -1)
		{
			switch(nIndPresetGame5[x][20+nCode])	// 0*5+nCode
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
		if(training->m_uVersus[nRefSongsGame5[nCurSongGame5]] && !training->m_bDouble[nRefSongsGame5[nCurSongGame5]])
		{
			arg.bCharacter = true;
			arg.nCharacter = training->m_uVersus[nRefSongsGame5[nCurSongGame5]];
			arg.nLevelCharacer = training->m_uLvlVersus[nRefSongsGame5[nCurSongGame5]];
			arg.bAutoPlay = true;
			if(!LoadSongCompile(arg, int(!x)+1)) return 0;
		}
	}
	return 1;
}

void DeleteSongGame5()
{
	for(int x = 0; x < 2; x++)
	{
		if(np[x])
		{
			int nPercent = int(dNotesP[x]*100.0);
			if(player_sesion[x]->nAprovedTraining[nRefSongsGame5[nCurSongGame5]] < nPercent)
			{
				player_sesion[x]->nAprovedTraining[nRefSongsGame5[nCurSongGame5]] = nPercent;
			}
		}
		if(g_bPlayer[x])
		{
			DeleteSongCompile(x+1);
		}
	}
	DeleteSong();
}

CString GetIDTotalSongsGame5(int nIndex)
{
	if(nIndex >= nSizeTotalGame5) return CString(_T(""));
	CString str;
	str = ( chTotalSongsGame5[nIndex]);
	return str;
}

CString GetNameTotalSongsGame5(int nIndex)
{
	if(nIndex >= nSizeTotalGame5) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame5[nIndex], chTotalSongsGame5[nIndex], chLang);
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
	str = training->m_strName[nIndex]+_T(" - ")+str;
	return str;
}

CString GetArtistTotalSongsGame5(int nIndex)
{
	if(nIndex >= nSizeTotalGame5) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame5[nIndex], chTotalSongsGame5[nIndex], chLang);
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

CString GetBPMTotalSongsGame5(int nIndex)
{
	if(nIndex >= nSizeTotalGame5) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame5[nIndex], chTotalSongsGame5[nIndex], chLang);
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

CString GetNameSongsGame5(int nIndex)
{
	if(nIndex >= nSizeSongsGame5) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame5[nIndex], chSongsGame5[nIndex], chLang);
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
	str = training->m_strName[nRefSongsGame5[nIndex]]+_T(" - ")+str;
	return str;
}

CString GetArtistSongsGame5(int nIndex)
{
	if(nIndex >= nSizeSongsGame5) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame5[nIndex], chSongsGame5[nIndex], chLang);
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

CString GetBPMSongsGame5(int nIndex)
{
	if(nIndex >= nSizeSongsGame5) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame5[nIndex], chSongsGame5[nIndex], chLang);
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

void GetLevelSongsGame5(int nIndex, int* nLevel)
{
	if(nIndex >= nSizeSongsGame5)
	{
		(*nLevel) = 1;
		return;
	}
	if(training->m_nSizeStep[nIndex] <= 0)
	{
		(*nLevel) = 1;
		return;
	}
	TCHAR chStep[260];
	_strcpy(chStep, training->m_strStep[nIndex][0]);
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.sksf"), chSongsGame5[nIndex], chSongsGame5[nIndex], chStep);
	FILE* hFile = _fopen(chFile, _T("rb"));
	if(hFile == NULL)
	{
		(*nLevel) = 1;
		return;
	}
	STEPHEADER hStep;
	fread(&hStep, sizeof(STEPHEADER), 1, hFile);
	(*nLevel) = min(max(hStep.nDificulty, 1), 30);
	fclose(hFile);
}

void GetBestScoresGame5(int nIndex)
{
	nBestScoreMachineGame5 = 0;
	for(int x = 0; x < 2; x++)
	{
		if(np[x])
		{
			nBestScorePlayerGame5[x] = player_sesion[x]->nAprovedTraining[nRefSongsGame5[nIndex]];
		}
		else
		{
			nBestScorePlayerGame5[x] = 0;
		}
	}
//	strBestScoreMachineGame5 = ( machine_sesion->chBrainShowerRecords[nRefSongsGame5[nIndex]]);
}

int LoadDefaultsGame5(int nPlayer)
{
	if(!AdaptMachineProfile(1, nPlayer)) return 0;
	return 1;
}

int EnumTotalSongsGame5()
{
	FILE* hFile = NULL;
	hFile = _fopen(_T("songTrainingNew.acc"), _T("r"));
	if(hFile == NULL) return 0;
	TCHAR ch[500];
	int nChannel = 0;
	nSizeTotalGame5 = 0;
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
				nSizeTotalGame5++;
				int nCode = str.Find(_T(':'));
				_strcpy(chTotalSongsGame5[nSizeTotalGame5-1], str.Left(nCode));
				nChaTotalSongsGame5[nSizeTotalGame5-1] = nChannel==-1?0:nChannel;
				int n1;
				nCode = str.GetLength() - nCode - 1;
				if(_sscanf(str.Right(nCode), _T("%d"), &n1) != 1) {fclose(hFile); return 0;}
				bIsSingleTotalSongGame5[nSizeTotalGame5-1] = n1?true:false;
			}
		}
	}
	fclose(hFile);
	nSizeTotalGame5Ex = nSizeTotalGame5;
	return 1;
}

int EnumSongsGame5()
{
	bool bAutorizeDoubles = !(np[0]&&np[1]);
	nSizeSongsGame5 = 0;
	int nAutorize[1000];
	for(int k = 0; k < 1000; k++)
	{
		nAutorize[k] = 0;
		nChaSongsGame5[k] = 0;
	}
	for(int d = 0; d < 2; d++) if(np[d])
	{
		for(int s = 0; s < nSizeTotalGame5; s++)
		{
			if(bAutorizeDoubles || bIsSingleTotalSongGame5[s])
				nAutorize[s] = 1;
		}
	}
	for(int s = 0; s < nSizeTotalGame5; s++)
	{
		if(nAutorize[s])
		{
			nChaSongsGame5[nSizeSongsGame5] = nChaTotalSongsGame5[s];
			nRefSongsGame5[nSizeSongsGame5] = s;
			_strcpy(chSongsGame5[nSizeSongsGame5], chTotalSongsGame5[s]);
			bIsSingleSongGame5[nSizeSongsGame5] = bIsSingleTotalSongGame5[s];
			nSizeSongsGame5++;
		}
	}
	return 1;
}

#define MAX_TARGET_X 60.0
double AlphaFuncForXGame5(double dx)
{
	if(abs(dx) >= MAX_TARGET_X) return 0.0;
	if(abs(dx) <= (MAX_TARGET_X/4*3)) return 1.0;
	return (MAX_TARGET_X - abs(dx)) / (MAX_TARGET_X/4);
}

double ThetaFuncForXGame5(double dx)
{
	if(abs(dx) >= MAX_TARGET_X/4) return -19.0*dx/abs(dx);
	/*if(abs(dx) <= (MAX_TARGET_X/4))*/ return -19.0*dx*4/MAX_TARGET_X;
	//return (MAX_TARGET_X - abs(dx)) / (MAX_TARGET_X/4);
}


void ActualizateIndexShowGame5()
{
	for(int x = 0; x < 9; x++)
	{
		nIndexShowGame5[x] = nCurSongGame5+x-4;
		bool bAsegureLoop = true;
		while(bAsegureLoop)
		{
			bAsegureLoop = false;
			if(nIndexShowGame5[x] < 0)
			{
				nIndexShowGame5[x] = nSizeSongsGame5+nIndexShowGame5[x];
				bAsegureLoop = true;
			}
			if(nIndexShowGame5[x] >= nSizeSongsGame5)
			{
				nIndexShowGame5[x] = nIndexShowGame5[x]-nSizeSongsGame5;
				bAsegureLoop = true;
			}
		}
	}
}

void MoveSongsGame5(bool bIsIzq)
{
	int nb = nCurSongGame5;
	if(bIsIzq)
	{
		nCurSongGame5--;
		if(nCurSongGame5 < 0) nCurSongGame5 = nSizeSongsGame5-1;
		if(nChaSongsGame5[nb] != nChaSongsGame5[nCurSongGame5]) nTimeChaGame5 = nCountGame5;
		nModeGame5 = -1;
		nChosesGame5 = 0;
		/*Borrar textura sobrante*/
		ActualizateIndexShowGame5();
		//Mover a la derecha los items
		for(int x = 7; x >= 0; x--)
		{
			nTexCurSongGame5[x+1] = nTexCurSongGame5[x];
		}
		// Finalmente cargar la nueva imagen
		nTexCurSongGame5[0] = nTexTrainingGame5[nIndexShowGame5[0]];
		nTimeCurSongGame5 = nCountGame5;
	}
	else
	{
		nCurSongGame5++;
		if(nCurSongGame5 >= nSizeSongsGame5) nCurSongGame5 = 0;
		if(nChaSongsGame5[nb] != nChaSongsGame5[nCurSongGame5]) nTimeChaGame5 = nCountGame5;
		nModeGame5 = 1;
		nChosesGame5 = 0;
		/*Borrar textura sobrante*/
		ActualizateIndexShowGame5();
		//Mover a la izquierda los items
		for(int x = 0; x < 8; x++)
		{
			nTexCurSongGame5[x] = nTexCurSongGame5[x+1];
		}
		// Finalmente cargar la nueva imagen
		nTexCurSongGame5[8] = nTexTrainingGame5[nIndexShowGame5[8]];
		nTimeCurSongGame5 = nCountGame5;
	}
}

int FoundPresetGame5(int x, int nFamily, int nBegin, int nEnd)
{
	int nFound = -1;
	for(int k = 0; k < nIndSizePresetGame5[x][nFamily]; k++)
	{
		if(nIndPresetGame5[x][nFamily*5+k] >= nBegin && nIndPresetGame5[x][nFamily*5+k] <= nEnd)
		{
			nFound = k;
			break;
		}
	}
	return nFound;
}

void CheckPresetGame5()
{
	for(int x = 0; x < 2; x++) if(np[x])
	{
		bool bPreset = false;
		int nFamily;
		// ** Primer nivel (0)
		nFamily = 0;
		// Speed Up
		if(nComandGame5[x][0] == 2 &&
		   nComandGame5[x][1] == 3 &&
		   nComandGame5[x][2] == 1 &&
		   nComandGame5[x][3] == 3 &&
		   nComandGame5[x][4] == 1 && !bPreset)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 1, 7);
			int nCurVel = 0;
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nCurVel = 0;
			}
			else nCurVel = nIndPresetGame5[x][nFamily*5+nFound];
			nCurVel++;
			if(nCurVel > 5 || nCurVel < 1)
			{
				nIndPresetGame5[x][nFamily*5+nFound] = 0;
				for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
					nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
				nIndSizePresetGame5[x][nFamily]--;
			}
			else
			{
				nIndPresetGame5[x][nFamily*5+nFound] = nCurVel;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
		}
		// Speed Down
		if(nComandGame5[x][0] == 2 &&
		   nComandGame5[x][1] == 1 &&
		   nComandGame5[x][2] == 3 &&
		   nComandGame5[x][3] == 1 &&
		   nComandGame5[x][4] == 3)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 1, 7);
			int nCurVel = 6;
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nCurVel = 6;
			}
			else nCurVel = nIndPresetGame5[x][nFamily*5+nFound];
			nCurVel--;
			if(nCurVel > 5 || nCurVel < 1)
			{
				nIndPresetGame5[x][nFamily*5+nFound] = 0;
				for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
					nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
				nIndSizePresetGame5[x][nFamily]--;
			}
			else
			{
				nIndPresetGame5[x][nFamily*5+nFound] = nCurVel;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
		}
		// ** Segundo nivel (1)
		nFamily = 1;
		// Vanish & NonStep (V & NS)
		if(nComandGame5[x][0] == 2 &&
		   nComandGame5[x][1] == 4 &&
		   nComandGame5[x][2] == 0 &&
		   nComandGame5[x][3] == 3 &&
		   nComandGame5[x][4] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 10, 11);
			int nCur = 0;
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nCur = 9;
			}
			else nCur = nIndPresetGame5[x][nFamily*5+nFound];
			nCur++;
			if(nCur > 11 || nCur < 10)
			{
				nIndPresetGame5[x][nFamily*5+nFound] = 0;
				for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
					nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
				nIndSizePresetGame5[x][nFamily]--;
			}
			else
			{
				nIndPresetGame5[x][nFamily*5+nFound] = nCur;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
		}
		// Flash (FL)
		if(nComandGame5[x][0] == 2 &&
		   nComandGame5[x][1] == 3 &&
		   nComandGame5[x][2] == 1 &&
		   nComandGame5[x][3] == 4 &&
		   nComandGame5[x][4] == 3 &&
		   nComandGame5[x][5] == 0 &&
		   nComandGame5[x][6] == 4 &&
		   nComandGame5[x][7] == 1 &&
		   nComandGame5[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 12, 12);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nIndPresetGame5[x][nFamily*5+nFound] = 12;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
			else
			{
				if(nIndPresetGame5[x][nFamily*5+nFound] == 12)	// Desactivar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
						nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
					nIndSizePresetGame5[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 12;
					nIndLastPresetGame5[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame5[x] = nCountGame5;
				}
			}
		}
		// Freedom (FD)
		if(nComandGame5[x][0] == 2 &&
		   nComandGame5[x][1] == 0 &&
		   nComandGame5[x][2] == 3 &&
		   nComandGame5[x][3] == 1 &&
		   nComandGame5[x][4] == 4 &&
		   nComandGame5[x][5] == 4 &&
		   nComandGame5[x][6] == 3 &&
		   nComandGame5[x][7] == 0 &&
		   nComandGame5[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 13, 13);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nIndPresetGame5[x][nFamily*5+nFound] = 13;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
			else
			{
				if(nIndPresetGame5[x][nFamily*5+nFound] == 13)	// Desactivar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
						nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
					nIndSizePresetGame5[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 13;
					nIndLastPresetGame5[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame5[x] = nCountGame5;
				}
			}
		}
		// Reverse Grade (RG)
		if(nComandGame5[x][0] == 2 &&
		   nComandGame5[x][1] == 1 &&
		   nComandGame5[x][2] == 3 &&
		   nComandGame5[x][3] == 2 &&
		   nComandGame5[x][4] == 0 &&
		   nComandGame5[x][5] == 4 &&
		   nComandGame5[x][6] == 4 &&
		   nComandGame5[x][7] == 0 &&
		   nComandGame5[x][8] == 2 &&
		   nComandGame5[x][9] == 3 &&
		   nComandGame5[x][10] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 16, 16);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nIndPresetGame5[x][nFamily*5+nFound] = 16;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
			else
			{
				if(nIndPresetGame5[x][nFamily*5+nFound] == 16)	// Desactivar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
						nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
					nIndSizePresetGame5[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 16;
					nIndLastPresetGame5[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame5[x] = nCountGame5;
				}
			}
		}
		// ** Quinto Nivel (4)
		nFamily = 4;
		// Snake (SN)
		if(nComandGame5[x][0] == 0 &&
		   nComandGame5[x][1] == 4 &&
		   nComandGame5[x][2] == 2 &&
		   nComandGame5[x][3] == 1 &&
		   nComandGame5[x][4] == 3 &&
		   nComandGame5[x][5] == 0 &&
		   nComandGame5[x][6] == 4 &&
		   nComandGame5[x][7] == 2 &&
		   nComandGame5[x][8] == 1 &&
		   nComandGame5[x][9] == 3)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 19, 19);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nIndPresetGame5[x][nFamily*5+nFound] = 19;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
			else
			{
				if(nIndPresetGame5[x][nFamily*5+nFound] == 19)	// Desactivar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
						nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
					nIndSizePresetGame5[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 19;
					nIndLastPresetGame5[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame5[x] = nCountGame5;
				}
			}
		}
		// NX Mode (NX)
		if(nComandGame5[x][0] == 3 &&
		   nComandGame5[x][1] == 4 &&
		   nComandGame5[x][2] == 2 &&
		   nComandGame5[x][3] == 1 &&
		   nComandGame5[x][4] == 0 &&
		   nComandGame5[x][5] == 3 &&
		   nComandGame5[x][6] == 4 &&
		   nComandGame5[x][7] == 2 &&
		   nComandGame5[x][8] == 1 &&
		   nComandGame5[x][9] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 20, 20);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nIndPresetGame5[x][nFamily*5+nFound] = 20;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
			else
			{
				if(nIndPresetGame5[x][nFamily*5+nFound] == 20)	// Desactivar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
						nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
					nIndSizePresetGame5[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 20;
					nIndLastPresetGame5[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame5[x] = nCountGame5;
				}
			}
		}
		// X Mode (X)
		if(nComandGame5[x][0] == 2 &&
		   nComandGame5[x][1] == 1 &&
		   nComandGame5[x][2] == 4 &&
		   nComandGame5[x][3] == 1 &&
		   nComandGame5[x][4] == 4 &&
		   nComandGame5[x][5] == 3 &&
		   nComandGame5[x][6] == 0 &&
		   nComandGame5[x][7] == 3 &&
		   nComandGame5[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 21, 21);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nIndPresetGame5[x][nFamily*5+nFound] = 21;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
			else
			{
				if(nIndPresetGame5[x][nFamily*5+nFound] == 21)	// Desactivar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
						nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
					nIndSizePresetGame5[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 21;
					nIndLastPresetGame5[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame5[x] = nCountGame5;
				}
			}
		}
		// UnderAttack (UA)
		if(nComandGame5[x][0] == 2 &&
		   nComandGame5[x][1] == 3 &&
		   nComandGame5[x][2] == 2 &&
		   nComandGame5[x][3] == 0 &&
		   nComandGame5[x][4] == 2 &&
		   nComandGame5[x][5] == 0 &&
		   nComandGame5[x][6] == 2 &&
		   nComandGame5[x][7] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 22, 23);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nIndPresetGame5[x][nFamily*5+nFound] = 22;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
			else
			{
				if(nIndPresetGame5[x][nFamily*5+nFound] == 22)	// Desactivar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
						nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
					nIndSizePresetGame5[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 22;
					nIndLastPresetGame5[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame5[x] = nCountGame5;
				}
			}
		}
		// Drop (DR)
		if(nComandGame5[x][0] == 4 &&
		   nComandGame5[x][1] == 4 &&
		   nComandGame5[x][2] == 3 &&
		   nComandGame5[x][3] == 0 &&
		   nComandGame5[x][4] == 1 &&
		   nComandGame5[x][5] == 4 &&
		   nComandGame5[x][6] == 3 &&
		   nComandGame5[x][7] == 0 &&
		   nComandGame5[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame5(x, nFamily, 22, 23);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame5[x][nFamily];
				nIndSizePresetGame5[x][nFamily]++;
				nIndPresetGame5[x][nFamily*5+nFound] = 23;
				nIndLastPresetGame5[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame5[x] = nCountGame5;
			}
			else
			{
				if(nIndPresetGame5[x][nFamily*5+nFound] == 23)	// Desactivar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame5[x][nFamily]-1); k++)
						nIndPresetGame5[x][nFamily*5+k] = nIndPresetGame5[x][nFamily*5+k+1];
					nIndSizePresetGame5[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame5[x][nFamily*5+nFound] = 23;
					nIndLastPresetGame5[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame5[x] = nCountGame5;
				}
			}
		}
		// Toggle Null
		if(nComandGame5[x][0] == 4 &&
		   nComandGame5[x][1] == 0 &&
		   nComandGame5[x][2] == 4 &&
		   nComandGame5[x][3] == 0 &&
		   nComandGame5[x][4] == 4 &&
		   nComandGame5[x][5] == 0)
		{
			bPreset = true;
			for(int z = 0; z < 30; z++) nIndPresetGame5[x][z] = 0;
			for(int z = 0; z < 6; z++) nIndSizePresetGame5[x][z] = 0;
			nIndLastPresetGame5[x] = -1;
			nTimeLastPresetGame5[x] = nCountGame5;
		}
		if(bPreset)
		{
			nChosesGame5 = 0;
			nComandGame5[x][0] = -1;
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_option, 0, &channels[2]);
		}
	}
}

#define TIME_FAST_CHANGE_GAME5 1000
#define TIME_CHANGE_LOW_GAME5 350
#define TIME_CHANGE_HIGH_GAME5 100

void PlaySoundTransiAboutLimitGame5(int nLimit)
{
	if(nLimit >= TIME_CHANGE_LOW_GAME5)
	{
		nTimeTransiSongBorderGame5 = TIME_TRANSI_SONGBORDER_GAME5;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
	}
	else
	{
		nTimeTransiSongBorderGame5 = TIME_CHANGE_HIGH_GAME5;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)6, sound_change2, 0, &channels[6]);
	}
}

#define DY_SONGBORDER_HOLD_GAME5 -3.2

int DrawInterfaceSelectGame5()
{
	if(!np[0] && !np[1]) return 1;
	nCountGame5 += dwTick-dwLastTick;
	if(movie_trainingbg->bLoad)
	{
		movie_trainingbg->Update(dwWait);
		movie_trainingbg->GrabAVIFrame();
		glBindTexture(GL_TEXTURE_2D, movie_trainingbg->tex);

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
	__int64 nTransBorder = nCountGame5 - nTimeCurSongGame5;
	int nOrder[] = {0,1,2,3,8,7,6,5,4};
	if(nChangeRetroGame5 == 0)
	{
		if(nChosesGame5 < CHOOSES_GAME5)
		{
			for(int i = 0; i < 9; i++)
			{
				int x = nOrder[i];
				double dTargetx = double(x-4)*MAX_TARGET_X/4;
				if(nCountGame5 >= TIME_START_SELECTOR_GAME5 && nCountGame5 < (TIME_START_SELECTOR_GAME5+TIME_TRANSI_SELECTOR_GAME5))
				{
					if(x == 4)
					{
						if(nCountGame5 < (TIME_TRANSI_START_MAIN_GAME5+TIME_START_SELECTOR_GAME5))
						{
							dApt = pow(double(nCountGame5-TIME_START_SELECTOR_GAME5)/TIME_TRANSI_START_MAIN_GAME5, 2);
							glPushMatrix();
							glTranslated(0.0, DY_SONGBORDER_HOLD_GAME5, -60.0);
							glScaled(1.0+(1-dApt), 1.0+(1-dApt), 1.0);
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
						if(nCountGame5 >= (TIME_TRANSI_START_MAIN_GAME5+TIME_START_SELECTOR_GAME5))
						{
							glPushMatrix();
							glTranslated(0.0, DY_SONGBORDER_HOLD_GAME5, -60.0);
							glScaled(1.0, 1.0, 1.0);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
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
						__int64 nStartTransi = TIME_START_SELECTOR_GAME5+TIME_TRANSI_SELECTOR_GAME5/4*(abs(x-4)-1);
						__int64 nLimitTransi = TIME_START_SELECTOR_GAME5+TIME_TRANSI_SELECTOR_GAME5/4*(abs(x-4));
						if(nCountGame5 >= nStartTransi && nCountGame5 < nLimitTransi)
						{
							dApt = double(nCountGame5-nStartTransi)/double(nLimitTransi-nStartTransi);
							double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(1.0-dApt)+dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0+DY_SONGBORDER_HOLD_GAME5;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXGame5(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.0;
							double dalpha = AlphaFuncForXGame5(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
						if(nCountGame5 >= nLimitTransi)
						{
							double dx = dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0+DY_SONGBORDER_HOLD_GAME5;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXGame5(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.0;
							double dalpha = AlphaFuncForXGame5(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
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
				if(nCountGame5 >= (TIME_START_SELECTOR_GAME5+TIME_TRANSI_SELECTOR_GAME5))
				{
					double dx = dTargetx;
					if(nModeGame5!=0)
					{
						if(nTransBorder < nTimeTransiSongBorderGame5)
						{
							dx += double(nModeGame5)*MAX_TARGET_X/4*(1.0-double(nTransBorder)/double(nTimeTransiSongBorderGame5));
						}
					}
#define TIME_SHOWIMAGE_SONGBORDER_GAME5 500
#define TIME_TRANSI_SHOWIMAGE_GAME5 500
					double dApt2;
					if(nTransBorder <= (TIME_SHOW_PREVIEW_GAME5))
					{
						dApt2 = 0.0;
					}
					if(nTransBorder > (TIME_SHOW_PREVIEW_GAME5) && nTransBorder <= (TIME_SHOW_PREVIEW_GAME5+TIME_TRANSI_SHOWIMAGE_GAME5))
					{
						dApt2 = pow(double(nTransBorder-TIME_SHOW_PREVIEW_GAME5)/TIME_TRANSI_SHOWIMAGE_GAME5, 0.5);
					}
					if(nTransBorder > (TIME_SHOW_PREVIEW_GAME5+TIME_TRANSI_SHOWIMAGE_GAME5))
					{
						dApt2 = 1.0;
					}
					double dy = pow(dx/MAX_TARGET_X,3.0)*50.0+DY_SONGBORDER_HOLD_GAME5;	// 0.75*40.0
					double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
					double dtheta = ThetaFuncForXGame5(dx);
					double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.0;
					double dalpha = AlphaFuncForXGame5(dx);
					glPushMatrix();
					glTranslated(dx, dy, dz-60.0+(x==4?(10.0*dApt2):0.0));
					glRotated(dtheta, 0.0, 1.0, 0.0);
					glScaled(dscale, dscale, 1.0);
					glColor4d(1.0, 1.0, 1.0, dalpha);
					if(x == 4 && bShow)
					{
						glPushMatrix();

						if(nTransBorder <= (TIME_SHOW_PREVIEW_GAME5+TIME_TRANSI_SHOWIMAGE_GAME5))
						{
							dApt = pow(double(nTransBorder-TIME_SHOW_PREVIEW_GAME5)/TIME_TRANSI_SHOWIMAGE_GAME5, 0.5);
						}
						if(nTransBorder > (TIME_SHOW_PREVIEW_GAME5+TIME_TRANSI_SHOWIMAGE_GAME5))
						{
							dApt = 1.0;
						}
						glTranslated(-13.0*dApt, -4.0*dApt, 0.0);

						if(nTransBorder > TIME_SHOW_PREVIEW_GAME5 && movie_songpreviewgame5->bLoad && movie_songpreviewgame5->bResize)
						{
							movie_songpreviewgame5->Update(dwWait);
							movie_songpreviewgame5->GrabAVIFrame();
							__int64 nCaretCover = movie_songpreviewgame5->dwNext;
							glBindTexture(GL_TEXTURE_2D, movie_songpreviewgame5->tex);
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
							glEnd();
							if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME5)
							{
								dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME5;
								glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
								glBindTexture(GL_TEXTURE_2D, nTexCurGame5);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
								glColor4d(1.0, 1.0, 1.0, 1.0);
							}
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
						}
						else
						{
							glBindTexture(GL_TEXTURE_2D, nTexCurGame5);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
						}
						glPopMatrix();
					}
					if(x == 4) glPushMatrix();
					dApt = dApt2;
					if(x == 4) glTranslated(13.0*dApt, 4.0*dApt, 0.0);
					glColor4d(1.0, 1.0, 1.0, dalpha);
					glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					if(x == 4 && nTransBorder > TIME_SHOW_PREVIEW_GAME5)
					{
						#define TIME_SHOWNAME_BESTSCORE_GAME5 500
						if(nTransBorder < (TIME_SHOW_PREVIEW_GAME5+TIME_SHOWNAME_BESTSCORE_GAME5))
						{
							dApt = double(nTransBorder-TIME_SHOW_PREVIEW_GAME5)/TIME_SHOWNAME_BESTSCORE_GAME5;
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[166]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						if(nTransBorder >= (TIME_SHOW_PREVIEW_GAME5+TIME_SHOWNAME_BESTSCORE_GAME5))
						{
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[166]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
						}
						__int64 nModeTime = nCountGame5 - nTransBorder-TIME_SHOW_PREVIEW_GAME5;
						if(nModeTime >= 0 && nModeTime < (TIME_SHOWNAME_BESTSCORE_GAME5) && bShow)
						{
							dApt = double(nModeTime)/TIME_SHOWNAME_BESTSCORE_GAME5;
							glColor4d(1.0, 1.0, 1.0, dApt);
							/*if(nBestScoreMachineGame5 != 0)
							{
								font_Micro->glDrawText(10.599, -9.0625, 9.05331, 9.0625, 0.0, 10.076, 0.0, 1.0, 1.5, strBestScoreMachineGame5, DT_CENTER|DT_VCENTER);
								CString str;
								str.Format(_T("%d%%"), nBestScoreMachineGame5);
								font_Micro->glDrawText(9.05331, -9.0625, 7.50762, 9.0625, 0.0, 8.53046, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
							}*/
							for(int k = 0; k < 2; k++) if(np[k])
							{
								if(nBestScorePlayerGame5[k] != 0)
								{
									CString str;
									str.Format(_T("%d%%"), nBestScorePlayerGame5[k]);
									font_Micro->glDrawText(-9.05331, -9.0625+(k?7.25:-7.25), -10.599, 9.0625+(k?7.25:-7.25), (k?7.25:-7.25), -9.57615, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
								}
							}
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						if(nModeTime >= (TIME_SHOWNAME_BESTSCORE_GAME5) && bShow)
						{
							/*if(nBestScoreMachineGame5 != 0)
							{
								font_Micro->glDrawText(10.599, -9.0625, 9.05331, 9.0625, 0.0, 10.076, 0.0, 1.0, 1.5, strBestScoreMachineGame5, DT_CENTER|DT_VCENTER);
								CString str;
								str.Format(_T("%d%%"), nBestScoreMachineGame5);
								font_Micro->glDrawText(9.05331, -9.0625, 7.50762, 9.0625, 0.0, 8.53046, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
							}*/
							for(int k = 0; k < 2; k++) if(np[k])
							{
								if(nBestScorePlayerGame5[k] != 0)
								{
									CString str;
									str.Format(_T("%d%%"), nBestScorePlayerGame5[k]);
									font_Micro->glDrawText(-9.05331, -9.0625+(k?7.25:-7.25), -10.599, 9.0625+(k?7.25:-7.25), (k?7.25:-7.25), -9.57615, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
								}
							}
						}
					}
					if(x == 4) glPopMatrix();
					glPopMatrix();
				}
			}
			glPushMatrix();
			glLoadIdentity();
			if(nTransBorder >= 0 && nTransBorder <= (TIME_SHOW_PREVIEW_GAME5))
			{
				dApt = 0.0;
			}
			if(nTransBorder > (TIME_SHOW_PREVIEW_GAME5) && nTransBorder <= (TIME_SHOW_PREVIEW_GAME5+TIME_TRANSI_SHOWIMAGE_GAME5))
			{
				dApt = pow(double(nTransBorder-TIME_SHOW_PREVIEW_GAME5)/TIME_TRANSI_SHOWIMAGE_GAME5, 0.5);
			}
			if(nTransBorder > (TIME_SHOW_PREVIEW_GAME5+TIME_TRANSI_SHOWIMAGE_GAME5))
			{
				dApt = 1.0;
			}
			glTranslated(13.0*dApt, 4.0*dApt, 0.0);
			if(nTransBorder >= 0 && nTransBorder <= TIME_GLOUTH_SONGBORDER_GAME5 && nChosesGame5 == 0)
			{
				dApt = double(nTransBorder)/TIME_GLOUTH_SONGBORDER_GAME5;
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[11]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
				glTranslated(0.0, DY_SONGBORDER_HOLD_GAME5, 0.0);
				glScaled(1.0+0.5*(dApt), 1.0+0.5*(dApt), 1.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(nTransBorder > TIME_TRANSI_SONGBORDER_GAME5 && nTransBorder <= (TIME_TRANSI_SONGBORDER_GAME5+TIME_GLOUTH_SONGBORDER_GAME5) && nChosesGame5 == 0)
			{
				dApt = double(nTransBorder-TIME_TRANSI_SONGBORDER_GAME5)/TIME_GLOUTH_SONGBORDER_GAME5;
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[11]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, dApt);
				glTranslated(0.0, DY_SONGBORDER_HOLD_GAME5, 0.0);
				glScaled(1.0+0.5*(1-dApt), 1.0+0.5*(1-dApt), 1.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
	#define TIME_GLOUTH_PER_SONGBORDER_GAME5 1500
			if(nTransBorder > (TIME_TRANSI_SONGBORDER_GAME5+TIME_GLOUTH_SONGBORDER_GAME5) && nChosesGame5 == 0)
			{
				dApt = abs(double((nTransBorder-TIME_TRANSI_SONGBORDER_GAME5-TIME_GLOUTH_SONGBORDER_GAME5)%TIME_GLOUTH_PER_SONGBORDER_GAME5-TIME_GLOUTH_PER_SONGBORDER_GAME5/2))*2/TIME_GLOUTH_PER_SONGBORDER_GAME5;
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[11]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, dApt);
				glTranslated(0.0, DY_SONGBORDER_HOLD_GAME5, 0.0);
				glScaled(1.0+0.1*(1-dApt), 1.0+0.1*(1-dApt), 1.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(nTransBorder > (TIME_TRANSI_SONGBORDER_GAME5+TIME_GLOUTH_SONGBORDER_GAME5) && nChosesGame5 == 1)
			{
				welres->skin[0]->Update(dwWait);
				welres->skin[1]->Update(dwWait);
				welres->skin[3]->Update(dwWait);
				dApt = double((nCountGame5-nTimeChosesGame5[nChosesGame5-1])%TIME_GLOUTH_SONGBORDER_GAME5)/TIME_GLOUTH_SONGBORDER_GAME5;
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[11]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
				glTranslated(0.0, DY_SONGBORDER_HOLD_GAME5, 0.0);
				glScaled(1.0+0.4*(dApt), 1.0+0.4*(dApt), 1.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, -50.0);
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
			glPopMatrix();
			if(nTransBorder >= TIME_SHOW_PREVIEW_GAME5)
			{
				if(!bShow)
				{
					TCHAR chFile[260];
					_sprintf(chFile, _T("song/%ls/%ls_P.avi"), chSongsGame5[nCurSongGame5], chSongsGame5[nCurSongGame5]);
					if(movie_songpreviewgame5->bLoad)
					{
						movie_songpreviewgame5->CloseAVI();
						delete movie_songpreviewgame5;
						movie_songpreviewgame5 = new CMovieStream2();
					}
					movie_songpreviewgame5->OpenAVI(chFile);
					movie_songpreviewgame5->bLoop = true;
					nTimeShow = nCountGame5;
					bShow = true;
					_sprintf(chFile, _T("song/%ls/%ls_P.mp3"), chSongsGame5[nCurSongGame5], chSongsGame5[nCurSongGame5]);
					if(bLoadMusicCoverGame5)
					{
						bLoadMusicCoverGame5 = false;
						FMOD_Sound_Release(music_coverGame5);
					}

					TCHAR chImg[260];
					_sprintf(chImg, _T("song/preview/%ls.PNG"), chSongsGame5[nCurSongGame5]);
					nTexCurGame5 = GenTexImage(chImg);

					FMOD_RESULT resc;
	#ifdef UNICODE
					char chAnsiFile[260];
					UnicodeToAnsi(chFile, chAnsiFile);
					resc = FMOD_System_CreateStream(sys, chAnsiFile, FMOD_HARDWARE | FMOD_2D, 0, &music_coverGame5);
	#else
					resc = FMOD_System_CreateStream(sys, chFile, FMOD_HARDWARE | FMOD_2D, 0, &music_coverGame5);
	#endif
					if(ERRCHECK(resc))
					{
						bLoadMusicCoverGame5 = true;
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, music_coverGame5, 0, &channels[9]);
					}
					else
					{
						bLoadMusicCoverGame5 = false;
						_printf(_T("No se puede cargar el archivo \"%ls\"\n"), chFile);
					}
					strDispGame5.Format(_T("%ls - %ls - %ls"), GetNameSongsGame5(nCurSongGame5), GetArtistSongsGame5(nCurSongGame5), GetBPMSongsGame5(nCurSongGame5));
					for(int k = 0; k < 2; k++) if(np[k])
					{
						GetLevelSongsGame5(nCurSongGame5, &nLevelCurGame5[k]);
					}
					GetBestScoresGame5(nCurSongGame5);
				}
			}



	#define TIME_START_UPDO_GAME5 0
	#define TIME_TRANSI_UPDO_GAME5 500
	#define TIME_GLOUTH_UPDO_GAME5 500
			if(nCountGame5 >= TIME_START_UPDO_GAME5 && nCountGame5 < (TIME_START_UPDO_GAME5+TIME_TRANSI_UPDO_GAME5))	// Transi Up and Down Game5
			{
				dApt = (double(nCountGame5)-TIME_START_UPDO_GAME5)/(TIME_TRANSI_UPDO_GAME5);
				glPushMatrix();
				glScaled(1.0+0.8*(1.0-dApt), 1.0+0.8*(1.0-dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[41+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[23+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[3]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[4]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[5]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[6]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625, -29.75, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nCountGame5 >= (TIME_START_UPDO_GAME5+TIME_TRANSI_UPDO_GAME5))	// Up and Down Game5
			{
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[41+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				if(nTransBorder > TIME_SHOW_PREVIEW_GAME5 && bShow)
				{
	#define SIZE_TEXT_GAME5 2
	#define VEL_SHOWTEXT_GAME5 0.01
	#define TIME_WAITTEXTNAME_GAME5 1500
					__int64 nTimeName = nTransBorder-TIME_SHOW_PREVIEW_GAME5-TIME_WAITTEXTNAME_GAME5;
					if(nTimeName < 0) nTimeName = 0;
					double dSizeTex = font_Car->CalcSingleLine(strDispGame5, SIZE_TEXT_GAME5);
					DWORD t1 = DWORD(double(48.0/VEL_SHOWTEXT_GAME5));
					DWORD t2 = DWORD(double(dSizeTex/VEL_SHOWTEXT_GAME5));
					DWORD t = t1+t2;
					DWORD nShowText = (DWORD(nTimeName)+t1)%t;
					double dx = -VEL_SHOWTEXT_GAME5*double(nShowText)+39.0;
					font_Car->glDrawText(-24.1, -17.9, -27.6, 17.9, -8.375+dx, -25.9, -50.0, 1.0, SIZE_TEXT_GAME5, strDispGame5, DT_VCENTER);
				}
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[23+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();

				DWORD dwNext = 0;
				DWORD dwFrame = 0;

				dwNext = (DWORD)(nCountGame5-max(nTimeEnArrowsGame5[1],nTimeEnArrowsGame5[6]));
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame5)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga5res->skin[3]->m_nSize)/(DWORD)nTimeTransiSongBorderGame5;
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[3]->texture[dwFrame]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame5-max(nTimeEnArrowsGame5[3],nTimeEnArrowsGame5[8]));
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame5)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga5res->skin[4]->m_nSize)/(DWORD)nTimeTransiSongBorderGame5;
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[4]->texture[dwFrame]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame5-max(nTimeEnArrowsGame5[0],nTimeEnArrowsGame5[5]));
				if(bEnArrowsGame5[0] || bEnArrowsGame5[5]) dwNext = dwNext%nTimeTransiSongBorderGame5;
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame5)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga5res->skin[5]->m_nSize)/(DWORD)nTimeTransiSongBorderGame5;
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[5]->texture[dwFrame]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame5-max(nTimeEnArrowsGame5[4],nTimeEnArrowsGame5[9]));
				if(bEnArrowsGame5[4] || bEnArrowsGame5[9]) dwNext = dwNext%nTimeTransiSongBorderGame5;
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame5)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga5res->skin[6]->m_nSize)/(DWORD)nTimeTransiSongBorderGame5;
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[6]->texture[dwFrame]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625, -29.75, -50.0);
				glEnd();

	#define TIME_TRANSI_SELECTORMODE_GAME5 300
	#define TIME_START_GRAPH_GAME5 100
	#define TIME_TRANSI_GRAPH_GAME5 500
	#define TIME_GLOUTH_GRAPH_GAME5 300
	#define TIME_START_LINEGRAPH_GAME5 100
	#define TIME_TRANSI_LINEGRAPH_GAME5 500
	#define TIME_TRUNC_LINEGRAPH_GAME5 1200
				/*Dibujo de la elección*/
				for(int k = 0; k < 2; k++) if(np[k])
				{
					__int64 nTransSelectM = nTransBorder;

	#define TIME_START_LR_GAME5 0
	#define TIME_TRANSI_LR_GAME5 500
	#define TIME_GLOUTH_LR_GAME5 500
					__int64 nCountSide = nCountGame5-(TIME_START_UPDO_GAME5+TIME_TRANSI_UPDO_GAME5);
					if(nCountSide >= TIME_START_LR_GAME5 && nCountSide <= (TIME_START_LR_GAME5+TIME_TRANSI_LR_GAME5))
					{
						dApt = pow(double(nCountSide-TIME_START_LR_GAME5)/TIME_TRANSI_LR_GAME5, 0.5);
						glPushMatrix();
						glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(1-dApt), 0.0, 0.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBindTexture(GL_TEXTURE_2D, ga5res->skin[(k?68:50)+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
	#define TIME_TRANSI_PRESET_GAME5 500
	#define TIME_GLOUTH_PRESET_GAME5 500
	#define TIME_SHOW_PRESET_GAME5 1500
					if(nCountSide > (TIME_START_LR_GAME5+TIME_TRANSI_LR_GAME5))
					{
						glPushMatrix();
						glTranslated(k?40.0:-40.0, 0.0, 0.0);
						glBindTexture(GL_TEXTURE_2D, ga5res->skin[(k?68:50)+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
						glEnd();
						glPopMatrix();
						__int64 nCountPreset = nCountGame5-nTimeLastPresetGame5[k];
						for(int q = 0; q < 6; q++)
						{
							double dAdv = 0.0;
							for(int w = 0; w < nIndSizePresetGame5[k][q]; w++)
							{
								int nInd = (q*5+w);
								if(nInd == nIndLastPresetGame5[k] || nIndLastPresetGame5[k] == -1)
								{
									__int64 nCountPreset = nCountGame5-nTimeLastPresetGame5[k];
									if(nCountPreset >= 0 && nCountPreset < TIME_TRANSI_PRESET_GAME5)
									{
										glPushMatrix();
										dApt = double(nCountPreset)/TIME_TRANSI_PRESET_GAME5;
										glLoadIdentity();
										glColor4d(1.0, 1.0, 1.0, dApt);
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.5+2.0*(1-dApt), 1.5+2.0*(1-dApt), 1.0);
										glBindTexture(GL_TEXTURE_2D, ga5res->skin[108+nIndPresetGame5[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
										glScaled(1.0+2.0*(1-dApt), 1.0, 1.0);
										glBindTexture(GL_TEXTURE_2D, ga5res->skin[108+nIndPresetGame5[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
										glBindTexture(GL_TEXTURE_2D, ga5res->skin[106]->GetCurrentTex());
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
										glBindTexture(GL_TEXTURE_2D, ga5res->skin[104]->GetCurrentTex());
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
									if(nCountPreset >= TIME_TRANSI_PRESET_GAME5 && nCountPreset < (TIME_TRANSI_PRESET_GAME5+TIME_SHOW_PRESET_GAME5))
									{
										glPushMatrix();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.5, 1.5, 1.0);
										glBindTexture(GL_TEXTURE_2D, ga5res->skin[108+nIndPresetGame5[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga5res->skin[108+nIndPresetGame5[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga5res->skin[106]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										if(nCountPreset < (TIME_TRANSI_PRESET_GAME5+TIME_GLOUTH_PRESET_GAME5))
										{
											dApt = double(nCountPreset - TIME_TRANSI_PRESET_GAME5)/TIME_GLOUTH_PRESET_GAME5;
											glLoadIdentity();
											glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
											glScaled(dApt*4.0, dApt*4.0, 1.0);
											glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
											glBindTexture(GL_TEXTURE_2D, ga5res->skin[103]->GetCurrentTex());
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
									if(nCountPreset >= (TIME_TRANSI_PRESET_GAME5+TIME_SHOW_PRESET_GAME5))
									{
										glPushMatrix();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.5, 1.5, 1.0);
										glBindTexture(GL_TEXTURE_2D, ga5res->skin[108+nIndPresetGame5[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME5+TIME_SHOW_PRESET_GAME5))
										{
											dApt = double(nCountPreset-(TIME_TRANSI_PRESET_GAME5+TIME_SHOW_PRESET_GAME5))/TIME_TRANSI_PRESET_GAME5;
											glLoadIdentity();
											glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
											glScaled(1.0+2.0*(dApt), 1.0, 1.0);
											glColor4d(1.0, 1.0, 1.0, 1-dApt);
											glBindTexture(GL_TEXTURE_2D, ga5res->skin[108+nIndPresetGame5[k][nInd]]->GetCurrentTex());
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
										glBindTexture(GL_TEXTURE_2D, ga5res->skin[106]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glPopMatrix();
										if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME5+TIME_SHOW_PRESET_GAME5)) dAdv += (1-dApt)*9.0;
										dAdv += 4.5;
									}
								}
								else
								{
									glPushMatrix();
									glLoadIdentity();
									glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
									glScaled(1.5, 1.5, 1.0);
									glBindTexture(GL_TEXTURE_2D, ga5res->skin[108+nIndPresetGame5[k][nInd]]->GetCurrentTex());
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
									glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
									glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
									glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
									glEnd();
									glLoadIdentity();
									glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga5res->skin[106]->GetCurrentTex());
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

	#define TIME_START_DIFI_GAME5 600
	#define TIME_TRANSI_DIFI_GAME5 500
	#define TIME_GLOUTH_DIFI_GAME5 500
	#define TIME_CONTROT_DIFI_GAME5 1000
					/*Dibujo del nivel*/
					int nFilterLevel = 0;
					int nItemsActive = 0;
					int nIndexActive = 0;
					if(nLevelCurGame5[k] <= 0 || nLevelCurGame5[k] > ga5res->skin[101]->m_nSize) nFilterLevel = ga5res->skin[101]->m_nSize;
					else nFilterLevel = nLevelCurGame5[k];
					if(nFilterLevel >= 1 && nFilterLevel <= 30)
					{
						nItemsActive = (nFilterLevel-1)%8+1;
						nIndexActive = (nFilterLevel-1)/8;
					}
					if(player_sesion[k]->nLevel <= (nLevelCurGame5[k]-3) && !player_sesion[k]->nLockedAsp[ITEM_SUPRIMELVL]) nIndexActive = 4;
					double dAngleDif = double(nTransSelectM%TIME_CONTROT_DIFI_GAME5)/TIME_CONTROT_DIFI_GAME5*360;
					if(nTransSelectM >= TIME_START_DIFI_GAME5 && nTransSelectM < (TIME_START_DIFI_GAME5+TIME_TRANSI_DIFI_GAME5))
					{
						dApt = double(nTransSelectM - TIME_START_DIFI_GAME5)/TIME_TRANSI_DIFI_GAME5;
						glPushMatrix();
						if(np[k] && !np[int(!k)])
						{
							double dWidth = 18.75;
							double dSizeItem = dWidth/8;
							double dStart = -dSizeItem*4+dSizeItem/2;
							glLoadIdentity();
							glTranslated(0.0, 18.7, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[102]->GetCurrentTex());
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
									glTranslated(dx, 18.7, 0.0);
									glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
									glBindTexture(GL_TEXTURE_2D, ga5res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glTranslated(0.0, 18.7, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[107]->GetCurrentTex());
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
								glTranslated(0.0, 18.7, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[108]->GetCurrentTex());
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
							glTranslated(k?4.75:-4.75, 18.7, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[102]->GetCurrentTex());
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
									glTranslated(dx, 18.7, 0.0);
									glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
									glBindTexture(GL_TEXTURE_2D, ga5res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glTranslated(k?4.75:-4.75, 18.7, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[107]->GetCurrentTex());
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
								glTranslated(k?4.75:-4.75, 18.7, 0.0);
								glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[108]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
						}
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, 18.7, 0.0);
						glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBindTexture(GL_TEXTURE_2D, ga5res->skin[101]->texture[nFilterLevel-1]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  0.875, -0.875, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -0.875, -0.875, -50.0);
						glEnd();
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, 18.7, 0.0);
						glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
						glRotated(dAngleDif, 0.0, 0.0, 1.0);
						glBindTexture(GL_TEXTURE_2D, ga5res->skin[106]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, 18.7, 0.0);
						glScaled((1-dApt)*4.0, (1-dApt)*4.0, 1.0);
						glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
						glBindTexture(GL_TEXTURE_2D, ga5res->skin[104]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nTransSelectM >= (TIME_START_DIFI_GAME5+TIME_TRANSI_DIFI_GAME5))
					{
						glPushMatrix();
						if(np[k] && !np[int(!k)])
						{
							double dWidth = 18.75;
							double dSizeItem = dWidth/8;
							double dStart = -dSizeItem*4+dSizeItem/2;
							glLoadIdentity();
							glTranslated(0.0, 18.7, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[102]->GetCurrentTex());
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
									glTranslated(dx, 18.7, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga5res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glTranslated(0.0, 18.7, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[107]->GetCurrentTex());
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
								glTranslated(0.0, 18.7, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[108]->GetCurrentTex());
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
							glTranslated(k?4.75:-4.75, 18.7, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[102]->GetCurrentTex());
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
									glTranslated(dx, 18.7, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga5res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glTranslated(k?4.75:-4.75, 18.7, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[107]->GetCurrentTex());
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
								glTranslated(k?4.75:-4.75, 18.7, 0.0);
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[108]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d( -dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 1.0); glVertex3d(  dWidth/2,  dSizeItem/2, -50.0);
								glTexCoord2d(1.0, 0.0); glVertex3d(  dWidth/2, -dSizeItem/2, -50.0);
								glTexCoord2d(0.0, 0.0); glVertex3d( -dWidth/2, -dSizeItem/2, -50.0);
								glEnd();
							}
						}
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, 18.7, 0.0);
						glBindTexture(GL_TEXTURE_2D, ga5res->skin[101]->texture[nFilterLevel-1]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  0.875, -0.875, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -0.875, -0.875, -50.0);
						glEnd();
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, 18.7, 0.0);
						glRotated(dAngleDif, 0.0, 0.0, 1.0);
						glBindTexture(GL_TEXTURE_2D, ga5res->skin[106]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						if(nTransSelectM < (TIME_START_DIFI_GAME5+TIME_TRANSI_DIFI_GAME5+TIME_GLOUTH_DIFI_GAME5))
						{
							dApt = double(nTransSelectM - TIME_START_DIFI_GAME5 -TIME_TRANSI_DIFI_GAME5)/TIME_GLOUTH_DIFI_GAME5;
							glLoadIdentity();
							glTranslated(k?10.875:-10.875, 18.7, 0.0);
							glScaled(dApt*4.0, dApt*4.0, 1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[103]->GetCurrentTex());
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
					if(bSkinGame5[k]) DrawSkinsGame5(k);
					if(bCharacterGame5[k]) DrawCharactersGame5(k);
					glPopMatrix();
				}

				__int64 nTrans1 = nCountGame5 - nTimeChaGame5;
				if(nTrans1 <= TIME_GLOUTH_UPDO_GAME5)
				{
					dApt = (double(nTrans1))/(TIME_GLOUTH_UPDO_GAME5);
					glBindTexture(GL_TEXTURE_2D, ga5res->skin[32+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
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
				strDispTime.Format(_T("%.2d"), piuconfig.nModeMach==1?99:int((nMaxGame5 - nCountGame5)/1000));
				font_Car->glDrawText(30.0, -40.0, -30.0, 40.0, 0, 15.5, -50.0, 1.0, 2.0, strDispTime, DT_VCENTER|DT_CENTER);
			}
			if(nCountGame5 >= TIME_START_START_GAME5)	// Reconocimiento de comandos
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
								if(bArrows[y] && !bEnArrowsGame5[y])
								{
									bEnArrowsGame5[y] = true;
									nTimeEnArrowsGame5[y] = nCountGame5;
									for(int q = (MAX_COMMANDS_GAME5-2); q >= 0; q--)
									{
										nComandGame5[x][q+1] = nComandGame5[x][q];
									}
									if(!bSkinGame5[x] && !bCharacterGame5[x])
									{
										nComandGame5[x][0] = y;
										bNewCommand = true;
									}
								}
								if(!bArrows[y] && bEnArrowsGame5[y])
								{
									bEnArrowsGame5[y] = false;
									bCh[y] = false;
									if(y == 0 || y == 4)
									{
										nTimeTransiSongBorderGame5 = TIME_TRANSI_SONGBORDER_GAME5;
									}
								}
							}
						}
						if(x == 1)
						{
							for(int y = 5; y < 10; y++)
							{
								if(bArrows[y] && !bEnArrowsGame5[y])
								{
									bEnArrowsGame5[y] = true;
									nTimeEnArrowsGame5[y] = nCountGame5;
									for(int q = (MAX_COMMANDS_GAME5-2); q >= 0; q--)
									{
										nComandGame5[x][q+1] = nComandGame5[x][q];
									}
									if(!bSkinGame5[x] && !bCharacterGame5[x])
									{
										nComandGame5[x][0] = y-5;
										bNewCommand = true;
									}
								}
								if(!bArrows[y] && bEnArrowsGame5[y])
								{
									bEnArrowsGame5[y] = false;
									bCh[y] = false;
									if(y == 5 || y == 9)
									{
										nTimeTransiSongBorderGame5 = TIME_TRANSI_SONGBORDER_GAME5;
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
							nChangeRetroGame5 = 2;
							nTimeChangeRetroGame5 = nCountGame5;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
							for(int y = 0; y < 10; y++)
							{
								bArrows[y] = false;
								bEnArrowsGame5[y] = false;
								bCh[y] = false;
							}
							break;
						}
						if(x == 1 && bArrows[7] && nCredit)
						{
							PlusStagePlayer(1);
							nCredit--;
							nChangeRetroGame5 = 2;
							nTimeChangeRetroGame5 = nCountGame5;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
							for(int y = 0; y < 10; y++)
							{
								bArrows[y] = false;
								bEnArrowsGame5[y] = false;
								bCh[y] = false;
							}
							break;
						}
					}
				}
				if(bEnArrowsGame5[0])
				{
					if(bSkinGame5[0] && !bCh[0])
					{
						TurnSkinsGame5(true, 0);
						bCh[0] = true;
					}
					else if(bCharacterGame5[0] && !bCh[0])
					{
						TurnCharactersGame5(true, 0);
						bCh[0] = true;
					}
					else
					{
						__int64 nCaret = nCountGame5 - nTimeEnArrowsGame5[0];
						int nLimit = min(nCaret,nCountGame5-nTimeChaGame5)>TIME_FAST_CHANGE_GAME5?TIME_CHANGE_HIGH_GAME5:TIME_CHANGE_LOW_GAME5;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[0])
						{
							bCh[0] = true;
							MoveSongsGame5(true);
							if(bShow)
							{
								glDeleteTextures(1, &nTexCurGame5);
								nTexCurGame5 = 0;
							}
							PlaySoundTransiAboutLimitGame5(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[0] = false;
						}
					}
				}
				if(bEnArrowsGame5[4])
				{
					if(bSkinGame5[0] && !bCh[4])
					{
						TurnSkinsGame5(false, 0);
						bCh[4] = true;
					}
					else if(bCharacterGame5[0] && !bCh[4])
					{
						TurnCharactersGame5(false, 0);
						bCh[4] = true;
					}
					else
					{
						__int64 nCaret = nCountGame5 - nTimeEnArrowsGame5[4];
						int nLimit = min(nCaret,nCountGame5-nTimeChaGame5)>TIME_FAST_CHANGE_GAME5?TIME_CHANGE_HIGH_GAME5:TIME_CHANGE_LOW_GAME5;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[4])
						{
							bCh[4] = true;
							MoveSongsGame5(false);
							if(bShow)
							{
								glDeleteTextures(1, &nTexCurGame5);
								nTexCurGame5 = 0;
							}
							PlaySoundTransiAboutLimitGame5(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[4] = false;
						}
					}
				}
				if(bEnArrowsGame5[1] && !bCh[1])
				{
					if(!bSkinGame5[0] && !bCharacterGame5[0])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame5 = 0;
						bool bLoopLook = true;
						int nIndexLook = nCurSongGame5;
						while(bLoopLook)
						{
							nIndexLook--;
							if(nIndexLook < 0) nIndexLook = nSizeSongsGame5-1;
							if(nChaSongsGame5[nIndexLook] != nChaSongsGame5[nCurSongGame5] || nIndexLook == nCurSongGame5)
								bLoopLook = false;
						}
						nCurSongGame5 = nIndexLook;
						nTimeChaGame5 = nCountGame5;
						ActualizateIndexShowGame5();
						for(int x = 0; x < 9; x++)
						{
							nTexCurSongGame5[x] = nTexTrainingGame5[nIndexShowGame5[x]];
						}
						bCh[1] = true;
						GetLevelSongsGame5(nCurSongGame5, &nLevelCurGame5[0]);
						GetBestScoresGame5(nCurSongGame5);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
				}
				if(bEnArrowsGame5[3] && !bCh[3])
				{
					if(!bSkinGame5[0] && !bCharacterGame5[0])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame5 = 0;
						bool bLoopLook = true;
						int nIndexLook = nCurSongGame5;
						while(bLoopLook)
						{
							nIndexLook++;
							if(nIndexLook >= nSizeSongsGame5) nIndexLook = 0;
							if(nChaSongsGame5[nIndexLook] != nChaSongsGame5[nCurSongGame5] || nIndexLook == nCurSongGame5)
								bLoopLook = false;
						}
						nCurSongGame5 = nIndexLook;
						nTimeChaGame5 = nCountGame5;
						ActualizateIndexShowGame5();
						for(int x = 0; x < 9; x++)
						{
							nTexCurSongGame5[x] = nTexTrainingGame5[nIndexShowGame5[x]];
						}
						bCh[3] = true;
						GetLevelSongsGame5(nCurSongGame5, &nLevelCurGame5[0]);
						GetBestScoresGame5(nCurSongGame5);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
				}
				if(bEnArrowsGame5[2] && !bCh[2])
				{
					bCh[2] = true;
					if(np[0])
					{
						if(bSkinGame5[0])
						{
							MakeChoiceSkinGame5(0);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else if(bCharacterGame5[0])
						{
							MakeChoiceCharacterGame5(0);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else
						{
							nChosesGame5++;
							nTimeChosesGame5[nChosesGame5-1] = nCountGame5;
							if(nChosesGame5 > 0 && nChosesGame5 < CHOOSES_GAME5) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
							if(nChosesGame5 >= CHOOSES_GAME5)
							{
								FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
								for(int x = 0; x < 10; x++) bCh[x] = false;
							}
						}
					}
				}
/*#define TIME_TOL_SWITCHUNLOCKER_GAME5 50
				if(((_abs64(nTimeEnArrowsGame5[1] - nTimeEnArrowsGame5[3]) < TIME_TOL_SWITCHUNLOCKER_GAME5 && (bEnArrowsGame5[1] || bEnArrowsGame5[3])) ||
					(_abs64(nTimeEnArrowsGame5[6] - nTimeEnArrowsGame5[8]) < TIME_TOL_SWITCHUNLOCKER_GAME5 && (bEnArrowsGame5[6] || bEnArrowsGame5[8]))) && bIsAvalibleUnlockerGame5)
				{
					nChangeRetroGame5 = 3;
					nTimeChangeRetroGame5 = nCountGame5;
					bIsAvalibleUnlockerGame5 = false;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int y = 0; y < 10; y++)
					{
						bArrows[y] = false;
						bEnArrowsGame5[y] = false;
						bCh[y] = false;
					}
				}*/

				// 2
				if(bEnArrowsGame5[5])
				{
					if(bSkinGame5[1] && !bCh[5])
					{
						TurnSkinsGame5(true, 1);
						bCh[5] = true;
					}
					else if(bCharacterGame5[1] && !bCh[5])
					{
						TurnCharactersGame5(true, 1);
						bCh[5] = true;
					}
					else
					{
						__int64 nCaret = nCountGame5 - nTimeEnArrowsGame5[5];
						int nLimit = min(nCaret,nCountGame5-nTimeChaGame5)>TIME_FAST_CHANGE_GAME5?TIME_CHANGE_HIGH_GAME5:TIME_CHANGE_LOW_GAME5;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[5])
						{
							bCh[5] = true;
							MoveSongsGame5(true);
							if(bShow)
							{
								glDeleteTextures(1, &nTexCurGame5);
								nTexCurGame5 = 0;
							}
							PlaySoundTransiAboutLimitGame5(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[5] = false;
						}
					}
				}
				if(bEnArrowsGame5[9])
				{
					if(bSkinGame5[1] && !bCh[9])
					{
						TurnSkinsGame5(false, 1);
						bCh[9] = true;
					}
					else if(bCharacterGame5[1] && !bCh[9])
					{
						TurnCharactersGame5(false, 1);
						bCh[9] = true;
					}
					else
					{
						__int64 nCaret = nCountGame5 - nTimeEnArrowsGame5[9];
						int nLimit = min(nCaret,nCountGame5-nTimeChaGame5)>TIME_FAST_CHANGE_GAME5?TIME_CHANGE_HIGH_GAME5:TIME_CHANGE_LOW_GAME5;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[9])
						{
							bCh[9] = true;
							MoveSongsGame5(false);
							if(bShow)
							{
								glDeleteTextures(1, &nTexCurGame5);
								nTexCurGame5 = 0;
							}
							PlaySoundTransiAboutLimitGame5(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[9] = false;
						}
					}
				}
				if(bEnArrowsGame5[6] && !bCh[6])
				{
					if(!bSkinGame5[1] && !bCharacterGame5[1])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame5 = 0;
						bool bLoopLook = true;
						int nIndexLook = nCurSongGame5;
						while(bLoopLook)
						{
							nIndexLook--;
							if(nIndexLook < 0) nIndexLook = nSizeSongsGame5-1;
							if(nChaSongsGame5[nIndexLook] != nChaSongsGame5[nCurSongGame5] || nIndexLook == nCurSongGame5)
								bLoopLook = false;
						}
						nCurSongGame5 = nIndexLook;
						nTimeChaGame5 = nCountGame5;
						ActualizateIndexShowGame5();
						for(int x = 0; x < 9; x++)
						{
							nTexCurSongGame5[x] = nTexTrainingGame5[nIndexShowGame5[x]];
						}
						bCh[6] = true;
						GetLevelSongsGame5(nCurSongGame5, &nLevelCurGame5[1]);
						GetBestScoresGame5(nCurSongGame5);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
				}
				if(bEnArrowsGame5[8] && !bCh[8])
				{
					if(!bSkinGame5[1] && !bCharacterGame5[1])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame5 = 0;
						bool bLoopLook = true;
						int nIndexLook = nCurSongGame5;
						while(bLoopLook)
						{
							nIndexLook++;
							if(nIndexLook >= nSizeSongsGame5) nIndexLook = 0;
							if(nChaSongsGame5[nIndexLook] != nChaSongsGame5[nCurSongGame5] || nIndexLook == nCurSongGame5)
								bLoopLook = false;
						}
						nCurSongGame5 = nIndexLook;
						nTimeChaGame5 = nCountGame5;
						ActualizateIndexShowGame5();
						for(int x = 0; x < 9; x++)
						{
							nTexCurSongGame5[x] = nTexTrainingGame5[nIndexShowGame5[x]];
						}
						bCh[8] = true;
						GetLevelSongsGame5(nCurSongGame5, &nLevelCurGame5[1]);
						GetBestScoresGame5(nCurSongGame5);
						bShow = false;
						FMOD_Channel_Stop(channels[9]);
					}
				}
				if(bEnArrowsGame5[7] && !bCh[7])
				{
					bCh[7] = true;
					if(np[1])
					{
						if(bSkinGame5[1])
						{
							MakeChoiceSkinGame5(1);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else if(bCharacterGame5[1])
						{
							MakeChoiceCharacterGame5(1);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else
						{
							nChosesGame5++;
							nTimeChosesGame5[nChosesGame5-1] = nCountGame5;
							if(nChosesGame5 > 0 && nChosesGame5 < CHOOSES_GAME5) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
							if(nChosesGame5 >= CHOOSES_GAME5)
							{
								FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
								for(int x = 0; x < 10; x++) bCh[x] = false;
							}
						}
					}
				}
				if(nCountGame5 >= nMaxGame5 && piuconfig.nModeMach!=1)
				{
					nChosesGame5 = 2;
					nTimeChosesGame5[nChosesGame5-1] = nCountGame5;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int x = 0; x < 10; x++) bCh[x] = false;
				}
				if(bNewCommand) CheckPresetGame5();
			}
		}
	#define TIME_EXIT_GAME5 1000
		if(nChosesGame5 >= CHOOSES_GAME5)
		{
			__int64 nTransEnd = nCountGame5 - nTimeChosesGame5[CHOOSES_GAME5-1];
			if(nTransEnd < TIME_EXIT_GAME5)
			{
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME5))
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
					if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME5))
					{
						if(x == 4)
						{
							if(nTransEnd < (TIME_TRANSI_SELECTOR_GAME5))
							{
								dApt = //pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME5, 2);
								pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME5 - 0.25, 2)/0.125 + 0.5;
								glPushMatrix();
								glTranslated(0.0, 0.0, -60.0);
								glScaled(dApt*1.0, dApt*1.0, 1.0);
								glColor4d(1.0, 1.0, 1.0, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME5);
								glPushMatrix();
								glTranslated(-13.0, -4.0, 0.0);
								if(nTransBorder > TIME_SHOW_PREVIEW_GAME5 && movie_songpreviewgame5->bLoad && movie_songpreviewgame5->bResize && bShow)
								{
									movie_songpreviewgame5->Update(dwWait);
									movie_songpreviewgame5->GrabAVIFrame();
									__int64 nCaretCover = movie_songpreviewgame5->dwNext;
									glBindTexture(GL_TEXTURE_2D, movie_songpreviewgame5->tex);

									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
									glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
									glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
									glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
									glEnd();
									if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME5)
									{
										dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME5;
										glColor4d(1.0, 1.0, 1.0, min(1.0-dApt, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME5));
										glBindTexture(GL_TEXTURE_2D, nTexCurGame5);
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
									glBindTexture(GL_TEXTURE_2D, nTexCurGame5);
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
									glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
									glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
									glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
									glEnd();
								}
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
								glEnd();
								glPopMatrix();
								glPushMatrix();
								glTranslated(13.0, 4.0, 0.0);
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
								glEnd();
								glPopMatrix();
								glPopMatrix();
								glColor4d(1.0, 1.0, 1.0, 1.0);
							}
						}
						else
						{
							__int64 nStartTransi = TIME_TRANSI_SELECTOR_GAME5/4*(-abs(x-4)+4-1);
							__int64 nLimitTransi = TIME_TRANSI_SELECTOR_GAME5/4*(-abs(x-4)+4);
							if(nTransEnd >= nStartTransi && nTransEnd < nLimitTransi)
							{
								dApt = double(nTransEnd-nStartTransi)/double(nLimitTransi-nStartTransi);
								double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(dApt)+dTargetx;
								double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
								double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
								double dtheta = ThetaFuncForXGame5(dx);
								double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.0;
								double dalpha = AlphaFuncForXGame5(dx);
								glPushMatrix();
								glTranslated(dx, dy, dz-60.0);
								glRotated(dtheta, 0.0, 1.0, 0.0);
								glScaled(dscale, dscale, 1.0);
								glColor4d(1.0, 1.0, 1.0, min(dalpha, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME5));
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
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
								double dtheta = ThetaFuncForXGame5(dx);
								double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.0;
								double dalpha = AlphaFuncForXGame5(dx);
								glPushMatrix();
								glTranslated(dx, dy, dz-60.0);
								glRotated(dtheta, 0.0, 1.0, 0.0);
								glScaled(dscale, dscale, 1.0);
								glColor4d(1.0, 1.0, 1.0, dalpha);
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
								glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
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
					if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_LR_GAME5))
					{
						dApt = pow(double(nTransEnd)/TIME_TRANSI_LR_GAME5, 0.5);
						glPushMatrix();
						glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(dApt), 0.0, 0.0);
						glColor4d(1.0, 1.0, 1.0, 1-dApt);
						glBindTexture(GL_TEXTURE_2D, ga5res->skin[(k?68:50)+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
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
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_UPDO_GAME5))
				{
					dApt = (double(nTransEnd))/(TIME_TRANSI_UPDO_GAME5);
					glPushMatrix();
					glScaled(1.0+0.8*(dApt), 1.0+0.8*(dApt), 1.0);
					glBindTexture(GL_TEXTURE_2D, ga5res->skin[41+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga5res->skin[23+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga5res->skin[3]->texture[0]);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga5res->skin[4]->texture[0]);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga5res->skin[5]->texture[0]);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga5res->skin[6]->texture[0]);
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
					player_sesion[k]->nLastTraining = nRefSongsGame5[nCurSongGame5];
				}
				if(movie_songpreviewgame5->bLoad)
				{
					movie_songpreviewgame5->CloseAVI();
					delete movie_songpreviewgame5;
					movie_songpreviewgame5 = new CMovieStream2();
				}
				glDeleteTextures(1, &nTexCurGame5);
				for(int x = 0; x < 9; x++) nTexCurSongGame5[x] = 0;
				bShow = false;
				return 2;
			}
		}
	}
	else
	{
#define TIME_EXIT_CHANGERETRO_GAME5 1000
		__int64 nTransEnd = nCountGame5 - nTimeChangeRetroGame5;
		if(nTransEnd < TIME_EXIT_CHANGERETRO_GAME5)
		{
			if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME5))
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
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME5))
				{
					if(x == 4)
					{
						if(nTransEnd < (TIME_TRANSI_SELECTOR_GAME5))
						{
							dApt = //pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME5, 2);
							pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME5 - 0.25, 2)/0.125 + 0.5;
							glPushMatrix();
							glTranslated(0.0, 0.0, -60.0);
							glScaled(dApt*1.0, dApt*1.0, 1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME5);
							glPushMatrix();
							glTranslated(-13.0, -4.0, 0.0);
							if(nTransBorder > TIME_SHOW_PREVIEW_GAME5 && movie_songpreviewgame5->bLoad && movie_songpreviewgame5->bResize && bShow)
							{
								movie_songpreviewgame5->Update(dwWait);
								movie_songpreviewgame5->GrabAVIFrame();
								__int64 nCaretCover = movie_songpreviewgame5->dwNext;
								glBindTexture(GL_TEXTURE_2D, movie_songpreviewgame5->tex);

								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
								glEnd();
								if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME5)
								{
									dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME5;
									glColor4d(1.0, 1.0, 1.0, min(1.0-dApt, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME5));
									glBindTexture(GL_TEXTURE_2D, nTexCurGame5);
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
								glBindTexture(GL_TEXTURE_2D, nTexCurGame5);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
							}
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glPopMatrix();
							glPushMatrix();
							glTranslated(13.0, 4.0, 0.0);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glPopMatrix();
							glPopMatrix();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
					}
					else
					{
						__int64 nStartTransi = TIME_TRANSI_SELECTOR_GAME5/4*(-abs(x-4)+4-1);
						__int64 nLimitTransi = TIME_TRANSI_SELECTOR_GAME5/4*(-abs(x-4)+4);
						if(nTransEnd >= nStartTransi && nTransEnd < nLimitTransi)
						{
							dApt = double(nTransEnd-nStartTransi)/double(nLimitTransi-nStartTransi);
							double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(dApt)+dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXGame5(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.0;
							double dalpha = AlphaFuncForXGame5(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, min(dalpha, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME5));
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
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
							double dtheta = ThetaFuncForXGame5(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.0;
							double dalpha = AlphaFuncForXGame5(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame5[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga5res->skin[14+nChaSongsGame5[nIndexShowGame5[x]]]->GetCurrentTex());
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
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_LR_GAME5))
				{
					dApt = pow(double(nTransEnd)/TIME_TRANSI_LR_GAME5, 0.5);
					glPushMatrix();
					glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(dApt), 0.0, 0.0);
					glColor4d(1.0, 1.0, 1.0, 1-dApt);
					glBindTexture(GL_TEXTURE_2D, ga5res->skin[(k?68:50)+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
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
			if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_UPDO_GAME5))
			{
				dApt = (double(nTransEnd))/(TIME_TRANSI_UPDO_GAME5);
				glPushMatrix();
				glScaled(1.0+0.8*(dApt), 1.0+0.8*(dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[41+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[23+nChaSongsGame5[nCurSongGame5]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[3]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[4]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[5]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga5res->skin[6]->texture[0]);
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
			nCurSongGame5 = 0;
			if(movie_songpreviewgame5->bLoad)
			{
				movie_songpreviewgame5->CloseAVI();
				delete movie_songpreviewgame5;
				movie_songpreviewgame5 = new CMovieStream2();
			}
			glDeleteTextures(1, &nTexCurGame5);
			for(int x = 0; x < 9; x++) nTexCurSongGame5[x] = 0;
			bShow = false;
			if(nChangeRetroGame5 == 2)
			{
				//SemiActualizateContextGame5();
				return 4;
			}
			if(nChangeRetroGame5 == 3)
			{
				nCaretCountGame5 = nCountGame5;
				return 5;
			}
			for(int k = 0; k < 2; k++) if(np[k])
			{
				player_sesion[k]->nLastTraining = nRefSongsGame5[nCurSongGame5];
			}
			nCaretCountGame5 = nCountGame5;
			SemiActualizateContextGame5();
			nMaxGame5 = nMaxGame5 - nCaretCountGame5;
		}
	}

	return 0;
}

void SemiActualizateContextGame5()
{
	int nPlayerFocus = 0;
	nChangeRetroGame5 = 0;
	switch(piuconfig.nTimePlay)
	{
	default:
		nMaxGame5 = 120000+TIME_TRANSI_UPDO_GAME5+TIME_START_UPDO_GAME5; break;
	case 1:
		nMaxGame5 = 90000+TIME_TRANSI_UPDO_GAME5+TIME_START_UPDO_GAME5; break;
	case 2:
		nMaxGame5 = 60000+TIME_TRANSI_UPDO_GAME5+TIME_START_UPDO_GAME5; break;
	}
	nTimeChangeRetroGame5 = 0;
	nModeGame5 = 0;
	nCurSongGame5 = 0;
	nChosesGame5 = 0;
	nCountGame5 = 0;
	nRetroActiveGame5 = 0;
	nTimeCurSongGame5 = TIME_START_SELECTOR_GAME5+TIME_TRANSI_SELECTOR_GAME5+TIME_GLOUTH_SELECTOR_GAME5;
	bLoadMusicCoverGame5 = false;
	strBestScoreMachineGame5 = _T("");
	nBestScoreMachineGame5 = 0;
	//nBestScoreMachineGame5Ex = 0;
	for(int x = 0; x < 2; x++)
	{
		nTimeSkinGame5[x] = 0;
		bSkinGame5[x] = false;
		nTimeTurnSkinGame5[x] = 0;
		nDirTurnSkinGame5[x] = 0;
		nTimeCharacterGame5[x] = 0;
		bCharacterGame5[x] = false;
		nTimeTurnCharacterGame5[x] = 0;
		nDirTurnCharacterGame5[x] = 0;
		nBestScorePlayerGame5[x] = 0;
		//nBestScorePlayerGame5Ex[x] = 0;
		nLevelCurGame5[x] = 1;
		nIndLastPresetGame5[x] = -1;	// Show all
		nTimeLastPresetGame5[x] = (TIME_START_UPDO_GAME5+TIME_TRANSI_UPDO_GAME5)+(TIME_START_LR_GAME5+TIME_TRANSI_LR_GAME5);
		if(np[x]) ShowSesionTrayExp(x);
		for(int y = 0; y < 10; y++) nCommandGame5[x][y] = -1;
		for(int y = 0; y < MAX_COMMANDS_GAME5; y++) nComandGame5[x][y] = -1;
	}

	for(int x = 0; x < 10; x++) {
        bArrows[x] = false;
    }
	for(int x = 0; x < 10; x++) bEnArrowsGame5[x] = false;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		EnumSongsGame5();
		nPlayerFocus = x;
		break;
	}
	bool bFoundLast = false;
	for(int x = 0; x < nSizeSongsGame5; x++)
	{
		if(nRefSongsGame5[x] == player_sesion[nPlayerFocus]->nLastTraining)
		{
			bFoundLast = true;
			nCurSongGame5 = x;
			/*for(int y = 0; y < 2; y++)
			{
				nModeCurSongGame5[y] = GetAvalibleModeGame5(x, player_sesion[y]->nLastSpecial[1]);
				nBackModeCurSongGame5[y] = nModeCurSongGame5[y];
				nPrefModeCurSongGame5[y] = nModeCurSongGame5[y];
			}*/
			break;
		}
	}
	if(!bFoundLast) nCurSongGame5 = 0;
	nTimeChaGame5 = TIME_START_UPDO_GAME5+TIME_TRANSI_UPDO_GAME5;

	ActualizateIndexShowGame5();
	for(int x = 0; x < 9; x++)
	{
		nTexCurSongGame5[x] = nTexTrainingGame5[nIndexShowGame5[x]];
	}
	GetBestScoresGame5(nCurSongGame5);
	EnumSkinsGame5();
	EnumCharactersGame5();
	dwLastTick = dwTick = SDL_GetTicks();
}

int StageCheckGame5()
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
		double dApt = double(5-nRankStepEval[x])/5*(bUsb[0]?2.0:1.0)*(player_sesion[x]->nLockedAsp[ITEM_MOREEXP]?2.0:1.0);
		PlusExpPlayer(x, nLevelCurGame5[x], dApt);
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
			if(nRankStepEval[x] >= 2 && bBonusStageGame5[x])
			{
				np[x]-=2;
				bBonusStageGame5[x] = false;
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

int CreateContextGame5()
{
	if(!EnumTotalSongsGame5()) return 0;
	training = new CMissionLoader();
	if(!training->LoadBinary(_T("Game5/Training.dat")))
	{
		if(!training->LoadText(_T("Game5/Training.acc"))) return 0;
		if(!training->SaveBinary(_T("Game5/Training.dat"))) return 0;
	}
	if(training->m_nSize != nSizeTotalGame5) return 0;

	nRefSkinGame5 = (int*)calloc(0, sizeof(int));
	uTexSkinGame5 = (GLuint*)calloc(0, sizeof(GLuint));
	nRefCharacterGame5 = (int*)calloc(0, sizeof(int));
	uTexCharacterGame5 = (GLuint*)calloc(0, sizeof(GLuint));
	return 1;
}

void DestroyContextGame5()
{
	delete training;
	delete nRefSkinGame5;
	delete uTexSkinGame5;
	delete nRefCharacterGame5;
	delete uTexCharacterGame5;
}

void ClearContextGame5()
{
	glDeleteTextures(nSizeTotalGame5, nTexTrainingGame5);
	delete nTexTrainingGame5;
	delete movie_songpreviewgame5;
	if(movie_trainingbg->bLoad) movie_trainingbg->CloseAVI();
	delete movie_trainingbg;
	delete ga5res;
	DeleteSkinsGame5();
	DeleteCharactersGame5();
}

void ActualizateContextGame5()
{
	nCurStage = 1;
	movie_trainingbg = new CMovieStream2();
	movie_songpreviewgame5 = new CMovieStream2();
	movie_trainingbg->OpenAVI(_T("Game5/ArcadeBG.avi"));
	movie_trainingbg->bLoop = true;

	ga5res = new CGame5Res();
	ga5res->LoadSkin();

	nTexTrainingGame5 = new GLuint[nSizeTotalGame5];
	for (int x = 0; x < nSizeTotalGame5; x++)
	{
		CString str;
		str.Format(_T("Game5/Lessons/tra%d.PNG"), x);
		nTexTrainingGame5[x] = GenTexImage(str);
	}

	switch(piuconfig.nTimePlay)
	{
	default:
		nMaxGame5 = 120000+TIME_TRANSI_UPDO_GAME5+TIME_START_UPDO_GAME5; break;
	case 1:
		nMaxGame5 = 90000+TIME_TRANSI_UPDO_GAME5+TIME_START_UPDO_GAME5; break;
	case 2:
		nMaxGame5 = 60000+TIME_TRANSI_UPDO_GAME5+TIME_START_UPDO_GAME5; break;
	}
	nInterGame5 = 0;
	nRetroActiveGame5 = 0;
	int nPlayerFocus = 0;
	nChangeRetroGame5 = 0;
	nTimeChangeRetroGame5 = 0;
	nModeGame5 = 0;
	nCurSongGame5 = 0;
	nChosesGame5 = 0;
	nCountGame5 = 0;
	nTimeCurSongGame5 = TIME_START_SELECTOR_GAME5+TIME_TRANSI_SELECTOR_GAME5+TIME_GLOUTH_SELECTOR_GAME5;
	bLoadMusicCoverGame5 = false;
	bContinueUsbGame5 = true;
	strBestScoreMachineGame5 = _T("");
	nBestScoreMachineGame5 = 0;
	//nBestScoreMachineGame5Ex = 0;
	bIsAvalibleUnlockerGame5 = true;
	for(int x = 0; x < 2; x++)
	{
		nTimeSkinGame5[x] = 0;
		bSkinGame5[x] = false;
		nTimeTurnSkinGame5[x] = 0;
		nDirTurnSkinGame5[x] = 0;
		nCurSkinGame5[x] = 0;
		nTimeCharacterGame5[x] = 0;
		bCharacterGame5[x] = false;
		nTimeTurnCharacterGame5[x] = 0;
		nDirTurnCharacterGame5[x] = 0;
		nCurCharacterGame5[x] = 0;
		nBestScorePlayerGame5[x] = 0;
		//nBestScorePlayerGame5Ex[x] = 0;
		bBonusStageGame5[x] = true;
		nLevelCurGame5[x] = 1;
		nIndLastPresetGame5[x] = -1;	// Show all
		nTimeLastPresetGame5[x] = (TIME_START_UPDO_GAME5+TIME_TRANSI_UPDO_GAME5)+(TIME_START_LR_GAME5+TIME_TRANSI_LR_GAME5);
		if(np[x]) ShowSesionTrayExp(x);
		for(int y = 0; y < 6; y++)
		{
			nIndSizePresetGame5[x][y] = 0;
			nIndPresetGame5[x][y*5] = 0;
			nIndPresetGame5[x][y*5+1] = 0;
			nIndPresetGame5[x][y*5+2] = 0;
			nIndPresetGame5[x][y*5+3] = 0;
			nIndPresetGame5[x][y*5+4] = 0;
		}
		nIndSizePresetGame5[x][0] = 1;
		nIndPresetGame5[x][0] = 1;
		for(int y = 0; y < 10; y++) nCommandGame5[x][y] = -1;
		for(int y = 0; y < MAX_COMMANDS_GAME5; y++) nComandGame5[x][y] = -1;
	}
	for(int x = 0; x < 10; x++) {
        bArrows[x] = false;
    }
	for(int x = 0; x < 10; x++) bEnArrowsGame5[x] = false;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		EnumSongsGame5();
		nPlayerFocus = x;
		break;
	}
	bool bFoundLast = false;
	for(int x = 0; x < nSizeSongsGame5; x++)
	{
		if(nRefSongsGame5[x] == player_sesion[nPlayerFocus]->nLastTraining)
		{
			bFoundLast = true;
			nCurSongGame5 = x;
			break;
		}
	}
	if(!bFoundLast) nCurSongGame5 = 0;
	nTimeChaGame5 = TIME_START_UPDO_GAME5+TIME_TRANSI_UPDO_GAME5;

	ActualizateIndexShowGame5();
	for(int x = 0; x < 9; x++)
	{
		nTexCurSongGame5[x] = nTexTrainingGame5[nIndexShowGame5[x]];
	}
	GetBestScoresGame5(nCurSongGame5);
	EnumSkinsGame5();
	EnumCharactersGame5();
	dwLastTick = dwTick = SDL_GetTicks();
}

bool bContAfterSplashGame5 = false;

int FrameGame5()
{
	ga5res->Update(dwWait);
	switch(nInterGame5)
	{
	case 0:
		switch(DrawInterfaceSelectGame5())
		{
		case 1:
			ClearContextGame5();
			return INT_WELCOME;
			break;
		case 2:
			if(training->m_strHis[nRefSongsGame5[nCurSongGame5]].IsEmpty())
			{
				LoadSongGame5();
				Go_Step();
				nInterGame5 = 1;
			}
			else
			{
				strHis = training->m_strHis[nRefSongsGame5[nCurSongGame5]];
				if(ActualizateContextHistory())
					nInterGame5 = 8;
				else
				{
					LoadSongGame5();
					Go_Step();
					nInterGame5 = 1;
				}
			}
			break;
		case 3:
			return INT_ITEMSHOP;
			break;
		case 4:
			//ClearContextGame5();
			ActualizateContextUsb();
			bSaveUsb = false;
			bContinueUsbGame5 = true;
			nInterGame5 = 4;
			for(int x = 0; x < 2; x++) if(np[x])
			{
				bShowSesionTray[x] = false;
				bShowSesionTrayExp[x] = false;
				if(uTexPlayerSplash[x] != uTexSplash[0]) glDeleteTextures(1, &uTexPlayerSplash[x]);
			}
			break;
		case 5:
			nModeUnlock = 1;
			ActualizateContextUnlock();
			nInterGame5 = 6;
			break;
		}
		break;
	case 1:
		if(!Draw_All())
		{
			nInterGame5 = 2;
			nModeStepEval = 0;	// Normal mode
			ActualizateContextStepEval();
		}
		break;
	case 2:
		if(!FrameStepEval())
		{
			nInterGame5 = 9;
			{
				int nCode = StageCheckGame5();
				if(nCode == 0)
				{
                    ActualizateContextStageStepEval();
                    nInterGame5 = 9;
				}
				else
				{
					nInterGame5 = 4;
					if(nCode == 1) bContinueUsbGame5 = false;
					if(nCode == 2) bContinueUsbGame5 = true;
					bSaveUsb = true;
					ActualizateContextUsb();
				}
				if(nSizeUnlock)
				{
					nModeUnlock = 0;
					ActualizateContextUnlock();
					nInterGame5 = 7;
					if(nCode != 0) bContAfterSplashGame5 = false;
					else bContAfterSplashGame5 = true;
				}
				DeleteSongGame5();
			}
			SemiActualizateContextGame5();
		}
		break;
	case 3:
		{
			int nCode = StageCheckGame5();
			if(nCode == 0) nInterGame5 = 9;
			else
			{
				nInterGame5 = 4;
				if(nCode == 1) bContinueUsbGame5 = false;
				if(nCode == 2) bContinueUsbGame5 = true;
				bSaveUsb = true;
				ActualizateContextUsb();
			}
			if(nSizeUnlock)
			{
				nModeUnlock = 0;
				ActualizateContextUnlock();
				nInterGame5 = 7;
				if(nCode != 0) bContAfterSplashGame5 = false;
				else bContAfterSplashGame5 = true;
			}
			DeleteSongGame5();
		}
		SemiActualizateContextGame5();
		break;
	case 4:	// Dibujar Sistema USB
		if(!FrameUsb())
		{
			nInterGame5 = 0;
			if(!bContinueUsbGame5)
			{
				ClearContextGame5();
				return INT_WELCOME;
			}
			else SemiActualizateContextGame5();
		}
		break;
	case 5:
		if(!FrameStageBreakStepEval())
		{
			nInterGame5 = 4;
			np[0] = 0;
			np[1] = 0;
			bSaveUsb = true;
			ActualizateContextUsb();
			bContinueUsbGame5 = false;
		}
		break;
	case 6:
		if(!FrameUnlock())
		{
			nInterGame5 = 0;
			SemiActualizateContextGame5();
			nMaxGame5 = nMaxGame5 - nCaretCountGame5;
		}
		break;
	case 7:
		if(FrameUnlock() != INT_UNLOCK)
		{
			nInterGame5 = bContAfterSplashGame5?0:4;
		}
		break;
	case 8:
		if(!FrameHistory())
		{
			LoadSongGame5();
			Go_Step();
			nInterGame5 = 1;
		}
		break;
	case 9:
		if(!FrameStageStepEval())
		{
			nInterGame5 = 0;
			nCurStage++;
		}
		break;
	}
	return INT_GAME_5;
}

int EventGame5(int nEvent, int nKey)
{
	switch(nInterGame5)
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
	case 8:
		EventHistory(nEvent, nKey);
		break;
	}
	return 1;
}
