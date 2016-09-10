#include "stepeval.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "StepEvalRes.h"
#include "OperStr.h"
#include "USBsys.h"

int nTotalScorePlayer[2];

CStepEvalRes* evalres;
CMovieStream2* movie_bgeval;
int nSizeMovieStage = 0;
int nMovieStage = -1;
int nSizeMovieBStage = 0;
CString* strNameMovieStage;
CString* strNameMovieBStage;
CMovieStream2* movie_stage = NULL;
FMOD_SOUND* sound_stage;
CMovieStream2* movie_gameover = NULL;
FMOD_SOUND* sound_gameover;
CMovieStream2* movie_stagebreak = NULL;
FMOD_SOUND* sound_hs;
CMovieStream2* movie_hs = NULL;
GLuint tex_bgeval;
FMOD_SOUND *sound_click = 0;
FMOD_SOUND *sound_boom = 0;
FMOD_SOUND *sound_ranka1 = 0;
FMOD_SOUND *sound_ranka2 = 0;
FMOD_SOUND *sound_rankb1 = 0;
FMOD_SOUND *sound_rankb2 = 0;
FMOD_SOUND *sound_rankc1 = 0;
FMOD_SOUND *sound_rankc2 = 0;
FMOD_SOUND *sound_rankd1 = 0;
FMOD_SOUND *sound_rankd2 = 0;
FMOD_SOUND *sound_rankf1 = 0;
FMOD_SOUND *sound_rankf2 = 0;
FMOD_SOUND *sound_ranks1 = 0;
FMOD_SOUND *sound_ranks2 = 0;
FMOD_SOUND *sound_ranksucess = 0;
FMOD_SOUND *sound_rankfailure = 0;
FMOD_SOUND *sound_break = 0;

int nModeStepEval = 0;
int nRankStepEval[2];
__int64 nCountStepEval = 0;
__int64 nTimeOutStepEval = 2000;
__int64 nTimeShowRankEval = 0;
__int64 nTimePlayer1StepEval = 0;
__int64 nTimePlayer2StepEval = 0;

int bMissionSucess = 0;
int nSizeCondSucess = 0;
CString* strCondSucess;

int nCurLetterStepEval[2];
int nDirTurnLetterStepEval[2];
__int64 nTimeTurnLetterStepEval[2];
int nSizeLetterStepEval = 0;
CString strCharSetHSSteopEval;
GLuint* uTexLetterStepEval;

bool bEnArrowsStepEval[10];
__int64 nTimeEnArrowsStepEval[10];

TCHAR chNameStepEval[2][12];
int nCountNameStepEval[2];
bool bChoosenNameStepEval[2];
bool bNewHighScore = false;
int nIndexHighScore[2];

double dNotesP[2];

////////////////////////////////////////////
// ** Funciones principales

