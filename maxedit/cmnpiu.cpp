#include "stdafx.h"
#include "cmnpiu.h"
#include "libc_max_common.h"

CGraphMain* g_gmMain = NULL;
CSoundSys* g_ssMain = NULL;
CGraphTexture* g_gtNoTex;

DWORD dwTick; 
DWORD dwLastTick;

PIUCONFIG piuconfig;

int nTotalCredit = 0;
CSoundChannel* channels[MAX_CHANNELS];

TCHAR* tc_MIND_TYPE[] = 
{_T("IMAGE"), _T("VIDEO"), _T("TEXT")};

TCHAR chMethod[32];
TCHAR chFamily[32];

void SetCurDir()
{
	TCHAR chCore[MAX_PATH];
	GetMaxProcessDirectory(chCore);
	SetMaxCurrentDirectory(chCore);
	_printf(_T("Set dir to process: %s"), chCore);
}

bool operator == (MID id1, MID id2)
{return id1.nId == id2.nId && id1.nSubId == id2.nSubId;}
bool operator != (MID id1, MID id2)
{return id1.nId != id2.nId || id1.nSubId != id2.nSubId;}