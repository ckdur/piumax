#include "MaxStepSmu.h"
#include "basutil.h"


const int nTIMIMG_NORMAL[] = {100, 150, 200, 250, -1};
const int nTIMIMG_HARD[] = {50, 100, 150, 200, -1};
const int nTIMIMG_EASY[] = {125, 200, 225, 250, -1};

// Static declarations
CString CMaxStepSmu::m_strFileMusic = _T("");
bool CMaxStepSmu::m_bIsMusic = false;
CSoundStream* CMaxStepSmu::m_ssMusic;
float CMaxStepSmu::m_fRush;

CString CMaxStepSmu::m_strFileBGA = _T("");
bool CMaxStepSmu::m_bIsBGA = false;
CMovieStream2* CMaxStepSmu::m_msBGA;

CString CMaxStepSmu::m_strFileSplash = _T("");
bool CMaxStepSmu::m_bIsSplash = false;
CGraphTexture* CMaxStepSmu::m_gtSplash;

CMaxGenericBGA* CMaxStepSmu::m_gbBGA;

CSoundRes* CMaxStepSmu::m_srMain;

unsigned int CMaxStepSmu::m_nPos = 0;

#define CHANNEL_STEPSMU_MUSIC 1
#define CHANNEL_STEPSMU_SOUNDS 2
#define CHANNEL_STEPSMU_ASSISTTICK 3

TCHAR* chFilesRes[] =
{
	_T("skins/mine.wav"),
	_T("skins/ATick.wav"),
	_T("skins/BS/right.wav"),
	_T("skins/BS/wrong.wav")
};

int CMaxStepSmu::LoadResources(LPCTSTR strMusic, LPCTSTR strBGA, LPCTSTR strSplash)
{
	m_strFileMusic = strMusic;
	if(!m_strFileMusic.empty())
	{
		m_ssMusic = new CSoundStream();
		int nStatus = g_ssMain->CreateStream(strMusic, 0, m_ssMusic);
		if(!nStatus) delete m_ssMusic;
		else m_bIsMusic = true;
	}

	m_strFileBGA = strBGA;
	if(!m_strFileBGA.empty())
	{
		m_msBGA = new CMovieStream2();
		bool bStatus = m_msBGA->OpenAVI(strBGA);
		if(!bStatus || !m_msBGA->bLoad) delete m_msBGA;
		else m_bIsBGA = true;
	}

	m_strFileSplash = strSplash;
	if(!m_strFileSplash.empty())
	{
		m_gtSplash = GenTexImage(strSplash);
		m_bIsSplash = true;
	}

	m_srMain = new CSoundRes();

	for(int x = 0; x < STEPSMUSRES_SIZE; x++)
	{
		m_srMain->AddSound(chFilesRes[x], 0);
	}

	m_nPos = 0;
	return 1;
}

void CMaxStepSmu::DelResources(void)
{
	if(m_bIsMusic)
	{
		m_ssMusic->Release();
		delete m_ssMusic;
		m_bIsMusic = false;
	}

	if(m_bIsBGA)
	{
		m_msBGA->CloseAVI();
		delete m_msBGA;
		m_bIsBGA = false;
	}
	else
	{
		delete m_gbBGA;
	}

	if(m_bIsSplash)
	{
		m_gtSplash->Release();
		delete m_gtSplash;
		m_bIsSplash = false;
	}
	m_srMain->Clear();
	delete m_srMain;
}

void CMaxStepSmu::DrawBGA(void)
{
	if(m_bIsBGA)
	{
		/*if(!m_bIsMusic)
		{*/
			m_msBGA->Update((DWORD)(float(dwTick-dwLastTick)*m_fRush));
		/*}
		else
		{
			unsigned int nPos = 0;
			channels[CHANNEL_STEPSMU_MUSIC]->GetPosition(&nPos, 0);
			m_msBGA->dwNext = (DWORD)nPos;
			m_msBGA->Update(0);
		}*/
		m_msBGA->GrabAVIFrame();
		g_gmMain->BindTexture(m_msBGA->tex);

		g_gmMain->Begin(PMG_QUADS);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(-80.0, -60.0, -100.0);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(80.0, -60.0, -100.0);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(80.0, 60.0, -100.0);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(-80.0, 60.0, -100.0);
		glEnd();
	}
	else
	{
		m_gbBGA->Update(dwTick - dwLastTick);
		m_gbBGA->Draw();
	}
	/*if(m_bIsSplash)
	{
		DrawSplash();
	}*/
}

void CMaxStepSmu::UpdateSong(void)
{
	if(m_bIsMusic)
	{
		channels[CHANNEL_STEPSMU_MUSIC]->GetPosition(&m_nPos, 0);
	}
}

void CMaxStepSmu::LaunchSong(void)
{
	LaunchSong(0);
}

void CMaxStepSmu::LaunchSong(unsigned int nPos)
{
	if(m_bIsMusic)
	{
		g_ssMain->PlaySound(CHANNEL_STEPSMU_MUSIC, m_ssMusic, true, channels[CHANNEL_STEPSMU_MUSIC]);
		float fFrec;
		channels[CHANNEL_STEPSMU_MUSIC]->GetFrequency(&fFrec);
		channels[CHANNEL_STEPSMU_MUSIC]->SetFrequency(fFrec*m_fRush);
		channels[CHANNEL_STEPSMU_MUSIC]->SetPosition(nPos, 0);
		channels[CHANNEL_STEPSMU_MUSIC]->SetPaused(false);
	}
	if (!m_bIsBGA)
	{
		m_gbBGA = new CMaxGenericBGA();
		m_gbBGA->SetChannel(channels[CHANNEL_STEPSMU_MUSIC]);
	}
}

void CMaxStepSmu::StopSong(void)
{
	if(m_bIsMusic)
	{
		channels[CHANNEL_STEPSMU_MUSIC]->Stop();
	}
}

void CMaxStepSmu::DrawSplash(void)
{
	if(m_bIsSplash)
	{
		g_gmMain->BindTexture(m_gtSplash);

		g_gmMain->Begin(PMG_QUADS);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(-80.0, -60.0, -100.0);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(80.0, -60.0, -100.0);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(80.0, 60.0, -100.0);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(-80.0, 60.0, -100.0);
		glEnd();
	}
}

// Normal declarations

