#ifndef MAXARGSTEP
#define MAXARGSTEP

struct ARGSTEP
{
	bool bAutoPlay;
	bool bAssistTick;

	bool bCharacter;
	int nLevelCharacer;
	int nTypeCharacter;
	int nCharacter;

	int nLevelMach;

	bool bIsSingle;
	bool bTrainingMode;
	int nSkin;
	bool bXMode;
	bool bNXMode;
	bool bEJ;
	bool bNJ;
	bool bHJ;
	bool bRS;
	bool bMirror;
	bool bFreedom;
	bool bRV;
	bool bRandomSkin;
	float fEVanish;
	float fIVanish;
	bool bDC;
	bool bAC;
	bool bEW;
	bool bFlash;
	bool bUnderAttack;
	bool bLeftAttack;
	bool bRightAttack;
	bool bDrop;
	bool bSnake;
	bool bSink;
	bool bRise;
	bool bJR;
	float fUserVel;
	float fIncrBar;
	float fDecrBar;
	float fRush;
};

#endif // MAXARGSTEP