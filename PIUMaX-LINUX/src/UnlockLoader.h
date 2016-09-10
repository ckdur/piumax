#pragma once

class CUnlockLoader
{
public:
	CUnlockLoader(void);
	~CUnlockLoader(void);
	int m_nSize;			// Size Unlock
	double* m_dReqGame1;
	double* m_dReqGame2;
	double* m_dReqGame3;
	double* m_dReqGame4;
	double* m_dReqGame5;
	int* m_nReqExp;
	int* m_nReqLvl;
	int* m_nSizeMission;
	unsigned int** m_uIDReqMission;
	int* m_nSizeCondUnlock;	// Size Unlocks
	unsigned int** m_uCondUnlock;	// ID Unlocks
	DWORD** m_dwCondUnlockModes;	// ID Unlocks Modes [Binary]
	int* m_nSizeCondAspUnlock;	// Size Aspect Unlocks
	unsigned int** m_uCondAspUnlock;	// ID Aspect Unlocks
	int LoadBinary(LPCTSTR chFile);
	int SaveBinary(LPCTSTR chName);
	int LoadText(LPCTSTR  chName);
	void Destroy(void);
	int SearchID(unsigned int nId);
};