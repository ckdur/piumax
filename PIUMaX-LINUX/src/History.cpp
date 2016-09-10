#include "game1.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "HistoryLoader.h"

CHistoryLoader* his;
CString strHis;
bool bEnArrowsHistory[10];
__int64 nTimeEnArrowsHistory[10];

int CreateContextHistory()
{
	his = new CHistoryLoader();
	return 1;
}

void DestroyContextHistory()
{
	delete his;
}

int ActualizateContextHistory()
{
	for(int x = 0; x < 10; x++)
	{
		bEnArrowsHistory[x] = false;
		nTimeEnArrowsHistory[x] = 0;
	}
	if(!his->LoadText(strHis)) return 0;
	dwTick = dwLastTick = SDL_GetTicks();
	return 1;
}

void ClearContextHistory()
{
	his->Destroy();
}

int FrameHistory()
{
	his->Update(dwWait);
	if(!his->Draw()) 
	{
		ClearContextHistory();
		return 0;
	}
	for(int x = 0; x < 2; x++) if(np[x])
	{
		static bool bCh[] = {false, false, false, false, false};
		if(x == 0)
		{
			for(int y = 0; y < 5; y++)
			{
				if(bArrows[y] && !bEnArrowsHistory[y])
				{
					bEnArrowsHistory[y] = true;
					//nTimeEnArrowsHistory[y] = nCountHistory;
				}
				if(!bArrows[y] && bEnArrowsHistory[y])
				{
					bEnArrowsHistory[y] = false;
					bCh[y] = false;
				}
			}
		}
		if(x == 1)
		{
			for(int y = 5; y < 10; y++)
			{
				if(bArrows[y] && !bEnArrowsHistory[y-5])
				{
					bEnArrowsHistory[y-5] = true;
					//nTimeEnArrowsHistory[y-5] = nCountHistory;
				}
				if(!bArrows[y] && bEnArrowsHistory[y-5])
				{
					bEnArrowsHistory[y-5] = false;
					bCh[y-5] = false;
				}
			}
		}
		if(bEnArrowsHistory[0] && !bCh[0])
		{
			bCh[0] = true;
		}
		if(bEnArrowsHistory[4] && !bCh[4])
		{
			bCh[4] = true;
		}
		if(bEnArrowsHistory[1] && !bCh[1])
		{
			bCh[1] = true;
		}
		if(bEnArrowsHistory[3] && !bCh[3])
		{
			bCh[3] = true;
		}
		if(bEnArrowsHistory[2] && !bCh[2])
		{
			his->PressCenter();
			bCh[2] = true;
		}
	}
	return 1;
}

int EventHistory(int nEvent, int nKey)
{
	if(nEvent == EV_KEYDOWN)
	{
		if(nKey == EV_P1_ARROW1) bArrows[0] = true;
		if(nKey == EV_P1_ARROW7) bArrows[1] = true;
		if(nKey == EV_P1_CENTER) bArrows[2] = true;
		if(nKey == EV_P1_ARROW9) bArrows[3] = true;
		if(nKey == EV_P1_ARROW3) bArrows[4] = true;
		if(nKey == EV_P2_ARROW1) bArrows[5] = true;
		if(nKey == EV_P2_ARROW7) bArrows[6] = true;
		if(nKey == EV_P2_CENTER) bArrows[7] = true;
		if(nKey == EV_P2_ARROW9) bArrows[8] = true;
		if(nKey == EV_P2_ARROW3) bArrows[9] = true;
	}
	if(nEvent == EV_KEYUP)
	{
		if(nKey == EV_P1_ARROW1) bArrows[0] = false;
		if(nKey == EV_P1_ARROW7) bArrows[1] = false;
		if(nKey == EV_P1_CENTER) bArrows[2] = false;
		if(nKey == EV_P1_ARROW9) bArrows[3] = false;
		if(nKey == EV_P1_ARROW3) bArrows[4] = false;
		if(nKey == EV_P2_ARROW1) bArrows[5] = false;
		if(nKey == EV_P2_ARROW7) bArrows[6] = false;
		if(nKey == EV_P2_CENTER) bArrows[7] = false;
		if(nKey == EV_P2_ARROW9) bArrows[8] = false;
		if(nKey == EV_P2_ARROW3) bArrows[9] = false;
	}
	return 1;
}