CMaxStepSmu::CMaxStepSmu(void)
{
	// Variables varias no reconocidas
	ox = 0;
	oy = 0;


	// Variables de estado de los comandos
	m_bDrop = false;
	m_dUserVel = 1.0;
	m_dMachVel = 1.0;
	m_dActionVel = 0;
	m_nLevelMach = 0;
	m_bTrainingMode = false;
	m_nAttack = 0;
	m_bXMode = false;
	m_bNXMode = false;
	m_bRS = false;
	m_bMirror = false;
	m_bFreedom = false;
	m_bRV = false;
	m_bRandomSkin = false;
	m_nSkin = 0;
	m_nVanish = 0;
	m_bDC = false;
	m_bAC = false;
	m_bEW = false;
	m_bUnderAttack = false;
	m_dPassVel = 0;
	m_bFlash = false;
	m_nModePlay = MODE_PLAYER_NULL;
	m_bLeftAttack = false;
	m_bRightAttack = false;
	m_bSnake = false;
	m_nPlayer = 1;
	m_bAutoPlay = false;
	m_nHlpAuto = 0;
	m_dIncrBar = 0;
	m_dDecrBar = 0;

	m_fEVanish = 0;
	m_fIVanish = 0;

	m_nModeMach = 0;

	m_strFileMusic = _T("");

	// Variables vitales
	m_nTime = 0;	// Tiempo?
	m_dBeatTime = 0;
	m_dBeatDraw = 0;
	m_dSizeArrows = 0;
	m_dDistArrows = 0;

	memset(&evarrows, 0, sizeof(EVENTARROWS));

	skin = (CSkin**)calloc(stepc.hStep.nSizeArrowGroups, sizeof(CSkin*));
	common = new CSkinCommon();
	bs = new CSkinBS();
	bsf = new CFont();
	skintrain[0] = new CSkin();
	skintrain[1] = new CSkin();
	skintrain[2] = new CSkin();

	m_nSizeJudTimes = JUDAMENT_MISS;
	m_nJudTimes = (int*)calloc(m_nSizeJudTimes, sizeof(int));

	// Loading judgament USER
	for(int j = 0; j < m_nSizeJudTimes; j++)
	{
		m_nJudTimes[j] = (j+1)*10;
	}

	memset(&status, 0 , sizeof(STEPSTATUS));
	status.nPerfect = (int*)calloc(stepc.hStep.nSizeArrowGroups, sizeof(int));
	status.nGreat = (int*)calloc(stepc.hStep.nSizeArrowGroups, sizeof(int));
	status.nGood = (int*)calloc(stepc.hStep.nSizeArrowGroups, sizeof(int));
	status.nBad = (int*)calloc(stepc.hStep.nSizeArrowGroups, sizeof(int));
	status.nMiss = (int*)calloc(stepc.hStep.nSizeArrowGroups, sizeof(int));
	status.nCombo = (int*)calloc(stepc.hStep.nSizeArrowGroups, sizeof(int));
	status.nMaxCombo = (int*)calloc(stepc.hStep.nSizeArrowGroups, sizeof(int));
	status.nMissCombo = (int*)calloc(stepc.hStep.nSizeArrowGroups, sizeof(int));
	status.nMaxMissCombo = (int*)calloc(stepc.hStep.nSizeArrowGroups, sizeof(int));

	status.nArrowsStatus=(int**)calloc(0, sizeof(int*));
	status.nBSStatus=(int*)calloc(0, sizeof(int));

	memset(&stepc.hStep, 0 , sizeof(STEPCOMPILERHEADER));

	bsc.nSizeBSQuestions=0;
	bsc.nSizeBSAnswers=0;
	bsc.nSizeBSPopUp=0;
	bsc.dWIBSQuestions = (float*)calloc(bsc.nSizeBSQuestions,sizeof(float));
	bsc.dWEBSQuestions = (float*)calloc(bsc.nSizeBSQuestions,sizeof(float));
	bsc.dWABSQuestions = (float*)calloc(bsc.nSizeBSQuestions,sizeof(float));
	bsc.dwBSQuestions = (DWORD*)calloc(bsc.nSizeBSQuestions,sizeof(DWORD));
	bsc.dwDBSQuestionsEx = (DWORD*)calloc(bsc.nSizeBSQuestions*40,sizeof(DWORD));
	bsc.dwDBSQuestions = (DWORD*)calloc(bsc.nSizeBSQuestions*30,sizeof(DWORD));
	bsc.dWBSAnswers = (float*)calloc(bsc.nSizeBSAnswers,sizeof(float));
	bsc.nWWBSAnswers = (__int64*)calloc(bsc.nSizeBSAnswers,sizeof(__int64));
	bsc.dwBSAnswers = (DWORD*)calloc(bsc.nSizeBSAnswers*10,sizeof(DWORD));
	bsc.nBSAnswers = (int*)calloc(bsc.nSizeBSAnswers*10,sizeof(int));
	bsc.nDBSAnswers = (int*)calloc(bsc.nSizeBSAnswers,sizeof(int));
	bsc.dWBSPopUp = (float*)calloc(bsc.nSizeBSPopUp,sizeof(float));
	bsc.dwBSPopUp = (DWORD*)calloc(bsc.nSizeBSPopUp,sizeof(DWORD));

	trac.nSizeArg = 0;
	trac.nArg = (int*)calloc(trac.nSizeArg*stepc.hStep.nArrowsPerLine,sizeof(int));

	m_nSizeATArray = 0;
	m_bATArray = (BYTE*)calloc(m_nSizeATArray, sizeof(BYTE));
	m_nWTATArray = (__int64*)calloc(m_nSizeATArray, sizeof(__int64));
}


CMaxStepSmu::~CMaxStepSmu(void)
{
	free(skin);
	free(common);
	free(bs);
	free(bsf);

	delete skintrain[0];
	delete skintrain[1];
	delete skintrain[2];

	free(m_nJudTimes);

	free(status.nPerfect);
	free(status.nGreat);
	free(status.nGood);
	free(status.nBad);
	free(status.nMiss);
	free(status.nCombo);
	free(status.nMaxCombo);
	free(status.nMissCombo);
	free(status.nMaxMissCombo);

	free(status.nArrowsStatus);
	free(status.nBSStatus);

	free(bsc.dWIBSQuestions);
	free(bsc.dWEBSQuestions);
	free(bsc.dWABSQuestions);
	free(bsc.dwBSQuestions);
	free(bsc.dwDBSQuestionsEx);
	free(bsc.dwDBSQuestions);
	free(bsc.dWBSAnswers);
	free(bsc.nWWBSAnswers);
	free(bsc.dwBSAnswers);
	free(bsc.nBSAnswers);
	free(bsc.nDBSAnswers);
	free(bsc.dWBSPopUp);
	free(bsc.dwBSPopUp);

	free(trac.nArg);

	free(m_bATArray);
	free(m_nWTATArray);
}

void CMaxStepSmu::InitCompiler(STEPCOMPILER* fsong)
{
	// STEPCOMPILER
	fsong->fAttrib=(float*)calloc(fsong->hStep.nSizeAttrib,sizeof(float));

	fsong->nWTChangeBPM=(__int64*)calloc(fsong->hStep.nSizeChangeBPM,sizeof(__int64));
	fsong->fWTChangeBPM=(float*)calloc(fsong->hStep.nSizeChangeBPM,sizeof(float));
	fsong->fChangeBPM=(float*)calloc(fsong->hStep.nSizeChangeBPM,sizeof(float));

	fsong->fITChangeVel=(float*)calloc(fsong->hStep.nSizeChangeVel,sizeof(float));
	fsong->fETChangeVel=(float*)calloc(fsong->hStep.nSizeChangeVel,sizeof(float));
	fsong->fChangeVel=(float*)calloc(fsong->hStep.nSizeChangeVel,sizeof(float));
	fsong->fIChangeVel=(float*)calloc(fsong->hStep.nSizeChangeVel,sizeof(float));

	fsong->fWTJumps=(float*)calloc(fsong->hStep.nSizeJumps,sizeof(float));
	fsong->fJumps=(float*)calloc(fsong->hStep.nSizeJumps,sizeof(float));

	fsong->fWTStops=(float*)calloc(fsong->hStep.nSizeStops,sizeof(float));
	fsong->fStops=(float*)calloc(fsong->hStep.nSizeStops,sizeof(float));

	fsong->hStepGroup=(STEPCOMPILERGROUP*)calloc(fsong->hStep.nSizeArrowGroups,sizeof(STEPCOMPILERGROUP));
	fsong->hMod=(STEPCOMPILERMOD*)calloc(fsong->hStep.nSizeMod,sizeof(STEPCOMPILERMOD));

	// STEPCOMPILERGROUP
	for(int i = 0; i < fsong->hStep.nSizeArrowGroups; i++)
	{
	    memset(&fsong->hStepGroup[i].hGroup, 0, sizeof(STEPCOMPILERGROUPHEADER));

		fsong->hStepGroup[i].fWDArrows=(float*)calloc(fsong->hStepGroup[i].hGroup.nSizeArrows,sizeof(float));
		fsong->hStepGroup[i].nWTArrows=(__int64*)calloc(fsong->hStepGroup[i].hGroup.nSizeArrows,sizeof(__int64));
		fsong->hStepGroup[i].nArrows=(int*)calloc(fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine,sizeof(int));
		fsong->hStepGroup[i].nPArrows=(int*)calloc(fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine,sizeof(int));
	}

	// STEPCOMPILERMOD
	for(int i = 0; i < fsong->hStep.nSizeMod; i++)
	{
		memset(&fsong->hMod[i].hMod, 0, sizeof(STEPCOMPILERMODHEADER));
		fsong->hMod[i].hModInfo=(STEPCOMPILERMODINFO*)calloc(fsong->hMod[i].hMod.nSize,sizeof(STEPCOMPILERMODINFO));
		for(int j = 0; j < fsong->hMod[i].hMod.nSize; j++)
		{
			fsong->hMod[i].hModInfo[j].nModInt=(int*)calloc(fsong->hMod[i].hMod.nSizeInt,sizeof(int));
			fsong->hMod[i].hModInfo[j].fModFloat=(float*)calloc(fsong->hMod[i].hMod.nSizeFloat,sizeof(float));
			memset(fsong->hMod[i].hModInfo[j].fWTMod, 0, 2*sizeof(float));
			memset(fsong->hMod[i].hModInfo[j].nWTMod, 0, 2*sizeof(__int64));
		}
	}
}

