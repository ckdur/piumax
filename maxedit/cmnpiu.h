#ifndef CMN_PIU
#define CMN_PIU 1

#include "stdafx.h"

#include "MID.h"

#ifdef PIUMAX_2
#include "intdef.h"
#include "resdef.h"
#include "MainTheme.h"
#endif
#include "eventdef.h"

#include "OGLSprite.h"
#include "MovieStream2.h"

// ** Definición del lua reader
#include "MaxLua.h"

// ** Definición principal de gestores
#include "GraphMain.h"

#include "SoundSys.h"
#include "StdInput.h"


#define MAX_CHANNELS 32

extern CGraphMain* g_gmMain;
extern CSoundSys *g_ssMain;

extern CSoundChannel* channels[MAX_CHANNELS];

extern CGraphTexture* g_gtNoTex;

//extern CMainTheme* g_mtTheme;

// ** DEFINICION DE BASICOS
void SetCurDir();

#ifdef UNICODE
#define SetCurMethod(family, method) wcscpy(chMethod, method); wcscpy(chFamily, family)
#else
#define SetCurMethod(family, method) strcpy(chMethod, method); strcpy(chFamily, family)
#endif

extern TCHAR* tc_MIND_TYPE[];

extern TCHAR chMethod[32];
extern TCHAR chFamily[32];

extern DWORD dwTick;
extern DWORD dwLastTick;
extern DWORD dwStepSmu;

struct PIUCONFIG
{
	int nLevelMach;
	int nBarLevel;
	int nStageBreak;
	int nLevelJud;
	int nTimePlay;
	int nCoinsCred1;
	int nCoinsCred2;
	int nModeMach;
	int nStageLimit;
	int nButtons[10][4];
	int nModeButtons[10][4];	// 0 Button, 1/2/3/4 up/left/down/right hat
	int nWhereButtons[10][4];	// -1 Teclado, -2 no asignado
	int nServiceButton[3];
	int nConfigButton[3];
	int nCoinButton1[3];
	int nCoinButton2[3];
	BYTE nSoundPrev;
	BYTE nRegSongsGame1[1000];
	BYTE nRegSongsGame3[1000];
};

extern PIUCONFIG piuconfig;

#ifdef PIUMAX_2
struct ANUMAX	//Animacion piu maX
{
	__int64 nDur;
	int nSizeObj;
	CString* strFile;
	double* dSizeX;
	double* dSizeY;
#ifdef ANUCREATOR
	GLuint* nTexObj;
#else
	CSprite** skin;
#endif

	int nSizeAnu;
	int* nObjRes;
	int* nSizeChk;
	double** dUbiX;
	double** dUbiY;
	double** dScaleX;
	double** dScaleY;
	double** dAngle;
	DWORD** dRGB;

	BYTE** bDraw;
	__int64** nBegAnu;
	__int64** nDurAnu;
	double** dExpTUbiX;
	double** dExpTUbiY;
	double** dExpTScaleX;
	double** dExpTScaleY;
	double** dExpTAngle;
	double** dExpTRGB;
};

#endif // PIUMAX_2

//extern CPortBuf* portLights;

