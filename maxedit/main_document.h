#ifndef MAXEDIT_DOCUMENT
#define MAXEDIT_DOCUMENT


#include "stdafx.h"
#include "defbas.h"
#include "SKSFv1.h"
#include "StepMania/MsdFile.h"	// For SM conversion
#include "NX2SRC/step.h"

#define DEFAULT_DOCUMENT_NAME _T("Nuevo Documento")
#define ALL_DOCUMENT_FILTER_NAME "MaxEdit Supported Files"
#define ALL_DOCUMENT_FILTER_PATTERN ".*\.(sksf2|sksf|sm|sma|ksf|ucs|nx|stx|not)"

#define MAIN_DOCUMENT_FILTER_NAME "MaxEdit PIUMaX2 Step \"Super KSF 2\" (.sksf2)"
#define MAIN_DOCUMENT_FILTER_PATTERN "*.sksf2"
#define MAIN_DOCUMENT_FILTER_EXTENSION ".sksf2"

#define SKSF_DOCUMENT_FILTER_NAME "StepEdit PIUMaX Step \"Super KSF\" (.sksf)"
#define SKSF_DOCUMENT_FILTER_PATTERN "*.sksf"
#define SKSF_DOCUMENT_FILTER_EXTENSION ".sksf"

#define SM_DOCUMENT_FILTER_NAME "StepMania 3.9/3.9+ file (.sm)"
#define SM_DOCUMENT_FILTER_PATTERN "*.sm"
#define SM_DOCUMENT_FILTER_EXTENSION ".sm"

#define SMA_DOCUMENT_FILTER_NAME "StepMania AldoMX file (.sma)"
#define SMA_DOCUMENT_FILTER_PATTERN "*.sma"
#define SMA_DOCUMENT_FILTER_EXTENSION ".sma"

#define KSF_DOCUMENT_FILTER_NAME "KickItUp Step File (.ksf)"
#define KSF_DOCUMENT_FILTER_PATTERN "*.ksf"
#define KSF_DOCUMENT_FILTER_EXTENSION ".ksf"

#define UCS_DOCUMENT_FILTER_NAME "Andamiro User Custom Step File (.ucs)"
#define UCS_DOCUMENT_FILTER_PATTERN "*.ucs"
#define UCS_DOCUMENT_FILTER_EXTENSION ".ucs"

#define NX10_DOCUMENT_FILTER_NAME "Andamiro NX/NX2/NXA Original Steps (.NX)"
#define NX10_DOCUMENT_FILTER_PATTERN "*.nx"
#define NX10_DOCUMENT_FILTER_EXTENSION ".nx"

#define STX_DOCUMENT_FILTER_NAME "Andamiro PrexX Original Steps (.stx)"
#define STX_DOCUMENT_FILTER_PATTERN "*.stx"
#define STX_DOCUMENT_FILTER_EXTENSION ".stx"

#define NOT_DOCUMENT_FILTER_NAME "Andamiro Extra Original Steps (.not)"
#define NOT_DOCUMENT_FILTER_PATTERN "*.not"
#define NOT_DOCUMENT_FILTER_EXTENSION ".not"


#define MAX_UNDOREDO_STACK 20

#define TOP_ARROWS (10.0)
#define BOTTOM_ARROWS (-60.0)

#define INSIDE_OF_OUTSIDE(nInt, nMinY, nMaxY, nIW, nEW) ((nMinY <= nInt[nIW] && nInt[nIW] < nMaxY) || \
					(nMinY < nInt[nEW] && nInt[nEW] <= nMaxY) || \
					(nInt[nIW] <= nMinY && nMinY < nInt[nEW]) || \
					(nInt[nIW] < nMaxY && nMaxY <= nInt[nEW]) || \
					(nIW == nEW && (nMinY <= nInt[nIW] && nInt[nIW] <= nMaxY)))

enum MAX_SM_STATUS
{
	MAX_SM_NONE,
	MAX_SM_FILED,
	MAX_SMA_FILED,
	MAX_SM_LOADED,
	MAX_SM_SIZE
};

// DEFINITION OF "SONG CONTEXT"
// Based in SKSFv.1
// Must be compatible with any format
struct STEPHEADER
{
	// PARAMETROS
	uint32_t dwVersion;		// Version de los steps (0x20)
	uint32_t dwLSong;			// Largo de la canción (en ms)
	int nOffset;			// Offset con la canción (en ms)

	int nDificulty;			// Dificultad (De 1 a lo que dé el int)

	float fBPM;				// BPM por defecto (usualmente 120)
	float fVel;				// Velocidad por defecto (usualmente 1.0)
	int nBeatSplit;			// BeatSplit por defecto
	int nTickCount;			// TickCount por defecto