void CMaxStepSmu::ReInitCompiler(STEPCOMPILER* fsong)
{
	// STEP
	fsong->fAttrib=(float*)max_realloc(fsong->fAttrib, fsong->hStep.nSizeAttrib*sizeof(float));
	fsong->hStepGroup=(STEPCOMPILERGROUP*)max_realloc(fsong->hStepGroup, fsong->hStep.nSizeArrowGroups*sizeof(STEPCOMPILERGROUP));

	fsong->nWTChangeBPM=(__int64*)max_realloc(fsong->nWTChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(__int64));
	fsong->fWTChangeBPM=(float*)max_realloc(fsong->fWTChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(float));
	fsong->fChangeBPM=(float*)max_realloc(fsong->fChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(float));

	fsong->fITChangeVel=(float*)max_realloc(fsong->fITChangeVel, fsong->hStep.nSizeChangeVel*sizeof(float));
	fsong->fETChangeVel=(float*)max_realloc(fsong->fETChangeVel, fsong->hStep.nSizeChangeVel*sizeof(float));
	fsong->fChangeVel=(float*)max_realloc(fsong->fChangeVel, fsong->hStep.nSizeChangeVel*sizeof(float));
	fsong->fIChangeVel=(float*)max_realloc(fsong->fIChangeVel, fsong->hStep.nSizeChangeVel*sizeof(float));

	fsong->fWTJumps=(float*)max_realloc(fsong->fWTJumps, fsong->hStep.nSizeJumps*sizeof(float));
	fsong->fJumps=(float*)max_realloc(fsong->fJumps, fsong->hStep.nSizeJumps*sizeof(float));

	fsong->fWTStops=(float*)max_realloc(fsong->fWTStops, fsong->hStep.nSizeStops*sizeof(float));
	fsong->fStops=(float*)max_realloc(fsong->fStops, fsong->hStep.nSizeStops*sizeof(float));

	fsong->hMod=(STEPCOMPILERMOD*)max_realloc(fsong->hMod, fsong->hStep.nSizeMod*sizeof(STEPCOMPILERMOD));

	// STEPGROUPHEADER
	for(int i = 0; i < fsong->hStep.nSizeArrowGroups; i++)
	{
		fsong->hStepGroup[i].fWDArrows=(float*)max_realloc(fsong->hStepGroup[i].fWDArrows, fsong->hStepGroup[i].hGroup.nSizeArrows*sizeof(float));
		fsong->hStepGroup[i].nWTArrows=(__int64*)max_realloc(fsong->hStepGroup[i].nWTArrows, fsong->hStepGroup[i].hGroup.nSizeArrows*sizeof(__int64));
	    fsong->hStepGroup[i].nArrows=(int*)max_realloc(fsong->hStepGroup[i].nArrows, fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine*sizeof(int));
		fsong->hStepGroup[i].nPArrows=(int*)max_realloc(fsong->hStepGroup[i].nPArrows, fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine*sizeof(int));
	}

	// STEPMOD
	for(int i = 0; i < fsong->hStep.nSizeMod; i++)
	{
		fsong->hMod[i].hModInfo=(STEPCOMPILERMODINFO*)max_realloc(fsong->hMod[i].hModInfo, fsong->hMod[i].hMod.nSize*sizeof(STEPCOMPILERMODINFO));
		for(int j = 0; j < fsong->hMod[i].hMod.nSize; j++)
		{
			fsong->hMod[i].hModInfo[j].nModInt=(int*)max_realloc(fsong->hMod[i].hModInfo[j].nModInt, fsong->hMod[i].hMod.nSizeInt*sizeof(int));
			fsong->hMod[i].hModInfo[j].fModFloat=(float*)max_realloc(fsong->hMod[i].hModInfo[j].fModFloat, fsong->hMod[i].hMod.nSizeFloat*sizeof(float));
		}
	}
}

void CMaxStepSmu::DeleteCompiler(STEPCOMPILER* fsong)
{
	free(fsong->nWTChangeBPM);
	free(fsong->fWTChangeBPM);
	free(fsong->fChangeBPM);

	free(fsong->fITChangeVel);
	free(fsong->fETChangeVel);
	free(fsong->fChangeVel);
	free(fsong->fIChangeVel);

	free(fsong->fWTJumps);
	free(fsong->fJumps);

	free(fsong->fWTStops);
	free(fsong->fStops);

	for(int i = 0; i < fsong->hStep.nSizeArrowGroups; i++)
	{
		free(fsong->hStepGroup[i].fWDArrows);
		free(fsong->hStepGroup[i].nWTArrows);
		free(fsong->hStepGroup[i].nArrows);
		free(fsong->hStepGroup[i].nPArrows);
	}

	// STEP
	free(fsong->fAttrib);
	free(fsong->hStepGroup);

	for(int i = 0; i < fsong->hStep.nSizeMod; i++)
	{
		for(int j = 0; j < fsong->hMod[i].hMod.nSize; j++)
		{
			free(fsong->hMod[i].hModInfo[j].nModInt);
			free(fsong->hMod[i].hModInfo[j].fModFloat);
		}
		free(fsong->hMod[i].hModInfo);
	}

	free(fsong->hMod);
}

int CMaxStepSmu::StepCompile()
{
	// ** Asignar algunos modificadores provenientes del documento
	memcpy(&stepc.hStep, &pDoc->song.hStep, sizeof(STEPHEADER));
	InitCompiler(&stepc);

	for(int i = 0; i < stepc.hStep.nSizeArrowGroups; i++)
	{
		memcpy(&stepc.hStepGroup[i].hGroup, &pDoc->song.hStepGroup[i].hGroup, sizeof(STEPGROUPHEADER));
		stepc.hStepGroup[i].hGroup.nSizeArrows = 0;
	}

	for(int i = 0; i < stepc.hStep.nSizeMod; i++)
	{
		memcpy(&stepc.hMod[i].hMod, &pDoc->song.hMod[i].hMod, sizeof(STEPMODHEADER));
	}

	ReInitCompiler(&stepc);

	int m_nLevelSong = stepc.hStep.nDificulty;
	PUNT_EVA1 = 1500+(500*(m_nLevelSong/8));
	PUNT_EVA2 = 1000+(500*(m_nLevelSong/8));
	PUNT_EVA3 = 500+(500*(m_nLevelSong/8));
	PUNT_EVA4 = 300*(m_nLevelSong/16);
	PUNT_EVA5 = m_nLevelSong<16?0:(-500*((m_nLevelSong-12)/4));
	///////////////////////////////////////////////////////////////
	srand( (unsigned)time( NULL ) );

	if(!CompileSong()) return 0;
	if(m_bTrainingMode) if(!CompileTraining()) return 0;
	if(m_bIsAsistTick) CompileAssistTick();

	m_nTime = (__int64)(((float)(stepc.hStep.nOffset))*m_fRush);

	m_nHlpAuto = 0;
	if(!LoadTextures()) return 0;
	if(!CompileBS()) return 0;
	AllocStatus();

	////////////////////////////

	memset(&evarrows, 0, sizeof(EVENTARROWS));
	evarrows.dSourceVel = m_dUserVel;
	evarrows.dDestVel = m_dUserVel;
	for(int i = 0; i < 10; i++)
	{
		evarrows.nArrows[i] = stepc.hStep.nOffset;
		evarrows.nGlowArrows[i] = stepc.hStep.nOffset;
	}

	m_dMachVel = stepc.hStep.fVel;
	status.dLife = 0.5;

	m_bIsLoaded = true;
	return 1;
}

void CMaxStepSmu::StepDestroy()
{
	if(m_bIsLoaded)
	{
		for(int i = 0; i < stepc.hStep.nSizeArrowGroups; i++)
		{
			memset(&stepc.hStepGroup[i].hGroup, 0, sizeof(STEPGROUPHEADER));
		}

		for(int i = 0; i < stepc.hStep.nSizeMod; i++)
		{
			memset(&stepc.hMod[i].hMod, 0, sizeof(STEPMODHEADER));
		}
		ReInitCompiler(&stepc);

		//if(m_bIsAsistTick) DeleteAssistTick();
		if(m_bTrainingMode) DeleteTrainingCompiler();
		DeleteTextures();
		DeleteBS();

		DeleteSong();
		memset(&stepc.hStep, 0, sizeof(STEPHEADER));
		DeleteStatus();

		m_bIsLoaded = false;
	}
}

/*int CMaxStepSmu::LoadTextures(void)
{
	return 0;
}

void CMaxStepSmu::DeleteTextures(void)
{
	for(int i = 0; i < stepc.hStep.nSizeArrowGroups; i++)
	{
		skin[i]->Delete();
	}
	common->Delete();
	bs->Delete();
	bsf->Delete();
}*/

