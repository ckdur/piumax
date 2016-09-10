#include "MaxStepSmu.h"

void CMaxStepSmu::StepEvaluate(int nLimitX, int* nMin, int* nMax, int nLevelMach)
{
	for( int n = 0; n < stepc.hStep.nSizeArrowGroups; n++)
	{
		if(!stepc.hStepGroup[n].hGroup.nNoCount)
		{
			// ** FASE DE DEPURACION DE LOS HOLDS
			vector<int> vnTakeTap;
			vector<int> vnTakeTapOffset;

			vector<int> vnTakeItem;
			vector<int> vnTakeItemOffset;

			bool *bPicked = new bool[nLimitX];
			for(int x = 0; x < nLimitX; x++) bPicked[x] = 0;

			for(int y = nMin[n]; y < nMax[n]; y++)
			{
				int nCombo = 0;
				bool bCount = false;
				__int64 nUbi = BeatWArrow(y, n);
				int nOffset = int(nUbi-m_nTime);
				// Make sure there is no tap in line
				bool bTake = true;		// true -> No tiene taps
				for(int x = 0; x < nLimitX; x++)
				{
					int nArrow = GetArrow(x, y, n);
					if(nArrow == 1)
					{
						bTake = false;
						break;
					}
				}
				// use this function
				if(nOffset <= m_nJudTimes[JUDAMENT_PERFECT])
				{
					int nCountArrows = 0;
					int nCountHolded = 0;
					for(int x = 0; x < nLimitX; x++)
					{
						int nArrow = GetArrow(x, y, n);
						if(nArrow >= 2 && nArrow <= 4)
						{
							int nP = GetArrowPar(x, y, n);
							if(nCombo < nP) nCombo = nP;
							if(GetArrowStatus(x, y, n) < 0)
							{
								if(bTake) nCountArrows++;
								if(evarrows.bArrows[x])
								{
									if(bTake) nCountHolded++;
									SetArrowStatus(x, y, n, JUDAMENT_PERFECT);
								}
							}
						}
					}
					if(nCountArrows <= nCountHolded && nCountArrows > 0 && bTake)
					{
						Score_DoJudgament(y, n, JUDAMENT_PERFECT, nCombo);
						for(int x = 0; x < nLimitX; x++)
						{
							int nArrow = GetArrow(x, y, n);
							if(nArrow >= 2 && nArrow <= 4)
							{
								//evarrows.nGlowArrows[x] = m_nTime;
								bPicked[x] = true;
							}
						}
					}
					else if(nOffset < -nLevelMach && nCountArrows > 0 && bTake)
					{
						Score_DoJudgament(y, n, JUDAMENT_MISS, nCombo);
						for(int x = 0; x < nLimitX; x++)
						{
							int nArrow = GetArrow(x, y, n);
							if(nArrow >= 2 && nArrow <= 4)
							{
								SetArrowStatus(x, y, n, JUDAMENT_MISS);
							}
						}
					}
				}

				// ** PARTE DE LA FASE DEPURACION TAP
				// filtrar los taps en orden
				if(!bTake && nOffset <= nLevelMach)
				{
					bool bCare = false;
					for(int x = 0; x < nLimitX; x++)
					{
						if(GetArrow(x, y, n) == 1 && GetArrowStatus(x, y, n) < 0)
							bCare = true;
					}
					if(bCare)
					{
						vector<int>::iterator it, it2;
						it2 = vnTakeTap.begin();
						for(it = vnTakeTapOffset.begin(); it != vnTakeTapOffset.end(); it++)
						{
							it2++;
							if(abs(nOffset) < (*it))
							{
								break;
							}
						}
						vnTakeTapOffset.insert(it, abs(nOffset));
						vnTakeTap.insert(it2, y);
					}
				}

				// ** PARTE DE LA FASE DEPURACION ITEM
				// filtrar los item en orden
				if(nOffset <= nLevelMach)
				{
					bool bCare = false;
					for(int x = 0; x < nLimitX; x++)
					{
						if(GetArrow(x, y, n) >= ITEM_STARTUP && GetArrowStatus(x, y, n) < 0)
							bCare = true;
					}
					if(bCare)
					{
						vector<int>::iterator it, it2;
						it2 = vnTakeItem.begin();
						for(it = vnTakeItemOffset.begin(); it != vnTakeItemOffset.end(); it++)
						{
							it2++;
							if(abs(nOffset) < (*it))
							{
								break;
							}
						}
						vnTakeItemOffset.insert(it, abs(nOffset));
						vnTakeItem.insert(it2, y);
					}
				}
			}

			// ** FASE DE DEPURACION DE LOS TAP
			for(size_t k = 0; k < vnTakeTap.size(); k++)
			{
				int nCombo = 0;
				int y = vnTakeTap[k];
				int nMaxOffset = 0;
				__int64 nUbi = BeatWArrow(y, n);
				int nCurOffset = int(nUbi-m_nTime);

				for(int x = 0; x < nLimitX; x++)
				{
					int nArrow = GetArrow(x, y, n);
					if(nArrow >= 1 && nArrow <= 4)
					{
						int nP = GetArrowPar(x, y, n);
						if(nCombo < nP) nCombo = nP;

						int nOffset = abs(int((__int64)(nUbi)-evarrows.nArrows[x]));

						if(nMaxOffset <= nOffset && nArrow == 1 && nMaxOffset >= 0)
						{
							nMaxOffset = nOffset;
						}
						else if(nArrow >= 2 && nArrow <= 4 && GetArrowStatus(x, y, n) < 0)
						{
							nMaxOffset = -1;
						}
					}

				}

				bool bJudged = false;
				for(int j = 0; j < JUDAMENT_MISS; j++)
				{
					if(nMaxOffset < m_nJudTimes[j] && nMaxOffset >= 0)
					{
						Score_DoJudgament(y, n, j, nCombo);
						for(int x = 0; x < nLimitX; x++)
						{
							int nArrow = GetArrow(x, y, n);
							if(nArrow == 1)
							{
								evarrows.nArrows[x] = stepc.hStep.nOffset;
								//evarrows.nGlowArrows[x] = m_nTime;
								SetArrowStatus(x, y, n, j);
							}
							if(j < JUDGAMENT_DRAW_LOSE && nArrow >= 1 && nArrow <= 4)
							{
								bPicked[x] = true;
							}
						}
						bJudged = true;
						break;
					}
				}

				if(!bJudged && nCurOffset < -nLevelMach)
				{
					Score_DoJudgament(y, n, JUDAMENT_MISS, nCombo);
					for(int x = 0; x < nLimitX; x++)
					{
						if(GetArrow(x, y, n) == 1)
						{
							SetArrowStatus(x, y, n, JUDAMENT_MISS);
						}
					}
				}
			}

			// ** FASE DE DEPURACION DE LOS ITEMS
			for(size_t k = 0; k < vnTakeItem.size(); k++)
			{
				int nCombo = 0;
				int y = vnTakeItem[k];
				bool bCount = false;
				__int64 nUbi = BeatWArrow(y, n);
				int nCurOffset = int(nUbi-m_nTime);
				for(int x = 0; x < nLimitX; x++)
				{
					int nOffset = abs(int((__int64)(nUbi)-evarrows.nArrows[x]));
					int nStatus = GetArrowStatus(x, y, n);
					if(nOffset <= nLevelMach && nStatus < 0 && GetArrow(x, y, n) >= ITEM_STARTUP)
					{
						evarrows.nArrows[x] = stepc.hStep.nOffset;
						//evarrows.nGlowArrows[x] = m_nTime;
						SetArrowStatus(x, y, n, JUDAMENT_PERFECT);
						Score_DoItem(y, x, n, GetArrow(x, y, n), GetArrowPar(x, y, n));
						bPicked[x] = true;
					}
					else if (nCurOffset <= -nLevelMach && nStatus < 0 && GetArrow(x, y, n) >= ITEM_STARTUP)
					{
						SetArrowStatus(x, y, n, JUDAMENT_MISS);
						Score_DoNotItem(y, x, n, GetArrow(x, y, n), GetArrowPar(x, y, n));
					}
				}
			}

			for(int x = 0; x < nLimitX; x++)
			{
				if(bPicked[x]) evarrows.nGlowArrows[x] = m_nTime;
			}

			delete bPicked;
		}
	}
}
