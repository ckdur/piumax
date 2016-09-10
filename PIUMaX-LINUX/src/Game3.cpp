#include "Game3.h"
#include "stepeval.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "Game3Res.h"
#include "USBsys.h"
#include "UnlockSys.h"

#define TIME_START_SELECTOR_GAME3 0
#define TIME_TRANSI_SELECTOR_GAME3 1000
#define TIME_GLOUTH_SELECTOR_GAME3 500

#define TIME_START_START_GAME3 1500

#define TIME_TRANSI_SONGBORDER_GAME3 500
#define TIME_GLOUTH_SONGBORDER_GAME3 500
#define TIME_TRANSI_START_MAIN_GAME3 500
#define CHOOSES_GAME3 2
#define MAX_COMMANDS_GAME3 12

#define TIME_SHOW_PREVIEW_GAME3 600

int nComandGame3[2][MAX_COMMANDS_GAME3];
int nInterGame3 = 0;
int nSizeSongsGame3 = 0;
int nModTotalSongsGame3[5000];
int nModSongsGame3[5000];
int nChaTotalSongsGame3[1000];
int nChaSongsGame3[1000];
int nNOLTotalSongsGame3[1000];
int nRefSongsGame3[1000];
GLuint nTexTotalSongsGame3[1000];
GLuint nTexSongsGame3[1000];
GLuint nTexCurSongGame3[9];
TCHAR chTotalSongsGame3[1000][100];
TCHAR chSongsGame3[1000][100];

int nIndexShowGame3[9];
int nLevelCurGame3[2];
double dDCurGame3[2][5];
int nCurSongGame3 = 0;
int nModeCurSongGame3[2];
int nBackModeCurSongGame3[2];
int nPrefModeCurSongGame3[2];
__int64 nTimeModeCurSongGame3[2];
__int64 nTimeCurSongGame3 = 0;
__int64 nCountGame3 = 0;
__int64 nMaxGame3 = 0;
__int64 nTimeChaGame3 = 0;

__int64 nTimeLastPresetGame3[2];
int nIndLastPresetGame3[2];
int nIndPresetGame3[2][30];
int nIndSizePresetGame3[2][6];
int nSkinGame3[2];
int nCharacterGame3[2];

int nChosesGame3;
__int64 nTimeChosesGame3[2];
int nChangeRetroGame3;
__int64 nTimeChangeRetroGame3;
int nRetroActiveGame3;

__int64 nCaretCountGame3;

bool bEnArrowsGame3[10];
__int64 nTimeEnArrowsGame3[10];
int nCommandGame3[2][10];

int nTimeTransiSongBorderGame3 = TIME_TRANSI_SONGBORDER_GAME3;
int nModeGame3 = 0;
bool bBonusStageGame3[2];
//bool bSaveUsbGame3 = false;
bool bContinueUsbGame3 = true;

GLuint movie_texGame3;
CMovieStream2* movie_specialbg;
CMovieStream2* movie_songpreviewgame3;

bool bLoadMusicCoverGame3 = false;
FMOD_SOUND* music_coverGame3;

CString strDispGame3;

CGame3Res* ga3res;

CString strBestScoreMachineGame3;
int nBestScoreMachineGame3;
int nBestScorePlayerGame3[2];

int FoundPresetGame3(int x, int nFamily, int nBegin, int nEnd);
void SemiActualizateContextGame3();

int nSizeSkinGame3 = 0;
int* nRefSkinGame3;
GLuint* uTexSkinGame3;
int nSizeCharacterGame3 = 0;
int* nRefCharacterGame3;
GLuint* uTexCharacterGame3;

#define MAX_SKINS_GAME3 20
#define MAX_CHARACTERS_GAME3 12

__int64 nTimeSkinGame3[2];
bool bSkinGame3[2];
__int64 nTimeTurnSkinGame3[2];
int nDirTurnSkinGame3[2];
int nCurSkinGame3[2];
__int64 nTimeCharacterGame3[2];
bool bCharacterGame3[2];
__int64 nTimeTurnCharacterGame3[2];
int nDirTurnCharacterGame3[2];
int nCurCharacterGame3[2];

bool bIsAvalibleUnlockerGame3;

int FoundPresetGame3(int x, int nFamily, int nBegin, int nEnd);

void MakeChoiceSkinGame3(int x)
{
	bSkinGame3[x] = false;
	int nPut = nRefSkinGame3[nCurSkinGame3[x]-1]+25;
	int nFamily = 2;
	int nFound = FoundPresetGame3(x, nFamily, 24, 45);
	if(nFound == -1 && nCurSkinGame3[x])
	{
		nFound = nIndSizePresetGame3[x][nFamily];
		nIndSizePresetGame3[x][nFamily]++;
		nIndPresetGame3[x][nFamily*5+nFound] = nPut;
		nIndLastPresetGame3[x] = nFamily*5+nFound;
		nTimeLastPresetGame3[x] = nCountGame3;
	}
	if(nFound != 1)
	{
		if(!nCurSkinGame3[x])	// Desactivar
		{
			nIndPresetGame3[x][nFamily*5+nFound] = 0;
			for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
				nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
			nIndSizePresetGame3[x][nFamily]--;
		}
		else	// Actualizar
		{
			nIndPresetGame3[x][nFamily*5+nFound] = nPut;
			nIndLastPresetGame3[x] = nFamily*5+nFound;
			nTimeLastPresetGame3[x] = nCountGame3;
		}
	}
}

void MakeChoiceCharacterGame3(int x)
{
	bCharacterGame3[x] = false;
	int nPut = nRefCharacterGame3[nCurCharacterGame3[x]-1]+47;
	int nFamily = 5;
	int nFound = FoundPresetGame3(x, nFamily, 46, 59);
	if(nFound == -1 && nCurCharacterGame3[x])
	{
		nFound = nIndSizePresetGame3[x][nFamily];
		nIndSizePresetGame3[x][nFamily]++;
		nIndPresetGame3[x][nFamily*5+nFound] = nPut;
		nIndLastPresetGame3[x] = nFamily*5+nFound;
		nTimeLastPresetGame3[x] = nCountGame3;
	}
	if(nFound != 1)
	{
		if(!nCurCharacterGame3[x])	// Desactivar
		{
			nIndPresetGame3[x][nFamily*5+nFound] = 0;
			for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
				nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
			nIndSizePresetGame3[x][nFamily]--;
		}
		else	// Actualizar
		{
			nIndPresetGame3[x][nFamily*5+nFound] = nPut;
			nIndLastPresetGame3[x] = nFamily*5+nFound;
			nTimeLastPresetGame3[x] = nCountGame3;
		}
	}
}


void TurnSkinsGame3(bool bIzq, int x)
{
	if(bIzq)
	{
		nCurSkinGame3[x]--;
		if(nCurSkinGame3[x] < 0) nCurSkinGame3[x] = nSizeSkinGame3;
	}
	else
	{
		nCurSkinGame3[x]++;
		if(nCurSkinGame3[x] >= (nSizeSkinGame3+1)) nCurSkinGame3[x] = 0;
	}
	nDirTurnSkinGame3[x] = bIzq?1:2;
	nTimeTurnSkinGame3[x] = nCountGame3;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
}

void TurnCharactersGame3(bool bIzq, int x)
{
	if(bIzq)
	{
		nCurCharacterGame3[x]--;
		if(nCurCharacterGame3[x] < 0) nCurCharacterGame3[x] = nSizeCharacterGame3;
	}
	else
	{
		nCurCharacterGame3[x]++;
		if(nCurCharacterGame3[x] >= (nSizeCharacterGame3+1)) nCurCharacterGame3[x] = 0;
	}
	nDirTurnCharacterGame3[x] = bIzq?1:2;
	nTimeTurnCharacterGame3[x] = nCountGame3;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
}