	int nMode;				// Mode of arrows (0 for single, 1 for doble, etc)

	// Atributos de datos
	int nSizeAttrib;		// Tamaño de los atributos de la canción
	int nSizeArrowGroups;	// Tamaño de los grupos de flechas

	// Atributos de los modificadores

	// Comunes
	int nSizeBeatSplit;		// Tamaño de los Beat Split
	int nSizeChangeBPM;		// Tamaño de los Cambios de BPM
	int nSizeChangeVel;		// Tamaño de los Cambios de Velocidades
	int nSizeJumps;			// Tamaño de saltos de canción
	int nSizeTickCount;		// Tamaño de los tickcounts
	int nSizeStops;			// Tamaño de los stops

	// Extendidos
	int nSizeMod;			// Tamaño de los modificadores opcionales

	// Flechas
	int nArrowsPerLine;		// Tamaño de los steps por linea
}
#ifdef __GNUC__
__attribute__((ms_struct))
#endif
;

struct STEPGROUPHEADER
{
	// PARAMETROS
	int nNoMiss;			// No se pierde en miss
	int nNoCount;			// No se toma en cuenta
	int nIsVanishSup;		// Hay Vanish superior
	int nIsVanishInf;		// Hay Vanish inferior
	int nIsSkin;			// Hay Skin obligatorio
	float fVanishSup;		// Vanish superior
	float fVanishInf;		// Vanish inferior
	int nSkin;				// Skin obligatorio

	int nAsociate;			// Asociar con el grupo n (para no volver a escribir cambios de BPM y etc), -1 si no

	// Atributos de datos
	int nSizeArrows;		// Tamaño de las flechas
}
#ifdef __GNUC__
__attribute__((ms_struct))
#endif
;

struct STEPGROUP
{
	STEPGROUPHEADER hGroup;

	int *nArrows;		// Flechas
	int *nPArrows;		// Parametros de flechas
};

struct STEPMODHEADER
{
	int nSize;		// Tamaño de los datos
	int nSizeInt;	// Tamaño de grupos de int (depente de nType y su script)
	int nSizeFloat;	// Tamaño de grupos de float (depente de nType y su script)
	int nType;		// Tipo de modificador (interno)
}
#ifdef __GNUC__
__attribute__((ms_struct))
#endif
;

struct STEPMODINFO
{
	int *nModInt;		// Parametros modificador (entero)
	float *fModFloat;		// Parametros modificador (coma flotante)
};

struct STEPMOD
{
	STEPMODHEADER hMod;
	STEPMODINFO* hModInfo;
};

struct STEPMODAUX
{
	float fIMod;	// Inicio modificador (BeatEdit)
	float fEMod;	// Inicio modificador (BeatEdit)
};

struct STEPAUX
{
	float *fWBeatSplit;	// Momentos en el cual ocurre un BeatSplit (en BeatEdit)

	float *fWChangeBPM;	// Momentos en el cual ocurre un Cambio de BPM (En BeatEdit)

	float *fIChangeVel;	// Momentos iniciales en el cual ocurre un Cambio de Velocidades
	float *fEChangeVel;	// Momentos finales en el cual ocurre un Cambio de Velocidades

	float *fIJumps;		// Saltos de canción (comienzo, y en BeatEdit)
	float *fEJumps;		// Saltos de canción (término, y en BeatEdit)

	float *fWTickCount;	// Momentos en el cual ocurren cambios de TickCount

	float *fIStops;		// Stops de canción (comienzo, y en BeatEdit)
	float *fEStops;		// Stops de canción (término, y en BeatEdit)

	STEPMODAUX *hMod;
};

struct STEPGROUPAUX
{
	float fLimitArrows;	// Ubicación de la ultima flecha (en BeatEdit)
};

struct STEP
{
	STEPHEADER hStep;

	float* fAttrib;		// Atributos de la canción (como Voltaje, Resistencia...)

	STEPGROUP* hStepGroup; // Grupos
	STEPMOD* hMod;		// Modificadores

	int *nWBeatSplit;	// Momentos en el cual ocurre un BeatSplit (en Arrows)
	int *nBeatSplit;	// Beat Splits (En Arrows)

	int *nWChangeBPM;	// Momentos en el cual ocurre un Cambio de BPM (En Arrows)
	float *fChangeBPM;	// Cambios de BPM

	int *nIChangeVel;	// Momentos iniciales en el cual ocurre un Cambio de Velocidades
	int *nEChangeVel;	// Momentos finales en el cual ocurre un Cambio de Velocidades
	float *fChangeVel;	// Cambios de Velocidades
	float *fIChangeVel;	// Inicio de la velocidad