int CreateContextStepEval()
{
	FILE* hFile = _fopen(_T("StepEval/stages.acc"), _T("r"));
	if(hFile != NULL)
	{
		TCHAR ch[260];
		_fgets(ch, 260, hFile);
		if(_sscanf(ch, _T("SIZE:%d"), &nSizeMovieStage) != 1)
		{
			fclose(hFile);
			return 0;
		}
		if(nSizeMovieStage)
		{
			strNameMovieStage = new CString[nSizeMovieStage];
			for(int x = 0; x < nSizeMovieStage; x++)
			{
				if(_fgets(ch, 260, hFile) == NULL) break;
				strNameMovieStage[x] = ( ch);
				strNameMovieStage[x].Remove(_T('\n'));
				strNameMovieStage[x].Remove(_T('\r'));
			}
		}
		fclose(hFile);
	}
	hFile = _fopen(_T("StepEval/BStages.acc"), _T("r"));
	if(hFile != NULL)
	{
		TCHAR ch[260];
		_fgets(ch, 260, hFile);
		if(_sscanf(ch, _T("SIZE:%d"), &nSizeMovieBStage) != 1)
		{
			fclose(hFile);
			return 0;
		}
		if(nSizeMovieBStage)
		{
			strNameMovieBStage = new CString[nSizeMovieBStage];
			for(int x = 0; x < nSizeMovieBStage; x++)
			{
				if(_fgets(ch, 260, hFile) == NULL) break;
				strNameMovieBStage[x] = ( ch);
				strNameMovieBStage[x].Remove(_T('\n'));
				strNameMovieBStage[x].Remove(_T('\r'));
			}
		}
		fclose(hFile);
	}
	FMOD_System_CreateStream(sys, "StepEval/Click.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound_click);
	FMOD_System_CreateStream(sys, "StepEval/Boom.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_boom);
	FMOD_System_CreateStream(sys, "StepEval/RankA1.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_ranka1);
	FMOD_System_CreateStream(sys, "StepEval/RankA2.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_ranka2);
	FMOD_System_CreateStream(sys, "StepEval/RankB1.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_rankb1);
	FMOD_System_CreateStream(sys, "StepEval/RankB2.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_rankb2);
	FMOD_System_CreateStream(sys, "StepEval/RankC1.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_rankc1);
	FMOD_System_CreateStream(sys, "StepEval/RankC2.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_rankc2);
	FMOD_System_CreateStream(sys, "StepEval/RankD1.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_rankd1);
	FMOD_System_CreateStream(sys, "StepEval/RankD2.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_rankd2);
	FMOD_System_CreateStream(sys, "StepEval/RankF1.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_rankf1);
	FMOD_System_CreateStream(sys, "StepEval/RankF2.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_rankf2);
	FMOD_System_CreateStream(sys, "StepEval/RankS1.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_ranks1);
	FMOD_System_CreateStream(sys, "StepEval/RankS2.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_ranks2);
	FMOD_System_CreateStream(sys, "StepEval/StageBreak.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_break);
	FMOD_System_CreateStream(sys, "StepEval/GAMEOVER.MP3", FMOD_HARDWARE | FMOD_2D, 0, &sound_gameover);
	FMOD_System_CreateStream(sys, "StepEval/HS/back.mp3", FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_NORMAL, 0, &sound_hs);
	FMOD_System_CreateStream(sys, "StepEval/RankMissionSucess.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_ranksucess);
	FMOD_System_CreateStream(sys, "StepEval/RankMissionFailure.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_rankfailure);

	return 1;
}

void DestroyContextStepEval()
{
	FMOD_Sound_Release(sound_click);
	FMOD_Sound_Release(sound_boom);
	FMOD_Sound_Release(sound_ranka1);
	FMOD_Sound_Release(sound_ranka2);
	FMOD_Sound_Release(sound_rankb1);
	FMOD_Sound_Release(sound_rankb2);
	FMOD_Sound_Release(sound_rankc1);
	FMOD_Sound_Release(sound_rankc2);
	FMOD_Sound_Release(sound_rankd1);
	FMOD_Sound_Release(sound_rankd2);
	FMOD_Sound_Release(sound_rankf1);
	FMOD_Sound_Release(sound_rankf2);
	FMOD_Sound_Release(sound_ranks1);
	FMOD_Sound_Release(sound_ranks2);
	FMOD_Sound_Release(sound_break);
	FMOD_Sound_Release(sound_gameover);
	FMOD_Sound_Release(sound_hs);
	FMOD_Sound_Release(sound_ranksucess);
	FMOD_Sound_Release(sound_rankfailure);

	if(nSizeMovieStage)
	{
		nSizeMovieStage = 0;
		delete[] strNameMovieStage;
	}
	if(nSizeMovieBStage)
	{
		nSizeMovieBStage = 0;
		delete[] strNameMovieBStage;
	}
	ClearContextStepEvalRes();
}

#define TIME_START_SCORELOOP_STEPEVAL 1000
#define TIME_CHANGE_NUMBER_STEPEVAL 100
#define TIME_START_RANK_STEPEVAL 600
#define TIME_TRANSI_RANK_STEPEVAL 200
#define TIME_GLOUTH_RANK_STEPEVAL 600
#define TIME_RUN_RANK_STEPEVAL 6000
#define TIME_START_ORBE_STEPEVAL 0
#define TIME_TRANSI_ORBE_STEPEVAL 500
#define TIME_GLOUTH_ORBE_STEPEVAL 500
#define TIME_START_INTRO_STEPEVAL 300
#define TIME_TRANSI_INTRO_STEPEVAL 500
#define TIME_GLOUTH_INTRO_STEPEVAL 500
#define TIME_START_TBAR_STEPEVAL 500
#define TIME_TRANSI_TBAR_STEPEVAL 500

void ActualizateContextStepEval()
{
    if(movie_bgeval == NULL)
	{
        movie_bgeval = new CMovieStream2();
        movie_bgeval->OpenAVI(_T("StepEval/back.avi"));
        movie_bgeval->bLoop = true;
	}
	if(movie_hs == NULL)
	{
        movie_hs = new CMovieStream2();
        movie_hs->OpenAVI(_T("StepEval/HS/back.avi"));
        movie_hs->bLoop = true;
	}

    if(evalres == NULL)
    {
        evalres = new CStepEvalRes();
        evalres->LoadSkin();
    }

	portLights->WriteVal(0);
	COperStr *oper;
	bMissionSucess = false;
	nCountStepEval = 0;
	nTimePlayer1StepEval = 0;
	nTimePlayer2StepEval = 0;
	//movie_bgeval->SetZeroCount();
	double d[] = {0.0, 0.0, 0.0, 0.0};
	switch(piuconfig.nLevelJud)
	{
	case 0:
		d[0]=0.85;d[1]=0.75;d[0]=0.65;d[0]=0.55;
		break;
	case 1:
		d[0]=0.9;d[1]=0.8;d[2]=0.7;d[3]=0.6;
		break;
	case 2:
		d[0]=0.9;d[1]=0.82;d[2]=0.74;d[3]=0.66;
		break;
	case 3:
		d[0]=0.92;d[1]=0.84;d[2]=0.76;d[3]=0.68;
		break;
	case 4:
		d[0]=0.94;d[1]=0.86;d[2]=0.78;d[3]=0.7;
		break;
	case 5:
		d[0]=0.94;d[1]=0.87;d[2]=0.80;d[3]=0.73;
		break;
	case 6:
		d[0]=0.95;d[1]=0.88;d[2]=0.83;d[3]=0.75;
		break;
	default:
		d[0]=0.9;d[1]=0.8;d[2]=0.7;d[3]=0.6;
		break;
	}
	if(g_bPlayer[0])
	{
		int nDig = 0;
		if(nModeStepEval == 2) viewp1->status.nScore += viewp1->status.nBSCorrect*100000;
		CString str;
		str.Format(_T("%.3d"), viewp1->status.nPerfect[0]+viewp1->status.nPerfect[1]+viewp1->status.nPerfect[2]);
		nDig+=str.GetLength();
		str.Format(_T("%.3d"), viewp1->status.nGreat[0]+viewp1->status.nGreat[1]+viewp1->status.nGreat[2]);
		nDig+=str.GetLength();
		str.Format(_T("%.3d"), viewp1->status.nGood[0]+viewp1->status.nGood[1]+viewp1->status.nGood[2]);
		nDig+=str.GetLength();
		str.Format(_T("%.3d"), viewp1->status.nBad[0]+viewp1->status.nBad[1]+viewp1->status.nBad[2]);
		nDig+=str.GetLength();
		str.Format(_T("%.3d"), viewp1->status.nMiss[0]+viewp1->status.nMiss[1]+viewp1->status.nMiss[2]);
		nDig+=str.GetLength();
		if(nModeStepEval == 2) str.Format(_T("%.3d %.3d"), viewp1->status.nBSCorrect, viewp1->status.nBSIncorrect);
		else str.Format(_T("%.3d"), viewp1->status.nMaxCombo[0]);
		nDig+=str.GetLength();
		double dTotalPond = double(viewp1->status.nPerfect[0]+viewp1->status.nPerfect[1]+viewp1->status.nPerfect[2]+
						 viewp1->status.nGreat[0]+viewp1->status.nGreat[1]+viewp1->status.nGreat[2]+
						 viewp1->status.nGood[0]+viewp1->status.nGood[1]+viewp1->status.nGood[2]+
						 viewp1->status.nBad[0]+viewp1->status.nBad[1]+viewp1->status.nBad[2]+
						 viewp1->status.nMiss[0]+viewp1->status.nMiss[1]+viewp1->status.nMiss[2]);
		double dPond = viewp1->status.nPerfect[0]+viewp1->status.nPerfect[1]+viewp1->status.nPerfect[2]+
						 (viewp1->status.nGreat[0]+viewp1->status.nGreat[1]+viewp1->status.nGreat[2])*0.9+
						 (viewp1->status.nGood[0]+viewp1->status.nGood[1]+viewp1->status.nGood[2])*0.75+
						 (viewp1->status.nBad[0]+viewp1->status.nBad[1]+viewp1->status.nBad[2])*0.6;
		double dNote = dTotalPond == 0.0?1.0:dPond/dTotalPond;
		double dNoteBS = ((viewp1->status.nBSCorrect+viewp1->status.nBSIncorrect)==0)?
			(1.0):(double(viewp1->status.nBSCorrect)/double(viewp1->status.nBSCorrect+viewp1->status.nBSIncorrect));
		dNote = min(dNote,dNoteBS);
		if(dNote >= d[0])	// Rank A
			nRankStepEval[0] = 1;
		if(dNote < d[0] && dNote >= d[1])	// Rank B
			nRankStepEval[0] = 2;
		if(dNote < d[1] && dNote >= d[2])	// Rank C
			nRankStepEval[0] = 3;
		if(dNote < d[2] && dNote >= d[3])	// Rank D
			nRankStepEval[0] = 4;
		if(dNote < d[3])	// Rank F
			nRankStepEval[0] = 5;
		if(dNote >= 0.95 && !viewp1->status.nMiss[0])	// Rank S
			nRankStepEval[0] = 0;

		dNotesP[0] = dNote;

		if(nModeStepEval == 3) str.Format(_T("%.1lf%%"), dNotesP[0]*100.0);
		else str.Format(_T("%.3d"), viewp1->status.nScore);
		nDig+=str.GetLength();

		if(nModeStepEval == 1)
		{
			str.Format(_T("%.3d %.3d"), viewp1->status.nBSCorrect, viewp1->status.nBSIncorrect);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp1->status.nHearts);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp1->status.nPotions);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp1->status.nMines);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp1->status.nVelocitys);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp1->status.nItems);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp1->status.nMaxMissComboTotal);
			nDig+=str.GetLength();
			str.Format(_T("%.1lf%%"), viewp1->status.dLife*100);
			nDig+=str.GetLength();

			if(!viewp1->m_bAutoPlay && viewp1->m_nCharacter == 0)
			{
                oper = new COperStr();
				str.Format(_T("%d"), viewp1->status.nPerfect[0]+viewp1->status.nPerfect[1]+viewp1->status.nPerfect[2]);
				oper->AddVar(CString(_T("nPerfect")), str);
				str.Format(_T("%d"), viewp1->status.nGreat[0]+viewp1->status.nGreat[1]+viewp1->status.nGreat[2]);
				oper->AddVar(CString(_T("nGreat")), str);
				str.Format(_T("%d"), viewp1->status.nGood[0]+viewp1->status.nGood[1]+viewp1->status.nGood[2]);
				oper->AddVar(CString(_T("nGood")), str);
				str.Format(_T("%d"), viewp1->status.nBad[0]+viewp1->status.nBad[1]+viewp1->status.nBad[2]);
				oper->AddVar(CString(_T("nBad")), str);
				str.Format(_T("%d"), viewp1->status.nMiss[0]+viewp1->status.nMiss[1]+viewp1->status.nMiss[2]);
				oper->AddVar(CString(_T("nMiss")), str);
				str.Format(_T("%d"), viewp1->status.nMaxComboTotal);
				oper->AddVar(CString(_T("nMaxCombo")), str);
				str.Format(_T("%d"), viewp1->status.nMaxMissComboTotal);
				oper->AddVar(CString(_T("nMissCombo")), str);

				str.Format(_T("%d"), viewp1->status.nPerfect[0]);
				oper->AddVar(CString(_T("nPerfect1")), str);
				str.Format(_T("%d"), viewp1->status.nGreat[0]);
				oper->AddVar(CString(_T("nGreat1")), str);
				str.Format(_T("%d"), viewp1->status.nGood[0]);
				oper->AddVar(CString(_T("nGood1")), str);
				str.Format(_T("%d"), viewp1->status.nBad[0]);
				oper->AddVar(CString(_T("nBad1")), str);
				str.Format(_T("%d"), viewp1->status.nMiss[0]);
				oper->AddVar(CString(_T("nMiss1")), str);
				str.Format(_T("%d"), viewp1->status.nMaxCombo[0]);
				oper->AddVar(CString(_T("nMaxCombo1")), str);
				str.Format(_T("%d"), viewp1->status.nMaxMissCombo[0]);
				oper->AddVar(CString(_T("nMissCombo1")), str);

				str.Format(_T("%d"), viewp1->status.nPerfect[1]);
				oper->AddVar(CString(_T("nPerfect2")), str);
				str.Format(_T("%d"), viewp1->status.nGreat[1]);
				oper->AddVar(CString(_T("nGreat2")), str);
				str.Format(_T("%d"), viewp1->status.nGood[1]);
				oper->AddVar(CString(_T("nGood2")), str);
				str.Format(_T("%d"), viewp1->status.nBad[1]);
				oper->AddVar(CString(_T("nBad2")), str);
				str.Format(_T("%d"), viewp1->status.nMiss[1]);
				oper->AddVar(CString(_T("nMiss2")), str);
				str.Format(_T("%d"), viewp1->status.nMaxCombo[1]);
				oper->AddVar(CString(_T("nMaxCombo2")), str);
				str.Format(_T("%d"), viewp1->status.nMaxMissCombo[1]);
				oper->AddVar(CString(_T("nMissCombo2")), str);

				str.Format(_T("%d"), viewp1->status.nPerfect[2]);
				oper->AddVar(CString(_T("nPerfect3")), str);
				str.Format(_T("%d"), viewp1->status.nGreat[2]);
				oper->AddVar(CString(_T("nGreat3")), str);
				str.Format(_T("%d"), viewp1->status.nGood[2]);
				oper->AddVar(CString(_T("nGood3")), str);
				str.Format(_T("%d"), viewp1->status.nBad[2]);
				oper->AddVar(CString(_T("nBad3")), str);
				str.Format(_T("%d"), viewp1->status.nMiss[2]);
				oper->AddVar(CString(_T("nMiss3")), str);
				str.Format(_T("%d"), viewp1->status.nMaxCombo[2]);
				oper->AddVar(CString(_T("nMaxCombo3")), str);
				str.Format(_T("%d"), viewp1->status.nMaxMissCombo[2]);
				oper->AddVar(CString(_T("nMissCombo3")), str);

				str.Format(_T("%d"), viewp1->status.nScore);
				oper->AddVar(CString(_T("nScore")), str);
				str.Format(_T("%d"), viewp1->status.nBSCorrect);
				oper->AddVar(CString(_T("nBSCorrect")), str);
				str.Format(_T("%d"), viewp1->status.nBSIncorrect);
				oper->AddVar(CString(_T("nBSIncorrect")), str);
				str.Format(_T("%d"), viewp1->status.nHearts);
				oper->AddVar(CString(_T("nHearts")), str);
				str.Format(_T("%d"), viewp1->status.nPotions);
				oper->AddVar(CString(_T("nPotions")), str);
				str.Format(_T("%d"), viewp1->status.nMines);
				oper->AddVar(CString(_T("nMines")), str);
				str.Format(_T("%d"), viewp1->status.nVelocitys);
				oper->AddVar(CString(_T("nVelocitys")), str);
				str.Format(_T("%d"), viewp1->status.nItems);
				oper->AddVar(CString(_T("nItems")), str);
				str.Format(_T("%lf"), viewp1->status.dLife*100);
				oper->AddVar(CString(_T("dLife")), str);
				str.Format(_T("%d"), nRankStepEval[0]);
				oper->AddVar(CString(_T("nRank")), str);
				if(g_bPlayer[1])
				{
					str.Format(_T("%d"), viewp2->status.nPerfect[0]+viewp2->status.nPerfect[1]+viewp2->status.nPerfect[2]);
					oper->AddVar(CString(_T("nOtherPerfect")), str);
					str.Format(_T("%d"), viewp2->status.nGreat[0]+viewp2->status.nGreat[1]+viewp2->status.nGreat[2]);
					oper->AddVar(CString(_T("nOtherGreat")), str);
					str.Format(_T("%d"), viewp2->status.nGood[0]+viewp2->status.nGood[1]+viewp2->status.nGood[2]);
					oper->AddVar(CString(_T("nOtherGood")), str);
					str.Format(_T("%d"), viewp2->status.nBad[0]+viewp2->status.nBad[1]+viewp2->status.nBad[2]);
					oper->AddVar(CString(_T("nOtherBad")), str);
					str.Format(_T("%d"), viewp2->status.nMiss[0]+viewp2->status.nMiss[1]+viewp2->status.nMiss[2]);
					oper->AddVar(CString(_T("nOtherMiss")), str);
					str.Format(_T("%d"), viewp2->status.nMaxComboTotal);
					oper->AddVar(CString(_T("nOtherMaxCombo")), str);
					str.Format(_T("%d"), viewp2->status.nMaxMissComboTotal);
					oper->AddVar(CString(_T("nOtherMissCombo")), str);

					str.Format(_T("%d"), viewp2->status.nPerfect[0]);
					oper->AddVar(CString(_T("nOtherPerfect1")), str);
					str.Format(_T("%d"), viewp2->status.nGreat[0]);
					oper->AddVar(CString(_T("nOtherGreat1")), str);
					str.Format(_T("%d"), viewp2->status.nGood[0]);
					oper->AddVar(CString(_T("nOtherGood1")), str);
					str.Format(_T("%d"), viewp2->status.nBad[0]);
					oper->AddVar(CString(_T("nOtherBad1")), str);
					str.Format(_T("%d"), viewp2->status.nMiss[0]);
					oper->AddVar(CString(_T("nOtherMiss1")), str);
					str.Format(_T("%d"), viewp2->status.nMaxCombo[0]);
					oper->AddVar(CString(_T("nOtherMaxCombo1")), str);
					str.Format(_T("%d"), viewp2->status.nMissCombo[0]);
					oper->AddVar(CString(_T("nOtherMissCombo1")), str);

					str.Format(_T("%d"), viewp2->status.nPerfect[1]);
					oper->AddVar(CString(_T("nOtherPerfect2")), str);
					str.Format(_T("%d"), viewp2->status.nGreat[1]);
					oper->AddVar(CString(_T("nOtherGreat2")), str);
					str.Format(_T("%d"), viewp2->status.nGood[1]);
					oper->AddVar(CString(_T("nOtherGood2")), str);
					str.Format(_T("%d"), viewp2->status.nBad[1]);
					oper->AddVar(CString(_T("nOtherBad2")), str);
					str.Format(_T("%d"), viewp2->status.nMiss[1]);
					oper->AddVar(CString(_T("nOtherMiss2")), str);
					str.Format(_T("%d"), viewp2->status.nMaxCombo[1]);
					oper->AddVar(CString(_T("nOtherMaxCombo2")), str);
					str.Format(_T("%d"), viewp2->status.nMissCombo[1]);
					oper->AddVar(CString(_T("nOtherMissCombo2")), str);

					str.Format(_T("%d"), viewp2->status.nPerfect[2]);
					oper->AddVar(CString(_T("nOtherPerfect3")), str);
					str.Format(_T("%d"), viewp2->status.nGreat[2]);
					oper->AddVar(CString(_T("nOtherGreat3")), str);
					str.Format(_T("%d"), viewp2->status.nGood[2]);
					oper->AddVar(CString(_T("nOtherGood3")), str);
					str.Format(_T("%d"), viewp2->status.nBad[2]);
					oper->AddVar(CString(_T("nOtherBad3")), str);
					str.Format(_T("%d"), viewp2->status.nMiss[2]);
					oper->AddVar(CString(_T("nOtherMiss3")), str);
					str.Format(_T("%d"), viewp2->status.nMaxCombo[2]);
					oper->AddVar(CString(_T("nOtherMaxCombo3")), str);
					str.Format(_T("%d"), viewp2->status.nMissCombo[2]);
					oper->AddVar(CString(_T("nOtherMissCombo3")), str);

					str.Format(_T("%d"), viewp2->status.nScore);
					oper->AddVar(CString(_T("nOtherScore")), str);
					str.Format(_T("%d"), viewp2->status.nBSCorrect);
					oper->AddVar(CString(_T("nOtherBSCorrect")), str);
					str.Format(_T("%d"), viewp2->status.nBSIncorrect);
					oper->AddVar(CString(_T("nOtherBSIncorrect")), str);
					str.Format(_T("%d"), viewp2->status.nHearts);
					oper->AddVar(CString(_T("nOtherHearts")), str);
					str.Format(_T("%d"), viewp2->status.nPotions);
					oper->AddVar(CString(_T("nOtherPotions")), str);
					str.Format(_T("%d"), viewp2->status.nMines);
					oper->AddVar(CString(_T("nOtherMines")), str);
					str.Format(_T("%d"), viewp2->status.nVelocitys);
					oper->AddVar(CString(_T("nOtherVelocitys")), str);
					str.Format(_T("%d"), viewp2->status.nItems);
					oper->AddVar(CString(_T("nOtherItems")), str);
					str.Format(_T("%lf"), viewp2->status.dLife*100);
					oper->AddVar(CString(_T("dOtherLife")), str);
					str.Format(_T("%d"), nRankStepEval[0]);
					oper->AddVar(CString(_T("nOtherRank")), str);
				}
				for(int u = 0; u < nSizeCondSucess; u++)
				{
					int n = oper->Solve(strCondSucess[u])!=0?(u+1):0;
					if(n > bMissionSucess) bMissionSucess = n;
				}
				delete oper;
			}
		}
		nTimePlayer1StepEval = __int64(nDig+3)*TIME_CHANGE_NUMBER_STEPEVAL+TIME_START_SCORELOOP_STEPEVAL;
	}
	if(g_bPlayer[1])
	{
		int nDig = 0;
		if(nModeStepEval == 2) viewp2->status.nScore += viewp2->status.nBSCorrect*100000;
		CString str;
		str.Format(_T("%.3d"), viewp2->status.nPerfect[0]+viewp2->status.nPerfect[1]+viewp2->status.nPerfect[2]);
		nDig+=str.GetLength();
		str.Format(_T("%.3d"), viewp2->status.nGreat[0]+viewp2->status.nGreat[1]+viewp2->status.nGreat[2]);
		nDig+=str.GetLength();
		str.Format(_T("%.3d"), viewp2->status.nGood[0]+viewp2->status.nGood[1]+viewp2->status.nGood[2]);
		nDig+=str.GetLength();
		str.Format(_T("%.3d"), viewp2->status.nBad[0]+viewp2->status.nBad[1]+viewp2->status.nBad[2]);
		nDig+=str.GetLength();
		str.Format(_T("%.3d"), viewp2->status.nMiss[0]+viewp2->status.nMiss[1]+viewp2->status.nMiss[2]);
		nDig+=str.GetLength();
		if(nModeStepEval == 2) str.Format(_T("%.3d %.3d"), viewp2->status.nBSCorrect, viewp2->status.nBSIncorrect);
		else str.Format(_T("%.3d"), viewp2->status.nMaxCombo[0]);
		nDig+=str.GetLength();
		double dTotalPond = double(viewp2->status.nPerfect[0]+viewp2->status.nPerfect[1]+viewp2->status.nPerfect[2]+
						 viewp2->status.nGreat[0]+viewp2->status.nGreat[1]+viewp2->status.nGreat[2]+
						 viewp2->status.nGood[0]+viewp2->status.nGood[1]+viewp2->status.nGood[2]+
						 viewp2->status.nBad[0]+viewp2->status.nBad[1]+viewp2->status.nBad[2]+
						 viewp2->status.nMiss[0]+viewp2->status.nMiss[1]+viewp2->status.nMiss[2]);
		double dPond = viewp2->status.nPerfect[0]+viewp2->status.nPerfect[1]+viewp2->status.nPerfect[2]+
						 (viewp2->status.nGreat[0]+viewp2->status.nGreat[1]+viewp2->status.nGreat[2])*0.9+
						 (viewp2->status.nGood[0]+viewp2->status.nGood[1]+viewp2->status.nGood[2])*0.75+
						 (viewp2->status.nBad[0]+viewp2->status.nBad[1]+viewp2->status.nBad[2])*0.6;
		double dNote = dTotalPond == 0.0?1.0:dPond/dTotalPond;
		double dNoteBS = ((viewp2->status.nBSCorrect+viewp2->status.nBSIncorrect)==0)?
			(1.0):(double(viewp2->status.nBSCorrect)/double(viewp2->status.nBSCorrect+viewp2->status.nBSIncorrect));
		dNote = min(dNote,dNoteBS);
		if(dNote >= d[0])	// Rank A
			nRankStepEval[1] = 1;
		if(dNote < d[0] && dNote >= d[1])	// Rank B
			nRankStepEval[1] = 2;
		if(dNote < d[1] && dNote >= d[2])	// Rank C
			nRankStepEval[1] = 3;
		if(dNote < d[2] && dNote >= d[3])	// Rank D
			nRankStepEval[1] = 4;
		if(dNote < d[3])	// Rank F
			nRankStepEval[1] = 5;
		if(dNote >= 0.95 && !viewp2->status.nMiss[0])	// Rank S
			nRankStepEval[1] = 0;

		dNotesP[1] = dNote;

		if(nModeStepEval == 3) str.Format(_T("%.1lf%%"), dNotesP[1]*100.0);
		else str.Format(_T("%.3d"), viewp2->status.nScore);
		nDig+=str.GetLength();

		if(nModeStepEval == 1)
		{
			str.Format(_T("%.3d %.3d"), viewp2->status.nBSCorrect, viewp2->status.nBSIncorrect);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp2->status.nHearts);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp2->status.nPotions);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp2->status.nMines);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp2->status.nVelocitys);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp2->status.nItems);
			nDig+=str.GetLength();
			str.Format(_T("%.3d"), viewp2->status.nMaxMissComboTotal);
			nDig+=str.GetLength();
			str.Format(_T("%.1lf%%"), viewp2->status.dLife*100);
			nDig+=str.GetLength();

			if(!viewp2->m_bAutoPlay && viewp2->m_nCharacter == 0)
			{
                oper = new COperStr();
				str.Format(_T("%d"), viewp2->status.nPerfect[0]+viewp2->status.nPerfect[1]+viewp2->status.nPerfect[2]);
				oper->AddVar(CString(_T("nPerfect")), str);
				str.Format(_T("%d"), viewp2->status.nGreat[0]+viewp2->status.nGreat[1]+viewp2->status.nGreat[2]);
				oper->AddVar(CString(_T("nGreat")), str);
				str.Format(_T("%d"), viewp2->status.nGood[0]+viewp2->status.nGood[1]+viewp2->status.nGood[2]);
				oper->AddVar(CString(_T("nGood")), str);
				str.Format(_T("%d"), viewp2->status.nBad[0]+viewp2->status.nBad[1]+viewp2->status.nBad[2]);
				oper->AddVar(CString(_T("nBad")), str);
				str.Format(_T("%d"), viewp2->status.nMiss[0]+viewp2->status.nMiss[1]+viewp2->status.nMiss[2]);
				oper->AddVar(CString(_T("nMiss")), str);
				str.Format(_T("%d"), viewp2->status.nMaxComboTotal);
				oper->AddVar(CString(_T("nMaxCombo")), str);
				str.Format(_T("%d"), viewp2->status.nMaxMissComboTotal);
				oper->AddVar(CString(_T("nMissCombo")), str);

				str.Format(_T("%d"), viewp2->status.nPerfect[0]);
				oper->AddVar(CString(_T("nPerfect1")), str);
				str.Format(_T("%d"), viewp2->status.nGreat[0]);
				oper->AddVar(CString(_T("nGreat1")), str);
				str.Format(_T("%d"), viewp2->status.nGood[0]);
				oper->AddVar(CString(_T("nGood1")), str);
				str.Format(_T("%d"), viewp2->status.nBad[0]);
				oper->AddVar(CString(_T("nBad1")), str);
				str.Format(_T("%d"), viewp2->status.nMiss[0]);
				oper->AddVar(CString(_T("nMiss1")), str);
				str.Format(_T("%d"), viewp2->status.nMaxCombo[0]);
				oper->AddVar(CString(_T("nMaxCombo1")), str);
				str.Format(_T("%d"), viewp2->status.nMaxMissCombo[0]);
				oper->AddVar(CString(_T("nMissCombo1")), str);

				str.Format(_T("%d"), viewp2->status.nPerfect[1]);
				oper->AddVar(CString(_T("nPerfect2")), str);
				str.Format(_T("%d"), viewp2->status.nGreat[1]);
				oper->AddVar(CString(_T("nGreat2")), str);
				str.Format(_T("%d"), viewp2->status.nGood[1]);
				oper->AddVar(CString(_T("nGood2")), str);
				str.Format(_T("%d"), viewp2->status.nBad[1]);
				oper->AddVar(CString(_T("nBad2")), str);
				str.Format(_T("%d"), viewp2->status.nMiss[1]);
				oper->AddVar(CString(_T("nMiss2")), str);
				str.Format(_T("%d"), viewp2->status.nMaxCombo[1]);
				oper->AddVar(CString(_T("nMaxCombo2")), str);
				str.Format(_T("%d"), viewp2->status.nMaxMissCombo[1]);
				oper->AddVar(CString(_T("nMissCombo2")), str);

				str.Format(_T("%d"), viewp2->status.nPerfect[2]);
				oper->AddVar(CString(_T("nPerfect3")), str);
				str.Format(_T("%d"), viewp2->status.nGreat[2]);
				oper->AddVar(CString(_T("nGreat3")), str);
				str.Format(_T("%d"), viewp2->status.nGood[2]);
				oper->AddVar(CString(_T("nGood3")), str);
				str.Format(_T("%d"), viewp2->status.nBad[2]);
				oper->AddVar(CString(_T("nBad3")), str);
				str.Format(_T("%d"), viewp2->status.nMiss[2]);
				oper->AddVar(CString(_T("nMiss3")), str);
				str.Format(_T("%d"), viewp2->status.nMaxCombo[2]);
				oper->AddVar(CString(_T("nMaxCombo3")), str);
				str.Format(_T("%d"), viewp2->status.nMaxMissCombo[2]);
				oper->AddVar(CString(_T("nMissCombo3")), str);

				str.Format(_T("%d"), viewp2->status.nScore);
				oper->AddVar(CString(_T("nScore")), str);
				str.Format(_T("%d"), viewp2->status.nBSCorrect);
				oper->AddVar(CString(_T("nBSCorrect")), str);
				str.Format(_T("%d"), viewp2->status.nBSIncorrect);
				oper->AddVar(CString(_T("nBSIncorrect")), str);
				str.Format(_T("%d"), viewp2->status.nHearts);
				oper->AddVar(CString(_T("nHearts")), str);
				str.Format(_T("%d"), viewp2->status.nPotions);
				oper->AddVar(CString(_T("nPotions")), str);
				str.Format(_T("%d"), viewp2->status.nMines);
				oper->AddVar(CString(_T("nMines")), str);
				str.Format(_T("%d"), viewp2->status.nVelocitys);
				oper->AddVar(CString(_T("nVelocitys")), str);
				str.Format(_T("%d"), viewp2->status.nItems);
				oper->AddVar(CString(_T("nItems")), str);
				str.Format(_T("%lf"), viewp2->status.dLife*100);
				oper->AddVar(CString(_T("dLife")), str);
				str.Format(_T("%d"), nRankStepEval[1]);
				oper->AddVar(CString(_T("nRank")), str);
				if(g_bPlayer[0])
				{
					str.Format(_T("%d"), viewp1->status.nPerfect[0]+viewp1->status.nPerfect[1]+viewp1->status.nPerfect[2]);
					oper->AddVar(CString(_T("nOtherPerfect")), str);
					str.Format(_T("%d"), viewp1->status.nGreat[0]+viewp1->status.nGreat[1]+viewp1->status.nGreat[2]);
					oper->AddVar(CString(_T("nOtherGreat")), str);
					str.Format(_T("%d"), viewp1->status.nGood[0]+viewp1->status.nGood[1]+viewp1->status.nGood[2]);
					oper->AddVar(CString(_T("nOtherGood")), str);
					str.Format(_T("%d"), viewp1->status.nBad[0]+viewp1->status.nBad[1]+viewp1->status.nBad[2]);
					oper->AddVar(CString(_T("nOtherBad")), str);
					str.Format(_T("%d"), viewp1->status.nMiss[0]+viewp1->status.nMiss[1]+viewp1->status.nMiss[2]);
					oper->AddVar(CString(_T("nOtherMiss")), str);
					str.Format(_T("%d"), viewp1->status.nMaxComboTotal);
					oper->AddVar(CString(_T("nOtherMaxCombo")), str);
					str.Format(_T("%d"), viewp1->status.nMaxMissComboTotal);
					oper->AddVar(CString(_T("nOtherMissCombo")), str);

					str.Format(_T("%d"), viewp1->status.nPerfect[0]);
					oper->AddVar(CString(_T("nOtherPerfect1")), str);
					str.Format(_T("%d"), viewp1->status.nGreat[0]);
					oper->AddVar(CString(_T("nOtherGreat1")), str);
					str.Format(_T("%d"), viewp1->status.nGood[0]);
					oper->AddVar(CString(_T("nOtherGood1")), str);
					str.Format(_T("%d"), viewp1->status.nBad[0]);
					oper->AddVar(CString(_T("nOtherBad1")), str);
					str.Format(_T("%d"), viewp1->status.nMiss[0]);
					oper->AddVar(CString(_T("nOtherMiss1")), str);
					str.Format(_T("%d"), viewp1->status.nMaxCombo[0]);
					oper->AddVar(CString(_T("nOtherMaxCombo1")), str);
					str.Format(_T("%d"), viewp1->status.nMaxMissCombo[0]);
					oper->AddVar(CString(_T("nOtherMissCombo1")), str);

					str.Format(_T("%d"), viewp1->status.nPerfect[1]);
					oper->AddVar(CString(_T("nOtherPerfect2")), str);
					str.Format(_T("%d"), viewp1->status.nGreat[1]);
					oper->AddVar(CString(_T("nOtherGreat2")), str);
					str.Format(_T("%d"), viewp1->status.nGood[1]);
					oper->AddVar(CString(_T("nOtherGood2")), str);
					str.Format(_T("%d"), viewp1->status.nBad[1]);
					oper->AddVar(CString(_T("nOtherBad2")), str);
					str.Format(_T("%d"), viewp1->status.nMiss[1]);
					oper->AddVar(CString(_T("nOtherMiss2")), str);
					str.Format(_T("%d"), viewp1->status.nMaxCombo[1]);
					oper->AddVar(CString(_T("nOtherMaxCombo2")), str);
					str.Format(_T("%d"), viewp1->status.nMaxMissCombo[1]);
					oper->AddVar(CString(_T("nOtherMissCombo2")), str);

					str.Format(_T("%d"), viewp1->status.nPerfect[2]);
					oper->AddVar(CString(_T("nOtherPerfect3")), str);
					str.Format(_T("%d"), viewp1->status.nGreat[2]);
					oper->AddVar(CString(_T("nOtherGreat3")), str);
					str.Format(_T("%d"), viewp1->status.nGood[2]);
					oper->AddVar(CString(_T("nOtherGood3")), str);
					str.Format(_T("%d"), viewp1->status.nBad[2]);
					oper->AddVar(CString(_T("nOtherBad3")), str);
					str.Format(_T("%d"), viewp1->status.nMiss[2]);
					oper->AddVar(CString(_T("nOtherMiss3")), str);
					str.Format(_T("%d"), viewp1->status.nMaxCombo[2]);
					oper->AddVar(CString(_T("nOtherMaxCombo3")), str);
					str.Format(_T("%d"), viewp1->status.nMaxMissCombo[2]);
					oper->AddVar(CString(_T("nOtherMissCombo3")), str);

					str.Format(_T("%d"), viewp1->status.nScore);
					oper->AddVar(CString(_T("nOtherScore")), str);
					str.Format(_T("%d"), viewp1->status.nBSCorrect);
					oper->AddVar(CString(_T("nOtherBSCorrect")), str);
					str.Format(_T("%d"), viewp1->status.nBSIncorrect);
					oper->AddVar(CString(_T("nOtherBSIncorrect")), str);
					str.Format(_T("%d"), viewp1->status.nHearts);
					oper->AddVar(CString(_T("nOtherHearts")), str);
					str.Format(_T("%d"), viewp1->status.nPotions);
					oper->AddVar(CString(_T("nOtherPotions")), str);
					str.Format(_T("%d"), viewp1->status.nMines);
					oper->AddVar(CString(_T("nOtherMines")), str);
					str.Format(_T("%d"), viewp1->status.nVelocitys);
					oper->AddVar(CString(_T("nOtherVelocitys")), str);
					str.Format(_T("%d"), viewp1->status.nItems);
					oper->AddVar(CString(_T("nOtherItems")), str);
					str.Format(_T("%lf"), viewp1->status.dLife*100);
					oper->AddVar(CString(_T("dOtherLife")), str);
					str.Format(_T("%d"), nRankStepEval[0]);
					oper->AddVar(CString(_T("nOtherRank")), str);
				}
				for(int u = 0; u < nSizeCondSucess; u++)
				{
					int n = oper->Solve(strCondSucess[u])!=0?(u+1):0;
					if(n > bMissionSucess) bMissionSucess = n;
				}
				delete oper;
			}
		}
		nTimePlayer2StepEval = __int64(nDig+7)*TIME_CHANGE_NUMBER_STEPEVAL+TIME_START_SCORELOOP_STEPEVAL;
	}
	nTimeShowRankEval = max(nTimePlayer1StepEval, nTimePlayer2StepEval);
	nTimeOutStepEval = nTimeShowRankEval+TIME_RUN_RANK_STEPEVAL;

	dwLastTick = dwTick = SDL_GetTicks();
}

int FrameStepEval()
{
	evalres->Update(dwWait);
	nCountStepEval += dwTick-dwLastTick;
	double dApt = 0.0;
	if(movie_bgeval->bLoad)
	{
		movie_bgeval->Update(dwWait);
		movie_bgeval->GrabAVIFrame();
		glBindTexture(GL_TEXTURE_2D, movie_bgeval->tex);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
		glEnd();
	}
#define TIME_EXIT_STEPEVAL 600
#define TIME_BLACK_BGA_STEPEVAL 500
	if(nCountStepEval >= 0 && nCountStepEval < TIME_BLACK_BGA_STEPEVAL)
	{
		dApt = double(nCountStepEval)/TIME_BLACK_BGA_STEPEVAL;
		glColor4d(0.0, 0.0, 0.0, 1.0-dApt);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
	}
#define TIME_EBLACK_BGA_STEPEVAL 500
	if(nCountStepEval >= (nTimeOutStepEval+TIME_EXIT_STEPEVAL-TIME_EBLACK_BGA_STEPEVAL) && nCountStepEval <= (nTimeOutStepEval+TIME_EXIT_STEPEVAL))
	{
		dApt = double(nCountStepEval-nTimeOutStepEval-TIME_EXIT_STEPEVAL+TIME_EBLACK_BGA_STEPEVAL)/TIME_EBLACK_BGA_STEPEVAL;
		glColor4d(0.0, 0.0, 0.0, dApt);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
	}

	// ** Draw Up And down
	if(nCountStepEval > TIME_START_INTRO_STEPEVAL && nCountStepEval < (TIME_TRANSI_INTRO_STEPEVAL+TIME_START_INTRO_STEPEVAL))
	{
		dApt = (double(nCountStepEval)-TIME_START_INTRO_STEPEVAL)/TIME_TRANSI_INTRO_STEPEVAL;
		glBindTexture(GL_TEXTURE_2D, evalres->skin[0]->GetCurrentTex());
		glPushMatrix();
		glScaled(1.0+0.5*(1.0-dApt), 1.0+0.5*(1.0-dApt), 1.0);
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
		glPopMatrix();
	}
	if(nCountStepEval >= (TIME_TRANSI_INTRO_STEPEVAL+TIME_START_INTRO_STEPEVAL) &&
		nCountStepEval < nTimeOutStepEval)
	{
		glBindTexture(GL_TEXTURE_2D, evalres->skin[0]->GetCurrentTex());
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
	}
	if(nCountStepEval >= nTimeOutStepEval && nCountStepEval < (nTimeOutStepEval+TIME_TRANSI_INTRO_STEPEVAL))
	{
		dApt = double(nCountStepEval-nTimeOutStepEval)/TIME_TRANSI_INTRO_STEPEVAL;
		glBindTexture(GL_TEXTURE_2D, evalres->skin[0]->GetCurrentTex());
		glPushMatrix();
		glScaled(1.0+0.5*(dApt), 1.0+0.5*(dApt), 1.0);
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
		glPopMatrix();
	}

	// ** Dibujo de las barras
	double dSpaceBarStepEval = 5.0;
	double dSizeBarStepEval = 4.8;
	int nSizeBars = 7;	// Por ahora
	if(nModeStepEval == 1 && !(g_bPlayer[0] && g_bPlayer[1]))	// Adventure Mod
	{
		dSpaceBarStepEval += dSizeBarStepEval/4;
		nSizeBars = 8;
	}
	double dTotalOff = double(nSizeBars)*dSpaceBarStepEval/2 - dSpaceBarStepEval/4;
	if(nCountStepEval >= TIME_START_TBAR_STEPEVAL && nCountStepEval < nTimeOutStepEval)
	{
		__int64 nTimeTbar = nCountStepEval - TIME_START_TBAR_STEPEVAL;
		for(int x = 0; x < nSizeBars; x++)
		{
			__int64 nTimeStart = TIME_TRANSI_TBAR_STEPEVAL/nSizeBars*x;
			__int64 nTimeEnd = TIME_TRANSI_TBAR_STEPEVAL/nSizeBars*(x+1);
			if(nTimeTbar >= nTimeStart && nTimeTbar < nTimeEnd)
			{
				dApt = double(nTimeTbar-nTimeStart)/double(nTimeEnd-nTimeStart);
				glPushMatrix();
				double dy = dTotalOff-dSpaceBarStepEval*double(x);
				glTranslated(0.0, dy, 0.0);
				glScaled(1.0, dApt, 1.0);
				glBindTexture(GL_TEXTURE_2D, evalres->skin[5]->GetCurrentTex());
				glColor4d(1.0, 1.0, 1.0, dApt);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  dSizeBarStepEval/2, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  dSizeBarStepEval/2, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -dSizeBarStepEval/2, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -dSizeBarStepEval/2, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(nTimeTbar >= nTimeEnd)
			{
				glPushMatrix();
				double dy = dTotalOff-dSpaceBarStepEval*double(x);
				glTranslated(0.0, dy, 0.0);
				glBindTexture(GL_TEXTURE_2D, evalres->skin[5]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  dSizeBarStepEval/2, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  dSizeBarStepEval/2, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -dSizeBarStepEval/2, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -dSizeBarStepEval/2, -50.0);
				glEnd();
				glPopMatrix();
			}
		}
	}
	if(nCountStepEval >= nTimeOutStepEval)
	{
		__int64 nTimeTbar = nCountStepEval - TIME_START_TBAR_STEPEVAL;
		for(int x = 0; x < nSizeBars; x++)
		{
			__int64 nTimeStart = TIME_TRANSI_TBAR_STEPEVAL/nSizeBars*x;
			__int64 nTimeEnd = TIME_TRANSI_TBAR_STEPEVAL/nSizeBars*(x+1);
			if(nTimeTbar >= nTimeStart && nTimeTbar < nTimeEnd)
			{
				dApt = double(nTimeTbar-nTimeStart)/double(nTimeEnd-nTimeStart);
				glPushMatrix();
				double dy = dTotalOff-dSpaceBarStepEval*double(x);
				glTranslated(0.0, dy, 0.0);
				glScaled(1.0, 1.0-dApt, 1.0);
				glBindTexture(GL_TEXTURE_2D, evalres->skin[5]->GetCurrentTex());
				glColor4d(1.0, 1.0, 1.0, 1-dApt);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  dSizeBarStepEval, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  dSizeBarStepEval, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -dSizeBarStepEval, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -dSizeBarStepEval, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(nTimeTbar < nTimeStart)
			{
				glPushMatrix();
				double dy = dTotalOff-dSpaceBarStepEval*double(x);
				glTranslated(0.0, dy, 0.0);
				glBindTexture(GL_TEXTURE_2D, evalres->skin[5]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  dSizeBarStepEval, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  dSizeBarStepEval, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -dSizeBarStepEval, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -dSizeBarStepEval, -50.0);
				glEnd();
				glPopMatrix();
			}
		}
	}

	// ** Dibujo del Orbe
	if(nCountStepEval < TIME_TRANSI_ORBE_STEPEVAL)
	{
		dApt = abs(double(nCountStepEval)/TIME_TRANSI_ORBE_STEPEVAL-0.5)*2;
		double dApt2 = double(nCountStepEval)/TIME_TRANSI_ORBE_STEPEVAL;
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, evalres->skin[3]->GetCurrentTex());
		glScaled(1.0+0.3*(1.0-dApt2), 1.0+0.3*(1.0-dApt2), 1.0);
		glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-22.0,  22.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 22.0,  22.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 22.0, -22.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-22.0, -22.0, -50.0);
		glEnd();
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glPopMatrix();
	}
	if(nCountStepEval > TIME_START_ORBE_STEPEVAL && nCountStepEval < (TIME_TRANSI_ORBE_STEPEVAL+TIME_START_ORBE_STEPEVAL))
	{
		dApt = (double(nCountStepEval)-TIME_START_ORBE_STEPEVAL)/TIME_TRANSI_ORBE_STEPEVAL;
		glBindTexture(GL_TEXTURE_2D, evalres->skin[2]->GetCurrentTex());
		glColor4d(1.0, 1.0, 1.0, dApt);
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-22.0,  22.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 22.0,  22.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 22.0, -22.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-22.0, -22.0, -50.0);
		glEnd();
		glColor4d(1.0, 1.0, 1.0, 1.0);
	}
	if(nCountStepEval >= (TIME_TRANSI_ORBE_STEPEVAL+TIME_START_ORBE_STEPEVAL) &&
		nCountStepEval < nTimeOutStepEval)
	{
		glBindTexture(GL_TEXTURE_2D, evalres->skin[2]->GetCurrentTex());
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-22.0,  22.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 22.0,  22.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 22.0, -22.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-22.0, -22.0, -50.0);
		glEnd();
		if(nCountStepEval < (TIME_TRANSI_ORBE_STEPEVAL+TIME_START_ORBE_STEPEVAL+TIME_GLOUTH_ORBE_STEPEVAL))
		{
			dApt = (double(nCountStepEval-TIME_START_INTRO_STEPEVAL-TIME_TRANSI_ORBE_STEPEVAL))/TIME_GLOUTH_ORBE_STEPEVAL;
			glBindTexture(GL_TEXTURE_2D, evalres->skin[4]->GetCurrentTex());
			glPushMatrix();
			glScaled(1.0+0.2*(dApt), 1.0+0.2*(dApt), 1.0);
			glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
			glBegin(GL_QUADS);
  			glTexCoord2d(0.0, 1.0); glVertex3d(-22.0,  22.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 22.0,  22.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 22.0, -22.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-22.0, -22.0, -50.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glPopMatrix();
		}
	}
	if(nCountStepEval >= nTimeOutStepEval && nCountStepEval < (nTimeOutStepEval+TIME_TRANSI_ORBE_STEPEVAL))
	{
		dApt = (double(nCountStepEval-nTimeOutStepEval))/TIME_TRANSI_ORBE_STEPEVAL;
		glBindTexture(GL_TEXTURE_2D, evalres->skin[2]->GetCurrentTex());
		glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-22.0,  22.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 22.0,  22.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 22.0, -22.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-22.0, -22.0, -50.0);
		glEnd();
		glColor4d(1.0, 1.0, 1.0, 1.0);
	}

	// Dibujo de la guia numerica (Perfect, Great...)
#define TIME_START_EVALGUIDE_GAME1 500
#define TIME_TRANSI_EVALGUIDE_GAME1 500
	if(nCountStepEval >= TIME_START_EVALGUIDE_GAME1 && nCountStepEval < nTimeOutStepEval)
	{
		__int64 nTimeTbar = nCountStepEval - TIME_START_EVALGUIDE_GAME1;
		int nOrderTex[] = {0,1,2,3,4,5,6,14,7,8,9,10,11,12,13,15};
		if(nModeStepEval == 2)
		{
			nOrderTex[5] = 14;
			nOrderTex[7] = 5;
		}
		for(int x = 0; x < nSizeBars; x++)
		{
			__int64 nTimeStart = TIME_TRANSI_EVALGUIDE_GAME1/nSizeBars*x;
			__int64 nTimeEnd = TIME_TRANSI_EVALGUIDE_GAME1/nSizeBars*(x+1);
			if(!(nModeStepEval == 1 && !(g_bPlayer[0] && g_bPlayer[1])))
			{
				if(nTimeTbar >= nTimeStart && nTimeTbar < nTimeEnd)
				{
					dApt = double(nTimeTbar-nTimeStart)/double(nTimeEnd-nTimeStart);
					glPushMatrix();
					double dy = dTotalOff-dSpaceBarStepEval*double(x);
					glTranslated(0.0, dy, 0.0);
					glScaled(1.0, dApt, 1.0);
					int y = nOrderTex[x];
					glBindTexture(GL_TEXTURE_2D, evalres->skin[15+y]->GetCurrentTex());
					glColor4d(1.0, 1.0, 1.0, dApt);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-dSizeBarStepEval*4,  dSizeBarStepEval/2, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( dSizeBarStepEval*4,  dSizeBarStepEval/2, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( dSizeBarStepEval*4, -dSizeBarStepEval/2, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-dSizeBarStepEval*4, -dSizeBarStepEval/2, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();
				}
				if(nTimeTbar >= nTimeEnd)
				{
					glPushMatrix();
					double dy = dTotalOff-dSpaceBarStepEval*double(x);
					glTranslated(0.0, dy, 0.0);
					int y = nOrderTex[x];
					glBindTexture(GL_TEXTURE_2D, evalres->skin[15+y]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-dSizeBarStepEval*4,  dSizeBarStepEval/2, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( dSizeBarStepEval*4,  dSizeBarStepEval/2, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( dSizeBarStepEval*4, -dSizeBarStepEval/2, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-dSizeBarStepEval*4, -dSizeBarStepEval/2, -50.0);
					glEnd();
					glPopMatrix();
				}
			}
			else
			{
				if(nTimeTbar >= nTimeStart && nTimeTbar < nTimeEnd)
				{
					dApt = double(nTimeTbar-nTimeStart)/double(nTimeEnd-nTimeStart);
					glPushMatrix();
					glColor4d(1.0, 1.0, 1.0, dApt);
					double dy = dTotalOff-dSpaceBarStepEval*double(x);
					glTranslated(-20.0, dy+dSpaceBarStepEval/2, 0.0);
					glScaled(1.0, dApt, 1.0);
					int y = nOrderTex[x];
					glBindTexture(GL_TEXTURE_2D, evalres->skin[15+y]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-dSizeBarStepEval*2,  dSizeBarStepEval/4, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( dSizeBarStepEval*2,  dSizeBarStepEval/4, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( dSizeBarStepEval*2, -dSizeBarStepEval/4, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-dSizeBarStepEval*2, -dSizeBarStepEval/4, -50.0);
					glEnd();
					glLoadIdentity();
					glTranslated(20.0, dy+dSpaceBarStepEval/2, 0.0);
					glScaled(1.0, dApt, 1.0);
					y = nOrderTex[x+8];
					glBindTexture(GL_TEXTURE_2D, evalres->skin[15+y]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-dSizeBarStepEval*2,  dSizeBarStepEval/4, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( dSizeBarStepEval*2,  dSizeBarStepEval/4, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( dSizeBarStepEval*2, -dSizeBarStepEval/4, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-dSizeBarStepEval*2, -dSizeBarStepEval/4, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();
				}
				if(nTimeTbar >= nTimeEnd)
				{
					glPushMatrix();
					double dy = dTotalOff-dSpaceBarStepEval*double(x);
					glTranslated(-20.0, dy+dSpaceBarStepEval/2, 0.0);
					int y = nOrderTex[x];
					glBindTexture(GL_TEXTURE_2D, evalres->skin[15+y]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-dSizeBarStepEval*2,  dSizeBarStepEval/4, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( dSizeBarStepEval*2,  dSizeBarStepEval/4, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( dSizeBarStepEval*2, -dSizeBarStepEval/4, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-dSizeBarStepEval*2, -dSizeBarStepEval/4, -50.0);
					glEnd();
					glLoadIdentity();
					glTranslated(20.0, dy+dSpaceBarStepEval/2, 0.0);
					y = nOrderTex[x+8];
					glBindTexture(GL_TEXTURE_2D, evalres->skin[15+y]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-dSizeBarStepEval*2,  dSizeBarStepEval/4, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( dSizeBarStepEval*2,  dSizeBarStepEval/4, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( dSizeBarStepEval*2, -dSizeBarStepEval/4, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-dSizeBarStepEval*2, -dSizeBarStepEval/4, -50.0);
					glEnd();
					glPopMatrix();
				}
			}
		}
	}

	// ** Dibujo de los numeros
	static bool bPlayClick = false;
	double dSizeNumberx = 2.0;
	double dSizeNumbery = 3.0;
	if(nCountStepEval >= TIME_START_SCORELOOP_STEPEVAL && nCountStepEval < nTimeOutStepEval)
	{
#define SIZE_SHOW_STEPEVAL 17
		CString strShow[SIZE_SHOW_STEPEVAL];
		for(int x = 0; x < 2; x++) if(g_bPlayer[x])
		{
			switch(x)
			{
			case 0:
				strShow[0].Format(_T("%.3d"), viewp1->status.nPerfect[0]+viewp1->status.nPerfect[1]+viewp1->status.nPerfect[2]);
				strShow[1].Format(_T("%.3d"), viewp1->status.nGreat[0]+viewp1->status.nGreat[1]+viewp1->status.nGreat[2]);
				strShow[2].Format(_T("%.3d"), viewp1->status.nGood[0]+viewp1->status.nGood[1]+viewp1->status.nGood[2]);
				strShow[3].Format(_T("%.3d"), viewp1->status.nBad[0]+viewp1->status.nBad[1]+viewp1->status.nBad[2]);
				strShow[4].Format(_T("%.3d"), viewp1->status.nMiss[0]+viewp1->status.nMiss[1]+viewp1->status.nMiss[2]);
				strShow[5].Format(_T("%.3d"), viewp1->status.nMaxCombo[0]);
				strShow[6].Format(_T("%.3d"), viewp1->status.nScore);
				if(nModeStepEval == 1 && !(g_bPlayer[0] && g_bPlayer[1]))
				{
					strShow[7].Format(_T("%.3d %.3d"), viewp1->status.nBSCorrect, viewp1->status.nBSIncorrect);
					strShow[8].Format(_T("%.3d"), viewp1->status.nHearts);
					strShow[9].Format(_T("%.3d"), viewp1->status.nPotions);
					strShow[10].Format(_T("%.3d"), viewp1->status.nMines);
					strShow[11].Format(_T("%.3d"), viewp1->status.nVelocitys);
					strShow[12].Format(_T("%.3d"), viewp1->status.nItems);
					strShow[13].Format(_T("%.3d"), viewp1->status.nMaxMissCombo[0]);
					strShow[14].Format(_T("%.1lf%%"), viewp1->status.dLife*100);
				}
				if(nModeStepEval == 2)
				{
					strShow[5].Format(_T("%.3d %.3d"), viewp1->status.nBSCorrect, viewp1->status.nBSIncorrect);
				}
				if(nModeStepEval == 3)
				{
					strShow[6].Format(_T("%.1lf%%"), dNotesP[x]*100.0);
				}
				break;
			default:
				strShow[0].Format(_T("%.3d"), viewp2->status.nPerfect[0]+viewp2->status.nPerfect[1]+viewp2->status.nPerfect[2]);
				strShow[1].Format(_T("%.3d"), viewp2->status.nGreat[0]+viewp2->status.nGreat[1]+viewp2->status.nGreat[2]);
				strShow[2].Format(_T("%.3d"), viewp2->status.nGood[0]+viewp2->status.nGood[1]+viewp2->status.nGood[2]);
				strShow[3].Format(_T("%.3d"), viewp2->status.nBad[0]+viewp2->status.nBad[1]+viewp2->status.nBad[2]);
				strShow[4].Format(_T("%.3d"), viewp2->status.nMiss[0]+viewp2->status.nMiss[1]+viewp2->status.nMiss[2]);
				strShow[5].Format(_T("%.3d"), viewp2->status.nMaxCombo[0]);
				strShow[6].Format(_T("%.3d"), viewp2->status.nScore);
				if(nModeStepEval == 1 && !(g_bPlayer[0] && g_bPlayer[1]))
				{
					strShow[7].Format(_T("%.3d %.3d"), viewp2->status.nBSCorrect, viewp2->status.nBSIncorrect);
					strShow[8].Format(_T("%.3d"), viewp2->status.nHearts);
					strShow[9].Format(_T("%.3d"), viewp2->status.nPotions);
					strShow[10].Format(_T("%.3d"), viewp2->status.nMines);
					strShow[11].Format(_T("%.3d"), viewp2->status.nVelocitys);
					strShow[12].Format(_T("%.3d"), viewp2->status.nItems);
					strShow[13].Format(_T("%.3d"), viewp2->status.nMaxMissCombo[0]);
					strShow[14].Format(_T("%.1lf%%"), viewp2->status.dLife*100);
				}
				if(nModeStepEval == 2)
				{
					strShow[5].Format(_T("%.3d %.3d"), viewp2->status.nBSCorrect, viewp2->status.nBSIncorrect);
				}
				if(nModeStepEval == 3)
				{
					strShow[6].Format(_T("%.1lf%%"), dNotesP[x]*100.0);
				}
				break;
			}
			__int64 nCountBeginShow = nCountStepEval-TIME_START_SCORELOOP_STEPEVAL;
			int nTotalSize = 0;
			int nLimitScan = nSizeBars;		// Varia
			if(nModeStepEval == 1 && !(g_bPlayer[0] && g_bPlayer[1])) nLimitScan = 16;
			for(int y = 0; y < nLimitScan; y++)
				nTotalSize += strShow[y].GetLength();
			int nSizeShow = int(nCountBeginShow/TIME_CHANGE_NUMBER_STEPEVAL);
			int nSize = min(nTotalSize, nSizeShow);
			double dOffx = 0;
			if(!bPlayClick && nCountBeginShow >= 0 && nCountStepEval < nTimeShowRankEval)
			{
				FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, sound_click, 0, &channels[9]);
				bPlayClick = true;
			}
			if(bPlayClick && nCountBeginShow >= 0 && nCountStepEval >= nTimeShowRankEval)
			{
				FMOD_Channel_Stop(channels[9]);
				bPlayClick = false;
			}
			for(int y = 0; y < nSize; y++)
			{
				int nIndex = 0;
				int nString = 0;
				int nStride = 0;
				int nColumn = 0;
				CString str;
				for(int z = 0; z < nLimitScan; z++)
				{
					if((y - nStride) < strShow[z].GetLength())
					{
						nString = z;
						nIndex = (y - nStride);
						break;
					}
					else nStride += strShow[z].GetLength();
				}
				nColumn = nString;
				if(nModeStepEval == 1 && !(g_bPlayer[0] && g_bPlayer[1]) && nColumn >= 8) nColumn -= 8;
				int nId = 0;
				if(nCountBeginShow >= (y*TIME_CHANGE_NUMBER_STEPEVAL) && nCountBeginShow < ((y+3)*TIME_CHANGE_NUMBER_STEPEVAL))
				{
					nId = rand()%10;
				}
				if(nCountBeginShow >= ((y+3)*TIME_CHANGE_NUMBER_STEPEVAL))
				{
					nId = strShow[nString].GetAt(nIndex)-_T('0');
				}
				if(nId != (_T(' ')-_T('0')))
				{
					if(nId == (_T('.')-_T('0'))) nId = 10;
					if(nId == (_T('%')-_T('0'))) nId = 11;
					glPushMatrix();
					double dx = (!x)?(-36.0+double(nIndex)*dSizeNumberx):(36.0-(strShow[nString].GetLength()-nIndex)*dSizeNumberx);
					if(nModeStepEval == 1 && !(g_bPlayer[0] && g_bPlayer[1]))
					{
						if(nColumn == nString)	// Lado Izquierdo
						{
							dx = -20.0-double(strShow[nString].GetLength())*dSizeNumberx/2+double(nIndex)*dSizeNumberx;
						}
						else	// Lado Derecho
						{
							dx = 20.0-double(strShow[nString].GetLength())*dSizeNumberx/2+double(nIndex)*dSizeNumberx;
						}
					}
					glTranslated(dx, dTotalOff-double(nColumn)*dSpaceBarStepEval, 0.0);
					glBindTexture(GL_TEXTURE_2D, evalres->skin[6]->texture[min(nId, evalres->skin[6]->m_nSize-1)]);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-dSizeNumberx/2,  dSizeNumbery/2, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( dSizeNumberx/2,  dSizeNumbery/2, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( dSizeNumberx/2, -dSizeNumbery/2, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-dSizeNumberx/2, -dSizeNumbery/2, -50.0);
					glEnd();
					glPopMatrix();
				}
			}
		}
	}

	static bool bPlayBoom = false;
	static bool bPlayMission = false;
	for(int x = 0; x < 2; x++) if(g_bPlayer[x])
	{
		if(nCountStepEval >= (nTimeShowRankEval+TIME_START_RANK_STEPEVAL) && nCountStepEval < (nTimeShowRankEval+TIME_START_RANK_STEPEVAL+TIME_TRANSI_RANK_STEPEVAL))
		{
			bPlayBoom = false;
			bPlayMission = false;
			dApt = (double(nCountStepEval-nTimeShowRankEval-TIME_START_RANK_STEPEVAL))/TIME_TRANSI_RANK_STEPEVAL;
			glPushMatrix();
			glTranslated((nModeStepEval==1 && !(g_bPlayer[0] && g_bPlayer[1]))?0.0:(x?30.0:-30.0), 0.0, 0.0);
			glScaled(1.0+0.2*(1.0-dApt), 1.0+0.2*(1.0-dApt), 1.0);
			glBindTexture(GL_TEXTURE_2D, evalres->skin[nRankStepEval[x]+7]->GetCurrentTex());
			glColor4d(1.0, 1.0, 1.0, dApt);
			glBegin(GL_QUADS);
  			glTexCoord2d(0.0, 1.0); glVertex3d(-18.0,  18.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 18.0,  18.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 18.0, -18.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-18.0, -18.0, -50.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glPopMatrix();
		}
#define TIME_START_RANKMISSION_STEPEVAL 3000
#define TIME_TRANSI_RANKMISSION_STEPEVAL 300
#define TIME_GLOUTH_RANKMISSION_STEPEVAL 300
		if(nCountStepEval >= (nTimeShowRankEval+TIME_START_RANK_STEPEVAL+TIME_TRANSI_RANK_STEPEVAL) && nCountStepEval < nTimeOutStepEval)
		{
			glPushMatrix();
			glTranslated((nModeStepEval==1 && !(g_bPlayer[0] && g_bPlayer[1]))?0.0:(x?30.0:-30.0), 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, evalres->skin[nRankStepEval[x]+7]->GetCurrentTex());
			glBegin(GL_QUADS);
  			glTexCoord2d(0.0, 1.0); glVertex3d(-18.0,  18.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 18.0,  18.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 18.0, -18.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-18.0, -18.0, -50.0);
			glEnd();
			if(!bPlayBoom)
			{
				FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)10, sound_boom, 0, &channels[10]);
				switch(min(np[0]?nRankStepEval[0]:7, np[1]?nRankStepEval[1]:7))
				{
				case 0:
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_ranks1, 0, &channels[11]);
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)12, sound_ranks2, 0, &channels[12]);
					break;
				case 1:
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_ranka1, 0, &channels[11]);
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)12, sound_ranka2, 0, &channels[12]);
					break;
				case 2:
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_rankb1, 0, &channels[11]);
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)12, sound_rankb2, 0, &channels[12]);
					break;
				case 3:
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_rankc1, 0, &channels[11]);
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)12, sound_rankc2, 0, &channels[12]);
					break;
				case 4:
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_rankd1, 0, &channels[11]);
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)12, sound_rankd2, 0, &channels[12]);
					break;
				case 5:
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_rankf1, 0, &channels[11]);
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)12, sound_rankf2, 0, &channels[12]);
					break;
				}
				bPlayBoom = true;
			}
			if(nCountStepEval < (nTimeShowRankEval+TIME_START_RANK_STEPEVAL+TIME_TRANSI_RANK_STEPEVAL+TIME_GLOUTH_RANK_STEPEVAL))
			{
				dApt = (double(nCountStepEval-nTimeShowRankEval-TIME_START_RANK_STEPEVAL-TIME_TRANSI_RANK_STEPEVAL))/TIME_GLOUTH_RANK_STEPEVAL;
				glLoadIdentity();
				glTranslated((nModeStepEval==1 && !(g_bPlayer[0] && g_bPlayer[1]))?0.0:(x?30.0:-30.0), 0.0, 0.0);
				glScaled(1.0+0.5*(dApt), 1.0+0.5*(dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, evalres->skin[nRankStepEval[x]+7]->GetCurrentTex());
				glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-18.0,  18.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 18.0,  18.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 18.0, -18.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-18.0, -18.0, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
			}
			glPopMatrix();
		}
		if(nModeStepEval == 1)
		{
			if(nCountStepEval >= (nTimeShowRankEval+TIME_START_RANKMISSION_STEPEVAL) && nCountStepEval < (nTimeShowRankEval+TIME_START_RANKMISSION_STEPEVAL+TIME_TRANSI_RANKMISSION_STEPEVAL))
			{
				bPlayMission = false;
				dApt = (double(nCountStepEval-nTimeShowRankEval-TIME_START_RANKMISSION_STEPEVAL))/TIME_TRANSI_RANKMISSION_STEPEVAL;
				glPushMatrix();
				glScaled(1.0+0.2*(1.0-dApt), 1.0+0.2*(1.0-dApt), 1.0);
				glBindTexture(GL_TEXTURE_2D, evalres->skin[(bMissionSucess?0:1)+31]->GetCurrentTex());
				glColor4d(1.0, 1.0, 1.0, dApt);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-27.75,  15.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 27.75,  15.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 27.75, -15.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-27.75, -15.0, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(nCountStepEval >= (nTimeShowRankEval+TIME_START_RANKMISSION_STEPEVAL+TIME_TRANSI_RANKMISSION_STEPEVAL) && nCountStepEval < nTimeOutStepEval)
			{
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, evalres->skin[(bMissionSucess?0:1)+31]->GetCurrentTex());
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-27.75,  15.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 27.75,  15.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 27.75, -15.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-27.75, -15.0, -50.0);
				glEnd();
				if(!bPlayMission)
				{
					if(bMissionSucess) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)13, sound_ranksucess, 0, &channels[13]);
					else FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)13, sound_rankfailure, 0, &channels[13]);
					bPlayMission = true;
				}
				if(nCountStepEval < (nTimeShowRankEval+TIME_START_RANKMISSION_STEPEVAL+TIME_TRANSI_RANKMISSION_STEPEVAL+TIME_GLOUTH_RANKMISSION_STEPEVAL))
				{
					dApt = (double(nCountStepEval-nTimeShowRankEval-TIME_START_RANKMISSION_STEPEVAL-TIME_TRANSI_RANKMISSION_STEPEVAL))/TIME_GLOUTH_RANKMISSION_STEPEVAL;
					glLoadIdentity();
					glScaled(1.0+0.5*(dApt), 1.0+0.5*(dApt), 1.0);
					glBindTexture(GL_TEXTURE_2D, evalres->skin[(bMissionSucess?0:1)+31]->GetCurrentTex());
					glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-27.75,  15.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 27.75,  15.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 27.75, -15.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-27.75, -15.0, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
				glPopMatrix();
			}
		}
	}

	if(nCountStepEval >= (nTimeOutStepEval) && nCountStepEval < (nTimeOutStepEval+TIME_EXIT_STEPEVAL))
	{
		dApt = (double(nCountStepEval-nTimeShowRankEval-nTimeOutStepEval))/TIME_EXIT_STEPEVAL;
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor4d(0.0, 0.0, 0.0, dApt);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
		glColor4d(1.0, 1.0, 1.0, 1.0);
	}
	if(nCountStepEval >= (nTimeOutStepEval+TIME_EXIT_STEPEVAL))
	{
		glColor4d(0.0, 0.0, 0.0, 0.0);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
		glColor4d(1.0, 1.0, 1.0, 1.0);
		FMOD_Channel_Stop(channels[11]);
		FMOD_Channel_Stop(channels[12]);
		ClearContextStepEvalRes();
		return 0;
	}
	return 1;
}

