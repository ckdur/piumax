// stepsmu.cpp: define las funciones exportadas de la aplicación DLL.
//

//#include "stdafx.h"
#include "stepsmu.h"
#include <vector>
using std::vector;

#define JUDAMENT_PERFECT 5
#define JUDAMENT_GREAT 4
#define JUDAMENT_GOOD 3
#define JUDAMENT_BAD 2
#define JUDAMENT_MISS 1

// Helpers
#define DIST_ARROW(x) (x<5?(x*m_nSizeArrows*0.95):(4*m_nSizeArrows*0.95+0.5+m_nSizeArrows+(x-5)*m_nSizeArrows*0.95))
#define MAX_BAR 1.2
///////////

// ** Global Variables
DWORD dwStepSmu = 0;
GLuint texBGA;
bool m_bSplash;
bool bFirstUseBGA = false;
bool g_bIsMusic = false;
bool g_bDraw = false;
unsigned int nPos = 0;
bool g_bPlayer[] = {false, false};
DWORD dwTickLoad = 0;
#define MIN_TIME_INIT 4000

FMOD_SOUND* sound_mine;
FMOD_RESULT       res_step;
FMOD_SOUND* music_s;
CMovieStream2 *backMovie;
CSKSFView* viewp1;
CSKSFView* viewp2;
CSKSFDoc* doc;
CSKSFDoc* GetDocument()
{
	return doc;
}
CMaxLights* songLight;
//////////////////////

CSKSFView::CSKSFView()
: ox(0)
, oy(0)
, m_nNext(0)
, m_dUserVel(1)
, m_dMachVel(1)
, m_dActionVel(1)
, m_bDrop(false)
, m_nSizeArrows(0)
, m_dLevelMach(0.5)
, m_nLevelMach(500)
, m_bLoadMachine(false)
, m_bTrainingMode(false)
, m_nCharacter(0)
, m_bCharacter(false)
, m_nAuxBeginCharacterStepL(0)
, m_nAuxBeginCharacterStepR(0)
, m_nAttack(0)
, m_bXMode(false)
, m_bNXMode(false)
, m_bHJ(false)
, m_bRS(false)
, m_bMirror(false)
, m_bFreedom(false)
, m_bRV(false)
, m_bRandomSkin(false)
, m_nSkin(-1)
, m_nVanish(-1)
, m_bDC(false)
, m_bAC(false)
, m_bEW(false)
, m_bUnderAttack(false)
, m_dPassVel(1.0)
, m_bRVel(false)
, m_bFlash(false)
, dBeat(0)
, dBeatnStop(0)
, m_bIsSingle(false)
, m_bLeftAttack(false)
, m_bRightAttack(false)
, m_bSnake(false)
, m_dClipMax1(-1.0)
, m_dClipMax2(-1.0)
, m_dClipMax3(-1.0)
, m_dClipMin1(2.0)
, m_dClipMin2(2.0)
, m_dClipMin3(2.0)
, m_nPlayer(0)
, m_bAutoPlay(false)
, m_bIsCharged(false)
, m_nHlpAuto(0)
, m_nLevelCharacter(0)
, m_nLevelSong(0)
, m_nTypeCharacter(0)
, dIncrBar(0)
, dDecrBar(0)
, PUNT_EVA1(1500)
, PUNT_EVA2(1000)
, PUNT_EVA3(500)
, PUNT_EVA4(0)
, PUNT_EVA5(0)
, dPowTimer(1.0)
{
	skin[0] = new CSkin();
	skin[1] = new CSkin();
	skin[2] = new CSkin();
	common = new CSkinCommon();
	bs = new CSkinBS();
	bsf = new CGLFont();

	status.nArrowsStatus1=(int*)calloc(0, sizeof(int));
	status.nArrowsStatus2=(int*)calloc(0, sizeof(int));
	status.nArrowsStatus3=(int*)calloc(0, sizeof(int));
	status.nBSStatus1=(int*)calloc(0, sizeof(int));

	songc.nSizeArrows1 = 0;
	songc.nSizeArrows2 = 0;
	songc.nSizeArrows3 = 0;
	songc.nSizeChangeBPM = 0;
	songc.nSizeChangeVel = 0;
	songc.nSizeJumps = 0;
	songc.nSizeStops = 0;
	songc.dwArrows1=(DWORD*)calloc(songc.nSizeArrows1*10,sizeof(DWORD));
	songc.dwArrows2=(DWORD*)calloc(songc.nSizeArrows2*10,sizeof(DWORD));
	songc.dwArrows3=(DWORD*)calloc(songc.nSizeArrows3*10,sizeof(DWORD));
	songc.dWArrows1=(double*)calloc(songc.nSizeArrows1,sizeof(double));
	songc.dWArrows2=(double*)calloc(songc.nSizeArrows2,sizeof(double));
	songc.dWArrows3=(double*)calloc(songc.nSizeArrows3,sizeof(double));
	songc.dWWArrows1=(double*)calloc(songc.nSizeArrows1,sizeof(double));
	songc.dWWArrows2=(double*)calloc(songc.nSizeArrows2,sizeof(double));
	songc.dWWArrows3=(double*)calloc(songc.nSizeArrows3,sizeof(double));
	songc.dWChangeBPM=(double*)calloc(songc.nSizeChangeBPM,sizeof(double));
	songc.dChangeBPM=(double*)calloc(songc.nSizeChangeBPM,sizeof(double));
	songc.dIChangeVel=(double*)calloc(songc.nSizeChangeVel,sizeof(double));
	songc.dEChangeVel=(double*)calloc(songc.nSizeChangeVel,sizeof(double));
	songc.dChangeVel=(double*)calloc(songc.nSizeChangeVel,sizeof(double));
	songc.dIJumps=(double*)calloc(songc.nSizeJumps,sizeof(double));
	songc.dEJumps=(double*)calloc(songc.nSizeJumps,sizeof(double));
	songc.dWStops=(double*)calloc(songc.nSizeStops,sizeof(double));
	songc.dStops=(double*)calloc(songc.nSizeStops,sizeof(double));

	bsc.nSizeBSQuestions=0;
	bsc.nSizeBSAnswers=0;
	bsc.nSizeBSPopUp=0;
	bsc.dWIBSQuestions = (double*)calloc(bsc.nSizeBSQuestions,sizeof(double));
	bsc.dWEBSQuestions = (double*)calloc(bsc.nSizeBSQuestions,sizeof(double));
	bsc.dWABSQuestions = (double*)calloc(bsc.nSizeBSQuestions,sizeof(double));
	bsc.dwBSQuestions = (DWORD*)calloc(bsc.nSizeBSQuestions,sizeof(DWORD));
	bsc.dwDBSQuestionsEx = (DWORD*)calloc(bsc.nSizeBSQuestions*40,sizeof(DWORD));
	bsc.dwDBSQuestions = (DWORD*)calloc(bsc.nSizeBSQuestions*30,sizeof(DWORD));
	bsc.dWBSAnswers = (double*)calloc(bsc.nSizeBSAnswers,sizeof(double));
	bsc.dWWBSAnswers = (double*)calloc(bsc.nSizeBSAnswers,sizeof(double));
	bsc.dwBSAnswers = (DWORD*)calloc(bsc.nSizeBSAnswers*10,sizeof(DWORD));
	bsc.nBSAnswers = (int*)calloc(bsc.nSizeBSAnswers*10,sizeof(int));
	bsc.nDBSAnswers = (int*)calloc(bsc.nSizeBSAnswers,sizeof(int));
	bsc.dWBSPopUp = (double*)calloc(bsc.nSizeBSPopUp,sizeof(double));
	bsc.dwBSPopUp = (DWORD*)calloc(bsc.nSizeBSPopUp,sizeof(DWORD));

	for(int x = 0; x < 3; x++)
	{
		effect.nSizeVanishChanges[x] = 0;
		effect.nSizeAttackChanges[x] = 0;
		effect.nSizeZigZagChanges[x] = 0;
		effect.dWVanishChanges[x] = (double*)calloc(effect.nSizeVanishChanges[x],sizeof(double));
		effect.dWAttackChanges[x] = (double*)calloc(effect.nSizeAttackChanges[x],sizeof(double));
		effect.dWZigZagChanges[x] = (double*)calloc(effect.nSizeZigZagChanges[x],sizeof(double));
		effect.dwVanishChanges[x] = (DWORD*)calloc(effect.nSizeVanishChanges[x],sizeof(DWORD));
		effect.dwAttackChanges[x] = (DWORD*)calloc(effect.nSizeAttackChanges[x],sizeof(DWORD));
		effect.dwZigZagChanges[x] = (DWORD*)calloc(effect.nSizeZigZagChanges[x],sizeof(DWORD));
	}

	memset(&machine.hMachine, 0, sizeof(MACHINE_HEADER));
	machine.dVtxMachine[0] = (double*)calloc(machine.hMachine.nSizeVtxMachine[0]*3,sizeof(double));
	machine.dVtxMachine[1] = (double*)calloc(machine.hMachine.nSizeVtxMachine[1]*3,sizeof(double));
	machine.nFaceMachine[0] = (int*)calloc(machine.hMachine.nSizeFaceMachine[0]*3,sizeof(int));
	machine.nFaceMachine[1] = (int*)calloc(machine.hMachine.nSizeFaceMachine[1]*3,sizeof(int));
	machine.dUVTMachine[0] = (double*)calloc(machine.hMachine.nSizeUVTMachine[0]*3,sizeof(double));
	machine.dUVTMachine[1] = (double*)calloc(machine.hMachine.nSizeUVTMachine[1]*3,sizeof(double));
	machine.nTMachine[0] = (int*)calloc(machine.hMachine.nSizeTMachine[0]*3,sizeof(int));
	machine.nTMachine[1] = (int*)calloc(machine.hMachine.nSizeTMachine[1]*3,sizeof(int));
	for(int x = 0; x < 10; x++)
	{
		machine.dVtxTap[x] = (double*)calloc(machine.hMachine.nSizeVtxTap[x]*3,sizeof(double));
		machine.nFaceTap[x] = (int*)calloc(machine.hMachine.nSizeFaceTap[x]*3,sizeof(int));
		machine.dUVTTap[x] = (double*)calloc(machine.hMachine.nSizeUVTTap[x]*3,sizeof(double));
		machine.nTTap[x] = (int*)calloc(machine.hMachine.nSizeTTap[x]*3,sizeof(int));
	}
	trac.nSizeArg = 0;
	trac.nArg = (int*)calloc(trac.nSizeArg*10,sizeof(int));
	memset(&crt.hCtr, 0, sizeof(CHARACTER_HEADER));
	crt.nArrows1 = (int*)calloc(crt.hCtr.nSizeArrows1*10,sizeof(int));
	crt.nArrows2 = (int*)calloc(crt.hCtr.nSizeArrows2*10,sizeof(int));
	crt.dWWArrows1 = (double*)calloc(crt.hCtr.nSizeArrows1,sizeof(double));
	crt.dWWArrows2 = (double*)calloc(crt.hCtr.nSizeArrows2,sizeof(double));
	crt.dVtxLFoot = (double*)calloc(crt.hCtr.nSizeVtxLFoot,sizeof(double));
	crt.dVtxRFoot = (double*)calloc(crt.hCtr.nSizeVtxRFoot,sizeof(double));
	crt.nFaceLFoot = (int*)calloc(crt.hCtr.nSizeFaceLFoot,sizeof(int));
	crt.nFaceRFoot = (int*)calloc(crt.hCtr.nSizeFaceRFoot,sizeof(int));
	crt.dUVTLFoot = (double*)calloc(crt.hCtr.nSizeUVTLFoot,sizeof(double));
	crt.dUVTRFoot = (double*)calloc(crt.hCtr.nSizeUVTRFoot,sizeof(double));
	crt.nTLFoot = (int*)calloc(crt.hCtr.nSizeTLFoot,sizeof(int));
	crt.nTRFoot = (int*)calloc(crt.hCtr.nSizeTRFoot,sizeof(int));
	crt.dwArrowsAP = (DWORD*)calloc(crt.hCtr.nSizeArrowsAP,sizeof(DWORD));
	crt.dWWArrowsAP = (double*)calloc(crt.hCtr.nSizeArrowsAP,sizeof(double));

	crt.dVtxDLLeg = (double*)calloc(crt.hCtr.nSizeVtxDLLeg,sizeof(double));
	crt.nFaceDLLeg = (int*)calloc(crt.hCtr.nSizeFaceDLLeg,sizeof(int));
	crt.dUVTDLLeg = (double*)calloc(crt.hCtr.nSizeUVTDLLeg,sizeof(double));
	crt.nTDLLeg = (int*)calloc(crt.hCtr.nSizeTDLLeg,sizeof(int));

	crt.dVtxDRLeg = (double*)calloc(crt.hCtr.nSizeVtxDRLeg,sizeof(double));
	crt.nFaceDRLeg = (int*)calloc(crt.hCtr.nSizeFaceDRLeg,sizeof(int));
	crt.dUVTDRLeg = (double*)calloc(crt.hCtr.nSizeUVTDRLeg,sizeof(double));
	crt.nTDRLeg = (int*)calloc(crt.hCtr.nSizeTDRLeg,sizeof(int));

	crt.dVtxULLeg = (double*)calloc(crt.hCtr.nSizeVtxULLeg,sizeof(double));
	crt.nFaceULLeg = (int*)calloc(crt.hCtr.nSizeFaceULLeg,sizeof(int));
	crt.dUVTULLeg = (double*)calloc(crt.hCtr.nSizeUVTULLeg,sizeof(double));
	crt.nTULLeg = (int*)calloc(crt.hCtr.nSizeTULLeg,sizeof(int));

	crt.dVtxURLeg = (double*)calloc(crt.hCtr.nSizeVtxURLeg,sizeof(double));
	crt.nFaceURLeg = (int*)calloc(crt.hCtr.nSizeFaceURLeg,sizeof(int));
	crt.dUVTURLeg = (double*)calloc(crt.hCtr.nSizeUVTURLeg,sizeof(double));
	crt.nTURLeg = (int*)calloc(crt.hCtr.nSizeTURLeg,sizeof(int));

	crt.dVtxHip = (double*)calloc(crt.hCtr.nSizeVtxHip,sizeof(double));
	crt.nFaceHip = (int*)calloc(crt.hCtr.nSizeFaceHip,sizeof(int));
	crt.dUVTHip = (double*)calloc(crt.hCtr.nSizeUVTHip,sizeof(double));
	crt.nTHip = (int*)calloc(crt.hCtr.nSizeTHip,sizeof(int));

	crt.dVtxWaist = (double*)calloc(crt.hCtr.nSizeVtxWaist,sizeof(double));
	crt.nFaceWaist = (int*)calloc(crt.hCtr.nSizeFaceWaist,sizeof(int));
	crt.dUVTWaist = (double*)calloc(crt.hCtr.nSizeUVTWaist,sizeof(double));
	crt.nTWaist = (int*)calloc(crt.hCtr.nSizeTWaist,sizeof(int));

	crt.dVtxChest = (double*)calloc(crt.hCtr.nSizeVtxChest,sizeof(double));
	crt.nFaceChest = (int*)calloc(crt.hCtr.nSizeFaceChest,sizeof(int));
	crt.dUVTChest = (double*)calloc(crt.hCtr.nSizeUVTChest,sizeof(double));
	crt.nTChest = (int*)calloc(crt.hCtr.nSizeTChest,sizeof(int));

	crt.dVtxHead = (double*)calloc(crt.hCtr.nSizeVtxHead,sizeof(double));
	crt.nFaceHead = (int*)calloc(crt.hCtr.nSizeFaceHead,sizeof(int));
	crt.dUVTHead = (double*)calloc(crt.hCtr.nSizeUVTHead,sizeof(double));
	crt.nTHead = (int*)calloc(crt.hCtr.nSizeTHead,sizeof(int));

	crt.dVtxULArm = (double*)calloc(crt.hCtr.nSizeVtxULArm,sizeof(double));
	crt.nFaceULArm = (int*)calloc(crt.hCtr.nSizeFaceULArm,sizeof(int));
	crt.dUVTULArm = (double*)calloc(crt.hCtr.nSizeUVTULArm,sizeof(double));
	crt.nTULArm = (int*)calloc(crt.hCtr.nSizeTULArm,sizeof(int));

	crt.dVtxDLArm = (double*)calloc(crt.hCtr.nSizeVtxDLArm,sizeof(double));
	crt.nFaceDLArm = (int*)calloc(crt.hCtr.nSizeFaceDLArm,sizeof(int));
	crt.dUVTDLArm = (double*)calloc(crt.hCtr.nSizeUVTDLArm,sizeof(double));
	crt.nTDLArm = (int*)calloc(crt.hCtr.nSizeTDLArm,sizeof(int));

	crt.dVtxLHand = (double*)calloc(crt.hCtr.nSizeVtxLHand,sizeof(double));
	crt.nFaceLHand = (int*)calloc(crt.hCtr.nSizeFaceLHand,sizeof(int));
	crt.dUVTLHand = (double*)calloc(crt.hCtr.nSizeUVTLHand,sizeof(double));
	crt.nTLHand = (int*)calloc(crt.hCtr.nSizeTLHand,sizeof(int));

	crt.dVtxURArm = (double*)calloc(crt.hCtr.nSizeVtxURArm,sizeof(double));
	crt.nFaceURArm = (int*)calloc(crt.hCtr.nSizeFaceURArm,sizeof(int));
	crt.dUVTURArm = (double*)calloc(crt.hCtr.nSizeUVTURArm,sizeof(double));
	crt.nTURArm = (int*)calloc(crt.hCtr.nSizeTURArm,sizeof(int));

	crt.dVtxDRArm = (double*)calloc(crt.hCtr.nSizeVtxDRArm,sizeof(double));
	crt.nFaceDRArm = (int*)calloc(crt.hCtr.nSizeFaceDRArm,sizeof(int));
	crt.dUVTDRArm = (double*)calloc(crt.hCtr.nSizeUVTDRArm,sizeof(double));
	crt.nTDRArm = (int*)calloc(crt.hCtr.nSizeTDRArm,sizeof(int));

	crt.dVtxRHand = (double*)calloc(crt.hCtr.nSizeVtxRHand,sizeof(double));
	crt.nFaceRHand = (int*)calloc(crt.hCtr.nSizeFaceRHand,sizeof(int));
	crt.dUVTRHand = (double*)calloc(crt.hCtr.nSizeUVTRHand,sizeof(double));
	crt.nTRHand = (int*)calloc(crt.hCtr.nSizeTRHand,sizeof(int));
}

CSKSFView::~CSKSFView()
{
	//delete dwArrowsClip;
	delete skin[0];
	delete skin[1];
	delete skin[2];
	delete common;
	delete bs;
	delete bsf;
	delete status.nArrowsStatus1;
	delete status.nArrowsStatus2;
	delete status.nArrowsStatus3;
	delete status.nBSStatus1;
	delete songc.dwArrows1;
	delete songc.dwArrows2;
	delete songc.dwArrows3;
	delete songc.dWArrows1;
	delete songc.dWArrows2;
	delete songc.dWArrows3;
	delete songc.dWWArrows1;
	delete songc.dWWArrows2;
	delete songc.dWWArrows3;
	delete songc.dWChangeBPM;
	delete songc.dChangeBPM;
	delete songc.dIChangeVel;
	delete songc.dEChangeVel;
	delete songc.dChangeVel;
	delete songc.dIJumps;
	delete songc.dEJumps;
	delete songc.dWStops;
	delete songc.dStops;
	delete bsc.dWIBSQuestions;
	delete bsc.dWEBSQuestions;
	delete bsc.dWABSQuestions;
	delete bsc.dwBSQuestions;
	delete bsc.dwDBSQuestionsEx;
	delete bsc.dwDBSQuestions;
	delete bsc.dWBSAnswers;
	delete bsc.dWWBSAnswers;
	delete bsc.dwBSAnswers;
	delete bsc.nBSAnswers;
	delete bsc.nDBSAnswers;
	delete bsc.dWBSPopUp;
	delete bsc.dwBSPopUp;
	for(int x = 0; x < 2; x++)
	{
		delete machine.dVtxMachine[x];
		delete machine.nFaceMachine[x];
		delete machine.dUVTMachine[x];
		delete machine.nTMachine[x];
	}
	for(int x = 0; x < 10; x++)
	{
		delete machine.dVtxTap[x];
		delete machine.nFaceTap[x];
		delete machine.dUVTTap[x];
		delete machine.nTTap[x];
	}
	delete trac.nArg;
	delete crt.nArrows1;
	delete crt.nArrows2;
	delete crt.dWWArrows1;
	delete crt.dWWArrows2;
	delete crt.dVtxLFoot;
	delete crt.dVtxRFoot;
	delete crt.nFaceLFoot;
	delete crt.nFaceRFoot;
	delete crt.dVtxDLLeg;
	delete crt.nFaceDLLeg;
	delete crt.dUVTDLLeg;
	delete crt.nTDLLeg;
	delete crt.dVtxDRLeg;
	delete crt.nFaceDRLeg;
	delete crt.dUVTDRLeg;
	delete crt.nTDRLeg;
	delete crt.dVtxULLeg;
	delete crt.nFaceULLeg;
	delete crt.dUVTULLeg;
	delete crt.nTULLeg;
	delete crt.dVtxURLeg;
	delete crt.nFaceURLeg;
	delete crt.dUVTURLeg;
	delete crt.nTURLeg;
	delete crt.dVtxHip;
	delete crt.nFaceHip;
	delete crt.dUVTHip;
	delete crt.nTHip;
	delete crt.dVtxWaist;
	delete crt.nFaceWaist;
	delete crt.dUVTWaist;
	delete crt.nTWaist;
	delete crt.dVtxChest;
	delete crt.nFaceChest;
	delete crt.dUVTChest;
	delete crt.nTChest;
	delete crt.dVtxHead;
	delete crt.nFaceHead;
	delete crt.dUVTHead;
	delete crt.nTHead;
	delete crt.dVtxULArm;
	delete crt.nFaceULArm;
	delete crt.dUVTULArm;
	delete crt.nTULArm;
	delete crt.dVtxDLArm;
	delete crt.nFaceDLArm;
	delete crt.dUVTDLArm;
	delete crt.nTDLArm;
	delete crt.dVtxLHand;
	delete crt.nFaceLHand;
	delete crt.dUVTLHand;
	delete crt.nTLHand;
	delete crt.dVtxURArm;
	delete crt.nFaceURArm;
	delete crt.dUVTURArm;
	delete crt.nTURArm;
	delete crt.dVtxDRArm;
	delete crt.nFaceDRArm;
	delete crt.dUVTDRArm;
	delete crt.nTDRArm;
	delete crt.dVtxRHand;
	delete crt.nFaceRHand;
	delete crt.dUVTRHand;
	delete crt.nTRHand;
	delete crt.dwArrowsAP;
	delete crt.dWWArrowsAP;
	delete crt.dUVTLFoot;
	delete crt.dUVTRFoot;
	delete crt.nTLFoot;
	delete crt.nTRFoot;
}

int CSKSFView::StepCompile()
{
	// ** Asignar algunos modificadores provenientes del documento
	for(int x = 0; x < 3; x++)
	{
		m_bIsArrowsMisible[x] = GetDocument()->IsArrowsMisible(x);
		m_bIsArrowsCountable[x] = GetDocument()->IsArrowsCountable(x);
	}
	m_dClipMin1 = GetDocument()->song.hStep.dClipMin1;
	m_dClipMax1 = GetDocument()->song.hStep.dClipMax1;
	m_dClipMin2 = GetDocument()->song.hStep.dClipMin2;
	m_dClipMax2 = GetDocument()->song.hStep.dClipMax2;
	m_dClipMin3 = GetDocument()->song.hStep.dClipMin3;
	m_dClipMax3 = GetDocument()->song.hStep.dClipMax3;
	m_nOffset = GetDocument()->song.hStep.nOffset;
	m_bGetArrowInStop = GetDocument()->song.hStep.bGetArrowInStop;
	m_nLevelSong = GetDocument()->song.hStep.nDificulty;

	PUNT_EVA1 = 1500+(500*(m_nLevelSong/8));
	PUNT_EVA2 = 1000+(500*(m_nLevelSong/8));
	PUNT_EVA3 = 500+(500*(m_nLevelSong/8));
	PUNT_EVA4 = 300*(m_nLevelSong/16);
	PUNT_EVA5 = m_nLevelSong<16?0:(-500*((m_nLevelSong-12)/4));
	///////////////////////////////////////////////////////////////
	srand( (unsigned)time( NULL ) );

	if(!CompileSong()) return 0;
	if(!CompileEffects()) return 0;
	if(m_bTrainingMode) if(!CompileTraining()) return 0;
	m_nNext = m_nOffset+piuconfig.nMachineOffset;

	m_nHlpAuto = 0;
	if(!LoadTextures()) return 0;
	if(!CompileBS()) return 0;
	AllocStatus();
	if(m_nCharacter > 0)
	{
		m_bLoadMachine = LoadMachine(_T("characters/Machine/Tapete.ase"));
		if(!m_bTrainingMode) if(!CompileTraining()) return 0;
		m_bCharacter = CompileCharacter();
	}
	m_nAuxBeginCharacterStepL = 0;
	m_nAuxBeginCharacterStepR = 0;
	if(m_nVanish > -1)
	{
		/*m_dClipMax1 = (-9+(double)m_nVanish)/10;
		m_dClipMax2 = (-9+(double)m_nVanish)/10;
		m_dClipMax3 = (-9+(double)m_nVanish)/10;*/
		m_dClipMin1 = -double(m_nVanish)/10;
		m_dClipMin2 = -double(m_nVanish)/10;
		m_dClipMin3 = -double(m_nVanish)/10;
	}
	if(m_bDrop)
	{
		m_dClipMin1 = -m_dClipMin1;
		m_dClipMax1 = -m_dClipMax1;
		m_dClipMin2 = -m_dClipMin2;
		m_dClipMax2 = -m_dClipMax2;
		m_dClipMin3 = -m_dClipMin3;
		m_dClipMax3 = -m_dClipMax3;
		double dTemp;
		dTemp = m_dClipMin1;
		m_dClipMin1 = m_dClipMax1;
		m_dClipMax1 = dTemp;
		dTemp = m_dClipMin2;
		m_dClipMin2 = m_dClipMax2;
		m_dClipMax2 = dTemp;
		dTemp = m_dClipMin3;
		m_dClipMin3 = m_dClipMax3;
		m_dClipMax3 = dTemp;
	}
	////////////////////////////
	m_dMachVel = songc.dChangeVel[0];
	m_bIsCharged = true;
	return 1;
}

void CSKSFView::StepDestroy()
{
	if(m_bIsCharged)
	{
		/*if(songc.nSizeArrows1)*/ skin[0]->Delete();
		/*if(songc.nSizeArrows2)*/ skin[1]->Delete();
		/*if(songc.nSizeArrows3)*/ skin[2]->Delete();
		common->Delete();
		bs->Delete();
		bsf->Delete();
		if(m_nCharacter > 0)
		{
			DeleteMachine();
			DeleteCharacter();
		}
		if(m_bTrainingMode) DeleteTrainingCompiler();
		DeleteBS();
		DeleteEffects();
		DeleteSong();
		DeleteStatus();
		m_bIsCharged = false;
	}
}

void CSKSFView::Event_View(int nEvent, int nKey)
{
	if ( nEvent == EV_KEYDOWN )
    {
		if(m_bIsSingle && m_nPlayer == 1)
		{
			if(nKey == EV_P1_ARROW1)
			{
				evarrows.nArrows[0] = m_nNext;
				evarrows.bArrows[0] = true;
			}
			if(nKey == EV_P1_ARROW7)
			{
				evarrows.nArrows[1] = m_nNext;
				evarrows.bArrows[1] = true;
			}
			if(nKey == EV_P1_CENTER)
			{
				evarrows.nArrows[2] = m_nNext;
				evarrows.bArrows[2] = true;
			}
			if(nKey == EV_P1_ARROW9)
			{
				evarrows.nArrows[3] = m_nNext;
				evarrows.bArrows[3] = true;
			}
			if(nKey == EV_P1_ARROW3)
			{
				evarrows.nArrows[4] = m_nNext;
				evarrows.bArrows[4] = true;
			}
		}
		if(m_bIsSingle && m_nPlayer == 2)
		{
			if(nKey == EV_P2_ARROW1)
			{
				evarrows.nArrows[0] = m_nNext;
				evarrows.bArrows[0] = true;
			}
			if(nKey == EV_P2_ARROW7)
			{
				evarrows.nArrows[1] = m_nNext;
				evarrows.bArrows[1] = true;
			}
			if(nKey == EV_P2_CENTER)
			{
				evarrows.nArrows[2] = m_nNext;
				evarrows.bArrows[2] = true;
			}
			if(nKey == EV_P2_ARROW9)
			{
				evarrows.nArrows[3] = m_nNext;
				evarrows.bArrows[3] = true;
			}
			if(nKey == EV_P2_ARROW3)
			{
				evarrows.nArrows[4] = m_nNext;
				evarrows.bArrows[4] = true;
			}
		}
		if(!m_bIsSingle)
		{
			if(nKey == EV_P1_ARROW1)
			{
				evarrows.nArrows[0] = m_nNext;
				evarrows.bArrows[0] = true;
			}
			if(nKey == EV_P1_ARROW7)
			{
				evarrows.nArrows[1] = m_nNext;
				evarrows.bArrows[1] = true;
			}
			if(nKey == EV_P1_CENTER)
			{
				evarrows.nArrows[2] = m_nNext;
				evarrows.bArrows[2] = true;
			}
			if(nKey == EV_P1_ARROW9)
			{
				evarrows.nArrows[3] = m_nNext;
				evarrows.bArrows[3] = true;
			}
			if(nKey == EV_P1_ARROW3)
			{
				evarrows.nArrows[4] = m_nNext;
				evarrows.bArrows[4] = true;
			}
			if(nKey == EV_P2_ARROW1)
			{
				evarrows.nArrows[5] = m_nNext;
				evarrows.bArrows[5] = true;
			}
			if(nKey == EV_P2_ARROW7)
			{
				evarrows.nArrows[6] = m_nNext;
				evarrows.bArrows[6] = true;
			}
			if(nKey == EV_P2_CENTER)
			{
				evarrows.nArrows[7] = m_nNext;
				evarrows.bArrows[7] = true;
			}
			if(nKey == EV_P2_ARROW9)
			{
				evarrows.nArrows[8] = m_nNext;
				evarrows.bArrows[8] = true;
			}
			if(nKey == EV_P2_ARROW3)
			{
				evarrows.nArrows[9] = m_nNext;
				evarrows.bArrows[9] = true;
			}
		}
    }
	if(nEvent == EV_KEYUP)
	{
		if(m_bIsSingle && m_nPlayer == 1)
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
		if(m_bIsSingle && m_nPlayer == 2)
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
		if(!m_bIsSingle)
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

void CSKSFView::Draw()
{
	double t, b, l, r, o, ob;
	skin[0]->Update(dwTick - dwLastTick);
	skin[1]->Update(dwTick - dwLastTick);
	skin[2]->Update(dwTick - dwLastTick);
	common->Update(dwTick - dwLastTick);
	bs->Update(dwTick - dwLastTick);
	// ** Timing
	static float sub = 0.0f;
	float fAdd = (float(dwTick - dwLastTick))+sub;
	sub = fAdd - (float)((__int64)fAdd);
	if(g_bIsMusic)
	{
        m_nNext += (__int64)fAdd;
        if(_abs64(m_nNext-((__int64)(nPos)+(__int64)m_nOffset+(__int64)piuconfig.nMachineOffset)) > 100)
            m_nNext = (__int64)(nPos)+(__int64)m_nOffset+(__int64)piuconfig.nMachineOffset;
	}
	else m_nNext += (__int64)fAdd;
	int nStop; double dActualBPM; double dst;
	dBeat = TimeToBeat(m_nNext, &nStop, &dst, &dActualBPM);
	dBeatnStop = dBeat+dst;
	for(int g = 0; g < songc.nSizeStops; g++)
	{
		if(dBeat > songc.dWStops[g])
		{
			dBeatnStop += songc.dStops[g];
		}
		else break;
	}
	for(int g = 0; g < songc.nSizeJumps; g++)
	{
		if(dBeat > songc.dIJumps[g])
		{
			dBeatnStop -= songc.dEJumps[g]-songc.dIJumps[g];
		}
		else break;
	}
	m_dLevelMach = (double)m_nLevelMach*dActualBPM/60000;
	ActualizateVel(dBeat);
	//////////////////////////////
	// ** Geometria Máquina
	if(m_bLoadMachine && m_bCharacter && m_nCharacter > 0)
	{
		double dOffsetMove[]={-35.0, -40.0, -55.0};
		for(int w = 0; w < (m_bIsSingle?1:2); w++)
		{
			glBindTexture(GL_TEXTURE_2D, m_uNullTex);
			glPushMatrix();
			glLoadIdentity();
			if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
			else
			{
				if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0);
				if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);
			}
			glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
			glColor4d(0.5, 0.5, 0.5, 1.0);
			for(int i = 0; i < machine.hMachine.nSizeFaceMachine[w]; i++)
			{
				glBegin(GL_POLYGON);
				for(int j = 0; j < 3; j++)
				{
					int nIndex = machine.nFaceMachine[w][i*3+j];
					glVertex3d(machine.dVtxMachine[w][nIndex*3], machine.dVtxMachine[w][nIndex*3+2], -machine.dVtxMachine[w][nIndex*3+1]);
				}
				glEnd();
			}
			glPopMatrix();
		}
		for(int w = 0; w < (m_bIsSingle?5:10); w++)
		{
			glBindTexture(GL_TEXTURE_2D, machine.nTexTap[w]);
			glPushMatrix();
			glLoadIdentity();
			if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
			else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
			glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			for(int i = 0; i < machine.hMachine.nSizeFaceTap[w]; i++)
			{
				glBegin(GL_POLYGON);
				for(int j = 0; j < 3; j++)
				{
					int nIndex = machine.nFaceTap[w][i*3+j];
					int nIndexT = machine.nTTap[w][i*3+j];
					glTexCoord3d(machine.dUVTTap[w][nIndexT*3], machine.dUVTTap[w][nIndexT*3+1], machine.dUVTTap[w][nIndexT*3+2]);
					glVertex3d(machine.dVtxTap[w][nIndex*3], machine.dVtxTap[w][nIndex*3+2], -machine.dVtxTap[w][nIndex*3+1]);
				}
				glEnd();
			}
			glPopMatrix();
		}
		// ** Dibujo Personaje
		glColor4d(1.0, 1.0, 1.0, 1.0);
		// * Pies
		int lf = 0;
		for(lf = m_nAuxBeginCharacterStepL; lf < crt.hCtr.nSizeArrows1; lf++)	// Encontrar el paso actual
		{
			m_nAuxBeginCharacterStepL = lf;
			if(crt.dWWArrows1[lf] >= dBeatnStop) break;
			else if(crt.hCtr.nSizeArrows1 == (lf+1)) break;
			//if((crt.hCtr.nSizeArrows1-1) == lf) lf++;
		}
		int rt = 0;
		for(rt = m_nAuxBeginCharacterStepR; rt < crt.hCtr.nSizeArrows2; rt++)
		{
			m_nAuxBeginCharacterStepR = rt;
			if(crt.dWWArrows2[rt] >= dBeatnStop) break;
			else if(crt.hCtr.nSizeArrows2 == (rt+1)) break;
			//if((crt.hCtr.nSizeArrows2-1) == rt) rt++;
		}
		// Derivar la posición donde se debe pisar
		double dIPosL[] = {0.0,0.0,0.0};
		double dEPosL[] = {0.0,0.0,0.0};
		double dPosL[] = {0.0,0.0,0.0};
		bool bAssigned = false;
		for(int y = 0; y < (m_bIsSingle?5:10); y++)
		{
			if(crt.nArrows1[lf*10+y] == 1 || crt.nArrows1[lf*10+y] == 3/* || crt.nArrows1[lf*10+y] == 4*/)
			{
				if(!bAssigned)
				{
					dEPosL[0] = machine.hMachine.dPosTap[y*3];
					dEPosL[1] = machine.hMachine.dPosTap[y*3+1];
					dEPosL[2] = machine.hMachine.dPosTap[y*3+2];
					bAssigned = true;
				}
				else
				{
					dEPosL[0] = (dEPosL[0]+machine.hMachine.dPosTap[y*3])/2;
					dEPosL[1] = (dEPosL[1]+machine.hMachine.dPosTap[y*3+1])/2;
					dEPosL[2] = (dEPosL[2]+machine.hMachine.dPosTap[y*3+2])/2;
				}
			}
		}
		bAssigned = false;
		if(lf == 0)
		{
			dIPosL[0] = machine.hMachine.dPosNoTapL[0];
			dIPosL[1] = machine.hMachine.dPosNoTapL[1];
			dIPosL[2] = machine.hMachine.dPosNoTapL[2];
		}
		else
		{
			int i = lf;
			if(crt.dWWArrows1[lf] >= dBeatnStop) i--;
			for(int y = 0; y < (m_bIsSingle?5:10); y++)
			{
				if(crt.nArrows1[i*10+y] == 1 || crt.nArrows1[i*10+y] == 3/* || crt.nArrows1[i*10+y] == 4*/)
				{
					if(!bAssigned)
					{
						dIPosL[0] = machine.hMachine.dPosTap[y*3];
						dIPosL[1] = machine.hMachine.dPosTap[y*3+1];
						dIPosL[2] = machine.hMachine.dPosTap[y*3+2];
						bAssigned = true;
					}
					else
					{
						dIPosL[0] = (dIPosL[0]+machine.hMachine.dPosTap[y*3])/2;
						dIPosL[1] = (dIPosL[1]+machine.hMachine.dPosTap[y*3+1])/2;
						dIPosL[2] = (dIPosL[2]+machine.hMachine.dPosTap[y*3+2])/2;
					}
				}
			}
		}
		double dInterv1 = lf==0?crt.dWWArrows1[lf]:crt.dWWArrows1[lf]-crt.dWWArrows1[lf-1];
		//double dInterv2 = crt.dWWArrows1[lf]-dBeatnStop;
		double dInterv = m_dLevelMach<dInterv1?m_dLevelMach:dInterv1;
		double dOffs = crt.dWWArrows1[lf]-dBeatnStop;
		if(dInterv > dOffs && dOffs >= 0)
		{
			double dConst = (dInterv-dOffs)/dInterv;
			dPosL[0] = (dEPosL[0]-dIPosL[0])*dConst+dIPosL[0];
			dPosL[1] = (dEPosL[1]-dIPosL[1])*dConst+dIPosL[1];
			dPosL[2] = (4.0/*h*/-4.0*4.0/*h*/*pow(dConst-0.5, 2))+dIPosL[2];
		}
		else
		{
			dPosL[0] = dIPosL[0];
			dPosL[1] = dIPosL[1];
			dPosL[2] = dIPosL[2];
		}

		double dIPosR[] = {0.0,0.0,0.0};
		double dEPosR[] = {0.0,0.0,0.0};
		double dPosR[] = {0.0,0.0,0.0};
		bAssigned = false;
		for(int y = 0; y < (m_bIsSingle?5:10); y++)
		{
			if(crt.nArrows2[rt*10+y] == 1 || crt.nArrows2[rt*10+y] == 3/* || crt.nArrows2[rt*10+y] == 4*/)
			{
				if(!bAssigned)
				{
					dEPosR[0] = machine.hMachine.dPosTap[y*3];
					dEPosR[1] = machine.hMachine.dPosTap[y*3+1];
					dEPosR[2] = machine.hMachine.dPosTap[y*3+2];
					bAssigned = true;
				}
				else
				{
					dEPosR[0] = (dEPosR[0]+machine.hMachine.dPosTap[y*3])/2;
					dEPosR[1] = (dEPosR[1]+machine.hMachine.dPosTap[y*3+1])/2;
					dEPosR[2] = (dEPosR[2]+machine.hMachine.dPosTap[y*3+2])/2;
				}
			}
		}
		bAssigned = false;
		if(rt == 0)
		{
			dIPosR[0] = machine.hMachine.dPosNoTapR[0];
			dIPosR[1] = machine.hMachine.dPosNoTapR[1];
			dIPosR[2] = machine.hMachine.dPosNoTapR[2];
		}
		else
		{
			int i = rt;
			if(crt.dWWArrows2[rt] >= dBeatnStop) i--;
			for(int y = 0; y < (m_bIsSingle?5:10); y++)
			{
				if(crt.nArrows2[i*10+y] == 1 || crt.nArrows2[i*10+y] == 3/* || crt.nArrows2[i*10+y] == 4*/)
				{
					if(!bAssigned)
					{
						dIPosR[0] = machine.hMachine.dPosTap[y*3];
						dIPosR[1] = machine.hMachine.dPosTap[y*3+1];
						dIPosR[2] = machine.hMachine.dPosTap[y*3+2];
						bAssigned = true;
					}
					else
					{
						dIPosR[0] = (dIPosR[0]+machine.hMachine.dPosTap[y*3])/2;
						dIPosR[1] = (dIPosR[1]+machine.hMachine.dPosTap[y*3+1])/2;
						dIPosR[2] = (dIPosR[2]+machine.hMachine.dPosTap[y*3+2])/2;
					}
				}
			}
		}
		dInterv1 = rt==0?crt.dWWArrows2[rt]:crt.dWWArrows2[rt]-crt.dWWArrows2[rt-1];
		//double dInterv2 = crt.dWWArrows2[rt]-dBeatnStop;
		dInterv = m_dLevelMach>dInterv1?dInterv1:m_dLevelMach;
		dOffs = crt.dWWArrows2[rt]-dBeatnStop;
		if(dInterv >= dOffs && dOffs >= 0)
		{
			double dConst = (dInterv-dOffs)/dInterv;
			dPosR[0] = (dEPosR[0]-dIPosR[0])*dConst+dIPosR[0];
			dPosR[1] = (dEPosR[1]-dIPosR[1])*dConst+dIPosR[1];
			dPosR[2] = (4.0/*h*/-4.0*4.0/*h*/*pow(dConst-0.5, 2))+dIPosR[2];
		}
		else
		{
			dPosR[0] = dIPosR[0];
			dPosR[1] = dIPosR[1];
			dPosR[2] = dIPosR[2];
		}

		// Derivar el angulo phi de rotación de acuerdo a la posicion de los pies.
		double dPhi = 0.0;
		if(dPosR[0]==dPosL[0])
		{
			if(dPosR[1]==dPosL[1]) dPhi=0.0;
			if(dPosR[1]>dPosL[1]) dPhi=M_PI/2;
			if(dPosR[1]<dPosL[1]) dPhi=-M_PI/2;

		}
		else
		{
			dPhi=atan((dPosR[1]-dPosL[1])/(dPosR[0]-dPosL[0]));
			if(dPosR[0]<dPosL[0]) dPhi+=M_PI;
		}
		double dCosPhi = cos(dPhi);
		double dSinPhi = sin(dPhi);
		double dPhiD = dPhi*180/M_PI;

		// Posicion del centro de la cadera
		double dMinS = machine.hMachine.dPosNoTapL[2];
		double dMaxS = dPosR[2]>dPosL[2]?dPosL[2]:dPosR[2];
		double d1=(crt.hCtr.dNormalHip[3]-crt.hCtr.dNormalHip[0]);
		double d2=sqrt(pow(dPosR[0]-dPosL[0],2)+pow(dPosR[1]-dPosL[1],2));
		double h1=crt.hCtr.dNormalHip[2]-crt.hCtr.dNormalDLLeg[2];
		double dPosHip[]={(dPosR[0]+dPosL[0])/2,(dPosR[1]+dPosL[1])/2,sqrt(pow(h1,2)-pow((d2-d1)/2,2))+(dMaxS-dMinS)/2+dMinS};

		// Calcular el jodido angulo de inclinación de la pierna
		double dPosHipL[]={dPosHip[0]-d1/2*dCosPhi, dPosHip[1]-d1/2*dSinPhi, dPosHip[2]};
		double dPosHipR[]={dPosHip[0]+d1/2*dCosPhi, dPosHip[1]+d1/2*dSinPhi, dPosHip[2]};
		d1 = crt.hCtr.dNormalULLeg[5]-crt.hCtr.dNormalULLeg[2];
		d2 = crt.hCtr.dNormalDLLeg[5]-crt.hCtr.dNormalDLLeg[2];
		double d3 = sqrt(pow(dPosHipL[0]-dPosL[0],2)+pow(dPosHipL[1]-dPosL[1],2)+pow(dPosHipL[2]-dPosL[2],2));
		double d4 = (-pow(d2, 2)+pow(d1, 2)+pow(d3, 2))/(2*d1*d3);
		double dThetaUL;
		if(d4 < -1.0 || d4 > 1.0) dThetaUL = 0.0;
		else dThetaUL = acos(d4);
		double dThetaDL = asin(d1*sin(dThetaUL)/d2);
		double dThetaULD = dThetaUL*180/M_PI;
		double dThetaDLD = dThetaDL*180/M_PI;

		d1 = crt.hCtr.dNormalHip[3]-crt.hCtr.dNormalHip[0];
		d2 = sqrt(pow(dPosHipR[0]-dPosL[0],2)+pow(dPosHipR[1]-dPosL[1],2)+pow(dPosHipR[2]-dPosL[2],2));
		d3 = sqrt(pow(dPosHipL[0]-dPosL[0],2)+pow(dPosHipL[1]-dPosL[1],2)+pow(dPosHipL[2]-dPosL[2],2));
		d4 = (-pow(d2, 2)+pow(d1, 2)+pow(d3, 2))/(2*d1*d3);
		double dPhiUL;
		if(d4 < -1.0 || d4 > 1.0) dPhiUL = 0.0;
		else dPhiUL = acos(d4);
		double dPhiULD = dPhiUL*180/M_PI;

		d1 = crt.hCtr.dNormalURLeg[5]-crt.hCtr.dNormalURLeg[2];
		d2 = crt.hCtr.dNormalDRLeg[5]-crt.hCtr.dNormalDRLeg[2];
		d3 = sqrt(pow(dPosHipR[0]-dPosR[0],2)+pow(dPosHipR[1]-dPosR[1],2)+pow(dPosHipR[2]-dPosR[2],2));
		d4 = (-pow(d2, 2)+pow(d1, 2)+pow(d3, 2))/(2*d1*d3);
		double dThetaUR;
		if(d4 < -1.0 || d4 > 1.0) dThetaUR = 0.0;
		else dThetaUR = acos(d4);
		double dThetaDR = asin(d1*sin(dThetaUR)/d2);
		double dThetaURD = dThetaUR*180/M_PI;
		double dThetaDRD = dThetaDR*180/M_PI;

		d1 = crt.hCtr.dNormalHip[3]-crt.hCtr.dNormalHip[0];
		d2 = sqrt(pow(dPosHipL[0]-dPosR[0],2)+pow(dPosHipL[1]-dPosR[1],2)+pow(dPosHipL[2]-dPosR[2],2));
		d3 = sqrt(pow(dPosHipR[0]-dPosR[0],2)+pow(dPosHipR[1]-dPosR[1],2)+pow(dPosHipR[2]-dPosR[2],2));
		d4 = (-pow(d2, 2)+pow(d1, 2)+pow(d3, 2))/(2*d1*d3);
		double dPhiUR;
		if(d4 < -1.0 || d4 > 1.0) dPhiUR = 0.0;
		else dPhiUR = acos(d4);
		double dPhiURD = dPhiUR*180/M_PI;

		// Encontrar la posicion de los hombros
		d1 = crt.hCtr.dNormalChest[3]-crt.hCtr.dNormalChest[0];
		double dPosChest[] = {dPosHip[0], dPosHip[1], dPosHip[2]+(crt.hCtr.dNormalWaist[5]-crt.hCtr.dNormalWaist[2])};
		double dPosChestL[]={dPosChest[0]-d1/2*dCosPhi, dPosChest[1]-d1/2*dSinPhi, dPosChest[2]};
		double dPosChestR[]={dPosChest[0]+d1/2*dCosPhi, dPosChest[1]+d1/2*dSinPhi, dPosChest[2]};

		// Encontrar la posicion de las manos
		d2 = crt.hCtr.dNormalULArm[5]-crt.hCtr.dNormalDLArm[2];
		d3 = d2*sin(dPhiUL-M_PI/2);
		d4 = d2*cos(dPhiUL-M_PI/2);
		double dPosHandL[]={dPosChest[0]-(d1/2+d3)*dCosPhi, dPosChest[1]-(d1/2+d3)*dSinPhi, dPosChest[2]-d4+(dPosR[2]-dMinS)/2};
		d2 = crt.hCtr.dNormalURArm[5]-crt.hCtr.dNormalDRArm[2];
		d3 = d2*sin(dPhiUR-M_PI/2);
		d4 = d2*cos(dPhiUR-M_PI/2);
		double dPosHandR[]={dPosChest[0]+(d1/2+d3)*dCosPhi, dPosChest[1]+(d1/2+d3)*dSinPhi, dPosChest[2]-d4+(dPosL[2]-dMinS)/2};

		// Encontrar los jodidos Ángulos de rotacion de los hombros y los codos
		// PD Los de los hombros ya los tenemos
		d1 = crt.hCtr.dNormalULArm[5]-crt.hCtr.dNormalULArm[2];
		d2 = crt.hCtr.dNormalDLArm[5]-crt.hCtr.dNormalDLArm[2];
		d3 = sqrt(pow(dPosChestL[0]-dPosHandL[0],2)+pow(dPosChestL[1]-dPosHandL[1],2)+pow(dPosChestL[2]-dPosHandL[2],2));
		d4 = (-pow(d2, 2)+pow(d1, 2)+pow(d3, 2))/(2*d1*d3);
		double dThetaULHand;
		if(d4 < -1.0 || d4 > 1.0) dThetaULHand = 0.0;
		else dThetaULHand = acos(d4);
		double dThetaDLHand = asin(d1*sin(dThetaULHand)/d2);
		double dThetaULHandD = dThetaULHand*180/M_PI;
		double dThetaDLHandD = dThetaDLHand*180/M_PI;

		d1 = crt.hCtr.dNormalURArm[5]-crt.hCtr.dNormalURArm[2];
		d2 = crt.hCtr.dNormalDRArm[5]-crt.hCtr.dNormalDRArm[2];
		d3 = sqrt(pow(dPosChestR[0]-dPosHandR[0],2)+pow(dPosChestR[1]-dPosHandR[1],2)+pow(dPosChestR[2]-dPosHandR[2],2));
		d4 = (-pow(d2, 2)+pow(d1, 2)+pow(d3, 2))/(2*d1*d3);
		double dThetaURHand;
		if(d4 < -1.0 || d4 > 1.0) dThetaURHand = 0.0;
		else dThetaURHand = acos(d4);
		double dThetaDRHand = asin(d1*sin(dThetaURHand)/d2);
		double dThetaURHandD = dThetaURHand*180/M_PI;
		double dThetaDRHandD = dThetaDRHand*180/M_PI;

		// Dibujo de los pies
		glBindTexture(GL_TEXTURE_2D, crt.nTextures[0]);
		glPushMatrix();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosL[0], dPosL[2], -dPosL[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(-dThetaDLD, 1.0, 0.0, 0.0);
		glRotated(90-dPhiULD, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceLFoot; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceLFoot[i*3+j];
				int nIndexT = crt.nTLFoot[i*3+j];
				glTexCoord3d(crt.dUVTLFoot[nIndexT*3], crt.dUVTLFoot[nIndexT*3+1], crt.dUVTLFoot[nIndexT*3+2]);
				double dX=(crt.dVtxLFoot[nIndex*3]-crt.hCtr.dNormalDLLeg[0]);//*dCosPhi-(crt.dVtxLFoot[nIndex*3+1]-crt.hCtr.dNormalDLLeg[1])*dSinPhi;
				double dY=(crt.dVtxLFoot[nIndex*3+2]-crt.hCtr.dNormalDLLeg[2]);
				double dZ=(crt.dVtxLFoot[nIndex*3+1]-crt.hCtr.dNormalDLLeg[1]);//*dCosPhi+(crt.dVtxLFoot[nIndex*3]-crt.hCtr.dNormalDLLeg[0])*dSinPhi;
				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[1]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosR[0], dPosR[2], -dPosR[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(-dThetaDRD, 1.0, 0.0, 0.0);
		glRotated(dPhiURD-90, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceRFoot; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceRFoot[i*3+j];
				int nIndexT = crt.nTRFoot[i*3+j];
				glTexCoord3d(crt.dUVTRFoot[nIndexT*3], crt.dUVTRFoot[nIndexT*3+1], crt.dUVTRFoot[nIndexT*3+2]);
				double dX=(crt.dVtxRFoot[nIndex*3]-crt.hCtr.dNormalDRLeg[0]);//*dCosPhi-(crt.dVtxRFoot[nIndex*3+1]-crt.hCtr.dNormalDRLeg[1])*dSinPhi;
				double dY=(crt.dVtxRFoot[nIndex*3+2]-crt.hCtr.dNormalDRLeg[2]);
				double dZ=(crt.dVtxRFoot[nIndex*3+1]-crt.hCtr.dNormalDRLeg[1]);//*dCosPhi+(crt.dVtxRFoot[nIndex*3]-crt.hCtr.dNormalDRLeg[0])*dSinPhi;
				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		// Dibujo de las Piernas (que jodido fue!)
		glBindTexture(GL_TEXTURE_2D, crt.nTextures[2]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosL[0], dPosL[2], -dPosL[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(-dThetaDLD, 1.0, 0.0, 0.0);
		glRotated(90-dPhiULD, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceDLLeg; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceDLLeg[i*3+j];
				int nIndexT = crt.nTDLLeg[i*3+j];
				glTexCoord3d(crt.dUVTDLLeg[nIndexT*3], crt.dUVTDLLeg[nIndexT*3+1], crt.dUVTDLLeg[nIndexT*3+2]);
				double dX=(crt.dVtxDLLeg[nIndex*3]-crt.hCtr.dNormalDLLeg[0]);
				double dY=(crt.dVtxDLLeg[nIndex*3+2]-crt.hCtr.dNormalDLLeg[2]);
				double dZ=(crt.dVtxDLLeg[nIndex*3+1]-crt.hCtr.dNormalDLLeg[1]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[4]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosHipL[0], dPosHipL[2], -dPosHipL[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(dThetaULD, 1.0, 0.0, 0.0);
		glRotated(90-dPhiULD, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceULLeg; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceULLeg[i*3+j];
				int nIndexT = crt.nTULLeg[i*3+j];
				glTexCoord3d(crt.dUVTULLeg[nIndexT*3], crt.dUVTULLeg[nIndexT*3+1], crt.dUVTULLeg[nIndexT*3+2]);
				double dX=(crt.dVtxULLeg[nIndex*3]-crt.hCtr.dNormalULLeg[3]);
				double dY=(crt.dVtxULLeg[nIndex*3+2]-crt.hCtr.dNormalULLeg[5]);
				double dZ=(crt.dVtxULLeg[nIndex*3+1]-crt.hCtr.dNormalULLeg[4]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[3]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosR[0], dPosR[2], -dPosR[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(-dThetaDRD, 1.0, 0.0, 0.0);
		glRotated(dPhiURD-90, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceDRLeg; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceDRLeg[i*3+j];
				int nIndexT = crt.nTDRLeg[i*3+j];
				glTexCoord3d(crt.dUVTDRLeg[nIndexT*3], crt.dUVTDRLeg[nIndexT*3+1], crt.dUVTDRLeg[nIndexT*3+2]);
				double dX=(crt.dVtxDRLeg[nIndex*3]-crt.hCtr.dNormalDRLeg[0]);
				double dY=(crt.dVtxDRLeg[nIndex*3+2]-crt.hCtr.dNormalDRLeg[2]);
				double dZ=(crt.dVtxDRLeg[nIndex*3+1]-crt.hCtr.dNormalDRLeg[1]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[5]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosHipR[0], dPosHipR[2], -dPosHipR[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(dThetaURD, 1.0, 0.0, 0.0);
		glRotated(dPhiURD-90, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceURLeg; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceURLeg[i*3+j];
				int nIndexT = crt.nTURLeg[i*3+j];
				glTexCoord3d(crt.dUVTURLeg[nIndexT*3], crt.dUVTURLeg[nIndexT*3+1], crt.dUVTURLeg[nIndexT*3+2]);
				double dX=(crt.dVtxURLeg[nIndex*3]-crt.hCtr.dNormalURLeg[3]);
				double dY=(crt.dVtxURLeg[nIndex*3+2]-crt.hCtr.dNormalURLeg[5]);
				double dZ=(crt.dVtxURLeg[nIndex*3+1]-crt.hCtr.dNormalURLeg[4]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		// Dibujo del cuerpo
		glBindTexture(GL_TEXTURE_2D, crt.nTextures[6]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosHip[0], dPosHip[2], -dPosHip[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceHip; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceHip[i*3+j];
				int nIndexT = crt.nTHip[i*3+j];
				glTexCoord3d(crt.dUVTHip[nIndexT*3], crt.dUVTHip[nIndexT*3+1], crt.dUVTHip[nIndexT*3+2]);
				double dX=(crt.dVtxHip[nIndex*3]-crt.hCtr.dNormalWaist[0]);
				double dY=(crt.dVtxHip[nIndex*3+2]-crt.hCtr.dNormalWaist[2]);
				double dZ=(crt.dVtxHip[nIndex*3+1]-crt.hCtr.dNormalWaist[1]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[7]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosHip[0], dPosHip[2], -dPosHip[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceWaist; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceWaist[i*3+j];
				int nIndexT = crt.nTWaist[i*3+j];
				glTexCoord3d(crt.dUVTWaist[nIndexT*3], crt.dUVTWaist[nIndexT*3+1], crt.dUVTWaist[nIndexT*3+2]);
				double dX=(crt.dVtxWaist[nIndex*3]-crt.hCtr.dNormalWaist[0]);
				double dY=(crt.dVtxWaist[nIndex*3+2]-crt.hCtr.dNormalWaist[2]);
				double dZ=(crt.dVtxWaist[nIndex*3+1]-crt.hCtr.dNormalWaist[1]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[8]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosHip[0], dPosHip[2], -dPosHip[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceChest; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceChest[i*3+j];
				int nIndexT = crt.nTChest[i*3+j];
				glTexCoord3d(crt.dUVTChest[nIndexT*3], crt.dUVTChest[nIndexT*3+1], crt.dUVTChest[nIndexT*3+2]);
				double dX=(crt.dVtxChest[nIndex*3]-crt.hCtr.dNormalWaist[0]);
				double dY=(crt.dVtxChest[nIndex*3+2]-crt.hCtr.dNormalWaist[2]);
				double dZ=(crt.dVtxChest[nIndex*3+1]-crt.hCtr.dNormalWaist[1]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		double dAngleHead = 0.0;
		while(dPhiD < 0)
		{
			dPhiD += 360;
		}
		while (dPhiD > 360)
		{
			dPhiD -= 360;
		}
		if(dPhiD >= 315 || dPhiD <= 45) dAngleHead = 0.0;
		if(dPhiD >= 135 && dPhiD <= 225) dAngleHead = 180.0;
		if(dPhiD > 45 && dPhiD < 135) dAngleHead = (dPhiD-90)/45*90+90;
		if(dPhiD > 225 && dPhiD < 315) dAngleHead = (dPhiD-270)/45*90+270;

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[9]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosHip[0], dPosHip[2], -dPosHip[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dAngleHead, 0.0, 1.0, 0.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceHead; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceHead[i*3+j];
				int nIndexT = crt.nTHead[i*3+j];
				glTexCoord3d(crt.dUVTHead[nIndexT*3], crt.dUVTHead[nIndexT*3+1], crt.dUVTHead[nIndexT*3+2]);
				double dX=(crt.dVtxHead[nIndex*3]-crt.hCtr.dNormalWaist[0]);
				double dY=(crt.dVtxHead[nIndex*3+2]-crt.hCtr.dNormalWaist[2]);
				double dZ=(crt.dVtxHead[nIndex*3+1]-crt.hCtr.dNormalWaist[1]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		// Dibujar las manos
		glBindTexture(GL_TEXTURE_2D, crt.nTextures[12]);
		glPushMatrix();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosHandL[0], dPosHandL[2], -dPosHandL[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(90-dPhiULD, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceLHand; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceLHand[i*3+j];
				int nIndexT = crt.nTLHand[i*3+j];
				glTexCoord3d(crt.dUVTLHand[nIndexT*3], crt.dUVTLHand[nIndexT*3+1], crt.dUVTLHand[nIndexT*3+2]);
				double dX=(crt.dVtxLHand[nIndex*3]-crt.hCtr.dNormalDLArm[0]);
				double dY=(crt.dVtxLHand[nIndex*3+2]-crt.hCtr.dNormalDLArm[2]);
				double dZ=(crt.dVtxLHand[nIndex*3+1]-crt.hCtr.dNormalDLArm[1]);
				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[15]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosHandR[0], dPosHandR[2], -dPosHandR[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(dPhiURD-90, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceRHand; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceRHand[i*3+j];
				int nIndexT = crt.nTRHand[i*3+j];
				glTexCoord3d(crt.dUVTRHand[nIndexT*3], crt.dUVTRHand[nIndexT*3+1], crt.dUVTRHand[nIndexT*3+2]);
				double dX=(crt.dVtxRHand[nIndex*3]-crt.hCtr.dNormalDRArm[0]);
				double dY=(crt.dVtxRHand[nIndex*3+2]-crt.hCtr.dNormalDRArm[2]);
				double dZ=(crt.dVtxRHand[nIndex*3+1]-crt.hCtr.dNormalDRArm[1]);
				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		// Dibujar los brazos
		glBindTexture(GL_TEXTURE_2D, crt.nTextures[11]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosHandL[0], dPosHandL[2], -dPosHandL[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(dThetaDLHandD, 1.0, 0.0, 0.0);
		glRotated(90-dPhiULD, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceDLArm; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceDLArm[i*3+j];
				int nIndexT = crt.nTDLArm[i*3+j];
				glTexCoord3d(crt.dUVTDLArm[nIndexT*3], crt.dUVTDLArm[nIndexT*3+1], crt.dUVTDLArm[nIndexT*3+2]);
				double dX=(crt.dVtxDLArm[nIndex*3]-crt.hCtr.dNormalDLArm[0]);
				double dY=(crt.dVtxDLArm[nIndex*3+2]-crt.hCtr.dNormalDLArm[2]);
				double dZ=(crt.dVtxDLArm[nIndex*3+1]-crt.hCtr.dNormalDLArm[1]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[10]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosChestL[0], dPosChestL[2], -dPosChestL[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(-dThetaULHandD, 1.0, 0.0, 0.0);
		glRotated(90-dPhiULD, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceULArm; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceULArm[i*3+j];
				int nIndexT = crt.nTULArm[i*3+j];
				glTexCoord3d(crt.dUVTULArm[nIndexT*3], crt.dUVTULArm[nIndexT*3+1], crt.dUVTULArm[nIndexT*3+2]);
				double dX=(crt.dVtxULArm[nIndex*3]-crt.hCtr.dNormalULArm[3]);
				double dY=(crt.dVtxULArm[nIndex*3+2]-crt.hCtr.dNormalULArm[5]);
				double dZ=(crt.dVtxULArm[nIndex*3+1]-crt.hCtr.dNormalULArm[4]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[14]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosHandR[0], dPosHandR[2], -dPosHandR[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(dThetaDRHandD, 1.0, 0.0, 0.0);
		glRotated(dPhiURD-90, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceDRArm; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceDRArm[i*3+j];
				int nIndexT = crt.nTDRArm[i*3+j];
				glTexCoord3d(crt.dUVTDRArm[nIndexT*3], crt.dUVTDRArm[nIndexT*3+1], crt.dUVTDRArm[nIndexT*3+2]);
				double dX=(crt.dVtxDRArm[nIndex*3]-crt.hCtr.dNormalDRArm[0]);
				double dY=(crt.dVtxDRArm[nIndex*3+2]-crt.hCtr.dNormalDRArm[2]);
				double dZ=(crt.dVtxDRArm[nIndex*3+1]-crt.hCtr.dNormalDRArm[1]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, crt.nTextures[13]);
		glPushMatrix();
		glLoadIdentity();
		if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(17.51965, 0.0, 0.0);
		else {if(m_nPlayer == 1 && m_bIsSingle) glTranslated(-11.2205, 0.0, 0.0); if(m_nPlayer == 2 && m_bIsSingle) glTranslated(46.2598, 0.0, 0.0);}
		glTranslated(dPosChestR[0], dPosChestR[2], -dPosChestR[1]);
		glTranslated(dOffsetMove[0], dOffsetMove[1], dOffsetMove[2]);
		glRotated(dPhiD, 0.0, 1.0, 0.0);
		glRotated(-dThetaURHandD, 1.0, 0.0, 0.0);
		glRotated(dPhiURD-90, 0.0, 0.0, 1.0);
		for(int i = 0; i < crt.hCtr.nSizeFaceURArm; i++)
		{
			glBegin(GL_POLYGON);
			for(int j = 0; j < 3; j++)
			{
				int nIndex = crt.nFaceURArm[i*3+j];
				int nIndexT = crt.nTURArm[i*3+j];
				glTexCoord3d(crt.dUVTURArm[nIndexT*3], crt.dUVTURArm[nIndexT*3+1], crt.dUVTURArm[nIndexT*3+2]);
				double dX=(crt.dVtxURArm[nIndex*3]-crt.hCtr.dNormalURArm[3]);
				double dY=(crt.dVtxURArm[nIndex*3+2]-crt.hCtr.dNormalURArm[5]);
				double dZ=(crt.dVtxURArm[nIndex*3+1]-crt.hCtr.dNormalURArm[4]);

				glVertex3d(dX, dY, -dZ);
			}
			glEnd();
		}
		glPopMatrix();
		//////////////////////
	}
	///////////////////////////
	int nHlp = m_nHlpAuto;
	if(m_bAutoPlay)
	{
		if(m_bCharacter)
		{
			for(int s = nHlp; s < crt.hCtr.nSizeArrowsAP; s++)
			{
				if(crt.dWWArrowsAP[s] <= dBeatnStop)
				{
					int nArr = LOWORD(crt.dwArrowsAP[s]);
					int nType = HIWORD(crt.dwArrowsAP[s]);
					if(nType == 1)
					{
						evarrows.nArrows[nArr] = m_nNext;
					}
					if(nType == 3)
					{
						evarrows.nArrows[nArr] = m_nNext;
						evarrows.bArrows[nArr] = true;
					}
					m_nHlpAuto = s+1;
				}
				else break;
			}
		}
		else
		{
			for(int s = nHlp; s < songc.nSizeArrows1; s++)
			{
				if(songc.dWWArrows1[s] <= dBeatnStop)
				{
					for(int t = 0; t < 10; t++)
					{
						if(LOWORD(songc.dwArrows1[s*10+t]) == 1)
						{
							evarrows.nArrows[t] = m_nNext;
						}
						if(LOWORD(songc.dwArrows1[s*10+t]) == 3)
						{
							evarrows.nArrows[t] = m_nNext;
							evarrows.bArrows[t] = true;
						}
					}
					m_nHlpAuto = s+1;
				}
				else break;
			}
		}
	}
	int nMin[3], nMax[3];
	for(int m = 0; m < 3; m++)
	{
		GetLimitsOfDrawing(dBeat, m, &nMin[m], &nMax[m]);
		nMin[m] = max(nMin[m], 0);
		nMax[m] = max(nMax[m], 0);
	}

	m_nSizeArrows = 51.0/8.0;
	double m_nDistArrows = 60.0/8.0;
	int nOrder[] = {0,4,1,3,2,5,9,6,8,7};
	int nLimitX = m_bIsSingle?5:10;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// ** Transformaciones aqui!
	if(m_bUnderAttack) glRotated(180.0, 0.0, 0.0, 1.0);
	if(m_bLeftAttack && m_bIsSingle)
	{
		glRotated(90.0, 0.0, 0.0, 1.0);
		if(!piuconfig.nModeMach) glTranslated(1.4*m_nSizeArrows+10.0, 0.0, 0.0);
	}
	if(m_bRightAttack && m_bIsSingle)
	{
		glRotated(270.0, 0.0, 0.0, 1.0);
		if(!piuconfig.nModeMach) glTranslated(1.4*m_nSizeArrows+10.0, 0.0, 0.0);
	}
	if(m_bNXMode)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glFrustum(-15.0,15.0,-15.0,15.0,30.0,200.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslated(0.0, 30.0, -60.0);
		glRotated(-60.0, 1.0, 0.0, 0.0);
	}
	if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(21.5, 0.0, 0.0);
	else if(m_nPlayer == 2 && m_bIsSingle) glTranslated(43.0, 0.0, 0.0);
	m_dPassVel = 1.0;
	if(m_bEW)
	{
		int nPart = (int)dBeat;
		m_dPassVel = abs(dBeat-(double)nPart-0.5)+0.5;
	}
	if(m_bRV)
	{
		int nPart = (int)dBeat;
		nPart = nPart%4;
		if(nPart == 0 && !m_bRVel)
		{
			evarrows.dSourceVel = m_dUserVel;
			evarrows.dDestVel = (double)(rand()%5+1);
			evarrows.dWWVel = dBeatnStop;
			evarrows.bVel = true;
			m_bRVel = true;
		}
		if(nPart == 3)
		{
			m_bRVel = false;
		}
	}
	////////////////////////////
	// Actualizar velocidad
	if(evarrows.bVel && dBeatnStop >= evarrows.dWWVel)
	{
		if(dBeatnStop >= (evarrows.dWWVel+1.0))
			m_dUserVel = evarrows.dDestVel;
		else
			m_dUserVel = (evarrows.dDestVel-evarrows.dSourceVel)*(dBeatnStop-evarrows.dWWVel)+evarrows.dSourceVel;
	}
	///////////////////////
	UpdateAttack();
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	if(!m_bFreedom)
	{
		for(int i = 0; i < nLimitX; i++)
		{
			int x = nOrder[i];
			t = 0;
			l = DIST_ARROW(double(x))-(m_nSizeArrows/2*(skin[0]->dScale-1.0));
			b = t+(m_bDrop?1:-1)*m_nSizeArrows+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[0]->dScale-1.0));
			r = l+m_nSizeArrows+(m_nSizeArrows*(skin[0]->dScale-1.0));
			SetOffset(&l, &t, &r, &b);
			glBindTexture(GL_TEXTURE_2D, skin[0]->skin[x%5+15]->GetCurrentTex());
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
			glEnd();
		}
		for(int i = 0; i < nLimitX; i++)
		{
			int x = nOrder[i];
			t = 0;
			l = DIST_ARROW(double(x))-(m_nSizeArrows/2*(skin[0]->dScale-1.0));
			b = t+(m_bDrop?1:-1)*m_nSizeArrows+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[0]->dScale-1.0));
			r = l+m_nSizeArrows+(m_nSizeArrows*(skin[0]->dScale-1.0));
			SetOffset(&l, &t, &r, &b);
			o = pow(1-dBeat+(double)((int)dBeat), double(5));
			glBindTexture(GL_TEXTURE_2D, skin[0]->skin[x%5+20]->GetCurrentTex());
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, o);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
			glEnd();
		}
	}
	double dPressBeat[10];
	double dPressBeatD[10];
	for(int x = 0; x < 10; x++)
	{
		double dApBPM, dStopCons, dCaretPress;
		int nPressStop;
		dPressBeatD[x] = dPressBeat[x] = TimeToBeat(evarrows.nArrows[x], &nPressStop, &dStopCons, &dApBPM);
		dCaretPress = dPressBeat[x]+dStopCons;
		for(int g = 0; g < songc.nSizeStops; g++)
		{
			if(dPressBeat[x] > songc.dWStops[g])
			{
				dCaretPress += songc.dStops[g];
			}
			else break;
		}
		for(int g = 0; g < songc.nSizeJumps; g++)
		{
			if(dPressBeat[x] > songc.dIJumps[g])
			{
				dCaretPress -= songc.dEJumps[g]-songc.dIJumps[g];
			}
			else break;
		}
		dPressBeat[x]=dCaretPress;
	}
	for( int n = 0; n < 3; n++)
	{
		int nPArrow = -1;	// Asumir la posible flecha en juego como -1
		vector<int> nPArrowIn;	// Almacenar los indices dentro del juego
		int nSizePai = 0;
		int nArrowBegin = 0;
		double dMinusOffset = m_dLevelMach;
		if(m_bIsArrowsCountable[n]) for(int y = nMin[n]; y < nMax[n]; y++)
		{
			double dUbi = BeatWArrow(y, n);
			double dOffset = dUbi-dBeatnStop;	// Desfase de la flecha con respecto al origen
			int nCombo = 0;
			bool bTake = true, bCount = false;
			bool bPicked[] = {false,false,false,false,false,false,false,false,false,false};
			for(int x = 0; x < nLimitX; x++)
			{
				DWORD dwArrow = GetArrow(x, y, n);
				if(LOWORD(dwArrow) == 1)
				{
					bTake = false;
					break;
				}
			}
			bool bCare = false;
			if(dOffset < (m_dLevelMach*pow(0.25, dPowTimer))) for(int x = 0; x < nLimitX; x++)
			{
				DWORD dwArrow = GetArrow(x, y, n);
				if(LOWORD(dwArrow) >= 2 && LOWORD(dwArrow) <= 4 && (GetArrowStatus(x, y, n) < 1 || GetArrowStatus(x, y, n) > 5))
				{
					if(evarrows.bArrows[x])
					{
						SetArrowStatus(x, y, n, 5);
						bCare = true;
						nCombo = HIWORD(GetArrow(x, nPArrow, n));
					}
				}
			}
			if(bTake && dOffset < (m_dLevelMach*pow(0.25, dPowTimer))) for(int x = 0; x < nLimitX; x++)
			{
				DWORD dwArrow = GetArrow(x, y, n);
				if(LOWORD(dwArrow) >= 2 && LOWORD(dwArrow) <= 4)
				{
					if(GetArrowStatus(x, y, n) < 1 || GetArrowStatus(x, y, n) > 5)
					{
						bCount = false;
						break;
					}
					else
					{
						bCount = true;
						bPicked[x] = true;
						nCombo = HIWORD(GetArrow(x, y, n));
					}
				}
			}
			if(!bCare) bCount = false;
			if(dOffset < (m_dLevelMach*pow(0.25, dPowTimer)) && bCount && bTake)
			{
				status.nPerfect[n]++;
				status.nCombo[n]+=nCombo;
				status.nComboTotal+=nCombo;
				status.nScore += PUNT_EVA1*nCombo;
				PlusCombo();
				status.nLastEva = m_nNext;
				status.nTypeEva = 5;
				status.dLife += dIncrBar*double(nCombo);
				status.nMissCombo[n] = 0;
				status.nMissComboTotal = 0;
				if(status.nCombo[n] > status.nMaxCombo[n]) status.nMaxCombo[n] = status.nCombo[n];
				if(status.nComboTotal > status.nMaxComboTotal) status.nMaxComboTotal = status.nComboTotal;
				for(int x = 0; x < 10; x++)
				{
					if(bPicked[x]) evarrows.nGlowArrows[x] = m_nNext;
				}
			}
			else
			{
				bool bCountThis = false;
				for(int x = 0; x < nLimitX; x++)
				{
					if(LOWORD(GetArrow(x, y, n)) >= 1
						&& (GetArrowStatus(x, y, n) < 1 || GetArrowStatus(x, y, n) > 5))
						bCountThis = true;
				}
				if(bCountThis)
				{
					if(dOffset < -m_dLevelMach)	// Esta flecha se ha perdido
					{
						bool bCountMiss = false;
						for(int x = 0; x < nLimitX; x++)
						{
							if(LOWORD(GetArrow(x, y, n)) >= 1 && LOWORD(GetArrow(x, y, n)) <= 4 && (GetArrowStatus(x, y, n) < 1 || GetArrowStatus(x, y, n) > 5))
							{
								SetArrowStatus(x, y, n, 1);	// Contar MISS
								bCountMiss = true;
							}
							if(LOWORD(GetArrow(x, y, n)) > 4 && (GetArrowStatus(x, y, n) < 1 || GetArrowStatus(x, y, n) > 5))
							{
								SetArrowStatus(x, y, n, 1);	// Contar MISS
							}
						}
						// Aqui cuenta un miss
						if(bCountMiss && SomeArrow(y, n) && m_bIsArrowsMisible[n])
						{
							status.nLastEva = m_nNext;
							status.nTypeEva = 1;
							status.nMiss[n]++;
							status.nScore+=PUNT_EVA5;
							MinusCombo();
							status.dLife -= dDecrBar;
							status.nCombo[n] = 0;
							status.nComboTotal = 0;
							status.nMissCombo[n]++;
							status.nMissComboTotal++;
							if(status.nMissCombo[n] > status.nMaxMissCombo[n]) status.nMaxMissCombo[n] = status.nMissCombo[n];
							if(status.nMissComboTotal > status.nMaxMissComboTotal) status.nMaxMissComboTotal = status.nMissComboTotal;
						}
					}
					else if(abs(dOffset) <= m_dLevelMach)	// Esta flecha se toma en cuenta!
					{
						nPArrowIn.push_back(y);
						if(dMinusOffset >= abs(dOffset) && SomeNote(y, n))
						{
							dMinusOffset = abs(dOffset);
							nPArrow = y;
						}
					}
					else if(dOffset > m_dLevelMach)	// Esta flecha no cuenta
					{
						break;
					}
				}
			}
		}

		nSizePai = nPArrowIn.size();
		for(int u = 0; u < nSizePai; u++)
		{
			nPArrow = nPArrowIn[u];
			int nCombo;
			double dUbi = BeatWArrow(nPArrow, n);
			double dOffset = 0.0;
			dOffset = 0.0;
			bool bCount = false;
			bool bPicked[] = {false,false,false,false,false,false,false,false,false,false};
			int nCounted = 0;
			for(int x = 0; x < nLimitX; x++)
			{
				double off = 0.0;
				DWORD dwArrow = GetArrow(x, nPArrow, n);
				if(LOWORD(dwArrow) == 1 && (GetArrowStatus(x, nPArrow, n) < 1 || GetArrowStatus(x, nPArrow, n) > 5)) // Flechas
				{
					off = abs(dPressBeat[x]-dUbi);
					bPicked[x] = true;
					bCount = true;
					nCombo = HIWORD(GetArrow(x, nPArrow, n));
					nCounted++;
				}
				if(LOWORD(dwArrow) >= 2 && LOWORD(dwArrow) <= 4 && evarrows.bArrows[x] && (dUbi-dBeatnStop) < (m_dLevelMach*pow(0.25, dPowTimer))
					&& (GetArrowStatus(x, nPArrow, n) < 1 || GetArrowStatus(x, nPArrow, n) > 5)) // Frizz
				{
					off = 0.0;	// esto porque es un frizz
					bPicked[x] = true;
					bCount = true;
					nCombo = HIWORD(GetArrow(x, nPArrow, n));
				}
				else if(LOWORD(dwArrow) >= 2 && LOWORD(dwArrow) <= 4
					&& (GetArrowStatus(x, nPArrow, n) < 1 || GetArrowStatus(x, nPArrow, n) > 5))
				{
					off = m_dLevelMach+0.1;
					bCount = true;
					nCombo = HIWORD(GetArrow(x, nPArrow, n));
				}
				if(LOWORD(dwArrow) > 4 && abs(dPressBeat[x]-dUbi)<=m_dLevelMach && GetArrowStatus(x, nPArrow, n) != 5 && u == 0) // Objetos
				{
					evarrows.nArrows[x] = m_nOffset;
					evarrows.nGlowArrows[x] = m_nNext;
					bPicked[x] = true;
					SetArrowStatus(x, nPArrow, n, 5);
					//off = dOffset;
					switch(LOWORD(dwArrow))
					{
					case 5:	// Minas
						status.nItems++;
						status.dLife -= double(HIWORD(dwArrow))*dDecrBar;
						status.nMines++;
						evarrows.bHidden = true;
						evarrows.nHidden = m_nNext;
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, sound_mine, 0, &channels[9]);
						break;
					case 6:	// Pociones
						status.nItems++;
						status.dLife += double(HIWORD(dwArrow))*dDecrBar;
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
						if(m_bDrop)
						{
							m_dClipMin1 = -m_dClipMin1;
							m_dClipMax1 = -m_dClipMax1;
							m_dClipMin2 = -m_dClipMin2;
							m_dClipMax2 = -m_dClipMax2;
							m_dClipMin3 = -m_dClipMin3;
							m_dClipMax3 = -m_dClipMax3;
							double dTemp;
							dTemp = m_dClipMin1;
							m_dClipMin1 = m_dClipMax1;
							m_dClipMax1 = dTemp;
							dTemp = m_dClipMin2;
							m_dClipMin2 = m_dClipMax2;
							m_dClipMax2 = dTemp;
							dTemp = m_dClipMin3;
							m_dClipMin3 = m_dClipMax3;
							m_dClipMax3 = dTemp;
						}
						m_bDrop = false;
						m_bXMode = false;
						m_bNXMode = false;
						evarrows.bHidden = true;
						evarrows.nHidden = m_nNext;
						break;
					case 10:	// Velocidad x1
						status.nItems++;
						status.nVelocitys++;
						evarrows.dSourceVel = m_dUserVel;
						evarrows.dDestVel = 1.0;
						evarrows.dWWVel = dBeatnStop;
						evarrows.bVel = true;
						break;
					case 11:	// Velocidad x2
						status.nItems++;
						status.nVelocitys++;
						evarrows.dSourceVel = m_dUserVel;
						evarrows.dDestVel = 2.0;
						evarrows.dWWVel = dBeatnStop;
						evarrows.bVel = true;
						break;
					case 12:	// Velocidad x3
						status.nItems++;
						status.nVelocitys++;
						evarrows.dSourceVel = m_dUserVel;
						evarrows.dDestVel = 3.0;
						evarrows.dWWVel = dBeatnStop;
						evarrows.bVel = true;
						break;
					case 13:	// Velocidad x4
						status.nItems++;
						status.nVelocitys++;
						evarrows.dSourceVel = m_dUserVel;
						evarrows.dDestVel = 4.0;
						evarrows.dWWVel = dBeatnStop;
						evarrows.bVel = true;
						break;
					case 14:	// Velocidad x5
						status.nItems++;
						status.nVelocitys++;
						evarrows.dSourceVel = m_dUserVel;
						evarrows.dDestVel = 5.0;
						evarrows.dWWVel = dBeatnStop;
						evarrows.bVel = true;
						break;
					case 15:	// Velocidad x8
						status.nItems++;
						status.nVelocitys++;
						evarrows.dSourceVel = m_dUserVel;
						evarrows.dDestVel = 8.0;
						evarrows.dWWVel = dBeatnStop;
						evarrows.bVel = true;
						break;
					case 16:	// Drop
						status.nItems++;
						m_bDrop = true;
						if(m_bDrop)
						{
							m_dClipMin1 = -m_dClipMin1;
							m_dClipMax1 = -m_dClipMax1;
							m_dClipMin2 = -m_dClipMin2;
							m_dClipMax2 = -m_dClipMax2;
							m_dClipMin3 = -m_dClipMin3;
							m_dClipMax3 = -m_dClipMax3;
							double dTemp;
							dTemp = m_dClipMin1;
							m_dClipMin1 = m_dClipMax1;
							m_dClipMax1 = dTemp;
							dTemp = m_dClipMin2;
							m_dClipMin2 = m_dClipMax2;
							m_dClipMax2 = dTemp;
							dTemp = m_dClipMin3;
							m_dClipMin3 = m_dClipMax3;
							m_dClipMax3 = dTemp;
						}
						m_bUnderAttack = false;
						m_bLeftAttack = false;
						m_bRightAttack = false;
						evarrows.bHidden = true;
						evarrows.nHidden = m_nNext;
						break;
					case 17:	// UnderAttack
						status.nItems++;
						if(m_bDrop)
						{
							m_dClipMin1 = -m_dClipMin1;
							m_dClipMax1 = -m_dClipMax1;
							m_dClipMin2 = -m_dClipMin2;
							m_dClipMax2 = -m_dClipMax2;
							m_dClipMin3 = -m_dClipMin3;
							m_dClipMax3 = -m_dClipMax3;
							double dTemp;
							dTemp = m_dClipMin1;
							m_dClipMin1 = m_dClipMax1;
							m_dClipMax1 = dTemp;
							dTemp = m_dClipMin2;
							m_dClipMin2 = m_dClipMax2;
							m_dClipMax2 = dTemp;
							dTemp = m_dClipMin3;
							m_dClipMin3 = m_dClipMax3;
							m_dClipMax3 = dTemp;
						}
						m_bDrop = false;
						m_bUnderAttack = true;
						m_bLeftAttack = false;
						m_bRightAttack = false;
						evarrows.bHidden = true;
						evarrows.nHidden = m_nNext;
						break;
					case 18:	// LeftAttack
						status.nItems++;
						if(m_bDrop)
						{
							m_dClipMin1 = -m_dClipMin1;
							m_dClipMax1 = -m_dClipMax1;
							m_dClipMin2 = -m_dClipMin2;
							m_dClipMax2 = -m_dClipMax2;
							m_dClipMin3 = -m_dClipMin3;
							m_dClipMax3 = -m_dClipMax3;
							double dTemp;
							dTemp = m_dClipMin1;
							m_dClipMin1 = m_dClipMax1;
							m_dClipMax1 = dTemp;
							dTemp = m_dClipMin2;
							m_dClipMin2 = m_dClipMax2;
							m_dClipMax2 = dTemp;
							dTemp = m_dClipMin3;
							m_dClipMin3 = m_dClipMax3;
							m_dClipMax3 = dTemp;
						}
						m_bDrop = false;
						m_bUnderAttack = false;
						m_bLeftAttack = true;
						m_bRightAttack = false;
						evarrows.bHidden = true;
						evarrows.nHidden = m_nNext;
						break;
					case 19:	// RightAttack
						status.nItems++;
						if(m_bDrop)
						{
							m_dClipMin1 = -m_dClipMin1;
							m_dClipMax1 = -m_dClipMax1;
							m_dClipMin2 = -m_dClipMin2;
							m_dClipMax2 = -m_dClipMax2;
							m_dClipMin3 = -m_dClipMin3;
							m_dClipMax3 = -m_dClipMax3;
							double dTemp;
							dTemp = m_dClipMin1;
							m_dClipMin1 = m_dClipMax1;
							m_dClipMax1 = dTemp;
							dTemp = m_dClipMin2;
							m_dClipMin2 = m_dClipMax2;
							m_dClipMax2 = dTemp;
							dTemp = m_dClipMin3;
							m_dClipMin3 = m_dClipMax3;
							m_dClipMax3 = dTemp;
						}
						m_bDrop = false;
						m_bUnderAttack = false;
						m_bLeftAttack = false;
						m_bRightAttack = true;
						evarrows.bHidden = true;
						evarrows.nHidden = m_nNext;
						break;
					case 20:	// Snake
						status.nItems++;
						m_bSnake = true;
						evarrows.bHidden = true;
						evarrows.nHidden = m_nNext;
						break;
					}
					bPicked[x] = true;
					evarrows.nArrows[x] = GetDocument()->song.hStep.nOffset;
					dPressBeat[x] = 0.0;
				}
				if(LOWORD(dwArrow) <= 4 && off >= dOffset) dOffset = off;
			}
			if(nCounted == 1 && u > 0) bCount = false;
			if(dOffset <= m_dLevelMach && bCount == true) // Esta entre el perfect y el bad
			{
				if(dOffset <= (m_dLevelMach*pow(1.0, dPowTimer)) && dOffset > (m_dLevelMach*pow(0.75, dPowTimer))) // Bad
				{
					for(int x = 0; x < nLimitX; x++)
					{
						if(LOWORD(GetArrow(x, nPArrow, n)) == 1)
							SetArrowStatus(x, nPArrow, n, 2);	// Contar BAD
						if(LOWORD(GetArrow(x, nPArrow, n)) >= 2 && LOWORD(GetArrow(x, nPArrow, n)) <= 4)
							SetArrowStatus(x, nPArrow, n, 5);	// Contar PERFECT PARA LOS FRIZZES
					}
					status.nBad[n]++;
					status.nScore += PUNT_EVA4;
					status.nLastEva = m_nNext;
					status.nTypeEva = 2;
					status.nCombo[n] = 0;
					status.nComboTotal = 0;
					status.dLife -= dDecrBar*0.5;
				}
				if(dOffset <= (m_dLevelMach*pow(0.75, dPowTimer)) && dOffset > (m_dLevelMach*pow(0.5, dPowTimer))) // Good
				{
					for(int x = 0; x < nLimitX; x++)
					{
						if(LOWORD(GetArrow(x, nPArrow, n)) == 1)
							SetArrowStatus(x, nPArrow, n, 3);	// Contar GOOD
						if(LOWORD(GetArrow(x, nPArrow, n)) >= 2 && LOWORD(GetArrow(x, nPArrow, n)) <= 4)
							SetArrowStatus(x, nPArrow, n, 5);	// Contar PERFECT PARA LOS FRIZZES
					}
					status.nGood[n]++;
					status.nScore += PUNT_EVA3;
					PlusCombo();
					status.nLastEva = m_nNext;
					status.nTypeEva = 3;
					status.nMissCombo[n] = 0;
					status.nMissComboTotal = 0;
				}
				if(dOffset <= (m_dLevelMach*pow(0.5, dPowTimer)) && dOffset > (m_dLevelMach*pow(0.25, dPowTimer))) // Great
				{
					for(int x = 0; x < nLimitX; x++)
					{
						if(LOWORD(GetArrow(x, nPArrow, n)) == 1)
							SetArrowStatus(x, nPArrow, n, 4);	// Contar GREAT
						if(LOWORD(GetArrow(x, nPArrow, n)) >= 2 && LOWORD(GetArrow(x, nPArrow, n)) <= 4)
							SetArrowStatus(x, nPArrow, n, 5);	// Contar PERFECT PARA LOS FRIZZES
					}
					status.nGreat[n]+=nCombo;
					status.nCombo[n]+=nCombo;
					status.nComboTotal+=nCombo;
					status.nScore += PUNT_EVA2*nCombo;
					PlusCombo();
					status.nLastEva = m_nNext;
					status.nTypeEva = 4;
					if(status.nCombo[n] > status.nMaxCombo[n]) status.nMaxCombo[n] = status.nCombo[n];
					if(status.nComboTotal > status.nMaxComboTotal) status.nMaxComboTotal = status.nComboTotal;
					status.dLife += dIncrBar*0.5*double(nCombo);
					status.nMissCombo[n] = 0;
					status.nMissComboTotal = 0;
					for(int x = 0; x < 10; x++)
					{
						if(bPicked[x]) evarrows.nGlowArrows[x] = m_nNext;
					}
				}
				if(dOffset <= (m_dLevelMach*pow(0.25, dPowTimer)) && dOffset >= 0.0)  // Perfect
				{
					for(int x = 0; x < nLimitX; x++)
					{
						if(LOWORD(GetArrow(x, nPArrow, n)) == 1)
							SetArrowStatus(x, nPArrow, n, 5);	// Contar perfect
						if(LOWORD(GetArrow(x, nPArrow, n)) >= 2 && LOWORD(GetArrow(x, nPArrow, n)) <= 4)
							SetArrowStatus(x, nPArrow, n, 5);	// Contar PERFECT PARA LOS FRIZZES
					}
					status.nPerfect[n]+=nCombo;
					status.nScore += PUNT_EVA1*nCombo;
					PlusCombo();
					status.nLastEva = m_nNext;
					status.nTypeEva = 5;
					status.nCombo[n]+=nCombo;
					status.nComboTotal+=nCombo;
					if(status.nCombo[n] > status.nMaxCombo[n]) status.nMaxCombo[n] = status.nCombo[n];
					if(status.nComboTotal > status.nMaxComboTotal) status.nMaxComboTotal = status.nComboTotal;
					status.dLife += dIncrBar*double(nCombo);
					status.nMissCombo[n] = 0;
					status.nMissComboTotal = 0;
					for(int x = 0; x < 10; x++)
					{
						if(bPicked[x]) evarrows.nGlowArrows[x] = m_nNext;
					}
				}
				for(int x = 0; x < 10; x++) // Asumir que la ultima vez que se presionó fue en el comienzo
					if(bPicked[x])
					{
						evarrows.nArrows[x] = GetDocument()->song.hStep.nOffset;
						dPressBeat[x] = 0.0;
					}
			}
		}
	}

	for(int n = 0; n < 3; n++)
	{
		for(int i = 0; i < nLimitX; i++)
		{
			int x = nOrder[i];
			for(int y = nMin[n]; y < nMax[n]; y++)
			{
				int nSkin = 0;
				if(1)
				{
					DWORD dwDArrow = LOWORD(GetArrow(x, y, n));
					bool bActiveX = true;
					if((dwDArrow == 1 || dwDArrow >= 5) && (GetArrowStatus(x, y, n) < 4 || (BeatArrow(y, n)-dBeat)>0.0))
					{
						t = (BeatArrow(y, n)-dBeat)*m_nDistArrows;
						if(t <= 0) bActiveX = false;
						l = DIST_ARROW(double(x))-(dwDArrow==1?(m_nSizeArrows/2*(skin[n]->dScale-1.0)):0.0);
						ModifyPosArrows(&l, &t);
						ModifyPosArrows2(&l, &t, x, y, n);
						if(m_bXMode && bActiveX)
						{
							if(x < 5 && (m_nPlayer == 1 || !m_bIsSingle))
								l += (m_bDrop?1:-1)*t;
							else
								l -= (m_bDrop?1:-1)*t;
						}
						b = t+(m_bDrop?1:-1)*m_nSizeArrows+(m_bDrop?1:-1)*(dwDArrow==1?(m_nSizeArrows*(skin[n]->dScale-1.0)):0.0);
						r = l+m_nSizeArrows+(dwDArrow==1?(m_nSizeArrows*(skin[n]->dScale-1.0)):0.0);
						DeriveOpacityPos(t, &o, n);
						DeriveOpacityPos(b, &ob, n);
						DeriveOpacityPos2(t, &o, y, n);
						DeriveOpacityPos2(b, &ob, y, n);
						SetOffset(&l, &t, &r, &b);
						if(dwDArrow==1)
						{
							if(m_bTrainingMode)
							{
								if(n == 0) nSkin = trac.nArg[y*10+x];
								else nSkin = 0;
							}
							else nSkin = n;
							glBindTexture(GL_TEXTURE_2D, skin[nSkin]->skin[abs(x%5)]->GetCurrentTex());
						}
						else
							glBindTexture(GL_TEXTURE_2D, common->skin[abs((int)dwDArrow-5)]->GetCurrentTex());
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, o);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
						glColor4d(1.0, 1.0, 1.0, ob);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
						glEnd();
					}
					if(dwDArrow >= 2 && dwDArrow <= 4)
					{
						int j, k;
						int nLim = songc.nSizeArrows1;
						if(n == 1) nLim = songc.nSizeArrows2;
						if(n == 2) nLim = songc.nSizeArrows3;
						bool bOptCond = false;
						//if((BeatArrow(y, n)-dBeat)<0.0) bOptCond=true;
						for(j = y; j < nLim; j++)
						{
							if(LOWORD(GetArrow(x, j, n)) == 4 || (GetArrowStatus(x, j+1, n)>3 && bOptCond))
							{
								break;
							}
						}
						for(k = y; k >= 0; k--)
						{
							if(LOWORD(GetArrow(x, k, n)) == 3 || (GetArrowStatus(x, j-1, n)>3 && bOptCond))
							{
								break;
							}
						}
						double dIfBeat = BeatArrow(k, n);
						double dEfBeat = BeatArrow(j, n);
						double ob;
						double st, sl, sb, sr;
						bool bActiveX = true;
						st = (dIfBeat-dBeat)*m_nDistArrows;
						if(st <= 0) bActiveX = false;
						sl = DIST_ARROW(double(x))-(m_nSizeArrows/2*(skin[n]->dScale-1.0));
						ModifyPosArrows(&sl, &st);
						ModifyPosArrows2(&sl, &st, x, y, n);
						if(m_bXMode && bActiveX)
						{
							if(x < 5 && (m_nPlayer == 1 || !m_bIsSingle))
								sl += (m_bDrop?1:-1)*st;
							else
								sl -= (m_bDrop?1:-1)*st;
						}
						sb = st+(m_bDrop?1:-1)*m_nSizeArrows+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[n]->dScale-1.0));
						sr = sl+m_nSizeArrows+(m_nSizeArrows*(skin[n]->dScale-1.0));
						if(j != y)
						{
							bool bDrawEx = false;
							l = sl;
							t = (BeatArrow(y, n)-dBeat)*m_nDistArrows;
							ModifyPosArrows(&l, &t);
							b = (BeatArrow(y+1, n)-dBeat)*m_nDistArrows;
							ModifyPosArrows(&r, &b);
							if(y == k)
							{
								t=st+(m_bDrop?1:-1)*m_nSizeArrows/2+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[n]->dScale-1.0))/2;
								if(((m_bDrop?1:-1)*b)<((m_bDrop?1:-1)*sb)) b = sb;
							}
							else
							{
								if(((m_bDrop?1:-1)*t)<((m_bDrop?1:-1)*sb)) t = sb;
								if(((m_bDrop?1:-1)*b)<((m_bDrop?1:-1)*sb)) b = sb;
							}
							if(!(((m_bDrop?1:-1)*b)<=((m_nSizeArrows/2)+(m_nSizeArrows*(skin[n]->dScale-1.0))/2) && GetArrowStatus(x, y, n) >= 4))
							{
								 //&& !((BeatArrow(y, n)-dBeat) <= 0.0 && (evarrows.bArrows[x] || GetArrowStatus(x, y, n) >= 4))
								if(evarrows.bArrows[x] &&
									((m_bDrop?1:-1)*t)<=((m_nSizeArrows/2)+(m_nSizeArrows*(skin[n]->dScale-1.0))/2))
								{
									t = (m_bDrop?1:-1)*m_nSizeArrows/2+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[n]->dScale-1.0))/2;
									bDrawEx = true;
								}
								if(evarrows.bArrows[x] && ((m_bDrop?1:-1)*b)<=((m_nSizeArrows/2)+(m_nSizeArrows*(skin[n]->dScale-1.0))/2))
									b = (m_bDrop?1:-1)*m_nSizeArrows/2+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[n]->dScale-1.0))/2;
								l = sl;
								r = sr;
								DeriveOpacityPos(t, &o, n);
								DeriveOpacityPos(b, &ob, n);
								DeriveOpacityPos2(t, &o, y, n);
								DeriveOpacityPos2(b, &ob, y, n);
								SetOffset(&l, &t, &r, &b);
								if(m_bTrainingMode)
								{
									if(n == 0) nSkin = trac.nArg[y*10+x];
									else nSkin = 0;
								}
								else nSkin = n;
								glBindTexture(GL_TEXTURE_2D, skin[nSkin]->skin[x%5+5]->GetCurrentTex());
								glBegin(GL_QUADS);
								glColor4d(1.0, 1.0, 1.0, o);
								glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
								glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
								glColor4d(1.0, 1.0, 1.0, ob);
								glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
								glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
								glEnd();
								if(bDrawEx)
								{
									t = 0;
									b = t+(m_bDrop?1:-1)*m_nSizeArrows+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[n]->dScale-1.0));
									l = sl;
									r = sr;
									SetOffset(&l, &t, &r, &b);
									glBindTexture(GL_TEXTURE_2D, skin[nSkin]->skin[x%5]->GetCurrentTex());
									glBegin(GL_QUADS);
									glColor4d(1.0, 1.0, 1.0, 1.0);
									glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
									glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
									glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
									glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
									glEnd();
								}
							}
						}
						if(j==y && !((BeatArrow(y, n)-dBeat) <= 0.0 && (evarrows.bArrows[x] || GetArrowStatus(x, y, n) >= 4)))
						{
							t = (dEfBeat-dBeat)*m_nDistArrows;
							l = DIST_ARROW(double(x));
							ModifyPosArrows(&l, &t);
							l -= (m_nSizeArrows/2*(skin[0]->dScale-1.0));
							b = t+(m_bDrop?1:-1)*m_nSizeArrows+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[n]->dScale-1.0));
							//r = l+m_nSizeArrows;
							l = sl;
							r = sr;
							if(((m_bDrop?1:-1)*b) < ((m_bDrop?1:-1)*st+m_nSizeArrows/2+(m_nSizeArrows*(skin[n]->dScale-1.0))/2))
								b = st+(m_bDrop?1:-1)*m_nSizeArrows/2;
							if(((m_bDrop?1:-1)*t) < ((m_bDrop?1:-1)*st+m_nSizeArrows/2+(m_nSizeArrows*(skin[n]->dScale-1.0))/2))
								t = st+(m_bDrop?1:-1)*m_nSizeArrows/2;
							if(evarrows.bArrows[x] && ((m_bDrop?1:-1)*t)<=((m_nSizeArrows/2)+(m_nSizeArrows*(skin[n]->dScale-1.0))/2))
								t = (m_bDrop?1:-1)*m_nSizeArrows/2+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[n]->dScale-1.0))/2;
							if(evarrows.bArrows[x] && ((m_bDrop?1:-1)*b)<=((m_nSizeArrows/2)+(m_nSizeArrows*(skin[n]->dScale-1.0))/2))
								b = (m_bDrop?1:-1)*m_nSizeArrows/2+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[n]->dScale-1.0))/2;
							double dAc = 1-abs(b-t)/(m_nSizeArrows*skin[n]->dScale);
							dAc = max(0.0, min(1.0, dAc));
							DeriveOpacityPos(t, &o, n);
							DeriveOpacityPos(b, &ob, n);
							DeriveOpacityPos2(t, &o, y, n);
							DeriveOpacityPos2(b, &ob, y, n);
							SetOffset(&l, &t, &r, &b);
							if(m_bTrainingMode)
							{
								if(n == 0) nSkin = trac.nArg[y*10+x];
								else nSkin = 0;
							}
							else nSkin = n;
							glBindTexture(GL_TEXTURE_2D, skin[nSkin]->skin[x%5+10]->GetCurrentTex());
							glBegin(GL_QUADS);
							glColor4d(1.0, 1.0, 1.0, o);
							glTexCoord2d(0.0, 1-dAc); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
							glTexCoord2d(1.0, 1-dAc); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
							glColor4d(1.0, 1.0, 1.0, ob);
							glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
							glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
							glEnd();
						}
						if(k==y && !((BeatArrow(y, n)-dBeat) <= 0.0 && (evarrows.bArrows[x] || GetArrowStatus(x, y, n) >= 4)))
						{
							DeriveOpacityPos(st, &o, n);
							DeriveOpacityPos(sb, &ob, n);
							DeriveOpacityPos2(st, &o, y, n);
							DeriveOpacityPos2(sb, &ob, y, n);
							SetOffset(&sl, &st, &sr, &sb);
							if(m_bTrainingMode)
							{
								if(n == 0) nSkin = trac.nArg[y*10+x];
								else nSkin = 0;
							}
							else nSkin = n;
							// Flecha
							glBindTexture(GL_TEXTURE_2D, skin[nSkin]->skin[x%5]->GetCurrentTex());
							glBegin(GL_QUADS);
							glColor4d(1.0, 1.0, 1.0, o);
							glTexCoord2d(0.0, 1.0); glVertex3d(sl, st, -50.0-OffsetInZArrow(x));
							glTexCoord2d(1.0, 1.0); glVertex3d(sr, st, -50.0-OffsetInZArrow(x));
							glColor4d(1.0, 1.0, 1.0, ob);
							glTexCoord2d(1.0, 0.0); glVertex3d(sr, sb, -50.0-OffsetInZArrow(x));
							glTexCoord2d(0.0, 0.0); glVertex3d(sl, sb, -50.0-OffsetInZArrow(x));
							glEnd();
						}
					}
				}
			}
		}
	}	// Fin del dibujo y reconocimiento
	// Dibujo y reconocimiento de Brain Shower
	int nPArrow = -1;	// Asumir la posible flecha en juego como -1
	bool bTimize = false;	// Asumir que es una respuesta legitima de BS
	double dMinusOffset = m_dLevelMach;
	if(m_bIsArrowsCountable[0]) for(int y = 0; y < bsc.nSizeBSAnswers; y++)
	{
		bool bCountThis = false;
		for(int x = 0; x < nLimitX; x++)
		{
			if(bsc.dwBSAnswers
				&& (status.nBSStatus1[y*10+x] != 1))
				bCountThis = true;
			if(bsc.dwBSAnswers
				&& (status.nBSStatus1[y*10+x] == 1))
			{
				bCountThis = false;
				break;
			}
		}
		if(bCountThis)
		{
			double dUbi = bsc.dWWBSAnswers[y];
			double dOffset = dUbi-dBeatnStop;	// Desfase de la flecha con respecto al origen
			if(dOffset < -m_dLevelMach)	// Esta flecha se ha perdido
			{
				// Aqui cuenta un X
				if(m_bIsArrowsMisible[0])
				{
					for(int x = 0; x < nLimitX; x++)
						if(bsc.nBSAnswers[y*10+x]) status.nBSStatus1[y*10+x] = 1;
					status.nLastBSEva = m_nNext;
					status.nTypeBSEva = 1;
					status.nBSIncorrect++;
				}
			}
			else if(dOffset >= -m_dLevelMach && dOffset <= m_dLevelMach)	// Esta flecha se toma en cuenta!
			{
				if(dMinusOffset >= abs(dOffset))
				{
					dMinusOffset = abs(dOffset);
					nPArrow = y;
				}
			}
			else if(dOffset > m_dLevelMach)	// Esta flecha no cuenta
			{
				break;
			}
		}
	}
	if(nPArrow != -1)
	{
		bool bCountThis = false;
		for(int x = 0; x < nLimitX; x++)
		{
			if(bsc.dwBSAnswers
				&& (status.nBSStatus1[nPArrow*10+x] != 1))
				bCountThis = true;
			if(bsc.dwBSAnswers
				&& (status.nBSStatus1[nPArrow*10+x] == 1))
			{
				bCountThis = false;
				break;
			}
		}
		if(bCountThis)
		{
			double dUbi = bsc.dWWBSAnswers[nPArrow];
			double dOffset = m_dLevelMach;
			bool bCount = false;
			int nArrow = -1;
			for(int x = 0; x < nLimitX; x++)
			{
				double off;
				DWORD dwArrow = bsc.dwBSAnswers[nPArrow*10+x];
				int nAnswer = bsc.nBSAnswers[nPArrow*10+x];
				if(dwArrow)
				{
					off = abs(dPressBeat[x]-dUbi);
					bCount = true;
				}
				if(dwArrow && off <= dOffset)
				{
					dOffset = off;
					nArrow = x;
				}
				if(HIWORD(dwArrow) == 0 && dwArrow)
				{
					bTimize = true;
				}
			}
			if(dOffset <= m_dLevelMach && bCount == true && nArrow != -1) // Esta entre el perfect y el bad
			{
				if(!bTimize)
				{
					if(dOffset <= m_dLevelMach && dOffset >= 0)
					{
						status.nBSStatus1[nPArrow*10+nArrow] = 1;
						evarrows.nGlowArrows[nArrow] = m_nNext;
						if(bsc.nBSAnswers[nPArrow*10+nArrow])
						{
							status.nBSCorrect++;
							status.nLastBSEva = m_nNext;
							status.nTypeBSEva = 2;
						}
						else
						{
							status.nBSIncorrect++;
							status.nLastBSEva = m_nNext;
							status.nTypeBSEva = 1;
						}
					}
				}
				else
				{
					if(dOffset <= (m_dLevelMach*pow(1.0, dPowTimer)) && dOffset > (m_dLevelMach*pow(0.75, dPowTimer))) // Bad
					{
						status.nBSStatus1[nPArrow*10+nArrow] = 1;
						status.nBad[0]++;
						status.nLastEva = m_nNext;
						status.nTypeEva = 2;
						status.nCombo[0] = 0;
						status.nComboTotal = 0;
						status.dLife -= dDecrBar*0.5;
						status.nScore += PUNT_EVA4;

					}
					if(dOffset <= (m_dLevelMach*pow(0.75, dPowTimer)) && dOffset > (m_dLevelMach*pow(0.5, dPowTimer))) // Good
					{
						status.nBSStatus1[nPArrow*10+nArrow] = 1;
						status.nGood[0]++;
						status.nLastEva = m_nNext;
						status.nTypeEva = 3;
						status.nMissCombo[0] = 0;
						status.nMissComboTotal = 0;
						status.nScore += PUNT_EVA3;
						PlusCombo();

					}
					if(dOffset <= (m_dLevelMach*pow(0.5, dPowTimer)) && dOffset > (m_dLevelMach*pow(0.25, dPowTimer))) // Great
					{
						status.nBSStatus1[nPArrow*10+nArrow] = 1;
						status.nGreat[0]++;
						status.nLastEva = m_nNext;
						status.nTypeEva = 4;
						status.nCombo[0]+=1;
						status.nComboTotal+=1;
						if(status.nCombo[0] > status.nMaxCombo[0]) status.nMaxCombo[0] = status.nCombo[0];
						if(status.nComboTotal > status.nMaxComboTotal) status.nMaxComboTotal = status.nComboTotal;
						status.nScore += PUNT_EVA2;
						PlusCombo();
						status.dLife += dIncrBar*0.5;
						status.nMissCombo[0] = 0;
						status.nMissComboTotal = 0;
						evarrows.nGlowArrows[nArrow] = m_nNext;


					}
					if(dOffset <= (m_dLevelMach*pow(0.25, dPowTimer)) && dOffset >= 0)  // Perfect
					{
						status.nBSStatus1[nPArrow*10+nArrow] = 1;
						status.nPerfect[0]++;
						status.nCombo[0]+=1;
						status.nComboTotal+=1;
						status.nScore += PUNT_EVA1;
						PlusCombo();
						status.nLastEva = m_nNext;
						status.nTypeEva = 5;
						if(status.nCombo[0] > status.nMaxCombo[0]) status.nMaxCombo[0] = status.nCombo[0];
						if(status.nComboTotal > status.nMaxComboTotal) status.nMaxComboTotal = status.nComboTotal;
						status.dLife += dIncrBar;
						status.nMissCombo[0] = 0;
						status.nMissComboTotal = 0;
						evarrows.nGlowArrows[nArrow] = m_nNext;


					}
				}
				evarrows.nArrows[nArrow] = m_nOffset;
			}
		}
	}
	for(int y = 0; y < bsc.nSizeBSAnswers; y++)
	{
		for(int i = 0; i < nLimitX; i++)
		{
			int nSkin = 0;
			int x = nOrder[i];
			if(status.nBSStatus1[y*10+x] <= 0 || status.nBSStatus1[y*10+x] > 2)
			{
				if(bsc.dwBSAnswers[y*10+x])
				{
					DWORD dwDArrow = LOWORD(bsc.dwBSAnswers[y*10+x]);
					DWORD dwGArrow = HIWORD(bsc.dwBSAnswers[y*10+x]);
					bool bActiveX = true;
					t = (bsc.dWBSAnswers[y]-dBeat)*m_nDistArrows;
					if(t <= 0) bActiveX = false;
					l = DIST_ARROW(double(x));
					ModifyPosArrows(&l, &t);
					ModifyPosArrows2(&l, &t, x, y, 3);
					if(m_bXMode && bActiveX)
					{
						if(x < 5 && (m_nPlayer == 1 || !m_bIsSingle))
							l += (m_bDrop?1:-1)*t;
						else
							l -= (m_bDrop?1:-1)*t;
					}
					b = t+(m_bDrop?1:-1)*m_nSizeArrows;
					r = l+m_nSizeArrows;
					DeriveOpacityPos(t, &o, 0);
					DeriveOpacityPos(b, &ob, 0);
					DeriveOpacityPos2(t, &o, y, 0);
					DeriveOpacityPos2(b, &ob, y, 0);
					SetOffset(&l, &t, &r, &b);
					if(bsc.nDBSAnswers[y] == 1 && dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
					{
						glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, o);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
						glColor4d(1.0, 1.0, 1.0, ob);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
						glEnd();
						CString str;
						str.Format(_T("%d"), dwDArrow);
						double dMinL = (l+m_nSizeArrows/2)-m_nSizeArrows/4*str.GetLength();
						for(int z = 0; z < str.GetLength(); z++)
						{
							double ll = dMinL+m_nSizeArrows/2*(double)z;
							double rr = dMinL+m_nSizeArrows/2*(double)(z+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(z);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[nAcc]);
							glBegin(GL_QUADS);
							glColor4d(1.0, 1.0, 1.0, o);
							glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0-OffsetInZArrow(x));
							glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0-OffsetInZArrow(x));
							glColor4d(1.0, 1.0, 1.0, ob);
							glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0-OffsetInZArrow(x));
							glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0-OffsetInZArrow(x));
							glEnd();
						}
					}
					else if(dwGArrow == 0)
					{
						glBindTexture(GL_TEXTURE_2D, skin[2]->skin[x%5]->GetCurrentTex());
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, o);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
						glColor4d(1.0, 1.0, 1.0, ob);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
						glEnd();
					}
					else if(dwGArrow == 18)
					{
						o = 1.0;
						double dClipMin = 0.2;
						double dClipMax = -1.0;
						// Primera asignacion
						dClipMin = -(double)dwDArrow/10+0.2;
						if(dwDArrow == 0) dClipMin = 0.2;
						if((dClipMax*60.0) > t && (dClipMax*60.0-10.0) < t)
						o = 1-abs(dClipMax*60.0-t)/10;
						else if((dClipMax*60.0-10.0) >= t) o = 0.0;
						if((dClipMin*60.0) < t && (dClipMin*60.0+10.0) > t)
							o = 1-abs(dClipMin*60.0-t)/10;
						else if((dClipMin*60.0+10.0) <= t) o = 0.0;
						if(bsc.nBSAnswers[y*10+x])
						{
							glBindTexture(GL_TEXTURE_2D, skin[0]->skin[x%5]->GetCurrentTex());
							glBegin(GL_QUADS);
							glColor4d(1.0, 1.0, 1.0, o);
							glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
							glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
							glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
							glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
							glEnd();
						}
						// Segunda asignación
						o = 1.0-o;
						glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, o);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
						glEnd();
						double ll = l+m_nSizeArrows/4;
						double rr = r-m_nSizeArrows/4;
						glBindTexture(GL_TEXTURE_2D, bs->skin[1]->texture[17]);
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, o);
						glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0-OffsetInZArrow(x));
						glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0-OffsetInZArrow(x));
						glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0-OffsetInZArrow(x));
						glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0-OffsetInZArrow(x));
						glEnd();
					}
					else
					{
						glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, o);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0-OffsetInZArrow(x));
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0-OffsetInZArrow(x));
						glColor4d(1.0, 1.0, 1.0, ob);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0-OffsetInZArrow(x));
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0-OffsetInZArrow(x));
						glEnd();
						double ll = l+(dwGArrow == 1?(m_nSizeArrows/4):0);
						double rr = r-(dwGArrow == 1?(m_nSizeArrows/4):0);
						glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[dwDArrow]);
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, o);
						glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0-OffsetInZArrow(x));
						glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0-OffsetInZArrow(x));
						glColor4d(1.0, 1.0, 1.0, ob);
						glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0-OffsetInZArrow(x));
						glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0-OffsetInZArrow(x));
						glEnd();
					}
				}
			}
		}
	}
	// Fin de Dibujo y reconocimiento de Brain shower
	for(int i = 0; i < nLimitX; i++)
	{
		int x = nOrder[i];
		t = 0;
		l = DIST_ARROW(double(x))-(m_nSizeArrows/2*(skin[0]->dScale-1.0));
		b = t+(m_bDrop?1:-1)*m_nSizeArrows+(m_bDrop?1:-1)*(m_nSizeArrows*(skin[0]->dScale-1.0));
		r = l+m_nSizeArrows+(m_nSizeArrows*(skin[0]->dScale-1.0));
		SetOffset(&l, &t, &r, &b);
		__int64 nCaret = _abs64(m_nNext-evarrows.nArrows[x]);
		if(nCaret <= 500)
		{
			double dCaret = (double)nCaret/500;
			glBindTexture(GL_TEXTURE_2D, skin[0]->skin[x%5+25]->GetCurrentTex());
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0-dCaret);
			glTexCoord2d(0.0, 1.0); glVertex3d(l-dCaret*1, t+(m_bDrop?-1:1)*dCaret, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(r+dCaret*1, t+(m_bDrop?-1:1)*dCaret, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(r+dCaret*1, b-(m_bDrop?-1:1)*dCaret, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(l-dCaret*1, b-(m_bDrop?-1:1)*dCaret, -50.0);
			glEnd();
		}
		nCaret = _abs64(m_nNext-evarrows.nGlowArrows[x]);
		if(nCaret <= 500)
		{
			double dCaret = (double)nCaret/500;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, skin[0]->skin[x%5]->GetCurrentTex());
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0-dCaret);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);
			glEnd();
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBindTexture(GL_TEXTURE_2D, skin[0]->skin[x%5+30]->GetCurrentTex());
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0-dCaret);
			glTexCoord2d(0.0, 1.0); glVertex3d(l-m_nSizeArrows-dCaret*m_nSizeArrows/2, t+(m_bDrop?-1:1)*(m_nSizeArrows+dCaret*m_nSizeArrows/2), -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(r+m_nSizeArrows+dCaret*m_nSizeArrows/2, t+(m_bDrop?-1:1)*(m_nSizeArrows+dCaret*m_nSizeArrows/2), -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(r+m_nSizeArrows+dCaret*m_nSizeArrows/2, b-(m_bDrop?-1:1)*(m_nSizeArrows+dCaret*m_nSizeArrows/2), -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(l-m_nSizeArrows-dCaret*m_nSizeArrows/2, b-(m_bDrop?-1:1)*(m_nSizeArrows+dCaret*m_nSizeArrows/2), -50.0);
			glEnd();
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	glPushMatrix();
	if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(0.5, 0.0, 0.0);
	else
	{
		if(m_bIsSingle && m_nPlayer == 1)
		{
			glTranslated(-21.0, 0.0, 0.0);
		}
		if(m_bIsSingle && m_nPlayer == 2)
		{
			glTranslated(22.0, 0.0, 0.0);
		}
	}
	if(m_bNXMode) glTranslated(0.0, 15.0, 0.0);
	glScaled(0.6, 0.6, 1.0);
	__int64 nCaret = _abs64(m_nNext-status.nLastEva);
	bool bMissCombo = status.nTypeEva<=2;
	int nShowCombo = bMissCombo?(status.nMissComboTotal):(status.nComboTotal);
	if(status.nTypeEva>0 && status.nTypeEva<=5)
	{
		if(nCaret >= 0 && nCaret <= 100)
		{
			double dApt = double(nCaret) / 100;
			l = -20.0;
			r = 20.0;
			t = 2.5;
			b = -2.5;
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glScaled(1.0+0.3*(1.0-dApt), 1.0+0.3*(1.0-dApt), 1.0);
			glBindTexture(GL_TEXTURE_2D, common->skin[status.nTypeEva+15]->GetCurrentTex());
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
			glEnd();
			if(nShowCombo >= 4)
			{
				l = -7.6;
				r = 7.6;
				t = -2.5;
				b = -4.5;
				glBindTexture(GL_TEXTURE_2D, common->skin[33+(bMissCombo?1:0)]->GetCurrentTex());
				glBegin(GL_QUADS);
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
				glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
				glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
				glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
				glEnd();
				CString strCombo;
				strCombo.Format(_T("%d"), nShowCombo);
				int nDisplay = strCombo.GetLength();
				for(int x = nDisplay+1; x <= 3; x++)
				{
					strCombo = _T("0")+strCombo;
				}
				nDisplay = nDisplay<=3?3:nDisplay;
				double dLength = 9.0;
				double dl = -4.5*nDisplay;
				t = -4.5;
				b = -9.5;
				for(int e = 0; e < nDisplay; e++)
				{
					l = dl+dLength*(e);
					r = dl+dLength*(e+1);
					int nNum;
					if(e >= strCombo.GetLength()) nNum= 0;
					else nNum= strCombo[e]-'0';
					glBindTexture(GL_TEXTURE_2D, common->skin[21+nNum]->GetCurrentTex());
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
					glEnd();
				}
			}
		}
		if(nCaret > 100 && nCaret <= 500)
		{
			//double dCaret = ((double)nCaret-200)/300;
			l = -20.0;
			r = 20.0;
			t = 2.5;
			b = -2.5;
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, common->skin[status.nTypeEva+15]->GetCurrentTex());
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
			glEnd();
			if(nShowCombo >= 4)
			{
				l = -7.6;
				r = 7.6;
				t = -2.5;
				b = -4.5;
				glBindTexture(GL_TEXTURE_2D, common->skin[33+(bMissCombo?1:0)]->GetCurrentTex());
				glBegin(GL_QUADS);
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
				glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
				glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
				glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
				glEnd();
				CString strCombo;
				strCombo.Format(_T("%d"), nShowCombo);
				int nDisplay = strCombo.GetLength();
				for(int x = nDisplay+1; x <= 3; x++)
				{
					strCombo = _T("0")+strCombo;
				}
				nDisplay = nDisplay<=3?3:nDisplay;
				double dLength = 9.0;
				double dl = -4.5*nDisplay;
				t = -4.5;
				b = -9.5;
				for(int e = 0; e < nDisplay; e++)
				{
					l = dl+dLength*(e);
					r = dl+dLength*(e+1);
					int nNum;
					if(e >= strCombo.GetLength()) nNum= 0;
					else nNum= strCombo[e]-'0';
					glBindTexture(GL_TEXTURE_2D, common->skin[21+nNum]->GetCurrentTex());
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
					glEnd();
				}
			}
		}
		if(nCaret > 500 && nCaret <= 1000)
		{
			double dCaret = ((double)nCaret-500)/500;
			l = -20.0-(5.0*dCaret);
			r = 20.0+(5.0*dCaret);
			t = 2.5-(1.25*dCaret);
			b = -2.5+(1.25*dCaret);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_LPHA);
			glBindTexture(GL_TEXTURE_2D, common->skin[status.nTypeEva+15]->GetCurrentTex());
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0-dCaret);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
			glEnd();
			if(nShowCombo >= 4)
			{
				l = -7.6;
				r = 7.6;
				t = -2.5;
				b = -4.5;
				glBindTexture(GL_TEXTURE_2D, common->skin[33+(bMissCombo?1:0)]->GetCurrentTex());
				glBegin(GL_QUADS);
				glColor4d(1.0, 1.0, 1.0, 1.0-dCaret);
				glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
				glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
				glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
				glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
				glEnd();
				CString strCombo;
				strCombo.Format(_T("%d"), nShowCombo);
				int nDisplay = strCombo.GetLength();
				for(int x = nDisplay+1; x <= 3; x++)
				{
					strCombo = _T("0")+strCombo;
				}
				nDisplay = nDisplay<=3?3:nDisplay;
				double dLength = 9.0;
				double dl = -4.5*nDisplay;
				t = -4.5;
				b = -9.5;
				for(int e = 0; e < nDisplay; e++)
				{
					l = dl+dLength*(e);
					r = dl+dLength*(e+1);
					int nNum;
					if(e >= strCombo.GetLength()) nNum= 0;
					else nNum= strCombo[e]-'0';
					glBindTexture(GL_TEXTURE_2D, common->skin[21+nNum]->GetCurrentTex());
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0-dCaret);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
					glEnd();
				}
			}
		}
	}
	glPopMatrix();
	glPushMatrix();
	if(m_bIsSingle && m_nPlayer == 2)
	{
		glScaled(-1.0, 1.0, 1.0);
	}
	if(status.dLife > MAX_BAR) status.dLife = MAX_BAR;
	if(status.dLife < 0.0) status.dLife = 0.0;
	double dLongBar = 33.0;
	double dMulti = status.dLife+pow((1-dBeat+(double)((int)dBeat)), double(5))/10-0.1;
	if(dBeat < 0) dMulti = 1.0;
	if(dMulti > 1.0 || status.dLife >= 1.0) dMulti = 1.0;
	if(dMulti < 0) dMulti = 0.0;
	t = 28.5;
	l = -39.0;
	b = t-3.0;
	r = l+dLongBar*dMulti;
	glBindTexture(GL_TEXTURE_2D, common->skin[32]->GetCurrentTex());
	glBegin(GL_QUADS);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
	glTexCoord2d(dMulti, 1.0); glVertex3d(r, t, -50);
	glTexCoord2d(dMulti, 0.0); glVertex3d(r, b, -50);
	glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
	glEnd();
	double dApt = 0.25+double(dwTick%80)/160;
	if(status.dLife >= 0.04 && status.dLife <= 0.96)
	{
		t = 28.5;
		l = -39.0+dLongBar*status.dLife-0.5;
		b = t-3.0;
		r = -39.0+dLongBar*status.dLife+0.5;
		glBindTexture(GL_TEXTURE_2D, 0);
		glBegin(GL_QUADS);
		glColor4d(0.6, 0.6, 0.0, dApt);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
		glTexCoord2d(dMulti, 1.0); glVertex3d(r, t, -50);
		glTexCoord2d(dMulti, 0.0); glVertex3d(r, b, -50);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
		glEnd();
	}
	if(status.dLife <= 0.2)
	{
		t = 28.5;
		l = -39.0;
		b = t-3.0;
		r = l+dLongBar;
		glBindTexture(GL_TEXTURE_2D, 0);
		glBegin(GL_QUADS);
		glColor4d(1.0, 0.0, 0.0, dApt);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
		glTexCoord2d(dMulti, 1.0); glVertex3d(r, t, -50);
		glTexCoord2d(dMulti, 0.0); glVertex3d(r, b, -50);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
		glEnd();
	}
	if(status.dLife >= 1.0)
	{
		t = 28.5;
		l = -39.0;
		b = t-3.0;
		r = l+dLongBar;
		glBindTexture(GL_TEXTURE_2D, 0);
		glBegin(GL_QUADS);
		glColor4d(1.0, 1.0, 1.0, dApt);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
		glTexCoord2d(dMulti, 1.0); glVertex3d(r, t, -50);
		glTexCoord2d(dMulti, 0.0); glVertex3d(r, b, -50);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
		glEnd();
	}
	t = 29.0;
	l = -39.0;
	b = t-4.0;
	r = l+dLongBar;
	glBindTexture(GL_TEXTURE_2D, common->skin[31]->GetCurrentTex());
	glBegin(GL_QUADS);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
	glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
	glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
	glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
	glEnd();
	// Dibujo de las preguntas de BS
	glPopMatrix();
	glPushMatrix();
	int nPQuest = -1;
	for(int y = 0; y < bsc.nSizeBSQuestions; y++)
	{
		double dE = bsc.dWEBSQuestions[y];
		if((y+1) < bsc.nSizeBSQuestions)
		{
			if((dE + 8*dActualBPM/600) <= bsc.dWIBSQuestions[y+1])
			{
				dE += 8*dActualBPM/600;
			}
			//else dE = (dE+bsc.dWIBSQuestions[y+1])/2;
		}
		else dE += 8*dActualBPM/600;

		double dI = bsc.dWIBSQuestions[y];
		if(y-1 >= 0)
		{
			if((dI-8*dActualBPM/600) >= bsc.dWEBSQuestions[y-1])
			{
				dI -= 8*dActualBPM/600;
			}
			//else dI = (dI+bsc.dWIBSQuestions[y-1])/2;
		}
		else dI -= 8*dActualBPM/600;

		if(dE > dBeat && dBeat>dI)
		{
			nPQuest = y;
		}
	}
	if(nPQuest != -1)
	{
		DWORD dwSubject = HIWORD(bsc.dwBSQuestions[nPQuest]);
		DWORD dwType = LOWORD(bsc.dwBSQuestions[nPQuest]);
		double dBSBPM = dActualBPM;
		switch(dwSubject)
		{
		case 0:
			if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(21.5, 0.0, 0.0);
			switch(dwType)
			{
			case 0:
				DrawBS03(nPQuest);
				break;
			case 1:
				DrawBS04(nPQuest);
				break;
			case 2:
				DrawBS05(nPQuest);
				break;
			case 3:
				if(m_nPlayer == 2 && m_bIsSingle && !piuconfig.nModeMach) glTranslated(2*m_nSizeArrows/3, 0.0, 0.0);
				DrawBS07(nPQuest);
				break;
			}
			break;
		case 1:
			if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(21.5, 0.0, 0.0);
			switch(dwType)
			{
			case 0:
				DrawBS03(nPQuest);
				break;
			case 1:
				DrawBS03(nPQuest);
				break;
			case 2:
				DrawBS02(nPQuest);
				break;
			case 3:
				DrawBS06(nPQuest);
				break;
			case 4:
				DrawBS06(nPQuest);
				break;
			}
			break;
		case 2:
			switch(dwType)
			{
			case 0:
				DrawBS01(nPQuest);
				break;
			case 1:
				DrawBS01(nPQuest);
				break;
			case 2:
				DrawBS01(nPQuest);
				break;
			case 3:
				DrawBS02(nPQuest);
				break;
			case 4:
				DrawBS01(nPQuest);
				break;
			}
			break;
		case 3:
			if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(21.5, 0.0, 0.0);
			switch(dwType)
			{
			case 0:
				DrawBS08(nPQuest);
				break;
			case 1:
				DrawBS06(nPQuest);
				break;
			case 2:
				DrawBS03(nPQuest);
				break;
			case 3:
				DrawBS08(nPQuest);
				break;
			case 4:
				if(m_nPlayer == 2 && m_bIsSingle && !piuconfig.nModeMach) glTranslated(2*m_nSizeArrows/3, 0.0, 0.0);
				DrawBS07(nPQuest);
				break;
			}
			break;
		}
	}
	glPopMatrix();
	//////////////////////////////////
	// Dibujo PopUp BS
	for(int y = 0; y < bsc.nSizeBSPopUp; y++)
	{
		t = 15.0; b = -15.0; l = -35.0; r = 35.0;
		__int64 nWPopUp = BeatToTime(bsc.dWBSPopUp[y]);
		__int64 nOff = m_nNext-nWPopUp;
		if(nOff>=-200 && nOff<0)
		{
			double dCaret = abs((double)nOff/200);
			glBindTexture(GL_TEXTURE_2D, bs->skin[11]->GetCurrentTex());
			glColor4d(1.0,1.0,1.0,1-dCaret);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(l-5*dCaret, t+5*dCaret, -50);
			glTexCoord2d(1.0, 1.0); glVertex3d(r+5*dCaret, t+5*dCaret, -50);
			glTexCoord2d(1.0, 0.0); glVertex3d(r+5*dCaret, b-5*dCaret, -50);
			glTexCoord2d(0.0, 0.0); glVertex3d(l-5*dCaret, b-5*dCaret, -50);
			glEnd();
		}
		if(nOff>=0 && nOff<3000)
		{
			glBindTexture(GL_TEXTURE_2D, bs->skin[11]->GetCurrentTex());
			glColor4d(1.0,1.0,1.0,1.0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
			glEnd();
			int nRef = min((int)bsc.dwBSPopUp[y], 49);
			bsf->glDrawText(t, l, b, r, 0.0, 0.0, -50.0, 2.0, 5.0, strBSDisp[nRef], DT_VCENTER|DT_CENTER);
		}
		if(nOff>=3000 && nOff<3200)
		{
			double dCaret = abs((double)(nOff-3000)/200);
			glBindTexture(GL_TEXTURE_2D, bs->skin[11]->GetCurrentTex());
			glColor4d(1.0,1.0,1.0,1.0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(l+5*dCaret, t-5*dCaret, -50);
			glTexCoord2d(1.0, 1.0); glVertex3d(r-5*dCaret, t-5*dCaret, -50);
			glTexCoord2d(1.0, 0.0); glVertex3d(r-5*dCaret, b+5*dCaret, -50);
			glTexCoord2d(0.0, 0.0); glVertex3d(l+5*dCaret, b+5*dCaret, -50);
			glEnd();
			glColor4d(1.0,1.0,1.0,1-dCaret);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(l-5*dCaret, t+5*dCaret, -50);
			glTexCoord2d(1.0, 1.0); glVertex3d(r+5*dCaret, t+5*dCaret, -50);
			glTexCoord2d(1.0, 0.0); glVertex3d(r+5*dCaret, b-5*dCaret, -50);
			glTexCoord2d(0.0, 0.0); glVertex3d(l-5*dCaret, b-5*dCaret, -50);
			glEnd();
		}
		if(nOff>=3200 && nOff<3400)
		{
			double dCaret = abs((double)(nOff-3200)/200);
			glBindTexture(GL_TEXTURE_2D, bs->skin[11]->GetCurrentTex());
			glColor4d(1.0,1.0,1.0,1-dCaret);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(l+5-10*dCaret, t-5+10*dCaret, -50);
			glTexCoord2d(1.0, 1.0); glVertex3d(r-5+10*dCaret, t-5+10*dCaret, -50);
			glTexCoord2d(1.0, 0.0); glVertex3d(r-5+10*dCaret, b+5-10*dCaret, -50);
			glTexCoord2d(0.0, 0.0); glVertex3d(l+5-10*dCaret, b+5-10*dCaret, -50);
			glEnd();
		}
	}
	//////////////////
	// Dibujo eva BS
	glPushMatrix();
	if(m_bIsSingle && piuconfig.nModeMach && !g_bPlayer[int(!(m_nPlayer-1))]) glTranslated(0.0, 0.0, 0.0);
	else
	{
		if(m_bIsSingle && m_nPlayer == 1) glTranslated(-20.0, 0.0, 0.0);
		if(m_bIsSingle && m_nPlayer == 2) glTranslated(20.0, 0.0, 0.0);
	}
	nCaret = _abs64(m_nNext-status.nLastBSEva);
	if(nCaret < 500 && status.nTypeBSEva)
	{
		t = 15.0; b = 5.0; l = -10.0; r = 10.0;
		double dCaret = (double)nCaret/500;
		if(status.nTypeBSEva == 1) glBindTexture(GL_TEXTURE_2D, bs->skin[10]->GetCurrentTex());
		else glBindTexture(GL_TEXTURE_2D, bs->skin[9]->GetCurrentTex());
		glBegin(GL_QUADS);
		glColor4d(1.0, 1.0, 1.0, 1.0-dCaret);
		glTexCoord2d(0.0, 1.0); glVertex3d(l-5*dCaret, t+5*dCaret, -50);
		glTexCoord2d(1.0, 1.0); glVertex3d(r+5*dCaret, t+5*dCaret, -50);
		glTexCoord2d(1.0, 0.0); glVertex3d(r+5*dCaret, b-5*dCaret, -50);
		glTexCoord2d(0.0, 0.0); glVertex3d(l-5*dCaret, b-5*dCaret, -50);
		glEnd();
	}
	glPopMatrix();
	// Dibujo del hidden
	if(evarrows.bHidden && m_nNext >= evarrows.nHidden && m_nNext <= (evarrows.nHidden+500))
	{
		t = 30.0; b = -30.0; l = -40.0; r = 40.0;
		double dCaret = (double)_abs64(m_nNext-evarrows.nHidden)/500;
		glBindTexture(GL_TEXTURE_2D, m_uNullTex);
		glBegin(GL_QUADS);
		glColor4d(1.0, 1.0, 1.0, 1.0-dCaret);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50);
		glEnd();
	}
	if(m_bAutoPlay)
	{
		if(m_bCharacter)
		{
			for(int s = nHlp; s < crt.hCtr.nSizeArrowsAP; s++)
			{
				if(crt.dWWArrowsAP[s] <= dBeatnStop)
				{
					int nArr = LOWORD(crt.dwArrowsAP[s]);
					int nType = HIWORD(crt.dwArrowsAP[s]);
					if(nType == 4)
					{
						evarrows.bArrows[nArr] = false;
					}
					m_nHlpAuto = s+1;
				}
				else break;
			}
		}
		else
		{
			for(int s = nHlp; s < songc.nSizeArrows1; s++)
			{
				if(songc.dWWArrows1[s] <= dBeatnStop)
				{
					for(int t = 0; t < 10; t++)
					{
						if(LOWORD(songc.dwArrows1[s*10+t]) == 4)
						{
							evarrows.bArrows[t] = false;
						}
					}
					m_nHlpAuto = s+1;
				}
				else break;
			}
		}
	}
	glColor4d(1.0, 1.0, 1.0, 1.0);
}

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

void CSKSFView::MinusCombo(void)
{
	if(status.nScore < 0) status.nScore = 0;
}

void CSKSFView::PlusCombo(void)
{
	status.nScore += fcombo(min(status.nComboTotal/50, max((m_nLevelSong-5)/2, 0)));
}

int CSKSFView::LoadTextures(void)
{
	if(!m_bTrainingMode)
	{
		if(m_bRandomSkin)
		{
			/*if(songc.nSizeArrows1) */skin[0]->LoadSkin(rand()%MAX_SKINS);
			/*if(songc.nSizeArrows2) */skin[1]->LoadSkin(rand()%MAX_SKINS);
			/*if(songc.nSizeArrows3) */skin[2]->LoadSkin(rand()%MAX_SKINS);
		}
		else
		{
			if(songc.nSizeArrows1)
			{
				if(m_nSkin != -1) skin[0]->LoadSkin(m_nSkin);
				else skin[0]->LoadSkin(GetDocument()->song.hStep.nSkin1);
			}
			skin[1]->LoadSkin(GetDocument()->song.hStep.nSkin2);
			skin[2]->LoadSkin(GetDocument()->song.hStep.nSkin3);
		}
	}
	else
	{
		skin[0]->LoadSkin(3);
		skin[1]->LoadSkin(1);
		skin[2]->LoadSkin(2);
	}
	common->LoadSkin();
	bs->LoadSkin();
	bsf->LoadFont(_T("skins/BS/BSFont.png"), 16, 16);

	for(int n = 0; n < 3; n++)for(int x = 0; x < 5; x++) for(int y = 0; y < skin[n]->skin[x+5]->m_nSize; y++)
	{
		glBindTexture(GL_TEXTURE_2D, skin[n]->skin[x+5]->texture[y]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	return 1;
}
double CSKSFView::TimeToBeat(__int64 dwMiliseconds, int* nStop, double* dModStop, double* dActualBPM)
{
	(*nStop) = -1;	// Asumir que no ha caido en ningun stop
	(*dModStop) = 0.0;
	if(dwMiliseconds < 0)
	{
		(*nStop) = -1;
		(*dModStop) = 0.0;
		(*dActualBPM) = songc.dChangeBPM[0];
		return ((double)dwMiliseconds)*songc.dChangeBPM[0]/60000;
	}
	double dBeat = 0.0;
	__int64 nMiliseconds = dwMiliseconds;
	int x;
	int yj=0,ys=0;
	for(x = 0; x < songc.nSizeChangeBPM; x++)
	{
		yj=0;
		double dSBeat = dBeat+double(nMiliseconds)*songc.dChangeBPM[x]/60000;
		for(int y = ys; y < songc.nSizeStops; y++)	// Stops
		{
			double dStBeat = songc.dWStops[y];
			for(int z = yj; z < songc.nSizeJumps; z++)	// Jumps
			{
				double dIjBeat = songc.dIJumps[z];
				double dEjBeat = songc.dEJumps[z];
				if(dIjBeat < dSBeat && dIjBeat >= dBeat && dIjBeat < dStBeat)
				{
					dSBeat += dEjBeat - dIjBeat;
					yj = z+1;
				}
			}
			if(dStBeat <= dSBeat && (dStBeat+songc.dStops[y]) >= dSBeat)
			{
				(*nStop) = y;
				(*dModStop) = dSBeat - dStBeat;
				dSBeat -= dSBeat - dStBeat;
			}
			else if((dStBeat+songc.dStops[y]) < dSBeat && dStBeat >= dBeat)
			{
				dSBeat -= songc.dStops[y];
			}
		}
		for(int y = yj; y < songc.nSizeJumps; y++)	// Jumps
		{
			double dIjBeat = songc.dIJumps[y];
			double dEjBeat = songc.dEJumps[y];
			if(dIjBeat < dSBeat && dIjBeat >= dBeat)
			{
				dSBeat += dEjBeat - dIjBeat;
				//yj = y+1;
			}
		}
		if((x+1) >= songc.nSizeChangeBPM)
		{
			(*dActualBPM) = songc.dChangeBPM[x];
			return dSBeat; // No existen mas cambios de BPM
		}
		else
		{
			double dBeatMil = (songc.dWChangeBPM[x+1]-songc.dWChangeBPM[x])+dBeat;
			if(dBeatMil > dSBeat)
			{
				// El beat actual no ha pasado por este cambio de BPM
				(*dActualBPM) = songc.dChangeBPM[x];
				return dSBeat;
			}
			else
			{
				dBeat += songc.dWChangeBPM[x+1]-songc.dWChangeBPM[x];
				nMiliseconds -= (__int64)((songc.dWChangeBPM[x+1]-songc.dWChangeBPM[x])/songc.dChangeBPM[x]*60000);
				for(int y = 0; y < songc.nSizeStops; y++)
				{
					if(songc.dWStops[y] >= songc.dWChangeBPM[x] && // WARNING
						songc.dWStops[y] < songc.dWChangeBPM[x+1])
					{
						nMiliseconds -= (__int64)(songc.dStops[y]/songc.dChangeBPM[x]*60000);
						ys = y+1;
					}
				}
				for(int y = 0; y < songc.nSizeJumps; y++)	// Jumps
				{
					if(songc.dIJumps[y] >= songc.dWChangeBPM[x] && // WARNING
						songc.dIJumps[y] < songc.dWChangeBPM[x+1])
					{
						double dIjBeat = songc.dIJumps[y];
						double dEjBeat = songc.dEJumps[y];
						nMiliseconds += (__int64)((dEjBeat - dIjBeat)/songc.dChangeBPM[x]*60000);
					}
				}
				(*dModStop) = 0.0;
				(*nStop) = -1;	// No ha caido en ningun stop porque paso por aqui
			}
		}
	}
	(*dActualBPM) = songc.dChangeBPM[0];
	return dBeat;
}

void CSKSFView::GetLimitsOfDrawing(double dBeat, int nGArrow, int* nMin, int* nMax)
{
	(*nMin) = 0;
	(*nMax) = 0;
	double dFactorVel = (m_dUserVel*m_dMachVel);
	double dMinClip = (dFactorVel>1.0?-5.0:-(5.0/dFactorVel))+dBeat;
	double dMaxClip = 100.0/dFactorVel+dBeat;
	if(nGArrow == 0)
	{
		for(int x = 0; x < songc.nSizeArrows1; x++)
		{
			(*nMax) = x+1;
			if(dMaxClip < songc.dWArrows1[x])
			{
				break;
			}
		}
		for(int x = songc.nSizeArrows1-1; x >= 0; x--)
		{
			(*nMin) = x;
			if(dMinClip > songc.dWArrows1[x])
			{
				break;
			}
		}
	}
	if(nGArrow == 1)
	{
		for(int x = 0; x < songc.nSizeArrows2; x++)
		{
			(*nMax) = x+1;
			if(dMaxClip <= songc.dWArrows2[x])
			{
				break;
			}
		}
		for(int x = songc.nSizeArrows2-1; x >= 0; x--)
		{
			(*nMin) = x;
			if(dMinClip >= songc.dWArrows2[x])
			{
				break;
			}
		}
	}
	if(nGArrow == 2)
	{
		for(int x = 0; x < songc.nSizeArrows3; x++)
		{
			(*nMax) = x+1;
			if(dMaxClip < songc.dWArrows3[x])
			{
				break;
			}
		}
		for(int x = songc.nSizeArrows3-1; x >= 0; x--)
		{
			(*nMin) = x;
			if(dMinClip > songc.dWArrows3[x])
			{
				break;
			}
		}
	}
}

int CSKSFView::GetAccessSkin(int x)
{
	int ox = x%5;
	return ox;
}

void CSKSFView::ModifyPosArrows(double* l, double* t)
{
	// ** Modificar si AC o DC está activado
	if(m_bDC && (*t) > 0)
	{
		double dt = (*t);
		dt = abs(dt);
		dt = pow(dt, 2)/10;
		(*t) = dt;
	}
	if(m_bAC && (*t) > 0)
	{
		double dt = (*t);
		dt = abs(dt);
		dt = sqrt(dt*10);
		(*t) = dt;
	}
	if(m_bSnake && (*t) > 0)
	{
		double dt = (*t);
		double dSin = sin(dt*M_PI/2);
		(*l) += dSin*2;
	}
	if(!m_bDrop)
		(*t) = -(*t);
	// ** Modificador actual de velocidad
	(*t) *= m_dMachVel*m_dUserVel*m_dPassVel;
}

void CSKSFView::ActualizateVel(double dBeat)
{
	for(int x = 0; x < songc.nSizeChangeVel; x++)
	{
		if(dBeat > songc.dIChangeVel[x])
		{
			if(dBeat <= songc.dEChangeVel[x])
			{
				// Aqui significa que el beat en proceso esta entre un cambio de velocidad
				double dBackMachVel;
				if((x-1) < 0) dBackMachVel = songc.dChangeVel[0];
				else dBackMachVel = songc.dChangeVel[x-1];
				m_dMachVel = dBackMachVel+
					(songc.dChangeVel[x]-dBackMachVel)*
					((dBeat-songc.dIChangeVel[x])/(songc.dEChangeVel[x]-songc.dIChangeVel[x]));
				break;
			}
			else if((x+1) >= songc.nSizeChangeVel)
			{
				// Aqui es que ya no hay mas cambios de velocidad
				m_dMachVel = songc.dChangeVel[x];
				break;
			}
			else if(songc.dIChangeVel[x+1] > dBeat)
			{
				// Aqui es que el beat actual no es mayor que donde se encuentra el proximo cambio
				m_dMachVel = songc.dChangeVel[x];
				break;
			}
			else
			{
				// Aqui significa que el beat actual es mayor que donde se encuentra el
				// proximo cambio, entonces será asumir la velocidad actual y continuar con el calculo
				m_dMachVel = songc.dChangeVel[x];
			}
		}
	}
}

void CSKSFView::DeriveOpacityPos(double t, double* o, int nGArrows)
{
	(*o) = 1.0;
	double dClipMin;
	double dClipMax;
	switch(nGArrows)
	{
	case 0:
		dClipMin = m_dClipMin1;
		dClipMax = m_dClipMax1;
		break;
	case 1:
		dClipMin = m_dClipMin2;
		dClipMax = m_dClipMax2;
		break;
	case 2:
		dClipMin = m_dClipMin3;
		dClipMax = m_dClipMax3;
		break;
	default:
		dClipMin = m_dClipMin1;
		dClipMax = m_dClipMax1;
		break;
	}
	if((dClipMax*60.0) > t && (dClipMax*60.0-10.0) < t)
		(*o) = 1-abs(dClipMax*60.0-t)/10;
	else if((dClipMax*60.0-10.0) >= t) (*o) = 0.0;
	if((dClipMin*60.0) < t && (dClipMin*60.0+10.0) > t)
		(*o) = 1-abs(dClipMin*60.0-t)/10;
	else if((dClipMin*60.0+10.0) <= t) (*o) = 0.0;
	if(m_bFlash)
	{
		int nPart = (int)dBeatnStop;
		double ko = abs(dBeatnStop-(double)nPart-0.5)*2;
		(*o) = min((*o), ko);
	}
}

void CSKSFView::SetOffset(double* l, double* t, double* r, double* b)
{
	(*l) -= 4*m_nSizeArrows*0.95+m_nSizeArrows;
	(*r) -= 4*m_nSizeArrows*0.95+m_nSizeArrows;
	if(m_bIsSingle)
	{
		(*l) -= (40.0-4*m_nSizeArrows*0.95+m_nSizeArrows)/4;
		(*r) -= (40.0-4*m_nSizeArrows*0.95+m_nSizeArrows)/4;
	}
	if(!m_bDrop)
	{
		(*t)+=/*3*m_nSizeArrows+2.0*/24.0;
		(*b)+=/*3*m_nSizeArrows+2.0*/24.0;
	}
	else
	{
		(*t)-=/*3*m_nSizeArrows-2.0*/24.0;
		(*b)-=/*3*m_nSizeArrows-2.0*/24.0;
	}
}

int CSKSFView::GetArrowStatus(int x, int y, int nGArrows)
{
	if(nGArrows == 0)
	{
		if(y >= songc.nSizeArrows1) return 0;
		return status.nArrowsStatus1[y*10+x];
	}
	if(nGArrows == 1)
	{
		if(y >= songc.nSizeArrows2) return 0;
		return status.nArrowsStatus2[y*10+x];
	}
	if(nGArrows == 2)
	{
		if(y >= songc.nSizeArrows3) return 0;
		return status.nArrowsStatus3[y*10+x];
	}
	return 0;
}

void CSKSFView::SetArrowStatus(int x, int y, int nGArrows, int nValue)
{
	if(nGArrows == 0 && y < songc.nSizeArrows1) status.nArrowsStatus1[y*10+x] = nValue;
	if(nGArrows == 1 && y < songc.nSizeArrows2) status.nArrowsStatus2[y*10+x] = nValue;
	if(nGArrows == 2 && y < songc.nSizeArrows3) status.nArrowsStatus3[y*10+x] = nValue;
}

int CSKSFView::CompileSong(void)
{
	for(int n = 0; n < 3; n++)
	{
		int nComp = GetDocument()->song.hStep.nSizeArrows1;
		switch(n)
		{
		case 0:
			nComp = GetDocument()->song.hStep.nSizeArrows1;
			break;
		case 1:
			nComp = GetDocument()->song.hStep.nSizeArrows2;
			break;
		case 2:
			nComp = GetDocument()->song.hStep.nSizeArrows3;
			break;
		}
		double dWOffset = 0.0;
		int nAssign[10];
		for(int x = 0; x < 10; x++) nAssign[x] = x;
		if(m_bMirror)
		{
			nAssign[0] = 1;
			nAssign[1] = 0;
			nAssign[2] = 2;
			nAssign[3] = 4;
			nAssign[4] = 3;
			nAssign[5] = 6;
			nAssign[6] = 5;
			nAssign[7] = 7;
			nAssign[8] = 9;
			nAssign[9] = 8;
		}
		for(int y = 0; y < nComp; y++)
		{
			int nStopVer = GetDocument()->VerificateStopChange(y);
			int nJumpVer = GetDocument()->VerificateJumpChangeEq(y);
			if(GetDocument()->SomeNote(y, n))
			{
				int a = songc.nSizeArrows1;
				switch(n)
				{
				case 0:
					a = songc.nSizeArrows1;
					songc.nSizeArrows1++;
					songc.dwArrows1=(DWORD*)realloc(songc.dwArrows1, songc.nSizeArrows1*10*sizeof(DWORD));
					songc.dWArrows1=(double*)realloc(songc.dWArrows1, songc.nSizeArrows1*sizeof(double));
					songc.dWWArrows1=(double*)realloc(songc.dWWArrows1, songc.nSizeArrows1*sizeof(double));
					for(int z = 0; z < 10; z++) songc.dwArrows1[(songc.nSizeArrows1-1)*10+z] = 0;
					break;
				case 1:
					a = songc.nSizeArrows2;
					songc.nSizeArrows2++;
					songc.dwArrows2=(DWORD*)realloc(songc.dwArrows2, songc.nSizeArrows2*10*sizeof(DWORD));
					songc.dWArrows2=(double*)realloc(songc.dWArrows2, songc.nSizeArrows2*sizeof(double));
					songc.dWWArrows2=(double*)realloc(songc.dWWArrows2, songc.nSizeArrows2*sizeof(double));
					for(int z = 0; z < 10; z++) songc.dwArrows2[(songc.nSizeArrows2-1)*10+z] = 0;
					break;
				case 2:
					a = songc.nSizeArrows3;
					songc.nSizeArrows3++;
					songc.dwArrows3=(DWORD*)realloc(songc.dwArrows3, songc.nSizeArrows3*10*sizeof(DWORD));
					songc.dWArrows3=(double*)realloc(songc.dWArrows3, songc.nSizeArrows3*sizeof(double));
					songc.dWWArrows3=(double*)realloc(songc.dWWArrows3, songc.nSizeArrows3*sizeof(double));
					for(int z = 0; z < 10; z++) songc.dwArrows3[(songc.nSizeArrows3-1)*10+z] = 0;
					break;
				}
				double dFactor = 1.0;
				if(nStopVer != -1)
				{
					dFactor = m_bGetArrowInStop? 0.0:1.0;
				}
				if(m_bRS)
				{
					if(m_bIsSingle)
					{
						int nDem = 5;
						for(int r = 0; r < 5; r++)
						{
							if((LOWORD(GetDocument()->GetArrow(r, y, n)) == 2
								&& LOWORD(GetDocument()->GetArrow(r, y-1, n)) != 2)
								|| LOWORD(GetDocument()->GetArrow(r, y, n)) != 2)
								nAssign[r] = -1;
							//else nDem--;
						}
						for(int r = 0; r < nDem; r++) if(nAssign[r] == -1)
						{
							bool bOpt = false;
							int nRand = 0;
							do
							{
								nRand = rand()%5;
								bOpt = false;
								for(int t = 0; t < 10; t++)
									if(nAssign[t] == nRand) bOpt = true;
							} while (bOpt);
							nAssign[r] = nRand;
						}
						nAssign[5] = 5;
						nAssign[6] = 6;
						nAssign[7] = 7;
						nAssign[8] = 8;
						nAssign[9] = 9;
					}
					else
					{
						int nDem = 10;
						for(int r = 0; r < 10; r++)
						{
							if((LOWORD(GetDocument()->GetArrow(r, y, n)) == 2
								&& LOWORD(GetDocument()->GetArrow(r, y-1, n)) != 2)
								|| LOWORD(GetDocument()->GetArrow(r, y, n)) != 2)
								nAssign[r] = -1;
							//else nDem--;
						}
						for(int r = 0; r < nDem; r++) if(nAssign[r] == -1)
						{
							bool bOpt = false;
							int nRand = 0;
							do
							{
								nRand = rand()%10;
								bOpt = false;
								for(int t = 0; t < 10; t++)
									if(nAssign[t] == nRand) bOpt = true;
							} while (bOpt);
							nAssign[r] = nRand;
						}
					}
				}
				for(int i = 0; i < (m_bIsSingle?5:10); i++)
				{
					int x = nAssign[i];
					DWORD dwArrow = GetDocument()->GetArrow(i, y, n);
					DWORD dwAdd = dwArrow;
					if(LOWORD(dwArrow)==2 && LOWORD(GetDocument()->GetArrow(i, y-1, n))!=2)
						dwAdd=(DWORD)MAKELONG(3, HIWORD(dwArrow));
					else if(LOWORD(dwArrow)==2 && LOWORD(GetDocument()->GetArrow(i, y+1, n))!=2)
						dwAdd=(DWORD)MAKELONG(4, HIWORD(dwArrow));
					else if(LOWORD(dwArrow)>2&&LOWORD(dwArrow)<=18) dwAdd=(DWORD)MAKELONG(LOWORD(dwArrow)+2,HIWORD(dwArrow));
					else if(LOWORD(dwArrow)>18) dwAdd = 0;
					switch(n)
					{
					case 0:
						songc.dwArrows1[a*10+x] = dwAdd;
						songc.dWArrows1[a] = GetDocument()->BeatArrow(y);
						songc.dWWArrows1[a] = GetDocument()->BeatArrow(y)+dFactor*(nStopVer!=-1?GetDocument()->song.dStops[nStopVer]:0.0)+dWOffset;
						break;
					case 1:
						songc.dwArrows2[a*10+x] = dwAdd;
						songc.dWArrows2[a] = GetDocument()->BeatArrow(y);
						songc.dWWArrows2[a] = GetDocument()->BeatArrow(y)+dFactor*(nStopVer!=-1?GetDocument()->song.dStops[nStopVer]:0.0)+dWOffset;
						break;
					case 2:
						songc.dwArrows3[a*10+x] = dwAdd;
						songc.dWArrows3[a] = GetDocument()->BeatArrow(y);
						songc.dWWArrows3[a] = GetDocument()->BeatArrow(y)+dFactor*(nStopVer!=-1?GetDocument()->song.dStops[nStopVer]:0.0)+dWOffset;
						break;
					}
					if(LOWORD(dwArrow) == 2 && LOWORD(dwAdd) != 4)
					{
						// ** Found back TC
						int i = 0;
						int q = 0;
						for(i = (GetDocument()->song.hStep.nSizeTickcount-1); i >= 0; i--)
						{
							if(GetDocument()->song.nWTickCount[i] <= y)
							{
								q = i;
								break;
							}
						}
						int nTickCount = GetDocument()->song.nTickCount[q];
						for(int j = 1; j < nTickCount; j++)
						{
							switch(n)
							{
							case 0:
								if((a+j) >= songc.nSizeArrows1)
								{
									songc.nSizeArrows1=a+j+1;
									songc.dwArrows1=(DWORD*)realloc(songc.dwArrows1, songc.nSizeArrows1*10*sizeof(DWORD));
									songc.dWArrows1=(double*)realloc(songc.dWArrows1, songc.nSizeArrows1*sizeof(double));
									songc.dWWArrows1=(double*)realloc(songc.dWWArrows1, songc.nSizeArrows1*sizeof(double));
									for(int z = 0; z < 10; z++) songc.dwArrows1[(songc.nSizeArrows1-1)*10+z] = 0;
								}
								songc.dwArrows1[(a+j)*10+x] = (DWORD)MAKELONG(2,HIWORD(dwAdd));
								songc.dWArrows1[a+j] = GetDocument()->BeatArrow(y)
									+(GetDocument()->BeatArrow(y+1)-GetDocument()->BeatArrow(y))/((double)nTickCount)*((double)j);
								songc.dWWArrows1[a+j] = GetDocument()->BeatArrow(y)+dWOffset
									+((nStopVer!=-1&&GetDocument()->song.hStep.bGetArrowInStop)?(GetDocument()->song.dStops[nStopVer]):(GetDocument()->BeatArrow(y+1)-GetDocument()->BeatArrow(y)))/((double)nTickCount)*((double)j);
								break;
							case 1:
								if((a+j) >= songc.nSizeArrows2)
								{
									songc.nSizeArrows2=a+j+1;
									songc.dwArrows2=(DWORD*)realloc(songc.dwArrows2, songc.nSizeArrows2*10*sizeof(DWORD));
									songc.dWArrows2=(double*)realloc(songc.dWArrows2, songc.nSizeArrows2*sizeof(double));
									songc.dWWArrows2=(double*)realloc(songc.dWWArrows2, songc.nSizeArrows2*sizeof(double));
									for(int z = 0; z < 10; z++) songc.dwArrows2[(songc.nSizeArrows2-1)*10+z] = 0;
								}
								songc.dwArrows2[(a+j)*10+x] = (DWORD)MAKELONG(2,HIWORD(dwAdd));
								songc.dWArrows2[a+j] = GetDocument()->BeatArrow(y)
									+(GetDocument()->BeatArrow(y+1)-GetDocument()->BeatArrow(y))/((double)nTickCount)*((double)j);
								songc.dWWArrows2[a+j] = GetDocument()->BeatArrow(y)+dWOffset
									+((nStopVer!=-1&&GetDocument()->song.hStep.bGetArrowInStop)?(GetDocument()->song.dStops[nStopVer]):(GetDocument()->BeatArrow(y+1)-GetDocument()->BeatArrow(y)))/((double)nTickCount)*((double)j);
								break;
							case 2:
								if((a+j) >= songc.nSizeArrows3)
								{
									songc.nSizeArrows3=a+j+1;
									songc.dwArrows3=(DWORD*)realloc(songc.dwArrows3, songc.nSizeArrows3*10*sizeof(DWORD));
									songc.dWArrows3=(double*)realloc(songc.dWArrows3, songc.nSizeArrows3*sizeof(double));
									songc.dWWArrows3=(double*)realloc(songc.dWWArrows3, songc.nSizeArrows3*sizeof(double));
									for(int z = 0; z < 10; z++) songc.dwArrows3[(songc.nSizeArrows3-1)*10+z] = 0;
								}
								songc.dwArrows3[(a+j)*10+x] = (DWORD)MAKELONG(2,HIWORD(dwAdd));
								songc.dWArrows3[a+j] = GetDocument()->BeatArrow(y)
									+(GetDocument()->BeatArrow(y+1)-GetDocument()->BeatArrow(y))/((double)nTickCount)*((double)j);
								songc.dWWArrows3[a+j] = GetDocument()->BeatArrow(y)+dWOffset
									+((nStopVer!=-1&&GetDocument()->song.hStep.bGetArrowInStop)?(GetDocument()->song.dStops[nStopVer]):(GetDocument()->BeatArrow(y+1)-GetDocument()->BeatArrow(y)))/((double)nTickCount)*((double)j);
								break;
							}
						}
					}
				}
			}
			if(nStopVer != -1)
			{
				dWOffset += GetDocument()->song.dStops[nStopVer];
			}
			if(nJumpVer != -1)
			{
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[nJumpVer])
					-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[nJumpVer]);
			}
		}
	}
	songc.nSizeChangeBPM = GetDocument()->song.hStep.nSizeChangeBPM;
	songc.nSizeChangeVel = GetDocument()->song.hStep.nSizeChangeVel;
	songc.nSizeStops = GetDocument()->song.hStep.nSizeStops;
	songc.nSizeJumps = GetDocument()->song.hStep.nSizeJumps;
	songc.dWChangeBPM=(double*)realloc(songc.dWChangeBPM, songc.nSizeChangeBPM*sizeof(double));
	songc.dChangeBPM=(double*)realloc(songc.dChangeBPM, songc.nSizeChangeBPM*sizeof(double));
	songc.dIChangeVel=(double*)realloc(songc.dIChangeVel, songc.nSizeChangeVel*sizeof(double));
	songc.dEChangeVel=(double*)realloc(songc.dEChangeVel, songc.nSizeChangeVel*sizeof(double));
	songc.dChangeVel=(double*)realloc(songc.dChangeVel, songc.nSizeChangeVel*sizeof(double));
	songc.dIJumps=(double*)realloc(songc.dIJumps, songc.nSizeJumps*sizeof(double));
	songc.dEJumps=(double*)realloc(songc.dEJumps, songc.nSizeJumps*sizeof(double));
	songc.dWStops=(double*)realloc(songc.dWStops, songc.nSizeStops*sizeof(double));
	songc.dStops=(double*)realloc(songc.dStops, songc.nSizeStops*sizeof(double));
	for(int x = 0; x < songc.nSizeChangeBPM; x++)
	{
		songc.dWChangeBPM[x]=GetDocument()->BeatArrow(GetDocument()->song.nWChangeBPM[x]);
		songc.dChangeBPM[x]=GetDocument()->song.dChangeBPM[x];
	}
	for(int x = 0; x < songc.nSizeStops; x++)
	{
		songc.dWStops[x]=GetDocument()->BeatArrow(GetDocument()->song.nWStops[x]);
		songc.dStops[x]=GetDocument()->song.dStops[x];
	}
	for(int x = 0; x < songc.nSizeChangeVel; x++)
	{
		songc.dIChangeVel[x]=GetDocument()->BeatArrow(GetDocument()->song.nIChangeVel[x]);
		songc.dEChangeVel[x]=GetDocument()->BeatArrow(GetDocument()->song.nEChangeVel[x]);
		songc.dChangeVel[x]=GetDocument()->song.dChanges[x];
	}
	for(int x = 0; x < songc.nSizeJumps; x++)
	{
		songc.dIJumps[x]=GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
		songc.dEJumps[x]=GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x]);
	}

	return 1;
}

int CSKSFView::CompileBS(void)
{
	// Compilador de Brain shower
	for(int y = 0; y < GetDocument()->song.hStep.nSizeArrows1; y++)
	{
		if(GetDocument()->SomeBSQuestion(y, 0))
		{
			// Extraer la información
			bsc.nSizeBSQuestions++;
			bsc.dWIBSQuestions = (double*)realloc(bsc.dWIBSQuestions, bsc.nSizeBSQuestions*sizeof(double));
			bsc.dWEBSQuestions = (double*)realloc(bsc.dWEBSQuestions, bsc.nSizeBSQuestions*sizeof(double));
			bsc.dWABSQuestions = (double*)realloc(bsc.dWABSQuestions, bsc.nSizeBSQuestions*sizeof(double));
			bsc.dwBSQuestions = (DWORD*)realloc(bsc.dwBSQuestions, bsc.nSizeBSQuestions*sizeof(DWORD));
			bsc.dwDBSQuestions = (DWORD*)realloc(bsc.dwDBSQuestions, bsc.nSizeBSQuestions*30*sizeof(DWORD));
			bsc.dwDBSQuestionsEx = (DWORD*)realloc(bsc.dwDBSQuestionsEx, bsc.nSizeBSQuestions*40*sizeof(DWORD));
			bsc.dWIBSQuestions[bsc.nSizeBSQuestions-1] = 0.0;
			bsc.dWEBSQuestions[bsc.nSizeBSQuestions-1] = 0.0;
			bsc.dWABSQuestions[bsc.nSizeBSQuestions-1] = 0.0;
			bsc.dwBSQuestions[bsc.nSizeBSQuestions-1] = 0;
			for(int x = 0; x < 30; x++) bsc.dwDBSQuestions[(bsc.nSizeBSQuestions-1)*30+x] = 0;
			for(int x = 0; x < 40; x++) bsc.dwDBSQuestionsEx[(bsc.nSizeBSQuestions-1)*40+x] = 0;

			DWORD dwInfo = 0x0;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, y, 0))==19)
				{
					dwInfo = HIWORD(GetDocument()->GetArrow(x, y, 0));
					break;
				}
			}
			int nSubject = dwInfo&0xF;
			int nType = (dwInfo>>4)&0xF;
			int nArg1 = (dwInfo>>8)&0xF;
			int nArg2 = (dwInfo>>12)&0xF;
			// Averiguar el numero de respuestas y procesar
			int nResp = 0;
			switch(nSubject)
			{
			case 0:	// Memoria
				switch(nType)
				{
					case 0: nResp = nArg2+1;
						GQMemo1(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 1: nResp = 5;	// Las 4 respuestas + 1
						GQMemo2(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 2: nResp = 6;	// Las 5 Respuestas + 1
						GQMemo3(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 3: nResp = nArg1*2+1; // Respuestas + 1
						GQMemo4(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 4: nResp = nArg1;
						GQMemo5(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
				}
				break;
			case 3:	// Analisys
				switch(nType)
				{
					case 0: nResp = 1;
						GQAnal1(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 1: nResp = 1;
						GQAnal2(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 2: nResp = 2;
						GQAnal3(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 3: nResp = 1;
						GQAnal4(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 4: nResp = nArg1*2+1;
						GQAnal5(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
				}
				break;
			case 1: // Observacion
				switch(nType)
				{
					case 0: nResp = 2; // La respuesta + el nivel de muestreo
						GQObse1(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 1: nResp = 2;
						GQObse2(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 2: nResp = 5;
						GQObse3(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 3: nResp = 1;
						GQObse4(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 4: nResp = 2;
						GQObse5(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
				}
				break;
			case 2:	// Matematicas
				switch(nType)
				{
					case 0: nResp = 1;
						GQMath1(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 1: nResp = 1;
						GQMath2(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 2: nResp = 1;
						GQMath3(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 3: nResp = 5;
						GQMath4(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
					case 4: nResp = 1;
						GQMath5(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
						break;
				}
				break;
			}
		}
		if(GetDocument()->SomeBSPopUp(y, 0))
		{
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, y, 0)) == 21)
				{
					bsc.nSizeBSPopUp++;
					bsc.dWBSPopUp = (double*)realloc(bsc.dWBSPopUp, bsc.nSizeBSPopUp*sizeof(double));
					bsc.dwBSPopUp = (DWORD*)realloc(bsc.dwBSPopUp, bsc.nSizeBSPopUp*sizeof(DWORD));
					int nIndex = bsc.nSizeBSPopUp-1;
					bsc.dWBSPopUp[nIndex] = GetDocument()->BeatArrow(y);
					bsc.dwBSPopUp[nIndex] = HIWORD(GetDocument()->GetArrow(x, y, 0));
				}
			}
		}
	}
	FILE* hFile;
	hFile = _fopen(_T("skins/BS/Messages.acc"), _T("r"));
	if(hFile == NULL)
	{
		for(int x = 0; x < 50; x++) strBSDisp[x] = _T("");
		return 0;
	}
	for(int x = 0; x < 50; x++)
	{
		TCHAR chMsg[500];
		_fgets(chMsg, 500, hFile);
		if(chMsg == NULL) strBSDisp[x] = _T("");
		else
		{
			strBSDisp[x] = ( chMsg);
			strBSDisp[x].Remove(_T('\n'));
			strBSDisp[x].Replace(_T('#'), _T('\n'));
		}
	}
	fclose(hFile);
	return 1;
}


void CSKSFView::GQMath1(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[1];
	int nArrayCoord[10];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	bool bRight = (rand()%2)?true:false;
	int nMode = rand()%3;	// 0 =, 1 <, 2 >
	if(nArg1 <= nArg2 || nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, Arg2 = %d, estos dos no pueden ser iguales, ni el segundo puede ser mayor que el primero, o el segundo ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg1, nArg2);
		_printf(str); return;
	}

	int nResult = 0;
	int nNumber = 0;
	int nOper = 0;	// 0 +, 1 -, 2 x, 3 /
	bool bCond = true;
	while(bCond)
	{
		nCarry = 0;
		nResult = 0;
		for(int a = 0; a < nArg2; a++)
		{
			if(a == 0)
			{
				nOper = 0;
				nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
			}
			else
			{
				int nOperPer[] = {0,1,2,3};
				if(nResult <= 0) nOperPer[3] = rand()%2;
				if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
				if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
				if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
				nOper = nOperPer[rand()%4];
				// Ahora el siguiente numero
				// tener cuidado, cuando es division
				// debe ser dividido por algun Dividendo del numero
				int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
				int nPer[20];
				for(int q = 0; q < 20; q++)
				{
					nPer[q] = q+1;
				}
				if(nOper == 3)	// Division, tener cuidado
				{
					// Encontrar los divisores del numero anterior
					int nCountDiv = 0;
					for(int q = 1; q <= nResult; q++)
					{
						int nDiv = nResult%q;
						if(nDiv == 0)	// Este numero es divisible
						{
							nPer[nCountDiv] = q;
							nCountDiv++;
						}
					}
					nSizePer = nCountDiv;
				}
				nNumber = nPer[rand()%nSizePer];
			}
			switch(nOper)
			{
			case 0: // Suma
				nResult += nNumber;
				break;
			case 1: // Resta
				nResult -= nNumber;
				break;
			case 2: // Multiplicacion
				nResult *= nNumber;
				break;
			case 3: // Division
				nResult /= nNumber;
				break;
			}
			if(a != 0)
			{
				bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
				nCarry++;
			}
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
			nCarry++;
		}
		bCond = nResult >= 20 || nResult < 0;
		if(nMode == 1) bCond = nResult >= 20 || nResult <= 0;
	}
	int nBackResult = nResult;
	nResult = 0;
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nMode+14, 1);
	nCarry++;
	int nArg3 = nArg1-nArg2;
	for(int a = 0; a < nArg3; a++)
	{
		if(a == 0)
		{
			nOper = 0;
			nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
		}
		else if(a != (nArg3-1))
		{
			int nOperPer[] = {0,1,2,3};
			if(nResult <= 0) nOperPer[3] = rand()%2;
			if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
			if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
			if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
			nOper = nOperPer[rand()%4];

			int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
			int nPer[20];
			for(int q = 0; q < 20; q++)
			{
				nPer[q] = q+1;
			}
			if(nOper == 3)	// Division, tener cuidado
			{
				// Encontrar los divisores del numero anterior
				int nCountDiv = 0;
				for(int q = 1; q <= nResult; q++)
				{
					int nDiv = nResult%q;
					if(nDiv == 0)	// Este numero es divisible
					{
						nPer[nCountDiv] = q;
						nCountDiv++;
					}
				}
				nSizePer = nCountDiv;
			}
			nNumber = nPer[rand()%nSizePer];
		}
		if(a == (nArg3-1) && bRight)
		{
			int nLeft = nBackResult - nResult;
			if(abs(nLeft) != nLeft) nOper = 1;
			else nOper = 0;
			if(nLeft == 0) nNumber = 0;
			else
			{
				if(nMode == 0)	// Igual
					nNumber = abs(nLeft);
				if(nMode == 1)	// Menor que
				{
					if(nOper == 0)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
				if(nMode == 2)	// Mayor que
				{
					if(nOper == 1)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
			}
		}
		if(a == (nArg3-1) && !bRight)
		{
			int nLeft = nBackResult - nResult;
			if(abs(nLeft) != nLeft) nOper = 1;
			else nOper = 0;
			if(nLeft == 0)
			{
				nNumber = rand()%38-19;
				if(nNumber < 0) nOper = 1;
				else nOper = 0;
			}
			else
			{
				if(nMode == 0)	// Igual
				{
					do
					{
						nNumber = rand()%19;
					}
					while(nNumber == abs(nLeft));
				}
				if(nMode == 2)	// Mayor que
				{
					if(nOper == 0)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
				if(nMode == 1)	// Menor que
				{
					if(nOper == 1)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
			}
		}
		switch(nOper)
		{
		case 0: // Suma
			nResult += nNumber;
			break;
		case 1: // Resta
			nResult -= nNumber;
			break;
		case 2: // Multiplicacion
			nResult *= nNumber;
			break;
		case 3: // Division
			nResult /= nNumber;
			break;
		}
		if(a != 0)
		{
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
			nCarry++;
		}
		bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
		nCarry++;
	}
	bsc.dWIBSQuestions[nIndex] = GetDocument()->BeatArrow(y);
	bsc.dwBSQuestions[nIndex] = (DWORD)MAKELONG(0, 2);
	// ** Esto es verdadero y falso, por lo tanto solo deben haber 2 respuestas maximo
	// Diseñar la respuesta
	nDis = 2;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
		}
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x]) nCaretSizeCoord++;
		}

		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		// Ahora... cual va a ser la verdadera?
		if(nDis != 0) nRight = rand()%(nDis);
		else nRight = 0;
		int nCarCount = 0;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x] == 1)
			{
				if(nRight == nCarCount)	// Aqui va la correcta
				{
					bsc.nBSAnswers[nInd*10+x] = 1;
					bsc.dwBSAnswers[nInd*10+x] = (DWORD)MAKELONG((int)(bRight), 8);
				}
				else			// Esto es un distractor
				{
					bsc.nBSAnswers[nInd*10+x] = 0;
					bsc.dwBSAnswers[nInd*10+x] = (DWORD)MAKELONG((int)(!bRight), 8);
				}
				nCarCount++;
			}
			if(nCarCount >= nDis) break;
		}
	}
}

void CSKSFView::GQMath2(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[1];
	int nArrayCoord[10];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg1 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, no puede ser igual a 0 (debe haber al menos un operando)\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg1);
		_printf(str); return;
	}

	int nResult = 0;
	int nNumber = 0;
	int nOper = 0;	// 0 +, 1 -, 2 x, 3 /
	bool bCond = true;
	while(bCond)
	{
		nCarry = 0;
		nResult = 0;
		for(int a = 0; a < nArg1; a++)
		{
			if(a == 0)
			{
				nOper = 0;
				nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
			}
			else
			{
				int nOperPer[] = {0,1,2,3};
				if(nResult <= 0) nOperPer[3] = rand()%2;
				if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
				if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
				if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
				nOper = nOperPer[rand()%4];
				// Ahora el siguiente numero
				// tener cuidado, cuando es division
				// debe ser dividido por algun Dividendo del numero
				int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
				int nPer[20];
				for(int q = 0; q < 20; q++)
				{
					nPer[q] = q+1;
				}
				if(nOper == 3)	// Division, tener cuidado
				{
					// Encontrar los divisores del numero anterior
					int nCountDiv = 0;
					for(int q = 1; q <= nResult; q++)
					{
						int nDiv = nResult%q;
						if(nDiv == 0)	// Este numero es divisible
						{
							nPer[nCountDiv] = q;
							nCountDiv++;
						}
					}
					nSizePer = nCountDiv;
				}
				nNumber = nPer[rand()%nSizePer];
			}
			switch(nOper)
			{
			case 0: // Suma
				nResult += nNumber;
				break;
			case 1: // Resta
				nResult -= nNumber;
				break;
			case 2: // Multiplicacion
				nResult *= nNumber;
				break;
			case 3: // Division
				nResult /= nNumber;
				break;
			}
			if(a != 0)
			{
				bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
				nCarry++;
			}
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
			nCarry++;
		}
		bCond = nResult >= nArg2 || nResult < 0;
	}
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(14, 1);
	nCarry++;
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(17, 1);
	nCarry++;

	bsc.dWIBSQuestions[nIndex] = GetDocument()->BeatArrow(y);
	bsc.dwBSQuestions[nIndex] = (DWORD)MAKELONG(1, 2);

	// ** Esto es encontrar la respuesta, por lo tanto solo deben haber 5 respuestas maximo
	// Diseñar la respuesta
	nDis = 5;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 1;	// Show numbers
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
		}
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x]) nCaretSizeCoord++;
		}

		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		// Ahora... cual va a ser la verdadera?
		if(nDis != 0) nRight = rand()%(nDis);
		else nRight = 0;
		int nCarCount = 0;
		int nCarryDis = 0;
		int nDistrac[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x] == 1)
			{
				if(nRight == nCarCount)	// Aqui va la correcta
				{
					bsc.nBSAnswers[nInd*10+x] = 1;
					bsc.dwBSAnswers[nInd*10+x] = (DWORD)MAKELONG(nResult, 1);
				}
				else			// Esto es un distractor
				{
					bsc.nBSAnswers[nInd*10+x] = 0;
					nNumber = rand()%max(10, nArg2);
					for(int t = 0; t < 10; t++)
					{
						if(nNumber == nDistrac[t] || nNumber == nResult)
						{
							t = 0;
							nNumber = rand()%max(10, nArg2);
						}
					}
					nDistrac[nCarryDis] = nNumber;
					nCarryDis++;
					bsc.dwBSAnswers[nInd*10+x] = (DWORD)MAKELONG(nNumber, 1);
				}
				nCarCount++;
			}
			if(nCarCount >= nDis) break;
		}
	}
}

void CSKSFView::GQMath3(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[1];
	int nArrayCoord[10];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg1 <= 1)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, no puede ser menor que 1 (debe haber al menos un operando para juzgar)\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg1);
		_printf(str); return;
	}

	int nResult = 0;
	int nNumber = 0;
	int nOper = 0;	// 0 +, 1 -, 2 x, 3 /
	int nOperSur = 0;
	int nOperDir = 0;
	if(nArg1 > 1)
		nOperSur = rand()%(nArg1-1);
	bool bCond = true;
	while(bCond)
	{
		nCarry = 0;
		nResult = 0;
		for(int a = 0; a < nArg1; a++)
		{
			if(a == 0)
			{
				nOper = 0;
				nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
			}
			else
			{
				int nOperPer[] = {0,1,2,3};
				if(nResult <= 0) nOperPer[3] = rand()%2;
				if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
				if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
				if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
				nOper = nOperPer[rand()%4];
				// Ahora el siguiente numero
				// tener cuidado, cuando es division
				// debe ser dividido por algun Dividendo del numero
				int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
				int nPer[20];
				for(int q = 0; q < 20; q++)
				{
					nPer[q] = q+1;
				}
				if(nOper == 3)	// Division, tener cuidado
				{
					// Encontrar los divisores del numero anterior
					int nCountDiv = 0;
					for(int q = 1; q <= nResult; q++)
					{
						int nDiv = nResult%q;
						if(nDiv == 0)	// Este numero es divisible
						{
							nPer[nCountDiv] = q;
							nCountDiv++;
						}
					}
					nSizePer = nCountDiv;
				}
				nNumber = nPer[rand()%nSizePer];
			}
			switch(nOper)
			{
			case 0: // Suma
				nResult += nNumber;
				break;
			case 1: // Resta
				nResult -= nNumber;
				break;
			case 2: // Multiplicacion
				nResult *= nNumber;
				break;
			case 3: // Division
				nResult /= nNumber;
				break;
			}
			if(a != 0)
			{
				if((a-1)==nOperSur)
				{
					bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(17, 1);
					nOperDir = nOper;
				}
				else
					bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
				nCarry++;
			}
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
			nCarry++;
		}
		bCond = nResult >= nArg2 || nResult < 0;
	}
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(14, 1);
	nCarry++;
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nResult, 1);
	nCarry++;

	bsc.dWIBSQuestions[nIndex] = GetDocument()->BeatArrow(y);
	bsc.dwBSQuestions[nIndex] = (DWORD)MAKELONG(2, 2);

	// ** Esto es encontrar el operando perdido, por lo tanto solo deben haber 4 respuestas maximo
	// Diseñar la respuesta
	nDis = 4;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;	// Show Symbols & numbers
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
		}
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x]) nCaretSizeCoord++;
		}

		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		// Ahora... cual va a ser la verdadera?
		if(nDis != 0) nRight = rand()%(nDis);
		else nRight = 0;
		int nCarCount = 0;
		int nOpers[] = {0, 1, 2, 3};
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x] == 1)
			{
				if(nRight == nCarCount)	// Aqui va la correcta
				{
					bsc.nBSAnswers[nInd*10+x] = 1;
					bsc.dwBSAnswers[nInd*10+x] = (DWORD)MAKELONG(nOperDir+10, 1);
					nOpers[nOperDir] = -1;
				}
				else			// Esto es un distractor
				{
					bsc.nBSAnswers[nInd*10+x] = 0;
					do
					{
						nOper = nOpers[rand()%4];
					}
					while (nOper == -1 || nOper == nOperDir);
					nOpers[nOper] = -1;
					bsc.dwBSAnswers[nInd*10+x] = (DWORD)MAKELONG(nOper+10, 1);
				}
				nCarCount++;
			}
			if(nCarCount >= nDis) break;
		}
	}
}

void CSKSFView::GQMath4(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[5];
	int nArrayCoord[50];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg2);
		_printf(str); return;
	}
	int nSum = min(4, nArg2);
	nSum = max(1, nSum);
	int nMin = 0;
	int nMax = 20;
	int nResults[] = {-1,-1,-1,-1,-1};
	if(nArg1)
	{
		int nCar = 0;
		for(int x = 0; x < 5; x++)
		{
			nResults[x] = (rand()%(17+x-nCar)) + nCar;
			nCar = nResults[x]+1;
		}
	}
	else
	{
		int nCar = 20;
		for(int x = 0; x < 5; x++)
		{
			nResults[x] = nCar - (rand()%(nCar-3+x));
			nCar = nResults[x]-1;
		}
	}
	int nCaretOpt[] = {0, 1, 2, 3, 4};
	int nCaretOrd[] = {0,0,0,0,0};
	for(int x = 0; x < 5; x++)
	{
		int nSumand[] = {0,0,0,0};
		int nSizeSum = (rand()%nSum);
		int nResult = 0;
		int nIndResp;
		do
		{
			nIndResp = nCaretOpt[rand()%5];
		} while(nIndResp == -1);
		nCaretOpt[nIndResp] = -1;
		nCaretOrd[x] = nIndResp;
		for(int z = 0; z <= nSizeSum; z++)
		{
			if(z != nSizeSum) nSumand[z] = (rand()%(nResults[x]-nResult+1));
			else nSumand[z] = nResults[x]-nResult;
			nResult+=nSumand[z];
			bsc.dwDBSQuestionsEx[nIndex*40+nIndResp*4+z] = MAKELONG(nSumand[z], 1);
		}
		bsc.dwDBSQuestionsEx[nIndex*40+20+x] = (rand()%3)+1;
	}
	bsc.dWIBSQuestions[nIndex] = GetDocument()->BeatArrow(y);
	bsc.dwBSQuestions[nIndex] = (DWORD)MAKELONG(3, 2);
	// ** Esto es juzgar terminos, por lo tanto solo deben haber 3 respuestas maximo
	// Diseñar la respuesta
	nDis = 5;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
		}
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = nFirst/5;
		nFirst *= 5;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		nDisAr[0] = nCaretOrd[i];
		bsc.nBSAnswers[nInd*10+nCaretOrd[i]+nFirst] = 1;
		bsc.dwBSAnswers[nInd*10+nCaretOrd[i]+nFirst] = MAKELONG(17, 1);
		bsc.dwDBSQuestionsEx[nIndex*40+25+nCaretOrd[i]] = (DWORD)BeatToTime(bsc.dWBSAnswers[nInd]);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%5;
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = MAKELONG(17, 1);
		}
	}
}

void CSKSFView::GQMath5(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[1];
	int nArrayCoord[10];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	int nMode = rand()%3;	// 0 =, 1 <, 2 >
	if(nArg1 <= nArg2 || nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, Arg2 = %d, estos dos no pueden ser iguales, ni el segundo puede ser mayor que el primero, o el segundo ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg1, nArg2);
		_printf(str); return;
	}

	int nResult = 0;
	int nNumber = 0;
	int nOper = 0;	// 0 +, 1 -, 2 x, 3 /
	bool bCond = true;
	while(bCond)
	{
		nCarry = 0;
		nResult = 0;
		for(int a = 0; a < nArg2; a++)
		{
			if(a == 0)
			{
				nOper = 0;
				nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
			}
			else
			{
				int nOperPer[] = {0,1,2,3};
				if(nResult <= 0) nOperPer[3] = rand()%2;
				if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
				if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
				if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
				nOper = nOperPer[rand()%4];
				// Ahora el siguiente numero
				// tener cuidado, cuando es division
				// debe ser dividido por algun Dividendo del numero
				int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
				int nPer[20];
				for(int q = 0; q < 20; q++)
				{
					nPer[q] = q+1;
				}
				if(nOper == 3)	// Division, tener cuidado
				{
					// Encontrar los divisores del numero anterior
					int nCountDiv = 0;
					for(int q = 1; q <= nResult; q++)
					{
						int nDiv = nResult%q;
						if(nDiv == 0)	// Este numero es divisible
						{
							nPer[nCountDiv] = q;
							nCountDiv++;
						}
					}
					nSizePer = nCountDiv;
				}
				nNumber = nPer[rand()%nSizePer];
			}
			switch(nOper)
			{
			case 0: // Suma
				nResult += nNumber;
				break;
			case 1: // Resta
				nResult -= nNumber;
				break;
			case 2: // Multiplicacion
				nResult *= nNumber;
				break;
			case 3: // Division
				nResult /= nNumber;
				break;
			}
			if(a != 0)
			{
				bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
				nCarry++;
			}
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
			nCarry++;
		}
		bCond = nResult >= 20 || nResult < 0;
		if(nMode == 1) bCond = nResult >= 20 || nResult <= 0;
	}
	int nBackResult = nResult;
	nResult = 0;
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(17, 1);
	nCarry++;
	int nArg3 = nArg1-nArg2;
	for(int a = 0; a < nArg3; a++)
	{
		if(a == 0)
		{
			nOper = 0;
			nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
		}
		else if(a != (nArg3-1))
		{
			int nOperPer[] = {0,1,2,3};
			if(nResult <= 0) nOperPer[3] = rand()%2;
			if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
			if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
			if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
			nOper = nOperPer[rand()%4];

			int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
			int nPer[20];
			for(int q = 0; q < 20; q++)
			{
				nPer[q] = q+1;
			}
			if(nOper == 3)	// Division, tener cuidado
			{
				// Encontrar los divisores del numero anterior
				int nCountDiv = 0;
				for(int q = 1; q <= nNumber; q++)
				{
					int nDiv = nNumber%q;
					if(nDiv == 0)	// Este numero es divisible
					{
						nPer[nCountDiv] = q;
						nCountDiv++;
					}
				}
				nSizePer = nCountDiv;
			}
			nNumber = nPer[rand()%nSizePer];
		}
		if(a == (nArg3-1))
		{
			int nLeft = nBackResult - nResult;
			if(abs(nLeft) != nLeft) nOper = 1;
			else nOper = 0;
			if(nLeft == 0) nNumber = 0;
			else
			{
				if(nMode == 0)	// Igual
					nNumber = abs(nLeft);
				if(nMode == 1)	// Menor que
				{
					if(nOper == 0)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
				if(nMode == 2)	// Mayor que
				{
					if(nOper == 1)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
			}
		}
		switch(nOper)
		{
		case 0: // Suma
			nResult += nNumber;
			break;
		case 1: // Resta
			nResult -= nNumber;
			break;
		case 2: // Multiplicacion
			nResult *= nNumber;
			break;
		case 3: // Division
			nResult /= nNumber;
			break;
		}
		if(a != 0)
		{
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
			nCarry++;
		}
		bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
		nCarry++;
	}
	bsc.dWIBSQuestions[nIndex] = GetDocument()->BeatArrow(y);
	bsc.dwBSQuestions[nIndex] = (DWORD)MAKELONG(4, 2);
	// ** Esto es juzgar terminos, por lo tanto solo deben haber 3 respuestas maximo
	// Diseñar la respuesta
	nDis = 3;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
		}
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x]) nCaretSizeCoord++;
		}

		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		// Ahora... cual va a ser la verdadera?
		if(nDis != 0) nRight = rand()%(nDis);
		else nRight = 0;
		int nCarCount = 0;
		int nModes[] = {0,1,2};
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x] == 1)
			{
				if(nRight == nCarCount)	// Aqui va la correcta
				{
					bsc.nBSAnswers[nInd*10+x] = 1;
					bsc.dwBSAnswers[nInd*10+x] = (DWORD)MAKELONG(nMode+14, 1);
					nModes[nMode] = -1;
				}
				else			// Esto es un distractor
				{
					bsc.nBSAnswers[nInd*10+x] = 0;
					int nCMode;
					do
					{
						nCMode = nModes[rand()%3];
					}
					while(nCMode == -1 || nMode == nCMode);
					nModes[nCMode] = -1;
					bsc.dwBSAnswers[nInd*10+x] = (DWORD)MAKELONG(nCMode+14, 1);
				}
				nCarCount++;
			}
			if(nCarCount >= nDis) break;
		}
	}
}

void CSKSFView::GQMemo1(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg2);
		_printf(str); return;
	}
	DWORD dwOrdAnsw[80];
	int nDist=nArg1/10;
	int nMode=nArg1-nDist*10;
	int nSize=nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	for(int x = 0; x < nSize; x++)
	{
		int kMode;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 10;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		nNum = rand()%nLimit;
		dwOrdAnsw[x*5]=MAKELONG(nNum, nType);
		for(int z = 1; z < 5; z++)
		{
			if(nMode <= 5) kMode = nMode;
			else
			{
				if(nMode == 6)
				{
					int nPosib[] = {0, 1, 3, 5};
					kMode = nPosib[rand()%4];
				}
				else
				{
					int nPosib[] = {0, 2, 4, 5};
					kMode = nPosib[rand()%4];
				}
			}
			switch(kMode)
			{
			case 0:nType = 1;nLimit = 10;break;
			case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
			case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
			case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
			case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
			case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
			}
			bool bCond = false;
			do
			{
				nNum = rand()%nLimit;
				DWORD dwAdd = MAKELONG(nNum, nType);
				bCond = false;
				for(int w = 0; w < 5; w++)
				{
					if(dwOrdAnsw[x*5+w] == dwAdd)
					{
						bCond = true;
					}
				}
			} while(bCond);
			dwOrdAnsw[x*5+z]=MAKELONG(nNum, nType);
		}
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(0,0);	// Tipo de pregunta: Memoria 1
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40]=MAKELONG(nDist+1,0);	// Distribución (1 o 2 veces lo normal)
	bsc.dwDBSQuestionsEx[nIndex*40+1]=MAKELONG(nSize,0);	// Tamaño (para ahorrar procesador)
	for(int x = 0; x < nSize; x++)
	{
		bsc.dwDBSQuestions[nIndex*30+x] = dwOrdAnsw[x*5];
	}
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = 5;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 1;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(10-nFirst);
			bID = false;
			for(int t = 0; t < 5; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*10+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*10+nFirst+nDisAr[0]] = dwOrdAnsw[(i-1)*5];
		bsc.nDBSAnswers[nInd] = 1;	// Mostrar numeros como tal
		for(int x = 1; x < nDis; x++)
		{
			do
			{
				nDisp = rand()%(10-nFirst);
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = dwOrdAnsw[(i-1)*5+x];
			bsc.nDBSAnswers[nInd] = 1;	// Mostrar numeros como tal
		}
	}
}

void CSKSFView::GQMemo2(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	DWORD dwCaret[] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
	int nPos1[] = {-1,-1,-1,-1,-1};
	int nPos2[] = {-1,-1,-1,-1,-1};
	int nOrd[] = {-1,-1,-1,-1,-1};
	int nMode=nArg1;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	for(int x = 0; x < 5; x++)
	{
		DWORD dwObj;
		int nsPos1;
		int nsPos2;
		int kMode;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 10;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwObj=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < 5; w++)
			{
				if(dwCaret[w] == dwObj)
				{
					bCond = true;
				}
			}
		} while(bCond);
		dwCaret[x] = dwObj;
		do
		{
			nsPos1 = rand()%5;
			bCond = false;
			for(int w = 0; w < 5; w++)
			{
				if(nPos1[w] == nsPos1)
				{
					bCond = true;
				}
			}
		} while(bCond);
		nPos1[x] = nsPos1;
		if(x != 4)
		{
			do
			{
				nsPos2 = rand()%4;
				bCond = false;
				for(int w = 0; w < 5; w++)
				{
					if(nPos2[w] == nsPos2)
					{
						bCond = true;
					}
				}
			} while(bCond);
		}
		else
		{
			nsPos2 = 5;
		}
		nPos2[x] = nsPos2;
	}
	bsc.dwBSQuestions[nIndex]=MAKELONG(1,0);	// Tipo de pregunta: Memoria 2
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nCount==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nCount==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);

	int nOtherPos[5];
	for(int x = 0; x < 5; x++)
	{
		switch(nPos1[x])
		{
		case 0: nOtherPos[x]=6; break;
		case 1: nOtherPos[x]=0; break;
		case 2: nOtherPos[x]=4; break;
		case 3: nOtherPos[x]=2; break;
		case 4: nOtherPos[x]=8; break;
		default: nOtherPos[x]=0; break;
		}
		// Item Display (Answers)
		bsc.dwDBSQuestionsEx[nIndex*40+nOtherPos[x]] = dwCaret[x];
		// Item Display (Questions)
		bsc.dwDBSQuestionsEx[nIndex*40+(nPos2[x]*2+1)] = dwCaret[x];
	}
	// Diseñar las respuestas y las preguntas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = 5;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = nFirst/5;
		nFirst *= 5;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		nDisAr[0] = nPos1[i-1];
		bsc.nBSAnswers[nInd*10+nPos1[i-1]+nFirst] = 1;
		bsc.dwBSAnswers[nInd*10+nPos1[i-1]+nFirst] = MAKELONG(17, 1);
		// Appear Time (Answers)
		bsc.dwDBSQuestionsEx[nIndex*40+10+nOtherPos[i-1]] = (DWORD)BeatToTime(bsc.dWBSAnswers[nInd]);
		// Dissapear Time (Answers)
		bsc.dwDBSQuestionsEx[nIndex*40+20+nOtherPos[i-1]] = (DWORD)(BeatToTime(bsc.dWBSAnswers[nInd])+500);
		// Appear Time (Questions)
		bsc.dwDBSQuestionsEx[nIndex*40+10+(nPos2[i-1]*2+1)] = (DWORD)BeatToTime(GetDocument()->BeatArrow(nCoord[i-1]));
		// Dissapear Time (Questions)
		bsc.dwDBSQuestionsEx[nIndex*40+20+(nPos2[i-1]*2+1)] = (DWORD)(BeatToTime(bsc.dWBSAnswers[nInd])+500);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%5;
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = MAKELONG(17, 1);
		}
	}
}

void CSKSFView::GQMemo3(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	DWORD dwOrdAnsw[] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
	int nRespAr[] = {-1,-1,-1,-1,-1};
	int nMode=nArg1;
	int nSize=5;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	for(int x = 0; x < nSize; x++)
	{
		int kMode;
		DWORD dwAdd;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 10;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < 5; w++)
			{
				if(dwOrdAnsw[w] == dwAdd)
				{
					bCond = true;
				}
			}
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%5;
			bID = false;
			for(int t = 0; t < 5; t++)
				if(nDisp == nRespAr[t])
					bID = true;
		} while (bID);
		nRespAr[x] = nDisp;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(2,0);	// Tipo de pregunta: Memoria 1
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	for(int x = 0; x < nSize; x++)
	{
		bsc.dwDBSQuestions[nIndex*30+x] = dwOrdAnsw[x];
		bsc.dwDBSQuestionsEx[nIndex*40+nRespAr[x]] = dwOrdAnsw[x];
	}
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = 5;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = nFirst/5;
		nFirst *= 5;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		nDisAr[0] = nRespAr[i-1];
		bsc.nBSAnswers[nInd*10+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*10+nFirst+nDisAr[0]] = MAKELONG(17,1);
		bsc.dwDBSQuestionsEx[nIndex*40+nDisAr[0]+5] = (DWORD)BeatToTime(bsc.dWBSAnswers[nInd]);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%5;
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = MAKELONG(17,1);
		}
	}
}

void CSKSFView::GQMemo4(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[36];
	int nArrayCoord[360];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg1 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg1);
		_printf(str); return;
	}
	int nSize = min(nArg1, nCount);
	int RespAr[15];
	for(int x = 0; x < nSize; x++)
	{
		do
		{
			RespAr[x] = rand()%10;
		} while (nArrayCoord[x*10+RespAr[x]] == 0);
		bsc.dwDBSQuestions[nIndex*30+x] = MAKELONG(RespAr[x],4);
		bsc.dwDBSQuestionsEx[nIndex*40+x] = (DWORD)BeatToTime(GetDocument()->BeatArrow(nCoord[x]));
	}

	for(int i = 0; i < nArg1; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr = RespAr[i];
		bsc.nBSAnswers[nInd*10+nDisAr] = 1;
		bsc.dwBSAnswers[nInd*10+nDisAr] = MAKELONG(1,0);
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(3,0);	// Tipo de pregunta: Memoria 4
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Media Respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[min(nArg1,nCount-1)]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40+30] = nSize;

	nDis = 5;
	for(int i = (nArg1+1); i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = RespAr[i-nArg1-1]/5;
		nFirst *= 5;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		nDisAr[0] = RespAr[i-nArg1-1]-nFirst;
		bsc.nBSAnswers[nInd*10+RespAr[i-nArg1-1]] = 1;
		bsc.dwBSAnswers[nInd*10+RespAr[i-nArg1-1]] = MAKELONG(17,1);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%5;
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = MAKELONG(17,1);
		}
	}
}

void CSKSFView::GQMemo5(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg1 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg1);
		_printf(str); return;
	}
	if(nArg2 > 10)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser mayor que 10\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg2);
		_printf(str); return;
	}
	int nSize = min(nArg1, nCount);
	int RespAr[15];
	for(int x = 0; x < nSize; x++)
	{
		do
		{
			RespAr[x] = rand()%10;
		} while (nArrayCoord[x*10+RespAr[x]] == 0);
		bsc.dwDBSQuestions[nIndex*30+x] = MAKELONG(RespAr[x],4);
		bsc.dwDBSQuestionsEx[nIndex*40+x] = (DWORD)BeatToTime(GetDocument()->BeatArrow(nCoord[x]));
	}

	bsc.dwBSQuestions[nIndex]=MAKELONG(4,0);	// Tipo de pregunta: Memoria 5
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Media Respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[min(nArg1,nCount-1)]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40+30] = nSize;

	nDis = 5;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = RespAr[i]/5;
		nFirst *= 5;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		nDisAr[0] = RespAr[i]-nFirst;
		bsc.nBSAnswers[nInd*10+RespAr[i]] = 1;
		bsc.dwBSAnswers[nInd*10+RespAr[i]] = MAKELONG(nArg2,18);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%5;
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = MAKELONG(nArg2,18);
		}
	}
}

void CSKSFView::GQObse1(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg2);
		_printf(str); return;
	}
	DWORD dwOrdAnsw[] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
	int nDist=nArg1/10;
	int nMode=nArg1-nDist*10;
	int nSize=nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	for(int x = 0; x <= nSize ; x++)
	{
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 20;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < 5; w++)
				if(dwOrdAnsw[w] == dwAdd)
					bCond = true;
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(0,1);	// Tipo de pregunta: Observación 1
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40]=MAKELONG(nDist+1,0);	// Distribución (1 o 2 veces lo normal)
	bsc.dwDBSQuestionsEx[nIndex*40+1]=MAKELONG(nSize,0);	// Tamaño (para ahorrar procesador)
	for(int x = 0; x < nSize; x++)
	{
		bsc.dwDBSQuestions[nIndex*30+x] = dwOrdAnsw[x];
	}
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = 5;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 1;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		nDis = min(nSize,nDis);
		int nDisAr[] = {-1,-1,-1,-1,-1};
		int nCaretAr[] = {-1,-1,-1,-1,-1};
		/////////////////////////
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(10-nFirst);
			bID = false;
			for(int t = 0; t < 5; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		/////////////////////////
		bsc.nBSAnswers[nInd*10+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*10+nFirst+nDisAr[0]] = dwOrdAnsw[nSize];
		for(int x = 1; x < nDis; x++)
		{
			int nDir;
			do
			{
				nDisp = rand()%(10-nFirst);
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%nSize;
				bID = false;
				for(int t = 0; t < nSize; t++)
					if(nDir == nCaretAr[t])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = dwOrdAnsw[nDir];
		}
	}
}

void CSKSFView::GQObse2(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg2);
		_printf(str); return;
	}
	DWORD dwOrdAnsw[] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
	int nDist=nArg1/10;
	int nMode=nArg1-nDist*10;
	int nSize=nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int nPosRep = (rand()%(nSize-1))+1;
	for(int x = 0; x < nSize; x++)
	{
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 20;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		if(x != nPosRep)
		{
			do
			{
				nNum = rand()%nLimit;
				dwAdd=MAKELONG(nNum, nType);
				bCond = false;
				for(int w = 0; w < 5; w++)
					if(dwOrdAnsw[w] == dwAdd)
						bCond = true;
			} while(bCond);
		}
		else
		{
			nNum = rand()%x;
			dwAdd=dwOrdAnsw[nNum];
		}
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(1,1);	// Tipo de pregunta: Observación 1
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40]=MAKELONG(nDist+1,0);	// Distribución (1 o 2 veces lo normal)
	bsc.dwDBSQuestionsEx[nIndex*40+1]=MAKELONG(nSize,0);	// Tamaño (para ahorrar procesador)
	for(int x = 0; x < nSize; x++)
	{
		bsc.dwDBSQuestions[nIndex*30+x] = dwOrdAnsw[x];
	}
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = 5;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 1;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		nDis = min(nSize-2,nDis);
		int nDisAr[] = {-1,-1,-1,-1,-1};
		int nCaretAr[] = {-1,-1,-1,-1,-1};
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(10-nFirst);
			bID = false;
			for(int t = 0; t < 5; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*10+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*10+nFirst+nDisAr[0]] = dwOrdAnsw[nPosRep];
		for(int x = 1; x < nDis; x++)
		{
			int nDir;
			do
			{
				nDisp = rand()%(10-nFirst);
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%nSize;
				bID = false;
				for(int t = 0; t < nSize; t++)
					if(nDir == nCaretAr[t] || dwOrdAnsw[nDir] == dwOrdAnsw[nPosRep])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = dwOrdAnsw[nDir];
		}
	}
}

void CSKSFView::GQObse3(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[5];
	int nArrayCoord[50];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	int nSum = 1;
	nSum = max(1, nSum);
	int nMin = 0;
	int nMax = 20;
	int nResults[] = {-1,-1,-1,-1,-1};
	if(nArg1)
	{
		int nCar = 0;
		for(int x = 0; x < 5; x++)
		{
			nResults[x] = (rand()%(17+x-nCar)) + nCar;
			nCar = nResults[x]+1;
		}
	}
	else
	{
		int nCar = 20;
		for(int x = 0; x < 5; x++)
		{
			nResults[x] = nCar - (rand()%(nCar-3+x));
			nCar = nResults[x]-1;
		}
	}
	int nCaretOpt[] = {0, 1, 2, 3, 4};
	int nCaretOrd[] = {0,0,0,0,0};
	for(int x = 0; x < 5; x++)
	{
		int nIndResp;
		do
		{
			nIndResp = nCaretOpt[rand()%5];
		} while(nIndResp == -1);
		nCaretOpt[nIndResp] = -1;
		nCaretOrd[x] = nIndResp;
		bsc.dwDBSQuestionsEx[nIndex*40+nIndResp*4] = MAKELONG(nResults[x], 1);
		bsc.dwDBSQuestionsEx[nIndex*40+20+x] = 0;
	}
	bsc.dWIBSQuestions[nIndex] = GetDocument()->BeatArrow(y);
	bsc.dwBSQuestions[nIndex] = (DWORD)MAKELONG(2, 1);
	// ** Esto es juzgar terminos, por lo tanto solo deben haber 3 respuestas maximo
	// Diseñar la respuesta
	nDis = 5;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = GetDocument()->BeatArrow(nCoord[i]);
		}
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = nFirst/5;
		nFirst *= 5;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		nDisAr[0] = nCaretOrd[i];
		bsc.nBSAnswers[nInd*10+nCaretOrd[i]+nFirst] = 1;
		bsc.dwBSAnswers[nInd*10+nCaretOrd[i]+nFirst] = MAKELONG(17, 1);
		bsc.dwDBSQuestionsEx[nIndex*40+25+nCaretOrd[i]] = (DWORD)BeatToTime(bsc.dWBSAnswers[nInd]);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%5;
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = MAKELONG(17, 1);
		}
	}
}

void CSKSFView::GQObse4(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg2);
		_printf(str); return;
	}
	DWORD dwOrdAnsw[] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
	int nIsMax=nArg1/10;
	int nMode=nArg1-nIsMax*10;
	int nSize= min(nArg2,7);
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int nCar = 7;
	int nResults[15];

	for(int x = 0; x < nSize ; x++)
	{
		nResults[x] = nCar - (rand()%(nCar-nSize+1+x));
		nCar = nResults[x]-1;
		//////////////////////////////////////////
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 20;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < 5; w++)
				if(dwOrdAnsw[w] == dwAdd)
					bCond = true;
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(3,1);	// Tipo de pregunta: Observación 4
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	for(int x = 0; x < nSize; x++)
	{
		for(int i = 0; i < nResults[x]; i++)
		{
			int nPosi;
			bool bCond = false;
			do
			{
				nPosi = rand()%40;
				if(bsc.dwDBSQuestionsEx[nIndex*40+nPosi]) bCond = true;
				else
				{
					bsc.dwDBSQuestionsEx[nIndex*40+nPosi] = dwOrdAnsw[x];
					bCond = false;
				}
			} while (bCond);
		}
	}
	DWORD dwBegin = (DWORD)BeatToTime(bsc.dWIBSQuestions[nIndex]);
	for(int x = 0; x < 12; x++)
	{
		int nPosi;
		bool bCond = false;
		do
		{
			nPosi = rand()%12;
			if(bsc.dwDBSQuestions[nIndex*30+nPosi]) bCond = true;
			else
			{
				bsc.dwDBSQuestions[nIndex*30+nPosi] = 75*x+dwBegin;
				bCond = false;
			}
		} while (bCond);
	}
	// Diseñar las respuestas
	nDis = 5;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 1;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		nDis = min(nSize,nDis);
		int nDisAr[] = {-1,-1,-1,-1,-1};
		int nCaretAr[] = {-1,-1,-1,-1,-1};
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(10-nFirst);
			bID = false;
			for(int t = 0; t < 5; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*10+nFirst+nDisAr[0]] = 1;
		nCaretAr[0] = nIsMax?0:(nSize-1);
		bsc.dwBSAnswers[nInd*10+nFirst+nDisAr[0]] = dwOrdAnsw[nCaretAr[0]];
		for(int x = 1; x < nDis; x++)
		{
			int nDir;
			do
			{
				nDisp = rand()%(10-nFirst);
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%nSize;
				bID = false;
				for(int t = 0; t < nSize; t++)
					if(nDir == nCaretAr[t])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = dwOrdAnsw[nDir];
		}
	}
}

void CSKSFView::GQObse5(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg2);
		_printf(str); return;
	}
	DWORD dwOrdAnsw[] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
	int nMode=nArg1;
	int nSize= min(nArg2,7);
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int nResults[15];
	int nHidPos = rand()%nSize;

	for(int x = 0; x < nSize ; x++)
	{
		nResults[x] = (rand()%(36/nSize-1))+1;
		//////////////////////////////////////////
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 20;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < 5; w++)
				if(dwOrdAnsw[w] == dwAdd)
					bCond = true;
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(4,1);	// Tipo de pregunta: Observación 4
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dwBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dwBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	for(int x = 0; x < nSize; x++)
	{
		for(int i = 0; i < nResults[x]; i++)
		{
			int nPosi;
			bool bCond = false;
			do
			{
				nPosi = rand()%40;
				if(bsc.dwDBSQuestionsEx[nIndex*40+nPosi]) bCond = true;
				else
				{
					bsc.dwDBSQuestionsEx[nIndex*40+nPosi] = dwOrdAnsw[x];
					bCond = false;
				}
			} while (bCond);
		}
	}
	DWORD dwBegin = (DWORD)BeatToTime(bsc.dWIBSQuestions[nIndex]);
	for(int x = 0; x < 12; x++)
	{
		int nPosi;
		bool bCond = false;
		do
		{
			nPosi = rand()%12;
			if(bsc.dwDBSQuestions[nIndex*30+nPosi]) bCond = true;
			else
			{
				bsc.dwDBSQuestions[nIndex*30+nPosi] = 75*x+dwBegin;
				bCond = false;
			}
		} while (bCond);
	}
	bsc.dwDBSQuestions[nIndex*30+13] = dwOrdAnsw[nHidPos];
	// Diseñar las respuestas
	nDis = 5;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 1;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		nDis = min(nSize,nDis);
		int nDisAr[] = {-1,-1,-1,-1,-1};
		int nCaretAr[] = {-1,-1,-1,-1,-1};
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(10-nFirst);
			bID = false;
			for(int t = 0; t < 5; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*10+nFirst+nDisAr[0]] = 1;
		nCaretAr[0] = nHidPos;
		bsc.dwBSAnswers[nInd*10+nFirst+nDisAr[0]] = dwOrdAnsw[nCaretAr[0]];
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp, nDir;
			do
			{
				nDisp = rand()%(10-nFirst);
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%nSize;
				bID = false;
				for(int t = 0; t < nSize; t++)
					if(nDir == nCaretAr[t])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = dwOrdAnsw[nDir];
		}
	}
}

void CSKSFView::GQAnal1(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	/*if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg2);
		_printf(str); return;
	}*/
	DWORD dwOrdAnsw[6];
	int nMode=nArg1;
	//int nSize=nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int kMode;
	if(nMode <= 5) kMode = nMode;
	else
	{
		if(nMode == 6)
		{
			int nPosib[] = {0, 1, 3, 5};
			kMode = nPosib[rand()%4];
		}
		else
		{
			int nPosib[] = {0, 2, 4, 5};
			kMode = nPosib[rand()%4];
		}
	}
	switch(kMode)
	{
	case 0:nType = 1;nLimit = 10;break;
	case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
	case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
	case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
	case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
	case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
	}
	nNum = rand()%nLimit;
	dwOrdAnsw[0]=MAKELONG(nNum, nType);
	dwOrdAnsw[5]=MAKELONG(nNum, nType==1?1:(nType+3));
	for(int z = 1; z < 5; z++)
	{
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 10;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			DWORD dwAdd = MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < 5; w++)
			{
				if(dwOrdAnsw[w] == dwAdd)
				{
					bCond = true;
				}
			}
		} while(bCond);
		dwOrdAnsw[z]=MAKELONG(nNum, nType);
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(0,3);	// Tipo de pregunta: Analisys 1
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		GetDocument()->BeatArrow(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		GetDocument()->BeatArrow(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40] = dwOrdAnsw[5];
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = 5;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 1;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(10-nFirst);
			bID = false;
			for(int t = 0; t < 5; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*10+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*10+nFirst+nDisAr[0]] = dwOrdAnsw[0];
		bsc.nDBSAnswers[nInd] = 1;	// Mostrar numeros como tal
		for(int x = 1; x < nDis; x++)
		{
			do
			{
				nDisp = rand()%(10-nFirst);
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = dwOrdAnsw[x];
			bsc.nDBSAnswers[nInd];	// Mostrar numeros como tal
		}
	}
}

void CSKSFView::GQAnal2(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg2);
		_printf(str); return;
	}
	DWORD dwOrdAnsw[] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
	int nMode=nArg1;
	int nSize= nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int nCar = 7;

	for(int x = 0; x < nSize ; x++)
	{
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 20;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < 5; w++)
				if(dwOrdAnsw[w] == dwAdd)
					bCond = true;
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(1,3);	// Tipo de pregunta: Observación 4
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	int nData[28][16];
	FILE* hFile;
	hFile = _fopen(_T("skins/BS/BS42.acc"), _T("r"));
	if(hFile != NULL)
	{
		TCHAR ch[100];
		for(int x = 0; x < 28; x++)
		{
			for(int i = 0; i < 4; i++)
			{
				_fgets(ch, 100, hFile);
				if(_sscanf(ch, _T("%d %d %d %d"), &nData[x][i*4], &nData[x][i*4+1], &nData[x][i*4+2], &nData[x][i*4+3]) != 4)
					_printf(_T("El archivo BS42.acc no tiene el formato adecuado, no se garantiza el funcionamiento del Brain Shower"));
			}
		}
	}
	else _printf(_T("El archivo BS42.acc no se encuentra en la ruta, no se garantiza el funcionamiento del Brain Shower"));
	fclose(hFile);
	int nSizePerData = 0;
	int nPerData[28];
	int nSizePosPerData[28];
	int nPosPerDataX[28][36];
	int nPosPerDataY[28][36];
	int nNumItem = 0;
	for(int x = 0; x < nSize; x++)
	{
		nSizePerData = 0;
		for(int j = 0; j < 28; j++) nSizePosPerData[j] = 0;
		for(int i = 0; i < 28; i++)	// Recorrer los arreglos
		{
			for(int u = 0; u < 6; u++) for(int v = 0; v < 6; v++)	// Recorrer las posiciones
			{
				bool bInc = true;
				for(int a = 0; a < 4; a++)
				{
					for(int b = 0; b < 4; b++)
					{
						if((v+b) < 6 && (u+a) < 6)
						{
							if(bsc.dwDBSQuestionsEx[nIndex*40+(v+b)*6+(u+a)] && nData[i][b*4+a])
							{
								bInc = false;
								break;
							}
						}
						else if(nData[i][b*4+a])
						{
							bInc = false;
							break;
						}
					}
					if(!bInc) break;
				}
				if(bInc)
				{
					nSizePosPerData[i]++;
					nPosPerDataX[i][nSizePosPerData[i]-1] = u;
					nPosPerDataY[i][nSizePosPerData[i]-1] = v;
				}
			}
			if(nSizePosPerData[i] != 0)
			{
				nSizePerData++;
				nPerData[nSizePerData-1] = i;
			}
		}
		if(nSizePerData == 0) break;	// Para que si no podemos incluir mas?
		// Randomizar el arreglo
		int p = rand()%nSizePerData;
		p = nPerData[p];
		int q = rand()%nSizePosPerData[p];
		int u = nPosPerDataX[p][q], v = nPosPerDataY[p][q];
		for(int a = 0; a < 4; a++)
		{
			for(int b = 0; b < 4; b++)
			{
				if((v+b) < 6 && (u+a) < 6)
				{
					if(nData[p][b*4+a])
					{
						bsc.dwDBSQuestionsEx[nIndex*40+(v+b)*6+(u+a)] = dwOrdAnsw[x];
						nNumItem++;
					}
				}
			}
		}
	}
	DWORD dwBegin = (DWORD)BeatToTime(bsc.dWIBSQuestions[nIndex]);
	for(int x = 0; x < 12; x++)
	{
		int nPosi;
		bool bCond = false;
		do
		{
			nPosi = rand()%12;
			if(bsc.dwDBSQuestions[nIndex*30+nPosi]) bCond = true;
			else
			{
				bsc.dwDBSQuestions[nIndex*30+nPosi] = 75*x+dwBegin;
				bCond = false;
			}
		} while (bCond);
	}
	// Diseñar las respuestas
	nDis = 5;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 1;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		int nCaretAr[] = {-1,-1,-1,-1,-1};
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(10-nFirst);
			bID = false;
			for(int t = 0; t < 5; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*10+nFirst+nDisAr[0]] = 1;
		nCaretAr[0] = nNumItem;
		bsc.dwBSAnswers[nInd*10+nFirst+nDisAr[0]] = MAKELONG(nNumItem,1);
		for(int x = 1; x < nDis; x++)
		{
			int nDir;
			do
			{
				nDisp = rand()%(10-nFirst);
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%36;
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDir == nCaretAr[t])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = MAKELONG(nDir,1);
		}
	}
}

void CSKSFView::GQAnal3(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg2);
		_printf(str); return;
	}
	// Encontrar el número de items
	int nCaretSizeCoord = 0;
	int nFirst = -1;
	if(nCount >= 2)
	{
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
	}
	DWORD dwOrdAnsw[] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
						,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
	int nDist=nArg1/10;
	int nMode=nArg1-nDist*10;
	int nSize=nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int nPosQ = rand()%nCaretSizeCoord;
	int nPosS = rand()%nSize;
	for(int x = 0; x < nCaretSizeCoord; x++)
	{
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 17;break;	// No podemos incluir la pregunta ahi... ^^
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < 5; w++)
				if(dwOrdAnsw[w] == dwAdd)
					bCond = true;
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(2,3);	// Tipo de pregunta: Analisys 3
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40]=MAKELONG(nDist+1,0);	// Distribución (1 o 2 veces lo normal)
	bsc.dwDBSQuestionsEx[nIndex*40+1]=MAKELONG(nSize*nCaretSizeCoord,0);	// Tamaño (para ahorrar procesador)
	for(int x = 0; x < nSize; x++) for(int i = 0; i < nCaretSizeCoord; i++)
	{
		if((x*nCaretSizeCoord+i)<30)
		{
			if(i == nPosQ && x == nPosS)
				bsc.dwDBSQuestions[nIndex*30+x*nCaretSizeCoord+i] = MAKELONG(17,1);
			else
				bsc.dwDBSQuestions[nIndex*30+x*nCaretSizeCoord+i] = dwOrdAnsw[i];
		}
	}
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = 5;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 1;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
		int nCaretAr[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(10-nFirst);
			bID = false;
			for(int t = 0; t < 5; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		nCaretAr[0] = nPosQ;
		bsc.nBSAnswers[nInd*10+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*10+nFirst+nDisAr[0]] = dwOrdAnsw[nPosQ];
		for(int x = 1; x < nDis; x++)
		{
			int nDir;
			do
			{
				nDisp = rand()%(10-nFirst);
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*10+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%nCaretSizeCoord;
				bID = false;
				for(int t = 0; t < nCaretSizeCoord; t++)
					if(nDir == nCaretAr[t])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = dwOrdAnsw[nDir];
		}
	}
}

void CSKSFView::GQAnal4(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[16];
	int nArrayCoord[160];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	int RespAr;
	do
	{
		RespAr = rand()%10;
	} while (nArrayCoord[RespAr] == 0);
	bsc.dwDBSQuestionsEx[nIndex*40] = MAKELONG(RespAr,4);
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(3,3);	// Tipo de pregunta: Analisys 4
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Media Respuesta (omitida)
		nCount==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nCount==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);

	// Vamos entonces a girar la flecha!
	int nGir = rand()%3; // 0 = 90, 1 = 180, 2 = 270
	bsc.dwDBSQuestionsEx[nIndex*40+31] = nGir;
	int nDesp = nGir+1;
	int nFirst = RespAr/5;
	int nArrow = RespAr-nFirst;
	if(nArrow != 2)
	{
		if(nArrow > 2) nArrow--;	// Normalizar la flecha (1245 a 1234)
		nArrow-=nDesp;				// Mover flecha
		while(nArrow > 4)
			nArrow-=4;
		while(nArrow < 0)
			nArrow+=4;
		if(nArrow > 1) nArrow++;	// Volver a liberar la flecha (1234 a 1245)
	}
	RespAr = nArrow+nFirst;
	nDis = 5;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = nFirst/5;
		nFirst *= 5;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		nDisAr[0] = RespAr-nFirst;
		bsc.nBSAnswers[nInd*10+RespAr] = 1;
		bsc.dwBSAnswers[nInd*10+RespAr] = MAKELONG(17,1);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%5;
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = MAKELONG(17,1);
		}
	}
}

void CSKSFView::GQAnal5(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int nCoord[34];
	int nArrayCoord[340];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < 10; k++) nArrayCoord[x*10+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < GetDocument()->song.hStep.nSizeArrows1; i++)
	{
		if(GetDocument()->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < 10; x++)
			{
				if(LOWORD(GetDocument()->GetArrow(x, i, 0)) == 20)
				{
					nArrayCoord[nCount*10+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(GetDocument()->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg1 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), GetDocument()->BeatArrow(y), (int)(GetDocument()->BeatArrow(y)/4), nArg1);
		_printf(str); return;
	}
	int nSize = min(nArg1, nCount);
	int RespAr[15];
	for(int x = 0; x < nSize; x++)
	{
		do
		{
			RespAr[x] = rand()%10;
		} while (nArrayCoord[x*10+RespAr[x]] == 0);
		bsc.dwDBSQuestions[nIndex*30+x] = MAKELONG(RespAr[x],4);
		bsc.dwDBSQuestionsEx[nIndex*40+x] = (DWORD)BeatToTime(GetDocument()->BeatArrow(nCoord[x]));
	}

	for(int i = 0; i < nArg1; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr = RespAr[i];
		bsc.nBSAnswers[nInd*10+nDisAr] = 1;
		bsc.dwBSAnswers[nInd*10+nDisAr] = MAKELONG(1,0);
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=MAKELONG(4,3);	// Tipo de pregunta: Analisys 5
	bsc.dWIBSQuestions[nIndex]=GetDocument()->BeatArrow(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Media Respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[min(nArg1,nCount-1)]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:GetDocument()->BeatArrow(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40+30] = nSize;

	// Vamos entonces a girar las flechas!
	int nGir = rand()%3; // 0 = 90, 1 = 180, 2 = 270
	bsc.dwDBSQuestionsEx[nIndex*40+31] = nGir;
	int nDesp = nGir+1;
	for(int x = 0; x < nSize; x++)
	{
		int nFirst = RespAr[x]/5;
		int nArrow = RespAr[x]-nFirst;
		if(nArrow != 2)
		{
			if(nArrow > 2) nArrow--;	// Normalizar la flecha (1245 a 1234)
			nArrow-=nDesp;				// Mover flecha
			while(nArrow > 4)
				nArrow-=4;
			while(nArrow < 0)
				nArrow+=4;
			if(nArrow > 1) nArrow++;	// Volver a liberar la flecha (1234 a 1245)
		}
		RespAr[x] = nArrow+nFirst;
	}
	nDis = 5;
	for(int i = (nArg1+1); i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		bsc.nSizeBSAnswers++;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
		int nInd = bsc.nSizeBSAnswers-1;
		bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.dWWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
		bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
		for(int x = 0; x < 10; x++)
		{
			bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
			bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*10+x]=0;
		}

		double dWOffset = 0.0;
		for(int x = 0; x < GetDocument()->song.hStep.nSizeStops; x++)
		{
			if(GetDocument()->song.nWStops[x] < y)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] == y && !m_bGetArrowInStop)
				dWOffset += GetDocument()->song.dStops[x];
			if(GetDocument()->song.nWStops[x] > y) break;
		}
		for(int x = 0; x < GetDocument()->song.hStep.nSizeJumps; x++)
		{
			if(GetDocument()->song.nIJumps[x] < y)
				dWOffset -= GetDocument()->BeatArrow(GetDocument()->song.nEJumps[x])-GetDocument()->BeatArrow(GetDocument()->song.nIJumps[x]);
			if(GetDocument()->song.nIJumps[x] > y) break;
		}
		bsc.dWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i]);
		bsc.dWWBSAnswers[nInd] = GetDocument()->BeatArrow(nCoord[i])+dWOffset;
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < 10; x++)
		{
			if(nArrayCoord[i*10+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = RespAr[i-nArg1-1]/5;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[] = {-1,-1,-1,-1,-1};
		nDisAr[0] = RespAr[i-nArg1-1]-nFirst;
		bsc.nBSAnswers[nInd*10+RespAr[i-nArg1-1]] = 1;
		bsc.dwBSAnswers[nInd*10+RespAr[i-nArg1-1]] = MAKELONG(17,1);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%5;
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*10+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*10+nDisp+nFirst] = MAKELONG(17,1);
		}
	}
}

void CSKSFView::DeleteBS(void)
{
	if(bsc.nSizeBSQuestions)
	{
		bsc.nSizeBSQuestions=0;
		bsc.dWIBSQuestions = (double*)realloc(bsc.dWIBSQuestions, bsc.nSizeBSQuestions*sizeof(double));
		bsc.dWEBSQuestions = (double*)realloc(bsc.dWEBSQuestions, bsc.nSizeBSQuestions*sizeof(double));
		bsc.dWABSQuestions = (double*)realloc(bsc.dWABSQuestions, bsc.nSizeBSQuestions*sizeof(double));
		bsc.dwBSQuestions = (DWORD*)realloc(bsc.dwBSQuestions, bsc.nSizeBSQuestions*sizeof(DWORD));
		bsc.dwDBSQuestions = (DWORD*)realloc(bsc.dwDBSQuestions, bsc.nSizeBSQuestions*30*sizeof(DWORD));
		bsc.dwDBSQuestionsEx = (DWORD*)realloc(bsc.dwDBSQuestionsEx, bsc.nSizeBSQuestions*40*sizeof(DWORD));
	}
	if(bsc.nSizeBSAnswers)
	{
		bsc.nSizeBSAnswers=0;
		bsc.dWBSAnswers = (double*)realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dWWBSAnswers = (double*)realloc(bsc.dWWBSAnswers, bsc.nSizeBSAnswers*sizeof(double));
		bsc.dwBSAnswers = (DWORD*)realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
	}
	if(bsc.nSizeBSPopUp)
	{
		bsc.nSizeBSPopUp = 0;
		bsc.dWBSPopUp = (double*)realloc(bsc.dWBSPopUp, bsc.nSizeBSPopUp*sizeof(double));
		bsc.dwBSPopUp = (DWORD*)realloc(bsc.dwBSPopUp, bsc.nSizeBSPopUp*sizeof(DWORD));
	}
}

int CSKSFView::DeleteSong(void)
{
	if(songc.nSizeArrows1)
	{
		songc.nSizeArrows1 = 0;
		songc.dwArrows1=(DWORD*)realloc(songc.dwArrows1, songc.nSizeArrows1*sizeof(DWORD)*10);
		songc.dWArrows1=(double*)realloc(songc.dWArrows1, songc.nSizeArrows1*sizeof(double));
		songc.dWWArrows1=(double*)realloc(songc.dWWArrows1, songc.nSizeArrows1*sizeof(double));
	}
	if(songc.nSizeArrows2)
	{
		songc.nSizeArrows2 = 0;
		songc.dwArrows2=(DWORD*)realloc(songc.dwArrows2, songc.nSizeArrows2*sizeof(DWORD)*10);
		songc.dWArrows2=(double*)realloc(songc.dWArrows2, songc.nSizeArrows2*sizeof(double));
		songc.dWWArrows2=(double*)realloc(songc.dWWArrows2, songc.nSizeArrows2*sizeof(double));
	}
	if(songc.nSizeArrows3)
	{
		songc.nSizeArrows3 = 0;
		songc.dwArrows3=(DWORD*)realloc(songc.dwArrows3, songc.nSizeArrows3*sizeof(DWORD)*10);
		songc.dWArrows3=(double*)realloc(songc.dWArrows3, songc.nSizeArrows3*sizeof(double));
		songc.dWWArrows3=(double*)realloc(songc.dWWArrows3, songc.nSizeArrows3*sizeof(double));
	}
	if(songc.nSizeChangeBPM)
	{
		songc.nSizeChangeBPM = 0;
		songc.dWChangeBPM=(double*)realloc(songc.dWChangeBPM, songc.nSizeChangeBPM*sizeof(double));
		songc.dChangeBPM=(double*)realloc(songc.dChangeBPM, songc.nSizeChangeBPM*sizeof(double));
	}
	if(songc.nSizeChangeVel)
	{
		songc.nSizeChangeVel = 0;
		songc.dIChangeVel=(double*)realloc(songc.dIChangeVel, songc.nSizeChangeVel*sizeof(double));
		songc.dEChangeVel=(double*)realloc(songc.dEChangeVel, songc.nSizeChangeVel*sizeof(double));
		songc.dChangeVel=(double*)realloc(songc.dChangeVel, songc.nSizeChangeVel*sizeof(double));
	}
	if(songc.nSizeJumps)
	{
		songc.nSizeJumps = 0;
		songc.dIJumps=(double*)realloc(songc.dIJumps, songc.nSizeJumps*sizeof(double));
		songc.dEJumps=(double*)realloc(songc.dEJumps, songc.nSizeJumps*sizeof(double));
	}
	if(songc.nSizeStops)
	{
		songc.nSizeStops = 0;
		songc.dWStops=(double*)realloc(songc.dWStops, songc.nSizeStops*sizeof(double));
		songc.dStops=(double*)realloc(songc.dStops, songc.nSizeStops*sizeof(double));
	}
	return 1;
}

DWORD CSKSFView::GetArrow(int x, int y, int nGArrow)
{
	int nRel;
	if(nGArrow == 0)
	{
		nRel = y*10+x;
		if(nRel < 0) return 0;
		if(nRel >= (songc.nSizeArrows1*10)) return 0;
		return songc.dwArrows1[nRel];
	}
	if(nGArrow == 1)
	{
		nRel = y*10+x;
		if(nRel < 0) return 0;
		if(nRel >= (songc.nSizeArrows2*10)) return 0;
		return songc.dwArrows2[nRel];
	}
	if(nGArrow == 2)
	{
		nRel = y*10+x;
		if(nRel < 0) return 0;
		if(nRel >= (songc.nSizeArrows3*10)) return 0;
		return songc.dwArrows3[nRel];
	}
	return 0;
}

double CSKSFView::BeatArrow(int nIndex, int nGArrow)
{
	if(nGArrow == 0)
	{
		if(nIndex < 0) return GetDocument()->BeatArrow(nIndex);
		if(nIndex > songc.nSizeArrows1) return 0.0;
		return songc.dWArrows1[nIndex];
	}
	if(nGArrow == 1)
	{
		if(nIndex < 0) return GetDocument()->BeatArrow(nIndex);
		if(nIndex > songc.nSizeArrows2) return 0.0;
		return songc.dWArrows2[nIndex];
	}
	if(nGArrow == 2)
	{
		if(nIndex < 0) return GetDocument()->BeatArrow(nIndex);
		if(nIndex > songc.nSizeArrows3) return 0.0;
		return songc.dWArrows3[nIndex];
	}
	if(nGArrow == 3)
	{
		if(nIndex < 0) return GetDocument()->BeatArrow(nIndex);
		if(nIndex > bsc.nSizeBSAnswers) return 0.0;
		return bsc.dWBSAnswers[nIndex];
	}
	return 0.0;
}

bool CSKSFView::SomeArrow(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > 2) return false;
	for(int x = 0; x < 10; x++)
	{
		DWORD dw = LOWORD(GetArrow(x, y, nGArrow));
		if(dw>=1&&dw<=4)
			bReturn = true;
	}
	return bReturn;
}

bool CSKSFView::SomeObject(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > 2) return false;
	for(int x = 0; x < 10; x++)
	{
		if(LOWORD(GetArrow(x, y, nGArrow))>4)
			bReturn = true;
	}
	return bReturn;
}

bool CSKSFView::SomeNote(int y, int nGArrow)
{
	return SomeArrow(y, nGArrow) || SomeObject(y, nGArrow);
}
double CSKSFView::OffsetInZArrow(int x)
{
	/*switch(x)
	{
	case 0:
	case 4:
	case 5:
	case 9:
		return 0.0;
		break;
	case 1:
	case 3:
	case 6:
	case 8:
		return 0.0;
		break;
	default:
		return 0.0;
	}*/return 0.0;
}

bool CSKSFView::LoadMachine(CString m_strFile)
{
	FILE* hFile;
	//TCHAR chFile[260];
	//_sprintf(chFile, _T("%ls"), m_strFile);
	hFile = _fopen(m_strFile, _T("r"));
	if(hFile == NULL) return false;
	TCHAR ch[500];
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &machine.hMachine.nSizeVtxMachine[y]);
		_fgets(ch, 500, hFile);
		_sscanf(ch, _T("*MESH_NUMFACES %d"), &machine.hMachine.nSizeFaceMachine[y]);
		machine.dVtxMachine[y] = (double*)realloc(machine.dVtxMachine[y], machine.hMachine.nSizeVtxMachine[y]*3*sizeof(double));
		machine.nFaceMachine[y] = (int*)realloc(machine.nFaceMachine[y], machine.hMachine.nSizeFaceMachine[y]*3*sizeof(int));
		for(int x = 0; x < machine.hMachine.nSizeVtxMachine[y]; x++)
		{
			if(_fgets(ch, 500, hFile) != NULL)
			{
				int n;
				double v[3];
				if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
				for(int z = 0; z < 3; z++) machine.dVtxMachine[y][n*3+z] = v[z];
			}
			else {fclose(hFile); return false;}
		}
		for(int x = 0; x < machine.hMachine.nSizeFaceMachine[y]; x++)
		{
			if(_fgets(ch, 500, hFile) != NULL)
			{
				int n;
				int f[3];
				int a[3];
				if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
				for(int z = 0; z < 3; z++) machine.nFaceMachine[y][n*3+z] = f[z];
			}
			else {fclose(hFile); return false;}
		}
		_fgets(ch, 500, hFile);
		_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &machine.hMachine.nSizeUVTMachine[y]);
		machine.dUVTMachine[y] = (double*)realloc(machine.dUVTMachine[y], machine.hMachine.nSizeUVTMachine[y]*3*sizeof(double));
		for(int x = 0; x < machine.hMachine.nSizeUVTMachine[y]; x++)
		{
			if(_fgets(ch, 500, hFile) != NULL)
			{
				int n;
				double v[3];
				if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
				for(int z = 0; z < 3; z++) machine.dUVTMachine[y][n*3+z] = v[z];
			}
			else {fclose(hFile); return false;}
		}
		_fgets(ch, 500, hFile);
		_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &machine.hMachine.nSizeTMachine[y]);
		machine.nTMachine[y] = (int*)realloc(machine.nTMachine[y], machine.hMachine.nSizeTMachine[y]*3*sizeof(int));
		for(int x = 0; x < machine.hMachine.nSizeTMachine[y]; x++)
		{
			if(_fgets(ch, 500, hFile) != NULL)
			{
				int n;
				int f[3];
				if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
				for(int z = 0; z < 3; z++) machine.nTMachine[y][n*3+z] = f[z];
			}
			else {fclose(hFile); return false;}
		}
	}
	for(int y = 0; y < 10; y++)
	{
		_fgets(ch, 500, hFile);
		_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &machine.hMachine.nSizeVtxTap[y]);
		_fgets(ch, 500, hFile);
		_sscanf(ch, _T("*MESH_NUMFACES %d"), &machine.hMachine.nSizeFaceTap[y]);
		machine.dVtxTap[y] = (double*)realloc(machine.dVtxTap[y], machine.hMachine.nSizeVtxTap[y]*3*sizeof(double));
		machine.nFaceTap[y] = (int*)realloc(machine.nFaceTap[y], machine.hMachine.nSizeFaceTap[y]*3*sizeof(int));
		for(int x = 0; x < machine.hMachine.nSizeVtxTap[y]; x++)
		{
			if(_fgets(ch, 500, hFile) != NULL)
			{
				int n;
				double v[3];
				if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
				for(int z = 0; z < 3; z++) machine.dVtxTap[y][n*3+z] = v[z];
			}
			else {fclose(hFile); return false;}
		}
		for(int x = 0; x < machine.hMachine.nSizeFaceTap[y]; x++)
		{
			if(_fgets(ch, 500, hFile) != NULL)
			{
				int n;
				int f[3];
				int a[3];
				if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
				for(int z = 0; z < 3; z++) machine.nFaceTap[y][n*3+z] = f[z];
			}
			else {fclose(hFile); return false;}
		}
		_fgets(ch, 500, hFile);
		_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &machine.hMachine.nSizeUVTTap[y]);
		machine.dUVTTap[y] = (double*)realloc(machine.dUVTTap[y], machine.hMachine.nSizeUVTTap[y]*3*sizeof(double));
		for(int x = 0; x < machine.hMachine.nSizeUVTTap[y]; x++)
		{
			if(_fgets(ch, 500, hFile) != NULL)
			{
				int n;
				double v[3];
				if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
				for(int z = 0; z < 3; z++) machine.dUVTTap[y][n*3+z] = v[z];
			}
			else {fclose(hFile); return false;}
		}
		_fgets(ch, 500, hFile);
		_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &machine.hMachine.nSizeTTap[y]);
		machine.nTTap[y] = (int*)realloc(machine.nTTap[y], machine.hMachine.nSizeTTap[y]*3*sizeof(int));
		for(int x = 0; x < machine.hMachine.nSizeTTap[y]; x++)
		{
			if(_fgets(ch, 500, hFile) != NULL)
			{
				int n;
				int f[3];
				if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
				for(int z = 0; z < 3; z++) machine.nTTap[y][n*3+z] = f[z];
			}
			else {fclose(hFile); return false;}
		}
	}
	for(int x = 0; x < 10; x++)
	{
		_fgets(ch, 500, hFile);
		CString str;
		str.Format(_T("Characters/Machine/%ls"), ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		TCHAR chNameTex[500];
		_strcpy(chNameTex, str);
		machine.nTexTap[x] = GenTexImage(chNameTex);
	}
	for(int y = 0; y < 10; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*POS_TAP %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) machine.hMachine.dPosTap[n*3+z] = v[z];
	}
	double v[3];
	_fgets(ch, 500, hFile);
	if(_sscanf(ch, _T("*POS_NOTAP_L_CZ\t%lg\t%lg\t%lg"), &v[0], &v[1], &v[2])!=3) {fclose(hFile); return false;}
	for(int z = 0; z < 3; z++) machine.hMachine.dPosNoTapL[z] = v[z];
	_fgets(ch, 500, hFile);
	if(_sscanf(ch, _T("*POS_NOTAP_R_CZ\t%lg\t%lg\t%lg"), &v[0], &v[1], &v[2])!=3) {fclose(hFile); return false;}
	for(int z = 0; z < 3; z++) machine.hMachine.dPosNoTapR[z] = v[z];
	fclose(hFile);
	return true;
}

void CSKSFView::DeleteMachine(void)
{
	memset(&machine.hMachine, 0, sizeof(MACHINE_HEADER));
	machine.dVtxMachine[0] = (double*)realloc(machine.dVtxMachine[0], machine.hMachine.nSizeVtxMachine[0]*3*sizeof(double));
	machine.dVtxMachine[1] = (double*)realloc(machine.dVtxMachine[1], machine.hMachine.nSizeVtxMachine[1]*3*sizeof(double));
	machine.nFaceMachine[0] = (int*)realloc(machine.nFaceMachine[0], machine.hMachine.nSizeFaceMachine[0]*3*sizeof(int));
	machine.nFaceMachine[1] = (int*)realloc(machine.nFaceMachine[1], machine.hMachine.nSizeFaceMachine[1]*3*sizeof(int));
	machine.dUVTMachine[0] = (double*)realloc(machine.dUVTMachine[0], machine.hMachine.nSizeUVTMachine[0]*3*sizeof(double));
	machine.dUVTMachine[1] = (double*)realloc(machine.dUVTMachine[1], machine.hMachine.nSizeUVTMachine[1]*3*sizeof(double));
	machine.nTMachine[0] = (int*)realloc(machine.nTMachine[0], machine.hMachine.nSizeTMachine[0]*3*sizeof(int));
	machine.nTMachine[1] = (int*)realloc(machine.nTMachine[1], machine.hMachine.nSizeTMachine[1]*3*sizeof(int));
	for(int x = 0; x < 10; x++)
	{
		machine.dVtxTap[x] = (double*)realloc(machine.dVtxTap[x], machine.hMachine.nSizeVtxTap[x]*3*sizeof(double));
		machine.nFaceTap[x] = (int*)realloc(machine.nFaceTap[x], machine.hMachine.nSizeFaceTap[x]*3*sizeof(int));
		machine.dUVTTap[x] = (double*)realloc(machine.dUVTTap[x], machine.hMachine.nSizeUVTTap[x]*3*sizeof(double));
		machine.nTTap[x] = (int*)realloc(machine.nTTap[x], machine.hMachine.nSizeTTap[x]*3*sizeof(int));
	}
}

int CSKSFView::CompileTraining(void)
{
	trac.nSizeArg = songc.nSizeArrows1;
	trac.nArg = (int*)realloc(trac.nArg, trac.nSizeArg*10*sizeof(int));
	for(int x = 0; x < (trac.nSizeArg*10); x++) trac.nArg[x] = 0;
	int nFoot = 0;
	for(int x = 0; x < songc.nSizeArrows1; x++)
	{
		int np = GetNumberPivotArrows(x);
		int sp = GetNumberAsignedArrows(x);
		if(np > 1)
		{
			if(sp > 0)
			{
				int f = 0;//nFoot==0?0:(nFoot==1?2:1);
				if(sp == 1)
				{
					for(int y = 0; y < (m_bIsSingle?5:10); y++)
					{
						if(trac.nArg[x*10+y] != 0)
						{
							nFoot = trac.nArg[x*10+y];
							f = trac.nArg[x*10+y]==0?0:(trac.nArg[x*10+y]==1?2:1);
							break;
						}
					}
				}
				else f = 0;
				for(int y = 0; y < (m_bIsSingle?5:10); y++) if((LOWORD(songc.dwArrows1[(x)*10+y]) == 1 || LOWORD(songc.dwArrows1[(x)*10+y]) == 3) && trac.nArg[x*10+y] == 0)
				{
					int b = x;
					if(LOWORD(songc.dwArrows1[(x)*10+y]) == 3)
					{
						for(b = x; b < songc.nSizeArrows1; b++)
						{
							if((b+1) >= songc.nSizeArrows1) break;
							if(LOWORD(songc.dwArrows1[b*10+y]) == 0 || LOWORD(songc.dwArrows1[b*10+y]) > 4)
							{
								b--;
								break;
							}
							if(LOWORD(songc.dwArrows1[b*10+y]) == 4)
								break;
						}
					}
					for(int i = x; i <= b; i++)
						trac.nArg[i*10+y] = f;
				}
			}
			else
			{
				int f = 1;
				for(int y = 0; y < (m_bIsSingle?5:10); y++) if((LOWORD(songc.dwArrows1[(x)*10+y]) >= 1 && LOWORD(songc.dwArrows1[(x)*10+y]) <= 4) && trac.nArg[x*10+y] == 0)
				{
					int b = x;
					if(LOWORD(songc.dwArrows1[(x)*10+y]) == 3)
					{
						for(b = x; b < songc.nSizeArrows1; b++)
						{
							if((b+1) >= songc.nSizeArrows1) break;
							if(LOWORD(songc.dwArrows1[b*10+y]) == 0 || LOWORD(songc.dwArrows1[b*10+y]) > 4)
							{
								b--;
								break;
							}
							if(LOWORD(songc.dwArrows1[b*10+y]) == 4)
								break;
						}
					}
					for(int i = x; i <= b; i++)
						trac.nArg[i*10+y] = f;
					if(np > 2) f = 0;
					else f = 2;
					np--;
				}
				nFoot = 0;
			}
		}
		else if(np > sp)
		{
			int n1 = GetPivotSub(x);
			int n2 = GetNextPivotSub(x, n1);
			int n3 = GetNextPivotArrowsNC(x);
			if(nFoot <= 0)
			{
				if(n1 < n2)
					nFoot = 1;
				if(n1 > n2)
					nFoot = 2;
			}
			for(int y = 0; y < (m_bIsSingle?5:10); y++)
			{
				if((LOWORD(songc.dwArrows1[x*10+y]) == 1 || LOWORD(songc.dwArrows1[(x)*10+y]) == 3) && trac.nArg[x*10+y] == 0)
				{
					int b = x;
					if(LOWORD(songc.dwArrows1[(x)*10+y]) == 3)
					{
						for(b = x; b < songc.nSizeArrows1; b++)
						{
							if((b+1) >= songc.nSizeArrows1) break;
							if(LOWORD(songc.dwArrows1[b*10+y]) == 0 || LOWORD(songc.dwArrows1[b*10+y]) > 4)
							{
								b--;
								break;
							}
							if(LOWORD(songc.dwArrows1[b*10+y]) == 4)
								break;
						}
					}
					for(int i = x; i <= b; i++)
						trac.nArg[i*10+y] = nFoot;
				}
			}
			if(n1 != n3)
			{
				nFoot = nFoot==1?2:1;
			}
		}
	}
	return 1;
}

int CSKSFView::GetPivotSub(int x)
{
	int nPivot = -1;
	for(int y = 0; y < (m_bIsSingle?5:10); y++) if(LOWORD(songc.dwArrows1[(x)*10+y]) == 1 || LOWORD(songc.dwArrows1[(x)*10+y]) == 3)
	{
		if(nPivot != -1) return -1;
		nPivot = y;
	}
	return nPivot;
}

int CSKSFView::GetNextPivotSub(int x, int n)
{
	int nPivot = n;
	for(int i = x+1; i < songc.nSizeArrows1; i++)
	{
		nPivot = GetPivotSub(i);
		if(nPivot != n && nPivot != -1) return nPivot;
	}
	return nPivot;
}

int CSKSFView::GetNextPivotArrowsNC(int x)
{
	int nPivot = -1;
	for(int i = x+1; i < songc.nSizeArrows1; i++)
	{
		nPivot = GetPivotSub(i);
		if(nPivot != -1) return nPivot;
	}
	return nPivot;
}


double CSKSFView::BeatWArrow(int nIndex, int nGArrow)
{
	if(nGArrow == 0)
	{
		if(nIndex < 0) return GetDocument()->BeatArrow(nIndex);
		if(nIndex > songc.nSizeArrows1) return 0.0;
		return songc.dWWArrows1[nIndex];
	}
	if(nGArrow == 1)
	{
		if(nIndex < 0) return GetDocument()->BeatArrow(nIndex);
		if(nIndex > songc.nSizeArrows2) return 0.0;
		return songc.dWWArrows2[nIndex];
	}
	if(nGArrow == 2)
	{
		if(nIndex < 0) return GetDocument()->BeatArrow(nIndex);
		if(nIndex > songc.nSizeArrows3) return 0.0;
		return songc.dWWArrows3[nIndex];
	}
	return 0.0;
}

void CSKSFView::DeleteTrainingCompiler(void)
{
	trac.nSizeArg = 0;
	trac.nArg = (int*)realloc(trac.nArg, trac.nSizeArg*10*sizeof(int));
}

int CSKSFView::GetNumberPivotArrows(int x)
{
	int nPivot = 0;
	for(int y = 0; y < (m_bIsSingle?5:10); y++)
	{
		if((LOWORD(songc.dwArrows1[x*10+y]) >= 1) && (LOWORD(songc.dwArrows1[x*10+y]) <= 4))
		{
			nPivot++;
		}
	}
	return nPivot;
}

int CSKSFView::GetNumberAsignedArrows(int x)
{
	int nPivot = 0;
	for(int y = 0; y < (m_bIsSingle?5:10); y++)
	{
		if((LOWORD(songc.dwArrows1[(x)*10+y]) >= 1) && (LOWORD(songc.dwArrows1[(x)*10+y])) <= 4 && trac.nArg[x*10+y] != 0)
		{
			nPivot++;
		}
	}
	return nPivot;
}

int CSKSFView::AddCharacterAutoPlay(double dWWArrow, int nArrow, int nType)
{
	int n = crt.hCtr.nSizeArrowsAP;
	crt.hCtr.nSizeArrowsAP++;
	crt.dwArrowsAP = (DWORD*)realloc(crt.dwArrowsAP, crt.hCtr.nSizeArrowsAP*sizeof(DWORD));
	crt.dWWArrowsAP = (double*)realloc(crt.dWWArrowsAP, crt.hCtr.nSizeArrowsAP*sizeof(double));
	crt.dWWArrowsAP[n] = dWWArrow;
	crt.dwArrowsAP[n] = MAKELONG(nArrow, nType);
	return 0;
}

double dJudTiming[] = {0.8,0.05,0.1};
double dJudChaos[] = {0.5,0.2,0.2};
double dJudStrong[] = {0.2,0.4,0.2};

double CSKSFView::EstimateStep(double dWWArrow)
{
	//					<<<<<<<<<<<<<<<<<<<<<<<<<<
	double dMatRange[] = {0.0,0.2,0.4,0.6,0.8,1.0};
	//					*Lost**M***B**Go**Gr***P**
	int nResult = 0;	// 0   1   2   3   4   5
	// ** Modificación de la matriz de acuerdo a las variables
	double dDifLev = m_nLevelSong-m_nLevelCharacter;
	double dDiv = pow(M_E, (-pow((dDifLev + 8)/6.8938921179725 - M_E,2)));
	if(dDifLev < -8.0) dDiv = 0.0;
	if(dDifLev > 10.0) dDiv = 1.0;
	dMatRange[1] = dDiv;
	dMatRange[4] = dDiv*(2-dDiv);
	double dMatHud[] = {0.5,0.2,0.2};
	for(int x = 0; x < 3; x++)
	{
		switch(m_nTypeCharacter)
		{
		case 0:
			dMatHud[x] = dJudTiming[x];
			break;
		case 1:
			dMatHud[x] = dJudChaos[x];
			break;
		case 2:
			dMatHud[x] = dJudStrong[x];
			break;
		}
	}
	dMatRange[0] = dMatHud[0]*dDiv;
	dMatRange[2] = dMatHud[1]*(dMatRange[4]-dMatRange[1])+dDiv;
	dMatRange[3] = dMatHud[2]*(dMatRange[4]-dMatRange[1])+dDiv;
	//////////
	// ** Calculo
	double dResult = double(rand())/RAND_MAX;	// [0.0, 1.0]
	for(int x = 0; x < 6; x++)
	{
		if(dMatRange[x] >= dResult)
		{
			nResult = x;
			break;
		}
	}
	if(nResult == 0) return -1.0;
	double dSign = (rand()%2)?1:-1;
	dResult = double(rand())/RAND_MAX;
	double dLine = 0.0;
	if(nResult == 1) dLine = m_dLevelMach*(1.0+0.2*dResult);
	else
	{
		double dPt = 5.5-double(nResult)+(dResult-0.5)*0.999999999;
		dLine = m_dLevelMach*pow(dPt/4, dPowTimer);
	}
	//double dLine = m_dLevelMach*pow(((5-double(nResult))+dResult)/4, dPowTimer);
	//double dLine = m_dLevelMach/4*((5-double(nResult))+dResult);
	/////////////
	return dWWArrow+dSign*dLine;
}

bool CSKSFView::CompileCharacter(void)
{
	// CARGAR EL SISTEMA DE PASOS
	for(int x = 0; x < songc.nSizeArrows1; x++)
	{
		// ** Actualizar el dLevelMach
		for(int y = 0; y < songc.nSizeChangeBPM; y++)
		{
			if(songc.dWArrows1[x]>= songc.dWChangeBPM[y])
				m_dLevelMach = (double)m_nLevelMach*songc.dChangeBPM[y]/60000;
			else break;
		}
		int nInc[] = {0,0};
		for(int y = 0; y < 10; y++)
		{
			if(LOWORD(songc.dwArrows1[x*10+y]) == 1 ||
				LOWORD(songc.dwArrows1[x*10+y]) == 3)
			{
				double dWPress = EstimateStep(songc.dWWArrows1[x]);
				if(dWPress != -1.0)
				{
					AddCharacterAutoPlay(dWPress, y, LOWORD(songc.dwArrows1[x*10+y]));
					switch(trac.nArg[x*10+y])
					{
					case 1:
						if(nInc[0] == 0)
						{
							nInc[0] = 1;
							crt.hCtr.nSizeArrows1++;
							crt.nArrows1 = (int*)realloc(crt.nArrows1, crt.hCtr.nSizeArrows1*10*sizeof(int));
							crt.dWWArrows1 = (double*)realloc(crt.dWWArrows1, crt.hCtr.nSizeArrows1*sizeof(double));
							for(int k = 0; k < 10; k++) crt.nArrows1[(crt.hCtr.nSizeArrows1-1)*10+k] = 0;
						}
						crt.nArrows1[(crt.hCtr.nSizeArrows1-1)*10+y] = LOWORD(songc.dwArrows1[x*10+y]);
						crt.dWWArrows1[crt.hCtr.nSizeArrows1-1] = dWPress;
						break;
					default:
						if(nInc[1] == 0)
						{
							nInc[1] = 1;
							crt.hCtr.nSizeArrows2++;
							crt.nArrows2 = (int*)realloc(crt.nArrows2, crt.hCtr.nSizeArrows2*10*sizeof(int));
							crt.dWWArrows2 = (double*)realloc(crt.dWWArrows2, crt.hCtr.nSizeArrows2*sizeof(double));
							for(int k = 0; k < 10; k++) crt.nArrows2[(crt.hCtr.nSizeArrows2-1)*10+k] = 0;
						}
						crt.nArrows2[(crt.hCtr.nSizeArrows2-1)*10+y] = LOWORD(songc.dwArrows1[x*10+y]);
						crt.dWWArrows2[crt.hCtr.nSizeArrows2-1] = dWPress;
						break;
					}
				}
			}
			if(LOWORD(songc.dwArrows1[x*10+y]) == 4)
			{
				double dWPress = EstimateStep(songc.dWWArrows1[x]);
				if(dWPress != -1.0)
					AddCharacterAutoPlay(dWPress, y, LOWORD(songc.dwArrows1[x*10+y]));
			}
		}
	}
	// CARGAR LA GEOMETRIA
	TCHAR chFile[260];
	_sprintf(chFile, _T("characters/%d/body.ase"), m_nCharacter);
	FILE* hFile = _fopen(chFile, _T("r"));
	if(hFile == NULL) return false;
	TCHAR ch[500];
	// LFoot (Pie Izquierdo)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxLFoot);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceLFoot);
	crt.dVtxLFoot = (double*)realloc(crt.dVtxLFoot, crt.hCtr.nSizeVtxLFoot*3*sizeof(double));
	crt.nFaceLFoot = (int*)realloc(crt.nFaceLFoot, crt.hCtr.nSizeFaceLFoot*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxLFoot; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxLFoot[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceLFoot; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceLFoot[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTLFoot);
	crt.dUVTLFoot = (double*)realloc(crt.dUVTLFoot, crt.hCtr.nSizeUVTLFoot*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTLFoot; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTLFoot[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTLFoot);
	crt.nTLFoot = (int*)realloc(crt.nTLFoot, crt.hCtr.nSizeTLFoot*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTLFoot; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTLFoot[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// RFoot (Pie Derecho)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxRFoot);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceRFoot);
	crt.dVtxRFoot = (double*)realloc(crt.dVtxRFoot, crt.hCtr.nSizeVtxRFoot*3*sizeof(double));
	crt.nFaceRFoot = (int*)realloc(crt.nFaceRFoot, crt.hCtr.nSizeFaceRFoot*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxRFoot; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxRFoot[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceRFoot; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceRFoot[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTRFoot);
	crt.dUVTRFoot = (double*)realloc(crt.dUVTRFoot, crt.hCtr.nSizeUVTRFoot*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTRFoot; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTRFoot[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTRFoot);
	crt.nTRFoot = (int*)realloc(crt.nTRFoot, crt.hCtr.nSizeTRFoot*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTRFoot; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTRFoot[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// DLLeg (Pie Derecho)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxDLLeg);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceDLLeg);
	crt.dVtxDLLeg = (double*)realloc(crt.dVtxDLLeg, crt.hCtr.nSizeVtxDLLeg*3*sizeof(double));
	crt.nFaceDLLeg = (int*)realloc(crt.nFaceDLLeg, crt.hCtr.nSizeFaceDLLeg*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxDLLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxDLLeg[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceDLLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceDLLeg[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTDLLeg);
	crt.dUVTDLLeg = (double*)realloc(crt.dUVTDLLeg, crt.hCtr.nSizeUVTDLLeg*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTDLLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTDLLeg[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTDLLeg);
	crt.nTDLLeg = (int*)realloc(crt.nTDLLeg, crt.hCtr.nSizeTDLLeg*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTDLLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTDLLeg[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// DRLeg (Pie Derecho)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxDRLeg);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceDRLeg);
	crt.dVtxDRLeg = (double*)realloc(crt.dVtxDRLeg, crt.hCtr.nSizeVtxDRLeg*3*sizeof(double));
	crt.nFaceDRLeg = (int*)realloc(crt.nFaceDRLeg, crt.hCtr.nSizeFaceDRLeg*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxDRLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxDRLeg[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceDRLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceDRLeg[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTDRLeg);
	crt.dUVTDRLeg = (double*)realloc(crt.dUVTDRLeg, crt.hCtr.nSizeUVTDRLeg*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTDRLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTDRLeg[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTDRLeg);
	crt.nTDRLeg = (int*)realloc(crt.nTDRLeg, crt.hCtr.nSizeTDRLeg*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTDRLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTDRLeg[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// ULLeg (Pierna Izquierda)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxULLeg);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceULLeg);
	crt.dVtxULLeg = (double*)realloc(crt.dVtxULLeg, crt.hCtr.nSizeVtxULLeg*3*sizeof(double));
	crt.nFaceULLeg = (int*)realloc(crt.nFaceULLeg, crt.hCtr.nSizeFaceULLeg*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxULLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxULLeg[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceULLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceULLeg[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTULLeg);
	crt.dUVTULLeg = (double*)realloc(crt.dUVTULLeg, crt.hCtr.nSizeUVTULLeg*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTULLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTULLeg[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTULLeg);
	crt.nTULLeg = (int*)realloc(crt.nTULLeg, crt.hCtr.nSizeTULLeg*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTULLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTULLeg[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// URLeg (Pierna Derecha)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxURLeg);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceURLeg);
	crt.dVtxURLeg = (double*)realloc(crt.dVtxURLeg, crt.hCtr.nSizeVtxURLeg*3*sizeof(double));
	crt.nFaceURLeg = (int*)realloc(crt.nFaceURLeg, crt.hCtr.nSizeFaceURLeg*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxURLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxURLeg[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceURLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceURLeg[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTURLeg);
	crt.dUVTURLeg = (double*)realloc(crt.dUVTURLeg, crt.hCtr.nSizeUVTURLeg*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTURLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTURLeg[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTURLeg);
	crt.nTURLeg = (int*)realloc(crt.nTURLeg, crt.hCtr.nSizeTURLeg*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTURLeg; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTURLeg[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// Hip (Cadera)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxHip);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceHip);
	crt.dVtxHip = (double*)realloc(crt.dVtxHip, crt.hCtr.nSizeVtxHip*3*sizeof(double));
	crt.nFaceHip = (int*)realloc(crt.nFaceHip, crt.hCtr.nSizeFaceHip*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxHip; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxHip[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceHip; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceHip[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTHip);
	crt.dUVTHip = (double*)realloc(crt.dUVTHip, crt.hCtr.nSizeUVTHip*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTHip; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTHip[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTHip);
	crt.nTHip = (int*)realloc(crt.nTHip, crt.hCtr.nSizeTHip*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTHip; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTHip[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// Waist (Cintura)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxWaist);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceWaist);
	crt.dVtxWaist = (double*)realloc(crt.dVtxWaist, crt.hCtr.nSizeVtxWaist*3*sizeof(double));
	crt.nFaceWaist = (int*)realloc(crt.nFaceWaist, crt.hCtr.nSizeFaceWaist*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxWaist; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxWaist[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceWaist; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceWaist[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTWaist);
	crt.dUVTWaist = (double*)realloc(crt.dUVTWaist, crt.hCtr.nSizeUVTWaist*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTWaist; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTWaist[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTWaist);
	crt.nTWaist = (int*)realloc(crt.nTWaist, crt.hCtr.nSizeTWaist*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTWaist; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTWaist[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// Chest (Pecho)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxChest);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceChest);
	crt.dVtxChest = (double*)realloc(crt.dVtxChest, crt.hCtr.nSizeVtxChest*3*sizeof(double));
	crt.nFaceChest = (int*)realloc(crt.nFaceChest, crt.hCtr.nSizeFaceChest*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxChest; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxChest[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceChest; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceChest[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTChest);
	crt.dUVTChest = (double*)realloc(crt.dUVTChest, crt.hCtr.nSizeUVTChest*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTChest; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTChest[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTChest);
	crt.nTChest = (int*)realloc(crt.nTChest, crt.hCtr.nSizeTChest*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTChest; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTChest[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// Head (Cabeza)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxHead);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceHead);
	crt.dVtxHead = (double*)realloc(crt.dVtxHead, crt.hCtr.nSizeVtxHead*3*sizeof(double));
	crt.nFaceHead = (int*)realloc(crt.nFaceHead, crt.hCtr.nSizeFaceHead*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxHead; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxHead[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceHead; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceHead[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTHead);
	crt.dUVTHead = (double*)realloc(crt.dUVTHead, crt.hCtr.nSizeUVTHead*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTHead; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTHead[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTHead);
	crt.nTHead = (int*)realloc(crt.nTHead, crt.hCtr.nSizeTHead*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTHead; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTHead[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// ULArm (Antebrazo Izquierdo)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxULArm);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceULArm);
	crt.dVtxULArm = (double*)realloc(crt.dVtxULArm, crt.hCtr.nSizeVtxULArm*3*sizeof(double));
	crt.nFaceULArm = (int*)realloc(crt.nFaceULArm, crt.hCtr.nSizeFaceULArm*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxULArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxULArm[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceULArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceULArm[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTULArm);
	crt.dUVTULArm = (double*)realloc(crt.dUVTULArm, crt.hCtr.nSizeUVTULArm*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTULArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTULArm[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTULArm);
	crt.nTULArm = (int*)realloc(crt.nTULArm, crt.hCtr.nSizeTULArm*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTULArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTULArm[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// DLArm (Brazo Izquierdo)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxDLArm);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceDLArm);
	crt.dVtxDLArm = (double*)realloc(crt.dVtxDLArm, crt.hCtr.nSizeVtxDLArm*3*sizeof(double));
	crt.nFaceDLArm = (int*)realloc(crt.nFaceDLArm, crt.hCtr.nSizeFaceDLArm*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxDLArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxDLArm[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceDLArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceDLArm[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTDLArm);
	crt.dUVTDLArm = (double*)realloc(crt.dUVTDLArm, crt.hCtr.nSizeUVTDLArm*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTDLArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTDLArm[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTDLArm);
	crt.nTDLArm = (int*)realloc(crt.nTDLArm, crt.hCtr.nSizeTDLArm*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTDLArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTDLArm[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// LHand (Mano Izquierda)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxLHand);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceLHand);
	crt.dVtxLHand = (double*)realloc(crt.dVtxLHand, crt.hCtr.nSizeVtxLHand*3*sizeof(double));
	crt.nFaceLHand = (int*)realloc(crt.nFaceLHand, crt.hCtr.nSizeFaceLHand*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxLHand; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxLHand[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceLHand; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceLHand[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTLHand);
	crt.dUVTLHand = (double*)realloc(crt.dUVTLHand, crt.hCtr.nSizeUVTLHand*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTLHand; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTLHand[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTLHand);
	crt.nTLHand = (int*)realloc(crt.nTLHand, crt.hCtr.nSizeTLHand*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTLHand; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTLHand[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// URArm (Antebrazo Derecho)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxURArm);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceURArm);
	crt.dVtxURArm = (double*)realloc(crt.dVtxURArm, crt.hCtr.nSizeVtxURArm*3*sizeof(double));
	crt.nFaceURArm = (int*)realloc(crt.nFaceURArm, crt.hCtr.nSizeFaceURArm*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxURArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxURArm[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceURArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceURArm[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTURArm);
	crt.dUVTURArm = (double*)realloc(crt.dUVTURArm, crt.hCtr.nSizeUVTURArm*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTURArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTURArm[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTURArm);
	crt.nTURArm = (int*)realloc(crt.nTURArm, crt.hCtr.nSizeTURArm*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTURArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTURArm[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// DRArm (Cabeza)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxDRArm);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceDRArm);
	crt.dVtxDRArm = (double*)realloc(crt.dVtxDRArm, crt.hCtr.nSizeVtxDRArm*3*sizeof(double));
	crt.nFaceDRArm = (int*)realloc(crt.nFaceDRArm, crt.hCtr.nSizeFaceDRArm*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxDRArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxDRArm[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceDRArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceDRArm[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTDRArm);
	crt.dUVTDRArm = (double*)realloc(crt.dUVTDRArm, crt.hCtr.nSizeUVTDRArm*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTDRArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTDRArm[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTDRArm);
	crt.nTDRArm = (int*)realloc(crt.nTDRArm, crt.hCtr.nSizeTDRArm*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTDRArm; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTDRArm[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	// RHand (Cabeza)
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMVERTEX %d"), &crt.hCtr.nSizeVtxRHand);
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMFACES %d"), &crt.hCtr.nSizeFaceRHand);
	crt.dVtxRHand = (double*)realloc(crt.dVtxRHand, crt.hCtr.nSizeVtxRHand*3*sizeof(double));
	crt.nFaceRHand = (int*)realloc(crt.nFaceRHand, crt.hCtr.nSizeFaceRHand*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeVtxRHand; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_VERTEX %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dVtxRHand[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	for(int x = 0; x < crt.hCtr.nSizeFaceRHand; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			int a[3];
			if(_sscanf(ch, _T("*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d"), &n, &f[0], &f[1], &f[2], &a[0], &a[1], &a[2]) != 7) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nFaceRHand[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVERTEX %d"), &crt.hCtr.nSizeUVTRHand);
	crt.dUVTRHand = (double*)realloc(crt.dUVTRHand, crt.hCtr.nSizeUVTRHand*3*sizeof(double));
	for(int x = 0; x < crt.hCtr.nSizeUVTRHand; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			double v[3];
			if(_sscanf(ch, _T("*MESH_TVERT %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.dUVTRHand[n*3+z] = v[z];
		}
		else {fclose(hFile); return false;}
	}
	_fgets(ch, 500, hFile);
	_sscanf(ch, _T("*MESH_NUMTVFACES %d"), &crt.hCtr.nSizeTRHand);
	crt.nTRHand = (int*)realloc(crt.nTRHand, crt.hCtr.nSizeTRHand*3*sizeof(int));
	for(int x = 0; x < crt.hCtr.nSizeTRHand; x++)
	{
		if(_fgets(ch, 500, hFile) != NULL)
		{
			int n;
			int f[3];
			if(_sscanf(ch, _T("*MESH_TFACE %d\t%d\t%d\t%d"), &n, &f[0], &f[1], &f[2]) != 4) {fclose(hFile); return false;}
			for(int z = 0; z < 3; z++) crt.nTRHand[n*3+z] = f[z];
		}
		else {fclose(hFile); return false;}
	}

	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_DLLeg %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalDLLeg[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_DRLeg %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalDRLeg[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_ULLeg %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalULLeg[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_URLeg %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalURLeg[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_Hip %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalHip[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_Waist %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalWaist[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_Chest %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalChest[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_Head %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalHead[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_ULArm %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalULArm[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_DLArm %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalDLArm[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_URArm %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalURArm[n*3+z] = v[z];
	}
	for(int y = 0; y < 2; y++)
	{
		_fgets(ch, 500, hFile);
		int n = 0;
		double v[3];
		if(_sscanf(ch, _T("*NORMAL_DRArm %d\t%lg\t%lg\t%lg"), &n, &v[0], &v[1], &v[2])!=4) {fclose(hFile); return false;}
		for(int z = 0; z < 3; z++) crt.hCtr.dNormalDRArm[n*3+z] = v[z];
	}

	for(int x = 0; x < 16; x++)
	{
		_fgets(ch, 500, hFile);
		CString str;
		str.Format(_T("Characters/%d/%ls"), m_nCharacter, ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		crt.nTextures[x] = GenTexImage(str);
	}

	fclose(hFile);
	return true;
}

void CSKSFView::DeleteCharacter(void)
{
	glDeleteTextures(16, crt.nTextures);
	memset(&crt.hCtr, 0, sizeof(CHARACTER_HEADER));
	crt.nArrows1 = (int*)realloc(crt.nArrows1, crt.hCtr.nSizeArrows1*10*sizeof(int));
	crt.nArrows2 = (int*)realloc(crt.nArrows2, crt.hCtr.nSizeArrows2*10*sizeof(int));
	crt.dWWArrows1 = (double*)realloc(crt.dWWArrows1, crt.hCtr.nSizeArrows1*sizeof(double));
	crt.dWWArrows2 = (double*)realloc(crt.dWWArrows2, crt.hCtr.nSizeArrows2*sizeof(double));
	crt.dVtxLFoot = (double*)realloc(crt.dVtxLFoot, crt.hCtr.nSizeVtxLFoot*sizeof(double));
	crt.dVtxRFoot = (double*)realloc(crt.dVtxRFoot, crt.hCtr.nSizeVtxRFoot*sizeof(double));
	crt.nFaceLFoot = (int*)realloc(crt.nFaceLFoot, crt.hCtr.nSizeFaceLFoot*sizeof(int));
	crt.nFaceRFoot = (int*)realloc(crt.nFaceRFoot, crt.hCtr.nSizeFaceRFoot*sizeof(int));
	crt.dUVTLFoot = (double*)realloc(crt.dUVTLFoot, crt.hCtr.nSizeUVTLFoot*sizeof(double));
	crt.dUVTRFoot = (double*)realloc(crt.dUVTRFoot, crt.hCtr.nSizeUVTRFoot*sizeof(double));
	crt.nTLFoot = (int*)realloc(crt.nTLFoot, crt.hCtr.nSizeTLFoot*sizeof(int));
	crt.nTRFoot = (int*)realloc(crt.nTRFoot, crt.hCtr.nSizeTRFoot*sizeof(int));

	crt.dVtxDLLeg = (double*)realloc(crt.dVtxDLLeg, crt.hCtr.nSizeVtxDLLeg*sizeof(double));
	crt.nFaceDLLeg = (int*)realloc(crt.nFaceDLLeg, crt.hCtr.nSizeFaceDLLeg*sizeof(int));
	crt.dUVTDLLeg = (double*)realloc(crt.dUVTDLLeg, crt.hCtr.nSizeUVTDLLeg*sizeof(double));
	crt.nTDLLeg = (int*)realloc(crt.nTDLLeg, crt.hCtr.nSizeTDLLeg*sizeof(int));

	crt.dVtxDRLeg = (double*)realloc(crt.dVtxDRLeg, crt.hCtr.nSizeVtxDRLeg*sizeof(double));
	crt.nFaceDRLeg = (int*)realloc(crt.nFaceDRLeg, crt.hCtr.nSizeFaceDRLeg*sizeof(int));
	crt.dUVTDRLeg = (double*)realloc(crt.dUVTDRLeg, crt.hCtr.nSizeUVTDRLeg*sizeof(double));
	crt.nTDRLeg = (int*)realloc(crt.nTDRLeg, crt.hCtr.nSizeTDRLeg*sizeof(int));

	crt.dVtxULLeg = (double*)realloc(crt.dVtxULLeg, crt.hCtr.nSizeVtxULLeg*sizeof(double));
	crt.nFaceULLeg = (int*)realloc(crt.nFaceULLeg, crt.hCtr.nSizeFaceULLeg*sizeof(int));
	crt.dUVTULLeg = (double*)realloc(crt.dUVTULLeg, crt.hCtr.nSizeUVTULLeg*sizeof(double));
	crt.nTULLeg = (int*)realloc(crt.nTULLeg, crt.hCtr.nSizeTULLeg*sizeof(int));

	crt.dVtxURLeg = (double*)realloc(crt.dVtxURLeg, crt.hCtr.nSizeVtxURLeg*sizeof(double));
	crt.nFaceURLeg = (int*)realloc(crt.nFaceURLeg, crt.hCtr.nSizeFaceURLeg*sizeof(int));
	crt.dUVTURLeg = (double*)realloc(crt.dUVTURLeg, crt.hCtr.nSizeUVTURLeg*sizeof(double));
	crt.nTURLeg = (int*)realloc(crt.nTURLeg, crt.hCtr.nSizeTURLeg*sizeof(int));

	crt.dVtxHip = (double*)realloc(crt.dVtxHip, crt.hCtr.nSizeVtxHip*sizeof(double));
	crt.nFaceHip = (int*)realloc(crt.nFaceHip, crt.hCtr.nSizeFaceHip*sizeof(int));
	crt.dUVTHip = (double*)realloc(crt.dUVTHip, crt.hCtr.nSizeUVTHip*sizeof(double));
	crt.nTHip = (int*)realloc(crt.nTHip, crt.hCtr.nSizeTHip*sizeof(int));

	crt.dVtxWaist = (double*)realloc(crt.dVtxWaist, crt.hCtr.nSizeVtxWaist*sizeof(double));
	crt.nFaceWaist = (int*)realloc(crt.nFaceWaist, crt.hCtr.nSizeFaceWaist*sizeof(int));
	crt.dUVTWaist = (double*)realloc(crt.dUVTWaist, crt.hCtr.nSizeUVTWaist*sizeof(double));
	crt.nTWaist = (int*)realloc(crt.nTWaist, crt.hCtr.nSizeTWaist*sizeof(int));

	crt.dVtxChest = (double*)realloc(crt.dVtxChest, crt.hCtr.nSizeVtxChest*sizeof(double));
	crt.nFaceChest = (int*)realloc(crt.nFaceChest, crt.hCtr.nSizeFaceChest*sizeof(int));
	crt.dUVTChest = (double*)realloc(crt.dUVTChest, crt.hCtr.nSizeUVTChest*sizeof(double));
	crt.nTChest = (int*)realloc(crt.nTChest, crt.hCtr.nSizeTChest*sizeof(int));

	crt.dVtxHead = (double*)realloc(crt.dVtxHead, crt.hCtr.nSizeVtxHead*sizeof(double));
	crt.nFaceHead = (int*)realloc(crt.nFaceHead, crt.hCtr.nSizeFaceHead*sizeof(int));
	crt.dUVTHead = (double*)realloc(crt.dUVTHead, crt.hCtr.nSizeUVTHead*sizeof(double));
	crt.nTHead = (int*)realloc(crt.nTHead, crt.hCtr.nSizeTHead*sizeof(int));

	crt.dVtxULArm = (double*)realloc(crt.dVtxULArm, crt.hCtr.nSizeVtxULArm*sizeof(double));
	crt.nFaceULArm = (int*)realloc(crt.nFaceULArm, crt.hCtr.nSizeFaceULArm*sizeof(int));
	crt.dUVTULArm = (double*)realloc(crt.dUVTULArm, crt.hCtr.nSizeUVTULArm*sizeof(double));
	crt.nTULArm = (int*)realloc(crt.nTULArm, crt.hCtr.nSizeTULArm*sizeof(int));

	crt.dVtxDLArm = (double*)realloc(crt.dVtxDLArm, crt.hCtr.nSizeVtxDLArm*sizeof(double));
	crt.nFaceDLArm = (int*)realloc(crt.nFaceDLArm, crt.hCtr.nSizeFaceDLArm*sizeof(int));
	crt.dUVTDLArm = (double*)realloc(crt.dUVTDLArm, crt.hCtr.nSizeUVTDLArm*sizeof(double));
	crt.nTDLArm = (int*)realloc(crt.nTDLArm, crt.hCtr.nSizeTDLArm*sizeof(int));

	crt.dVtxLHand = (double*)realloc(crt.dVtxLHand, crt.hCtr.nSizeVtxLHand*sizeof(double));
	crt.nFaceLHand = (int*)realloc(crt.nFaceLHand, crt.hCtr.nSizeFaceLHand*sizeof(int));
	crt.dUVTLHand = (double*)realloc(crt.dUVTLHand, crt.hCtr.nSizeUVTLHand*sizeof(double));
	crt.nTLHand = (int*)realloc(crt.nTLHand, crt.hCtr.nSizeTLHand*sizeof(int));

	crt.dVtxURArm = (double*)realloc(crt.dVtxURArm, crt.hCtr.nSizeVtxURArm*sizeof(double));
	crt.nFaceURArm = (int*)realloc(crt.nFaceURArm, crt.hCtr.nSizeFaceURArm*sizeof(int));
	crt.dUVTURArm = (double*)realloc(crt.dUVTURArm, crt.hCtr.nSizeUVTURArm*sizeof(double));
	crt.nTURArm = (int*)realloc(crt.nTURArm, crt.hCtr.nSizeTURArm*sizeof(int));

	crt.dVtxDRArm = (double*)realloc(crt.dVtxDRArm, crt.hCtr.nSizeVtxDRArm*sizeof(double));
	crt.nFaceDRArm = (int*)realloc(crt.nFaceDRArm, crt.hCtr.nSizeFaceDRArm*sizeof(int));
	crt.dUVTDRArm = (double*)realloc(crt.dUVTDRArm, crt.hCtr.nSizeUVTDRArm*sizeof(double));
	crt.nTDRArm = (int*)realloc(crt.nTDRArm, crt.hCtr.nSizeTDRArm*sizeof(int));

	crt.dVtxRHand = (double*)realloc(crt.dVtxRHand, crt.hCtr.nSizeVtxRHand*sizeof(double));
	crt.nFaceRHand = (int*)realloc(crt.nFaceRHand, crt.hCtr.nSizeFaceRHand*sizeof(int));
	crt.dUVTRHand = (double*)realloc(crt.dUVTRHand, crt.hCtr.nSizeUVTRHand*sizeof(double));
	crt.nTRHand = (int*)realloc(crt.nTRHand, crt.hCtr.nSizeTRHand*sizeof(int));

	crt.dwArrowsAP = (DWORD*)realloc(crt.dwArrowsAP, crt.hCtr.nSizeArrowsAP*sizeof(DWORD));
	crt.dWWArrowsAP = (double*)realloc(crt.dWWArrowsAP, crt.hCtr.nSizeArrowsAP*sizeof(double));
}

__int64 CSKSFView::BeatToTime(double dBeat)
{
	if(dBeat < 0) return (__int64)(dBeat*60000/songc.dChangeBPM[0]);
	__int64 nMil = 0;
	double dSBeat = dBeat;
	int x;
	for(x = 0; x < songc.nSizeChangeBPM; x++)
	{
		__int64 dSMil = nMil+(__int64)(dSBeat*60000/songc.dChangeBPM[x]);
		__int64 nOffset = 0;
		double dMinB = songc.dWChangeBPM[x];
		double dMaxB = (x+1) >= songc.nSizeChangeBPM?dBeat:min(songc.dWChangeBPM[x+1],dBeat);
		for(int y = 0; y < songc.nSizeJumps; y++)
		{
			if(songc.dIJumps[y] >= dMinB && songc.dIJumps[y] < dMaxB)
			{
				nOffset-=(__int64)((songc.dEJumps[y]-songc.dIJumps[y])*60000/songc.dChangeBPM[x]);
			}
		}
		for(int y = 0; y < songc.nSizeStops; y++)
		{
			if(songc.dWStops[y] >= dMinB && songc.dWStops[y] < dMaxB)
			{
				nOffset+=(__int64)((songc.dStops[y])*60000/songc.dChangeBPM[x]);
			}
		}
		if((x+1) >= songc.nSizeChangeBPM)
		{
			return dSMil+nOffset; // No existen mas cambios de BPM
		}
		else
		{
			if(songc.dWChangeBPM[x+1] > dBeat)
			{
				// El beat actual no ha pasado por este cambio de BPM
				return dSMil+nOffset;
			}
			else
			{
				dSBeat -= songc.dWChangeBPM[x+1]-songc.dWChangeBPM[x];
				nMil += (__int64)((songc.dWChangeBPM[x+1]-songc.dWChangeBPM[x])*60000/songc.dChangeBPM[x]);
				nMil += nOffset;
			}
		}
	}
	return nMil;
}

void CSKSFView::DrawBS01(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	double dOffIQuest = bsc.dWIBSQuestions[y];
	double dOffAQuest = bsc.dWABSQuestions[y];
	double dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nNext;
	///////////////////////////////
	double t=3.5, b=-3.5, l=-40.0, r=40.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		glPushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[15]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo
	{
		glBindTexture(GL_TEXTURE_2D, bs->skin[15]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);
		glEnd();
		CString strDisp = _T("");
		int nSize = 0;
		for(int x = 0; x < 30; x++)
		{
			if(bsc.dwDBSQuestions[y*30+x] == 0) break;
			nSize++;
		}
		bool bMode = false;
		for(int x = 0; x < nSize; x++)
		{
			if(bMode)
			{
				int nNumber = LOWORD(bsc.dwDBSQuestions[y*30+x]);
				switch(nNumber)
				{
				case 10:
					strDisp += _T("+");
					break;
				case 11:
					strDisp += _T("-");
					break;
				case 12:
					strDisp += _T("*");
					break;
				case 13:
					strDisp += _T("/");
					break;
				case 14:
					strDisp += _T("=");
					break;
				case 15:
					strDisp += _T("<");
					break;
				case 16:
					strDisp += _T(">");
					break;
				default:
					strDisp += _T("?");
					break;
				}
			}
			else
			{
				int nNumber = LOWORD(bsc.dwDBSQuestions[y*30+x]);
				if(nNumber > 100 || nNumber < 0) nNumber = 0;
				CString str;
				str.Format(_T("%d"), nNumber);
				strDisp += str;
			}
			if(nType != 1 || (x+2) != nSize) bMode = !bMode;
		}
		int nSizeShow = strDisp.GetLength();
		double nMinL = -1.75*(double)nSizeShow;
		for(int x = 0; x < nSizeShow; x++)
		{
			int nInd = 0;
			TCHAR chCaret = strDisp.GetAt(x);
			if(chCaret >= '0' && chCaret <= '9')
				nInd = chCaret - '0';
			else if(chCaret == '+')
				nInd = 10;
			else if(chCaret == '-')
				nInd = 11;
			else if(chCaret == '*')
				nInd = 12;
			else if(chCaret == '/')
				nInd = 13;
			else if(chCaret == '=')
				nInd = 14;
			else if(chCaret == '<')
				nInd = 15;
			else if(chCaret == '>')
				nInd = 16;
			else nInd = 17;
			t=3.5; b=-3.5; l=nMinL+3.5*x; r=nMinL+3.5*(x+1);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, bs->skin[1]->texture[nInd]);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);
			glEnd();
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=3.5; b=-3.5; l=-40.0; r=40.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBindTexture(GL_TEXTURE_2D, bs->skin[15]->texture[0]);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		glPushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[15]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);
		glEnd();
		glPopMatrix();
	}
}

void CSKSFView::DrawBS02(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	double dOffIQuest = bsc.dWIBSQuestions[y];
	double dOffAQuest = bsc.dWABSQuestions[y];
	double dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nNext;
	///////////////////////////////
	double t=0.0, b=-30.0, l=-20.0, r=20.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		glPushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo
	{
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);
		glEnd();
		for(int x = 0; x < 5; x++)
		{
			if(m_nNext <= bsc.dwDBSQuestionsEx[y*40+25+x] ||
				(m_nNext >= (bsc.dwDBSQuestionsEx[y*40+25+x]+500)
				&& m_nNext <= (bsc.dwDBSQuestionsEx[y*40+25+x]+1000)))
			{
				glPushMatrix();
				switch(x)
				{
				case 0:
					glTranslated(-18.83, -28.239, 0.0);
					break;
				case 4:
					glTranslated(6.276, -28.239, 0.0);
					break;
				case 1:
					glTranslated(-18.83, -10.293, 0.0);
					break;
				case 3:
					glTranslated(6.276, -10.293, 0.0);
					break;
				default:
					glTranslated(-6.277, -19.706, 0.0);
					break;
				}
				int nCount = 0;
				for(int z = 0; z < 4; z++)
				{
					if(bsc.dwDBSQuestionsEx[y*40+4*x+z])
					{
						nCount++;
					}
				}
				for(int z = 0; z < 4; z++)
				{
					if(bsc.dwDBSQuestionsEx[y*40+4*x+z])
					{
						if(nCount == 0)
						{
							b = 0.7065; t = 8.7065; l = 2.2765; r = 10.2765;
						}
						else
						{
							switch(bsc.dwDBSQuestionsEx[y*40+20+x])
							{
							case 0:
								b = 0.7065; t = 8.7065; l = 2.2765; r = 10.2765;
								break;
							case 1:
								switch(z)
								{
								case 0:
									b = 0.471; t = 4.471; l = 1.5176; r = 5.5176;
									break;
								case 1:
									b = 0.471; t = 4.471; l = 7.0352; r = 11.0352;
									break;
								case 2:
									b = 4.942; t = 8.942; l = 1.5176; r = 5.5176;
									break;
								case 3:
									b = 4.942; t = 8.942; l = 7.0352; r = 11.0352;
									break;
								}
								break;
							case 2:
								switch(z)
								{
								case 0:
									b = 0.471; t = 5.471; l = 3.7765; r = 8.7765;
									break;
								case 1:
									b = 0.2065; t = 3.2065; l = 9.2765; r = 12.2765;
									break;
								case 2:
									b = 0.2065; t = 3.2065; l = 0.2765; r = 3.2765;
									break;
								case 3:
									b = 6.2065; t = 9.2065; l = 9.2765; r = 12.2765;
									break;
								}
								break;
							case 3:
								switch(z)
								{
								case 0:
									b = 0.471; t = 5.471; l = 3.7765; r = 8.7765;
									break;
								case 1:
									b = 6.2065; t = 9.2065; l = 9.2765; r = 12.2765;
									break;
								case 2:
									b = 0.2065; t = 3.2065; l = 9.2765; r = 12.2765;
									break;
								case 3:
									b = 6.2065; t = 9.2065; l = 0.2765; r = 3.2765;
									break;
								}
								break;
							}
						}
						int nGroup = HIWORD(bsc.dwDBSQuestionsEx[y*40+x*4+z]);
						int nNumber = LOWORD(bsc.dwDBSQuestionsEx[y*40+x*4+z]);
						glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//x==0?-50.004:-50.005);
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//x==0?-50.004:-50.005);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//x==0?-50.004:-50.005);
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//x==0?-50.004:-50.005);
						glEnd();
						CString str;
						str.Format(_T("%d"), nNumber);
						double dSizeCaret = r-l;
						double dMinL = (l+dSizeCaret/2)-dSizeCaret/4*str.GetLength();
						for(int w = 0; w < str.GetLength(); w++)
						{
							double ll = dMinL+dSizeCaret/2*(double)w;
							double rr = dMinL+dSizeCaret/2*(double)(w+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(w);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							glBindTexture(GL_TEXTURE_2D, bs->skin[nGroup]->texture[nAcc]);
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
							glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//x==0?-50.002:-50.003);
							glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//x==0?-50.002:-50.003);
							glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//x==0?-50.002:-50.003);
							glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//x==0?-50.002:-50.003);
							glEnd();
						}
					}
				}
				glPopMatrix();
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=0.0; b=-30.0; l=-20.0; r=20.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		glPushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);
		glEnd();
		glPopMatrix();
	}
}

void CSKSFView::DrawBS03(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	int nSubject = HIWORD(bsc.dwBSQuestions[y]);
	double dOffIQuest = bsc.dWIBSQuestions[y];
	double dOffAQuest = bsc.dWABSQuestions[y];
	double dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nNext;
	///////////////////////////////
	double t=40.0, b=-20.0, l=-3.5, r=3.5;
	double _t=-20.0, _b=-30.0, _l=-5.0, _r=5.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		glPushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[12]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(0.0, -10.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[17]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(_l, _t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(_r, _t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(_r, _b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(_l, _b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo
	{
		glBindTexture(GL_TEXTURE_2D, bs->skin[12]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, bs->skin[17]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(_l, _t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(_r, _t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(_r, _b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(_l, _b, -50.0);//-50.006);
		glEnd();
		int nSize = LOWORD(bsc.dwDBSQuestionsEx[y*40+1]);
		double dDist = (double)LOWORD(bsc.dwDBSQuestionsEx[y*40]);
		double dSizeObj = 6.0;
		double dDistObj = 7.0;
		double h = ((t-b)+((double)nSize-1)*dDistObj)*dDist;	// La altura del panel + NumerodeItems*Dist entre ellas
		for(int x = 0; x < nSize; x++)
		{
			double dMult = ((double)(nBeat-nOffIQuest))/((double)(nOffAQuest-nOffIQuest));
			b = ((double)x)*dDistObj*dDist;
			b -= h*dMult;
			b += 30.0;
			if(b <= 30.0 && b >= -20.0)	// Se encuentra en los límites
			{
				t = b+dSizeObj; l = -3.0; r = l+dSizeObj;
				if(bsc.dwDBSQuestions[y*30+x])
				{
					DWORD dwDArrow = LOWORD(bsc.dwDBSQuestions[y*30+x]);
					DWORD dwGArrow = HIWORD(bsc.dwDBSQuestions[y*30+x]);
					if(dwGArrow == 1)
					{
						glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.005);
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.005);
						glEnd();
					}
					if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
					{
						CString str;
						if(nType == 2 && nSubject == 3 && dwDArrow == 17) str = _T("?");
						else str.Format(_T("%d"), dwDArrow);
						double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
						for(int z = 0; z < str.GetLength(); z++)
						{
							double ll = dMinL+dSizeObj/2*(double)z;
							double rr = dMinL+dSizeObj/2*(double)(z+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(z);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[nAcc]);
							glBegin(GL_QUADS);
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.003);
							glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.003);
							glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.004);
							glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.004);
							glEnd();
						}
					}
					else
					{
						/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
						double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
						glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[dwDArrow]);
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
						glEnd();
					}
				}
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=40.0; b=-20.0; l=-3.5; r=3.5;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBindTexture(GL_TEXTURE_2D, bs->skin[12]->texture[0]);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		glPushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[12]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslated(0.0, -10.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[17]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(_l, _t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(_r, _t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(_r, _b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(_l, _b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

void CSKSFView::DrawBS04(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	double dOffIQuest = bsc.dWIBSQuestions[y];
	double dOffAQuest = bsc.dWABSQuestions[y];
	double dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nNext;
	///////////////////////////////
	double t=0.0, b=-30.0, l=-20.0, r=20.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		glPushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffAQuest - nBeat) > 0)	// Nudo
	{
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		// Dibujo principal (mostrar todo)
		for(int x = 0; x < 9; x++)
		{
			b = 0.7065; t = 8.7065; l = 2.2765; r = 10.2765;
			double dSizeObj = r-l;
			glPushMatrix();
			switch(x)
			{
			case 0:
				glTranslated(-18.83, -10.293, 0.0);
				break;
			case 1:
				glTranslated(-6.277, -10.293, 0.0);
				break;
			case 2:
				glTranslated(6.276, -10.293, 0.0);
				break;
			case 3:
				glTranslated(-18.83, -19.706, 0.0);
				break;
			case 4:
				glTranslated(-6.277, -19.706, 0.0);
				break;
			case 5:
				glTranslated(6.276, -19.706, 0.0);
				break;
			case 6:
				glTranslated(-18.83, -28.239, 0.0);
				break;
			case 7:
				glTranslated(-6.277, -28.239, 0.0);
				break;
			case 8:
				glTranslated(6.276, -28.239, 0.0);
				break;
			}
			if(bsc.dwDBSQuestionsEx[y*40+x])
			{
				DWORD dwDArrow = LOWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				DWORD dwGArrow = HIWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				if(dwGArrow == 1)
				{
					glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t,-50.0);// -50.004);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b,-50.0);// -50.005);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.005);
					glEnd();
				}
				if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
				{
					CString str;
					str.Format(_T("%d"), dwDArrow);
					double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
					for(int z = 0; z < str.GetLength(); z++)
					{
						double ll = dMinL+dSizeObj/2*(double)z;
						double rr = dMinL+dSizeObj/2*(double)(z+1);
						int nAcc = 0;
						TCHAR chAcc = str.GetAt(z);
						if(chAcc >= '0' && chAcc <= '9')
							nAcc = chAcc - '0';
						else nAcc = 17;
						glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[nAcc]);
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.004);
						glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.004);
						glEnd();
					}
				}
				else
				{
					/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
					double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
					glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[dwDArrow]);
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.003);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.003);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
					glEnd();
				}
			}
			glPopMatrix();
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=0.0; b=-30.0; l=-20.0; r=20.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffAQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo 2
	{
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		// Dibujo secundario (mostrar de aceurdo al display)
		for(int x = 0; x < 9; x++)
		{
			if(m_nNext >= bsc.dwDBSQuestionsEx[y*40+10+x] &&
				m_nNext <= bsc.dwDBSQuestionsEx[y*40+20+x])
			{
				b = 0.7065; t = 8.7065; l = 2.2765; r = 10.2765;
				double dSizeObj = r-l;
				glPushMatrix();
				switch(x)
				{
				case 0:
					glTranslated(-18.83, -10.293, 0.0);
					break;
				case 1:
					glTranslated(-6.277, -10.293, 0.0);
					break;
				case 2:
					glTranslated(6.276, -10.293, 0.0);
					break;
				case 3:
					glTranslated(-18.83, -19.706, 0.0);
					break;
				case 4:
					glTranslated(-6.277, -19.706, 0.0);
					break;
				case 5:
					glTranslated(6.276, -19.706, 0.0);
					break;
				case 6:
					glTranslated(-18.83, -28.239, 0.0);
					break;
				case 7:
					glTranslated(-6.277, -28.239, 0.0);
					break;
				case 8:
					glTranslated(6.276, -28.239, 0.0);
					break;
				}
				if(bsc.dwDBSQuestionsEx[y*40+x])
				{
					DWORD dwDArrow = LOWORD(bsc.dwDBSQuestionsEx[y*40+x]);
					DWORD dwGArrow = HIWORD(bsc.dwDBSQuestionsEx[y*40+x]);
					if(dwGArrow == 1)
					{
						glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.005);
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.005);
						glEnd();
					}
					if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
					{
						CString str;
						str.Format(_T("%d"), dwDArrow);
						double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
						for(int z = 0; z < str.GetLength(); z++)
						{
							double ll = dMinL+dSizeObj/2*(double)z;
							double rr = dMinL+dSizeObj/2*(double)(z+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(z);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[nAcc]);
							glBegin(GL_QUADS);
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.003);
							glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.003);
							glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.004);
							glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.004);
							glEnd();
						}
					}
					else
					{
						/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
						double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
						glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[dwDArrow]);
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
						glEnd();
					}
				}
				glPopMatrix();
			}
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// End
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		glPushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t,-50.0);// -50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

void CSKSFView::DrawBS05(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	double dOffIQuest = bsc.dWIBSQuestions[y];
	double dOffAQuest = bsc.dWABSQuestions[y];
	double dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nNext;
	///////////////////////////////
	double t=0.0, b=-30.0, l=-20.0, r=20.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		t=0.0; b=-30.0; l=-20.0; r=20.0;
		glPushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
		t=60.0; b=0.0; l=-3.5; r=3.5;
		glPushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[12]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffAQuest - nBeat) > 0)	// Nudo
	{
		t=0.0; b=-30.0; l=-20.0; r=20.0;
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		t=60.0; b=0.0; l=-3.5; r=3.5;
		glBindTexture(GL_TEXTURE_2D, bs->skin[12]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		int nSize = 5;
		double dSizeObj = 6.0;
		double dDistObj = 7.0;
		double h = ((40)+((double)nSize-1)*dDistObj);	// La altura del panel + NumerodeItems*Dist entre ellas
		for(int x = 0; x < nSize; x++)
		{
			double dMult = ((double)(nBeat-nOffIQuest))/((double)(nOffAQuest-nOffIQuest));
			b = ((double)x)*dDistObj;
			b -= h*dMult;
			b += 30.0;
			if(b <=40.0 && b >= 0.0)	// Se encuentra en los límites
			{
				t = b+dSizeObj; l = -3.0; r = l+dSizeObj;
				if(bsc.dwDBSQuestions[y*30+x])
				{
					DWORD dwDArrow = LOWORD(bsc.dwDBSQuestions[y*30+x]);
					DWORD dwGArrow = HIWORD(bsc.dwDBSQuestions[y*30+x]);
					if(dwGArrow == 1)
					{
						glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.005);
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.005);
						glEnd();
					}
					if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
					{
						CString str;
						str.Format(_T("%d"), dwDArrow);
						double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
						for(int z = 0; z < str.GetLength(); z++)
						{
							double ll = dMinL+dSizeObj/2*(double)z;
							double rr = dMinL+dSizeObj/2*(double)(z+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(z);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[nAcc]);
							glBegin(GL_QUADS);
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.003);
							glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.003);
							glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.004);
							glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.004);
							glEnd();
						}
					}
					else
					{
						/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
						double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
						glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[dwDArrow]);
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
						glEnd();
					}
				}
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=0.0; b=-30.0; l=-20.0; r=20.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
			glEnd();
			glPopMatrix();
			t=60.0; b=0.0; l=-3.5; r=3.5;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBindTexture(GL_TEXTURE_2D, bs->skin[12]->texture[0]);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.003);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.003);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.003);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.003);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffAQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo 2
	{
		t=0.0; b=-30.0; l=-20.0; r=20.0;
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		t=60.0; b=0.0; l=-3.5; r=3.5;
		glBindTexture(GL_TEXTURE_2D, bs->skin[12]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		// Dibujo secundario (mostrar de aceurdo al display)
		for(int x = 0; x < 9; x++)
		{
			if(m_nNext <= bsc.dwDBSQuestionsEx[y*40+5+x] ||
				(m_nNext >= (bsc.dwDBSQuestionsEx[y*40+5+x]+500)
				&& m_nNext <= (bsc.dwDBSQuestionsEx[y*40+5+x]+1000)))
			{
				b = 0.7065; t = 8.7065; l = 2.2765; r = 10.2765;
				double dSizeObj = r-l;
				glPushMatrix();
				switch(x)
				{
				case 0:
					glTranslated(-18.83, -28.239, 0.0);
					break;
				case 4:
					glTranslated(6.276, -28.239, 0.0);
					break;
				case 1:
					glTranslated(-18.83, -10.293, 0.0);
					break;
				case 3:
					glTranslated(6.276, -10.293, 0.0);
					break;
				default:
					glTranslated(-6.277, -19.706, 0.0);
					break;
				}
				if(bsc.dwDBSQuestionsEx[y*40+x])
				{
					DWORD dwDArrow = LOWORD(bsc.dwDBSQuestionsEx[y*40+x]);
					DWORD dwGArrow = HIWORD(bsc.dwDBSQuestionsEx[y*40+x]);
					if(dwGArrow == 1)
					{
						glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.005);
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.005);
						glEnd();
					}
					if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
					{
						CString str;
						str.Format(_T("%d"), dwDArrow);
						double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
						for(int z = 0; z < str.GetLength(); z++)
						{
							double ll = dMinL+dSizeObj/2*(double)z;
							double rr = dMinL+dSizeObj/2*(double)(z+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(z);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[nAcc]);
							glBegin(GL_QUADS);
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.003);
							glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.003);
							glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.004);
							glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.004);
							glEnd();
						}
					}
					else
					{
						/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
						double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
						glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[dwDArrow]);
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
						glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
						glEnd();
					}
				}
				glPopMatrix();
			}
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// End
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		t=0.0; b=-30.0; l=-20.0; r=20.0;
		glPushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
		t=60.0; b=0.0; l=-3.5; r=3.5;
		glPushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[12]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

void CSKSFView::DrawBS06(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	int nSubject = HIWORD(bsc.dwBSQuestions[y]);
	double dOffIQuest = bsc.dWIBSQuestions[y];
	double dOffAQuest = bsc.dWABSQuestions[y];
	double dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nNext;
	///////////////////////////////
	double t=0.0, b=-30.0, l=-20.0, r=20.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		glPushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffAQuest - nBeat) > 0)	// Nudo
	{
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		double dSizeObj = 4.0;
		double dDistObjX = 6.2766;
		double dDistObjY = 4.7066;
		for(int x = 0; x < 36; x++)
		{
			int ox = x%6;
			int oy = x/6;
			if(m_nNext >= bsc.dwDBSQuestions[y*30+ox] && m_nNext >= bsc.dwDBSQuestions[y*30+oy+6] && bsc.dwDBSQuestionsEx[y*40+x])
			{
				t = -dDistObjY*((double)oy)-1.351; b = t-dSizeObj;
				l = dDistObjX*((double)ox)-17.83; r = l+dSizeObj;
				DWORD dwDArrow = LOWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				DWORD dwGArrow = HIWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				if(dwGArrow == 1)
				{
					glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.005);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.005);
					glEnd();
				}
				if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
				{
					CString str;
					str.Format(_T("%d"), dwDArrow);
					double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
					for(int z = 0; z < str.GetLength(); z++)
					{
						double ll = dMinL+dSizeObj/2*(double)z;
						double rr = dMinL+dSizeObj/2*(double)(z+1);
						int nAcc = 0;
						TCHAR chAcc = str.GetAt(z);
						if(chAcc >= '0' && chAcc <= '9')
							nAcc = chAcc - '0';
						else nAcc = 17;
						glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[nAcc]);
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.004);
						glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.004);
						glEnd();
					}
				}
				else
				{
					/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
					double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
					glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[dwDArrow]);
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.003);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.003);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
					glEnd();
				}
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=0.0; b=-30.0; l=-20.0; r=20.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffAQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo 2
	{
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		double dSizeObj = 4.0;
		double dDistObjX = 6.2766;
		double dDistObjY = 4.7066;
		for(int x = 0; x < 36; x++)
		{
			int ox = x%6;
			int oy = x/6;
			if(m_nNext >= bsc.dwDBSQuestions[y*30+ox] && m_nNext >= bsc.dwDBSQuestions[y*30+oy+6] && (bsc.dwDBSQuestionsEx[y*40+x] == bsc.dwDBSQuestions[y*30+13]))
			{
				t = -dDistObjY*((double)oy)-1.351; b = t-dSizeObj;
				l = dDistObjX*((double)ox)-17.83; r = l+dSizeObj;
				DWORD dwDArrow = 17;//LOWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				DWORD dwGArrow = 1;HIWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				if(dwGArrow == 1)
				{
					glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.005);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.005);
					glEnd();
				}
				if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
				{
					CString str;
					if(nType == 4 && nSubject == 1) str = _T("?");
					else str.Format(_T("%d"), dwDArrow);
					double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
					for(int z = 0; z < str.GetLength(); z++)
					{
						double ll = dMinL+dSizeObj/2*(double)z;
						double rr = dMinL+dSizeObj/2*(double)(z+1);
						int nAcc = 0;
						TCHAR chAcc = str.GetAt(z);
						if(chAcc >= '0' && chAcc <= '9')
							nAcc = chAcc - '0';
						else nAcc = 17;
						glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[nAcc]);
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.003);
						glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.004);
						glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.004);
						glEnd();
					}
				}
				else
				{
					/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
					double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
					glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[dwDArrow]);
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.003);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.003);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
					glEnd();
				}
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=0.0; b=-30.0; l=-20.0; r=20.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		glPushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[13]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

void CSKSFView::DrawBS07(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	int nSubject = HIWORD(bsc.dwBSQuestions[y]);
	double dOffIQuest = bsc.dWIBSQuestions[y];
	double dOffAQuest = bsc.dWABSQuestions[y];
	double dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nNext;
	///////////////////////////////
	double dSizeItem = 2*m_nSizeArrows/3;
	double t=24.0-dSizeItem, b=24.0-dSizeItem*16, l=-dSizeItem, r=0.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		glPushMatrix();
		glTranslated(0.0, 80.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[14]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffAQuest - nBeat) > 0)	// Nudo
	{
		glBindTexture(GL_TEXTURE_2D, bs->skin[14]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		int nSize = bsc.dwDBSQuestionsEx[y*40+30];
		for(int x = 0; x < nSize; x++)
		{
			if(m_nNext >= bsc.dwDBSQuestionsEx[y*40+x])
			{
				t = 24.0-dSizeItem-(dSizeItem*(double)x); b = t-dSizeItem; l=-dSizeItem, r=0.0;
				DWORD dwDArrow = LOWORD(bsc.dwDBSQuestions[y*30+x]);
				DWORD dwGArrow = HIWORD(bsc.dwDBSQuestions[y*30+x]);
				if(dwGArrow == 1)
				{
					glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.005);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.005);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.005);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.005);
					glEnd();
				}
				if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
				{
					CString str;
					str.Format(_T("%d"), dwDArrow);
					double dMinL = (l+dSizeItem/2)-dSizeItem/4*str.GetLength();
					for(int z = 0; z < str.GetLength(); z++)
					{
						double ll = dMinL+dSizeItem/2*(double)z;
						double rr = dMinL+dSizeItem/2*(double)(z+1);
						int nAcc = 0;
						TCHAR chAcc = str.GetAt(z);
						if(chAcc >= '0' && chAcc <= '9')
							nAcc = chAcc - '0';
						else nAcc = 17;
						glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[nAcc]);
						glBegin(GL_QUADS);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.004);
						glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.004);
						glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.004);
						glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.004);
						glEnd();
					}
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[dwDArrow]);
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
					glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
					glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
					glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
					glEnd();
				}
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			t=24.0-m_nSizeArrows; b=24.0-m_nSizeArrows*8; l=-m_nSizeArrows/2; r=0.0;
			glBindTexture(GL_TEXTURE_2D, bs->skin[14]->texture[0]);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.003);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.003);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.003);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.003);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffAQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo
	{
		glBindTexture(GL_TEXTURE_2D, bs->skin[14]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		if(nSubject == 3 && nType == 4)
		{
			t=-23.0; b=-30.0; l=-40.0; r=40.0;
			glBindTexture(GL_TEXTURE_2D, bs->skin[15]->texture[0]);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
			glEnd();
		}
		int nSize = bsc.dwDBSQuestionsEx[y*40+30];
		for(int x = 0; x < nSize; x++)
		{
			t = 24.0-dSizeItem-(dSizeItem*(double)x); b = t-dSizeItem; l=-dSizeItem, r=0.0;
			glBindTexture(GL_TEXTURE_2D, bs->skin[18]->texture[0]);
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.005);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.005);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.005);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.005);
			glEnd();

			if(nSubject == 3 && nType == 4)
			{
				t=-23.0; b=-30.0; double dSizeObj = 7.0;
				CString str;
				int nAng = 0;
				switch(bsc.dwDBSQuestionsEx[y*40+31])
				{
				case 0:
					nAng = 90; break;
				case 1:
					nAng = 180; break;
				default:
					nAng = 270; break;
				}
				str.Format(_T("%d"), nAng);
				double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
				for(int z = 0; z < str.GetLength(); z++)
				{
					double ll = dMinL+dSizeObj/2*(double)z;
					double rr = dMinL+dSizeObj/2*(double)(z+1);
					int nAcc = 0;
					TCHAR chAcc = str.GetAt(z);
					if(chAcc >= '0' && chAcc <= '9')
						nAcc = chAcc - '0';
					else nAcc = 17;
					glBindTexture(GL_TEXTURE_2D, bs->skin[1]->texture[nAcc]);
					glBegin(GL_QUADS);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.003);
					glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.003);
					glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.003);
					glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.003);
					glEnd();
				}
			}
		}
		if((nOffAQuest - nBeat) >= -500 && nSubject == 3 && nType == 4)
		{
			t=-23.0; b=-30.0; l=-40.0; r=40.0;
			double dApGlout = abs((double)nOffAQuest - (double)nBeat)/500;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBindTexture(GL_TEXTURE_2D, bs->skin[15]->texture[0]);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.002);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.002);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.002);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.002);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		glPushMatrix();
		glTranslated(0.0, 80.0*dCarUp, 0.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[14]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

void CSKSFView::DrawBS08(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	int nSubject = HIWORD(bsc.dwBSQuestions[y]);
	double dOffIQuest = bsc.dWIBSQuestions[y];
	double dOffAQuest = bsc.dWABSQuestions[y];
	double dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nNext;
	///////////////////////////////
	double t=15.0, b=-15.0, l=-15.0, r=15.0; double dSizeItem = t-b;
	if((nOffIQuest - nBeat) <= 0 && (nOffAQuest - nBeat) > 0)	// Nudo
	{
		glBindTexture(GL_TEXTURE_2D, bs->skin[19]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		t=7.5; b=-7.5; l=-7.5; r=7.5;
		DWORD dwDArrow = LOWORD(bsc.dwDBSQuestionsEx[y*40]);
		DWORD dwGArrow = HIWORD(bsc.dwDBSQuestionsEx[y*40]);
		if(dwGArrow == 1)
		{
			glBindTexture(GL_TEXTURE_2D, bs->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.005);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.005);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.005);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.005);
			glEnd();
		}
		if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
		{
			CString str;
			str.Format(_T("%d"), dwDArrow);
			double dMinL = (l+dSizeItem/2)-dSizeItem/4*str.GetLength();
			for(int z = 0; z < str.GetLength(); z++)
			{
				double ll = dMinL+dSizeItem/2*(double)z;
				double rr = dMinL+dSizeItem/2*(double)(z+1);
				int nAcc = 0;
				TCHAR chAcc = str.GetAt(z);
				if(chAcc >= '0' && chAcc <= '9')
					nAcc = chAcc - '0';
				else nAcc = 17;
				glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[nAcc]);
				glBegin(GL_QUADS);
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.004);
				glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.004);
				glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.004);
				glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.004);
				glEnd();
			}
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, bs->skin[dwGArrow]->texture[dwDArrow]);
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
			glEnd();
		}
		if(nSubject == 3 && nType == 3)
		{
			t=-15.0; b=-22.0; l=-40.0; r=40.0;
			glBindTexture(GL_TEXTURE_2D, bs->skin[15]->texture[0]);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.004);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.004);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.004);
			glEnd();
			t=-15.0; b=-22.0; double dSizeObj = 7.0;
			CString str;
			int nAng = 0;
			switch(bsc.dwDBSQuestionsEx[y*40+31])
			{
			case 0:
				nAng = 90; break;
			case 1:
				nAng = 180; break;
			default:
				nAng = 270; break;
			}
			str.Format(_T("%d"), nAng);
			double dMinL = (dSizeObj/2)-dSizeObj/4*str.GetLength();
			for(int z = 0; z < str.GetLength(); z++)
			{
				double ll = dMinL+dSizeObj/2*(double)z;
				double rr = dMinL+dSizeObj/2*(double)(z+1);
				int nAcc = 0;
				TCHAR chAcc = str.GetAt(z);
				if(chAcc >= '0' && chAcc <= '9')
					nAcc = chAcc - '0';
				else nAcc = 17;
				glBindTexture(GL_TEXTURE_2D, bs->skin[1]->texture[nAcc]);
				glBegin(GL_QUADS);
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glTexCoord2d(0.0, 1.0); glVertex3d(ll, t, -50.0);//-50.003);
				glTexCoord2d(1.0, 1.0); glVertex3d(rr, t, -50.0);//-50.003);
				glTexCoord2d(1.0, 0.0); glVertex3d(rr, b, -50.0);//-50.003);
				glTexCoord2d(0.0, 0.0); glVertex3d(ll, b, -50.0);//-50.003);
				glEnd();
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			glPushMatrix();
			glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
			t=15.0; b=-15.0; l=-15.0; r=15.0;
			glBindTexture(GL_TEXTURE_2D, bs->skin[19]->texture[0]);
			glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
			glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.003);
			glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.003);
			glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.003);
			glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.003);
			glEnd();
			if(nSubject == 3 && nType == 3)
			{
				t=-15.0; b=-22.0; l=-40.0; r=40.0;
				glScaled(1.0+dApGlout, 1.0+dApGlout, 1.0);
				glColor4d(1.0, 1.0, 1.0, 1-dApGlout);
				glBindTexture(GL_TEXTURE_2D, bs->skin[15]->texture[0]);
				glBegin(GL_POLYGON);
				glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.002);
				glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.002);
				glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.002);
				glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.002);
				glEnd();
			}
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		glPushMatrix();
		glScaled(1.0+dCarUp, 1.0+dCarUp, 1.0);
		glBindTexture(GL_TEXTURE_2D, bs->skin[19]->texture[0]);
		glColor4d(1.0, 1.0, 1.0, 1.0-dCarUp);
		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 1.0); glVertex3d(l, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 1.0); glVertex3d(r, t, -50.0);//-50.006);
		glTexCoord2d(1.0, 0.0); glVertex3d(r, b, -50.0);//-50.006);
		glTexCoord2d(0.0, 0.0); glVertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

int CSKSFView::CompileEffects(void)
{
	for(int n = 0; n < 3; n++)
	{
		int nComp = GetDocument()->song.hStep.nSizeArrows1;
		switch(n)
		{
		case 0:
			nComp = GetDocument()->song.hStep.nSizeArrows1;
			break;
		case 1:
			nComp = GetDocument()->song.hStep.nSizeArrows2;
			break;
		case 2:
			nComp = GetDocument()->song.hStep.nSizeArrows3;
			break;
		}
		for(int y = 0; y < nComp; y++)
		{
			if(GetDocument()->SomeEffect(y, n))
			{
				for(int x = 0; x < 10; x++)
				{
					DWORD dwArrow = GetDocument()->GetArrow(x, y, n);
					DWORD dwType = LOWORD(dwArrow);
					int i = 0;
					switch(dwType)
					{
					case 22:
						i = effect.nSizeVanishChanges[n];
						effect.nSizeVanishChanges[n]++;
						effect.dWVanishChanges[n] = (double*)realloc(effect.dWVanishChanges[n], effect.nSizeVanishChanges[n]*sizeof(double));
						effect.dwVanishChanges[n] = (DWORD*)realloc(effect.dwVanishChanges[n], effect.nSizeVanishChanges[n]*sizeof(DWORD));
						effect.dWVanishChanges[n][i] = GetDocument()->BeatArrow(y);
						effect.dwVanishChanges[n][i] = HIWORD(dwArrow);
						break;
					case 23:
						i = effect.nSizeAttackChanges[n];
						effect.nSizeAttackChanges[n]++;
						effect.dWAttackChanges[n] = (double*)realloc(effect.dWAttackChanges[n], effect.nSizeAttackChanges[n]*sizeof(double));
						effect.dwAttackChanges[n] = (DWORD*)realloc(effect.dwAttackChanges[n], effect.nSizeAttackChanges[n]*sizeof(DWORD));
						effect.dWAttackChanges[n][i] = GetDocument()->BeatArrow(y);
						effect.dwAttackChanges[n][i] = HIWORD(dwArrow);
						break;
					case 24:
						i = effect.nSizeZigZagChanges[n];
						effect.nSizeZigZagChanges[n]++;
						effect.dWZigZagChanges[n] = (double*)realloc(effect.dWZigZagChanges[n], effect.nSizeZigZagChanges[n]*sizeof(double));
						effect.dwZigZagChanges[n] = (DWORD*)realloc(effect.dwZigZagChanges[n], effect.nSizeZigZagChanges[n]*sizeof(DWORD));
						effect.dWZigZagChanges[n][i] = GetDocument()->BeatArrow(y);
						effect.dwZigZagChanges[n][i] = HIWORD(dwArrow);
						break;
					}
				}
			}
		}
	}
	return 1;
}

void CSKSFView::DeleteEffects(void)
{
	for(int x = 0; x < 3; x++)
	{
		effect.nSizeVanishChanges[x] = 0;
		effect.nSizeAttackChanges[x] = 0;
		effect.nSizeZigZagChanges[x] = 0;
		effect.dWVanishChanges[x] = (double*)realloc(effect.dWVanishChanges[x], effect.nSizeVanishChanges[x]*sizeof(double));
		effect.dWAttackChanges[x] = (double*)realloc(effect.dWAttackChanges[x], effect.nSizeAttackChanges[x]*sizeof(double));
		effect.dWZigZagChanges[x] = (double*)realloc(effect.dWZigZagChanges[x], effect.nSizeZigZagChanges[x]*sizeof(double));
		effect.dwVanishChanges[x] = (DWORD*)realloc(effect.dwVanishChanges[x], effect.nSizeVanishChanges[x]*sizeof(DWORD));
		effect.dwAttackChanges[x] = (DWORD*)realloc(effect.dwAttackChanges[x], effect.nSizeAttackChanges[x]*sizeof(DWORD));
		effect.dwZigZagChanges[x] = (DWORD*)realloc(effect.dwZigZagChanges[x], effect.nSizeZigZagChanges[x]*sizeof(DWORD));
	}
}

void CSKSFView::ModifyPosArrows2(double* l, double* t, int x, int y, int nGArrow)
{
	if(nGArrow < 0 || nGArrow > 3) return;
	double dUbi = BeatArrow(y, nGArrow);
	int n = nGArrow;
	if(nGArrow == 3) n = 0;
	for(int r = 0; r < effect.nSizeZigZagChanges[n]; r++)
	{
		double dCar = effect.dWZigZagChanges[n][r];
		bool bTake = true;
		if(dCar > dUbi) bTake = false;
		if((r+1) < effect.nSizeZigZagChanges[n])
		{
			dCar = effect.dWZigZagChanges[n][r+1];
			if(dCar <= dUbi) bTake = false;
		}
		if(bTake)
		{
			// Modificar la posicion de las flechas del zig zag
			int nFarrot = min(8, (int)LOWORD(effect.dwZigZagChanges[n][r]));
			int nx = x%5;
			double dt = (*t);
			dt = (m_bDrop?1:-1)*(dt);
			double dMult;
			if(dt > 0)
			{
				switch(nFarrot)
				{
				case 1:
					switch(nx)
					{
					case 0: dMult = 3; break;
					case 1: dMult = 1; break;
					case 2: dMult = -2; break;
					case 3: dMult = 1; break;
					case 4: dMult = -3; break;
					}
					if((dt) > 40.0)
						(*l) += m_nSizeArrows*0.9*dMult;
					if((dt) <= 40.0 && (dt) > 30.0)
					{
						double dMu = ((dt)-30.0)/10.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}
					break;
				case 2:
					switch(nx)
					{
					case 0: dMult = 3; break;
					case 1: dMult = 1; break;
					case 2: dMult = -2; break;
					case 3: dMult = 1; break;
					case 4: dMult = -3; break;
					}
					if((dt) <= 50.0 && (dt) > 40.0)
					{
						double dMu = (50.0-(dt))/10.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}
					if(dt <= 40.0 && dt > 30.0)
						(*l) += m_nSizeArrows*0.9*dMult;
					if((dt) <= 30.0 && (dt) > 20.0)
					{
						double dMu = ((dt)-20.0)/10.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}
					break;
				case 3:
					switch(nx)
					{
					case 0: dMult = 3; break;
					case 1: dMult = 1; break;
					case 2: dMult = -2; break;
					case 3: dMult = 1; break;
					case 4: dMult = -3; break;
					}
					if((dt) <= 20.0 && (dt) > 10.0)
					{
						double dMu = ((dt)-10.0)/10.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}
					if(dt <= 30.0 && dt > 20.0)
						(*l) += m_nSizeArrows*0.9*dMult;
					if((dt) <= 40.0 && (dt) > 30.0)
					{
						double dMu = (40.0-(dt))/10.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}
					if((dt) <= 60.0 && (dt) > 50.0)
					{
						double dMu = ((dt)-50.0)/10.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}
					if(dt <= 70.0 && dt > 60.0)
						(*l) += m_nSizeArrows*0.9*dMult;
					break;
				case 4:
					switch(nx)
					{
					case 0: dMult = 3; break;
					case 1: dMult = 1; break;
					case 2: dMult = -2; break;
					case 3: dMult = 1; break;
					case 4: dMult = -3; break;
					}
					if((dt) <= 15.0 && (dt) > 10.0)
					{
						double dMu = ((dt)-10.0)/5.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}
					if(dt <= 20.0 && dt > 15.0)
						(*l) += m_nSizeArrows*0.9*dMult;
					if((dt) <= 25.0 && (dt) > 20.0)
					{
						double dMu = (25.0-(dt))/5.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}

					if((dt) <= 35.0 && (dt) > 30.0)
					{
						double dMu = ((dt)-30.0)/5.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}
					if(dt <= 40.0 && dt > 35.0)
						(*l) += m_nSizeArrows*0.9*dMult;
					if((dt) <= 45.0 && (dt) > 40.0)
					{
						double dMu = (45.0-(dt))/5.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}

					if((dt) <= 55.0 && (dt) > 50.0)
					{
						double dMu = ((dt)-50.0)/5.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}
					if(dt <= 60.0 && dt > 55.0)
						(*l) += m_nSizeArrows*0.9*dMult;
					if((dt) <= 65.0 && (dt) > 60.0)
					{
						double dMu = (65.0-(dt))/5.0;
						(*l) += m_nSizeArrows*0.9*dMult*dMu;
					}
					break;
				case 5:
					{
						double df[] = {
						0,1,2,3,4,
						1,3,0,4,2,
						2,4,1,3,0,
						3,0,4,1,2,
						4,1,2,0,3,
						1,3,4,2,0,
						0,2,1,4,3
						};
						int nAcc = int(dt/10);
						nAcc = min(6,nAcc);
						dMult = -df[nAcc*5+nx]+df[(nAcc>5?6:(nAcc+1))*5+nx];
						double dPos = df[nAcc*5+nx]-double(nx);
						double dMu = (dt/10)-double(int(dt/10));
						(*l) += dPos*m_nSizeArrows*0.9+dMult*m_nSizeArrows*0.9*dMu;
					}
					break;
				case 6:
					{
						double df[] = {
						0,1,2,3,4,
						3,4,1,2,0,
						4,2,3,0,1,
						2,0,4,1,3,
						1,3,2,0,4,
						0,1,4,2,3,
						4,0,1,2,3
						};
						int nAcc = int(dt/10);
						nAcc = min(6,nAcc);
						dMult = -df[nAcc*5+nx]+df[(nAcc>5?6:(nAcc+1))*5+nx];
						double dPos = df[nAcc*5+nx]-double(nx);
						double dMu = (dt/10)-double(int(dt/10));
						(*l) += dPos*m_nSizeArrows*0.9+dMult*m_nSizeArrows*0.9*dMu;
					}
					break;
				case 7:
					{
						double df[] = {
						0,1,2,3,4,
						1,3,4,2,0,
						4,1,2,0,3,
						3,0,4,1,2,
						2,4,1,3,0,
						1,3,0,4,2,
						0,2,1,4,3
						};
						int nAcc = int(dt/10);
						nAcc = min(6,nAcc);
						dMult = -df[nAcc*5+nx]+df[(nAcc>5?6:(nAcc+1))*5+nx];
						double dPos = df[nAcc*5+nx]-double(nx);
						double dMu = (dt/10)-double(int(dt/10));
						(*l) += dPos*m_nSizeArrows*0.9+dMult*m_nSizeArrows*0.9*dMu;
					}
					break;
				case 8:
					{
						double df[] = {
						0,1,2,3,4,
						4,0,1,2,3,
						1,3,2,0,4,
						0,1,4,2,3,
						2,0,4,1,3,
						4,2,3,0,1,
						3,4,1,2,0
						};
						int nAcc = int(dt/10);
						nAcc = min(6,nAcc);
						dMult = -df[nAcc*5+nx]+df[(nAcc>5?6:(nAcc+1))*5+nx];
						double dPos = df[nAcc*5+nx]-double(nx);
						double dMu = (dt/10)-double(int(dt/10));
						(*l) += dPos*m_nSizeArrows*0.9+dMult*m_nSizeArrows*0.9*dMu;
					}
					break;
				}
				break;
			}
		}
	}
}

void CSKSFView::DeriveOpacityPos2(double t, double* o, int y, int nGArrow)
{
	// En esta funcion se debe hacer el dinamismo entre la primera y la segunda
	if(nGArrow < 0 || nGArrow > 2) return;
	double dUbi = BeatArrow(y, nGArrow);
	int n = nGArrow;
	for(int r = 0; r < effect.nSizeVanishChanges[n]; r++)
	{
		double dCar = effect.dWVanishChanges[n][r];
		bool bTake = true;
		if(dCar > dUbi) bTake = false;
		if((r+1) < effect.nSizeVanishChanges[n])
		{
			dCar = effect.dWVanishChanges[n][r+1];
			if(dCar <= dUbi) bTake = false;
		}
		if(bTake)
		{
			DWORD dwVanish = LOWORD(effect.dwVanishChanges[n][r]);
			int nIsSup = dwVanish&0xFF;
			int nReg = (dwVanish>>8)&0xFF;
			double ob = 1.0;
			double dClipMin = 0.2;
			double dClipMax = -1.0;
			// Unica asignacion
			if(nIsSup)
			{
				if(nReg != 0) dClipMin = -(double)nReg/10;
			}
			else
			{
				if(nReg != 0) dClipMax = (-9+(double)nReg)/10;
			}
			if((dClipMax*60.0) > t && (dClipMax*60.0-10.0) < t)
			ob = 1-abs(dClipMax*60.0-t)/10;
			else if((dClipMax*60.0-10.0) >= t) ob = 0.0;
			if((dClipMin*60.0) < t && (dClipMin*60.0+10.0) > t)
				ob = 1-abs(dClipMin*60.0-t)/10;
			else if((dClipMin*60.0+10.0) <= t) ob = 0.0;
			(*o) = min(ob, (*o));
			break;
		}
	}
}

void CSKSFView::UpdateAttack(void)
{
	double dUbi = dBeat;
	for(int n = 0; n < 3; n++) for(int r = 0; r < effect.nSizeAttackChanges[n]; r++)
	{
		double dCar = effect.dWAttackChanges[n][r];
		bool bTake = true;
		if(dCar > dUbi) bTake = false;
		else if((dCar+m_dLevelMach) < dUbi) bTake = false;
		if((r+1) < effect.nSizeAttackChanges[n])
		{
			dCar = effect.dWAttackChanges[n][r+1];
			if(dCar <= dUbi) bTake = false;
		}
		if(bTake)
		{
			int nAttack = LOWORD(effect.dwAttackChanges[n][r])%4;
			evarrows.bHidden = true;
			evarrows.nHidden = BeatToTime(effect.dWAttackChanges[n][r]);
			switch(nAttack)
			{
			case 0:
				m_bUnderAttack = false;
				m_bLeftAttack = false;
				m_bRightAttack = false;
				break;
			case 1:
				m_bUnderAttack = true;
				m_bLeftAttack = false;
				m_bRightAttack = false;
				break;
			case 2:
				m_bUnderAttack = false;
				m_bLeftAttack = true;
				m_bRightAttack = false;
				break;
			case 3:
				m_bUnderAttack = false;
				m_bLeftAttack = false;
				m_bRightAttack = true;
				break;
			}
		}
	}
}

void CSKSFView::AllocStatus(void)
{
	for(int x = 0; x < 3; x++)
	{
		status.nPerfect[x] = 0;		// Perfect
		status.nGreat[x] = 0;			// Great
		status.nGood[x] = 0;			// Good
		status.nBad[x] = 0;			// Bad
		status.nMiss[x] = 0;			// Miss
		status.nCombo[x] = 0;			// Combo actual
		status.nMaxCombo[x] = 0;		// Max Combo
		status.nMissCombo[x] = 0;		// Combo actual
		status.nMaxMissCombo[x] = 0;	// Max Combo
	}
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
	status.nLastEva = 0;		// Ultimo momento en el cual apareció un PERFECT, GREAT, BAD...
	status.nTypeEva = 0;			// Tipo del último pop que ha salido
	status.nLastBSPop = 0;		// Ultimo pop de Brain Shower
	status.nIndexBSPop = 0;		// Numero del pop Brain Shower
	status.nLastBSQs = 0;		// Ultima pregunta de Brain Shower
	status.nIndexBSQs = 0;			// Numero de la pregunta Brain Shower
	status.nLastBSEva = 0;		// Ya sabe
	status.nTypeBSEva = 0;			// Tipo de evaluacion
	if(songc.nSizeArrows1)
	{
		status.nArrowsStatus1=(int*)realloc(status.nArrowsStatus1, songc.nSizeArrows1*10*sizeof(int));
		for(int x = 0; x < (songc.nSizeArrows1*10); x++)
			status.nArrowsStatus1[x] = 0;
	}
	if(songc.nSizeArrows2)
	{
		status.nArrowsStatus2=(int*)realloc(status.nArrowsStatus2, songc.nSizeArrows2*10*sizeof(int));
		for(int x = 0; x < (songc.nSizeArrows2*10); x++)
			status.nArrowsStatus2[x] = 0;
	}
	if(songc.nSizeArrows3)
	{
		status.nArrowsStatus3=(int*)realloc(status.nArrowsStatus3, songc.nSizeArrows3*10*sizeof(int));
		for(int x = 0; x < (songc.nSizeArrows3*10); x++)
			status.nArrowsStatus3[x] = 0;
	}
	if(bsc.nSizeBSAnswers)
	{
		status.nBSStatus1=(int*)realloc(status.nBSStatus1, bsc.nSizeBSAnswers*10*sizeof(int));
		for(int x = 0; x < (bsc.nSizeBSAnswers*10); x++)
			status.nBSStatus1[x] = 0;
	}
	for(int x = 0; x < 10; x++)
	{
		evarrows.bArrows[x] = false;
		evarrows.nArrows[x] = m_nOffset;
		evarrows.nGlowArrows[x] = m_nOffset;
	}
	evarrows.bCredit[0] = false;
	evarrows.bCredit[1] = false;
	evarrows.bService = false;
	evarrows.bOption = false;
	evarrows.bVel = false;
	evarrows.dSourceVel = 1.0;
	evarrows.dDestVel = 1.0;
	evarrows.dWWVel = 0.0;
	evarrows.bHidden = false;
	evarrows.nHidden = 0;
	status.dLife = 0.5;	// barra de vida a la mitad
}

void CSKSFView::DeleteStatus(void)
{
	if(songc.nSizeArrows1)
	{
		status.nArrowsStatus1=(int*)realloc(status.nArrowsStatus1, 0);
	}
	if(songc.nSizeArrows2)
	{
		status.nArrowsStatus2=(int*)realloc(status.nArrowsStatus2, 0);
	}
	if(songc.nSizeArrows3)
	{
		status.nArrowsStatus3=(int*)realloc(status.nArrowsStatus3, 0);
	}
	if(bsc.nSizeBSAnswers)
	{
		status.nBSStatus1=(int*)realloc(status.nBSStatus1, 0);
	}
}

// Construcción o destrucción de CSKSFDoc

CSKSFDoc::CSKSFDoc()
: b(false)
{
	memset(&song.hStep, 0, sizeof(STEPHEADER));
	song.dwArrows1=(DWORD*)calloc(song.hStep.nSizeArrows1*10,sizeof(DWORD));
	song.dwArrows2=(DWORD*)calloc(song.hStep.nSizeArrows2*10,sizeof(DWORD));
	song.dwArrows3=(DWORD*)calloc(song.hStep.nSizeArrows3*10,sizeof(DWORD));
	song.nWChangeBPM=(int32_t*)calloc(song.hStep.nSizeChangeBPM,sizeof(int32_t));
	song.dChangeBPM=(double*)calloc(song.hStep.nSizeChangeBPM,sizeof(double));
	song.nWBeatSplit=(int32_t*)calloc(song.hStep.nSizeBeatSplit,sizeof(int32_t));
	song.nBeatSplit=(int32_t*)calloc(song.hStep.nSizeBeatSplit,sizeof(int32_t));
	song.nIChangeVel=(int32_t*)calloc(song.hStep.nSizeChangeVel,sizeof(int32_t));
	song.nEChangeVel=(int32_t*)calloc(song.hStep.nSizeChangeVel,sizeof(int32_t));
	song.dChanges=(double*)calloc(song.hStep.nSizeChangeVel,sizeof(double));
	song.nIJumps=(int32_t*)calloc(song.hStep.nSizeJumps,sizeof(int32_t));
	song.nEJumps=(int32_t*)calloc(song.hStep.nSizeJumps,sizeof(int32_t));
	song.nWTickCount=(int32_t*)calloc(song.hStep.nSizeTickcount,sizeof(int32_t));
	song.nTickCount=(int32_t*)calloc(song.hStep.nSizeTickcount,sizeof(int32_t));
	song.nWStops=(int32_t*)calloc(song.hStep.nSizeStops,sizeof(int32_t));
	song.dStops=(double*)calloc(song.hStep.nSizeStops,sizeof(double));
}

CSKSFDoc::~CSKSFDoc()
{
	delete song.dwArrows1;
	delete song.dwArrows2;
	delete song.dwArrows3;
	delete song.nWChangeBPM;
	delete song.dChangeBPM;
	delete song.nWBeatSplit;
	delete song.nBeatSplit;
	delete song.nIChangeVel;
	delete song.nEChangeVel;
	delete song.dChanges;
	delete song.nIJumps;
	delete song.nEJumps;
	delete song.nWTickCount;
	delete song.nTickCount;
	delete song.nWStops;
	delete song.dStops;
}

int CSKSFDoc::LoadFile(LPCTSTR strFile)
{
	if(b)
	{
		memset(&song.hStep, 0, sizeof(STEPHEADER));
		song.dwArrows1=(DWORD*)realloc(song.dwArrows1, song.hStep.nSizeArrows1*10*sizeof(DWORD));
		song.dwArrows2=(DWORD*)realloc(song.dwArrows2, song.hStep.nSizeArrows2*10*sizeof(DWORD));
		song.dwArrows3=(DWORD*)realloc(song.dwArrows3, song.hStep.nSizeArrows3*10*sizeof(DWORD));
		song.nWChangeBPM=(int32_t*)realloc(song.nWChangeBPM, song.hStep.nSizeChangeBPM*sizeof(int32_t));
		song.dChangeBPM=(double*)realloc(song.dChangeBPM, song.hStep.nSizeChangeBPM*sizeof(double));
		song.nWBeatSplit=(int32_t*)realloc(song.nWBeatSplit, song.hStep.nSizeBeatSplit*sizeof(int32_t));
		song.nBeatSplit=(int32_t*)realloc(song.nBeatSplit, song.hStep.nSizeBeatSplit*sizeof(int32_t));
		song.nIChangeVel=(int32_t*)realloc(song.nIChangeVel, song.hStep.nSizeChangeVel*sizeof(int32_t));
		song.nEChangeVel=(int32_t*)realloc(song.nEChangeVel, song.hStep.nSizeChangeVel*sizeof(int32_t));
		song.dChanges=(double*)realloc(song.dChanges, song.hStep.nSizeChangeVel*sizeof(double));
		song.nIJumps=(int32_t*)realloc(song.nIJumps, song.hStep.nSizeJumps*sizeof(int32_t));
		song.nEJumps=(int32_t*)realloc(song.nEJumps, song.hStep.nSizeJumps*sizeof(int32_t));
		song.nWTickCount=(int32_t*)realloc(song.nWTickCount, song.hStep.nSizeTickcount*sizeof(int32_t));
		song.nTickCount=(int32_t*)realloc(song.nTickCount, song.hStep.nSizeTickcount*sizeof(int32_t));
		song.nWStops=(int32_t*)realloc(song.nWStops, song.hStep.nSizeStops*sizeof(int32_t));
		song.dStops=(double*)realloc(song.dStops, song.hStep.nSizeStops*sizeof(double));
	}
	b = true;
	FILE* hFile;
	hFile = _fopen(strFile, _T("rb"));
	if(hFile == NULL)
	{
		_printf(_T("Fallo al abrir archivo SKSF\n"));
		return 0;
	}
	fread(&song.hStep, sizeof(STEPHEADER), 1, hFile);
	// ** Reservando memoria
	song.dwArrows1=(DWORD*)realloc(song.dwArrows1, song.hStep.nSizeArrows1*10*sizeof(DWORD));
	song.dwArrows2=(DWORD*)realloc(song.dwArrows2, song.hStep.nSizeArrows2*10*sizeof(DWORD));
	song.dwArrows3=(DWORD*)realloc(song.dwArrows3, song.hStep.nSizeArrows3*10*sizeof(DWORD));
	song.nWChangeBPM=(int32_t*)realloc(song.nWChangeBPM, song.hStep.nSizeChangeBPM*sizeof(int32_t));
	song.dChangeBPM=(double*)realloc(song.dChangeBPM, song.hStep.nSizeChangeBPM*sizeof(double));
	song.nWBeatSplit=(int32_t*)realloc(song.nWBeatSplit, song.hStep.nSizeBeatSplit*sizeof(int32_t));
	song.nBeatSplit=(int32_t*)realloc(song.nBeatSplit, song.hStep.nSizeBeatSplit*sizeof(int32_t));
	song.nIChangeVel=(int32_t*)realloc(song.nIChangeVel, song.hStep.nSizeChangeVel*sizeof(int32_t));
	song.nEChangeVel=(int32_t*)realloc(song.nEChangeVel, song.hStep.nSizeChangeVel*sizeof(int32_t));
	song.dChanges=(double*)realloc(song.dChanges, song.hStep.nSizeChangeVel*sizeof(double));
	song.nIJumps=(int32_t*)realloc(song.nIJumps, song.hStep.nSizeJumps*sizeof(int32_t));
	song.nEJumps=(int32_t*)realloc(song.nEJumps, song.hStep.nSizeJumps*sizeof(int32_t));
	song.nWTickCount=(int32_t*)realloc(song.nWTickCount, song.hStep.nSizeTickcount*sizeof(int32_t));
	song.nTickCount=(int32_t*)realloc(song.nTickCount, song.hStep.nSizeTickcount*sizeof(int32_t));
	song.nWStops=(int32_t*)realloc(song.nWStops, song.hStep.nSizeStops*sizeof(int32_t));
	song.dStops=(double*)realloc(song.dStops, song.hStep.nSizeStops*sizeof(double));
	// ** Lectura General
	fread(song.dwArrows1,	song.hStep.nSizeArrows1*sizeof(DWORD)*10, 1, hFile);
	fread(song.dwArrows2,	song.hStep.nSizeArrows2*sizeof(DWORD)*10, 1, hFile);
	fread(song.dwArrows3,	song.hStep.nSizeArrows3*sizeof(DWORD)*10, 1, hFile);
	fread(song.nWChangeBPM,	song.hStep.nSizeChangeBPM*sizeof(int32_t), 1, hFile);
	fread(song.dChangeBPM,	song.hStep.nSizeChangeBPM*sizeof(double), 1, hFile);
	fread(song.nWBeatSplit,	song.hStep.nSizeBeatSplit*sizeof(int32_t), 1, hFile);
	fread(song.nBeatSplit,	song.hStep.nSizeBeatSplit*sizeof(int32_t), 1, hFile);
	fread(song.nIChangeVel,	song.hStep.nSizeChangeVel*sizeof(int32_t), 1, hFile);
	fread(song.nEChangeVel,	song.hStep.nSizeChangeVel*sizeof(int32_t), 1, hFile);
	fread(song.dChanges,	song.hStep.nSizeChangeVel*sizeof(double), 1, hFile);
	fread(song.nIJumps,		song.hStep.nSizeJumps*sizeof(int32_t), 1, hFile);
	fread(song.nEJumps,		song.hStep.nSizeJumps*sizeof(int32_t), 1, hFile);
	fread(song.nWTickCount,	song.hStep.nSizeTickcount*sizeof(int32_t), 1, hFile);
	fread(song.nTickCount,	song.hStep.nSizeTickcount*sizeof(int32_t), 1, hFile);
	fread(song.nWStops,		song.hStep.nSizeStops*sizeof(int32_t), 1, hFile);
	fread(song.dStops,		song.hStep.nSizeStops*sizeof(double), 1, hFile);
	fclose(hFile);
	return 1;
}

void CSKSFDoc::CloseFile()
{
	if(b)
	{
		memset(&song.hStep, 0, sizeof(STEPHEADER));
		song.dwArrows1=(DWORD*)realloc(song.dwArrows1, song.hStep.nSizeArrows1*10*sizeof(DWORD));
		song.dwArrows2=(DWORD*)realloc(song.dwArrows2, song.hStep.nSizeArrows2*10*sizeof(DWORD));
		song.dwArrows3=(DWORD*)realloc(song.dwArrows3, song.hStep.nSizeArrows3*10*sizeof(DWORD));
		song.nWChangeBPM=(int32_t*)realloc(song.nWChangeBPM, song.hStep.nSizeChangeBPM*sizeof(int32_t));
		song.dChangeBPM=(double*)realloc(song.dChangeBPM, song.hStep.nSizeChangeBPM*sizeof(double));
		song.nWBeatSplit=(int32_t*)realloc(song.nWBeatSplit, song.hStep.nSizeBeatSplit*sizeof(int32_t));
		song.nBeatSplit=(int32_t*)realloc(song.nBeatSplit, song.hStep.nSizeBeatSplit*sizeof(int32_t));
		song.nIChangeVel=(int32_t*)realloc(song.nIChangeVel, song.hStep.nSizeChangeVel*sizeof(int32_t));
		song.nEChangeVel=(int32_t*)realloc(song.nEChangeVel, song.hStep.nSizeChangeVel*sizeof(int32_t));
		song.dChanges=(double*)realloc(song.dChanges, song.hStep.nSizeChangeVel*sizeof(double));
		song.nIJumps=(int32_t*)realloc(song.nIJumps, song.hStep.nSizeJumps*sizeof(int32_t));
		song.nEJumps=(int32_t*)realloc(song.nEJumps, song.hStep.nSizeJumps*sizeof(int32_t));
		song.nWTickCount=(int32_t*)realloc(song.nWTickCount, song.hStep.nSizeTickcount*sizeof(int32_t));
		song.nTickCount=(int32_t*)realloc(song.nTickCount, song.hStep.nSizeTickcount*sizeof(int32_t));
		song.nWStops=(int32_t*)realloc(song.nWStops, song.hStep.nSizeStops*sizeof(int32_t));
		song.dStops=(double*)realloc(song.dStops, song.hStep.nSizeStops*sizeof(double));
		b = false;
	}
}

DWORD CSKSFDoc::GetArrow(int x, int y, int nGArrow)
{
	int nRel;
	if(nGArrow == 0)
	{
		nRel = y*10+x;
		if(nRel < 0) return 0;
		if(nRel >= (song.hStep.nSizeArrows1*10)) return 0;
		return song.dwArrows1[nRel];
	}
	if(nGArrow == 1)
	{
		nRel = y*10+x;
		if(nRel < 0) return 0;
		if(nRel >= (song.hStep.nSizeArrows2*10)) return 0;
		return song.dwArrows2[nRel];
	}
	if(nGArrow == 2)
	{
		nRel = y*10+x;
		if(nRel < 0) return 0;
		if(nRel >= (song.hStep.nSizeArrows3*10)) return 0;
		return song.dwArrows3[nRel];
	}
	return 0;
}

double CSKSFDoc::BeatArrow(int nIndex)
{
	double dBeat = 0.0;
	int nCar = song.nWBeatSplit[0]; // Aunque ia sabemos que es 0
	if(nIndex == 0) return 0;
	int x;
	for(x = 1; x < song.hStep.nSizeBeatSplit; x++)
	{
		if(nIndex <= song.nWBeatSplit[x])
		{
			break;
		}
		else
		{
			dBeat += (song.nWBeatSplit[x]-nCar)*(1/((double)song.nBeatSplit[x-1]));
			nCar = song.nWBeatSplit[x];
		}
	}
	dBeat += (nIndex-nCar)*(1/((double)song.nBeatSplit[x-1]));
	return dBeat;
}

int CSKSFDoc::VerificateBeatSplitChange(int nIndex)
{
	for(int x = 0; x < song.hStep.nSizeBeatSplit; x++)
	{
		if(song.nWBeatSplit[x] == nIndex)
			return x;
	}
	return -1;
}

int CSKSFDoc::VerificateBPMChange(int nIndex)
{
	for(int x = 0; x < song.hStep.nSizeChangeBPM; x++)
	{
		if(song.nWChangeBPM[x] == nIndex)
			return x;
	}
	return -1;
}

int CSKSFDoc::VerificateVelChange(int nIndex)
{
	// ** ADVERTENCIA... ESTA ES SOLO PARA EL EDITOR
	// Varia la condición (de <= a ==)
	for(int x = 0; x < song.hStep.nSizeChangeVel; x++)
	{
		if(song.nIChangeVel[x] <= nIndex && song.nEChangeVel[x] >= nIndex)
			return x;
	}
	return -1;
}

int CSKSFDoc::VerificateTickCountChange(int nIndex)
{
	for(int x = 0; x < song.hStep.nSizeTickcount; x++)
	{
		if(song.nWTickCount[x] == nIndex)
			return x;
	}
	return -1;
}

int CSKSFDoc::VerificateStopChange(int nIndex)
{
	for(int x = 0; x < song.hStep.nSizeStops; x++)
	{
		if(song.nWStops[x] == nIndex)
			return x;
	}
	return -1;
}

int CSKSFDoc::VerificateJumpChange(int nIndex)
{
	// ** ADVERTENCIA... ESTA ES SOLO PARA EL EDITOR
	// Varia la condición (de <= a ==)
	for(int x = 0; x < song.hStep.nSizeJumps; x++)
	{
		if(song.nIJumps[x] <= nIndex && song.nEJumps[x] >= nIndex)
			return x;
	}
	return -1;
}

int CSKSFDoc::VerificateJumpChangeEq(int nIndex)
{
	for(int x = 0; x < song.hStep.nSizeJumps; x++)
	{
		if(song.nIJumps[x] == nIndex)
			return x;
	}
	return -1;
}

bool CSKSFDoc::SomeArrow(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > 2) return false;
	for(int x = 0; x < 10; x++)
	{
		DWORD dw = LOWORD(GetArrow(x, y, nGArrow));
		if(dw==1||dw==2)
			bReturn = true;
	}
	return bReturn;
}

bool CSKSFDoc::SomeObject(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > 2) return false;
	for(int x = 0; x < 10; x++)
	{
		if(LOWORD(GetArrow(x, y, nGArrow))>2 && LOWORD(GetArrow(x, y, nGArrow))<=18) bReturn = true;
	}
	return bReturn;
}

bool CSKSFDoc::SomeNote(int y, int nGArrow)
{
	return SomeArrow(y, nGArrow) || SomeObject(y, nGArrow);
}

bool CSKSFDoc::SomeBSQuestion(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > 2) return false;
	for(int x = 0; x < 10; x++)
	{
		DWORD dw = LOWORD(GetArrow(x, y, nGArrow));
		if(dw==19)
			bReturn = true;
	}
	return bReturn;
}

bool CSKSFDoc::SomeBSAnswer(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > 2) return false;
	for(int x = 0; x < 10; x++)
	{
		DWORD dw = LOWORD(GetArrow(x, y, nGArrow));
		if(dw==20)
			bReturn = true;
	}
	return bReturn;
}

bool CSKSFDoc::SomeBSPopUp(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > 2) return false;
	for(int x = 0; x < 10; x++)
	{
		DWORD dw = LOWORD(GetArrow(x, y, nGArrow));
		if(dw==21)
			bReturn = true;
	}
	return bReturn;
}

bool CSKSFDoc::SomeEffect(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > 2) return false;
	for(int x = 0; x < 10; x++)
	{
		DWORD dw = LOWORD(GetArrow(x, y, nGArrow));
		if(dw>21 && dw<=24)
			bReturn = true;
	}
	return bReturn;
}

bool CSKSFDoc::IsArrowsMisible(int nGArrows)
{
	if(nGArrows == 0) return !(song.hStep.dwArgArrows1 & ARGNOTE_NOMISS);
	if(nGArrows == 1) return !(song.hStep.dwArgArrows2 & ARGNOTE_NOMISS);
	if(nGArrows == 2) return !(song.hStep.dwArgArrows3 & ARGNOTE_NOMISS);
	return false;
}

bool CSKSFDoc::IsArrowsCountable(int nGArrows)
{
	if(nGArrows == 0)
		return !(song.hStep.dwArgArrows1 & ARGNOTE_NOCOUNT);
	if(nGArrows == 1)
		return !(song.hStep.dwArgArrows2 & ARGNOTE_NOCOUNT);
	if(nGArrows == 2)
		return !(song.hStep.dwArgArrows3 & ARGNOTE_NOCOUNT);
	return false;
}

// ** Exportacion

int LoadSong(ARGSTEP arg)
{
	char ch[260];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if(!arg.strSplash.IsEmpty() && !arg.bAutoPlay)
	{
		GLuint uSplash;
		uSplash = GenTexImage(arg.strSplash);
		if(uSplash != m_uNullTex)
		{
			glBindTexture(GL_TEXTURE_2D, uSplash);
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(-40.0, 30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(40.0, 30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(40.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
			glEnd();
			SDL_GL_SwapBuffers();
			glDeleteTextures(1, &uSplash);
		}
		else
		{
			SDL_GL_SwapBuffers();
		}
	}
	else
	{
		SDL_GL_SwapBuffers();
	}
	ClearDraw();
	dwTickLoad = SDL_GetTicks();
	if(!arg.strBGA.IsEmpty())
	{
		bFirstUseBGA = true;
		backMovie = new CMovieStream2();
		backMovie->OpenAVI(arg.strBGA);
	}
	if(!arg.strMusic.IsEmpty())
	{
#ifdef UNICODE
		UnicodeToAnsi(arg.strMusic, ch);
		res_step = FMOD_System_CreateStream(sys, ch, FMOD_HARDWARE | FMOD_2D, 0, &music_s);
#else
		res_step = FMOD_System_CreateStream(sys, arg.strMusic, FMOD_HARDWARE | FMOD_2D, 0, &music_s);
#endif
		if(ERRCHECK(res_step)) g_bIsMusic = true;
	}
	songLight = new CMaxLights();
	if(!arg.strLights.IsEmpty())
	{
		songLight->Open(arg.strLights);
	}
	FMOD_System_CreateStream(sys, "Skins/mine.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_mine);
	dwTick = SDL_GetTicks();
	dwLastTick = SDL_GetTicks();
	return 1;
}

int LoadSongCompile(ARGSTEP arg, int nPlayer)
{
	if(nPlayer < 0 || nPlayer > 2) return 0;
	doc = new CSKSFDoc();
	if(!doc->LoadFile(arg.strFile))
	{
		_printf(_T("Fallo SKSFDoc\n"));
		doc->CloseFile();
		delete doc;
		return 0;
	}
	if(nPlayer == 1)
	{
		if(g_bPlayer[0])
		{
			delete viewp1;
			g_bPlayer[0] = false;
		}
		if(!g_bPlayer[0]) viewp1 = new CSKSFView();
		viewp1->m_nPlayer = 1;
		viewp1->StepDestroy();
		viewp1->m_nSkin = arg.nSkin;
		viewp1->m_bIsSingle = arg.bIsSingle;
		viewp1->m_bTrainingMode = arg.bTrainingMode;
		viewp1->m_bRS = arg.bRS;
		viewp1->m_nSkin = arg.nSkin;
		viewp1->m_dUserVel = arg.dUserVel;
		viewp1->m_bXMode = arg.bXMode;
		viewp1->m_bNXMode = arg.bNXMode;
		viewp1->m_bHJ = arg.bHJ;
		viewp1->m_bRS = arg.bRS;
		viewp1->m_bMirror = arg.bMirror;
		viewp1->m_bFreedom = arg.bFreedom;
		viewp1->m_bRV = arg.bRV;
		viewp1->m_bRandomSkin = arg.bRandomSkin;
		viewp1->m_nVanish = arg.nVanish;
		viewp1->m_bDC = arg.bDC;
		viewp1->m_bAC = arg.bAC;
		viewp1->m_bEW = arg.bEW;
		viewp1->m_bFlash = arg.bFlash;
		viewp1->m_bUnderAttack = arg.bUnderAttack;
		viewp1->m_bLeftAttack = arg.bLeftAttack;
		viewp1->m_bRightAttack = arg.bRightAttack;
		viewp1->m_bSnake = arg.bSnake;
		viewp1->m_nCharacter = arg.nCharacter;
		viewp1->m_bCharacter = arg.bCharacter;
		viewp1->m_bAutoPlay = arg.bAutoPlay;
		viewp1->m_nLevelCharacter = arg.nLevelCharacer;
		viewp1->m_nTypeCharacter = arg.nTypeCharacter;
		viewp1->dIncrBar = arg.dIncrBar;
		viewp1->dDecrBar = arg.dDecrBar;
		viewp1->m_nLevelMach = arg.nLevelMach;
		viewp1->m_bDrop = arg.bDrop;
		viewp1->dPowTimer = arg.dPowTimer;
		if(!viewp1->StepCompile())
		{
			delete viewp1;
			_printf(_T("Fallo al compilar SKSF 1p\n"));
			return 0;
		}
		g_bPlayer[0] = true;
	}
	if(nPlayer == 2)
	{
		if(g_bPlayer[1])
		{
			delete viewp2;
			g_bPlayer[1] = false;
		}
		if(!g_bPlayer[1]) viewp2 = new CSKSFView();
		viewp2->m_nPlayer = 2;
		viewp2->StepDestroy();
		viewp2->m_nSkin = arg.nSkin;
		viewp2->m_bIsSingle = arg.bIsSingle;
		viewp2->m_bTrainingMode = arg.bTrainingMode;
		viewp2->m_bRS = arg.bRS;
		viewp2->m_nSkin = arg.nSkin;
		viewp2->m_dUserVel = arg.dUserVel;
		viewp2->m_bXMode = arg.bXMode;
		viewp2->m_bNXMode = arg.bNXMode;
		viewp2->m_bHJ = arg.bHJ;
		viewp2->m_bRS = arg.bRS;
		viewp2->m_bMirror = arg.bMirror;
		viewp2->m_bFreedom = arg.bFreedom;
		viewp2->m_bRV = arg.bRV;
		viewp2->m_bRandomSkin = arg.bRandomSkin;
		viewp2->m_nVanish = arg.nVanish;
		viewp2->m_bDC = arg.bDC;
		viewp2->m_bAC = arg.bAC;
		viewp2->m_bEW = arg.bEW;
		viewp2->m_bFlash = arg.bFlash;
		viewp2->m_bUnderAttack = arg.bUnderAttack;
		viewp2->m_bLeftAttack = arg.bLeftAttack;
		viewp2->m_bRightAttack = arg.bRightAttack;
		viewp2->m_bSnake = arg.bSnake;
		viewp2->m_nCharacter = arg.nCharacter;
		viewp2->m_bCharacter = arg.bCharacter;
		viewp2->m_bAutoPlay = arg.bAutoPlay;
		viewp2->m_nLevelCharacter = arg.nLevelCharacer;
		viewp2->m_nTypeCharacter = arg.nTypeCharacter;
		viewp2->dIncrBar = arg.dIncrBar;
		viewp2->dDecrBar = arg.dDecrBar;
		viewp2->m_nLevelMach = arg.nLevelMach;
		viewp2->m_bDrop = arg.bDrop;
		viewp2->dPowTimer = arg.dPowTimer;
		if(!viewp2->StepCompile())
		{
			delete viewp2;
			_printf(_T("Fallo al compilar SKSF 2p\n"));
			return 0;
		}
		g_bPlayer[1] = true;
	}
	doc->CloseFile();
	delete doc;
	return 1;
}

void DeleteSong()
{
	if(bFirstUseBGA)
	{
		if(backMovie->bLoad) backMovie->CloseAVI();
		delete backMovie;
		bFirstUseBGA = false;
	}
	if(g_bIsMusic)
	{
		FMOD_Channel_Stop(channels[1]);
		FMOD_Sound_Release(music_s); music_s = 0;
		g_bIsMusic = false;
	}
	songLight->Close();
	delete songLight;
	FMOD_Sound_Release(sound_mine);
}

void DeleteSongCompile(int nPlayer)
{
	if(nPlayer == 1 && g_bPlayer[0])
	{
		viewp1->m_nPlayer = 0;
		viewp1->StepDestroy();
		delete viewp1;
		g_bPlayer[0] = false;
	}
	if(nPlayer == 2 && g_bPlayer[1])
	{
		viewp2->m_nPlayer = 0;
		viewp2->StepDestroy();
		delete viewp2;
		g_bPlayer[1] = false;
	}
}

void Event_Step(int nEvent, int nKey)
{
	if(g_bPlayer[0]) if(!viewp1->m_bAutoPlay) viewp1->Event_View(nEvent, nKey);
	if(g_bPlayer[1]) if(!viewp2->m_bAutoPlay) viewp2->Event_View(nEvent, nKey);
}

int Draw_All()
{
	static bool m_bBusy = false;
	if(g_bDraw && !m_bBusy && dwTick >= dwLastTick)
	{
		m_bBusy = true;
		FMOD_Channel_GetPosition(channels[1], &nPos, FMOD_TIMEUNIT_MS);
		FMOD_BOOL nIsPlaying = 0;
		FMOD_Channel_IsPlaying(channels[1], &nIsPlaying);
		if(!nIsPlaying || channels[1] == NULL)
		{
			m_bBusy = false;
			return 0;
		}
		dwStepSmu = (DWORD)nPos;
		// ** BGA
		if(bFirstUseBGA) if(backMovie->bLoad)
		{
			/*if(nPos >= backMovie->dwNext)
			{
				backMovie->dwNext = (DWORD)(nPos);
				backMovie->Update(0);
			}
			else if(!nIsPlaying)*/
				backMovie->Update(dwTick - dwLastTick);
			backMovie->GrabAVIFrame();
			glBindTexture(GL_TEXTURE_2D, backMovie->tex);
			glBegin(GL_QUADS);
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(80.0, -60.0, -100.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(80.0, 60.0, -100.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-80.0, 60.0, -100.0);
			glEnd();
		}
		songLight->Update(dwTick - dwLastTick);
		/////////
		if(g_bPlayer[0]) viewp1->Draw();
		if(g_bPlayer[1]) viewp2->Draw();
		m_bBusy = false;
	}
	return 1;
}

void DeleteContextStep()
{
	delete doc;
	delete viewp1;
	delete viewp2;
	DeleteSong();
	delete backMovie;
}

void Go_Step()
{
	DWORD dwTickConf = SDL_GetTicks();
	if((dwTickConf-dwTickLoad) < MIN_TIME_INIT)
	{
		SDL_Delay(MIN_TIME_INIT-(dwTickConf-dwTickLoad));
	}
	g_bDraw = true;
	FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)1, music_s, 0, &channels[1]);
	FMOD_Channel_SetPosition(channels[1], 0, FMOD_TIMEUNIT_MS);
	dwTick = SDL_GetTicks();
	dwLastTick = SDL_GetTicks();
}

CMaxLights::CMaxLights()
{
	nSizeLights = 0;
	nSizeChangeBPM = 0;
	bLights=(BYTE*)calloc(nSizeLights*3, sizeof(BYTE));
	dChangeBPM=(double*)calloc(nSizeChangeBPM,sizeof(double));
	dWChangeBPM=(double*)calloc(nSizeChangeBPM,sizeof(double));
	dwOffset = 0;
	dwNext = 0;
	nTickCount = 1;
	bLoop = false;
}

CMaxLights::~CMaxLights()
{
	nSizeLights = 0;
	nSizeChangeBPM = 0;
	delete bLights;
	delete dChangeBPM;
	delete dWChangeBPM;
}
#define MAX_READBUF 1000

int CMaxLights::Open(LPCTSTR strFile)
{
	FILE* hFile = _fopen(strFile, _T("r"));
	if(hFile == NULL) return 0;
	CString strArg;
	bool bConBPM = false;
	bool bConStop = false;
	CString strCom, strPar;
	TCHAR chArg[MAX_READBUF];
	while(_fgets(chArg, MAX_READBUF, hFile) != NULL)
	{
		strArg = ( chArg);
		strArg.Remove(_T('\n'));	// Remover los saltos de linea
		strArg.Remove(_T('\t'));	// Remover las tabulaciones
		int nCode = strArg.Find(_T(':'), 0);
		if(nCode != -1)
		{
			strCom = strArg.Left(nCode);
			strPar = strArg.Right(strArg.GetLength()-nCode-1);
			strCom.MakeUpper();
			if(strCom == _T("#BPM"))
			{
				bConBPM=true;
				while(bConBPM)
				{
					int nBackCode = 0;
					CString strBpm;
					double dBPM=0;
					nCode = strPar.Find(_T(';'));
					if(nCode == -1)
					{
						bConBPM = true;
						strBpm = strPar.Mid(nBackCode, strPar.GetLength()-2*nBackCode+1);
					}
					else
					{
						bConBPM = false;
						strBpm = strPar.Mid(nBackCode, nCode-nBackCode);
					}
					strBpm.Remove(_T(' '));
					strBpm.Remove(_T('\t'));
					strBpm.Remove(_T('\n'));
					if(!strBpm.IsEmpty())
					{
						float n1=0;
						if(_sscanf(strBpm, _T("%f"), &n1)!=1)
						{
							_printf(_T("ERROR: Mal especificado #BPM"));
							fclose(hFile);
							return 0;
						}
						if(n1 > 0)
						{
							dBPM = n1;
							nSizeChangeBPM++;
							dWChangeBPM=(double*)realloc(dWChangeBPM, nSizeChangeBPM*sizeof(double));
							dChangeBPM=(double*)realloc(dChangeBPM, nSizeChangeBPM*sizeof(double));
							dWChangeBPM[nSizeChangeBPM-1]=0;
							dChangeBPM[nSizeChangeBPM-1]=dBPM;
						}
						else
						{
							_printf(_T("ERROR: #BPM... Tiene 0.0 o un valor negativo"));
							fclose(hFile);
							return 0;
						}
					}
					if(bConBPM)
					{
						_fgets(chArg, MAX_READBUF, hFile);
						strPar = ( chArg);
						strPar.Remove(_T('\n'));	// Remover los saltos de linea
						strPar.Remove(_T('\t'));	// Remover las tabulaciones
					}
				}
			}
			if(strCom == _T("#LIGHTMAP"))
			{
				bool bConLight = true;
				int nCountArrows = 0;
				_fgets(chArg, MAX_READBUF, hFile);	// Lo q seria la primera flecha
				strPar = ( chArg);
				strPar.Remove(_T('\n'));	// Remover los saltos de linea
				strPar.Remove(_T('\t'));	// Remover las tabulaciones
				strPar.Remove(_T(' '));		// Remover los espacios
				strPar.Remove(_T('.'));
				int nBeatCount = 0, nBackBeatCount = 0, nWBeatCount = 0;
				while(bConLight)
				{
					int nBackCode = 0;
					CString strArrow = strPar;
					BYTE m_bLights[3];
					nCode = strArrow.Find(_T('2'));
					if(nCode == -1)
					{
						bConLight = true;
					}
					else
					{
						bConLight = false;
						break;
					}
					nCode = strArrow.Find(_T('|'));
					if(nCode != -1)	// Cambio de BPM en-linea
					{
						float n1 = 0;
						if(_sscanf(strArrow, _T("|%f|"), &n1) != 1) _printf(_T("ERROR: |BPM| Error"));
						else if(n1 <= 0) _printf(_T("ERROR: |BPM| Error"));
						else
						{
							double dBPM = n1;
							nSizeChangeBPM++;
							dWChangeBPM=(double*)realloc(dWChangeBPM, nSizeChangeBPM*sizeof(double));
							dChangeBPM=(double*)realloc(dChangeBPM, nSizeChangeBPM*sizeof(double));
							dWChangeBPM[nSizeChangeBPM-1]=double(nSizeLights)/double(nTickCount);
							dChangeBPM[nSizeChangeBPM-1]=dBPM;
						}
					}
					else if(!strArrow.IsEmpty())
					{
						nSizeLights++;
						bLights=(BYTE*)realloc(bLights, nSizeLights*sizeof(BYTE)*3);
						for(int x = 0; x < 3; x++) m_bLights[x]=0;
						for(int x = 0; x < 3; x++)
						{
							if(strArrow.GetLength() > x)
							{
								if(strArrow.GetAt(x) == _T('1'))
								{
									m_bLights[x]=1;
								}
								if(strArrow.GetAt(x) == _T('4'))
								{
									m_bLights[x]=2;
								}
							}
						}
						bLights[(nSizeLights-1)*3]=m_bLights[0];
						bLights[(nSizeLights-1)*3+1]=m_bLights[1];
						bLights[(nSizeLights-1)*3+2]=m_bLights[2];
					}
					if(bConLight)
					{
						_fgets(chArg, MAX_READBUF, hFile);
						strPar = ( chArg);
						int nCaret = strPar.Find(_T("//"));
						if(nCaret != -1)	// Comentarios
						{
							strPar = strPar.Left(nCaret);
						}
						strPar.Remove(_T('\n'));	// Remover los saltos de linea
						strPar.Remove(_T('\t'));	// Remover las tabulaciones
						strPar.Remove(_T(' '));		// Remover los espacios
						strPar.Remove(_T('.'));
					}
				}
			}
			strPar.Remove(_T(';'));
			if(strCom == _T("#OFFSET"))
			{
				int nOffset;
				if(_sscanf(strPar, _T("%d"), &nOffset)!=1) _printf(_T("ERROR: Mal especificado #OFFSET"));
				nOffset = -nOffset;
			}
			if(strCom == _T("#STARTTIME"))
			{
				int nOffset;
				if(_sscanf(strPar, _T("%d"), &nOffset)!=1) _printf(_T("ERROR: Mal especificado #STARTTIME"));
				nOffset = -nOffset;
			}
			if(strCom == _T("#TICKCOUNT"))
			{
				int nTc;
				if(_sscanf(strPar, _T("%d"), &nTc)!=1) _printf(_T("ERROR: Mal especificado #TICKCOUNT"));
				nTickCount = nTc;
			}
		}
	}
	fclose(hFile);
	return 1;
}

void CMaxLights::Close()
{
	nSizeLights = 0;
	nSizeChangeBPM = 0;
	bLights=(BYTE*)realloc(bLights, nSizeLights*3*sizeof(BYTE));
	dChangeBPM=(double*)realloc(dChangeBPM, nSizeChangeBPM*sizeof(double));
	dWChangeBPM=(double*)realloc(dWChangeBPM, nSizeChangeBPM*sizeof(double));
}

void CMaxLights::Update(DWORD dwMilliseconds)
{
	if(nSizeLights <= 0) return;
	if(nSizeChangeBPM <= 0) return;
	dwNext += dwMilliseconds;
	__int64 nMiliseconds = dwNext + dwOffset;

	double dBeat = 0.0;

	if(nMiliseconds < 0)
	{
		dBeat = ((double)nMiliseconds)*dChangeBPM[0]/60000;
	}

	for(int x = 0; x < nSizeChangeBPM; x++)
	{
		double dSBeat = dBeat+double(nMiliseconds)*dChangeBPM[x]/60000;
		if((x+1) >= nSizeChangeBPM)
		{
			dBeat = dSBeat;
			break;
		}
		else
		{
			double dBeatMil = (dWChangeBPM[x+1]-dWChangeBPM[x])+dBeat;
			if(dBeatMil > dSBeat)
			{
				dBeat = dSBeat;
				break;
			}
			else
			{
				dBeat += dWChangeBPM[x+1]-dWChangeBPM[x];
				nMiliseconds -= (__int64)((dWChangeBPM[x+1]-dWChangeBPM[x])/dChangeBPM[x]*60000);
			}
		}
	}
	int nLight = int(double(dBeat*double(nTickCount)));
	if(nLight >= nSizeLights)
	{
		if(bLoop)
		{
			nLight = 0;
			dwNext = 0;
		}
		else
		{
			nLight = nSizeLights-1;
		}
	}
	int nData = 0;
	for(int x = 0; x < 3; x++)
	{
		nData |= bLights[nLight*3+x]<<x;
	}
	//nData = ~nData;
	portLights->WriteVal(nData);
}

void CMaxLights::SetZero()
{
	dwNext = 0;
	int nData = 0;
	for(int x = 0; x < 3; x++)
	{
		nData |= bLights[x]<<x;
	}
	//nData = ~nData;
	portLights->WriteVal(nData);
}

void CSKSFView::Score_DoJudgament(int y, int nGArrows, int nJudge, int nCombo)	// USER:DEF
{
	if(!nCombo) return;
	status.nLastEva = m_nNext;
	status.nTypeEva = nJudge;
	int n=nGArrows;
	switch(nJudge)
	{
	case JUDAMENT_BAD:
		status.nBad[n]++;
		status.nScore += PUNT_EVA4;	// USER
		status.nCombo[n] = 0;
		status.nComboTotal = 0;
		status.dLife -= dDecrBar*0.5;
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
		status.dLife += dIncrBar*0.5*double(nCombo);
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
		status.dLife += dIncrBar*double(nCombo);
		status.nMissCombo[n] = 0;
		status.nMissComboTotal = 0;
		break;
	case JUDAMENT_MISS:
		status.nMiss[n]++;
		status.nScore+=PUNT_EVA5;	// USER
		MinusCombo();
		status.dLife -= dDecrBar;
		status.nCombo[n] = 0;
		status.nComboTotal = 0;
		status.nMissCombo[n]++;
		status.nMissComboTotal++;
		if(status.nMissCombo[n] > status.nMaxMissCombo[n]) status.nMaxMissCombo[n] = status.nMissCombo[n];
		if(status.nMissComboTotal > status.nMaxMissComboTotal) status.nMaxMissComboTotal = status.nMissComboTotal;
		break;
	}
}

void CSKSFView::Score_DoItem(int y, int x, int nGArrows, int nType, int nPar)
{
	// USER:DEF
	double dMult = double(nPar);
	if(nPar < 0) dMult = 1/double(-nPar);
	switch(nType)
	{
	case 5:	// Minas
		status.nItems++;
		status.dLife -= double(nPar)*dDecrBar;
		status.nMines++;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nNext;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, sound_mine, 0, &channels[9]);
		break;
	case 6:	// Pociones
		status.nItems++;
		status.dLife += double(nPar)*dDecrBar;
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
		if(m_bDrop)
		{
			m_dClipMin1 = -m_dClipMin1;
			m_dClipMax1 = -m_dClipMax1;
			m_dClipMin2 = -m_dClipMin2;
			m_dClipMax2 = -m_dClipMax2;
			m_dClipMin3 = -m_dClipMin3;
			m_dClipMax3 = -m_dClipMax3;
			double dTemp;
			dTemp = m_dClipMin1;
			m_dClipMin1 = m_dClipMax1;
			m_dClipMax1 = dTemp;
			dTemp = m_dClipMin2;
			m_dClipMin2 = m_dClipMax2;
			m_dClipMax2 = dTemp;
			dTemp = m_dClipMin3;
			m_dClipMin3 = m_dClipMax3;
			m_dClipMax3 = dTemp;
		}
		m_bDrop = false;
		m_bXMode = false;
		m_bNXMode = false;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nNext;
		break;
	case 10:	// Velocidad x1
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 1.0;
		evarrows.dWWVel = dBeatnStop;
		evarrows.bVel = true;
		break;
	case 11:	// Velocidad x2
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 2.0;
		evarrows.dWWVel = dBeatnStop;
		evarrows.bVel = true;
		break;
	case 12:	// Velocidad x3
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 3.0;
		evarrows.dWWVel = dBeatnStop;
		evarrows.bVel = true;
		break;
	case 13:	// Velocidad x4
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 4.0;
		evarrows.dWWVel = dBeatnStop;
		evarrows.bVel = true;
		break;
	case 14:	// Velocidad x5
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 5.0;
		evarrows.dWWVel = dBeatnStop;
		evarrows.bVel = true;
		break;
	case 15:	// Velocidad x8
		status.nItems++;
		status.nVelocitys++;
		evarrows.dSourceVel = m_dUserVel;
		evarrows.dDestVel = 8.0;
		evarrows.dWWVel = dBeatnStop;
		evarrows.bVel = true;
		break;
	case 16:	// Drop
		status.nItems++;
		m_bDrop = true;
		if(m_bDrop)
		{
			m_dClipMin1 = -m_dClipMin1;
			m_dClipMax1 = -m_dClipMax1;
			m_dClipMin2 = -m_dClipMin2;
			m_dClipMax2 = -m_dClipMax2;
			m_dClipMin3 = -m_dClipMin3;
			m_dClipMax3 = -m_dClipMax3;
			double dTemp;
			dTemp = m_dClipMin1;
			m_dClipMin1 = m_dClipMax1;
			m_dClipMax1 = dTemp;
			dTemp = m_dClipMin2;
			m_dClipMin2 = m_dClipMax2;
			m_dClipMax2 = dTemp;
			dTemp = m_dClipMin3;
			m_dClipMin3 = m_dClipMax3;
			m_dClipMax3 = dTemp;
		}
		m_bUnderAttack = false;
		m_bLeftAttack = false;
		m_bRightAttack = false;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nNext;
		break;
	case 17:	// UnderAttack
		status.nItems++;
		if(m_bDrop)
		{
			m_dClipMin1 = -m_dClipMin1;
			m_dClipMax1 = -m_dClipMax1;
			m_dClipMin2 = -m_dClipMin2;
			m_dClipMax2 = -m_dClipMax2;
			m_dClipMin3 = -m_dClipMin3;
			m_dClipMax3 = -m_dClipMax3;
			double dTemp;
			dTemp = m_dClipMin1;
			m_dClipMin1 = m_dClipMax1;
			m_dClipMax1 = dTemp;
			dTemp = m_dClipMin2;
			m_dClipMin2 = m_dClipMax2;
			m_dClipMax2 = dTemp;
			dTemp = m_dClipMin3;
			m_dClipMin3 = m_dClipMax3;
			m_dClipMax3 = dTemp;
		}
		m_bDrop = false;
		m_bUnderAttack = true;
		m_bLeftAttack = false;
		m_bRightAttack = false;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nNext;
		break;
	case 18:	// LeftAttack
		status.nItems++;
		if(m_bDrop)
		{
			m_dClipMin1 = -m_dClipMin1;
			m_dClipMax1 = -m_dClipMax1;
			m_dClipMin2 = -m_dClipMin2;
			m_dClipMax2 = -m_dClipMax2;
			m_dClipMin3 = -m_dClipMin3;
			m_dClipMax3 = -m_dClipMax3;
			double dTemp;
			dTemp = m_dClipMin1;
			m_dClipMin1 = m_dClipMax1;
			m_dClipMax1 = dTemp;
			dTemp = m_dClipMin2;
			m_dClipMin2 = m_dClipMax2;
			m_dClipMax2 = dTemp;
			dTemp = m_dClipMin3;
			m_dClipMin3 = m_dClipMax3;
			m_dClipMax3 = dTemp;
		}
		m_bDrop = false;
		m_bUnderAttack = false;
		m_bLeftAttack = true;
		m_bRightAttack = false;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nNext;
		break;
	case 19:	// RightAttack
		status.nItems++;
		if(m_bDrop)
		{
			m_dClipMin1 = -m_dClipMin1;
			m_dClipMax1 = -m_dClipMax1;
			m_dClipMin2 = -m_dClipMin2;
			m_dClipMax2 = -m_dClipMax2;
			m_dClipMin3 = -m_dClipMin3;
			m_dClipMax3 = -m_dClipMax3;
			double dTemp;
			dTemp = m_dClipMin1;
			m_dClipMin1 = m_dClipMax1;
			m_dClipMax1 = dTemp;
			dTemp = m_dClipMin2;
			m_dClipMin2 = m_dClipMax2;
			m_dClipMax2 = dTemp;
			dTemp = m_dClipMin3;
			m_dClipMin3 = m_dClipMax3;
			m_dClipMax3 = dTemp;
		}
		m_bDrop = false;
		m_bUnderAttack = false;
		m_bLeftAttack = false;
		m_bRightAttack = true;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nNext;
		break;
	case 20:	// Snake
		status.nItems++;
		m_bSnake = true;
		evarrows.bHidden = true;
		evarrows.nHidden = m_nNext;
		break;
	}
}
