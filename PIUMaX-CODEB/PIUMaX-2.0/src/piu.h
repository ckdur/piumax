#ifndef PIUMAX_HEADER
#define PIUMAX_HEADER

#include "stdafx.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"

#include "MaxInterface.h"

#define MAX_LOADSTRING 100

// ** Auxiliares

int ProcEvent(int nKey, int nEvent);

void DrawInterface();

// ** Utilidades para eventos

int EventThread(void* data);
int PollEventThread(int* nKey, int* nEvent);

int ReadModeFromACC(LPCTSTR strFile);

int ReadLightsConfigFromACC(LPCTSTR strFile);

#endif // PIUMAX_HEADER