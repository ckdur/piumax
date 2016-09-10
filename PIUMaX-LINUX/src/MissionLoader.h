#pragma once

class CMissionLoader
{
public:
	CMissionLoader(void);
	~CMissionLoader(void);
	int m_nSize;			// Size Missions
	unsigned int* m_uID;	// ID Mission
	unsigned int* m_uStage;	// Stages Gastados
	unsigned int* m_uPath;	// Caminos
	CString* m_strName;		// Mission name
	CString* m_strLand;		// Land name
	int* m_nSizeSong;
	CString** m_strSong;		// ID Song
	int* m_nSizeStep;
	CString** m_strStep;		// ID Step
	CString* m_strHis;		// History file
	CString* m_strShowCond;	// Condition in show
	CString* m_strComment;	// Comment
	CString* m_strGo;		// Sprite file if go
	unsigned int* m_uLvl;	// Nivel
	unsigned int* m_uPlayer;	// Nivel
	unsigned int* m_uMileage;	// Mileage de la mission
	BYTE* m_bBreak;			// Is StageBreak?
	BYTE* m_bLink;			// Is Linking to another location?
	BYTE* m_bActUMod;		// Can User change mod?
	unsigned int* m_uIcon;	// Icon (optional, 0 doesn't)
	unsigned int* m_uIconSucess;	// Icon if mission sucess (optional, 0 doesn't)
	unsigned int* m_uMap;	// Map (Zero Based)
	double* m_dUbiX;		// Location X
	double* m_dUbiY;		// Location Y
	double* m_dInitialBar;	// Barra inicial
	double* m_dIncrBar;		// Incrementacion de barra
	double* m_dDecrBar;		// Decrementacion de barra
	unsigned int* m_uGo;	// ID Go (if link [m_bLink])
	BYTE* m_bDouble;		// Es doble?
	unsigned int* m_uVersus;	// ID retador
	unsigned int* m_uLvlVersus;
	int* m_nSizeMod;		// Size Modifiers
	int** m_nMod;			// Modifiers

	int* m_nSizeCond;		// Size conditions
	CString** m_strCond;	// Condition
	CString** m_strHPass;	// History file if pass
	CString** m_strHNoPass;	// History file if fail
	BYTE** m_bShowSucess;	// Show "Mission Sucess" if pass?
	int** m_nSizeCondOpen;	// Size open mission
	unsigned int*** m_uCondOpen;	// Open mission
	int** m_nSizeCondUnlock;	// Size Unlocks
	unsigned int*** m_uCondUnlock;	// ID Unlocks
	DWORD*** m_dwCondUnlockModes;	// ID Unlocks Modes [Binary]
	int** m_nSizeCondAspUnlock;	// Size Aspect Unlocks
	unsigned int*** m_uCondAspUnlock;	// ID Aspect Unlocks
	int LoadBinary(LPCTSTR chFile);
	int SaveBinary(LPCTSTR chName);
	int LoadText(LPCTSTR  chName);
	void Destroy(void);
	int SearchID(unsigned int nId);
};