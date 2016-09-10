#ifndef MAX_STEPSMU
#define MAX_STEPSMU

#include "GLFont.h"
#include "Skin.h"
#include "SkinCommon.h"
#include "SkinBS.h"
#include "SoundRes.h"
#include "argstep.h"
#include "main_document.h"
#include "MaxGenericBGA.h"

// PIU-SPECIFIC
#define ARROWSLINE_SINGLE 5
#define ARROWSLINE_DOUBLE (ARROWSLINE_SINGLE*2)
// 1 MEASURE PER RANDOMVELOCITY
#define RV_BEATS_EVENT 4
#define RV_MIN_VEL 1
#define RV_MAX_VEL 5
#define ITEM_STARTUP 5

const int g_nAssignMirror[] = {1, 0, 2, 4, 3};
const int g_nAssignMirrorSize = sizeof(g_nAssignMirror)/sizeof(int);

// DRAW-SPECIFIC
#define TRANSLATE_INITIAL_PLAYER -20.0
#define TRANSLATE_NEXT_PLAYER 40.0
#define TRANSLATE_Y_PLAYER 21.0
#define ZOFFSET_ARROWS -50.0
#define SIZE_ARROWS (51.0/8.0)
#define DIST_ARROWS (SIZE_ARROWS*0.95)
#define SIZE_DISPLAY_ARROWS (60.0)	// Used for AC/DC
#define TOP_DRAW 30.0
#define BOTTOM_DRAW -30.0
#define LEFT_DRAW -40.0
#define RIGHT_DRAW 40.0
#define VANISH_TRANSITION 10.0

// The GU comes from Glow User
#define DRAWGU_TIME 500
#define DRAWGU_SCALEBEGIN 1.0
#define DRAWGU_SCALEEND 1.1
#define DRAWGU_OPACITYBEGIN 1.0
#define DRAWGU_OPACITYEND 0.0

// Glow when you are ok with a arrow
#define DRAWGLOW_TIME 500
#define DRAWGLOW_SCALEBEGIN 3.0
#define DRAWGLOW_SCALEEND 3.1
#define DRAWGLOW_OPACITYBEGIN 1.0
#define DRAWGLOW_OPACITYEND 0.0

// skins definitions
#define SKIN_TRAIN_CENTER 3
#define SKIN_TRAIN_LEFT 1
#define SKIN_TRAIN_RIGHT 2

// EVA definitions
#define EVA_1_EVENT_TIME 100
#define EVA_2_EVENT_TIME 500
#define EVA_3_EVENT_TIME 500

#define MAX_BAR 1.2

enum STEPSMUSRES
{
	STEPSMUSRES_MINE,
	STEPSMUSRES_ASSISTTICK,
	STEPSMUSRES_RIGHT,
	STEPSMUSRES_WRONG,
	STEPSMUSRES_SIZE
};

enum JUDAMENT_ENUM
{
	JUDAMENT_PERFECT,
	JUDAMENT_GREAT,
	JUDAMENT_GOOD,
	JUDAMENT_BAD,
	JUDAMENT_MISS
};

#define JUDGAMENT_DRAW_LOSE JUDAMENT_GOOD

enum MODE_PLAYER
{
	MODE_PLAYER_NULL,
	MODE_PLAYER_SINGLE,
	MODE_PLAYER_DOUBLE,
	MODE_PLAYER_SIZE
};

struct STEPSTATUS
{
	int *nPerfect;			// Perfect
	int *nGreat;			// Great
	int *nGood;				// Good
	int *nBad;				// Bad
	int *nMiss;				// Miss
	int *nCombo;			// Combo actual
	int *nMaxCombo;			// Max Combo
	int *nMissCombo;		// Combo actual
	int *nMaxMissCombo;		// Max Combo

	int nComboTotal;
	int nMaxComboTotal;
	int nMissComboTotal;
	int nMaxMissComboTotal;

	int nScore;				// Puntaje de la canción
	int nMines;				// Minas
	int nHearts;			// Corazones
	int nPotions;			// Pociones
	int nVelocitys;			// Velocidades
	int nItems;				// Total items
	int nBSCorrect;			// Brain Shower Correctas
	int nBSIncorrect;		// Brain Shower Incorrectas

	double dLife;			// Barra de vida

	int** nArrowsStatus;	// 5 Perfect, 4 great, 3 bad, 2 good, & 1 miss; 0 NO ASIGNADO
	int* nBSStatus;			// 2 Correct, 1 incorrect, 0 NO ASIGNADO
};

struct EVENTARROWS
{
	bool bArrows[10];
	bool bCredit[2];
	bool bService;
	bool bOption;
	__int64 nArrows[10];
	__int64 nGlowArrows[10];

	// Action-Vel issue
	bool bVel;
	float dSourceVel;
	float dDestVel;
	float dWWVel;