void CMaxStepSmu::Event_View(int nEvent, int nKey)
{
	if ( nEvent == EV_KEYDOWN )
    {
		if(m_nModePlay == MODE_PLAYER_SINGLE && m_nPlayer == 1)
		{
			if(nKey == EV_P1_ARROW1)
			{
				evarrows.nArrows[0] = m_nTime;
				evarrows.bArrows[0] = true;
			}
			if(nKey == EV_P1_ARROW7)
			{
				evarrows.nArrows[1] = m_nTime;
				evarrows.bArrows[1] = true;
			}
			if(nKey == EV_P1_CENTER)
			{
				evarrows.nArrows[2] = m_nTime;
				evarrows.bArrows[2] = true;
			}
			if(nKey == EV_P1_ARROW9)
			{
				evarrows.nArrows[3] = m_nTime;
				evarrows.bArrows[3] = true;
			}
			if(nKey == EV_P1_ARROW3)
			{
				evarrows.nArrows[4] = m_nTime;
				evarrows.bArrows[4] = true;
			}
		}
		if(m_nModePlay == MODE_PLAYER_SINGLE && m_nPlayer == 2)
		{
			if(nKey == EV_P2_ARROW1)
			{
				evarrows.nArrows[0] = m_nTime;
				evarrows.bArrows[0] = true;
			}
			if(nKey == EV_P2_ARROW7)
			{
				evarrows.nArrows[1] = m_nTime;
				evarrows.bArrows[1] = true;
			}
			if(nKey == EV_P2_CENTER)
			{
				evarrows.nArrows[2] = m_nTime;
				evarrows.bArrows[2] = true;
			}
			if(nKey == EV_P2_ARROW9)
			{
				evarrows.nArrows[3] = m_nTime;
				evarrows.bArrows[3] = true;
			}
			if(nKey == EV_P2_ARROW3)
			{
				evarrows.nArrows[4] = m_nTime;
				evarrows.bArrows[4] = true;
			}
		}
		if(m_nModePlay == MODE_PLAYER_DOUBLE)
		{
			if(nKey == EV_P1_ARROW1)
			{
				evarrows.nArrows[0] = m_nTime;
				evarrows.bArrows[0] = true;
			}
			if(nKey == EV_P1_ARROW7)
			{
				evarrows.nArrows[1] = m_nTime;
				evarrows.bArrows[1] = true;
			}
			if(nKey == EV_P1_CENTER)
			{
				evarrows.nArrows[2] = m_nTime;
				evarrows.bArrows[2] = true;
			}
			if(nKey == EV_P1_ARROW9)
			{
				evarrows.nArrows[3] = m_nTime;
				evarrows.bArrows[3] = true;
			}
			if(nKey == EV_P1_ARROW3)
			{
				evarrows.nArrows[4] = m_nTime;
				evarrows.bArrows[4] = true;
			}
			if(nKey == EV_P2_ARROW1)
			{
				evarrows.nArrows[5] = m_nTime;
				evarrows.bArrows[5] = true;
			}
			if(nKey == EV_P2_ARROW7)
			{
				evarrows.nArrows[6] = m_nTime;
				evarrows.bArrows[6] = true;
			}
			if(nKey == EV_P2_CENTER)
			{
				evarrows.nArrows[7] = m_nTime;
				evarrows.bArrows[7] = true;
			}
			if(nKey == EV_P2_ARROW9)
			{
				evarrows.nArrows[8] = m_nTime;
				evarrows.bArrows[8] = true;
			}
			if(nKey == EV_P2_ARROW3)
			{
				evarrows.nArrows[9] = m_nTime;
				evarrows.bArrows[9] = true;
			}
		}
    }
	if(nEvent == EV_KEYUP)
	{
		if(m_nModePlay == MODE_PLAYER_SINGLE && m_nPlayer == 1)
		{
			if(nKey == EV_P1_ARROW1)
				evarrows.bArrows[0] = false;
			if(nKey == EV_P1_ARROW7)
				evarrows.bArrows[1] = false;
			if(nKey == EV_P1_CENTER)
				evarrows.bArrows[2] = false;
			if(nKey == EV_P1_ARROW9)
				evarrows.bArrows[3] = false;
			if(nKey == EV_P1_ARROW3)
				evarrows.bArrows[4] = false;
		}
		if(m_nModePlay == MODE_PLAYER_SINGLE && m_nPlayer == 2)
		{
			if(nKey == EV_P2_ARROW1)
				evarrows.bArrows[0] = false;
			if(nKey == EV_P2_ARROW7)
				evarrows.bArrows[1] = false;
			if(nKey == EV_P2_CENTER)
				evarrows.bArrows[2] = false;
			if(nKey == EV_P2_ARROW9)
				evarrows.bArrows[3] = false;
			if(nKey == EV_P2_ARROW3)
				evarrows.bArrows[4] = false;
		}
		if(m_nModePlay == MODE_PLAYER_DOUBLE)
		{
			if(nKey == EV_P1_ARROW1)
				evarrows.bArrows[0] = false;
			if(nKey == EV_P1_ARROW7)
				evarrows.bArrows[1] = false;
			if(nKey == EV_P1_CENTER)
				evarrows.bArrows[2] = false;
			if(nKey == EV_P1_ARROW9)
				evarrows.bArrows[3] = false;
			if(nKey == EV_P1_ARROW3)
				evarrows.bArrows[4] = false;
			if(nKey == EV_P2_ARROW1)
				evarrows.bArrows[5] = false;
			if(nKey == EV_P2_ARROW7)
				evarrows.bArrows[6] = false;
			if(nKey == EV_P2_CENTER)
				evarrows.bArrows[7] = false;
			if(nKey == EV_P2_ARROW9)
				evarrows.bArrows[8] = false;
			if(nKey == EV_P2_ARROW3)
				evarrows.bArrows[9] = false;
		}
	}
}

float CMaxStepSmu::UbiXArrow(int nPos, int nLimitX, float dDistBetArrows, float dSizeArrows, float dBeat)
{
	int nPads = (nLimitX-1)/ARROWSLINE_SINGLE+1;
	float dTotal = float(nPads*(ARROWSLINE_SINGLE-1))*dDistBetArrows+float(nPads-1)*dSizeArrows;
	int nPad = nPos/ARROWSLINE_SINGLE;
	int nArr = nPos%ARROWSLINE_SINGLE;
	float fUbi = float(nArr)*dDistBetArrows+float(nPad)*(float(ARROWSLINE_SINGLE-1)*dDistBetArrows+dSizeArrows)-dTotal/2;
	if(m_bSnake && dBeat < 0.0)
	{
		// USER: snake frecuency = 0.5beat
		fUbi += dDistBetArrows*sin(float(M_PI*dBeat));
	}
	if(m_bXMode && dBeat < 0.0)
	{
		if((m_nModePlay == MODE_PLAYER_SINGLE && (m_nPlayer%2) == 1) || (m_nModePlay == MODE_PLAYER_DOUBLE && (nPos/ARROWSLINE_SINGLE%2) == 0))
		{
			fUbi -= UbiYArrow(nPos, nLimitX, dDistBetArrows, dSizeArrows, dBeat);
		}
		else
		{
			fUbi += UbiYArrow(nPos, nLimitX, dDistBetArrows, dSizeArrows, dBeat);
		}
	}
	return fUbi;
}

float CMaxStepSmu::UbiYArrow(int nPos, int nLimitX, float dDistBetArrows, float dSizeArrows, float dBeat)
{
	float dFactorVel = m_dUserVel*m_dMachVel*m_dPassVel;
	float fUbi = dBeat*dFactorVel*dSizeArrows;
	if((m_bAC || m_bDC) && dBeat < 0.0 && abs(fUbi) < SIZE_DISPLAY_ARROWS)
	{
		float fPow = 1.0f;
		fPow *= m_bAC?0.5f:1.0f;
		fPow *= m_bDC?2.0f:1.0f;
		fUbi = fUbi/abs(fUbi)*float(SIZE_DISPLAY_ARROWS)*pow(abs(float(fUbi/SIZE_DISPLAY_ARROWS)), fPow);
	}
	return fUbi;
}

int CMaxStepSmu::LoadTextures(void)
{
	skin = (CSkin**)max_realloc(skin, stepc.hStep.nSizeArrowGroups*sizeof(CSkin*));
	for(int n = 0; n < stepc.hStep.nSizeArrowGroups; n++)
	{
		skin[n] = new CSkin();
		if(m_bRandomSkin)
		{
			skin[n]->LoadSkin(rand()%MAX_SKINS);
		}
		else
		{
			if (stepc.hStepGroup[n].hGroup.nSkin == 0 && n == 0 && m_nSkin != -1) skin[0]->LoadSkin(m_nSkin);
			else
			{
				skin[n]->LoadSkin(stepc.hStepGroup[n].hGroup.nSkin);
			}
		}
	}

	if(m_bTrainingMode)
	{
		skintrain[0]->LoadSkin(SKIN_TRAIN_CENTER);
		skintrain[1]->LoadSkin(SKIN_TRAIN_LEFT);
		skintrain[2]->LoadSkin(SKIN_TRAIN_RIGHT);
	}
	common->LoadSkin();
	bs->LoadSkin();
	bsf->LoadFont(_T("skins\\BS\\BSFont.png"), 16, 16);

	/*for(int n = 0; n < 3; n++)for(int x = 0; x < 5; x++) for(int y = 0; y < skin[n]->skin[x+5]->m_nSize; y++)
	{
		glBindTexture(GL_TEXTURE_2D, skin[n]->skin[x+5]->texture[y]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}*/
	return 1;
}

void CMaxStepSmu::DeleteTextures(void)
{
	for(int n = 0; n < stepc.hStep.nSizeArrowGroups; n++)
	{
		skin[n]->Delete();
		delete skin[n];
	}
	skin = (CSkin**)max_realloc(skin, 0);

	if(m_bTrainingMode)
	{
		skintrain[0]->Delete();
		skintrain[1]->Delete();
		skintrain[2]->Delete();
	}
	common->Delete();
	bs->Delete();
	bsf->Delete();
}