int EventStepEval(int nEvent, int nKey)
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

int FrameStageBreakStepEval()
{
	bMissionSucess = 0;
	static bool bIsp = false;
	if(!bIsp)
	{
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, sound_break, 0, &channels[9]);
		if(movie_stagebreak == NULL)
        {
            movie_stagebreak = new CMovieStream2();
            movie_stagebreak->OpenAVI(_T("StepEval/break.avi"));
        }
		if(movie_stagebreak->bLoad) movie_stagebreak->SetZeroCount();
		bIsp = true;
	}

	if(movie_stagebreak->bLoad)
	{
		movie_stagebreak->Update(dwWait);
		movie_stagebreak->GrabAVIFrame();
		glGenTextures(1, &tex_bgeval);
		glBindTexture(GL_TEXTURE_2D, tex_bgeval);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, movie_stagebreak->nwidth,  movie_stagebreak->nheight, 0, GL_RGB, GL_UNSIGNED_BYTE, movie_stagebreak->data);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
		glEnd();
		glDeleteTextures(1, &tex_bgeval);
	}

	FMOD_BOOL bIs;
	FMOD_Channel_IsPlaying(channels[9], &bIs);
	if(!bIs || channels[9] == NULL)
	{
		bIsp = false;
		if(movie_stagebreak->bLoad) movie_stagebreak->CloseAVI();
		if(movie_stagebreak != NULL) {delete movie_stagebreak; movie_stagebreak = NULL;}
		return 0;
	}
	return 1;
}