	bool bHidden;
	__int64 nHidden;

	__int64 nLastEva;		// Ultimo momento en el cual apareció un PERFECT, GREAT, BAD...
	int nTypeEva;			// Tipo del último pop que ha salido
	__int64 nLastBSPop;		// Ultimo pop de Brain Shower
	int nIndexBSPop;		// Numero del pop Brain Shower
	__int64 nLastBSQs;		// Ultima pregunta de Brain Shower
	int nIndexBSQs;			// Numero de la pregunta Brain Shower
	__int64 nLastBSEva;		// Ya sabe
	int nTypeBSEva;			// Tipo de evaluacion
};

struct TRAINERCOMPILER
{
	int nSizeArg;
	int* nArg;
};

struct BSCOMPILER
{
	int nSizeBSQuestions;
	float *dWIBSQuestions;	// Momentos Iniciales en los que salen las preguntas de Brain shower
	float *dWEBSQuestions;	// Momentos Finales en los que salen las preguntas de Brain shower
	float *dWABSQuestions; // Argumento opcional (Para memoria, observacion & Analisis)
	DWORD *dwBSQuestions;	// Argumentos de la pregunta (IDMateria+IDPregunta)
	DWORD *dwDBSQuestionsEx;// Lo que dice la pregunta de BS extendido
	DWORD *dwDBSQuestions;	// Lo que dice la pregunta de BS

	int nSizeBSAnswers;
	float *dWBSAnswers;	// Momentos en los que salen las respuestas (Beat-Draw)
	__int64 *nWWBSAnswers;	// Momentos en los que salen las respuestas (Beat-Time)
	DWORD *dwBSAnswers;		// Argumentos de las respuestas (IDSkin|IDItem)
	int *nBSAnswers;		// Argumentos de las respuestas (IDSkin|IDItem)
	int *nDBSAnswers;		// Modo de display (1 si deben aparecer números)

	int nSizeBSPopUp;
	float* dWBSPopUp;
	DWORD* dwBSPopUp;
};

typedef STEPMODHEADER STEPCOMPILERMODHEADER;
typedef STEPGROUPHEADER STEPCOMPILERGROUPHEADER;	// Is the same
typedef STEPHEADER STEPCOMPILERHEADER;	// Is the same

struct STEPCOMPILERMODINFO
{
	int *nModInt;		// Parametros modificador (entero, se dejan las ubicaciones)
	float *fModFloat;		// Parametros modificador (coma flotante)
	float fWTMod[2];		// Las dos ubicaciones traducidas a Beats-Tiempo
	__int64 nWTMod[2];		// Las dos ubicaciones traducidas a Tiempo
};

struct STEPCOMPILERMOD
{
	STEPCOMPILERMODHEADER hMod;
	STEPCOMPILERMODINFO* hModInfo;
};

struct STEPCOMPILERGROUP
{
	STEPCOMPILERGROUPHEADER hGroup;

	float *fWDArrows;	// Ubicacion de las flechas en (beats-dibujo)
	__int64 *nWTArrows;	// Ubicacion de las flechas en (tiempo)
	int *nArrows;		// Flechas
	int *nPArrows;		// Parametros de flechas
};

// ** ATENCION: EL USO DE BEATS EN ESTE COMPILADOR ES

/*

Beats-Editor - Ubicación basada en el editor (Sin Stops ni jumps)
Beats-Dibujo - Ubicación donde se debe dibujar (Parece que solo aplica a lo que se DEBE dibujar)

Existe un concepto llamado Beat-Tiempo que depende solo del BPM y el tiempo

*/

struct STEPCOMPILER
{
	STEPCOMPILERHEADER hStep;

	float* fAttrib;		// Atributos de la canción (como Voltaje, Resistencia...)

	STEPCOMPILERGROUP* hStepGroup; // Grupos
	STEPCOMPILERMOD* hMod;		// Modificadores

	// ** LOS BEATSPLITS NO SE TRADUCEN

	__int64 *nWTChangeBPM;	// Momentos en el cual ocurre un Cambio de BPM (En Tiempo)
	float *fWTChangeBPM;	// Momentos en el cual ocurre un Cambio de BPM (En Beats-Tiempo)
	float *fChangeBPM;	// Cambios de BPM

	float *fITChangeVel;	// Momentos iniciales en el cual ocurre un Cambio de Velocidades (En Beats-Tiempo)
	float *fETChangeVel;	// Momentos finales en el cual ocurre un Cambio de Velocidades (En Beats-Tiempo)
	float *fChangeVel;	// Cambios de Velocidades
	float *fIChangeVel;	// Inicio de la velocidad

	float *fWTJumps;		// Ubicaciones saltos de canción (en Beats-Tiempo)
	float *fJumps;		// Saltos de canción (en Beats-Tiempo)