	int *nIJumps;		// Saltos de canción (comienzo, y en Arrows)
	int *nEJumps;		// Saltos de canción (término, y en Arrows)

	int *nWTickCount;	// Momentos en el cual ocurren cambios de TickCount
	int *nTickCount;	// TickCount

	int *nIStops;		// Stops de canción (comienzo, y en Arrows)
	int *nEStops;		// Stops de canción (término, y en Arrows)
};

enum MAXSTEP_MOD
{
	MAXSTEP_BEATSPLIT,
	MAXSTEP_CHANGEBPM,
	MAXSTEP_CHANGEVEL,
	MAXSTEP_JUMP,
	MAXSTEP_TICKCOUNT,
	MAXSTEP_STOP,
	MAXSTEP_SIZE
};

struct MAXSTEP_CLIPBOARD_HEADER
{
	int nW;	// Width Data (columns)
	int nH;	// Height Data (rows)
}
#ifdef __GNUC__
__attribute__((ms_struct))
#endif
;

struct MAXSTEP_CLIPBOARD
{
	MAXSTEP_CLIPBOARD_HEADER hStep;
	int *nArrows;		// Flechas
	int *nPArrows;		// Parametros de flechas
};

struct MAXMOD_CLIPBOARD_HEADER
{
	int nSize;		// Tamaño de los datos
	int nSizeInt;	// Tamaño de grupos de int (depente de nType y su script)
	int nSizeFloat;	// Tamaño de grupos de float (depente de nType y su script)
	int nType;		// Tipo de modificador (interno)
	int nRef;		// Reference position
}
#ifdef __GNUC__
__attribute__((ms_struct))
#endif
;

struct MAXMOD_CLIPBOARD
{
	MAXMOD_CLIPBOARD_HEADER hMod;
	STEPMODINFO *hModInfo;
};

struct MAX_STACKELEM_HEADER
{
	size_t nFile;	// Size (in char) about namefile
	bool bIsFile;

	// This data is markable if nIsFile is false

	// Data location (past, in max-min form)
	int nMinX;
	int nMaxX;
	int nMinY;
	int nMaxY;

	int nFamily;
	int nGroup;	// NOT REFERENCE
}
#ifdef __GNUC__
__attribute__((ms_struct))
#endif
;

struct MAX_STACKELEM
{
	MAX_STACKELEM_HEADER hElem;
	char* chFile;	// Whole namefile backup

	// This data is markable if nIsFile is false

	// Only if nFamily is 1
	int *nArrows;		// Flechas
	int *nPArrows;		// Parametros de flechas

	// Only if nFamily is 2
	STEPMODHEADER *hMod;	// UNIQUE
	STEPMODINFO *hModInfo;
};

class CMaxDocument
{
public:
    CMaxDocument();
    CMaxDocument(int ord);
    ~CMaxDocument();

    CString m_strFile;
    CString m_strName;
    bool m_bSaved;
    bool m_bFiled;

	void Init(void);
	void Reinit(void);
    int Open(LPCTSTR strFile);
    int Save(void);
    int Save(LPCTSTR strFile);
    void Close(void);
	int New(void);
	int New(int ord);
	int NewInit(void);
	void InitArrowGruops(int nSizeBefore, int nSize);
	void InitAux(void);
	void ReinitAux(void);

	// SaveRAW Versions
	int SaveRAW_SKSF2(LPCTSTR strFile, STEP* fsong); // ** Save SKSFv2 edit
	int SaveRAW_SKSF1(LPCTSTR strFile, SKSFv1* fsong); // ** Save SKSFv1 edit
	int Save_UCS_SKSF2(LPCTSTR strFile, STEP* fsong); // ** Save UCS using SKSFv2

	// OpenRAW Versions
	int OpenRAW_SKSF2(LPCTSTR strFile, STEP* fsong); // ** Open SKSFv2 edit
	int OpenRAW_SKSF1(LPCTSTR strFile, SKSFv1* fsong); // ** Open SKSFv1 edit
	int OpenRAW_SM(LPCTSTR strFile, MsdFile* fsong); // ** Open SM/SMA/KSF using MsdFile
	int OpenRAW_NX10(LPCTSTR strFile, CNX10Step* fsong); // ** Open NX10 using Step.h/Step.cpp NX2SRC
	int Open_UCS_SKSF2(LPCTSTR strFile, STEP* fsong); // ** Open UCS using SKSFv2