int FrameGameOverStepEval()
{
	static bool bIsp = false;
	if(!bIsp)
	{
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, sound_gameover, 0, &channels[9]);
		if(movie_gameover == NULL)
        {
            movie_gameover = new CMovieStream2();
            movie_gameover->OpenAVI(_T("StepEval/GAMEOVER.avi"));
        }
		if(movie_gameover->bLoad) movie_gameover->SetZeroCount();
		bIsp = true;
	}

	if(movie_gameover->bLoad)
	{
		movie_gameover->Update(dwWait);
		movie_gameover->GrabAVIFrame();
		glGenTextures(1, &tex_bgeval);
		glBindTexture(GL_TEXTURE_2D, tex_bgeval);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, movie_gameover->nwidth,  movie_gameover->nheight, 0, GL_RGB, GL_UNSIGNED_BYTE, movie_gameover->data);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
		glEnd();
		glDeleteTextures(1, &tex_bgeval);
	}

	FMOD_BOOL bIs;
	FMOD_Channel_IsPlaying(channels[9], &bIs);
	if(!bIs || channels[9] == NULL)
	{
		SetVolume(piuconfig.nSoundPrev?0:1);
		bIsp = false;
		if(movie_gameover) {if(movie_gameover->bLoad) movie_gameover->CloseAVI(); delete movie_gameover; movie_gameover = NULL;}
		return 0;
	}
	return 1;
}