	// ** LOS TICKCOUNTS NO SE TRADUCEN

	float *fWTStops;		// Ubicación Stops de canción (en Beats-Tiempo)
	float *fStops;		// Stops de canción (en Beats-Tiempo)
};

class CMaxStepSmu
{
public:
	CMaxStepSmu(void);
	~CMaxStepSmu(void);

	// Variables varias no reconocidas
	int ox;
	int oy;

	// Variables de estado de los comandos
	bool m_bDrop;
	float m_dUserVel;
	float m_dMachVel;
	float m_dActionVel;
	int m_nLevelMach;		// Timing (indexed)
	bool m_bTrainingMode;
	int m_nAttack;
	bool m_bXMode;
	bool m_bNXMode;
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
	float m_dPassVel;
	bool m_bFlash;
	int m_nModePlay;
	bool m_bLeftAttack;
	bool m_bRightAttack;
	bool m_bSnake;
	int m_nPlayer;
	bool m_bAutoPlay;
	int m_nHlpAuto;
	float m_dIncrBar;
	float m_dDecrBar;

	float m_fEVanish;
	float m_fIVanish;

	float m_dSizeArrows;
	float m_dDistArrows;

	int m_nSizeJudTimes;
	int *m_nJudTimes;

	// DEPRECATED

	// ASSIST TICK
	bool m_bIsAsistTick;
	int m_nSizeATArray;
	BYTE* m_bATArray;
	__int64* m_nWTATArray;
	void DoAssistTick(void);
	void CompileAssistTick(void);
	int IncrAssistTickOrVerificate(__int64 nTime);


	// PIUCONFIG-SPECIFIC
	int m_nModeMach;

	static CString m_strFileMusic;
	static bool m_bIsMusic;
	static CSoundStream* m_ssMusic;
	static float m_fRush;

	static CString m_strFileBGA;
	static bool m_bIsBGA;
	static CMovieStream2* m_msBGA;

	static CString m_strFileSplash;
	static bool m_bIsSplash;
	static CGraphTexture* m_gtSplash;

	static CMaxGenericBGA* m_gbBGA;

	static CSoundRes* m_srMain;
	static int LoadResources(LPCTSTR strMusic, LPCTSTR strBGA, LPCTSTR strSplash);
	static void DelResources(void);
	static void DrawBGA(void);
	static void DrawSplash(void);
	static void UpdateSong(void);
	static void LaunchSong(void);
	static void LaunchSong(unsigned int nPos);
	static void StopSong(void);
	static unsigned int m_nPos;

	// Variables vitales
	__int64 m_nTime;	// Tiempo?
	float m_dBeatTime;
	float m_dBeatDraw;

	// Documento asociado
	CMaxDocument* pDoc;

	// El step compilado (Ubicaciones pasadas a beats)
	STEPCOMPILER stepc;

	// El estado del step
	STEPSTATUS status;

	// El training compilado
	TRAINERCOMPILER trac;

	// El BrainShower compilado
	BSCOMPILER bsc;
	CString strBSDisp[50];

	// Funciones del Brain Shower
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

	// Variables de eventos
	EVENTARROWS evarrows;	// Estructura que guarda tiempos de flechas y algo mas?
	bool m_bIsLoaded;

	// Funciones de Carga/Descarga
	int LoadTextures(void);
	void DeleteTextures(void);
	int CompileSong(void);
	int DeleteSong(void);
	int CompileTraining(void);
	void DeleteTrainingCompiler(void);
	int CompileBS(void);
	void DeleteBS(void);
	void AllocStatus(void);
	void DeleteStatus(void);
	int StepCompile();
	void StepDestroy();
	void SetArg(ARGSTEP* arg);

	// Dibujos
	void Draw();

	void DrawStep(void);
	void DrawBS(void);

	void DrawArrow(double dX, double dY, double dZ, double dScale, double dRotate, CGraphTexture* tex);	// General DrawArrow
	void DrawArrowGuide(int nLimitX, int* nOrder, double o);	// Draw the arrow guide
	void DrawArrowGlow(int nLimitX, int* nOrder);	// Draw the arrow glow

	void DrawArrowOp(double dX, double dY, double dZ, double dScale, double dRotate,
		double dYOpSup, double dYOpInf, double dOpLen, double dOpMult, CGraphTexture* tex);	// General DrawArrow With Opacity

	void DrawHoldOp(double dX, double dY_top, double dY_bot, double dZ, double dScale, double dRotate,
		double dYOpSup, double dYOpInf, double dOpLen, double dOpMult, CGraphTexture* tex);	// General DrawHold With Opacity

	void DrawBottomHoldOp(double dX, double dY_top, double dY_bot, double dZ, double dScale, double dRotate,
		double dYOpSup, double dYOpInf, double dOpLen, double dOpMult, CGraphTexture* texHold, CGraphTexture* tex);

