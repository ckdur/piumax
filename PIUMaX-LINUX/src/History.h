#ifndef PIU_HISTORY
#define PIU_HISTORY

#include "stdafx.h"

int CreateContextHistory();
void DestroyContextHistory();
int ActualizateContextHistory();
void ClearContextHistory();
int FrameHistory();
int EventHistory(int nEvent, int nKey);

extern CString strHis;

#endif // PIU_HISTORY