void ActualizateContextStageStepEval()
{
    static int nRep[] = {-1,-1,-1};
	if(nCurStage <= 1)
    {
        nRep[0] = -1;
        nRep[1] = -1;
        nRep[2] = -1;
    }
    int nSel = 0;
    bool bCount = false;
    if((nCurStage >= nSizeMovieStage && nCurStage <= 2) || nCurStage >= 3)
        for(int x = 0; x < 3; x++)
            nRep[x] = -1;
    do
    {
        bCount = false;
        nSel = rand()%(nCurStage <= 2?nSizeMovieStage:nSizeMovieBStage);
        if(nCurStage <= 2) for(int x = 0; x < 3; x++) if(nRep[x] == nSel) bCount = true;
    } while (nCurStage <= 2 && bCount);
    if(nCurStage <= 2) nRep[max(nCurStage-1, 0)] = nSel;
    CString strFile;
    strFile = _T("StepEval/") + (nCurStage <= 2?strNameMovieStage[nSel]:strNameMovieBStage[nSel]) + _T(".avi");
    movie_stage = new CMovieStream2();
    movie_stage->OpenAVI(strFile);
    strFile = _T("StepEval/") + (nCurStage <= 2?strNameMovieStage[nSel]:strNameMovieBStage[nSel]) + _T(".MP3");
#ifdef UNICODE
    char chFile[260];
    UnicodeToAnsi(strFile, chFile);
    FMOD_System_CreateStream(sys, chFile, FMOD_HARDWARE | FMOD_2D, 0, &sound_stage);
#else
    FMOD_System_CreateStream(sys, strFile, FMOD_HARDWARE | FMOD_2D, 0, &sound_stage);
#endif
}