float CMaxStepSmu::TimeToBeat(__int64 nTime)
{
	if(nTime < 0)
	{
		return float(nTime)*stepc.hStep.fBPM/60000;
	}

	int x = 0;

	if(0 >= stepc.hStep.nSizeChangeBPM || nTime < stepc.nWTChangeBPM[0])
	{
		return float(nTime)*stepc.hStep.fBPM/60000; // No existen mas cambios de BPM
	}
	else
	{
		float dBeat = float(stepc.nWTChangeBPM[0])*stepc.hStep.fBPM/60000;
		__int64 nTimeBef = stepc.nWTChangeBPM[0];

		for(x = 0; x < stepc.hStep.nSizeChangeBPM; x++)
		{
			if((x+1) >= stepc.hStep.nSizeChangeBPM || nTime < stepc.nWTChangeBPM[x+1])
			{
				return dBeat+float(nTime-nTimeBef)*stepc.fChangeBPM[x]/60000; // No existen mas cambios de BPM
			}
			else
			{
				dBeat += float(stepc.nWTChangeBPM[x+1]-nTimeBef)*stepc.fChangeBPM[x]/60000;
				nTimeBef = stepc.nWTChangeBPM[x+1];
			}
		}
		return dBeat;
	}
}

/*
sos muy lindu miau :3
*/

float CMaxStepSmu::BeatT2BeatD(float dBeat)
{
	if(dBeat < 0)
	{
		return dBeat;
	}

	float dBeats = dBeat;

	for(int x = 0; x < stepc.hStep.nSizeStops; x++)
	{
		if(dBeat >= stepc.fWTStops[x])
		{
			dBeats -= min(dBeat-stepc.fWTStops[x], stepc.fStops[x]);
		}
		if((x+1) >= stepc.hStep.nSizeStops || dBeat < stepc.fWTStops[x+1])
		{
			break;
		}
	}

	for(int x = 0; x < stepc.hStep.nSizeJumps; x++)
	{
		if(dBeat >= stepc.fWTJumps[x])
		{
			dBeats += stepc.fJumps[x];
		}
		if((x+1) >= stepc.hStep.nSizeJumps || dBeat < stepc.fWTJumps[x+1])
		{
			break;
		}
	}

	return dBeats;
}

__int64 CMaxStepSmu::BeatToTime(float dBeat)
{
	if(dBeat < 0)
	{
		return (__int64)(dBeat*60000/stepc.hStep.fBPM);
	}

	int x = 0;

	if(0 >= stepc.hStep.nSizeChangeBPM || dBeat < stepc.fWTChangeBPM[0])
	{
		return (__int64)(dBeat*60000/stepc.hStep.fBPM); // No existen mas cambios de BPM
	}
	else
	{
		__int64 nTime = (__int64)(stepc.fWTChangeBPM[0]*60000/stepc.hStep.fBPM);
		float fBeatBef = stepc.fWTChangeBPM[0];

		for(x = 0; x < stepc.hStep.nSizeChangeBPM; x++)
		{
			if((x+1) >= stepc.hStep.nSizeChangeBPM || dBeat < stepc.fWTChangeBPM[x+1])
			{
				return nTime+(__int64)((dBeat-fBeatBef)*60000/stepc.fChangeBPM[x]); // No existen mas cambios de BPM
			}
			else
			{
				nTime += (__int64)((stepc.fWTChangeBPM[x+1]-fBeatBef)*60000/stepc.fChangeBPM[x]);
				fBeatBef = stepc.fWTChangeBPM[x+1];
			}
		}
		return nTime;
	}
}

void CMaxStepSmu::GetLimitsOfDrawing(float dBeatDraw, int nGArrow, int* nMin, int* nMax)
{
	if(nGArrow < 0 || nGArrow >= stepc.hStep.nSizeArrowGroups) return;
	*nMin = 0;
	*nMax = 0;
	int n = nGArrow;
	float dFactorVel = m_dUserVel*m_dMachVel;

	for(int x = 0; x < stepc.hStepGroup[n].hGroup.nSizeArrows; x++)
	{
		*nMax = x+1;
		float dBeat = dBeatDraw-float(stepc.hStepGroup[n].fWDArrows[x]);
		float dUbi = UbiYArrow(0, 0, m_dDistArrows, m_dSizeArrows, dBeat);
		if(BOTTOM_ARROWS > dUbi)
		{
			break;
		}
	}
	for(int x = stepc.hStepGroup[n].hGroup.nSizeArrows-1; x >= 0; x--)
	{
		*nMin = x;
		float dUbi = (dBeatDraw-stepc.hStepGroup[n].fWDArrows[x])*dFactorVel;
		if(TOP_ARROWS < dUbi)
		{
			break;
		}
	}
}

void CMaxStepSmu::UpdateVel(float dBeat)
{
	m_dMachVel = stepc.hStep.fVel;
	for(int x = 0; x < stepc.hStep.nSizeChangeVel; x++)
	{
		if(dBeat > stepc.fITChangeVel[x])
		{
			if(dBeat <= stepc.fETChangeVel[x])
			{
				// Aqui significa que el beat en proceso esta entre un cambio de velocidad
				float fBackMachVel;
				fBackMachVel = stepc.fIChangeVel[x];
				m_dMachVel = fBackMachVel+
					(stepc.fChangeVel[x]-fBackMachVel)*
					((dBeat-stepc.fITChangeVel[x])/(stepc.fETChangeVel[x]-stepc.fITChangeVel[x]));
				break;
			}
			else if((x+1) >= stepc.hStep.nSizeChangeVel)
			{
				// Aqui es que ya no hay mas cambios de velocidad
				m_dMachVel = stepc.fChangeVel[x];
				break;
			}
			else if(stepc.fITChangeVel[x+1] > dBeat)
			{
				// Aqui es que el beat actual no es mayor que donde se encuentra el proximo cambio
				m_dMachVel = stepc.fChangeVel[x];
				break;
			}
			else
			{
				// Aqui significa que el beat actual es mayor que donde se encuentra el
				// proximo cambio, entonces será asumir la velocidad actual y continuar con el calculo
				m_dMachVel = stepc.fChangeVel[x];
			}
		}
	}
}

int CMaxStepSmu::GetArrowStatus(int x, int y, int nGArrows)
{
	if(nGArrows >= 0 && nGArrows < stepc.hStep.nSizeArrowGroups)
	{
		if(y >= stepc.hStepGroup[nGArrows].hGroup.nSizeArrows) return -1;
		if(x < 0 || x >= stepc.hStep.nArrowsPerLine) return -1;
		return status.nArrowsStatus[nGArrows][y*stepc.hStep.nArrowsPerLine+x];
	}
	return -1;
}

void CMaxStepSmu::SetArrowStatus(int x, int y, int nGArrows, int nValue)
{
	if(nGArrows >= 0 && nGArrows < stepc.hStep.nSizeArrowGroups &&
		y < stepc.hStepGroup[nGArrows].hGroup.nSizeArrows &&
		x < stepc.hStep.nArrowsPerLine) status.nArrowsStatus[nGArrows][y*stepc.hStep.nArrowsPerLine+x] = nValue;
}

