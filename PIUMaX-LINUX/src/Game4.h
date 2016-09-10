#ifndef PIU_GAME4
#define PIU_GAME4

#include "stdafx.h"

int CreateContextGame4();
void DestroyContextGame4();
void ActualizateContextGame4();
void ClearContextGame4();
int FrameGame4();
int EventGame4(int nEvent, int nKey);
CString GetNameTotalSongsGame4(int nIndex);
CString GetArtistTotalSongsGame4(int nIndex);
CString GetBPMTotalSongsGame4(int nIndex);
CString GetIDTotalSongsGame4(int nIndex);

#endif // PIU_GAME4