void ClearContextStageStepEval()
{
    if(movie_stage)
    {
        if(movie_stage->bLoad) movie_stage->CloseAVI();
        delete movie_stage;
        movie_stage = NULL;
    }
    FMOD_Sound_Release(sound_stage);
}

int FrameStageStepEval()
{
    static bool bIsp = false;
	bool bIsRepr = true;
	if((nCurStage <= 2 && nSizeMovieStage <= 0) || (nCurStage >= 3 && nSizeMovieBStage <= 0) || (!np[0] && !np[1]))
		bIsRepr = false;

    if(!bIsp && bIsRepr)
    {
        FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, sound_stage, 0, &channels[9]);
        bIsp = true;
    }

	if(movie_stage) if(movie_stage->bLoad)
	{
		movie_stage->Update(dwWait);
		movie_stage->GrabAVIFrame();
		glBindTexture(GL_TEXTURE_2D, movie_stage->tex);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
		glEnd();
	}

	FMOD_BOOL bIs;
	FMOD_Channel_IsPlaying(channels[9], &bIs);
	if(!bIs || !bIsRepr || channels[9] == NULL)
	{
        bIsp = false;
        ClearContextStageStepEval();
		return 0;
	}
	return 1;
}

int EnumLetterStepEval();

void ClearContextHSStepEval()
{
	if(nSizeLetterStepEval)
	{
		strCharSetHSSteopEval = _T("");
		glDeleteTextures(nSizeLetterStepEval, uTexLetterStepEval);
		nSizeLetterStepEval = 0;
		delete uTexLetterStepEval;
	}
}