/*extern int nTotalCredit;
extern int nCredit;
extern int nSubCredit1;
extern int nSubCredit2;
extern bool bArrows[10];
#ifndef ANUCREATOR
extern int np[2];
extern bool bSaveUsbActive[2];
extern int nGame;
extern GLuint m_uNullTex;
extern FMOD_SOUND *sound_preStart;
extern FMOD_SOUND *sound_start;
extern FMOD_SOUND *sound_pulse;
extern FMOD_SOUND *sound_change1;
extern FMOD_SOUND *sound_change2;
extern FMOD_SOUND *sound_change3;
extern FMOD_SOUND *sound_switch;
extern FMOD_SOUND *sound_credit;
extern FMOD_SOUND *sound_subcred;
extern FMOD_SOUND *sound_lock;
extern FMOD_SOUND *sound_lockoff;
extern FMOD_SOUND *sound_option;
//extern FMOD_CHANNEL *channels;
#endif // ANUCREATOR
extern TCHAR chLang[3];
extern int nInterAfterUsb;

extern int nSizeTotalGame1;
extern int nSizeTotalGame2;
extern int nSizeTotalGame3;
extern int nSizeTotalGame4;
extern int nSizeTotalGame5;

extern int nSizeTotalGame1Ex;
extern int nSizeTotalGame2Ex;
extern int nSizeTotalGame3Ex;
extern int nSizeTotalGame4Ex;
extern int nSizeTotalGame5Ex;

extern int nChaTotalSongsGame4[1000];

extern TCHAR chStdModes[10][5];
extern TCHAR chSpeModes[5][5];
extern TCHAR chBraModes[5][5];
extern bool g_bPlayer[];
extern bool g_bIsMusic;

extern int nRankStepEval[2];

extern bool bSaveUsb;
extern int bUsb[2];
extern TCHAR chNameProfUsb[2][9];
extern GLuint uTexSplash[7];

extern __int64 nGeneralCount;

extern __int64 nTimeShowSesionTray[2];
extern __int64 nTimeShowSesionTrayMileage[2];
extern __int64 nTimeShowSesionTrayExp[2];
extern bool bShowSesionTray[2];
extern bool bShowSesionTrayMileage[2];
extern bool bShowSesionTrayExp[2];

extern int nTotalScorePlayer[2];

extern GLuint uTexPlayerSplash[2];
extern DWORD dwWait;

#ifndef ANUCREATOR


extern int nCurStage;
#endif

void ClearDraw();

int SaveMachineProfile();
int AdaptMachineProfile(int nMode, int nPlayerd);
void ConfigGetDefaults(int nCaret);
void PlusCredit(int nSet);
int GenDefPlayer();
int LoadMachineProfile();
int GenDefMach();
void PlusStagePlayer(int nPlay);
void PlusExpPlayer(int nPlayer, int nLvl, double dApt);
void PlusTotalCredit();

void ShowSesionTray(int nPlayer);
void ShowSesionTrayMileage(int nPlayer);
void ShowSesionTrayExp(int nPlayer);
void HideSesionTray(int nPlayer);
void HideSesionTrayMileage(int nPlayer);
void HideSesionTrayExp(int nPlayer);
void SetVolume(FMOD_BOOL bIsZero);

struct EVENTARROWS
{
	bool bArrows[10];
	bool bCredit[2];
	bool bService;
	bool bOption;
	__int64 nArrows[10];
	__int64 nGlowArrows[10];
	bool bVel;
	double dSourceVel;
	double dDestVel;
	double dWWVel;
	bool bHidden;
	__int64 nHidden;
};
struct SONGSTATUS
{
	int nPerfect[3];		// Perfect
	int nGreat[3];			// Great
	int nGood[3];			// Good
	int nBad[3];			// Bad
	int nMiss[3];			// Miss
	int nCombo[3];			// Combo actual
	int nMaxCombo[3];		// Max Combo
	int nMissCombo[3];		// Combo actual
	int nMaxMissCombo[3];	// Max Combo
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
	__int64 nLastEva;		// Ultimo momento en el cual apareció un PERFECT, GREAT, BAD...
	int nTypeEva;			// Tipo del último pop que ha salido
	__int64 nLastBSPop;		// Ultimo pop de Brain Shower
	int nIndexBSPop;		// Numero del pop Brain Shower
	__int64 nLastBSQs;		// Ultima pregunta de Brain Shower
	int nIndexBSQs;			// Numero de la pregunta Brain Shower
	__int64 nLastBSEva;		// Ya sabe
	int nTypeBSEva;			// Tipo de evaluacion

	int* nArrowsStatus1;	// 5 Perfect, 4 great, 3 bad, 2 good, & 1 miss; 0 NO ASIGNADO
	int* nArrowsStatus2;	// 5 Perfect, 4 great, 3 bad, 2 good, & 1 miss; 0 NO ASIGNADO
	int* nArrowsStatus3;	// 5 Perfect, 4 great, 3 bad, 2 good, & 1 miss; 0 NO ASIGNADO
	int* nBSStatus1;		// 2 Correct, 1 incorrect, 0 NO ASIGNADO
};
struct STEPHEADER
{
	DWORD dwVersion;	// Version de los steps
	DWORD dwLSong;		// Largo de la canción (en ms)
	int nOffset;		//

	double dVoltage;	// Voltaje
	double dStrength;	// Resistencia
	double dFreeze;		// Freeze
	double dCaos;		// Caos
	double dAir;		// Aire

	int nDificulty;		// Dificultad (De 1 a 99)

	int nSizeArrows1;	// Tamaño de las flechas 1
	int nSizeArrows2;	// Tamaño de las flechas 2
	int nSizeArrows3;	// Tamaño de las flechas 3
	int nSizeBeatSplit;	// Tamaño de los Beat Split
	int nSizeChangeBPM;	// Tamaño de los Cambios de BPM
	int nSizeChangeVel;	// Tamaño de los Cambios de Velocidades
	int nSizeJumps;		// Tamaño de saltos de canción
	int nSizeTickcount;	// Tamaño de los tickcounts
	int nSizeStops;		// Tamaño de los stops

	bool bGetArrowInStop;	// Coger flechas despues de un stop?
	DWORD dwArgArrows1;	// Argumentos de las flechas 1
	DWORD dwArgArrows2;	// Argumentos de las flechas 2
	DWORD dwArgArrows3;	// Argumentos de las flechas 3
	int nSkin1;			// Skin de el grupo de flechas 1
	int nSkin2;			// Skin de el grupo de flechas 2
	int nSkin3;			// Skin de el grupo de flechas 3
	double dClipMin1;	// Clip minimo de las flechas 1
	double dClipMin2;	// Clip minimo de las flechas 2
	double dClipMin3;	// Clip minimo de las flechas 3
	double dClipMax1;	// Clip maximo de las flechas 1
	double dClipMax2;	// Clip maximo de las flechas 2
	double dClipMax3;	// Clip maximo de las flechas 3
};

struct SONG
{
	FMOD_SOUND* music;
	STEPHEADER hStep;

	DWORD *dwArrows1;	// Flechas 1
	DWORD *dwArrows2;	// Flechas 2
	DWORD *dwArrows3;	// Flechas 3

	int *nWBeatSplit;	// Momentos en el cual ocurre un BeatSplit (en Arrows)
	int *nBeatSplit;	// Beat Splits (En Arrows)

	int *nWChangeBPM;	// Momentos en el cual ocurre un Cambio de BPM (En Arrows)
	double *dChangeBPM;	// Cambios de BPM

	int *nIChangeVel;	// Momentos iniciales en el cual ocurre un Cambio de Velocidades
	int *nEChangeVel;	// Momentos finales en el cual ocurre un Cambio de Velocidades
	double *dChanges;	// Cambios de Velocidades

	int *nIJumps;		// Saltos de canción (comienzo, y en Arrows)
	int *nEJumps;		// Saltos de canción (término, y en Arrows)

	int *nWStops;		// Momentos en el cual ocurre un stop
	double *dStops;		// Stops

	int *nWTickCount;	// Momentos en el cual ocurren cambios de TickCount
	int *nTickCount;	// TickCount
};

struct SONGCOMPILER
{
	int nSizeArrows1;
	int nSizeArrows2;
	int nSizeArrows3;

	DWORD *dwArrows1;	// Flechas 1
	DWORD *dwArrows2;	// Flechas 2
	DWORD *dwArrows3;	// Flechas 3

	double *dWArrows1;	// Ubicaciones (en Beats) donde se encuentran las flechas 1.
	double *dWArrows2;	// Ubicaciones (en Beats) donde se encuentran las flechas 2.
	double *dWArrows3;	// Ubicaciones (en Beats) donde se encuentran las flechas 3.

	double *dWWArrows1;	// Ubicaciones (en Beats, contando Stops) donde se encuentran las flechas 1.
	double *dWWArrows2;	// Ubicaciones (en Beats, contando Stops) donde se encuentran las flechas 2.
	double *dWWArrows3;	// Ubicaciones (en Beats, contando Stops) donde se encuentran las flechas 3.

	int nSizeChangeBPM;
	double *dWChangeBPM;// Momentos en el cual ocurre un Cambio de BPM (En Arrows)
	double *dChangeBPM;	// Cambios de BPM

	int nSizeChangeVel;
	double *dIChangeVel;// Momentos iniciales en el cual ocurre un Cambio de Velocidades
	double *dEChangeVel;// Momentos finales en el cual ocurre un Cambio de Velocidades
	double *dChangeVel;	// Cambios de Velocidades

	int nSizeJumps;
	double *dIJumps;	// Saltos de canción (comienzo, y en beats)
	double *dEJumps;	// Saltos de canción (término, y en beats)

	int nSizeStops;
	double *dWStops;	// Momentos en el cual ocurre un stop
	double *dStops;		// Stops

	int nSizeBSPopUp;
	double *dWBSPopUp;	// Momentos en el q sale un pop up de Brain Shower
	DWORD *dwBSPopUp;	// Argumentos del PopUp (IDMateria+numero)
};

struct BSCOMPILER
{
	int nSizeBSQuestions;
	double *dWIBSQuestions;	// Momentos Iniciales en los que salen las preguntas de Brain shower
	double *dWEBSQuestions;	// Momentos Finales en los que salen las preguntas de Brain shower
	double *dWABSQuestions; // Argumento opcional (Para memoria, observacion & Analisis)
	DWORD *dwBSQuestions;	// Argumentos de la pregunta (IDMateria+IDPregunta)
	DWORD *dwDBSQuestionsEx;// Lo que dice la pregunta de BS extendido
	DWORD *dwDBSQuestions;	// Lo que dice la pregunta de BS

	int nSizeBSAnswers;
	double *dWBSAnswers;	// Momentos en los que salen las respuestas
	double *dWWBSAnswers;	// Momentos en los que salen las respuestas
	DWORD *dwBSAnswers;		// Argumentos de las respuestas (IDSkin|IDItem)
	int *nBSAnswers;		// Argumentos de las respuestas (IDSkin|IDItem)
	int *nDBSAnswers;		// Modo de display (1 si deben aparecer números)

	int nSizeBSPopUp;
	double* dWBSPopUp;
	DWORD* dwBSPopUp;
};

struct TRAINERCOMPILER
{
	int nSizeArg;
	int* nArg;
};

struct CHARACTER_HEADER
{
	int nSizeVtxHead;	// Tamaño Vertices Cabeza
	int nSizeVtxChest;	// Tamaño Vertices Pecho
	int nSizeVtxWaist;	// Tamaño Vertices Cintura
	int nSizeVtxHip;	// Tamaño Vertices Cadera
	int nSizeVtxULArm;	// Tamaño Vertices Brazo Izquierdo
	int nSizeVtxURArm;	// Tamaño Vertices Brazo Derecho
	int nSizeVtxDLArm;	// Tamaño Vertices AnteBrazo Izquierdo
	int nSizeVtxDRArm;	// Tamaño Vertices AnteBrazo Derecho
	int nSizeVtxLHand;	// Tamaño Vertices Mano Izquierda
	int nSizeVtxRHand;	// Tamaño Vertices Mano Derecha
	int nSizeVtxULLeg;	// Tamaño Vertices Pierna Izquierdo
	int nSizeVtxURLeg;	// Tamaño Vertices Pierna Derecho
	int nSizeVtxDLLeg;	// Tamaño Vertices AntePierna Izquierdo
	int nSizeVtxDRLeg;	// Tamaño Vertices AntePierna Derecho
	int nSizeVtxLFoot;	// Tamaño Vertices Pie Izquierdo
	int nSizeVtxRFoot;	// Tamaño Vertices Pie Derecho

	int nSizeFaceHead;	// Tamaño Vertices Cabeza
	int nSizeFaceChest;	// Tamaño Vertices Pecho
	int nSizeFaceWaist;	// Tamaño Vertices Cintura
	int nSizeFaceHip;	// Tamaño Vertices Cadera
	int nSizeFaceULArm;	// Tamaño Vertices Brazo Izquierdo
	int nSizeFaceURArm;	// Tamaño Vertices Brazo Derecho
	int nSizeFaceDLArm;	// Tamaño Vertices AnteBrazo Izquierdo
	int nSizeFaceDRArm;	// Tamaño Vertices AnteBrazo Derecho
	int nSizeFaceLHand;	// Tamaño Vertices Mano Izquierda
	int nSizeFaceRHand;	// Tamaño Vertices Mano Derecha
	int nSizeFaceULLeg;	// Tamaño Vertices Pierna Izquierdo
	int nSizeFaceURLeg;	// Tamaño Vertices Pierna Derecho
	int nSizeFaceDLLeg;	// Tamaño Vertices AntePierna Izquierdo
	int nSizeFaceDRLeg;	// Tamaño Vertices AntePierna Derecho
	int nSizeFaceLFoot;	// Tamaño Vertices Pie Izquierdo
	int nSizeFaceRFoot;	// Tamaño Vertices Pie Derecho

	int nSizeUVTHead;	// Tamaño Vertices Cabeza
	int nSizeUVTChest;	// Tamaño Vertices Pecho
	int nSizeUVTWaist;	// Tamaño Vertices Cintura
	int nSizeUVTHip;	// Tamaño Vertices Cadera
	int nSizeUVTULArm;	// Tamaño Vertices Brazo Izquierdo
	int nSizeUVTURArm;	// Tamaño Vertices Brazo Derecho
	int nSizeUVTDLArm;	// Tamaño Vertices AnteBrazo Izquierdo
	int nSizeUVTDRArm;	// Tamaño Vertices AnteBrazo Derecho
	int nSizeUVTLHand;	// Tamaño Vertices Mano Izquierda
	int nSizeUVTRHand;	// Tamaño Vertices Mano Derecha
	int nSizeUVTULLeg;	// Tamaño Vertices Pierna Izquierdo
	int nSizeUVTURLeg;	// Tamaño Vertices Pierna Derecho
	int nSizeUVTDLLeg;	// Tamaño Vertices AntePierna Izquierdo
	int nSizeUVTDRLeg;	// Tamaño Vertices AntePierna Derecho
	int nSizeUVTLFoot;	// Tamaño Vertices Pie Izquierdo
	int nSizeUVTRFoot;	// Tamaño Vertices Pie Derecho

	int nSizeTHead;	// Tamaño Vertices Cabeza
	int nSizeTChest;	// Tamaño Vertices Pecho
	int nSizeTWaist;	// Tamaño Vertices Cintura
	int nSizeTHip;	// Tamaño Vertices Cadera
	int nSizeTULArm;	// Tamaño Vertices Brazo Izquierdo
	int nSizeTURArm;	// Tamaño Vertices Brazo Derecho
	int nSizeTDLArm;	// Tamaño Vertices AnteBrazo Izquierdo
	int nSizeTDRArm;	// Tamaño Vertices AnteBrazo Derecho
	int nSizeTLHand;	// Tamaño Vertices Mano Izquierda
	int nSizeTRHand;	// Tamaño Vertices Mano Derecha
	int nSizeTULLeg;	// Tamaño Vertices Pierna Izquierdo
	int nSizeTURLeg;	// Tamaño Vertices Pierna Derecho
	int nSizeTDLLeg;	// Tamaño Vertices AntePierna Izquierdo
	int nSizeTDRLeg;	// Tamaño Vertices AntePierna Derecho
	int nSizeTLFoot;	// Tamaño Vertices Pie Izquierdo
	int nSizeTRFoot;	// Tamaño Vertices Pie Derecho

	double dNormalHead[6];	// Cabeza
	double dNormalChest[6];	// Pecho
	double dNormalWaist[6];	// Cintura
	double dNormalHip[6];	// Cadera
	double dNormalULArm[6];	// Brazo Izquierdo
	double dNormalURArm[6];	// Brazo Derecho
	double dNormalDLArm[6];	// AnteBrazo Izquierdo
	double dNormalDRArm[6];	// AnteBrazo Derecho
	double dNormalULLeg[6];	// Pierna Izquierdo
	double dNormalURLeg[6];	// Pierna Derecho
	double dNormalDLLeg[6];	// AntePierna Izquierdo
	double dNormalDRLeg[6];	// AntePierna Derecho

	int nSizeArrows1;
	int nSizeArrows2;

	int nSizeArrowsAP;
};

struct CHARACTER
{
	CHARACTER_HEADER hCtr;
	// Todas las posiciones estan basadas en el 0,0,0
	double* dVtxHead;	// Cabeza
	double* dVtxChest;	// Pecho
	double* dVtxWaist;	// Cintura
	double* dVtxHip;	// Cadera
	double* dVtxULArm;	// Brazo Izquierdo
	double* dVtxURArm;	// Brazo Derecho
	double* dVtxDLArm;	// AnteBrazo Izquierdo
	double* dVtxDRArm;	// AnteBrazo Derecho
	double* dVtxLHand;	// Mano Izquierda
	double* dVtxRHand;	// Mano Derecha
	double* dVtxULLeg;	// Pierna Izquierdo
	double* dVtxURLeg;	// Pierna Derecho
	double* dVtxDLLeg;	// AntePierna Izquierdo
	double* dVtxDRLeg;	// AntePierna Derecho
	double* dVtxLFoot;	// Pie Izquierdo
	double* dVtxRFoot;	// Pie Derecho

	int* nFaceHead;		// Cabeza
	int* nFaceChest;	// Pecho
	int* nFaceWaist;	// Cintura
	int* nFaceHip;		// Cadera
	int* nFaceULArm;	// Brazo Izquierdo
	int* nFaceURArm;	// Brazo Derecho
	int* nFaceDLArm;	// AnteBrazo Izquierdo
	int* nFaceDRArm;	// AnteBrazo Derecho
	int* nFaceLHand;	// Mano Izquierda
	int* nFaceRHand;	// Mano Derecha
	int* nFaceULLeg;	// Pierna Izquierdo
	int* nFaceURLeg;	// Pierna Derecho
	int* nFaceDLLeg;	// AntePierna Izquierdo
	int* nFaceDRLeg;	// AntePierna Derecho
	int* nFaceLFoot;	// Pie Izquierdo
	int* nFaceRFoot;	// Pie Derecho

	double* dUVTHead;	// Cabeza
	double* dUVTChest;	// Pecho
	double* dUVTWaist;	// Cintura
	double* dUVTHip;	// Cadera
	double* dUVTULArm;	// Brazo Izquierdo
	double* dUVTURArm;	// Brazo Derecho
	double* dUVTDLArm;	// AnteBrazo Izquierdo
	double* dUVTDRArm;	// AnteBrazo Derecho
	double* dUVTLHand;	// Mano Izquierda
	double* dUVTRHand;	// Mano Derecha
	double* dUVTULLeg;	// Pierna Izquierdo
	double* dUVTURLeg;	// Pierna Derecho
	double* dUVTDLLeg;	// AntePierna Izquierdo
	double* dUVTDRLeg;	// AntePierna Derecho
	double* dUVTLFoot;	// Pie Izquierdo
	double* dUVTRFoot;	// Pie Derecho

	int* nTHead;	// Cabeza
	int* nTChest;	// Pecho
	int* nTWaist;	// Cintura
	int* nTHip;	// Cadera
	int* nTULArm;	// Brazo Izquierdo
	int* nTURArm;	// Brazo Derecho
	int* nTDLArm;	// AnteBrazo Izquierdo
	int* nTDRArm;	// AnteBrazo Derecho
	int* nTLHand;	// Mano Izquierda
	int* nTRHand;	// Mano Derecha
	int* nTULLeg;	// Pierna Izquierdo
	int* nTURLeg;	// Pierna Derecho
	int* nTDLLeg;	// AntePierna Izquierdo
	int* nTDRLeg;	// AntePierna Derecho
	int* nTLFoot;	// Pie Izquierdo
	int* nTRFoot;	// Pie Derecho

	int* nArrows1;
	int* nArrows2;
	double* dWWArrows1;
	double* dWWArrows2;

	DWORD* dwArrowsAP;
	double* dWWArrowsAP;

	// Partes del desarrollo
	// Texturas
	GLuint nTextures[16];
};

struct MACHINE_HEADER
{
	int nSizeVtxMachine[2];
	int nSizeFaceMachine[2];
	int nSizeUVTMachine[2];
	int nSizeTMachine[2];
	int nSizeVtxTap[10];
	int nSizeFaceTap[10];
	int nSizeUVTTap[10];
	int nSizeTTap[10];

	double dPosTap[30];
	double dPosNoTapL[3];
	double dPosNoTapR[3];
};

struct MACHINE
{
	MACHINE_HEADER hMachine;
	double* dVtxMachine[2];
	int* nFaceMachine[2];
	double* dUVTMachine[2];
	int* nTMachine[2];
	double* dVtxTap[10];
	int* nFaceTap[10];
	double* dUVTTap[10];
	int* nTTap[10];
	GLuint nTexTap[10];
	GLuint nTexMach[2];
};

struct EFFECT_COMP
{
	int nSizeVanishChanges[3];
	double* dWVanishChanges[3];
	DWORD* dwVanishChanges[3];

	int nSizeAttackChanges[3];
	double* dWAttackChanges[3];
	DWORD* dwAttackChanges[3];

	int nSizeZigZagChanges[3];
	double* dWZigZagChanges[3];
	DWORD* dwZigZagChanges[3];
};

struct ARGSTEP
{
	CString strMusic;
	CString strSplash;
	CString strBGA;
	CString strFile;
	CString strLights;
	bool bLoadBS;
	bool bAutoPlay;
	int nLevelCharacer;
	int nTypeCharacter;
	int nLevelMach;

	bool bIsSingle;
	bool bTrainingMode;
	int nSkin;
	double dVel;
	bool bXMode;
	bool bNXMode;
	bool bHJ;
	bool bRS;
	bool bMirror;
	bool bFreedom;
	bool bRV;
	bool bRandomSkin;
	int nVanish;
	bool bDC;
	bool bAC;
	bool bEW;
	bool bFlash;
	bool bUnderAttack;
	bool bLeftAttack;
	bool bRightAttack;
	bool bDrop;
	bool bSnake;
	int nCharacter;
	bool bCharacter;
	double dUserVel;
	double dIncrBar;
	double dDecrBar;
	double dPowTimer;
};

struct PLAYER
{
	TCHAR chName[12];
	BYTE nLockedSongNew[10000];
	BYTE nLockedSpecNew[5000];
	BYTE nLockedBrainShower[1000];
	BYTE nLockedChar[20];
	BYTE nLockedAsp[100];
	BYTE nMission[1000];
	BYTE nUnlockedMission[1000];
	BYTE nUnlockedWay[1000];
	BYTE nInfoAdventure[1000];

	int nLastArcade[2];
	int nArcadeRecords[10000][20];

	int nLastSpecial[2];
	int nSpecialRecords[5000][20];

	int nLastBrainShower[2];
	int nBrainShowerRecords[1000][20];
	int nBrainShowerRecordsEx[1000][20];

	int nLastTraining;
	int nAprovedTraining[1000];

	int nPuntGames[20];
	int nSongGames[20][4];

	int nLastMission;
	BYTE nBgaSongNew[1000];
	BYTE nBgaSpecNew[1000];

	BYTE nLevel;
	int nExp;
	int nMileage;
};

struct MACHINE_PROFILE
{
	int nSizeSongLockNew;
	int nSizeSongLockRetro;
	int nSizeSpecLockNew;
	int nSizeSpecLockRetro;
	int nSizeBrainShowerLock;
	int nSizeMissionUnlocked;
	int nSizeCharLock;
	int nSizeAspLock;

	BYTE nLockedSongNew[10000];
	BYTE nLockedSpecNew[5000];
	BYTE nLockedBrainShower[1000];
	BYTE nLockedChar[20];
	BYTE nLockedAsp[100];
	BYTE nMission[1000];
	BYTE nUnlockedMission[1000];
	BYTE nUnlockedWay[1000];
	BYTE nInfoAdventure[1000];

	TCHAR chArcadeRecords[10000][12];
	int nArcadeRecords[10000];

	TCHAR chSpecialRecords[5000][12];
	int nSpecialRecords[5000];

	TCHAR chBrainShowerRecords[1000][12];
	int nBrainShowerRecords[1000];
	int nBrainShowerRecordsEx[1000];

	int nBufRecords[20];
	TCHAR chRecords[20][12];
	int nRecords[20];

	BYTE nLevel;
	int nExp;
	int nMileage;
};*/

#endif // CMN_PIU