void DrawSkinsGame3(int x)
{
#define TIME_TRANSI_SKIN_GAME3 500
	__int64 nTimeSkin = nCountGame3 - nTimeSkinGame3[x];
	double dApt;
	int n[5];
	for(int y = 0; y < 5; y++)
	{
		n[y] = (y-2+nCurSkinGame3[x])%(nSizeSkinGame3+1);
		while(n[y] < 0) n[y] += (nSizeSkinGame3+1);
	}
	if(nTimeSkin >= 0 && nTimeSkin < TIME_TRANSI_SKIN_GAME3)
	{
		dApt = double(nTimeSkin)/TIME_TRANSI_SKIN_GAME3;
		glColor4d(1.0, 1.0, 1.0, dApt);
		glPushMatrix();
		glTranslated(0.0, -2.5*(1-dApt)+11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga3res->skin[168]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
		glBindTexture(GL_TEXTURE_2D, ga3res->skin[167]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[2]==0?ga3res->skin[169]->GetCurrentTex():uTexSkinGame3[n[2]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(10.0-5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga3res->skin[167]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[3]==0?ga3res->skin[169]->GetCurrentTex():uTexSkinGame3[n[3]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(-10.0+5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga3res->skin[167]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[1]==0?ga3res->skin[169]->GetCurrentTex():uTexSkinGame3[n[1]-1]);
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
	if(nTimeSkin >= TIME_TRANSI_SKIN_GAME3)
	{
#define TIME_TURNSKIN_GAME3 300
		glPushMatrix();
		glTranslated(0.0, 11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga3res->skin[168]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		__int64 nTimeTurn = nCountGame3 - nTimeTurnSkinGame3[x];
		dApt = 0.0;
		if(nTimeTurn >= 0 && nTimeTurn <= TIME_TURNSKIN_GAME3 && nDirTurnSkinGame3[x])
		{
			dApt = (nDirTurnSkinGame3[x]==1?-1:1)*(1-double(nTimeTurn)/TIME_TURNSKIN_GAME3);
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
			glBindTexture(GL_TEXTURE_2D, ga3res->skin[167]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, n[u]==0?ga3res->skin[169]->GetCurrentTex():uTexSkinGame3[n[u]-1]);
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

void DrawCharactersGame3(int x)
{
#define TIME_TRANSI_CHARACTER_GAME3 500
	__int64 nTimeCharacter = nCountGame3 - nTimeCharacterGame3[x];
	double dApt;
	int n[5];
	for(int y = 0; y < 5; y++)
	{
		n[y] = (y-2+nCurCharacterGame3[x])%(nSizeCharacterGame3+1);
		while(n[y] < 0) n[y] += (nSizeCharacterGame3+1);
	}
	if(nTimeCharacter >= 0 && nTimeCharacter < TIME_TRANSI_CHARACTER_GAME3)
	{
		dApt = double(nTimeCharacter)/TIME_TRANSI_CHARACTER_GAME3;
		glColor4d(1.0, 1.0, 1.0, dApt);
		glPushMatrix();
		glTranslated(0.0, -2.5*(1-dApt)+11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga3res->skin[171]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
		glBindTexture(GL_TEXTURE_2D, ga3res->skin[170]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[2]==0?ga3res->skin[172]->GetCurrentTex():uTexCharacterGame3[n[2]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(10.0-5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga3res->skin[170]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[3]==0?ga3res->skin[172]->GetCurrentTex():uTexCharacterGame3[n[3]-1]);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, -5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0,  5.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(-10.0+5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga3res->skin[170]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[1]==0?ga3res->skin[172]->GetCurrentTex():uTexCharacterGame3[n[1]-1]);
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
	if(nTimeCharacter >= TIME_TRANSI_CHARACTER_GAME3)
	{
#define TIME_TURNCHARACTER_GAME3 300
		glPushMatrix();
		glTranslated(0.0, 11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga3res->skin[171]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();
		__int64 nTimeTurn = nCountGame3 - nTimeTurnCharacterGame3[x];
		dApt = 0.0;
		if(nTimeTurn >= 0 && nTimeTurn <= TIME_TURNCHARACTER_GAME3 && nDirTurnCharacterGame3[x])
		{
			dApt = (nDirTurnCharacterGame3[x]==1?-1:1)*(1-double(nTimeTurn)/TIME_TURNCHARACTER_GAME3);
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
			glBindTexture(GL_TEXTURE_2D, ga3res->skin[170]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, n[u]==0?ga3res->skin[172]->GetCurrentTex():uTexCharacterGame3[n[u]-1]);
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

void DeleteSkinsGame3();
void DeleteCharactersGame3();

void EnumSkinsGame3()
{
	DeleteSkinsGame3();
	for(int x = 0; x < MAX_SKINS_GAME3; x++)
	{
		if((np[0] && !player_sesion[0]->nLockedAsp[x]) ||
			(np[1] && !player_sesion[1]->nLockedAsp[x]))
		{
			nSizeSkinGame3++;
			nRefSkinGame3 = (int*)realloc(nRefSkinGame3, nSizeSkinGame3*sizeof(int));
			uTexSkinGame3 = (GLuint*)realloc(uTexSkinGame3, nSizeSkinGame3*sizeof(GLuint));
			nRefSkinGame3[nSizeSkinGame3-1] = x;
			CString str;
			str.Format(_T("skins/%d/Preview.png"), x+1);
			uTexSkinGame3[nSizeSkinGame3-1] = GenTexImage(str);
		}
	}
}

void EnumCharactersGame3()
{
	DeleteCharactersGame3();
	for(int x = 0; x < MAX_CHARACTERS_GAME3; x++)
	{
		if((np[0] && !player_sesion[0]->nLockedAsp[20+x]) ||
			(np[1] && !player_sesion[1]->nLockedAsp[20+x]))
		{
			nSizeCharacterGame3++;
			nRefCharacterGame3 = (int*)realloc(nRefCharacterGame3, nSizeCharacterGame3*sizeof(int));
			uTexCharacterGame3 = (GLuint*)realloc(uTexCharacterGame3, nSizeCharacterGame3*sizeof(GLuint));
			nRefCharacterGame3[nSizeCharacterGame3-1] = x;
			CString str;
			str.Format(_T("Characters/%d/Preview.png"), x+1);
			uTexCharacterGame3[nSizeCharacterGame3-1] = GenTexImage(str);
		}
	}
}

void DeleteSkinsGame3()
{
	if(nSizeSkinGame3)
	{
		glDeleteTextures(nSizeSkinGame3, uTexSkinGame3);
		nSizeSkinGame3 = 0;
		nRefSkinGame3 = (int*)realloc(nRefSkinGame3, nSizeSkinGame3*sizeof(int));
		uTexSkinGame3 = (GLuint*)realloc(uTexSkinGame3, nSizeSkinGame3*sizeof(GLuint));
	}
}

void DeleteCharactersGame3()
{
	if(nSizeCharacterGame3)
	{
		glDeleteTextures(nSizeCharacterGame3, uTexCharacterGame3);
		nSizeCharacterGame3 = 0;
		nRefCharacterGame3 = (int*)realloc(nRefCharacterGame3, nSizeCharacterGame3*sizeof(int));
		uTexCharacterGame3 = (GLuint*)realloc(uTexCharacterGame3, nSizeCharacterGame3*sizeof(GLuint));
	}
}

int FoundFirstRetroGame3(int nSet)
{
	for(int x = 0; x < 1000; x++)
	{
		if(nNOLTotalSongsGame3[x] == nSet)
			return x;
	}
	return 0;
}

int LoadSongGame3()
{
	// ** Cargar datos basicos
	ARGSTEP arg;
	arg.strMusic.Format(_T("song/%ls/%ls.mp3"), chSongsGame3[nCurSongGame3], chSongsGame3[nCurSongGame3]);
	arg.strLights.Format(_T("song/%ls/%ls.lgt"), chSongsGame3[nCurSongGame3], chSongsGame3[nCurSongGame3]);
	CString str;
	TCHAR ch[260];
	for(int x = 0; x < 2; x++) if(np[x])
	{
		str.Format(_T("song/%ls/%ls.acc"), chSongsGame3[nCurSongGame3], chSongsGame3[nCurSongGame3]);
		FILE* hFile = _fopen(str, _T("r"));
		if(hFile != NULL)
		{
			if( _fgets(ch, 260, hFile) != NULL)
			{
				arg.strBGA.Format(_T("song/%ls/%ls"), chSongsGame3[nCurSongGame3], ch);
				if(arg.strBGA.Find(_T('[')) != -1)
					arg.strBGA.Format(_T("song/GNRBGA/%ls"), ch);
				arg.strBGA.Remove(_T('\n')); arg.strBGA.Remove(_T('\r'));
			}
			else arg.strBGA.Format(_T("song/%ls/%ls.avi"), chSongsGame3[nCurSongGame3], chSongsGame3[nCurSongGame3]);
			fclose(hFile);
		}
		else arg.strBGA.Format(_T("song/%ls/%ls.avi"), chSongsGame3[nCurSongGame3], chSongsGame3[nCurSongGame3]);
		if(player_sesion[x]->nLockedAsp[ITEM_BGAOFF])
		{
			arg.strBGA = _T("");
			break;
		}
		if(player_sesion[x]->nBgaSpecNew[nRefSongsGame3[nCurSongGame3]] != 1)
		{
			if(player_sesion[x]->nBgaSpecNew[nRefSongsGame3[nCurSongGame3]] == 0)
			{
				arg.strBGA = _T("");
				break;
			}
			str.Format(_T("song/%ls/%ls.acc"), chSongsGame3[nCurSongGame3], chSongsGame3[nCurSongGame3]);
			hFile = _fopen(str, _T("r"));
			if(hFile != NULL)
			{
				TCHAR ch[260];
				bool bCount = false;
				for(int y = 0; y < (player_sesion[x]->nBgaSpecNew[nRefSongsGame3[nCurSongGame3]]-1); y++)
				{
					if(_fgets(ch, 260, hFile) == NULL) break;
					bCount = true;
					str = ( ch);
				}
				fclose(hFile);
				if(bCount)
				{
					arg.strBGA.Format(_T("song/%ls/%ls"), chSongsGame3[nCurSongGame3], str);
					break;
				}
			}
		}
	}
	arg.strSplash.Format(_T("song/%ls/%ls.png"), chSongsGame3[nCurSongGame3], chSongsGame3[nCurSongGame3]);
	arg.bAutoPlay = false;
	if(!LoadSong(arg)) return 0;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		HideSesionTrayExp(x);
		arg.strFile.Format(_T("song/%ls/%ls_%ls.sksf"), chSongsGame3[nCurSongGame3], chSongsGame3[nCurSongGame3], chSpeModes[nModeCurSongGame3[x]]);
		arg.dUserVel = 1.0;
		arg.bIsSingle = nModeCurSongGame3[x]<3?true:false;
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
		nCode = FoundPresetGame3(x, 0, 1, 7);
		if(nCode == -1)
			arg.dUserVel = 1.0;
		else
		{
			switch(nIndPresetGame3[x][nCode])	// 0*5+nCode
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
		nCode = FoundPresetGame3(x, 0, 8, 9);
		if(nCode != -1)
		{
			switch(nIndPresetGame3[x][nCode])	// 0*5+nCode
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
		nCode = FoundPresetGame3(x, 1, 10, 11);
		if(nCode != -1)
		{
			switch(nIndPresetGame3[x][5+nCode])	// 0*5+nCode
			{
			case 10:
				arg.nVanish = 5;
				break;
			case 11:
				arg.nVanish = 11;
				break;
			}
		}
		if(FoundPresetGame3(x, 1, 12, 12) != -1) arg.bFlash = true;
		if(FoundPresetGame3(x, 1, 13, 13) != -1) arg.bFreedom = true;
		// Familia 2
		nCode = FoundPresetGame3(x, 2, 24, 45);
		if(nCode != -1)
		{
			switch(nIndPresetGame3[x][10+nCode])
			{
			case 24:
				arg.bRandomSkin = true;
				break;
			default:
				arg.nSkin = nIndPresetGame3[x][10+nCode]-24;
				break;
			}
		}
		// Familia 5
		nCode = FoundPresetGame3(x, 5, 46, 59);
		if(nCode != -1)
		{
			switch(nIndPresetGame3[x][25+nCode])
			{
			case 46:
				arg.bCharacter = true;
				arg.nCharacter = (rand()%12)+1;
				arg.nLevelCharacer = 20; //????
				break;
			default:
				arg.bCharacter = true;
				arg.nCharacter = nIndPresetGame3[x][25+nCode]-46;
				arg.nLevelCharacer = 20; //????
				break;
			}
		}
		// Familia 3
		nCode = FoundPresetGame3(x, 3, 14, 15);
		if(nCode != -1)
		{
			switch(nIndPresetGame3[x][15+nCode])	// 0*5+nCode
			{
			case 14:
				arg.bRS = true;
				break;
			case 15:
				arg.bMirror = true;
				break;
			}
		}
		//if(FoundPresetGame3(x, 3, 16, 16) != -1);
		if(FoundPresetGame3(x, 3, 17, 17) != -1) arg.bTrainingMode = true;
		if(FoundPresetGame3(x, 3, 18, 18) != -1) arg.dPowTimer = POW_TIMER_HARD;
		// Familia 4
		if(FoundPresetGame3(x, 4, 19, 19) != -1) arg.bSnake = true;
		if(FoundPresetGame3(x, 4, 20, 20) != -1) arg.bNXMode = true;
		if(FoundPresetGame3(x, 4, 21, 21) != -1) arg.bXMode = true;
		nCode = FoundPresetGame3(x, 4, 22, 23);
		if(nCode != -1)
		{
			switch(nIndPresetGame3[x][20+nCode])	// 0*5+nCode
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

void DeleteSongGame3()
{
	for(int x = 0; x < 2; x++) if(np[x])
	{
		if(player_sesion[x]->nSpecialRecords[nRefSongsGame3[nCurSongGame3]*5+nModeCurSongGame3[x]][0] < (x?viewp2->status.nScore:viewp1->status.nScore))
		{
			for(int y = 19; y >= 0; y--)
			{
				player_sesion[x]->nSpecialRecords[nRefSongsGame3[nCurSongGame3]*5+nModeCurSongGame3[x]][y+1] =
					player_sesion[x]->nSpecialRecords[nRefSongsGame3[nCurSongGame3]*5+nModeCurSongGame3[x]][y];
			}
			player_sesion[x]->nSpecialRecords[nRefSongsGame3[nCurSongGame3]*5+nModeCurSongGame3[x]][0] = (x?viewp2->status.nScore:viewp1->status.nScore);
		}
		if(machine_sesion->nSpecialRecords[nRefSongsGame3[nCurSongGame3]*5+nModeCurSongGame3[x]] < (x?viewp2->status.nScore:viewp1->status.nScore))
		{
			machine_sesion->nSpecialRecords[nRefSongsGame3[nCurSongGame3]*5+nModeCurSongGame3[x]] = (x?viewp2->status.nScore:viewp1->status.nScore);
#ifndef _WIN32
			Utf16ToUtf32(player_sesion[x]->chName, machine_sesion->chArcadeRecords[nRefSongsGame3[nCurSongGame3]*10+nModeCurSongGame3[x]], 12);
#else
			_strcpy(machine_sesion->chArcadeRecords[nRefSongsGame3[nCurSongGame3]*10+nModeCurSongGame3[x]], player_sesion[x]->chName);
#endif
		}
	}
	for(int x = 0; x < 2; x++) if(g_bPlayer[x])
	{
		DeleteSongCompile(x+1);
	}
	DeleteSong();
}

CString GetIDTotalSongsGame3(int nIndex)
{
	if(nIndex >= nSizeTotalGame3) return CString(_T(""));
	CString str;
	str = ( chTotalSongsGame3[nIndex]);
	return str;
}

CString GetNameTotalSongsGame3(int nIndex)
{
	if(nIndex >= nSizeTotalGame3) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame3[nIndex], chTotalSongsGame3[nIndex], chLang);
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

CString GetArtistTotalSongsGame3(int nIndex)
{
	if(nIndex >= nSizeTotalGame3) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame3[nIndex], chTotalSongsGame3[nIndex], chLang);
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

CString GetBPMTotalSongsGame3(int nIndex)
{
	if(nIndex >= nSizeTotalGame3) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chTotalSongsGame3[nIndex], chTotalSongsGame3[nIndex], chLang);
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

CString GetNameSongsGame3(int nIndex)
{
	if(nIndex >= nSizeSongsGame3) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame3[nIndex], chSongsGame3[nIndex], chLang);
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

CString GetArtistSongsGame3(int nIndex)
{
	if(nIndex >= nSizeSongsGame3) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame3[nIndex], chSongsGame3[nIndex], chLang);
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

CString GetBPMSongsGame3(int nIndex)
{
	if(nIndex >= nSizeSongsGame3) return CString(_T("No song"));
	TCHAR chFile[260];
	_sprintf(chFile, _T("song/%ls/%ls_%ls.acc"), chSongsGame3[nIndex], chSongsGame3[nIndex], chLang);
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

void GetLevelSongsGame3(int nIndex, int nMode, int* nLevel, double* d1, double* d2, double* d3, double* d4, double* d5)
{
	if(nIndex >= nSizeSongsGame3)
	{
		(*nLevel) = 1;
		(*d1) = 0.0;
		(*d2) = 0.0;
		(*d3) = 0.0;
		(*d4) = 0.0;
		(*d5) = 0.0;
		return;
	}
	if(nMode >= 5)
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
	_sprintf(chFile, _T("song/%ls/%ls_%ls.sksf"), chSongsGame3[nIndex], chSongsGame3[nIndex], chSpeModes[nMode]);
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

void GetBestScoresGame3(int nIndex, int nMode)
{
	nBestScoreMachineGame3 = machine_sesion->nSpecialRecords[nRefSongsGame3[nIndex]*5+nMode];
	for(int x = 0; x < 2; x++)
	{
		if(np[x]) nBestScorePlayerGame3[x] = player_sesion[x]->nSpecialRecords[nRefSongsGame3[nIndex]*5+nMode][0];
		else nBestScorePlayerGame3[x] = 0;
	}
	strBestScoreMachineGame3 = ( machine_sesion->chSpecialRecords[nRefSongsGame3[nIndex]*5+nMode]);
}

int LoadDefaultsGame3(int nPlayer)
{
	if(!AdaptMachineProfile(1, nPlayer)) return 0;
	return 1;
}

int EnumTotalSongsGame3()
{
	FILE* hFile = NULL;
	hFile = _fopen(_T("songSpecialNew.acc"), _T("r"));
	if(hFile == NULL) return 0;
	TCHAR ch[500];
	int nChannel = 0;
	int nIsRetro = 0;
	nSizeTotalGame3 = 0;
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
				if(nChannel == -1)	// Retro Zone
				{
					nIsRetro = 1;
				}
			}
			if(str.Find(_T(':')) != -1)	// Song
			{
				nSizeTotalGame3++;
				int nCode = str.Find(_T(':'));
				_strcpy(chTotalSongsGame3[nSizeTotalGame3-1], str.Left(nCode));
				nCode = str.GetLength() - nCode - 1;
				CString stra = str.Right(nCode);
				int n[] = {0,0,0,0,0};
				if(_sscanf(stra, _T("\t%d\t%d\t%d\t%d\t%d"), &n[0], &n[1], &n[2], &n[3], &n[4]) != 5) {fclose(hFile); return 0;}
				for(int x = 0; x < 5; x++) nModTotalSongsGame3[(nSizeTotalGame3-1)*5+x] = n[x];
				nChaTotalSongsGame3[nSizeTotalGame3-1] = nChannel==-1?0:nChannel;
				nNOLTotalSongsGame3[nSizeTotalGame3-1] = nIsRetro;
			}
		}
	}
	fclose(hFile);
	nSizeTotalGame3Ex = 0;
	for(int x = 0; x < (nSizeTotalGame3*5); x++) if(nModTotalSongsGame3[x]) nSizeTotalGame3Ex++;
	for(int x = 0; x < nSizeTotalGame3; x++)
	{
		TCHAR str[260];
		_sprintf(str, _T("song/preview/%ls.png"), chTotalSongsGame3[x]);
		//UnicodeToAnsi(str, &ch);
		nTexTotalSongsGame3[x] = GenTexImage(str);
	}
	return 1;
}

int EnumSongsGame3(int nSet)
{
	nSizeSongsGame3 = 0;
	int nAutorize[5000];
	for(int k = 0; k < 5000; k++)
	{
		nAutorize[k] = 0;
		nModSongsGame3[k] = 0;
		if(k < 1000)
		{
			//nTexSongsGame3[k] = m_uNullTex;
			nChaSongsGame3[k] = 0;
		}
	}
	int nNp = min(np[0]?np[0]:8, np[1]?np[1]:8);
	for(int d = 0; d < 2; d++) if(np[d])
	{
		for(int s = 0; s < nSizeTotalGame3; s++)
		{
			int nMinus = 2;
			switch(nChaTotalSongsGame3[s])
			{
			case 0:
				nMinus = 8;
				break;
			case 1:
				nMinus = 7;
				break;
			case 2:
				nMinus = 6;
				break;
			case 3:
				nMinus = 5;
				break;
			case 4:
				nMinus = 4;
				break;
			case 5:
				nMinus = 3;
				break;
			case 7:
				nMinus = 1;
				break;
			}
			if(nNp < nMinus)
			{
				for(int m = 0; m < 5; m++)
				{
					nAutorize[s*5+m] = 0;
				}
			}
			else
			{
				for(int m = 0; m < 5; m++)
				{
					if(!player_sesion[d]->nLockedSpecNew[s*5+m] || !machine_sesion->nLockedSpecNew[s*5+m] || player_sesion[d]->nLockedAsp[ITEM_ALLSONG]) nAutorize[s*5+m] = 1;
				}
			}
		}
	}
	for(int s = 0; s < nSizeTotalGame3; s++)
	{
		bool bAutorize = false;
		for(int m = 0; m < ((np[0]&&np[1])?3:5); m++) if(nModTotalSongsGame3[s*5+m] && nAutorize[s*5+m]) bAutorize = true;
		if(nNOLTotalSongsGame3[s] != nSet) bAutorize = false;
		if(bAutorize)
		{
			for(int m = 0; m < 5; m++)
				if(nModTotalSongsGame3[s*5+m] && nAutorize[s*5+m])
					nModSongsGame3[nSizeSongsGame3*5+m] = 1;
			nTexSongsGame3[nSizeSongsGame3] = nTexTotalSongsGame3[s];
			nChaSongsGame3[nSizeSongsGame3] = nChaTotalSongsGame3[s];
			nRefSongsGame3[nSizeSongsGame3] = s;
			_strcpy(chSongsGame3[nSizeSongsGame3], chTotalSongsGame3[s]);
			nSizeSongsGame3++;
		}
	}
	return 1;
}

int GetAvalibleModeGame3(int nIndexSong, int nModeGame3Song)
{
	if(nIndexSong < 0 || nIndexSong >= nSizeSongsGame3) return 0;
	if(nModeGame3Song < 0 || nModeGame3Song >= 5)
	{
		for(int y = 0; y < 5; y++)
		{
			if(nModSongsGame3[nIndexSong*5+y]) return y;
		}
	}
	if(nModSongsGame3[nIndexSong*5+nModeGame3Song])
		return nModeGame3Song;
	else
	{
		for(int y = 0; y < 5; y++)
		{
			if(nModSongsGame3[nIndexSong*5+y]) return y;
		}
	}
	return 0;
}

bool IsAvalibleModeGame3(int nIndexSong, int nModeGame3Song)
{
	if(nIndexSong < 0 || nIndexSong >= nSizeSongsGame3) return false;
	if(nModeGame3Song < 0 || nModeGame3Song >= 5) return false;
	if(np[0] && np[1] && nModeGame3Song >= 3) return false;
	if(!nModSongsGame3[nIndexSong*5+nModeGame3Song]) return false;
	return true;
}

bool IsAvalibleModeTotalGame3(int nIndexSong, int nModeGame3Song)
{
	if(nIndexSong < 0 || nIndexSong >= nSizeTotalGame3) return false;
	if(nModeGame3Song < 0 || nModeGame3Song >= 5) return false;
	if(!nModTotalSongsGame3[nIndexSong*5+nModeGame3Song]) return false;
	return true;
}

#define MAX_TARGET_X 60.0
double AlphaFuncForXGame3(double dx)
{
	if(abs(dx) >= MAX_TARGET_X) return 0.0;
	if(abs(dx) <= (MAX_TARGET_X/4*3)) return 1.0;
	return (MAX_TARGET_X - abs(dx)) / (MAX_TARGET_X/4);
}

double ThetaFuncForXGame3(double dx)
{
	if(abs(dx) >= MAX_TARGET_X/4) return -19.0*dx/abs(dx);
	/*if(abs(dx) <= (MAX_TARGET_X/4))*/ return -19.0*dx*4/MAX_TARGET_X;
	//return (MAX_TARGET_X - abs(dx)) / (MAX_TARGET_X/4);
}


void ActualizateIndexShowGame3()
{
	for(int x = 0; x < 9; x++)
	{
		nIndexShowGame3[x] = nCurSongGame3+x-4;
		bool bAsegureLoop = true;
		while(bAsegureLoop)
		{
			bAsegureLoop = false;
			if(nIndexShowGame3[x] < 0)
			{
				nIndexShowGame3[x] = nSizeSongsGame3+nIndexShowGame3[x];
				bAsegureLoop = true;
			}
			if(nIndexShowGame3[x] >= nSizeSongsGame3)
			{
				nIndexShowGame3[x] = nIndexShowGame3[x]-nSizeSongsGame3;
				bAsegureLoop = true;
			}
		}
	}
}

void MoveSongsGame3(bool bIsIzq)
{
	int nb = nCurSongGame3;
	if(bIsIzq)
	{
		nCurSongGame3--;
		if(nCurSongGame3 < 0) nCurSongGame3 = nSizeSongsGame3-1;
		if(nChaSongsGame3[nb] != nChaSongsGame3[nCurSongGame3]) nTimeChaGame3 = nCountGame3;
		nModeGame3 = -1;
		nChosesGame3 = 0;
		/*Borrar textura sobrante*/
		/*if(nTexCurSongGame3[8] != m_uNullTex)
			glDeleteTextures(1, &nTexCurSongGame3[8]);*/
		ActualizateIndexShowGame3();
		//Mover a la derecha los items
		for(int x = 7; x >= 0; x--)
		{
			nTexCurSongGame3[x+1] = nTexCurSongGame3[x];
		}
		// Finalmente cargar la nueva imagen
		/*TCHAR chFile[260];
		_sprintf(chFile, _T("song/preview/%ls.png"), chSongsGame3[nIndexShowGame3[0]]);*/
		nTexCurSongGame3[0] = nTexSongsGame3[nIndexShowGame3[0]];
		nTimeCurSongGame3 = nCountGame3;
	}
	else
	{
		nCurSongGame3++;
		if(nCurSongGame3 >= nSizeSongsGame3) nCurSongGame3 = 0;
		if(nChaSongsGame3[nb] != nChaSongsGame3[nCurSongGame3]) nTimeChaGame3 = nCountGame3;
		nModeGame3 = 1;
		nChosesGame3 = 0;
		/*Borrar textura sobrante*/
		//if(nTexCurSongGame3[0] != m_uNullTex) glDeleteTextures(1, &nTexCurSongGame3[0]);
		ActualizateIndexShowGame3();
		//Mover a la izquierda los items
		for(int x = 0; x < 8; x++)
		{
			nTexCurSongGame3[x] = nTexCurSongGame3[x+1];
		}
		// Finalmente cargar la nueva imagen
		TCHAR chFile[260];
		_sprintf(chFile, _T("song/preview/%ls.png"), chSongsGame3[nIndexShowGame3[8]]);
		nTexCurSongGame3[8] = nTexSongsGame3[nIndexShowGame3[8]];
		nTimeCurSongGame3 = nCountGame3;
	}
	for(int y = 0; y < 2; y++)
	{
		for(int k = 0; k < 5; k++) dDCurGame3[y][k] = 0.0;
		nTimeModeCurSongGame3[y] = nCountGame3;
		nBackModeCurSongGame3[y] = nModeCurSongGame3[y];
		nModeCurSongGame3[y] = GetAvalibleModeGame3(nCurSongGame3, nPrefModeCurSongGame3[y]);
	}
	//if(movie_array[nb]->bResize) movie_array[nb]->ClearResize();
}

int FoundPresetGame3(int x, int nFamily, int nBegin, int nEnd)
{
	int nFound = -1;
	for(int k = 0; k < nIndSizePresetGame3[x][nFamily]; k++)
	{
		if(nIndPresetGame3[x][nFamily*5+k] >= nBegin && nIndPresetGame3[x][nFamily*5+k] <= nEnd)
		{
			nFound = k;
			break;
		}
	}
	return nFound;
}

void CheckPresetGame3()
{
	for(int x = 0; x < 2; x++) if(np[x])
	{
		bool bPreset = false;
		int nFamily;
		// ** Primer nivel (0)
		nFamily = 0;
		// Random Velocity (RV)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 3 &&
		   nComandGame3[x][2] == 1 &&
		   nComandGame3[x][3] == 3 &&
		   nComandGame3[x][4] == 1 &&
		   nComandGame3[x][5] == 3 &&
		   nComandGame3[x][6] == 1 &&
		   nComandGame3[x][7] == 3 &&
		   nComandGame3[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 1, 7);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 7;
				nIndLastPresetGame3[x] = nFamily*5+nFound;
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 7)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 7;
					nIndLastPresetGame3[x] = nFamily*5+nFound;
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Speed Up
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 3 &&
		   nComandGame3[x][2] == 1 &&
		   nComandGame3[x][3] == 3 &&
		   nComandGame3[x][4] == 1 && !bPreset)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 1, 7);
			int nCurVel = 0;
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nCurVel = 0;
			}
			else nCurVel = nIndPresetGame3[x][nFamily*5+nFound];
			nCurVel++;
			if(nCurVel > 5 || nCurVel < 1)
			{
				nIndPresetGame3[x][nFamily*5+nFound] = 0;
				for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
					nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
				nIndSizePresetGame3[x][nFamily]--;
			}
			else
			{
				nIndPresetGame3[x][nFamily*5+nFound] = nCurVel;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
		}
		// Speed Down
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 1 &&
		   nComandGame3[x][2] == 3 &&
		   nComandGame3[x][3] == 1 &&
		   nComandGame3[x][4] == 3)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 1, 7);
			int nCurVel = 6;
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nCurVel = 6;
			}
			else nCurVel = nIndPresetGame3[x][nFamily*5+nFound];
			nCurVel--;
			if(nCurVel > 5 || nCurVel < 1)
			{
				nIndPresetGame3[x][nFamily*5+nFound] = 0;
				for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
					nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
				nIndSizePresetGame3[x][nFamily]--;
			}
			else
			{
				nIndPresetGame3[x][nFamily*5+nFound] = nCurVel;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
		}
		// EarthWorm (EW)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 1 &&
		   nComandGame3[x][2] == 0 &&
		   nComandGame3[x][3] == 3 &&
		   nComandGame3[x][4] == 4 &&
		   nComandGame3[x][5] == 1 &&
		   nComandGame3[x][6] == 3 &&
		   nComandGame3[x][7] == 0 &&
		   nComandGame3[x][8] == 4)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, 0, 1, 7);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 6;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 6)	// Desactivar
				{
					nIndPresetGame3[x][nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][k] = nIndPresetGame3[x][k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 6;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Aceleration (AC)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 3 &&
		   nComandGame3[x][2] == 3 &&
		   nComandGame3[x][3] == 1 &&
		   nComandGame3[x][4] == 1 &&
		   nComandGame3[x][5] == 4 &&
		   nComandGame3[x][6] == 4 &&
		   nComandGame3[x][7] == 0 &&
		   nComandGame3[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 8, 9);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 8;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 8)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 8;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Desceleration (DC)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 1 &&
		   nComandGame3[x][2] == 1 &&
		   nComandGame3[x][3] == 3 &&
		   nComandGame3[x][4] == 3 &&
		   nComandGame3[x][5] == 0 &&
		   nComandGame3[x][6] == 0 &&
		   nComandGame3[x][7] == 4 &&
		   nComandGame3[x][8] == 4)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 8, 9);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 9;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 9)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 9;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// ** Segundo nivel (1)
		nFamily = 1;
		// Vanish & NonStep (V & NS)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 4 &&
		   nComandGame3[x][2] == 0 &&
		   nComandGame3[x][3] == 3 &&
		   nComandGame3[x][4] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 10, 11);
			int nCur = 0;
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nCur = 9;
			}
			else nCur = nIndPresetGame3[x][nFamily*5+nFound];
			nCur++;
			if(nCur > 11 || nCur < 10)
			{
				nIndPresetGame3[x][nFamily*5+nFound] = 0;
				for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
					nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
				nIndSizePresetGame3[x][nFamily]--;
			}
			else
			{
				nIndPresetGame3[x][nFamily*5+nFound] = nCur;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
		}
		// Flash (FL)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 3 &&
		   nComandGame3[x][2] == 1 &&
		   nComandGame3[x][3] == 4 &&
		   nComandGame3[x][4] == 3 &&
		   nComandGame3[x][5] == 0 &&
		   nComandGame3[x][6] == 4 &&
		   nComandGame3[x][7] == 1 &&
		   nComandGame3[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 12, 12);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 12;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 12)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 12;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Freedom (FD)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 0 &&
		   nComandGame3[x][2] == 3 &&
		   nComandGame3[x][3] == 1 &&
		   nComandGame3[x][4] == 4 &&
		   nComandGame3[x][5] == 4 &&
		   nComandGame3[x][6] == 3 &&
		   nComandGame3[x][7] == 0 &&
		   nComandGame3[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 13, 13);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 13;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 13)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 13;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// ** Tercer nivel (2)
		nFamily = 2;
		// Random Skin (RSK)
		if(nComandGame3[x][0] == 1 &&
		   nComandGame3[x][1] == 1 &&
		   nComandGame3[x][2] == 1 &&
		   nComandGame3[x][3] == 3 &&
		   nComandGame3[x][4] == 3 &&
		   nComandGame3[x][5] == 3 &&
		   nComandGame3[x][6] == 0 &&
		   nComandGame3[x][7] == 0 &&
		   nComandGame3[x][8] == 0 &&
		   nComandGame3[x][9] == 4 &&
		   nComandGame3[x][10] == 4 &&
		   nComandGame3[x][11] == 4)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 24, 45);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 24;
				nIndLastPresetGame3[x] = nFamily*5+nFound;
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 24)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 24;
					nIndLastPresetGame3[x] = nFamily*5+nFound;
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		nFamily = 5;
		// Random Character (RCH)
		if(nComandGame3[x][0] == 4 &&
		   nComandGame3[x][1] == 4 &&
		   nComandGame3[x][2] == 4 &&
		   nComandGame3[x][3] == 0 &&
		   nComandGame3[x][4] == 0 &&
		   nComandGame3[x][5] == 0 &&
		   nComandGame3[x][6] == 3 &&
		   nComandGame3[x][7] == 3 &&
		   nComandGame3[x][8] == 3 &&
		   nComandGame3[x][9] == 1 &&
		   nComandGame3[x][10] == 1 &&
		   nComandGame3[x][11] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 46, 59);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 45;
				nIndLastPresetGame3[x] = nFamily*5+nFound;
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 45)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 45;
					nIndLastPresetGame3[x] = nFamily*5+nFound;
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Skins
		if(nComandGame3[x][0] == 1 &&
		   nComandGame3[x][1] == 2 &&
		   nComandGame3[x][2] == 4 &&
		   nComandGame3[x][3] == 0 &&
		   nComandGame3[x][4] == 2 &&
		   nComandGame3[x][5] == 3)
		{
			bPreset = true;
			bSkinGame3[x] = true;
			nTimeSkinGame3[x] = nCountGame3;
			nCurSkinGame3[x] = 0;
			nDirTurnSkinGame3[x] = 0;
		}
		if(nComandGame3[x][0] == 3 &&
		   nComandGame3[x][1] == 2 &&
		   nComandGame3[x][2] == 0 &&
		   nComandGame3[x][3] == 4 &&
		   nComandGame3[x][4] == 2 &&
		   nComandGame3[x][5] == 1)
		{
			bPreset = true;
			bCharacterGame3[x] = true;
			nTimeCharacterGame3[x] = nCountGame3;
			nCurCharacterGame3[x] = 0;
			nDirTurnCharacterGame3[x] = 0;
		}
		// Characters
		// ** Cuarto Nivel (3)
		nFamily = 3;
		// Random Step (RS)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 4 &&
		   nComandGame3[x][2] == 0 &&
		   nComandGame3[x][3] == 4 &&
		   nComandGame3[x][4] == 0 &&
		   nComandGame3[x][5] == 3 &&
		   nComandGame3[x][6] == 1 &&
		   nComandGame3[x][7] == 3 &&
		   nComandGame3[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 14, 15);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 14;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 14)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 14;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Mirror (M)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 1 &&
		   nComandGame3[x][2] == 3 &&
		   nComandGame3[x][3] == 0 &&
		   nComandGame3[x][4] == 4 &&
		   nComandGame3[x][5] == 1 &&
		   nComandGame3[x][6] == 3 &&
		   nComandGame3[x][7] == 0 &&
		   nComandGame3[x][8] == 4)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 14, 15);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 15;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 15)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 15;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Reverse Grade (RG)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 1 &&
		   nComandGame3[x][2] == 3 &&
		   nComandGame3[x][3] == 2 &&
		   nComandGame3[x][4] == 0 &&
		   nComandGame3[x][5] == 4 &&
		   nComandGame3[x][6] == 4 &&
		   nComandGame3[x][7] == 0 &&
		   nComandGame3[x][8] == 2 &&
		   nComandGame3[x][9] == 3 &&
		   nComandGame3[x][10] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 16, 16);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 16;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 16)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 16;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Training Mode (T)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 4 &&
		   nComandGame3[x][2] == 1 &&
		   nComandGame3[x][3] == 4 &&
		   nComandGame3[x][4] == 1 &&
		   nComandGame3[x][5] == 3 &&
		   nComandGame3[x][6] == 0 &&
		   nComandGame3[x][7] == 3 &&
		   nComandGame3[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 17, 17);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 17;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 17)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 17;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Hard Judgament (HJ)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 4 &&
		   nComandGame3[x][2] == 3 &&
		   nComandGame3[x][3] == 1 &&
		   nComandGame3[x][4] == 0 &&
		   nComandGame3[x][5] == 1 &&
		   nComandGame3[x][6] == 4 &&
		   nComandGame3[x][7] == 2 &&
		   nComandGame3[x][8] == 0 &&
		   nComandGame3[x][9] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 18, 18);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 18;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 18)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 18;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// ** Quinto Nivel (4)
		nFamily = 4;
		// Snake (SN)
		if(nComandGame3[x][0] == 0 &&
		   nComandGame3[x][1] == 4 &&
		   nComandGame3[x][2] == 2 &&
		   nComandGame3[x][3] == 1 &&
		   nComandGame3[x][4] == 3 &&
		   nComandGame3[x][5] == 0 &&
		   nComandGame3[x][6] == 4 &&
		   nComandGame3[x][7] == 2 &&
		   nComandGame3[x][8] == 1 &&
		   nComandGame3[x][9] == 3)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 19, 19);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 19;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 19)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 19;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// NX Mode (NX)
		if(nComandGame3[x][0] == 3 &&
		   nComandGame3[x][1] == 4 &&
		   nComandGame3[x][2] == 2 &&
		   nComandGame3[x][3] == 1 &&
		   nComandGame3[x][4] == 0 &&
		   nComandGame3[x][5] == 3 &&
		   nComandGame3[x][6] == 4 &&
		   nComandGame3[x][7] == 2 &&
		   nComandGame3[x][8] == 1 &&
		   nComandGame3[x][9] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 20, 20);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 20;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 20)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 20;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// X Mode (X)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 1 &&
		   nComandGame3[x][2] == 4 &&
		   nComandGame3[x][3] == 1 &&
		   nComandGame3[x][4] == 4 &&
		   nComandGame3[x][5] == 3 &&
		   nComandGame3[x][6] == 0 &&
		   nComandGame3[x][7] == 3 &&
		   nComandGame3[x][8] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 21, 21);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 21;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 21)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 21;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// UnderAttack (UA)
		if(nComandGame3[x][0] == 2 &&
		   nComandGame3[x][1] == 3 &&
		   nComandGame3[x][2] == 2 &&
		   nComandGame3[x][3] == 0 &&
		   nComandGame3[x][4] == 2 &&
		   nComandGame3[x][5] == 0 &&
		   nComandGame3[x][6] == 2 &&
		   nComandGame3[x][7] == 0)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 22, 23);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 22;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 22)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 22;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Drop (DR)
		if(nComandGame3[x][0] == 4 &&
		   nComandGame3[x][1] == 4 &&
		   nComandGame3[x][2] == 3 &&
		   nComandGame3[x][3] == 0 &&
		   nComandGame3[x][4] == 1 &&
		   nComandGame3[x][5] == 4 &&
		   nComandGame3[x][6] == 3 &&
		   nComandGame3[x][7] == 0 &&
		   nComandGame3[x][8] == 1)
		{
			bPreset = true;
			// Encontrar la clave
			int nFound = FoundPresetGame3(x, nFamily, 22, 23);
			if(nFound == -1)
			{
				nFound = nIndSizePresetGame3[x][nFamily];
				nIndSizePresetGame3[x][nFamily]++;
				nIndPresetGame3[x][nFamily*5+nFound] = 23;
				nIndLastPresetGame3[x] = nFamily*5+nFound;	//1*5+nFound
				nTimeLastPresetGame3[x] = nCountGame3;
			}
			else
			{
				if(nIndPresetGame3[x][nFamily*5+nFound] == 23)	// Desactivar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 0;
					for(int k = nFound; k < (nIndSizePresetGame3[x][nFamily]-1); k++)
						nIndPresetGame3[x][nFamily*5+k] = nIndPresetGame3[x][nFamily*5+k+1];
					nIndSizePresetGame3[x][nFamily]--;
				}
				else	// Actualizar
				{
					nIndPresetGame3[x][nFamily*5+nFound] = 23;
					nIndLastPresetGame3[x] = nFamily*5+nFound;	//0*5+nFound
					nTimeLastPresetGame3[x] = nCountGame3;
				}
			}
		}
		// Toggle Null
		if(nComandGame3[x][0] == 4 &&
		   nComandGame3[x][1] == 0 &&
		   nComandGame3[x][2] == 4 &&
		   nComandGame3[x][3] == 0 &&
		   nComandGame3[x][4] == 4 &&
		   nComandGame3[x][5] == 0)
		{
			bPreset = true;
			for(int z = 0; z < 30; z++) nIndPresetGame3[x][z] = 0;
			for(int z = 0; z < 6; z++) nIndSizePresetGame3[x][z] = 0;
			nIndLastPresetGame3[x] = -1;
			nTimeLastPresetGame3[x] = nCountGame3;
		}
		if(bPreset)
		{
			nChosesGame3 = 0;
			nComandGame3[x][0] = -1;
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_option, 0, &channels[2]);
		}
	}
}

