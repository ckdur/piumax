#ifndef PIU_GAME3
#define PIU_GAME3

#include "stdafx.h"

int CreateContextGame3();
void DestroyContextGame3();
void ActualizateContextGame3();
void ClearContextGame3();
int FrameGame3();
int EventGame3(int nEvent, int nKey);
CString GetNameTotalSongsGame3(int nIndex);
CString GetArtistTotalSongsGame3(int nIndex);
CString GetBPMTotalSongsGame3(int nIndex);
CString GetIDTotalSongsGame3(int nIndex);
bool IsAvalibleModeTotalGame3(int nIndexSong, int nModeGame3Song);

#endif // PIU_GAME3