void ActualizateContextHSStepEval()
{
    if(movie_bgeval == NULL)
	{
        movie_bgeval = new CMovieStream2();
        movie_bgeval->OpenAVI(_T("StepEval/back.avi"));
        movie_bgeval->bLoop = true;
	}
	if(movie_stagebreak == NULL)
	{
        movie_stagebreak = new CMovieStream2();
        movie_stagebreak->OpenAVI(_T("StepEval/break.avi"));
	}
	if(movie_gameover == NULL)
	{
        movie_gameover = new CMovieStream2();
        movie_gameover->OpenAVI(_T("StepEval/GAMEOVER.avi"));
	}
	if(movie_hs == NULL)
	{
        movie_hs = new CMovieStream2();
        movie_hs->OpenAVI(_T("StepEval/HS/back.avi"));
        movie_hs->bLoop = true;
	}

    if(evalres == NULL)
    {
        evalres = new CStepEvalRes();
        evalres->LoadSkin();
    }

	if(!EnumLetterStepEval())
	{
		bNewHighScore = false;
		return;
	}
	bNewHighScore = false;
	nCountStepEval = 0;
	for(int x = 0; x < 2; x++)
	{
		nIndexHighScore[x] = -1;
		if(bSaveUsbActive[x] && nTotalScorePlayer[x] >= machine_sesion->nRecords[19] && !np[x])
		{
			bNewHighScore = true;
			for(int y = 19; y >= 0; y--)
			{
				nIndexHighScore[x] = y;
				if(nTotalScorePlayer[x] < machine_sesion->nRecords[y])
				{
					nIndexHighScore[x]++;
					break;
				}
			}
			if(x == 1)
			{
				if(nIndexHighScore[int(!x)] >= nIndexHighScore[x]) nIndexHighScore[int(!x)]++;
			}
			for(int y = 18; y >= nIndexHighScore[x]; y--)
			{
				machine_sesion->nRecords[y+1] = machine_sesion->nRecords[y];
				_strcpy(machine_sesion->chRecords[y+1], machine_sesion->chRecords[y]);
			}
		}
		nDirTurnLetterStepEval[x] = 0;
		nTimeTurnLetterStepEval[x] = 0;
		nCurLetterStepEval[x] = 0;
		memset(chNameStepEval[x], 0, 12*sizeof(TCHAR));
		nCountNameStepEval[x] = 0;
		bChoosenNameStepEval[x] = false;
	}
	for(int x = 0; x < 10; x++)
	{
		bEnArrowsStepEval[x] = 0;
		nTimeEnArrowsStepEval[x] = 0;
	}
	if(!bNewHighScore) ClearContextHSStepEval();
}

