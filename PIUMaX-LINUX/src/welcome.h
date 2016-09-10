#ifndef PIU_WELCOME
#define PIU_WELCOME

#include "stdafx.h"

int CreateContextWelcome();
void DestroyContextWelcome();
void ActualizateContextWelcome();
void ClearContextWelcome();
int FrameWelcome();
int EventWelcome(int nEvent, int nKey);

#endif // PIU_WELCOME