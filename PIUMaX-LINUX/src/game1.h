#ifndef PIU_GAME1
#define PIU_GAME1

#include "stdafx.h"

int CreateContextGame1();
void DestroyContextGame1();
void ActualizateContextGame1();
void ClearContextGame1();
int FrameGame1();
int EventGame1(int nEvent, int nKey);
CString GetNameTotalSongsGame1(int nIndex);
CString GetArtistTotalSongsGame1(int nIndex);
CString GetBPMTotalSongsGame1(int nIndex);
CString GetIDTotalSongsGame1(int nIndex);
bool IsAvalibleModeTotal(int nIndexSong, int nModeGame1Song);

#endif // PIU_GAME1