int CMaxStepSmu::CompileSong(void)
{
	// Compile mods first
	__int64 nTimeBPM = 0;
	float fWChangeBPMPast = 0;
	float fBPM = stepc.hStep.fBPM;
	for(int x = 0; x < stepc.hStep.nSizeChangeBPM; x++)
	{
		stepc.fChangeBPM[x]=pDoc->song.fChangeBPM[x];
		stepc.fWTChangeBPM[x]=pDoc->BeatArrowTime(pDoc->song.nWChangeBPM[x]);

		float fWChangeBPM=stepc.fWTChangeBPM[x];
		nTimeBPM+=(__int64)((fWChangeBPM-fWChangeBPMPast)*60000.0/fBPM);
		stepc.nWTChangeBPM[x]=nTimeBPM;

		fWChangeBPMPast = fWChangeBPM;
		fBPM = stepc.fChangeBPM[x];

	}
	for(int x = 0; x < stepc.hStep.nSizeStops; x++)
	{
		stepc.fWTStops[x]=pDoc->BeatArrowTime(pDoc->song.nIStops[x]);
		stepc.fStops[x]=pDoc->BeatArrowTime(pDoc->song.nEStops[x])-pDoc->BeatArrowTime(pDoc->song.nIStops[x]);
	}
	for(int x = 0; x < stepc.hStep.nSizeChangeVel; x++)
	{
		stepc.fITChangeVel[x]=pDoc->BeatArrowTime(pDoc->song.nIChangeVel[x]);
		stepc.fETChangeVel[x]=pDoc->BeatArrowTime(pDoc->song.nEChangeVel[x]);
		stepc.fChangeVel[x]=pDoc->song.fChangeVel[x];
		stepc.fIChangeVel[x]=pDoc->song.fIChangeVel[x];
	}
	for(int x = 0; x < stepc.hStep.nSizeJumps; x++)
	{
		stepc.fWTJumps[x]=pDoc->BeatArrowTime(pDoc->song.nIJumps[x]);
		stepc.fJumps[x]=pDoc->BeatArrowEdit(pDoc->song.nEJumps[x])-pDoc->BeatArrowEdit(pDoc->song.nIJumps[x]);
	}

	for(int n = 0; n < stepc.hStep.nSizeArrowGroups; n++)
	{
		int nComp = pDoc->song.hStepGroup[n].hGroup.nSizeArrows;
		int nArrowsPerLine = stepc.hStep.nArrowsPerLine;

		double dWOffset = 0.0;
		int *nAssign = new int[nArrowsPerLine];		// USER: FREEIT
		int *nHold = new int[nArrowsPerLine];		// USER: FREEIT
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			nAssign[x] = x;
			nHold[x] = -1;
		}
		if(m_bMirror)
		{
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				nAssign[x] = g_nAssignMirror[x%g_nAssignMirrorSize]+x/g_nAssignMirrorSize*g_nAssignMirrorSize;
			}
		}
		for(int y = 0; y < nComp; y++)
		{
			int nStopVer = pDoc->VerificateMod(MAXSTEP_STOP, y, y);
			int nJumpVer = pDoc->VerificateMod(MAXSTEP_JUMP, y, y);
			if(pDoc->SomeNote(y, n))
			{
				double dFactor = 1.0;

				// Random asignment if there is a randomstep
				if(m_bRS)
				{
					int nDem = min(nArrowsPerLine, m_nModePlay==MODE_PLAYER_SINGLE?ARROWSLINE_SINGLE:ARROWSLINE_DOUBLE);
					for(int r = 0; r < nDem; r++)
					{
						if(nHold[r] < y)
							nAssign[r] = -1;
					}
					for(int r = 0; r < nDem; r++) if(nAssign[r] == -1)
					{
						bool bOpt = false;
						int nRand = 0;
						do
						{
							nRand = rand()%nDem;
							bOpt = false;
							for(int t = 0; t < nDem; t++)
								if(nAssign[t] == nRand) bOpt = true;
						} while (bOpt);
						nAssign[r] = nRand;
					}
				}

				for(int i = 0; i < nArrowsPerLine; i++)
				{
					int x = nAssign[i];
					int nArrow = pDoc->GetArrow(i, y, n);
					int nPArrow = pDoc->GetArrowPar(i, y, n);
					int nAdd = nArrow;
					int nPAdd = nPArrow;
					if(nJumpVer >= 0)
					{
						if(y != pDoc->song.nIJumps[nJumpVer] && y != pDoc->song.nEJumps[nJumpVer])
							nPAdd = 0;
					}

					// Normal arrows/items or bottom-holds
					if(nArrow == 1 || nArrow == 4 || (nArrow >= ITEM_STARTUP && nArrow <= 20))
					{
						float fUbiD = pDoc->BeatArrowDraw(y);
						__int64 nUbiT = BeatToTime(pDoc->BeatArrowTime(y));
						float fUbiE = pDoc->BeatArrowEdit(y);

						int a = 0;
						a = IncrStepOrVerificate(n, nUbiT, fUbiD);

						stepc.hStepGroup[n].fWDArrows[a] = fUbiD;
						stepc.hStepGroup[n].nWTArrows[a] = nUbiT;

						stepc.hStepGroup[n].nArrows[a*nArrowsPerLine+x] = nAdd;
						stepc.hStepGroup[n].nPArrows[a*nArrowsPerLine+x] = nPAdd;

						for(int j = 0; j < nArrowsPerLine; j++)
						{
							x = nAssign[j];
							if(nHold[i] > y && stepc.hStepGroup[n].nArrows[a*nArrowsPerLine+x] == 0)
							{
								stepc.hStepGroup[n].nArrows[a*nArrowsPerLine+x] = 3;
								stepc.hStepGroup[n].nPArrows[a*nArrowsPerLine+x] = 1;
							}
						}
					}
					// Holds
					else if(nArrow >= 2 && nArrow <= 3 && nHold[i] < y)
					{
						// ** Found foward last Hold => nHold[i]
						nHold[i] = y+1;
						int nArrowj = 0;
						for(int j = y+1; j < nComp; j++)
						{
							nArrowj = pDoc->GetArrow(i, j, n);
							if(nArrowj != 3 && nArrowj != 4) break;
							nHold[i] = j;
							if(nArrowj == 4) break;
						}

						int nJumpVerBottom = pDoc->VerificateMod(MAXSTEP_JUMP, y, nHold[i]);

						// ** Found back TC
						int nTickCount = stepc.hStep.nTickCount;
						for(int j = 0; j < pDoc->song.hStep.nSizeTickCount; j++)
						{
							if(pDoc->song.nWTickCount[j] <= y)
							{
								nTickCount = pDoc->song.nTickCount[j];
							}
							else break;
						}

						double dBeatEdit = pDoc->BeatArrowEdit(y);
						double dBeatEditLast = pDoc->BeatArrowEdit(nHold[i]);
						double dDiffBeat = dBeatEditLast-dBeatEdit;
						int nTimes = int(dDiffBeat*double(nTickCount))+1;

						if(float(nTimes) != (dDiffBeat*double(nTickCount)+1)) nArrowj=3;

						// Add the respective
						for(int j = 0; j < nTimes; j++)
						{
							if(j==(nTimes-1)&&nArrowj!=4) break;

							float fUbiE = float(dBeatEdit)+float(j)/float(nTickCount==0?1:nTickCount);
							float fUbiD = pDoc->BeatArrowDraw(fUbiE);
							__int64 nUbiT = BeatToTime(pDoc->BeatArrowTime(fUbiE));

							int a = IncrStepOrVerificate(n, nUbiT, fUbiD);

							stepc.hStepGroup[n].fWDArrows[a] = fUbiD;
							stepc.hStepGroup[n].nWTArrows[a] = nUbiT;

							// Verificate if there's a jump
							int nJump = -1;
							for(int k = 0; k < pDoc->song.hStep.nSizeJumps; k++)
							{
								if(pDoc->hAux.fIJumps[k] < fUbiE && fUbiE <= pDoc->hAux.fEJumps[k])
								{
									nJump = k;
									break;
								}
							}

							// So then, add the f*ing arrow
							stepc.hStepGroup[n].nArrows[a*nArrowsPerLine+x] = j==0?nArrow:(j==(nTimes-1)&&nArrowj==4?4:3);
							stepc.hStepGroup[n].nPArrows[a*nArrowsPerLine+x] = nJump==-1?nPArrow:0;
						}

					}

				}
			}

		}
		SortStepGroup(n);
	}

	return 1;
}

int CMaxStepSmu::DeleteSong(void)
{
	DeleteCompiler(&stepc);
	return 1;
}

int CMaxStepSmu::CompileTraining(void)
{
	return 1;
}

void CMaxStepSmu::DeleteTrainingCompiler(void)
{
}

void CMaxStepSmu::AllocStatus(void)
{
	status.nComboTotal = 0;
	status.nMaxComboTotal = 0;
	status.nMissComboTotal = 0;
	status.nMaxMissComboTotal = 0;
	status.nScore = 0;				// Puntaje de la canción
	status.nMines = 0;				// Minas
	status.nHearts = 0;			// Corazones
	status.nPotions = 0;			// Pociones
	status.nVelocitys = 0;			// Velocidades
	status.nItems = 0;				// Total items
	status.nBSCorrect = 0;			// Brain Shower Correctas
	status.nBSIncorrect = 0;		// Brain Shower Incorrectas
	status.dLife = 0;			// Barra de vida

	status.nPerfect = (int*)max_realloc(status.nPerfect, stepc.hStep.nSizeArrowGroups*sizeof(int));
	status.nGreat = (int*)max_realloc(status.nGreat, stepc.hStep.nSizeArrowGroups*sizeof(int));
	status.nGood = (int*)max_realloc(status.nGood, stepc.hStep.nSizeArrowGroups*sizeof(int));
	status.nBad = (int*)max_realloc(status.nBad, stepc.hStep.nSizeArrowGroups*sizeof(int));
	status.nMiss = (int*)max_realloc(status.nMiss, stepc.hStep.nSizeArrowGroups*sizeof(int));
	status.nCombo = (int*)max_realloc(status.nCombo, stepc.hStep.nSizeArrowGroups*sizeof(int));
	status.nMaxCombo = (int*)max_realloc(status.nMaxCombo, stepc.hStep.nSizeArrowGroups*sizeof(int));
	status.nMissCombo = (int*)max_realloc(status.nMissCombo, stepc.hStep.nSizeArrowGroups*sizeof(int));
	status.nMaxMissCombo = (int*)max_realloc(status.nMaxMissCombo, stepc.hStep.nSizeArrowGroups*sizeof(int));
	memset(status.nPerfect, 0, stepc.hStep.nSizeArrowGroups*sizeof(int));
	memset(status.nGreat, 0, stepc.hStep.nSizeArrowGroups*sizeof(int));
	memset(status.nGood, 0, stepc.hStep.nSizeArrowGroups*sizeof(int));
	memset(status.nBad, 0, stepc.hStep.nSizeArrowGroups*sizeof(int));
	memset(status.nMiss, 0, stepc.hStep.nSizeArrowGroups*sizeof(int));
	memset(status.nCombo, 0, stepc.hStep.nSizeArrowGroups*sizeof(int));
	memset(status.nMaxCombo, 0, stepc.hStep.nSizeArrowGroups*sizeof(int));
	memset(status.nMissCombo, 0, stepc.hStep.nSizeArrowGroups*sizeof(int));
	memset(status.nMaxMissCombo, 0, stepc.hStep.nSizeArrowGroups*sizeof(int));

	status.nArrowsStatus=(int**)max_realloc(status.nArrowsStatus, stepc.hStep.nSizeArrowGroups*sizeof(int*));
	for(int n=0; n<stepc.hStep.nSizeArrowGroups; n++)
	{
		status.nArrowsStatus[n] = (int*)calloc(stepc.hStepGroup[n].hGroup.nSizeArrows*stepc.hStep.nArrowsPerLine, sizeof(int));
		for(int u = 0; u < stepc.hStepGroup[n].hGroup.nSizeArrows*stepc.hStep.nArrowsPerLine; u++)
			status.nArrowsStatus[n][u] = -1;
	}
	status.nBSStatus=(int*)max_realloc(status.nBSStatus, bsc.nSizeBSAnswers*stepc.hStep.nArrowsPerLine*sizeof(int));
	memset(status.nBSStatus, 0, bsc.nSizeBSAnswers*stepc.hStep.nArrowsPerLine*sizeof(int));
}

