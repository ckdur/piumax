#ifndef PIU_USB
#define PIU_USB

#include "stdafx.h"

int CreateContextUsb();
void DestroyContextUsb();
void ActualizateContextUsb();
int FrameUsb();
int EventUsb(int nEvent, int nKey);

#endif // PIU_USB