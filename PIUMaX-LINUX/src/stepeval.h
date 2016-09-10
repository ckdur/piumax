#ifndef PIU_STEPEVAL
#define PIU_STEPEVAL

#include "stdafx.h"

int CreateContextStepEval();
void DestroyContextStepEval();
void ActualizateContextStepEval();
void ClearContextStepEvalRes();
void ActualizateContextStageStepEval();
void ClearContextStageStepEval();
int FrameStepEval();
int EventStepEval(int nEvent, int nKey);
int FrameStageStepEval();
int FrameStageBreakStepEval();
int FrameGameOverStepEval();
void ActualizateContextHSStepEval();
int FrameHSStepEval();

extern int nModeStepEval;
extern int bMissionSucess;
extern int nSizeCondSucess;
extern CString* strCondSucess;

extern double dNotesP[2];

extern bool bNewHighScore;

#endif // PIU_STEPEVAL
