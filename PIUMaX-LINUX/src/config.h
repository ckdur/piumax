#ifndef PIU_CONFIG
#define PIU_CONFIG

#include "stdafx.h"

int CreateContextConfig();
void DestroyContextConfig();
void ActualizateContextConfig();
int FrameConfig();
int EventConfig(int nEvent, int nKey);
int EventConfigEx(SDL_Event ev);

#endif // PIU_CONFIG