void CMaxStepSmu::DeleteStatus(void)
{
	for(int n=0; n<stepc.hStep.nSizeArrowGroups; n++)
	{
		free(status.nArrowsStatus[n]);
	}
}

int CMaxStepSmu::GetArrow(int x, int y, int nGArrows)
{
	if(nGArrows < 0 || nGArrows >= stepc.hStep.nSizeArrowGroups) return 0;
	if(y < 0 || y >= stepc.hStepGroup[nGArrows].hGroup.nSizeArrows) return 0;
	if(x < 0 || x >= stepc.hStep.nArrowsPerLine) return 0;
	return stepc.hStepGroup[nGArrows].nArrows[y*stepc.hStep.nArrowsPerLine+x];
}

int CMaxStepSmu::GetArrowPar(int x, int y, int nGArrows)
{
	if(nGArrows < 0 || nGArrows >= stepc.hStep.nSizeArrowGroups) return 0;
	if(y < 0 || y >= stepc.hStepGroup[nGArrows].hGroup.nSizeArrows) return 0;
	if(x < 0 || x >= stepc.hStep.nArrowsPerLine) return 0;
	return stepc.hStepGroup[nGArrows].nPArrows[y*stepc.hStep.nArrowsPerLine+x];
}

__int64 CMaxStepSmu::BeatWArrow(int nIndex, int nGArrows)
{
	if(nGArrows < 0 || nGArrows >= stepc.hStep.nSizeArrowGroups) return 0;
	int y = nIndex;
	if(y < 0)
		return stepc.hStepGroup[nGArrows].nWTArrows[0];
	if(y >= stepc.hStepGroup[nGArrows].hGroup.nSizeArrows)
		return stepc.hStepGroup[nGArrows].nWTArrows[stepc.hStepGroup[nGArrows].hGroup.nSizeArrows-1];
	return stepc.hStepGroup[nGArrows].nWTArrows[y];
}

void CMaxStepSmu::Score_DoJudgament(int y, int nGArrows, int nJudge, int nCombo)	// USER:DEF
{
	if(!nCombo) return;
	if(stepc.hStepGroup[nGArrows].hGroup.nNoMiss && nJudge == JUDAMENT_MISS) return;
	evarrows.nLastEva = m_nTime;
	evarrows.nTypeEva = JUDAMENT_MISS-nJudge+1;
	int n=nGArrows;
	switch(nJudge)
	{
	case JUDAMENT_BAD:
		status.nBad[n]++;
		status.nScore += PUNT_EVA4;	// USER
		status.nCombo[n] = 0;
		status.nComboTotal = 0;
		status.dLife -= m_dDecrBar*0.5;
		break;
	case JUDAMENT_GOOD:
		status.nGood[n]++;
		status.nScore += PUNT_EVA3; // USER
		PlusCombo();
		status.nMissCombo[n] = 0;
		status.nMissComboTotal = 0;
		break;
	case JUDAMENT_GREAT:
		status.nGreat[n]+=nCombo;
		status.nCombo[n]+=nCombo;
		status.nComboTotal+=nCombo;
		status.nScore += PUNT_EVA2*nCombo;	// USER
		PlusCombo();
		if(status.nCombo[n] > status.nMaxCombo[n]) status.nMaxCombo[n] = status.nCombo[n];
		if(status.nComboTotal > status.nMaxComboTotal) status.nMaxComboTotal = status.nComboTotal;
		status.dLife += m_dIncrBar*0.5*double(nCombo);
		status.nMissCombo[n] = 0;
		status.nMissComboTotal = 0;
		break;
	case JUDAMENT_PERFECT:
		status.nPerfect[n]+=nCombo;
		status.nScore += PUNT_EVA1*nCombo; // USER
		PlusCombo();
		status.nCombo[n]+=nCombo;
		status.nComboTotal+=nCombo;
		if(status.nCombo[n] > status.nMaxCombo[n]) status.nMaxCombo[n] = status.nCombo[n];
		if(status.nComboTotal > status.nMaxComboTotal) status.nMaxComboTotal = status.nComboTotal;
		status.dLife += m_dIncrBar*double(nCombo);
		status.nMissCombo[n] = 0;
		status.nMissComboTotal = 0;
		break;
	case JUDAMENT_MISS:
		status.nMiss[n]++;
		status.nScore+=PUNT_EVA5;	// USER
		MinusCombo();
		status.dLife -= m_dDecrBar;
		status.nCombo[n] = 0;
		status.nComboTotal = 0;
		status.nMissCombo[n]++;
		status.nMissComboTotal++;
		if(status.nMissCombo[n] > status.nMaxMissCombo[n]) status.nMaxMissCombo[n] = status.nMissCombo[n];
		if(status.nMissComboTotal > status.nMaxMissComboTotal) status.nMaxMissComboTotal = status.nMissComboTotal;
		break;
	}
}

void CMaxStepSmu::Score_DoItem(int y, int x, int nGArrows, int nType, int nPar)
{
	// USER:DEF
	double dMult = double(nPar);
	if(nPar < 0) dMult = 1/double(-nPar);
	switch(nType)
	{
	case 5:	// Minas
		status.nItems++;
		status.dLife -= dMult*m_dDecrBar;	// USER
		status.nMines++;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nTime;
		g_ssMain->PlaySound(CHANNEL_STEPSMU_SOUNDS, m_srMain->At(STEPSMUSRES_MINE), false, channels[CHANNEL_STEPSMU_SOUNDS]);
		break;
	case 6:	// Pociones
		status.nItems++;
		status.dLife += dMult*m_dDecrBar;
		status.nPotions++;
		break;
	case 7:	// Corazones
		status.nItems++;
		status.nHearts++;
		break;
	case 8:	// Flash
		status.nItems++;
		m_bFlash = true;
		break;
	case 9:	// Back to Normal
		status.nItems++;
		m_bFlash = false;
		m_bSnake = false;
		m_bLeftAttack = false;
		m_bRightAttack = false;
		m_bUnderAttack = false;
		m_bDrop = false;
		m_bXMode = false;
		m_bNXMode = false;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nTime;
		break;
	case 10:	// Velocidad x1
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 1.0;
		evarrows.dWWVel = m_dBeatDraw;
		evarrows.bVel = true;
		break;
	case 11:	// Velocidad x2
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 2.0;
		evarrows.dWWVel = m_dBeatDraw;
		evarrows.bVel = true;
		break;
	case 12:	// Velocidad x3
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 3.0;
		evarrows.dWWVel = m_dBeatDraw;
		evarrows.bVel = true;
		break;
	case 13:	// Velocidad x4
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 4.0;
		evarrows.dWWVel = m_dBeatDraw;
		evarrows.bVel = true;
		break;
	case 14:	// Velocidad x5
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 5.0;
		evarrows.dWWVel = m_dBeatDraw;
		evarrows.bVel = true;
		break;
	case 15:	// Velocidad x8
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 8.0;
		evarrows.dWWVel = m_dBeatDraw;
		evarrows.bVel = true;
		break;
	case 16:	// Drop
		status.nItems++;
		m_bDrop = true;
		m_bUnderAttack = false;
		m_bLeftAttack = false;
		m_bRightAttack = false;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nTime;
		break;
	case 17:	// UnderAttack
		status.nItems++;
		m_bDrop = false;
		m_bUnderAttack = true;
		m_bLeftAttack = false;
		m_bRightAttack = false;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nTime;
		break;
	case 18:	// LeftAttack
		status.nItems++;
		m_bDrop = false;
		m_bUnderAttack = false;
		m_bLeftAttack = true;
		m_bRightAttack = false;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nTime;
		break;
	case 19:	// RightAttack
		status.nItems++;
		m_bDrop = false;
		m_bUnderAttack = false;
		m_bLeftAttack = false;
		m_bRightAttack = true;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nTime;
		break;
	case 20:	// Snake
		status.nItems++;
		m_bSnake = true;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nTime;
		break;
	}
}