	void DrawObject(double t, double l, double b, double r,
					double dX, double dY, double dZ, double dScale, double dRotate,
					CGraphTexture* tex);

	void DrawObjectOp(double t, double l, double b, double r,
					double dX, double dY, double dZ, double dRotate,
					double dYOpSup, double dYOpInf, double dOpLen, double dOpMult, bool bVRaster, CGraphTexture* tex);

	void DrawObjectOp(double t, double l, double b, double r, double v_t, double v_b,
					double dX, double dY, double dZ, double dRotate,
					double dYOpSup, double dYOpInf, double dOpLen, double dOpMult, bool bVRaster, CGraphTexture* tex);

	float UbiXArrow(int nPos, int nLimitX, float dDistBetArrows, float dSizeArrows, float dBeat);
	float UbiYArrow(int nPos, int nLimitX, float dDistBetArrows, float dSizeArrows, float dBeat);

	// Dibujo del brainShower

	void DrawBS01(int y);
	void DrawBS02(int y);
	void DrawBS03(int y);
	void DrawBS04(int y);
	void DrawBS05(int y);
	void DrawBS06(int y);
	void DrawBS07(int y);
	void DrawBS08(int y);

	// Eventos
	void Event_View(int nEvent, int nKey);

	// Objetos de dibujo
	CSkin** skin;
	CSkin* skintrain[3];
	CSkinCommon* common;
	CSkinBS* bs;
	CFont* bsf;

	// Funciones de Steps
	int GetArrow(int x, int y, int nGArrow);
	int GetArrowPar(int x, int y, int nGArrow);	// USER:DEF

	// Auxiliares de tiempo
	float TimeToBeat(__int64 nTime);
	float BeatT2BeatD(float dBeatTime);	// USER:DEF
	__int64 BeatToTime(float dBeatTime);
	__int64 BeatWArrow(int nIndex, int nGArrows);

	// Auxiliares de dibujo
	void GetLimitsOfDrawing(float dBeatDraw, int nGArrow, int* nMin, int* nMax);
	void UpdateVel(float dBeat);

	// Auxiliadores (Unknown)
	int IncrStepOrVerificate(int n, __int64 nUbiT, float fUbiD);
	int IncrStep(int n);
	void SortStepGroup(int n);
	int IncrBSAnswer(int y);

	int GetArrowStatus(int x, int y, int nGArrows);
	void SetArrowStatus(int x, int y, int nGArrows, int nValue);
	int GetAccessSkin(int x);
	bool SomeArrow(int y, int nGArrow);
	bool SomeObject(int y, int nGArrow);
	bool SomeNote(int y, int nGArrow);
	int GetPivotSub(int x);
	int GetNumberPivotArrows(int x);
	int GetNumberAsignedArrows(int x);
	int GetNextPivotSub(int x, int n);
	int GetNextPivotArrowsNC(int x);
	double EstimateStep(double dWWArrow);

	void PlusCombo(void);
	void MinusCombo(void);
	int PUNT_EVA1;
	int PUNT_EVA2;
	int PUNT_EVA3;
	int PUNT_EVA4;
	int PUNT_EVA5;

	void InitCompiler(STEPCOMPILER* fsong);
	void ReInitCompiler(STEPCOMPILER* fsong);
	void DeleteCompiler(STEPCOMPILER* fsong);

	// Funciones posiblemente reemplazables
	double BeatArrow(int nIndex, int nGArrow);
	double OffsetInZArrow(int x);

	// Funciones de puntaje
	void Score_DoJudgament(int y, int nGArrows, int nJudge, int nCombo);	// USER:DEF
	void Score_DoItem(int y, int x, int nGArrows, int nType, int nPar);	// USER:DEF
	void Score_DoNotItem(int y, int x, int nGArrows, int nType, int nPar);	// USER:DEF

	// Funciones de reconocimiento
	void StepEvaluate(int nLimitX, int* nMin, int* nMax, int nLevelMach);
	void BSEvaluate(int nLimitX, int nLevelMach);

	// Variables y funciones pertenecientes al ICharacter (despreciadas)
	//MACHINE machine;
	//bool LoadMachine(CString m_strFile);
	//void DeleteMachine(void);
	//bool m_bLoadMachine;
	//CHARACTER crt;
	//bool CompileCharacter(void);
	//void DeleteCharacter(void);
	//int m_nCharacter;
	//bool m_bCharacter;
	//int m_nAuxBeginCharacterStepL;
	//int m_nAuxBeginCharacterStepR;
	//int AddCharacterAutoPlay(double dWWArrow, int nArrow, int nType);
	//int m_nLevelCharacter;
	//int m_nTypeCharacter;

	// Variables y funciones no funcionales
	//int CompileEffects(void);
	//void DeleteEffects(void);
	//void UpdateAttack(void);
};

#endif