#define TIME_FAST_CHANGE_GAME3 1000
#define TIME_CHANGE_LOW_GAME3 350
#define TIME_CHANGE_HIGH_GAME3 100

void PlaySoundTransiAboutLimitGame3(int nLimit)
{
	if(nLimit >= TIME_CHANGE_LOW_GAME3)
	{
		nTimeTransiSongBorderGame3 = TIME_TRANSI_SONGBORDER_GAME3;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
	}
	else
	{
		nTimeTransiSongBorderGame3 = TIME_CHANGE_HIGH_GAME3;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)6, sound_change2, 0, &channels[6]);
	}
}

int DrawInterfaceSelectGame3()
{
	if(!np[0] && !np[1]) return 1;
	nCountGame3 += dwTick-dwLastTick;
	if(movie_specialbg->bLoad)
	{
		movie_specialbg->Update(dwWait);
		movie_specialbg->GrabAVIFrame();
		glBindTexture(GL_TEXTURE_2D, movie_specialbg->tex);

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
	__int64 nTransBorder = nCountGame3 - nTimeCurSongGame3;
	int nOrder[] = {0,1,2,3,8,7,6,5,4};
	if(nChangeRetroGame3 == 0)
	{
		if(nChosesGame3 < CHOOSES_GAME3)
		{
			for(int i = 0; i < 9; i++)
			{
				int x = nOrder[i];
				double dTargetx = double(x-4)*MAX_TARGET_X/4;
				if(nCountGame3 >= TIME_START_SELECTOR_GAME3 && nCountGame3 < (TIME_START_SELECTOR_GAME3+TIME_TRANSI_SELECTOR_GAME3))
				{
					if(x == 4)
					{
						if(nCountGame3 < (TIME_TRANSI_START_MAIN_GAME3+TIME_START_SELECTOR_GAME3))
						{
							dApt = pow(double(nCountGame3-TIME_START_SELECTOR_GAME3)/TIME_TRANSI_START_MAIN_GAME3, 2);
							glPushMatrix();
							glTranslated(0.0, 0.0, -60.0);
							glScaled(1.25+(1-dApt), 1.25+(1-dApt), 1.0);
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
						if(nCountGame3 >= (TIME_TRANSI_START_MAIN_GAME3+TIME_START_SELECTOR_GAME3))
						{
							glPushMatrix();
							glTranslated(0.0, 0.0, -60.0);
							glScaled(1.25, 1.25, 1.0);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
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
						__int64 nStartTransi = TIME_START_SELECTOR_GAME3+TIME_TRANSI_SELECTOR_GAME3/4*(abs(x-4)-1);
						__int64 nLimitTransi = TIME_START_SELECTOR_GAME3+TIME_TRANSI_SELECTOR_GAME3/4*(abs(x-4));
						if(nCountGame3 >= nStartTransi && nCountGame3 < nLimitTransi)
						{
							dApt = double(nCountGame3-nStartTransi)/double(nLimitTransi-nStartTransi);
							double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(1.0-dApt)+dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXGame3(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForXGame3(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
							glBegin(GL_QUADS);
  							glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
						if(nCountGame3 >= nLimitTransi)
						{
							double dx = dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXGame3(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForXGame3(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
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
				if(nCountGame3 >= (TIME_START_SELECTOR_GAME3+TIME_TRANSI_SELECTOR_GAME3))
				{
					double dx = dTargetx;
					if(nModeGame3!=0)
					{
						if(nTransBorder < nTimeTransiSongBorderGame3)
						{
							dx += double(nModeGame3)*MAX_TARGET_X/4*(1.0-double(nTransBorder)/double(nTimeTransiSongBorderGame3));
						}
					}
					double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
					double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
					double dtheta = ThetaFuncForXGame3(dx);
					double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
					double dalpha = AlphaFuncForXGame3(dx);
					glPushMatrix();
					glTranslated(dx, dy, dz-60.0);
					glRotated(dtheta, 0.0, 1.0, 0.0);
					glScaled(dscale, dscale, 1.0);
					glColor4d(1.0, 1.0, 1.0, dalpha);
					if(x == 4 && nTransBorder > TIME_SHOW_PREVIEW_GAME3 && movie_songpreviewgame3->bLoad && movie_songpreviewgame3->bResize && bShow)
					{
	#define TIME_SHOWIMAGE_SONGBORDER_GAME3 500
						movie_songpreviewgame3->Update(dwWait);
						movie_songpreviewgame3->GrabAVIFrame();
						__int64 nCaretCover = movie_songpreviewgame3->dwNext;
						glBindTexture(GL_TEXTURE_2D, movie_songpreviewgame3->tex);

						glColor4d(1.0, 1.0, 1.0, 1.0);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
						glEnd();
						if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME3)
						{
							dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME3;
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
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
						glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
						glEnd();
					}
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-15.4166,  11.5625, 0.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.4166,  11.5625, 0.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.4166, -11.5625, 0.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.4166, -11.5625, 0.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					if(x == 4 && nTransBorder > TIME_SHOW_PREVIEW_GAME3)
					{
						#define TIME_SHOWNAME_BESTSCORE_GAME3 500
						if(nTransBorder < (TIME_SHOW_PREVIEW_GAME3+TIME_SHOWNAME_BESTSCORE_GAME3))
						{
							dApt = double(nTransBorder-TIME_SHOW_PREVIEW_GAME3)/TIME_SHOWNAME_BESTSCORE_GAME3;
							glColor4d(1.0, 1.0, 1.0, dApt);
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[166]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						if(nTransBorder >= (TIME_SHOW_PREVIEW_GAME3+TIME_SHOWNAME_BESTSCORE_GAME3))
						{
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[166]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
						}
						__int64 nModeTime = nCountGame3 - max(max(nTimeModeCurSongGame3[0], nTimeModeCurSongGame3[1]), nTransBorder+TIME_SHOW_PREVIEW_GAME3);
						if(nModeTime >= 0 && nModeTime < (TIME_SHOWNAME_BESTSCORE_GAME3) && bShow)
						{
							dApt = double(nModeTime)/TIME_SHOWNAME_BESTSCORE_GAME3;
							glColor4d(1.0, 1.0, 1.0, dApt);
							if(nBestScoreMachineGame3 != 0)
							{
								font_Micro->glDrawText(10.599, -9.0625, 9.05331, 9.0625, 0.0, 10.076/*9.82615*/, 0.0, 1.0, 1.5, strBestScoreMachineGame3, DT_CENTER|DT_VCENTER);
								CString str;
								str.Format(_T("%d"), nBestScoreMachineGame3);
								font_Micro->glDrawText(9.05331, -9.0625, 7.50762, 9.0625, 0.0, 8.53046/*8.28046*/, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
							}
							for(int k = 0; k < 2; k++) if(np[k])
							{
								if(nBestScorePlayerGame3[k] != 0)
								{
									CString str;
									str.Format(_T("%d"), nBestScorePlayerGame3[k]);
									font_Micro->glDrawText(-9.05331, -9.0625+(k?7.25:-7.25), -10.599, 9.0625+(k?7.25:-7.25), (k?7.25:-7.25), -9.57615, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
								}
							}
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						if(nModeTime >= (TIME_SHOWNAME_BESTSCORE_GAME3) && bShow)
						{
							if(nBestScoreMachineGame3 != 0)
							{
								font_Micro->glDrawText(10.599, -9.0625, 9.05331, 9.0625, 0.0, 10.076/*9.82615*/, 0.0, 1.0, 1.5, strBestScoreMachineGame3, DT_CENTER|DT_VCENTER);
								CString str;
								str.Format(_T("%d"), nBestScoreMachineGame3);
								font_Micro->glDrawText(9.05331, -9.0625, 7.50762, 9.0625, 0.0, 8.53046/*8.28046*/, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
							}
							for(int k = 0; k < 2; k++) if(np[k])
							{
								if(nBestScorePlayerGame3[k] != 0)
								{
									CString str;
									str.Format(_T("%d"), nBestScorePlayerGame3[k]);
									font_Micro->glDrawText(-9.05331, -9.0625+(k?7.25:-7.25), -10.599, 9.0625+(k?7.25:-7.25), (k?7.25:-7.25), -9.57615, 0.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
								}
							}
						}
					}
					glPopMatrix();
				}
			}
			if(nTransBorder >= 0 && nTransBorder <= TIME_GLOUTH_SONGBORDER_GAME3 && nChosesGame3 == 0)
			{
				dApt = double(nTransBorder)/TIME_GLOUTH_SONGBORDER_GAME3;
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[11]->GetCurrentTex());
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
			if(nTransBorder > TIME_TRANSI_SONGBORDER_GAME3 && nTransBorder <= (TIME_TRANSI_SONGBORDER_GAME3+TIME_GLOUTH_SONGBORDER_GAME3) && nChosesGame3 == 0)
			{
				dApt = double(nTransBorder-TIME_TRANSI_SONGBORDER_GAME3)/TIME_GLOUTH_SONGBORDER_GAME3;
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[11]->GetCurrentTex());
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
	#define TIME_GLOUTH_PER_SONGBORDER_GAME3 1500
			if(nTransBorder > (TIME_TRANSI_SONGBORDER_GAME3+TIME_GLOUTH_SONGBORDER_GAME3) && nChosesGame3 == 0)
			{
				dApt = abs(double((nTransBorder-TIME_TRANSI_SONGBORDER_GAME3-TIME_GLOUTH_SONGBORDER_GAME3)%TIME_GLOUTH_PER_SONGBORDER_GAME3-TIME_GLOUTH_PER_SONGBORDER_GAME3/2))*2/TIME_GLOUTH_PER_SONGBORDER_GAME3;
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[11]->GetCurrentTex());
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
			if(nTransBorder > (TIME_TRANSI_SONGBORDER_GAME3+TIME_GLOUTH_SONGBORDER_GAME3) && nChosesGame3 == 1)
			{
				welres->skin[0]->Update(dwWait);
				welres->skin[1]->Update(dwWait);
				welres->skin[3]->Update(dwWait);
				dApt = double((nCountGame3-nTimeChosesGame3[nChosesGame3-1])%TIME_GLOUTH_SONGBORDER_GAME3)/TIME_GLOUTH_SONGBORDER_GAME3;
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[11]->GetCurrentTex());
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
			if(nTransBorder >= TIME_SHOW_PREVIEW_GAME3)
			{
				if(!bShow)
				{
					/*movie_array[nCurSongGame3]->SetZeroCount();
					movie_array[nCurSongGame3]->SetResize();*/
					TCHAR chFile[260];
					_sprintf(chFile, _T("song/%ls/%ls_P.avi"), chSongsGame3[nCurSongGame3], chSongsGame3[nCurSongGame3]);
					if(movie_songpreviewgame3->bLoad)
					{
						movie_songpreviewgame3->CloseAVI();
						delete movie_songpreviewgame3;
						movie_songpreviewgame3 = new CMovieStream2();
					}
					movie_songpreviewgame3->OpenAVI(chFile);
					movie_songpreviewgame3->bLoop = true;
					nTimeShow = nCountGame3;
					bShow = true;
					_sprintf(chFile, _T("song/%ls/%ls_P.mp3"), chSongsGame3[nCurSongGame3], chSongsGame3[nCurSongGame3]);
					if(bLoadMusicCoverGame3)
					{
						bLoadMusicCoverGame3 = false;
						FMOD_Sound_Release(music_coverGame3);
					}
					FMOD_RESULT resc;
#ifdef UNICODE
					char chAnsiFile[260];
					UnicodeToAnsi(chFile, chAnsiFile);
					resc = FMOD_System_CreateStream(sys, chAnsiFile, FMOD_HARDWARE | FMOD_2D, 0, &music_coverGame3);
#else
					resc = FMOD_System_CreateStream(sys, chFile, FMOD_HARDWARE | FMOD_2D, 0, &music_coverGame3);
#endif
					if(ERRCHECK(resc))
					{
						bLoadMusicCoverGame3 = true;
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, music_coverGame3, 0, &channels[9]);
					}
					else
					{
						bLoadMusicCoverGame3 = false;
						_printf(_T("No se puede cargar el archivo \"%ls\"\n"), chFile);
					}
					strDispGame3.Format(_T("%ls - %ls - %ls"), GetNameSongsGame3(nCurSongGame3), GetArtistSongsGame3(nCurSongGame3), GetBPMSongsGame3(nCurSongGame3));
					for(int k = 0; k < 2; k++) if(np[k])
					{
						GetLevelSongsGame3(nCurSongGame3, nModeCurSongGame3[k], &nLevelCurGame3[k],
						&dDCurGame3[k][0],
						&dDCurGame3[k][1],
						&dDCurGame3[k][2],
						&dDCurGame3[k][3],
						&dDCurGame3[k][4]);
					}
					GetBestScoresGame3(nCurSongGame3, nModeCurSongGame3[np[0]?0:1]);
				}
			}



	#define TIME_START_UPDO_GAME3 0
	#define TIME_TRANSI_UPDO_GAME3 500
	#define TIME_GLOUTH_UPDO_GAME3 500
			if(nCountGame3 >= TIME_START_UPDO_GAME3 && nCountGame3 < (TIME_START_UPDO_GAME3+TIME_TRANSI_UPDO_GAME3))	// Transi Up and Down Game3
			{
				dApt = (double(nCountGame3)-TIME_START_UPDO_GAME3)/(TIME_TRANSI_UPDO_GAME3);
				glPushMatrix();
				glScaled(1.0+0.8*(1.0-dApt), 1.0+0.8*(1.0-dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[41+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[23+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[3]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[4]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[5]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[6]->texture[0]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625, -29.75, -50.0);
				glEnd();
				/*Dibujo de los modos*/
				for(int k = 0; k < 5; k++)
				{
					double dx = double(k)*3.725-7.45;
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[(IsAvalibleModeGame3(nCurSongGame3, k)?89:94)+k]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d( -1.8625+dx, 24.25+1.396875, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  1.8625+dx, 24.25+1.396875, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  1.8625+dx, 24.25-1.396875, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -1.8625+dx, 24.25-1.396875, -50.0);
					glEnd();
				}
				/*Dibujo de la elección*/
				for(int k = 0; k < 2; k++) if(np[k])
				{
					double dx = double(nModeCurSongGame3[k])*3.725-7.45+(k?-0.465625:0.465625);
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[+k]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d( -1.396875+dx, 26.69453125+1.04765625, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  1.396875+dx, 26.69453125+1.04765625, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  1.396875+dx, 26.69453125-1.04765625, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -1.396875+dx, 26.69453125-1.04765625, -50.0);
					glEnd();
				}
				glPopMatrix();
			}
			if(nCountGame3 >= (TIME_START_UPDO_GAME3+TIME_TRANSI_UPDO_GAME3))	// Up and Down Game3
			{
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[41+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				if(nTransBorder > TIME_SHOW_PREVIEW_GAME3 && bShow)
				{
	#define SIZE_TEXT_GAME3 1.0
	#define VEL_SHOWTEXT_GAME3 0.01
	#define TIME_WAITTEXTNAME_GAME3 1500
					__int64 nTimeName = nTransBorder-TIME_SHOW_PREVIEW_GAME3-TIME_WAITTEXTNAME_GAME3;
					if(nTimeName < 0) nTimeName = 0;
					double dSizeTex = font_Car->CalcSingleLine(strDispGame3, SIZE_TEXT_GAME3);
					DWORD t1 = DWORD(double(18.75/VEL_SHOWTEXT_GAME3));
					DWORD t2 = DWORD(double(dSizeTex/VEL_SHOWTEXT_GAME3));
					DWORD t = t1+t2;
					DWORD nShowText = (DWORD(nTimeName)+t1)%t;
					double dx = -VEL_SHOWTEXT_GAME3*double(nShowText)+18.75;
					font_Car->glDrawText(-22.25, -9.375, -23.75, 9.375, -8.375+dx, -23, -50.0, 1.0, SIZE_TEXT_GAME3, strDispGame3, DT_VCENTER);
				}
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[23+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();

				DWORD dwNext = 0;
				DWORD dwFrame = 0;

				dwNext = (DWORD)(nCountGame3-max(nTimeEnArrowsGame3[1],nTimeEnArrowsGame3[6]));
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame3)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga3res->skin[3]->m_nSize)/(DWORD)nTimeTransiSongBorderGame3;
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[3]->texture[dwFrame]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame3-max(nTimeEnArrowsGame3[3],nTimeEnArrowsGame3[8]));
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame3)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga3res->skin[4]->m_nSize)/(DWORD)nTimeTransiSongBorderGame3;
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[4]->texture[dwFrame]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame3-max(nTimeEnArrowsGame3[0],nTimeEnArrowsGame3[5]));
				if(bEnArrowsGame3[0] || bEnArrowsGame3[5]) dwNext = dwNext%nTimeTransiSongBorderGame3;
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame3)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga3res->skin[5]->m_nSize)/(DWORD)nTimeTransiSongBorderGame3;
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[5]->texture[dwFrame]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
				glEnd();

				dwNext = (DWORD)(nCountGame3-max(nTimeEnArrowsGame3[4],nTimeEnArrowsGame3[9]));
				if(bEnArrowsGame3[4] || bEnArrowsGame3[9]) dwNext = dwNext%nTimeTransiSongBorderGame3;
				if(dwNext>=(DWORD)(nTimeTransiSongBorderGame3)) dwFrame = 0;
				else dwFrame = (dwNext*(DWORD)ga3res->skin[6]->m_nSize)/(DWORD)nTimeTransiSongBorderGame3;
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[6]->texture[dwFrame]);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625, -29.75, -50.0);
				glEnd();

				/*Dibujo de los modos*/
				for(int k = 0; k < 5; k++)
				{
					double dx = double(k)*3.725-7.45;
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[(IsAvalibleModeGame3(nCurSongGame3, k)?89:94)+k]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d( -1.8625+dx, 24.25+1.396875, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  1.8625+dx, 24.25+1.396875, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  1.8625+dx, 24.25-1.396875, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -1.8625+dx, 24.25-1.396875, -50.0);
					glEnd();
				}

	#define TIME_TRANSI_SELECTORMODE_GAME3 300
	#define TIME_START_GRAPH_GAME3 100
	#define TIME_TRANSI_GRAPH_GAME3 500
	#define TIME_GLOUTH_GRAPH_GAME3 300
	#define TIME_START_LINEGRAPH_GAME3 100
	#define TIME_TRANSI_LINEGRAPH_GAME3 500
	#define TIME_TRUNC_LINEGRAPH_GAME3 1200
				/*Dibujo de la elección*/
				for(int k = 0; k < 2; k++) if(np[k])
				{
					double dx = double(nModeCurSongGame3[k])*3.725-7.45+(k?0.465625:-0.465625);
					double dy = abs(double((nCountGame3-(TIME_START_UPDO_GAME3+TIME_TRANSI_UPDO_GAME3)+(k?250:0))%500)/500-0.5)*2;
					__int64 nTransSelectM = nCountGame3 - nTimeModeCurSongGame3[k];
					if(nTransSelectM >= 0 && nTransSelectM < TIME_TRANSI_SELECTORMODE_GAME3)
					{
						dx+=(double(nBackModeCurSongGame3[k])-double(nModeCurSongGame3[k]))*3.725*(1.0-pow(double(nTransSelectM)/TIME_TRANSI_SELECTORMODE_GAME3,0.5));
					}
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[99+k]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d( -1.396875+dx, 26.69453125+1.04765625+dy, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d(  1.396875+dx, 26.69453125+1.04765625+dy, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d(  1.396875+dx, 26.69453125-1.04765625+dy, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d( -1.396875+dx, 26.69453125-1.04765625+dy, -50.0);
					glEnd();

#define TIME_START_LR_GAME3 0
#define TIME_TRANSI_LR_GAME3 500
#define TIME_GLOUTH_LR_GAME3 500
					__int64 nCountSide = nCountGame3-(TIME_START_UPDO_GAME3+TIME_TRANSI_UPDO_GAME3);
					if(nCountSide >= TIME_START_LR_GAME3 && nCountSide <= (TIME_START_LR_GAME3+TIME_TRANSI_LR_GAME3))
					{
						dApt = pow(double(nCountSide-TIME_START_LR_GAME3)/TIME_TRANSI_LR_GAME3, 0.5);
						glPushMatrix();
						glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(1-dApt), 0.0, 0.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBindTexture(GL_TEXTURE_2D, ga3res->skin[(k?68:50)+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
#define TIME_TRANSI_PRESET_GAME3 500
#define TIME_GLOUTH_PRESET_GAME3 500
#define TIME_SHOW_PRESET_GAME3 1500
					if(nCountSide > (TIME_START_LR_GAME3+TIME_TRANSI_LR_GAME3))
					{
						glPushMatrix();
						glTranslated(k?40.0:-40.0, 0.0, 0.0);
						glBindTexture(GL_TEXTURE_2D, ga3res->skin[(k?68:50)+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  8.0,  30.0, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  8.0, -30.0, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -8.0, -30.0, -50.0);
						glEnd();
						glPopMatrix();
						__int64 nCountPreset = nCountGame3-nTimeLastPresetGame3[k];
						for(int q = 0; q < 6; q++)
						{
							double dAdv = 0.0;
							for(int w = 0; w < nIndSizePresetGame3[k][q]; w++)
							{
								int nInd = (q*5+w);
								if(nInd == nIndLastPresetGame3[k] || nIndLastPresetGame3[k] == -1)
								{
									__int64 nCountPreset = nCountGame3-nTimeLastPresetGame3[k];
									if(nCountPreset >= 0 && nCountPreset < TIME_TRANSI_PRESET_GAME3)
									{
										glPushMatrix();
										dApt = double(nCountPreset)/TIME_TRANSI_PRESET_GAME3;
										glLoadIdentity();
										glColor4d(1.0, 1.0, 1.0, dApt);
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
										glBindTexture(GL_TEXTURE_2D, ga3res->skin[108+nIndPresetGame3[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
										glScaled(1.0+2.0*(1-dApt), 1.0, 1.0);
										glBindTexture(GL_TEXTURE_2D, ga3res->skin[108+nIndPresetGame3[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glScaled(1.0+2.0*(1-dApt), 1.0+2.0*(1-dApt), 1.0);
										glBindTexture(GL_TEXTURE_2D, ga3res->skin[106]->GetCurrentTex());
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
										glBindTexture(GL_TEXTURE_2D, ga3res->skin[104]->GetCurrentTex());
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
									if(nCountPreset >= TIME_TRANSI_PRESET_GAME3 && nCountPreset < (TIME_TRANSI_PRESET_GAME3+TIME_SHOW_PRESET_GAME3))
									{
										glPushMatrix();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga3res->skin[108+nIndPresetGame3[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga3res->skin[108+nIndPresetGame3[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(1.0/4, 1.0); glVertex3d( -4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  4.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  4.5, -1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d( -4.5, -1.5, -50.0);
										glEnd();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga3res->skin[106]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										if(nCountPreset < (TIME_TRANSI_PRESET_GAME3+TIME_GLOUTH_PRESET_GAME3))
										{
											dApt = double(nCountPreset - TIME_TRANSI_PRESET_GAME3)/TIME_GLOUTH_PRESET_GAME3;
											glLoadIdentity();
											glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
											glScaled(dApt*4.0, dApt*4.0, 1.0);
											glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
											glBindTexture(GL_TEXTURE_2D, ga3res->skin[103]->GetCurrentTex());
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
									if(nCountPreset >= (TIME_TRANSI_PRESET_GAME3+TIME_SHOW_PRESET_GAME3))
									{
										glPushMatrix();
										glLoadIdentity();
										glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
										glBindTexture(GL_TEXTURE_2D, ga3res->skin[108+nIndPresetGame3[k][nInd]]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME3+TIME_SHOW_PRESET_GAME3))
										{
											dApt = double(nCountPreset-(TIME_TRANSI_PRESET_GAME3+TIME_SHOW_PRESET_GAME3))/TIME_TRANSI_PRESET_GAME3;
											glLoadIdentity();
											glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv+(k?-4.5:4.5), -double(q)*4.5+11.25, 0.0);
											glScaled(1.0+2.0*(dApt), 1.0, 1.0);
											glColor4d(1.0, 1.0, 1.0, 1-dApt);
											glBindTexture(GL_TEXTURE_2D, ga3res->skin[108+nIndPresetGame3[k][nInd]]->GetCurrentTex());
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
										glBindTexture(GL_TEXTURE_2D, ga3res->skin[106]->GetCurrentTex());
										glBegin(GL_QUADS);
										glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 1.0); glVertex3d(  1.5,  1.5, -50.0);
										glTexCoord2d(1.0, 0.0); glVertex3d(  1.5, -1.5, -50.0);
										glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
										glEnd();
										glPopMatrix();
										if(nCountPreset < (2*TIME_TRANSI_PRESET_GAME3+TIME_SHOW_PRESET_GAME3)) dAdv += (1-dApt)*9.0;
										dAdv += 4.5;
									}
								}
								else
								{
									glPushMatrix();
									glLoadIdentity();
									glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga3res->skin[108+nIndPresetGame3[k][nInd]]->GetCurrentTex());
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d( -1.5,  1.5, -50.0);
									glTexCoord2d(1.0/4, 1.0); glVertex3d(  1.5,  1.5, -50.0);
									glTexCoord2d(1.0/4, 0.0); glVertex3d(  1.5, -1.5, -50.0);
									glTexCoord2d(0.0, 0.0); glVertex3d( -1.5, -1.5, -50.0);
									glEnd();
									glLoadIdentity();
									glTranslated((k?37.0:-37.0)+(k?-1:1)*dAdv, -double(q)*4.5+11.25, 0.0);
									glBindTexture(GL_TEXTURE_2D, ga3res->skin[106]->GetCurrentTex());
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

#define TIME_TRANSI_LIFEALTER_GAME3 500
					int nMinus = 2;
					switch(nChaSongsGame3[nCurSongGame3])
					{
					case 0:
						nMinus = 8;
						break;
					case 1:
						nMinus = 7;
						break;
					case 2:
						nMinus = 6;
						break;
					case 3:
						nMinus = 5;
						break;
					case 4:
						nMinus = 4;
						break;
					case 5:
						nMinus = 3;
						break;
					case 7:
						nMinus = 1;
						break;
					}
					__int64 nTimeLife = nCountGame3;
					int nLife = np[k];
					int nLifeG = np[k]-nMinus;
					if(nLifeG < 0)
					{
						nLife = 0;
						nLifeG = 0;
					}
					glPushMatrix();
					glTranslated(k?30.0:-30.0, k?-28.0:28.0, 0.0);
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[173+nLifeG]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-4.1666,  0.5, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 4.1666,  0.5, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 4.1666, -0.5, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-4.1666, -0.5, -50.0);
					glEnd();
					dApt = abs(double((nCountGame3)%TIME_TRANSI_LIFEALTER_GAME3)/TIME_TRANSI_LIFEALTER_GAME3-0.5)*2;
					glColor4d(1.0, 1.0, 1.0, dApt);
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[173+nLife]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-4.1666,  0.5, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 4.1666,  0.5, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 4.1666, -0.5, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-4.1666, -0.5, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();

					/*Dibujo del Graph*/

					if(nTransSelectM >= TIME_START_GRAPH_GAME3 && nTransSelectM < (TIME_START_GRAPH_GAME3+TIME_TRANSI_GRAPH_GAME3))
					{
						dApt = pow(double(nTransSelectM - TIME_START_GRAPH_GAME3)/TIME_TRANSI_GRAPH_GAME3, 0.5);
						glPushMatrix();
						glTranslated((k?30.0:-30.0)+(k?1:-1)*17.5*(1-dApt), k?-20.0:20.0, 0.0);
						glScaled(0.5+0.5*dApt, 0.5+0.5*dApt, 1.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						for(int l = 0; l < 6; l++)
						{
							double da = 0.0, de = 0.0;
							if(l == 0) da = dDCurGame3[k][0];
							else da = dDCurGame3[k][l-1];
							if(l == 5) de = dDCurGame3[k][4];
							else de = dDCurGame3[k][l];
							double dx = 0.46875+2.34375*double(l)-7.5;
							double dy = -3.75;
							double dy1 = 9.375*da-3.75;
							double dy2 = 9.375*de-3.75;
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[10]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(double(l)/6, 1.0-da); glVertex3d( dx,  dy1, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0-de); glVertex3d(  dx+2.34375,  dy2, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0); glVertex3d(  dx+2.34375, dy, -50.0);
							glTexCoord2d(double(l)/6, 1.0); glVertex3d( dx, dy, -50.0);
							glEnd();
						}
						glBindTexture(GL_TEXTURE_2D, ga3res->skin[7]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  7.5, -7.5, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -7.5, -7.5, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nTransSelectM >= (TIME_START_GRAPH_GAME3+TIME_TRANSI_GRAPH_GAME3))
					{
						glPushMatrix();
						glTranslated(k?30.0:-30.0, k?-20.0:20.0, 0.0);
						for(int l = 0; l < 6; l++)
						{
							double da = 0.0, de = 0.0;
							if(l == 0) da = dDCurGame3[k][0];
							else da = dDCurGame3[k][l-1];
							if(l == 5) de = dDCurGame3[k][4];
							else de = dDCurGame3[k][l];
							double dx = 0.46875+2.34375*double(l)-7.5;
							double dy = -3.75;
							double dy1 = 9.375*da-3.75;
							double dy2 = 9.375*de-3.75;
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[10]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(double(l)/6, 1.0-da); glVertex3d( dx,  dy1, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0-de); glVertex3d(  dx+2.34375,  dy2, -50.0);
							glTexCoord2d(double(l+1)/6, 1.0); glVertex3d(  dx+2.34375, dy, -50.0);
							glTexCoord2d(double(l)/6, 1.0); glVertex3d( dx, dy, -50.0);
							glEnd();
						}
						glBindTexture(GL_TEXTURE_2D, ga3res->skin[7]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  7.5,  7.5, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  7.5, -7.5, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -7.5, -7.5, -50.0);
						glEnd();
						__int64 nTransGraphLine = (nTransSelectM - (TIME_START_GRAPH_GAME3+TIME_TRANSI_GRAPH_GAME3))%TIME_TRUNC_LINEGRAPH_GAME3;
						if(nTransGraphLine >= TIME_START_LINEGRAPH_GAME3 && nTransGraphLine < (TIME_START_LINEGRAPH_GAME3+TIME_TRANSI_LINEGRAPH_GAME3))
						{
							double t = double(nTransGraphLine)-TIME_START_LINEGRAPH_GAME3;
							double dx=0.46875+14.0625*t/TIME_TRANSI_LINEGRAPH_GAME3-7.5;
							double dy=0.0;
							double da=0.0;
							if(t<(TIME_TRANSI_LINEGRAPH_GAME3/6))
							{
								da=t*6/TIME_TRANSI_LINEGRAPH_GAME3;
								dy=9.375*dDCurGame3[k][0]-3.75;
							}
							if(t>=(TIME_TRANSI_LINEGRAPH_GAME3/6) && t < (5*TIME_TRANSI_LINEGRAPH_GAME3/6))
							{
								da = 1.0;
								int n1 = int((t-TIME_TRANSI_LINEGRAPH_GAME3/6)*6/TIME_TRANSI_LINEGRAPH_GAME3);
								int n2 = int((t-TIME_TRANSI_LINEGRAPH_GAME3/6)*6/TIME_TRANSI_LINEGRAPH_GAME3)+1;
								double tl = double(n2)*TIME_TRANSI_LINEGRAPH_GAME3/6;
								dy = 9.375*((t-tl)*6/TIME_TRANSI_LINEGRAPH_GAME3*(dDCurGame3[k][n2]-dDCurGame3[k][n1])+dDCurGame3[k][n1])-3.75;
							}
							if(t>(5*TIME_TRANSI_LINEGRAPH_GAME3/6))
							{
								da=1.0-(t-5*TIME_TRANSI_LINEGRAPH_GAME3/6)*6/TIME_TRANSI_LINEGRAPH_GAME3;
								dy=9.375*dDCurGame3[k][4]-3.75;
							}
							glTranslated(dx, dy, 0.0);
							glColor4d(1.0, 1.0, 1.0, da);
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[9]->GetCurrentTex());
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d( -1.0,  1.0, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d(  1.0,  1.0, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d(  1.0, -1.0, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d( -1.0, -1.0, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
						glPopMatrix();
						if(nTransSelectM < (TIME_START_GRAPH_GAME3+TIME_TRANSI_GRAPH_GAME3+TIME_GLOUTH_GRAPH_GAME3))
						{
							dApt = pow(double(nTransSelectM-TIME_START_GRAPH_GAME3-TIME_TRANSI_GRAPH_GAME3)/TIME_GLOUTH_GRAPH_GAME3, 0.5);
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[8]->GetCurrentTex());
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

	#define TIME_START_DIFI_GAME3 600
	#define TIME_TRANSI_DIFI_GAME3 500
	#define TIME_GLOUTH_DIFI_GAME3 500
	#define TIME_CONTROT_DIFI_GAME3 1000
					/*Dibujo del nivel*/
					int nFilterLevel = 0;
					int nItemsActive = 0;
					int nIndexActive = 0;
					if(nLevelCurGame3[k] <= 0 || nLevelCurGame3[k] > ga3res->skin[101]->m_nSize) nFilterLevel = ga3res->skin[101]->m_nSize;
					else nFilterLevel = nLevelCurGame3[k];
					if(nFilterLevel >= 1 && nFilterLevel <= 30)
					{
						nItemsActive = (nFilterLevel-1)%8+1;
						nIndexActive = (nFilterLevel-1)/8;
					}
					if(player_sesion[k]->nLevel <= (nLevelCurGame3[k]-3) && !player_sesion[k]->nLockedAsp[ITEM_SUPRIMELVL]) nIndexActive = 4;
					double dAngleDif = double(nTransSelectM%TIME_CONTROT_DIFI_GAME3)/TIME_CONTROT_DIFI_GAME3*360;
					if(nTransSelectM >= TIME_START_DIFI_GAME3 && nTransSelectM < (TIME_START_DIFI_GAME3+TIME_TRANSI_DIFI_GAME3))
					{
						dApt = double(nTransSelectM - TIME_START_DIFI_GAME3)/TIME_TRANSI_DIFI_GAME3;
						glPushMatrix();
						if(np[k] && !np[int(!k)])
						{
							double dWidth = 18.75;
							double dSizeItem = dWidth/8;
							double dStart = -dSizeItem*4+dSizeItem/2;
							glLoadIdentity();
							glTranslated(0.0, -26.375, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[102]->GetCurrentTex());
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
									glBindTexture(GL_TEXTURE_2D, ga3res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[107]->GetCurrentTex());
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
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[108]->GetCurrentTex());
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
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[102]->GetCurrentTex());
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
									glBindTexture(GL_TEXTURE_2D, ga3res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[107]->GetCurrentTex());
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
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[108]->GetCurrentTex());
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
						glBindTexture(GL_TEXTURE_2D, ga3res->skin[101]->texture[nFilterLevel-1]);
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
						glBindTexture(GL_TEXTURE_2D, ga3res->skin[106]->GetCurrentTex());
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
						glBindTexture(GL_TEXTURE_2D, ga3res->skin[104]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nTransSelectM >= (TIME_START_DIFI_GAME3+TIME_TRANSI_DIFI_GAME3))
					{
						glPushMatrix();
						if(np[k] && !np[int(!k)])
						{
							double dWidth = 18.75;
							double dSizeItem = dWidth/8;
							double dStart = -dSizeItem*4+dSizeItem/2;
							glLoadIdentity();
							glTranslated(0.0, -26.375, 0.0);
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[102]->GetCurrentTex());
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
									glBindTexture(GL_TEXTURE_2D, ga3res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[107]->GetCurrentTex());
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
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[108]->GetCurrentTex());
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
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[102]->GetCurrentTex());
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
									glBindTexture(GL_TEXTURE_2D, ga3res->skin[(q>=nItemsActive?86:0)+nIndexActive]->GetCurrentTex());
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
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[107]->GetCurrentTex());
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
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[108]->GetCurrentTex());
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
						glBindTexture(GL_TEXTURE_2D, ga3res->skin[101]->texture[nFilterLevel-1]);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  0.875,  0.875, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  0.875, -0.875, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -0.875, -0.875, -50.0);
						glEnd();
						glLoadIdentity();
						glTranslated(k?10.875:-10.875, -26.375, 0.0);
						glRotated(dAngleDif, 0.0, 0.0, 1.0);
						glBindTexture(GL_TEXTURE_2D, ga3res->skin[106]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d( -1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d(  1.5625,  1.5625, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d(  1.5625, -1.5625, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d( -1.5625, -1.5625, -50.0);
						glEnd();
						if(nTransSelectM < (TIME_START_DIFI_GAME3+TIME_TRANSI_DIFI_GAME3+TIME_GLOUTH_DIFI_GAME3))
						{
							dApt = double(nTransSelectM - TIME_START_DIFI_GAME3 -TIME_TRANSI_DIFI_GAME3)/TIME_GLOUTH_DIFI_GAME3;
							glLoadIdentity();
							glTranslated(k?10.875:-10.875, -26.375, 0.0);
							glScaled(dApt*4.0, dApt*4.0, 1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[103]->GetCurrentTex());
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
					if(bSkinGame3[k]) DrawSkinsGame3(k);
					if(bCharacterGame3[k]) DrawCharactersGame3(k);
					glPopMatrix();
				}

				__int64 nTrans1 = nCountGame3 - nTimeChaGame3;
				if(nTrans1 <= TIME_GLOUTH_UPDO_GAME3)
				{
					dApt = (double(nTrans1))/(TIME_GLOUTH_UPDO_GAME3);
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[32+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
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
				strDispTime.Format(_T("%.2d"), piuconfig.nModeMach==1?99:int((nMaxGame3 - nCountGame3)/1000));
				font_Micro->glDrawText(30.0, -40.0, -30.0, 40.0, 0, 17.75, -50.0, 1.0, 1.5, strDispTime, DT_VCENTER|DT_CENTER);
			}
			if(nCountGame3 >= TIME_START_START_GAME3)	// Reconocimiento de comandos
			{
				bool bNewCommand = false;
				static bool bCh[] = {false, false, false, false, false,
									false, false, false, false, false};
				for(int x = 0; x < 2; x++)
				{
					if(np[x] > 0)
					{
						if(x == 0)
						{
							for(int y = 0; y < 5; y++)
							{
								if(bArrows[y] && !bEnArrowsGame3[y])
								{
									bEnArrowsGame3[y] = true;
									nTimeEnArrowsGame3[y] = nCountGame3;
									for(int q = (MAX_COMMANDS_GAME3-2); q >= 0; q--)
									{
										nComandGame3[x][q+1] = nComandGame3[x][q];
									}
									if(!bSkinGame3[x] && !bCharacterGame3[x])
									{
										nComandGame3[x][0] = y;
										bNewCommand = true;
									}
								}
								if(!bArrows[y] && bEnArrowsGame3[y])
								{
									bEnArrowsGame3[y] = false;
									bCh[y] = false;
									if(y == 0 || y == 4)
									{
										nTimeTransiSongBorderGame3 = TIME_TRANSI_SONGBORDER_GAME3;
									}
								}
							}
						}
						if(x == 1)
						{
							for(int y = 5; y < 10; y++)
							{
								if(bArrows[y] && !bEnArrowsGame3[y])
								{
									bEnArrowsGame3[y] = true;
									nTimeEnArrowsGame3[y] = nCountGame3;
									for(int q = (MAX_COMMANDS_GAME3-2); q >= 0; q--)
									{
										nComandGame3[x][q+1] = nComandGame3[x][q];
									}
									if(!bSkinGame3[x] && !bCharacterGame3[x])
									{
										nComandGame3[x][0] = y-5;
										bNewCommand = true;
									}
								}
								if(!bArrows[y] && bEnArrowsGame3[y])
								{
									bEnArrowsGame3[y] = false;
									bCh[y] = false;
									if(y == 5 || y == 9)
									{
										nTimeTransiSongBorderGame3 = TIME_TRANSI_SONGBORDER_GAME3;
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
							nChangeRetroGame3 = 2;
							nTimeChangeRetroGame3 = nCountGame3;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
							for(int y = 0; y < 10; y++)
							{
								bArrows[y] = false;
								bEnArrowsGame3[y] = false;
								bCh[y] = false;
							}
							break;
						}
						if(x == 1 && bArrows[7] && nCredit)
						{
							PlusStagePlayer(1);
							nCredit--;
							nChangeRetroGame3 = 2;
							nTimeChangeRetroGame3 = nCountGame3;
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
							for(int y = 0; y < 10; y++)
							{
								bArrows[y] = false;
								bEnArrowsGame3[y] = false;
								bCh[y] = false;
							}
							break;
						}
					}
				}
				if(bEnArrowsGame3[0])
				{
					if(bSkinGame3[0] && !bCh[0])
					{
						TurnSkinsGame3(true, 0);
						bCh[0] = true;
					}
					else if(bCharacterGame3[0] && !bCh[0])
					{
						TurnCharactersGame3(true, 0);
						bCh[0] = true;
					}
					else
					{
						__int64 nCaret = nCountGame3 - nTimeEnArrowsGame3[0];
						int nLimit = min(nCaret,nCountGame3-nTimeChaGame3)>TIME_FAST_CHANGE_GAME3?TIME_CHANGE_HIGH_GAME3:TIME_CHANGE_LOW_GAME3;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[0])
						{
							bCh[0] = true;
							MoveSongsGame3(true);
							PlaySoundTransiAboutLimitGame3(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[0] = false;
						}
					}
				}
				if(bEnArrowsGame3[4])
				{
					if(bSkinGame3[0] && !bCh[4])
					{
						TurnSkinsGame3(false, 0);
						bCh[4] = true;
					}
					else if(bCharacterGame3[0] && !bCh[4])
					{
						TurnCharactersGame3(false, 0);
						bCh[4] = true;
					}
					else
					{
						__int64 nCaret = nCountGame3 - nTimeEnArrowsGame3[4];
						int nLimit = min(nCaret,nCountGame3-nTimeChaGame3)>TIME_FAST_CHANGE_GAME3?TIME_CHANGE_HIGH_GAME3:TIME_CHANGE_LOW_GAME3;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[4])
						{
							bCh[4] = true;
							MoveSongsGame3(false);
							PlaySoundTransiAboutLimitGame3(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[4] = false;
						}
					}
				}
				if(bEnArrowsGame3[1] && !bCh[1])
				{
					if(!bSkinGame3[0] && !bCharacterGame3[0])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame3 = 0;
						nTimeModeCurSongGame3[0] = nCountGame3;
						nBackModeCurSongGame3[0] = nModeCurSongGame3[0];
						do
						{
							nModeCurSongGame3[0]--;
							if(nModeCurSongGame3[0] < 0) nModeCurSongGame3[0] = 4;
						} while(!IsAvalibleModeGame3(nCurSongGame3, nModeCurSongGame3[0]));
						nPrefModeCurSongGame3[0] = nModeCurSongGame3[0];
						bCh[1] = true;
						GetLevelSongsGame3(nCurSongGame3, nModeCurSongGame3[0], &nLevelCurGame3[0],
						&dDCurGame3[0][0],
						&dDCurGame3[0][1],
						&dDCurGame3[0][2],
						&dDCurGame3[0][3],
						&dDCurGame3[0][4]);
						GetBestScoresGame3(nCurSongGame3, nModeCurSongGame3[0]);
					}
				}
				if(bEnArrowsGame3[3] && !bCh[3])
				{
					if(!bSkinGame3[0] && !bCharacterGame3[0])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame3 = 0;
						nTimeModeCurSongGame3[0] = nCountGame3;
						nBackModeCurSongGame3[0] = nModeCurSongGame3[0];
						do
						{
							nModeCurSongGame3[0]++;
							if(nModeCurSongGame3[0] > 4) nModeCurSongGame3[0] = 0;
						} while(!IsAvalibleModeGame3(nCurSongGame3, nModeCurSongGame3[0]));
						nPrefModeCurSongGame3[0] = nModeCurSongGame3[0];
						bCh[3] = true;
						GetLevelSongsGame3(nCurSongGame3, nModeCurSongGame3[0], &nLevelCurGame3[0],
						&dDCurGame3[0][0],
						&dDCurGame3[0][1],
						&dDCurGame3[0][2],
						&dDCurGame3[0][3],
						&dDCurGame3[0][4]);
						GetBestScoresGame3(nCurSongGame3, nModeCurSongGame3[0]);
					}
				}
				if(bEnArrowsGame3[2] && !bCh[2])
				{
					bCh[2] = true;
					if(np[0])
					{
						if(bSkinGame3[0])
						{
							MakeChoiceSkinGame3(0);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else if(bCharacterGame3[0])
						{
							MakeChoiceCharacterGame3(0);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else
						{
							nChosesGame3++;
							nTimeChosesGame3[nChosesGame3-1] = nCountGame3;
							if(nChosesGame3 > 0 && nChosesGame3 < CHOOSES_GAME3) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
							if(nChosesGame3 >= CHOOSES_GAME3)
							{
								FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
								for(int x = 0; x < 10; x++) bCh[x] = false;
							}
						}
					}
				}
#define TIME_TOL_SWITCHRETRO_GAME3 30
				if((_abs64(nTimeEnArrowsGame3[0] - nTimeEnArrowsGame3[4]) < TIME_TOL_SWITCHRETRO_GAME3 && (bEnArrowsGame3[0] && bEnArrowsGame3[4])) ||
					(_abs64(nTimeEnArrowsGame3[5] - nTimeEnArrowsGame3[9]) < TIME_TOL_SWITCHRETRO_GAME3 && (bEnArrowsGame3[5] && bEnArrowsGame3[9])) && (!np[0]||np[0]!=1) && (!np[1]||np[1]!=1))
				{
					nChangeRetroGame3 = 1;
					nTimeChangeRetroGame3 = nCountGame3;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int y = 0; y < 10; y++)
					{
						bArrows[y] = false;
						bEnArrowsGame3[y] = false;
						bCh[y] = false;
					}
				}
#define TIME_TOL_SWITCHUNLOCKER_GAME3 30
				if(((_abs64(nTimeEnArrowsGame3[1] - nTimeEnArrowsGame3[3]) < TIME_TOL_SWITCHUNLOCKER_GAME3 && (bEnArrowsGame3[1] && bEnArrowsGame3[3])) ||
					(_abs64(nTimeEnArrowsGame3[6] - nTimeEnArrowsGame3[8]) < TIME_TOL_SWITCHUNLOCKER_GAME3 && (bEnArrowsGame3[6] && bEnArrowsGame3[8]))) && bIsAvalibleUnlockerGame3)
				{
					nChangeRetroGame3 = 3;
					nTimeChangeRetroGame3 = nCountGame3;
					bIsAvalibleUnlockerGame3 = false;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int y = 0; y < 10; y++)
					{
						bArrows[y] = false;
						bEnArrowsGame3[y] = false;
						bCh[y] = false;
					}
				}

				// 2
				if(bEnArrowsGame3[5])
				{
					if(bSkinGame3[1] && !bCh[5])
					{
						TurnSkinsGame3(true, 1);
						bCh[5] = true;
					}
					else if(bCharacterGame3[1] && !bCh[5])
					{
						TurnCharactersGame3(true, 1);
						bCh[5] = true;
					}
					else
					{
						__int64 nCaret = nCountGame3 - nTimeEnArrowsGame3[5];
						int nLimit = min(nCaret,nCountGame3-nTimeChaGame3)>TIME_FAST_CHANGE_GAME3?TIME_CHANGE_HIGH_GAME3:TIME_CHANGE_LOW_GAME3;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[5])
						{
							bCh[5] = true;
							MoveSongsGame3(true);
							PlaySoundTransiAboutLimitGame3(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[5] = false;
						}
					}
				}
				if(bEnArrowsGame3[9])
				{
					if(bSkinGame3[1] && !bCh[9])
					{
						TurnSkinsGame3(false, 1);
						bCh[9] = true;
					}
					else if(bCharacterGame3[1] && !bCh[9])
					{
						TurnCharactersGame3(false, 1);
						bCh[9] = true;
					}
					else
					{
						__int64 nCaret = nCountGame3 - nTimeEnArrowsGame3[9];
						int nLimit = min(nCaret,nCountGame3-nTimeChaGame3)>TIME_FAST_CHANGE_GAME3?TIME_CHANGE_HIGH_GAME3:TIME_CHANGE_LOW_GAME3;
						nCaret = nCaret % nLimit;
						if(nCaret < (nLimit/2) && !bCh[9])
						{
							bCh[9] = true;
							MoveSongsGame3(false);
							PlaySoundTransiAboutLimitGame3(nLimit);
							bShow = false;
							FMOD_Channel_Stop(channels[9]);
						}
						if(nCaret >= (nLimit/2))
						{
							bCh[9] = false;
						}
					}
				}
				if(bEnArrowsGame3[6] && !bCh[6])
				{
					if(!bSkinGame3[1] && !bCharacterGame3[1])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame3 = 0;
						nTimeModeCurSongGame3[1] = nCountGame3;
						nBackModeCurSongGame3[1] = nModeCurSongGame3[1];
						do
						{
							nModeCurSongGame3[1]--;
							if(nModeCurSongGame3[1] < 0) nModeCurSongGame3[1] = 4;
						} while(!IsAvalibleModeGame3(nCurSongGame3, nModeCurSongGame3[1]));
						nPrefModeCurSongGame3[1] = nModeCurSongGame3[1];
						bCh[6] = true;
						GetLevelSongsGame3(nCurSongGame3, nModeCurSongGame3[1], &nLevelCurGame3[1],
						&dDCurGame3[1][0],
						&dDCurGame3[1][1],
						&dDCurGame3[1][2],
						&dDCurGame3[1][3],
						&dDCurGame3[1][4]);
						GetBestScoresGame3(nCurSongGame3, nModeCurSongGame3[1]);
					}
				}
				if(bEnArrowsGame3[8] && !bCh[8])
				{
					if(!bSkinGame3[1] && !bCharacterGame3[1])
					{
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)7, sound_switch, 0, &channels[7]);
						nChosesGame3 = 0;
						nTimeModeCurSongGame3[1] = nCountGame3;
						nBackModeCurSongGame3[1] = nModeCurSongGame3[1];
						do
						{
							nModeCurSongGame3[1]++;
							if(nModeCurSongGame3[1] > 4) nModeCurSongGame3[1] = 0;
						} while(!IsAvalibleModeGame3(nCurSongGame3, nModeCurSongGame3[1]));
						nPrefModeCurSongGame3[1] = nModeCurSongGame3[1];
						bCh[8] = true;
						GetLevelSongsGame3(nCurSongGame3, nModeCurSongGame3[1], &nLevelCurGame3[1],
						&dDCurGame3[1][0],
						&dDCurGame3[1][1],
						&dDCurGame3[1][2],
						&dDCurGame3[1][3],
						&dDCurGame3[1][4]);
						GetBestScoresGame3(nCurSongGame3, nModeCurSongGame3[1]);
					}
				}
				if(bEnArrowsGame3[7] && !bCh[7])
				{
					bCh[7] = true;
					if(np[1])
					{
						if(bSkinGame3[1])
						{
							MakeChoiceSkinGame3(1);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else if(bCharacterGame3[1])
						{
							MakeChoiceCharacterGame3(1);
							FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
						}
						else
						{
							nChosesGame3++;
							nTimeChosesGame3[nChosesGame3-1] = nCountGame3;
							if(nChosesGame3 > 0 && nChosesGame3 < CHOOSES_GAME3) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
							if(nChosesGame3 >= CHOOSES_GAME3)
							{
								FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
								for(int x = 0; x < 10; x++) bCh[x] = false;
							}
						}
					}
				}
				if(nCountGame3 >= nMaxGame3 && piuconfig.nModeMach!=1)
				{
					nChosesGame3 = 2;
					nTimeChosesGame3[nChosesGame3-1] = nCountGame3;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					for(int x = 0; x < 10; x++) bCh[x] = false;
				}
				if(bNewCommand) CheckPresetGame3();
			}
		}
	#define TIME_EXIT_GAME3 1000
		if(nChosesGame3 >= CHOOSES_GAME3)
		{
			__int64 nTransEnd = nCountGame3 - nTimeChosesGame3[CHOOSES_GAME3-1];
			if(nTransEnd < TIME_EXIT_GAME3)
			{
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME3))
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
					if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME3))
					{
						if(x == 4)
						{
							if(nTransEnd < (TIME_TRANSI_SELECTOR_GAME3))
							{
								dApt = //pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME3, 2);
								pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME3 - 0.25, 2)/0.125 + 0.5;
								glPushMatrix();
								glTranslated(0.0, 0.0, -60.0);
								glScaled(dApt*1.25, dApt*1.25, 1.0);
								glColor4d(1.0, 1.0, 1.0, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME3);
								if(/*x == 4 && */nTransBorder > TIME_SHOW_PREVIEW_GAME3 && movie_songpreviewgame3->bLoad && movie_songpreviewgame3->bResize && bShow)
								{
									movie_songpreviewgame3->Update(dwWait);
									movie_songpreviewgame3->GrabAVIFrame();
									__int64 nCaretCover = movie_songpreviewgame3->dwNext;
									glBindTexture(GL_TEXTURE_2D, movie_songpreviewgame3->tex);

									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
									glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
									glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
									glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
									glEnd();
									if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME3)
									{
										dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME3;
										glColor4d(1.0, 1.0, 1.0, min(1.0-dApt, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME3));
										glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
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
									glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
									glBegin(GL_QUADS);
									glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
									glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
									glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
									glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
									glEnd();
								}
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
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
							__int64 nStartTransi = TIME_TRANSI_SELECTOR_GAME3/4*(-abs(x-4)+4-1);
							__int64 nLimitTransi = TIME_TRANSI_SELECTOR_GAME3/4*(-abs(x-4)+4);
							if(nTransEnd >= nStartTransi && nTransEnd < nLimitTransi)
							{
								dApt = double(nTransEnd-nStartTransi)/double(nLimitTransi-nStartTransi);
								double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(dApt)+dTargetx;
								double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
								double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
								double dtheta = ThetaFuncForXGame3(dx);
								double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
								double dalpha = AlphaFuncForXGame3(dx);
								glPushMatrix();
								glTranslated(dx, dy, dz-60.0);
								glRotated(dtheta, 0.0, 1.0, 0.0);
								glScaled(dscale, dscale, 1.0);
								glColor4d(1.0, 1.0, 1.0, min(dalpha, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME3));
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
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
								double dtheta = ThetaFuncForXGame3(dx);
								double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
								double dalpha = AlphaFuncForXGame3(dx);
								glPushMatrix();
								glTranslated(dx, dy, dz-60.0);
								glRotated(dtheta, 0.0, 1.0, 0.0);
								glScaled(dscale, dscale, 1.0);
								glColor4d(1.0, 1.0, 1.0, dalpha);
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
								glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
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
					if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_LR_GAME3))
					{
						dApt = pow(double(nTransEnd)/TIME_TRANSI_LR_GAME3, 0.5);
						glPushMatrix();
						glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(dApt), 0.0, 0.0);
						glColor4d(1.0, 1.0, 1.0, 1-dApt);
						glBindTexture(GL_TEXTURE_2D, ga3res->skin[(k?68:50)+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
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
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_UPDO_GAME3))
				{
					dApt = (double(nTransEnd))/(TIME_TRANSI_UPDO_GAME3);
					glPushMatrix();
					glScaled(1.0+0.8*(dApt), 1.0+0.8*(dApt), 1.0);
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[41+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[23+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[3]->texture[0]);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[4]->texture[0]);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[5]->texture[0]);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[6]->texture[0]);
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
					player_sesion[k]->nLastSpecial[0] = nRefSongsGame3[nCurSongGame3];
					player_sesion[k]->nLastSpecial[1] = nModeCurSongGame3[k];
				}
				if(movie_songpreviewgame3->bLoad)
				{
					movie_songpreviewgame3->CloseAVI();
					delete movie_songpreviewgame3;
					movie_songpreviewgame3 = new CMovieStream2();
				}
				/*for(int x = 0; x < nSizeMovies; x++)
				{
					if(movie_array[x]->bLoad) movie_array[x]->CloseAVI();
					delete movie_array[x];
				}*/
				//glDeleteTextures(9, nTexCurSongGame3);
				for(int x = 0; x < 9; x++) nTexCurSongGame3[x] = 0;
				bShow = false;
				LoadSongGame3();
				Go_Step();
				return 2;
			}
		}
	}
	else
	{
#define TIME_EXIT_CHANGERETRO_GAME3 1000
		__int64 nTransEnd = nCountGame3 - nTimeChangeRetroGame3;
		if(nTransEnd < TIME_EXIT_CHANGERETRO_GAME3)
		{
			if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME3))
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
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_SELECTOR_GAME3))
				{
					if(x == 4)
					{
						if(nTransEnd < (TIME_TRANSI_SELECTOR_GAME3))
						{
							dApt = //pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME3, 2);
							pow(double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME3 - 0.25, 2)/0.125 + 0.5;
							glPushMatrix();
							glTranslated(0.0, 0.0, -60.0);
							glScaled(dApt*1.25, dApt*1.25, 1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME3);
							if(/*x == 4 && */nTransBorder > TIME_SHOW_PREVIEW_GAME3 && movie_songpreviewgame3->bLoad && movie_songpreviewgame3->bResize && bShow)
							{
								movie_songpreviewgame3->Update(dwWait);
								movie_songpreviewgame3->GrabAVIFrame();
								__int64 nCaretCover = movie_songpreviewgame3->dwNext;
								glBindTexture(GL_TEXTURE_2D, movie_songpreviewgame3->tex);

								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5, -10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, 10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, 10.599, 0.0);
								glEnd();
								if(nCaretCover <= TIME_SHOWIMAGE_SONGBORDER_GAME3)
								{
									dApt = double(nCaretCover)/TIME_SHOWIMAGE_SONGBORDER_GAME3;
									glColor4d(1.0, 1.0, 1.0, min(1.0-dApt, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME3));
									glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
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
								glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
								glBegin(GL_QUADS);
								glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
								glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
								glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
								glEnd();
							}
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
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
						__int64 nStartTransi = TIME_TRANSI_SELECTOR_GAME3/4*(-abs(x-4)+4-1);
						__int64 nLimitTransi = TIME_TRANSI_SELECTOR_GAME3/4*(-abs(x-4)+4);
						if(nTransEnd >= nStartTransi && nTransEnd < nLimitTransi)
						{
							dApt = double(nTransEnd-nStartTransi)/double(nLimitTransi-nStartTransi);
							double dx = (((x<4)?-MAX_TARGET_X:MAX_TARGET_X)-dTargetx)*(dApt)+dTargetx;
							double dy = pow(dx/MAX_TARGET_X,3.0)*50.0;	// 0.75*40.0
							double dz = -pow(dx/MAX_TARGET_X,2.0)*25.0;	// 0.875*40.0
							double dtheta = ThetaFuncForXGame3(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForXGame3(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, min(dalpha, 1.0-double(nTransEnd)/TIME_TRANSI_SELECTOR_GAME3));
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
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
							double dtheta = ThetaFuncForXGame3(dx);
							double dscale = -0.75/MAX_TARGET_X*abs(dx)+1.25;
							double dalpha = AlphaFuncForXGame3(dx);
							glPushMatrix();
							glTranslated(dx, dy, dz-60.0);
							glRotated(dtheta, 0.0, 1.0, 0.0);
							glScaled(dscale, dscale, 1.0);
							glColor4d(1.0, 1.0, 1.0, dalpha);
							glBindTexture(GL_TEXTURE_2D, nTexCurSongGame3[x]);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(-14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 14.5,  10.599, 0.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 14.5, -10.599, 0.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-14.5, -10.599, 0.0);
							glEnd();
							glBindTexture(GL_TEXTURE_2D, ga3res->skin[14+nChaSongsGame3[nIndexShowGame3[x]]]->GetCurrentTex());
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
				if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_LR_GAME3))
				{
					dApt = pow(double(nTransEnd)/TIME_TRANSI_LR_GAME3, 0.5);
					glPushMatrix();
					glTranslated(k?40.0:-40.0+(k?-8.0:8.0)*(dApt), 0.0, 0.0);
					glColor4d(1.0, 1.0, 1.0, 1-dApt);
					glBindTexture(GL_TEXTURE_2D, ga3res->skin[(k?68:50)+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
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
			if(nTransEnd >= 0 && nTransEnd < (TIME_TRANSI_UPDO_GAME3))
			{
				dApt = (double(nTransEnd))/(TIME_TRANSI_UPDO_GAME3);
				glPushMatrix();
				glScaled(1.0+0.8*(dApt), 1.0+0.8*(dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[41+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[23+nChaSongsGame3[nCurSongGame3]]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[3]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[4]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(  8.625,  29.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 24.625,  29.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 24.625,  13.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(  8.625,  13.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[5]->texture[0]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-24.625, -13.75, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( -8.625, -13.75, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( -8.625, -29.75, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-24.625, -29.75, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, ga3res->skin[6]->texture[0]);
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
			nCurSongGame3 = 0;
			if(movie_songpreviewgame3->bLoad)
			{
				movie_songpreviewgame3->CloseAVI();
				delete movie_songpreviewgame3;
				movie_songpreviewgame3 = new CMovieStream2();
			}
			//glDeleteTextures(9, nTexCurSongGame3);
			for(int x = 0; x < 9; x++) nTexCurSongGame3[x] = 0;
			bShow = false;
			if(nChangeRetroGame3 == 2)
			{
				//SemiActualizateContextGame3();
				return 4;
			}
			if(nChangeRetroGame3 == 3)
			{
				nCaretCountGame3 = nCountGame3;
				return 5;
			}
			for(int k = 0; k < 2; k++) if(np[k])
			{
				int nCar = FoundFirstRetroGame3(nRetroActiveGame3?0:1);
				player_sesion[k]->nLastSpecial[0] = nCar;
				player_sesion[k]->nLastSpecial[1] = nModeCurSongGame3[k];
			}
			nCaretCountGame3 = nCountGame3;
			SemiActualizateContextGame3();
			nMaxGame3 = nMaxGame3 - nCaretCountGame3;
		}
	}

	return 0;
}

void SemiActualizateContextGame3()
{
	int nPlayerFocus = 0;
	nChangeRetroGame3 = 0;
	switch(piuconfig.nTimePlay)
	{
	default:
		nMaxGame3 = 120000+TIME_TRANSI_UPDO_GAME3+TIME_START_UPDO_GAME3; break;
	case 1:
		nMaxGame3 = 90000+TIME_TRANSI_UPDO_GAME3+TIME_START_UPDO_GAME3; break;
	case 2:
		nMaxGame3 = 60000+TIME_TRANSI_UPDO_GAME3+TIME_START_UPDO_GAME3; break;
	}
	nTimeChangeRetroGame3 = 0;
	nModeGame3 = 0;
	nCurSongGame3 = 0;
	nChosesGame3 = 0;
	nCountGame3 = 0;
	nRetroActiveGame3 = 0;
	nTimeCurSongGame3 = TIME_START_SELECTOR_GAME3+TIME_TRANSI_SELECTOR_GAME3+TIME_GLOUTH_SELECTOR_GAME3;
	bLoadMusicCoverGame3 = false;
	strBestScoreMachineGame3 = _T("");
	nBestScoreMachineGame3 = 0;
	for(int x = 0; x < 2; x++)
	{
		nTimeSkinGame3[x] = 0;
		bSkinGame3[x] = false;
		nTimeTurnSkinGame3[x] = 0;
		nDirTurnSkinGame3[x] = 0;
		nTimeCharacterGame3[x] = 0;
		bCharacterGame3[x] = false;
		nTimeTurnCharacterGame3[x] = 0;
		nDirTurnCharacterGame3[x] = 0;
		nBestScorePlayerGame3[x] = 0;
		nModeCurSongGame3[x] = 0;
		nBackModeCurSongGame3[x] = 0;
		nTimeModeCurSongGame3[x] = TIME_START_SELECTOR_GAME3+TIME_TRANSI_SELECTOR_GAME3+TIME_GLOUTH_SELECTOR_GAME3;
		nPrefModeCurSongGame3[x] = 0;
		nLevelCurGame3[x] = 1;
		nIndLastPresetGame3[x] = -1;	// Show all
		nTimeLastPresetGame3[x] = (TIME_START_UPDO_GAME3+TIME_TRANSI_UPDO_GAME3)+(TIME_START_LR_GAME3+TIME_TRANSI_LR_GAME3);
		if(np[x]) ShowSesionTrayExp(x);
		for(int y = 0; y < 10; y++) nCommandGame3[x][y] = -1;
		for(int y = 0; y < MAX_COMMANDS_GAME3; y++) nComandGame3[x][y] = -1;
	}
	for(int x = 0; x < 10; x++) bEnArrowsGame3[x] = false;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		nRetroActiveGame3 = nNOLTotalSongsGame3[player_sesion[x]->nLastSpecial[0]];
		EnumSongsGame3(nNOLTotalSongsGame3[player_sesion[x]->nLastSpecial[0]]);
		nPlayerFocus = x;
		break;
	}
	bool bFoundLast = false;
	for(int x = 0; x < nSizeSongsGame3; x++)
	{
		if(nRefSongsGame3[x] == player_sesion[nPlayerFocus]->nLastSpecial[0])
		{
			bFoundLast = true;
			nCurSongGame3 = x;
			break;
		}
	}
	for(int y = 0; y < 2; y++) if(np[y])
	{
		nModeCurSongGame3[y] = GetAvalibleModeGame3(nCurSongGame3, player_sesion[y]->nLastArcade[1]);
		nBackModeCurSongGame3[y] = nModeCurSongGame3[y];
		nPrefModeCurSongGame3[y] = nModeCurSongGame3[y];
	}
	if(!bFoundLast) nCurSongGame3 = 0;
	nTimeChaGame3 = TIME_START_UPDO_GAME3+TIME_TRANSI_UPDO_GAME3;

	ActualizateIndexShowGame3();
	for(int x = 0; x < 9; x++)
	{
		/*TCHAR chFile[260];
		_sprintf(chFile, _T("song/preview/%ls.png"), chSongsGame3[nIndexShowGame3[x]]);*/
		nTexCurSongGame3[x] = nTexSongsGame3[nIndexShowGame3[x]];;
	}
	GetBestScoresGame3(nCurSongGame3, nModeCurSongGame3[nPlayerFocus]);
	EnumSkinsGame3();
	EnumCharactersGame3();
	dwLastTick = dwTick = SDL_GetTicks();
}

int StageCheckGame3()
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
		player_sesion[x]->nMileage += nMileage;
		double dApt = double(5-nRankStepEval[x])/5*(bUsb[0]?1.0:0.5)*(player_sesion[x]->nLockedAsp[ITEM_MOREEXP]?2.0:1.0);
		PlusExpPlayer(x, nLevelCurGame3[x], dApt);
		if(piuconfig.nModeMach)
		{
			np[x] = 0;
		}
		else
		{
			int nMinus = 2;
			switch(nChaTotalSongsGame3[player_sesion[x]->nLastSpecial[0]])
			{
			case 0:
				nMinus = 8;
				break;
			case 1:
				nMinus = 7;
				break;
			case 2:
				nMinus = 6;
				break;
			case 3:
				nMinus = 5;
				break;
			case 4:
				nMinus = 4;
				break;
			case 5:
				nMinus = 3;
				break;
			case 7:
				nMinus = 1;
				break;
			}
			bIs[x] = true;
			np[x]-=nMinus;
			//if(nRankStepEval[x] >= 5) np[x]-=2;
			if(nRankStepEval[x] >= 2 && bBonusStageGame3[x])
			{
				np[x]-=2;
				bBonusStageGame3[x] = false;
			}
			np[x] = max(0, np[x]);
			//if(np[x] <= 1) np[x] = 0;
		}
	}
	if((!np[0]) && (!np[1])) return 1;
	else if((!np[0] && bIs[0]) || (!np[1] && bIs[1])) return 2;
	return 0;
}

////////////////////////////////////////////////
// ** Funciones principales

int CreateContextGame3()
{
	if(!EnumTotalSongsGame3()) return 0;
	nRefSkinGame3 = (int*)calloc(0, sizeof(int));
	uTexSkinGame3 = (GLuint*)calloc(0, sizeof(GLuint));
	nRefCharacterGame3 = (int*)calloc(0, sizeof(int));
	GLuint* uTexCharacterGame3 = (GLuint*)calloc(0, sizeof(GLuint));
	return 1;
}

void DestroyContextGame3()
{
	glDeleteTextures(nSizeTotalGame3, nTexTotalSongsGame3);
	delete nRefSkinGame3;
	delete uTexSkinGame3;
	delete nRefCharacterGame3;
	delete uTexCharacterGame3;
}

void ClearContextGame3()
{
	if(movie_specialbg->bLoad) movie_specialbg->CloseAVI();
	delete movie_specialbg;
	delete ga3res;
	DeleteSkinsGame3();
	DeleteCharactersGame3();
}

void ActualizateContextGame3()
{
	nCurStage = 1;
	movie_specialbg = new CMovieStream2();
	movie_songpreviewgame3 = new CMovieStream2();
	movie_specialbg->OpenAVI(_T("Game3/ArcadeBG.avi"));
	movie_specialbg->bLoop = true;

	ga3res = new CGame3Res();
	ga3res->LoadSkin();

	switch(piuconfig.nTimePlay)
	{
	default:
		nMaxGame3 = 120000+TIME_TRANSI_UPDO_GAME3+TIME_START_UPDO_GAME3; break;
	case 1:
		nMaxGame3 = 90000+TIME_TRANSI_UPDO_GAME3+TIME_START_UPDO_GAME3; break;
	case 2:
		nMaxGame3 = 60000+TIME_TRANSI_UPDO_GAME3+TIME_START_UPDO_GAME3; break;
	}
	nInterGame3 = 0;
	nRetroActiveGame3 = 0;
	int nPlayerFocus = 0;
	nChangeRetroGame3 = 0;
	nTimeChangeRetroGame3 = 0;
	nModeGame3 = 0;
	nCurSongGame3 = 0;
	nChosesGame3 = 0;
	nCountGame3 = 0;
	nTimeCurSongGame3 = TIME_START_SELECTOR_GAME3+TIME_TRANSI_SELECTOR_GAME3+TIME_GLOUTH_SELECTOR_GAME3;
	bLoadMusicCoverGame3 = false;
	bContinueUsbGame3 = true;
	strBestScoreMachineGame3 = _T("");
	nBestScoreMachineGame3 = 0;
	bIsAvalibleUnlockerGame3 = true;
	for(int x = 0; x < 2; x++)
	{
		nTimeSkinGame3[x] = 0;
		bSkinGame3[x] = false;
		nTimeTurnSkinGame3[x] = 0;
		nDirTurnSkinGame3[x] = 0;
		nCurSkinGame3[x] = 0;
		nTimeCharacterGame3[x] = 0;
		bCharacterGame3[x] = false;
		nTimeTurnCharacterGame3[x] = 0;
		nDirTurnCharacterGame3[x] = 0;
		nCurCharacterGame3[x] = 0;
		nBestScorePlayerGame3[x] = 0;
		bBonusStageGame3[x] = true;
		nModeCurSongGame3[x] = 0;
		nBackModeCurSongGame3[x] = 0;
		nTimeModeCurSongGame3[x] = TIME_START_SELECTOR_GAME3+TIME_TRANSI_SELECTOR_GAME3+TIME_GLOUTH_SELECTOR_GAME3;
		nPrefModeCurSongGame3[x] = 0;
		nLevelCurGame3[x] = 1;
		nIndLastPresetGame3[x] = -1;	// Show all
		nTimeLastPresetGame3[x] = (TIME_START_UPDO_GAME3+TIME_TRANSI_UPDO_GAME3)+(TIME_START_LR_GAME3+TIME_TRANSI_LR_GAME3);
		if(np[x]) ShowSesionTrayExp(x);
		for(int y = 0; y < 6; y++)
		{
			nIndSizePresetGame3[x][y] = 0;
			nIndPresetGame3[x][y*5] = 0;
			nIndPresetGame3[x][y*5+1] = 0;
			nIndPresetGame3[x][y*5+2] = 0;
			nIndPresetGame3[x][y*5+3] = 0;
			nIndPresetGame3[x][y*5+4] = 0;
		}
		dDCurGame3[x][0] = 0.0;
		dDCurGame3[x][1] = 0.0;
		dDCurGame3[x][2] = 0.0;
		dDCurGame3[x][3] = 0.0;
		dDCurGame3[x][4] = 0.0;
		for(int y = 0; y < 10; y++) nCommandGame3[x][y] = -1;
		for(int y = 0; y < MAX_COMMANDS_GAME3; y++) nComandGame3[x][y] = -1;
	}
	for(int x = 0; x < 10; x++) bEnArrowsGame3[x] = false;
	for(int x = 0; x < 2; x++) if(np[x])
	{
		nRetroActiveGame3 = nNOLTotalSongsGame3[player_sesion[x]->nLastSpecial[0]];
		EnumSongsGame3(nNOLTotalSongsGame3[player_sesion[x]->nLastSpecial[0]]);
		nPlayerFocus = x;
		break;
	}
	bool bFoundLast = false;
	for(int x = 0; x < nSizeSongsGame3; x++)
	{
		if(nRefSongsGame3[x] == player_sesion[nPlayerFocus]->nLastSpecial[0])
		{
			bFoundLast = true;
			nCurSongGame3 = x;
			break;
		}
	}
	for(int y = 0; y < 2; y++) if(np[y])
	{
		if(y == nPlayerFocus)
			nModeCurSongGame3[y] = GetAvalibleModeGame3(nCurSongGame3, player_sesion[nPlayerFocus]->nLastArcade[1]);
		else
			nModeCurSongGame3[y] = GetAvalibleModeGame3(nCurSongGame3, 0);
		nBackModeCurSongGame3[y] = nModeCurSongGame3[y];
		nPrefModeCurSongGame3[y] = nModeCurSongGame3[y];
	}
	if(!bFoundLast) nCurSongGame3 = 0;
	nTimeChaGame3 = TIME_START_UPDO_GAME3+TIME_TRANSI_UPDO_GAME3;

	ActualizateIndexShowGame3();
	for(int x = 0; x < 9; x++)
	{
		/*TCHAR chFile[260];
		_sprintf(chFile, _T("song/preview/%ls.png"), chSongsGame3[nIndexShowGame3[x]]);*/
		nTexCurSongGame3[x] = nTexSongsGame3[nIndexShowGame3[x]];;
	}
	GetBestScoresGame3(nCurSongGame3, nModeCurSongGame3[nPlayerFocus]);
	EnumSkinsGame3();
	EnumCharactersGame3();
	dwLastTick = dwTick = SDL_GetTicks();
}

bool bContAfterSplashGame3 = false;

int FrameGame3()
{
	ga3res->Update(dwWait);
	switch(nInterGame3)
	{
	case 0:
		switch(DrawInterfaceSelectGame3())
		{
		case 1:
			ClearContextGame3();
			return INT_WELCOME;
			break;
		case 2:
			nInterGame3 = 1;
			break;
		case 3:
			return INT_ITEMSHOP;
			break;
		case 4:
			//ClearContextGame3();
			ActualizateContextUsb();
			bSaveUsb = false;
			bContinueUsbGame3 = true;
			nInterGame3 = 4;
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
			nInterGame3 = 6;
			break;
		}
		break;
	case 1:
		if(!Draw_All())
		{
			nInterGame3 = 2;
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
			if(nMC > 50 || (dMB <= 0.0 && piuconfig.nStageBreak!=0 && nCurStage >= piuconfig.nStageBreak))
			{
				nInterGame3 = 5;
				np[0] = 0;
				np[1] = 0;
				DeleteSongGame3();
			}
		}
		break;
	case 2:
		if(!FrameStepEval())
		{
			nInterGame3 = 8;
			ActualizateContextStageStepEval();
			{
				DeleteSongGame3();
				int nCode = StageCheckGame3();
				if(nCode == 0) nInterGame3 = 8;
				else
				{
					nInterGame3 = 4;
					if(nCode == 1) bContinueUsbGame3 = false;
					if(nCode == 2) bContinueUsbGame3 = true;
					bSaveUsb = true;
					ActualizateContextUsb();
				}
				if(nSizeUnlock)
				{
					nModeUnlock = 0;
					ActualizateContextUnlock();
					nInterGame3 = 7;
					if(nCode != 0) bContAfterSplashGame3 = false;
					else bContAfterSplashGame3 = true;
				}
			}
			SemiActualizateContextGame3();
		}
		break;
	case 3:
		{
			int nCode = StageCheckGame3();
			if(nCode == 0) nInterGame3 = 8;
			else
			{
				nInterGame3 = 4;
				if(nCode == 1) bContinueUsbGame3 = false;
				if(nCode == 2) bContinueUsbGame3 = true;
				bSaveUsb = true;
				ActualizateContextUsb();
			}
			if(nSizeUnlock)
			{
				nModeUnlock = 0;
				ActualizateContextUnlock();
				nInterGame3 = 7;
				if(nCode != 0) bContAfterSplashGame3 = false;
				else bContAfterSplashGame3 = true;
			}
			DeleteSongGame3();
		}
		SemiActualizateContextGame3();
		break;
	case 4:	// Dibujar Sistema USB
		if(!FrameUsb())
		{
			nInterGame3 = 0;
			if(!bContinueUsbGame3)
			{
				ClearContextGame3();
				return INT_WELCOME;
			}
			else SemiActualizateContextGame3();
		}
		break;
	case 5:
		if(!FrameStageBreakStepEval())
		{
			nInterGame3 = 4;
			np[0] = 0;
			np[1] = 0;
			bSaveUsb = true;
			ActualizateContextUsb();
			bContinueUsbGame3 = false;
		}
		break;
	case 6:
		if(!FrameUnlock())
		{
			nInterGame3 = 0;
			SemiActualizateContextGame3();
			nMaxGame3 = nMaxGame3 - nCaretCountGame3;
		}
		break;
	case 7:
		if(FrameUnlock() != INT_UNLOCK)
		{
			nInterGame3 = bContAfterSplashGame3?8:4;
		}
		break;
	case 8:
		if(!FrameStageStepEval())
		{
			nInterGame3 = 0;
			nCurStage++;
		}
		break;
	}
	return INT_GAME_3;
}

int EventGame3(int nEvent, int nKey)
{
	switch(nInterGame3)
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
	}
	return 1;
}
