#include "cmnpiu.h"

int LoadSong(ARGSTEP arg);
int LoadSongCompile(ARGSTEP arg, int nPlayer);
void DeleteSong();
void DeleteSongCompile(int nPlayer);
void Event_Step(int nEvent, int nKey);
int Draw_All();
void DeleteContextStep();
void Go_Step();

class CSKSFView
{
public:
	CSKSFView();
	~CSKSFView();

	// Auxiliadores
	void DeriveOpacityPos(double t, double* o, int nGArrows);
	void SetOffset(double* l, double* t, double* r, double* b);
	void GetLimitsOfDrawing(double dBeat, int nGArrow, int* nMin, int* nMax);
	void ModifyPosArrows(double* l, double* t);
	double TimeToBeat(__int64 dwMiliseconds, int* nStop, double* dModStop, double* dActualBPM);
	__int64 BeatToTime(double dBeat);
	void ActualizateVel(double dBeat);
	int GetArrowStatus(int x, int y, int nGArrows);
	void SetArrowStatus(int x, int y, int nGArrows, int nValue);
	int GetAccessSkin(int x);
	bool SomeArrow(int y, int nGArrow);
	bool SomeObject(int y, int nGArrow);
	bool SomeNote(int y, int nGArrow);
	void Score_DoJudgament(int y, int nGArrows, int nJudge, int nCombo);
	void Score_DoItem(int y, int x, int nGArrows, int nType, int nPar);

public:
	// Funciones de la clase
	SONGCOMPILER songc;
	int ox;
	int oy;
	int LoadTextures(void);
	__int64 m_nNext;
	double m_dUserVel;
	double m_dMachVel;
	double m_dActionVel;
	bool m_bDrop;
	double m_nSizeArrows;
	EVENTARROWS evarrows;
	SONGSTATUS status;
	double m_dLevelMach;
	CString m_strFileMusic;
	CSkin* skin[3];
	CSkinCommon* common;
	CSkinBS* bs;
	CGLFont* bsf;
	int m_nLevelMach;

public:
	int CompileSong(void);
	int DeleteSong(void);
	DWORD GetArrow(int x, int y, int nGArrow);
	double BeatArrow(int nIndex, int nGArrow);
	double OffsetInZArrow(int x);
	bool LoadMachine(CString m_strFile);
	MACHINE machine;
	void DeleteMachine(void);
	bool m_bLoadMachine;
	bool m_bTrainingMode;
	int CompileTraining(void);
	int GetPivotSub(int x);
	double BeatWArrow(int nIndex, int nGArrow);
	TRAINERCOMPILER trac;
	void DeleteTrainingCompiler(void);
	int GetNumberPivotArrows(int x);
	int GetNumberAsignedArrows(int x);
	int GetNextPivotSub(int x, int n);
	int GetNextPivotArrowsNC(int x);
	CHARACTER crt;
	bool CompileCharacter(void);
	void DeleteCharacter(void);
	int m_nCharacter;
	bool m_bCharacter;
	int m_nAuxBeginCharacterStepL;
	int m_nAuxBeginCharacterStepR;
	BSCOMPILER bsc;
	CString strBSDisp[50];
	void GQMath1(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQMath2(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQMath3(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQMath4(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQMath5(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQMemo1(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQMemo2(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQMemo3(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQMemo4(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQMemo5(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQObse1(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQObse2(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQObse3(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQObse4(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQObse5(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQAnal1(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQAnal2(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQAnal3(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQAnal4(int y, int nIndex, int nResp, int nArg1, int nArg2);
	void GQAnal5(int y, int nIndex, int nResp, int nArg1, int nArg2);
	int CompileBS(void);
	void DeleteBS(void);
	void DrawBS01(int y);
	void DrawBS02(int y);
	void DrawBS03(int y);
	void DrawBS04(int y);
	void DrawBS05(int y);
	void DrawBS06(int y);
	void DrawBS07(int y);
	void DrawBS08(int y);
	int m_nAttack;
	bool m_bXMode;
	bool m_bNXMode;
	bool m_bHJ;
	bool m_bRS;
	bool m_bMirror;
	bool m_bFreedom;
	bool m_bRV;
	bool m_bRandomSkin;
	int m_nSkin;
	int m_nVanish;
	bool m_bDC;
	bool m_bAC;
	bool m_bEW;
	bool m_bUnderAttack;
	double m_dPassVel;
	bool m_bRVel;
	bool m_bFlash;
	double dBeat;
	double dBeatnStop;
	bool m_bIsSingle;
	bool m_bLeftAttack;
	bool m_bRightAttack;
	bool m_bSnake;
	EFFECT_COMP effect;
	int CompileEffects(void);
	void DeleteEffects(void);
	void ModifyPosArrows2(double* l, double* t, int x, int y, int nGArrow);
	void DeriveOpacityPos2(double t, double* o, int y, int nGArrow);
	void UpdateAttack(void);
	void AllocStatus(void);
	void DeleteStatus(void);

	int StepCompile();
	void StepDestroy();
	void Draw();

	void Event_View(int nEvent, int nKey);
	int AddCharacterAutoPlay(double dWWArrow, int nArrow, int nType);
	double EstimateStep(double dWWArrow);
	void PlusCombo(void);
	void MinusCombo(void);

	int m_nPlayer;
	bool m_bAutoPlay;

	// Incluidos desde SKSFDoc
	double m_dClipMax1;
	double m_dClipMax2;
	double m_dClipMax3;
	double m_dClipMin1;
	double m_dClipMin2;
	double m_dClipMin3;
	bool m_bIsArrowsCountable[3];
	bool m_bIsArrowsMisible[3];
	int m_nOffset;
	bool m_bGetArrowInStop;

	// Auxiliar de acceso
	bool m_bIsCharged;
	int m_nHlpAuto;
	int m_nLevelCharacter;
	int m_nLevelSong;
	int m_nTypeCharacter;
	double dIncrBar;
	double dDecrBar;
	double dPowTimer;

	// Auxiliares de puntaje
	int PUNT_EVA1;
	int PUNT_EVA2;
	int PUNT_EVA3;
	int PUNT_EVA4;
	int PUNT_EVA5;
};

class CSKSFDoc
{
public: 
	CSKSFDoc();
	~CSKSFDoc();
	int LoadFile(LPCTSTR strFile);
	void CloseFile();
	
public:
	SONG song;
	DWORD GetArrow(int x, int y, int nGArrow);
	double BeatArrow(int nIndex);
	int VerificateBeatSplitChange(int nIndex);
	int VerificateBPMChange(int nIndex);
	int VerificateVelChange(int nIndex);
	int VerificateTickCountChange(int nIndex);
	int VerificateStopChange(int nIndex);
	int VerificateJumpChange(int nIndex);
	int VerificateJumpChangeEq(int nIndex);
	bool SomeArrow(int y, int nGArrow);
	bool SomeObject(int y, int nGArrow);
	bool SomeNote(int y, int nGArrow);
	bool IsArrowsMisible(int nGArrows);
	bool IsArrowsCountable(int nGArrows);
	bool SomeBSQuestion(int y, int nGArrow);
	bool SomeBSAnswer(int y, int nGArrow);
	bool SomeBSPopUp(int y, int nGArrow);
	bool SomeEffect(int y, int nGArrow);
	bool b;
};

class CMaxLights
{
public: 
	CMaxLights();
	~CMaxLights();

public:
	int nSizeLights;
	BYTE* bLights;
	int nSizeChangeBPM;
	double* dChangeBPM;
	double* dWChangeBPM;
	int nTickCount;
	__int64 dwOffset;
	__int64 dwNext;
	bool bLoop;
	int Open(LPCTSTR strFile);
	void Close();
	void Update(DWORD dwMilliseconds);
	void SetZero();
};