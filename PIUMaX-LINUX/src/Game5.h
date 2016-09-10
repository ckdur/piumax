#ifndef PIU_GAME5
#define PIU_GAME5

#include "stdafx.h"

int CreateContextGame5();
void DestroyContextGame5();
void ActualizateContextGame5();
void ClearContextGame5();
int FrameGame5();
int EventGame5(int nEvent, int nKey);
CString GetNameTotalSongsGame5(int nIndex);
CString GetArtistTotalSongsGame5(int nIndex);
CString GetBPMTotalSongsGame5(int nIndex);
CString GetIDTotalSongsGame5(int nIndex);

#endif // PIU_GAME5