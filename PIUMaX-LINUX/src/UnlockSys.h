#ifndef PIU_UNLOCK
#define PIU_UNLOCK

#include "stdafx.h"

int CreateContextUnlock();
void DestroyContextUnlock();
void ActualizateContextUnlock();
int FrameUnlock();
int EventUnlock(int nEvent, int nKey);
void RegisterUnlock(int nRef, int nID, BYTE* nPar);
void CheckForUnlock();

extern int nModeUnlock;
extern int nSizeUnlock;

#endif // PIU_UNLOCK