void DrawLettersStepEval(int x)
{
#define TIME_TRANSI_LETTER_STEPEVAL 500
	glPushMatrix();
	glTranslated(x?20.0:-20.0, 0.0, 0.0);
	__int64 nTimeSkin = nCountStepEval/* - nTimeTurnLetterStepEval[x]*/;
	double dApt;
	int n[5];
	for(int y = 0; y < 5; y++)
	{
		n[y] = (y-2+nCurLetterStepEval[x])%(nSizeLetterStepEval+2);
		while(n[y] < 0) n[y] += (nSizeLetterStepEval+1);
	}
	if(nTimeSkin >= 0 && nTimeSkin < TIME_TRANSI_LETTER_STEPEVAL)
	{
		dApt = double(nTimeSkin)/TIME_TRANSI_LETTER_STEPEVAL;
		glColor4d(1.0, 1.0, 1.0, dApt);
		/*glPushMatrix();
		glTranslated(0.0, -2.5*(1-dApt)+11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[168]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();*/
		glPushMatrix();
		glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
		glBindTexture(GL_TEXTURE_2D, evalres->skin[34]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[2]==0?evalres->skin[37]->GetCurrentTex():(n[2]==1?evalres->skin[36]->GetCurrentTex():uTexLetterStepEval[n[2]-2]));
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -10.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(10.0-5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, evalres->skin[34]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[3]==0?evalres->skin[37]->GetCurrentTex():(n[3]==1?evalres->skin[36]->GetCurrentTex():uTexLetterStepEval[n[3]-2]));
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(-10.0+5.0*(1-dApt), 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, evalres->skin[34]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, n[1]==0?evalres->skin[37]->GetCurrentTex():(n[1]==1?evalres->skin[36]->GetCurrentTex():uTexLetterStepEval[n[1]-2]));
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
	if(nTimeSkin >= TIME_TRANSI_LETTER_STEPEVAL)
	{
#define TIME_TURNLETTER_STEPEVAL 100
		/*glPushMatrix();
		glTranslated(0.0, 11.25, 0.0);
		glBindTexture(GL_TEXTURE_2D, ga1res->skin[168]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 10.0,  2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
		glEnd();
		glPopMatrix();*/
		__int64 nTimeTurn = nCountStepEval - nTimeTurnLetterStepEval[x];
		dApt = 0.0;
		if(nTimeTurn >= 0 && nTimeTurn <= TIME_TURNLETTER_STEPEVAL && nDirTurnLetterStepEval[x])
		{
			dApt = (nDirTurnLetterStepEval[x]==1?-1:1)*(1-double(nTimeTurn)/TIME_TURNLETTER_STEPEVAL);
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
			glBindTexture(GL_TEXTURE_2D, evalres->skin[34]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  5.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -5.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, n[u]==0?evalres->skin[37]->GetCurrentTex():(n[u]==1?evalres->skin[36]->GetCurrentTex():uTexLetterStepEval[n[u]-2]));
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
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.0, x?-23.0:-15.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, evalres->skin[35]->GetCurrentTex());
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 1.0); glVertex3d( -30.0,  3.75, -50.0);
	glTexCoord2d(1.0, 1.0); glVertex3d(  30.0,  3.75, -50.0);
	glTexCoord2d(1.0, 0.0); glVertex3d(  30.0, -3.75, -50.0);
	glTexCoord2d(0.0, 0.0); glVertex3d( -30.0, -3.75, -50.0);
	glEnd();
	CString str;
	str = ( chNameStepEval[x]);
	font_Micro->glDrawText(3.75, -30.0, -3.75, 30.0, -15.0, 0.0, -50.0, 1.0, 3.0, str, DT_CENTER|DT_VCENTER);
	str.Format(_T("%.8d"), nTotalScorePlayer[x]);
	font_Micro->glDrawText(3.75, -30.0, -3.75, 30.0, 15.0, 0.0, -50.0, 1.0, 3.0, str, DT_CENTER|DT_VCENTER);
	str.Format(_T("%d"), nIndexHighScore[x]+1);
	font_Micro->glDrawText(3.75, -30.0, -3.75, 30.0, -27.0, 0.0, -50.0, 1.0, 3.0, str, DT_CENTER|DT_VCENTER);
	glPopMatrix();
}

int EnumLetterStepEval()
{
	FILE* hFile = _fopen(_T("StepEval/HS/letters.acc"), _T("r"));
	if(hFile == NULL) return 0;
	TCHAR ch[260];
	CString strPar;
	CString strArg;
	int nCode = 0;
	while(_fgets(ch, 260, hFile) != NULL)
	{
		strPar = ( ch);
		strPar.Remove(_T('\n'));
		if(strPar.Find(_T("//")) == -1)
		{
			if(strPar.Find(_T("#SIZELETTER")) != -1)
			{
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
				strArg.Remove(_T(' '));
				strArg.Remove(_T('\t'));
				if(_sscanf(strArg, _T("%d"), &nSizeLetterStepEval) != 1) {fclose(hFile); return 0;}
				uTexLetterStepEval = new GLuint[nSizeLetterStepEval];
				for(int x = 0; x < nSizeLetterStepEval; x++)
				{
					uTexLetterStepEval[x] = 0;
				}
			}
			if(strPar.Find(_T("#CHARSET")) != -1)
			{
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				strCharSetHSSteopEval = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
				if(strCharSetHSSteopEval.GetLength() < nSizeLetterStepEval) {fclose(hFile); return 0;}
			}
		}
	}
	fclose(hFile);
	for(int x = 0; x < nSizeLetterStepEval; x++)
	{
		strPar.Format(_T("StepEval/HS/Letter%d.png"), x+1);
		uTexLetterStepEval[x] = GenTexImage(strPar);
	}
	return 1;
}

void TurnLetterStepEval(bool bIzq, int x)
{
	if(bIzq)
	{
		nCurLetterStepEval[x]--;
		if(nCurLetterStepEval[x] < 0) nCurLetterStepEval[x] = nSizeLetterStepEval+1;
	}
	else
	{
		nCurLetterStepEval[x]++;
		if(nCurLetterStepEval[x] >= (nSizeLetterStepEval+2)) nCurLetterStepEval[x] = 0;
	}
	nDirTurnLetterStepEval[x] = bIzq?1:2;
	nTimeTurnLetterStepEval[x] = nCountStepEval;
	//FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
}

void MakeChoiceLetterStepEval(int x)
{
	if(nCurLetterStepEval[x] == 0 && nCountNameStepEval[x] > 0)
	{
		nCountNameStepEval[x]--;
		chNameStepEval[x][nCountNameStepEval[x]] = 0;
	}
	else if(nCurLetterStepEval[x] == 1)
	{
		if(nCurLetterStepEval[int(!x)] == 1 || !bSaveUsbActive[int(!x)] || np[int(!x)])
		{
			bChoosenNameStepEval[0] = true;
			bChoosenNameStepEval[1] = true;
			chNameStepEval[0][nCountNameStepEval[0]] = 0;
			chNameStepEval[1][nCountNameStepEval[1]] = 0;
			for(int y = 0; y < 2; y++) if(bSaveUsbActive[y] && nIndexHighScore[y]>=0 && nIndexHighScore[y]<=19)
			{
				_strcpy(machine_sesion->chRecords[nIndexHighScore[y]], chNameStepEval[y]);
				machine_sesion->nRecords[nIndexHighScore[y]] = nTotalScorePlayer[y];
			}
		}
	}
	else if(nCurLetterStepEval[x] != 0 && nCurLetterStepEval[x] != 1 && nCountNameStepEval[x] < 8)
	{
		chNameStepEval[x][nCountNameStepEval[x]] = strCharSetHSSteopEval.GetAt(nCurLetterStepEval[x]-2);
		nCountNameStepEval[x]++;
		if(nCountNameStepEval[x] == 7) nCurLetterStepEval[x] = 1;
	}
}

int FrameHSStepEval()
{
	nCountStepEval += dwTick - dwLastTick;
	static bool bIsp = false;
	if(!bIsp)
	{
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, sound_hs, 0, &channels[9]);
		if(movie_hs->bLoad) movie_hs->SetZeroCount();
		bIsp = true;
	}

	if(movie_hs->bLoad)
	{
		movie_hs->Update(dwWait);
		movie_hs->GrabAVIFrame();
		glGenTextures(1, &tex_bgeval);
		glBindTexture(GL_TEXTURE_2D, tex_bgeval);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, movie_hs->nwidth,  movie_hs->nheight, 0, GL_RGB, GL_UNSIGNED_BYTE, movie_hs->data);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
		glEnd();
		glDeleteTextures(1, &tex_bgeval);
	}

	glBindTexture(GL_TEXTURE_2D, evalres->skin[33]->GetCurrentTex());
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
	glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
	glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
	glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
	glEnd();

	CString strDispTime;
	strDispTime.Format(_T("%.2d"), int((30000 - nCountStepEval)/1000));
	font_Micro->glDrawText(30.0, -40.0, -30.0, 40.0, -37.2, 27.2, -50.0, 1.0, 1.5, strDispTime, DT_VCENTER|DT_CENTER);
	static bool bCh[] = {false, false, false, false, false,
							false, false, false, false, false};

	for(int x = 0; x < 2; x++) if(bSaveUsbActive[x] && !np[x])
	{
		DrawLettersStepEval(x);
		if(x == 0)
		{
			for(int y = 0; y < 5; y++)
			{
				if(bArrows[y] && !bEnArrowsStepEval[y])
				{
					bEnArrowsStepEval[y] = true;
					nTimeEnArrowsStepEval[y] = nCountStepEval;
				}
				if(!bArrows[y] && bEnArrowsStepEval[y])
				{
					bEnArrowsStepEval[y] = false;
					bCh[y] = false;
				}
			}
		}
		if(x == 1)
		{
			for(int y = 5; y < 10; y++)
			{
				if(bArrows[y] && !bEnArrowsStepEval[y])
				{
					bEnArrowsStepEval[y] = true;
					nTimeEnArrowsStepEval[y] = nCountStepEval;
				}
				if(!bArrows[y] && bEnArrowsStepEval[y])
				{
					bEnArrowsStepEval[y] = false;
					bCh[y] = false;
				}
			}
		}
#define TIME_FAST_CHANGE_STEPEVAL 500
#define TIME_CHANGE_LOW_STEPEVAL 500
#define TIME_CHANGE_HIGH_STEPEVAL 100
		if(bEnArrowsStepEval[0])
		{
			__int64 nCaret = nCountStepEval - nTimeEnArrowsStepEval[0];
			int nLimit = nCaret>TIME_FAST_CHANGE_STEPEVAL?TIME_CHANGE_HIGH_STEPEVAL:TIME_CHANGE_LOW_STEPEVAL;
			nCaret = nCaret % nLimit;
			if(nCaret < (nLimit/2) && !bCh[0])
			{
				bCh[0] = true;
				TurnLetterStepEval(true, 0);
				if(nLimit >= TIME_CHANGE_LOW_STEPEVAL) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
				//PlaySoundTransiAboutLimit(nLimit);
				//bShow = false;
				//FMOD_Channel_Stop(channels[9]);
			}
			if(nCaret >= (nLimit/2))
			{
				bCh[0] = false;
			}
		}
		if(bEnArrowsStepEval[4])
		{
			__int64 nCaret = nCountStepEval - nTimeEnArrowsStepEval[4];
			int nLimit = nCaret>TIME_FAST_CHANGE_STEPEVAL?TIME_CHANGE_HIGH_STEPEVAL:TIME_CHANGE_LOW_STEPEVAL;
			nCaret = nCaret % nLimit;
			if(nCaret < (nLimit/2) && !bCh[4])
			{
				bCh[4] = true;
				TurnLetterStepEval(false, 0);
				if(nLimit >= TIME_CHANGE_LOW_STEPEVAL) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
				//PlaySoundTransiAboutLimit(nLimit);
				//bShow = false;
				//FMOD_Channel_Stop(channels[9]);
			}
			if(nCaret >= (nLimit/2))
			{
				bCh[4] = false;
			}
		}
		if(bEnArrowsStepEval[2] && !bCh[2])
		{
			MakeChoiceLetterStepEval(0);
			bCh[2] = true;
		}
		if(bEnArrowsStepEval[5])
		{
			__int64 nCaret = nCountStepEval - nTimeEnArrowsStepEval[5];
			int nLimit = nCaret>TIME_FAST_CHANGE_STEPEVAL?TIME_CHANGE_HIGH_STEPEVAL:TIME_CHANGE_LOW_STEPEVAL;
			nCaret = nCaret % nLimit;
			if(nCaret < (nLimit/2) && !bCh[5])
			{
				bCh[5] = true;
				TurnLetterStepEval(true, 1);
				if(nLimit >= TIME_CHANGE_LOW_STEPEVAL) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
				//PlaySoundTransiAboutLimit(nLimit);
				//bShow = false;
				//FMOD_Channel_Stop(channels[9]);
			}
			if(nCaret >= (nLimit/2))
			{
				bCh[5] = false;
			}
		}
		if(bEnArrowsStepEval[9])
		{
			__int64 nCaret = nCountStepEval - nTimeEnArrowsStepEval[9];
			int nLimit = nCaret>TIME_FAST_CHANGE_STEPEVAL?TIME_CHANGE_HIGH_STEPEVAL:TIME_CHANGE_LOW_STEPEVAL;
			nCaret = nCaret % nLimit;
			if(nCaret < (nLimit/2) && !bCh[9])
			{
				bCh[9] = true;
				TurnLetterStepEval(false, 1);
				if(nLimit >= TIME_CHANGE_LOW_STEPEVAL) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
				//PlaySoundTransiAboutLimit(nLimit);
				//bShow = false;
				//FMOD_Channel_Stop(channels[9]);
			}
			if(nCaret >= (nLimit/2))
			{
				bCh[9] = false;
			}
		}
		if(bEnArrowsStepEval[7] && !bCh[7])
		{
			MakeChoiceLetterStepEval(1);
			bCh[7] = true;
		}
	}
	if(nCountStepEval >= 30000)
	{
		bChoosenNameStepEval[0] = true;
		bChoosenNameStepEval[1] = true;
		chNameStepEval[0][nCountNameStepEval[0]] = 0;
		chNameStepEval[1][nCountNameStepEval[1]] = 0;
		for(int y = 0; y < 2; y++) if(!np[y] && bSaveUsbActive[y] && nIndexHighScore[y]>=0 && nIndexHighScore[y]<=19)
		{
			_strcpy(machine_sesion->chRecords[nIndexHighScore[y]], chNameStepEval[y]);
			machine_sesion->nRecords[nIndexHighScore[y]] = nTotalScorePlayer[y];
		}
	}
	if((bChoosenNameStepEval[0] || !bSaveUsbActive[0] || np[0]) && (bChoosenNameStepEval[1] || !bSaveUsbActive[1] || np[1]))
	{
		FMOD_Channel_Stop(channels[9]);
		ClearContextHSStepEval();
		bIsp = false;
		for(int x = 0; x < 10; x++) bCh[x] = false;

		ClearContextStepEvalRes();
		return 0;
	}
	return 1;
}

void ClearContextStepEvalRes(void)
{
    if(evalres) {delete evalres; evalres = NULL;}
    if(movie_hs) {if(movie_hs->bLoad) movie_hs->CloseAVI(); delete movie_hs; movie_hs = NULL;}
    if(movie_bgeval) {if(movie_bgeval->bLoad) movie_bgeval->CloseAVI(); delete movie_bgeval; movie_bgeval = NULL;}
    if(movie_stagebreak) {if(movie_stagebreak->bLoad) movie_stagebreak->CloseAVI(); delete movie_stagebreak; movie_stagebreak = NULL;}
    if(movie_gameover) {if(movie_gameover->bLoad) movie_gameover->CloseAVI(); delete movie_gameover; movie_gameover = NULL;}
}