void CMaxStepSmu::Score_DoNotItem(int y, int x, int nGArrows, int nType, int nPar)
{	// USER:DEF
}

int CMaxStepSmu::IncrStepOrVerificate(int n, __int64 nUbiT, float fUbiD)
{
	int a = -1;
	int nArrowsPerLine = stepc.hStep.nArrowsPerLine;
	// Verificate if exists any arrow at the same position
	for(int i = 0; i < stepc.hStepGroup[n].hGroup.nSizeArrows; i++)
	{
		if(stepc.hStepGroup[n].nWTArrows[i] == nUbiT && stepc.hStepGroup[n].fWDArrows[i] == fUbiD)
		{
			a = i;
			break;
		}
	}

	// If doesn't exists, create it
	if(a == -1)
	{
		a = IncrStep(n);
	}
	return a;
}

int CMaxStepSmu::IncrStep(int n)
{
	int nArrowsPerLine = stepc.hStep.nArrowsPerLine;
	int a = stepc.hStepGroup[n].hGroup.nSizeArrows;
	stepc.hStepGroup[n].hGroup.nSizeArrows++;
	ReInitCompiler(&stepc);
	for(int z = 0; z < nArrowsPerLine; z++)
	{
		stepc.hStepGroup[n].nArrows[a*nArrowsPerLine+z] = 0;
		stepc.hStepGroup[n].nPArrows[a*nArrowsPerLine+z] = 0;
	}
	stepc.hStepGroup[n].fWDArrows[a] = 0;
	stepc.hStepGroup[n].nWTArrows[a] = 0;
	return a;
}

void CMaxStepSmu::SortStepGroup(int n)
{
	int nArrowsPerLine = stepc.hStep.nArrowsPerLine;
	int* nAT = new int[nArrowsPerLine];
	for(int i = 0; i < stepc.hStepGroup[n].hGroup.nSizeArrows; i++)
	{
		for(int j = 0; j < stepc.hStepGroup[n].hGroup.nSizeArrows-1; j++)
		{
			if(stepc.hStepGroup[n].nWTArrows[j] > stepc.hStepGroup[n].nWTArrows[j+1])
			{
				__int64 nTemp;
				float fTemp;

				nTemp = stepc.hStepGroup[n].nWTArrows[j];
				stepc.hStepGroup[n].nWTArrows[j] = stepc.hStepGroup[n].nWTArrows[j+1];
				stepc.hStepGroup[n].nWTArrows[j+1] = nTemp;

				fTemp = stepc.hStepGroup[n].fWDArrows[j];
				stepc.hStepGroup[n].fWDArrows[j] = stepc.hStepGroup[n].fWDArrows[j+1];
				stepc.hStepGroup[n].fWDArrows[j+1] = fTemp;

				memcpy(nAT, stepc.hStepGroup[n].nArrows+j*nArrowsPerLine, nArrowsPerLine*sizeof(int));
				memcpy(stepc.hStepGroup[n].nArrows+j*nArrowsPerLine, stepc.hStepGroup[n].nArrows+(j+1)*nArrowsPerLine, nArrowsPerLine*sizeof(int));
				memcpy(stepc.hStepGroup[n].nArrows+(j+1)*nArrowsPerLine, nAT, nArrowsPerLine*sizeof(int));

				memcpy(nAT, stepc.hStepGroup[n].nPArrows+j*nArrowsPerLine, nArrowsPerLine*sizeof(int));
				memcpy(stepc.hStepGroup[n].nPArrows+j*nArrowsPerLine, stepc.hStepGroup[n].nPArrows+(j+1)*nArrowsPerLine, nArrowsPerLine*sizeof(int));
				memcpy(stepc.hStepGroup[n].nPArrows+(j+1)*nArrowsPerLine, nAT, nArrowsPerLine*sizeof(int));


			}
		}
	}
	delete nAT;
}

// ** THIS FUNCTIONS ARE DEPRECATED

int fcombo(int k)
{
	if(k == 0) return 0;
	int x = k/5;
	int y = (k-1)%5;
	int nx = 0;
	if(y == 0) nx = 100;
	else if(y == 1) nx = 200;
	else if(y == 2) nx = 300;
	else if(y == 3) nx = 400;
	else if(y == 4) nx = 500;
	int ny = (int)pow(10, double(x));
	return nx*ny;
}

void CMaxStepSmu::MinusCombo(void)
{
	if(status.nScore < 0) status.nScore = 0;
}

void CMaxStepSmu::PlusCombo(void)
{
	status.nScore += fcombo(min(status.nComboTotal/50, max((stepc.hStep.nDificulty-5)/2, 0)));
}

void CMaxStepSmu::SetArg(ARGSTEP* arg)
{
	m_bAutoPlay = arg->bAutoPlay;
	m_bIsAsistTick= arg->bAssistTick;

	/*m_bCharacter = arg->bCharacter;
	m_nLevelCharacer = arg->nLevelCharacer;
	m_nTypeCharacter = arg->nTypeCharacter;
	m_nCharacter = arg->nCharacter;*/

	m_nLevelMach = arg->nLevelMach;

	if(arg->bIsSingle) m_nModePlay = MODE_PLAYER_SINGLE;
	else m_nModePlay = MODE_PLAYER_DOUBLE;
	m_bTrainingMode = arg->bTrainingMode;
	m_nSkin = arg->nSkin;
	m_bXMode = arg->bXMode;
	m_bNXMode = arg->bNXMode;
	m_fRush = arg->fRush;

	for(int j = 0; j < m_nSizeJudTimes; j++)
	{
		if(arg->bEJ) m_nJudTimes[j] = int(float(nTIMIMG_EASY[j])*m_fRush);
		else if(arg->bHJ) m_nJudTimes[j] = int(float(nTIMIMG_HARD[j])*m_fRush);
		else  m_nJudTimes[j] = int(float(nTIMIMG_NORMAL[j])*m_fRush);
	}

	m_bRS = arg->bRS;
	m_bMirror = arg->bMirror;
	m_bFreedom = arg->bFreedom;
	m_bRV = arg->bRV;
	m_bRandomSkin = arg->bRandomSkin;
	m_fEVanish = arg->fEVanish;
	m_fIVanish = arg->fIVanish;
	m_bDC = arg->bDC;
	m_bAC = arg->bAC;
	m_bEW = arg->bEW;
	m_bFlash = arg->bFlash;
	m_bUnderAttack = arg->bUnderAttack;
	m_bLeftAttack = arg->bLeftAttack;
	m_bRightAttack = arg->bRightAttack;
	m_bDrop = arg->bDrop;
	m_bSnake = arg->bSnake;
	//m_bSink = arg->bSink;
	//m_bRise = arg->bRise;
	//m_bRG = arg->bJR;
	m_dUserVel = arg->fUserVel;
	m_dIncrBar = arg->fIncrBar;
	m_dDecrBar = arg->fDecrBar;
}

void CMaxStepSmu::DoAssistTick(void)
{
	if(m_bIsAsistTick)
	{
		bool bTick = false;
		for(int y = 0; y < m_nSizeATArray; y++)
		{
			if(m_nTime>=m_nWTATArray[y] && !m_bATArray[y])
			{
				m_bATArray[y]=1;
				bTick = true;
			}
		}
		if(bTick)
		{
			g_ssMain->PlaySound(CHANNEL_STEPSMU_ASSISTTICK, m_srMain->At(STEPSMUSRES_ASSISTTICK),0,channels[CHANNEL_STEPSMU_ASSISTTICK]);
		}
	}
}

void CMaxStepSmu::CompileAssistTick(void)
{
	int nArrowsPerLine = stepc.hStep.nArrowsPerLine;
	for(int n = 0; n < stepc.hStep.nSizeArrowGroups; n++)
	{
		for(int y = 0; y < stepc.hStepGroup[n].hGroup.nSizeArrows; y++)
		{
			for(int x = 0; x <
				min(nArrowsPerLine,
					m_nModePlay==MODE_PLAYER_SINGLE?ARROWSLINE_SINGLE:ARROWSLINE_DOUBLE);
				x++)
			{
				int nCheck = GetArrow(x, y, n);
				if(nCheck==1||nCheck==2)
				{
					__int64 nTime = stepc.hStepGroup[n].nWTArrows[y];
					int a = IncrAssistTickOrVerificate(nTime);
					m_bATArray[a] = m_nTime>nTime?1:0;
					m_nWTATArray[a] = nTime;
					break;
				}
			}
		}
	}
}

int CMaxStepSmu::IncrAssistTickOrVerificate(__int64 nTime)
{
	int a = -1;
	for(int y = 0; y < m_nSizeATArray; y++)
	{
		if(m_nWTATArray[y] == nTime)
		{
			a = y;
			break;
		}
	}

	if(a == -1)
	{
		a = m_nSizeATArray;
		m_nSizeATArray++;
		m_bATArray = (BYTE*)max_realloc(m_bATArray, m_nSizeATArray*sizeof(BYTE));
		m_nWTATArray = (__int64*)max_realloc(m_nWTATArray, m_nSizeATArray*sizeof(__int64));
	}
	return a;
}
