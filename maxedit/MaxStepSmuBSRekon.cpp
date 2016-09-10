#include "MaxStepSmu.h"

void CMaxStepSmu::BSEvaluate(int nLimitX, int nLevelMach)
{
	int nPArrow = -1;	// Asumir la posible flecha en juego como -1
	bool bTimize = false;	// Asumir que es una respuesta legitima de BS
	__int64 nMinusOffset = nLevelMach;
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	if(stepc.hStep.nSizeArrowGroups > 0) if(!stepc.hStepGroup[0].hGroup.nNoCount) for(int y = 0; y < bsc.nSizeBSAnswers; y++)
	{
		bool bCountThis = false;
		for(int x = 0; x < nLimitX; x++)
		{
			if(bsc.dwBSAnswers
				&& (status.nBSStatus[y*nArrowsPerLine+x] != 1))
				bCountThis = true;
			if(bsc.dwBSAnswers
				&& (status.nBSStatus[y*nArrowsPerLine+x] == 1))
			{
				bCountThis = false;
				break;
			}
		}
		if(bCountThis)
		{
			__int64 nUbi = bsc.nWWBSAnswers[y];
			__int64 nOffset = nUbi-m_nTime;	// Desfase de la flecha con respecto al origen
			if(nOffset < -nLevelMach)	// Esta flecha se ha perdido
			{
				// Aqui cuenta un X
				if(!stepc.hStepGroup[0].hGroup.nNoMiss)
				{
					for(int x = 0; x < nLimitX; x++)
						if(bsc.nBSAnswers[y*nArrowsPerLine+x]) status.nBSStatus[y*nArrowsPerLine+x] = 1;
					evarrows.nLastBSEva = m_nTime;
					evarrows.nTypeBSEva = 1;
					status.nBSIncorrect++;
				}
			}
			else if(nOffset >= -nLevelMach && nOffset <= nLevelMach)	// Esta flecha se toma en cuenta!
			{
				if(nMinusOffset >= abs(nOffset))
				{
					nMinusOffset = abs(nOffset);
					nPArrow = y;
				}
			}
			else if(nOffset > nLevelMach)	// Esta flecha no cuenta
			{
				break;
			}
		}
	}
	if(nPArrow != -1)
	{
		bool bCountThis = false;
		for(int x = 0; x < nLimitX; x++)
		{
			if(bsc.dwBSAnswers
				&& (status.nBSStatus[nPArrow*nArrowsPerLine+x] != 1))
				bCountThis = true;
			if(bsc.dwBSAnswers
				&& (status.nBSStatus[nPArrow*nArrowsPerLine+x] == 1))
			{
				bCountThis = false;
				break;
			}
		}
		if(bCountThis)
		{
			__int64 nUbi = bsc.nWWBSAnswers[nPArrow];
			__int64 nOffset = nLevelMach;	// Desfase de la flecha con respecto al origen
			bool bCount = false;
			int nArrow = -1;
			for(int x = 0; x < nLimitX; x++)
			{
				__int64 off;
				DWORD dwArrow = bsc.dwBSAnswers[nPArrow*nArrowsPerLine+x];
				int nAnswer = bsc.nBSAnswers[nPArrow*nArrowsPerLine+x];
				if(dwArrow)
				{
					off = abs(evarrows.nArrows[x]-nUbi);
					bCount = true;
				}
				if(dwArrow && off <= nOffset)
				{
					nOffset = off;
					nArrow = x;
				}
				if(HIWORD(dwArrow) == 0 && dwArrow)
				{
					bTimize = true;
				}
			}
			if(nOffset <= nLevelMach && bCount == true && nArrow != -1) // Esta entre el perfect y el bad
			{
				if(!bTimize)
				{
					if(nOffset <= nLevelMach && nOffset >= 0)
					{
						status.nBSStatus[nPArrow*nArrowsPerLine+nArrow] = 1;
						evarrows.nGlowArrows[nArrow] = m_nTime;
						if(bsc.nBSAnswers[nPArrow*nArrowsPerLine+nArrow])
						{
							status.nBSCorrect++;
							evarrows.nLastBSEva = m_nTime;
							evarrows.nTypeBSEva = 2;
						}
						else
						{
							status.nBSIncorrect++;
							evarrows.nLastBSEva = m_nTime;
							evarrows.nTypeBSEva = 1;
						}
					}
				}
				else
				{
					for(int j = 0; j < JUDAMENT_MISS; j++)
					{
						if(nOffset < m_nJudTimes[j] && nOffset >= 0)
						{
							status.nBSStatus[nPArrow*nArrowsPerLine+nArrow] = 1;
							Score_DoJudgament(nPArrow, 0, j, 1);
							break;
						}
					}
				}
				evarrows.nArrows[nArrow] = stepc.hStep.nOffset;
			}
		}
	}
}
