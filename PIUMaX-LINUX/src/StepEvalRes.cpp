#include "stdafx.h"
#include "StepEvalRes.h"

CStepEvalRes::CStepEvalRes(void)
{
	for(int x = 0; x < SIZE_STEPEVAL; x++)
		skin[x] = new COGLSprite();
}

CStepEvalRes::~CStepEvalRes(void)
{
	for(int x = 0; x < SIZE_STEPEVAL; x++)
		delete skin[x];
}

int CStepEvalRes::LoadSkin(void)
{
	int nDur[SIZE_STEPEVAL], nFrames[SIZE_STEPEVAL];
	FILE* hFile;
	TCHAR str[260];
	TCHAR chArchives[][260] =
	{
		_T("UpAndDownPlayer"),
		_T("UpAndDownAdv"),
		_T("Orbe"),
		_T("OrbeGlouth"),
		_T("OrbeRainbow"),
		_T("BarEval"),
		_T("NumberEval"),
		_T("RankS"),
		_T("RankA"),
		_T("RankB"),
		_T("RankC"),
		_T("RankD"),
		_T("RankF"),
		_T("RankGlouth1"),
		_T("RankGlouth2"),
		_T("PerfectEval"),
		_T("GreatEval"),
		_T("GoodEval"),
		_T("BadEval"),
		_T("MissEval"),
		_T("MaxComboEval"),
		_T("TotalScoreEval"),
		_T("HeartEval"),
		_T("PotionEval"),
		_T("MineEval"),
		_T("VelocityEval"),
		_T("TotalItemEval"),
		_T("MissComboEval"),
		_T("LifeGaugeEval"),
		_T("OXEval"),
		_T("ComboOXEval"),
		_T("SucessEval"),
		_T("FailureEval"),
		_T("HS/UpAndDown"),
		_T("HS/LetterBorder"),
		_T("HS/ScoreBorder"),
		_T("HS/EndLetter"),
		_T("HS/BackSpace")
	};
	TCHAR chDir[260] = _T("StepEval/");
	_sprintf(str, STROP _T("config.acc\0"), chDir);
	hFile = _fopen(str, _T("r"));
	if(hFile == NULL)
	{
		return 0;
	}
	int nLang[SIZE_STEPEVAL];
	TCHAR ch[100];
	for(int x = 0; x < SIZE_STEPEVAL; x++)
	{
		if(_fgets(ch, 100, hFile) == NULL) {fclose(hFile); return 0;}
		if(_sscanf(ch, _T("%d\t%d\t%d"), &nFrames[x], &nDur[x], &nLang[x]) != 3) {fclose(hFile); return 0;}
	}
	fclose(hFile);
	for(int x = 0; x < SIZE_STEPEVAL; x++)
	{
		if(nLang[x]) _sprintf(str, _T("%ls%ls_%ls.png"), chDir, chArchives[x], chLang);
		else _sprintf(str, _T("%ls%ls.png"), chDir, chArchives[x]);
		skin[x]->InitImage(str, nFrames[x]);
		skin[x]->SetDuration(nDur[x]);
	}
	return 1;
}

void CStepEvalRes::Update(DWORD dwMiliseconds)
{
	for(int x = 0; x < SIZE_STEPEVAL; x++)
		skin[x]->Update(dwMiliseconds);
}

void CStepEvalRes::Delete()
{
	for(int x = 0; x < SIZE_STEPEVAL; x++)
		skin[x]->DeleteImage();
}