	// RAW Conversions
	int Convert_SKSFv1_SKSFv2(STEP* fdest, SKSFv1* forigin); // v1 2 v2
	int Convert_SM_SKSFv2(STEP* fdest, MsdFile* forigin, CString strMode, bool bPlus); // SM 2 v2
	int Convert_SMA_SKSFv2(STEP* fdest, MsdFile* forigin, CString strMode); // SMA 2 v2
	int Convert_KSF_SKSFv2(STEP* fdest, MsdFile* forigin); // ksf 2 v2
	int Convert_NX10_SKSFv2(STEP* fdest, CNX10Step* forigin); // NX10 2 v2
	int m_nStatusSM;
	int Convert_SKSFv2_SKSFv1(SKSFv1* fdest, STEP* forigin); // v2 2 v1

	// RAW Initializations
	void Init_SKSF2(STEP* fsong);
	void Init_SKSF1(SKSFv1* fsong);

	// RAW Re-Initializations
	void ReInit_SKSF2(STEP* fsong);
	void ReInit_SKSF1(SKSFv1* fsong);

	// RAW Destroy
	void Delete_SKSF2(STEP* fsong);
	void Delete_SKSF1(SKSFv1* fsong);

	// Vars
	STEP song;
	STEPGROUPAUX* hGroupAux;
	STEPAUX hAux;
	STEPMODAUX* hModAux;
	int m_nArrowsPerLine;
	MsdFile m_sm;
	CString m_strSplash;
	CString m_strMusic;
	CString m_strBGA;

	// Convierte flechas en beats para el modo edición (Lo mas simple)
	float BeatArrowEdit(int nArrow);
	float BeatArrowDraw(int nArrow);
	float BeatArrowTime(int nArrow);
	float BeatArrowDraw(float dBeatEdit);
	float BeatArrowTime(float dBeatEdit);

	float BeatArrowEdit(STEP* fsong, int nArrow);
	float BeatArrowDraw(STEP* fsong, int nArrow);
	float BeatArrowTime(STEP* fsong, int nArrow);
	float BeatArrowDraw(STEP* fsong, float dBeatEdit);
	float BeatArrowTime(STEP* fsong, float dBeatEdit);

	int UbicateAndModificateAccordBeatSM(STEP* fdest, float fBeatGo, float fBPMa);
	int UbicateAndModificateAccordBeatSM(STEP* fdest, float fBeatGo, float fBPMa, int* nAdded);
	int UbicateAndModificateAccordStopSM(STEP* fdest, float fBeatGo, float fBeatDur, float fBPMa, int* nLast);

	int ArrowBeatEdit(float fBeat);
	int Actualizate(void);
	int GetArrow(int x, int y, int nGArrow);
	void SetArrow(int x, int y, int nGArrow, int nType, int nPar);
	int GetArrowPar(int x, int y, int nGArrow);
	void ResizeSteps(int nGroup, int nSize);
	bool SomeNote(int y, int n);
	bool SomeBSQuestion(int y, int nGArrow);
	bool SomeBSAnswer(int y, int nGArrow);
	bool SomeBSPopUp(int y, int nGArrow);
	// Utils about Mods
	void AddMod(int nMod, int* nInt, int nSizeInt, float* fFloat, int nSizeFloat);
	void EditMod(int nMod, int y, int* nInt, int nSizeInt, float* fFloat, int nSizeFloat);
	void DelMod(int nMod, int y);
	void GetMod(int nMod, int y, int** nInt, float** fFloat);
	void FreeMod(int** nInt, float** fFloat);
	void GetParMod(int nMod, int* nSize, int* nSizeInt, int* nSizeFloat);
	int VerificateMod(int nMod, int y, int by);
	int VerificateMod(int nMod, int y, int by, int i);
	// Mod-dependant
	void GetModWhere(int nMod, int* nIWhere, int* nEWhere);
	// Utils about groups
	void AddGroup(void);
	void DelGroup(int nGroup);
	void Fix(void);
	void Fix(STEP *fsong);
	// Utils about UNDO/REDO
	void RegArrows(int m_nX, int m_nY, int m_nBackX, int m_nBackY, int m_nFamily, int m_nGroup, int nReg = -1);
	void RegMods(int m_nX, int m_nY, int m_nBackX, int m_nBackY, int m_nFamily, int m_nGroup, int nReg = -1);
	void RegSong(STEP* fsong);
	void RegAction(MAX_STACKELEM* elem);
	void RegAction(MAX_STACKELEM* elem, int n);
	void FreeAction(MAX_STACKELEM* elem);
	void AppAction(int n, int bBackUp);
	void DelAction(int n);
	void Undo(void);
	void Redo(void);
	bool CanUndo(void);
	bool CanRedo(void);
	void DelStackUndo(void);

	// UNDO/REDO
	vector<MAX_STACKELEM*> m_sevStack;
	int m_nCurStack;

};

#endif // MAXEDIT_DOCUMENT
