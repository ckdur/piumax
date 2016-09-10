#include "main_document.h"
#include "libc_max_common.h"
#include "basutil.h"

#define MAX_BEATSPLIT 192

// THIS IS A THRESHOLD FOR TRANSLATING BPM - STOPS UBICATIONS. Imprecise by this value, if this is 0 then
// Is the most exact possible until the definition of MAX_BEATSPLIT
#define UMBRAL_MS_SOB 5
#define ARGNOTE_NOMISS 0x00000001
#define ARGNOTE_NOCOUNT 0x00000002

int CMaxDocument::Convert_SKSFv1_SKSFv2(STEP* fdest, SKSFv1* forigin) // v1 2 v2
{
	memset(&fdest->hStep, 0, sizeof(STEPHEADER));

	fdest->hStep.nSizeChangeBPM = forigin->hStep.nSizeChangeBPM-1;
	fdest->hStep.nSizeBeatSplit = forigin->hStep.nSizeBeatSplit-1;
	fdest->hStep.nSizeChangeVel = forigin->hStep.nSizeChangeVel-1;
	fdest->hStep.nSizeJumps = forigin->hStep.nSizeJumps;
	fdest->hStep.nSizeTickCount = forigin->hStep.nSizeTickcount-1;
	fdest->hStep.nSizeStops = forigin->hStep.nSizeStops;
	fdest->hStep.nArrowsPerLine = 10;
	fdest->hStep.nSizeAttrib = 5;
	fdest->hStep.dwLSong = forigin->hStep.dwLSong;
	fdest->hStep.dwVersion = 0x20;
	fdest->hStep.nOffset = forigin->hStep.nOffset;
	fdest->hStep.nDificulty = forigin->hStep.nDificulty;
	fdest->hStep.nMode = 1;

	for(int x = 0; x < 3; x++)
	{
		int nSizeArrows;
		switch(x)
		{
		case 0:
			nSizeArrows = forigin->hStep.nSizeArrows1;
			break;
		case 1:
			nSizeArrows = forigin->hStep.nSizeArrows2;
			break;
		case 2:
			nSizeArrows = forigin->hStep.nSizeArrows3;
			break;
		}
		if(nSizeArrows > 0)
		{
			fdest->hStep.nSizeArrowGroups++;
		}
	}

	Init_SKSF2(fdest);

	// Anotate statics
	fdest->fAttrib[0] = (float)forigin->hStep.dVoltage;
	fdest->fAttrib[1] = (float)forigin->hStep.dStrength;
	fdest->fAttrib[2] = (float)forigin->hStep.dFreeze;
	fdest->fAttrib[3] = (float)forigin->hStep.dCaos;
	fdest->fAttrib[4] = (float)forigin->hStep.dAir;

	// Generate the beatsplits (for stop conversion)
	vector<int> nBS_Stops, nS_Stops;
	nBS_Stops.resize(forigin->hStep.nSizeStops, 1);
	nS_Stops.resize(forigin->hStep.nSizeStops, 1);
	int nSum = 0;
	for(int x = 0; x < forigin->hStep.nSizeStops; x++)
	{
		double dMinusSobBeat = 1.0;	// Minor sobrant beat
		int nBS = 1;

		double dBPMa = 1;
		for(int i = forigin->hStep.nSizeBeatSplit-1; i >= 0; i--)
		{
			dBPMa = forigin->dChangeBPM[i];
			if(forigin->nWChangeBPM[i] <= forigin->nWStops[x])
			{
				break;
			}
		}

		double dUmSobBeat = dBPMa*UMBRAL_MS_SOB/60000;

		for(int i = 1; i <= MAX_BEATSPLIT; i++)
		{
			double dFrac, dInt;
			dFrac = modf(forigin->dStops[x]*double(i), &dInt)/double(i);
			if(dMinusSobBeat > dFrac)
			{
				dMinusSobBeat = dFrac;
				nBS = i;
			}
			if(dMinusSobBeat <= dUmSobBeat) break;
		}
		nBS_Stops[x] = nBS;
		nS_Stops[x] = int(forigin->dStops[x]*double(nBS));
		nSum += nS_Stops[x];
	}

	fdest->hStep.fBPM = float(forigin->dChangeBPM[0]);
	for(int x = 1; x < forigin->hStep.nSizeChangeBPM; x++)
	{
		int nLoc = forigin->nWChangeBPM[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nWChangeBPM[x] > forigin->nWStops[i])
			{
				nLoc+=nS_Stops[i];
			}
		}
		fdest->nWChangeBPM[x-1] = nLoc;
		fdest->fChangeBPM[x-1] = float(forigin->dChangeBPM[x]);
	}

	fdest->hStep.nTickCount = forigin->nTickCount[0]*forigin->nBeatSplit[0];
	for(int x = 1; x < forigin->hStep.nSizeTickcount; x++)
	{
		int nLoc = forigin->nWTickCount[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nWTickCount[x] > forigin->nWStops[i])
			{
				nLoc+=nS_Stops[i];
			}
		}
		fdest->nWTickCount[x-1] = nLoc;

		int nBSa = 1;
		for(int i = forigin->hStep.nSizeBeatSplit-1; i >= 0; i--)
		{
			nBSa = forigin->nBeatSplit[i];
			if(forigin->nWBeatSplit[i] <= forigin->nWTickCount[x])
			{
				break;
			}
		}

		fdest->nTickCount[x-1] = forigin->nTickCount[x]*nBSa;
	}

	fdest->hStep.nBeatSplit = forigin->nBeatSplit[0];
	for(int x = 1; x < forigin->hStep.nSizeBeatSplit; x++)
	{
		int nLoc = forigin->nWBeatSplit[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nWBeatSplit[x] > forigin->nWStops[i])
			{
				nLoc+=nS_Stops[i];
			}
		}

		int nTCa = 1;
		for(int i = forigin->hStep.nSizeTickcount-1; i >= 0; i--)
		{
			nTCa = forigin->nTickCount[i];
			if(forigin->nWTickCount[i] <= forigin->nWTickCount[x])
			{
				break;
			}
		}

		int nVer = -1;
		for(int i = 0; i < fdest->hStep.nSizeTickCount; i++)
		{
			if(nLoc == fdest->nWTickCount[i])
			{
				nVer = i;
				break;
			}
		}

		if(nVer == -1)
		{
			fdest->hStep.nSizeTickCount++;
			ReInit_SKSF2(fdest);
			fdest->nWTickCount[fdest->hStep.nSizeTickCount-1] = nLoc;
			fdest->nTickCount[fdest->hStep.nSizeTickCount-1] = forigin->nBeatSplit[x]*nTCa;
		}

		fdest->nWBeatSplit[x-1] = nLoc;
		fdest->nBeatSplit[x-1] = forigin->nBeatSplit[x];
	}

	fdest->hStep.fVel = float(forigin->dChanges[0]);
	for(int x = 1; x < forigin->hStep.nSizeChangeVel; x++)
	{
		int nLoc = forigin->nIChangeVel[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nIChangeVel[x] > forigin->nWStops[i])
			{
				nLoc+=nS_Stops[i];
			}
		}
		fdest->nIChangeVel[x-1] = nLoc;
		nLoc = forigin->nEChangeVel[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nEChangeVel[x] > forigin->nWStops[i])
			{
				nLoc+=nS_Stops[i];
			}
		}
		fdest->nEChangeVel[x-1] = nLoc;
		fdest->fChangeVel[x-1] = float(forigin->dChanges[x]);
		fdest->fIChangeVel[x-1] = float(forigin->dChanges[x-1]);
	}

	for(int x = 0; x < forigin->hStep.nSizeJumps; x++)
	{
		int nLoc = forigin->nIJumps[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nIJumps[x] >= forigin->nWStops[i])
			{
				nLoc+=nS_Stops[i];
			}
		}
		fdest->nIJumps[x] = nLoc;
		nLoc = forigin->nEJumps[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nEJumps[x] > forigin->nWStops[i])
			{
				nLoc+=nS_Stops[i];
			}
		}
		fdest->nEJumps[x] = nLoc;
	}

	for(int x = 0; x < forigin->hStep.nSizeStops; x++)
	{
		int nLoc = forigin->nWStops[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nWStops[x] > forigin->nWStops[i])
			{
				nLoc+=nS_Stops[i];
			}
		}
		fdest->nIStops[x] = nLoc;
		fdest->nEStops[x] = nLoc+nS_Stops[x];

		// Found back past BS, if this BS is equal to past BS,
		// is not necesary to add another
		int nBSa = fdest->hStep.nBeatSplit;
		int nWBSa = 0;
		int niBSa = -1;
		for(int i = fdest->hStep.nSizeBeatSplit-1; i >= 0; i--)
		{
			if(fdest->nWBeatSplit[i] <= nLoc)
			{
				nBSa = fdest->nBeatSplit[i];
				nWBSa = fdest->nWBeatSplit[i];
				niBSa = i;
				break;
			}
		}

		if(nBSa != nBS_Stops[x])
		{
			int nAdd = nWBSa==nLoc?1:2;
			int nMod1 = nWBSa==nLoc?niBSa:fdest->hStep.nSizeBeatSplit;
			int nMod2 = nWBSa==nLoc?fdest->hStep.nSizeBeatSplit:(fdest->hStep.nSizeBeatSplit+1);
			fdest->hStep.nSizeBeatSplit+=nAdd;
			ReInit_SKSF2(fdest);
			if(nMod1 != -1)
			{
				fdest->nWBeatSplit[nMod1] = fdest->nIStops[x];
				fdest->nBeatSplit[nMod1] = nBS_Stops[x];
			}
			else fdest->hStep.nBeatSplit = nBS_Stops[x];
			fdest->nWBeatSplit[nMod2] = fdest->nEStops[x];
			fdest->nBeatSplit[nMod2] = nBSa;
		}

		for(int y = 0; y < fdest->hStep.nSizeBeatSplit; y++)
		{
			for(int x = 0; x < (fdest->hStep.nSizeBeatSplit-1); x++)
			{
				if(fdest->nWBeatSplit[x] > fdest->nWBeatSplit[x+1])
				{
					int nTemp1 = fdest->nWBeatSplit[x];
					int dTemp2 = fdest->nBeatSplit[x];
					fdest->nWBeatSplit[x] = fdest->nWBeatSplit[x+1];
					fdest->nBeatSplit[x] = fdest->nBeatSplit[x+1];
					fdest->nWBeatSplit[x+1] = nTemp1;
					fdest->nBeatSplit[x+1] = dTemp2;
				}
			}
		}
	}

	Fix(fdest);

	// Convert steps
	int a = 0;
	for(int x = 0; x < 3; x++)
	{
		uint32_t* dwArrows;
		uint32_t dwArgs;
		int nSizeArrows;
		int nSkin;
		double dClipMin;
		double dClipMax;
		switch(x)
		{
		case 0:
			nSizeArrows = forigin->hStep.nSizeArrows1;
			dwArrows = forigin->dwArrows1;
			dwArgs = forigin->hStep.dwArgArrows1;
			nSkin = forigin->hStep.nSkin1;
			dClipMin = forigin->hStep.dClipMin1;
			dClipMax = forigin->hStep.dClipMax1;
			break;
		case 1:
			nSizeArrows = forigin->hStep.nSizeArrows2;
			dwArrows = forigin->dwArrows2;
			dwArgs = forigin->hStep.dwArgArrows2;
			nSkin = forigin->hStep.nSkin2;
			dClipMin = forigin->hStep.dClipMin2;
			dClipMax = forigin->hStep.dClipMax2;
			break;
		case 2:
			nSizeArrows = forigin->hStep.nSizeArrows3;
			dwArrows = forigin->dwArrows3;
			dwArgs = forigin->hStep.dwArgArrows3;
			nSkin = forigin->hStep.nSkin3;
			dClipMin = forigin->hStep.dClipMin3;
			dClipMax = forigin->hStep.dClipMax3;
			break;
		}
		if(nSizeArrows > 0)
		{
			fdest->hStepGroup[a].hGroup.nSizeArrows = nSizeArrows+nSum;

			fdest->hStepGroup[a].hGroup.nNoMiss = (dwArgs&ARGNOTE_NOMISS)?1:0;
			fdest->hStepGroup[a].hGroup.nNoCount = (dwArgs&ARGNOTE_NOCOUNT)?1:0;
			fdest->hStepGroup[a].hGroup.fVanishSup = float(((dClipMin*60.0)-TOP_ARROWS)/(BOTTOM_ARROWS-TOP_ARROWS))*100.0f;
			fdest->hStepGroup[a].hGroup.fVanishInf = float(((dClipMax*60.0)-BOTTOM_ARROWS)/(TOP_ARROWS-BOTTOM_ARROWS))*100.0f;
			if(fdest->hStepGroup[a].hGroup.fVanishSup <= 0.0f || fdest->hStepGroup[a].hGroup.fVanishSup >= 100.0f)
			{
				fdest->hStepGroup[a].hGroup.nIsVanishSup = 0;
				fdest->hStepGroup[a].hGroup.fVanishSup = 0.0f;
			}
			else fdest->hStepGroup[a].hGroup.nIsVanishSup = 1;
			if(fdest->hStepGroup[a].hGroup.fVanishInf <= 0.0f || fdest->hStepGroup[a].hGroup.fVanishInf >= 100.0f)
			{
				fdest->hStepGroup[a].hGroup.nIsVanishInf = 0;
				fdest->hStepGroup[a].hGroup.fVanishInf = 0.0f;
			}
			else fdest->hStepGroup[a].hGroup.nIsVanishInf = 1;
			fdest->hStepGroup[a].hGroup.nSkin = nSkin;
			fdest->hStepGroup[a].hGroup.nIsSkin = nSkin==0?0:1;

			ReInit_SKSF2(fdest);
			memset(fdest->hStepGroup[a].nArrows, 0, fdest->hStepGroup[a].hGroup.nSizeArrows*fdest->hStep.nArrowsPerLine*sizeof(int));
			memset(fdest->hStepGroup[a].nPArrows, 0, fdest->hStepGroup[a].hGroup.nSizeArrows*fdest->hStep.nArrowsPerLine*sizeof(int));
			for(int y = 0; y < nSizeArrows; y++)
			{
				int nLoc = y;
				int nDistribute = -1;
				for(int i = 0; i < forigin->hStep.nSizeStops; i++)
				{
					if((y >= forigin->nWStops[i] && !forigin->hStep.bGetArrowInStop) ||
						(y > forigin->nWStops[i] && forigin->hStep.bGetArrowInStop))
					{
						nLoc+=nS_Stops[i];
					}
					if(y == forigin->nWStops[i] && forigin->hStep.bGetArrowInStop)
					{
						nDistribute = i;
					}
				}
				for(int k = 0; k < 10; k++)
				{
					int nType = LOWORD(dwArrows[y*10+k]);
					if(nType == 2)
					{
						nType = 3;
						if((y-1) >= 0)
						{
							if(LOWORD(dwArrows[(y-1)*10+k]) != 2)
								nType = 2;
						}
						else if((y-1) < 0)
							nType = 2;
						if((y+1) < nSizeArrows)
						{
							if(LOWORD(dwArrows[(y+1)*10+k]) != 2)
								nType = 4;
						}
						else if((y+1) >= nSizeArrows)
							nType = 4;
					}
					else if(nType > 2)
					{
						nType += 2;
					}
					fdest->hStepGroup[a].nArrows[nLoc*10+k] = nType;
					fdest->hStepGroup[a].nPArrows[nLoc*10+k] = HIWORD(dwArrows[y*10+k]);

					// Distribute
					if(LOWORD(dwArrows[y*10+k])==2 && LOWORD(dwArrows[(y+1)*10+k])==2 && nDistribute != -1)
					{
						for(int w = 1; w < nS_Stops[nDistribute]; w++)
						{
							fdest->hStepGroup[a].nArrows[(nLoc+w)*10+k] = 3;
							fdest->hStepGroup[a].nPArrows[(nLoc+w)*10+k] = HIWORD(dwArrows[y*10+k]);
						}
					}
				}
			}
			a++;
		}
	}
	return 1;
}

int CMaxDocument::Convert_SKSFv2_SKSFv1(SKSFv1* fdest, STEP* forigin)// v2 2 v1
{
	memset(&fdest->hStep, 0, sizeof(SKSFv1HEADER));

	fdest->hStep.nSizeChangeBPM = forigin->hStep.nSizeChangeBPM+1;
	fdest->hStep.nSizeBeatSplit = forigin->hStep.nSizeBeatSplit+1;
	fdest->hStep.nSizeChangeVel = forigin->hStep.nSizeChangeVel+1;
	fdest->hStep.nSizeJumps = forigin->hStep.nSizeJumps;
	fdest->hStep.nSizeTickcount = forigin->hStep.nSizeTickCount+1;
	fdest->hStep.nSizeStops = forigin->hStep.nSizeStops;
	fdest->hStep.dwLSong = forigin->hStep.dwLSong;
	fdest->hStep.dwVersion = 0x18;
	fdest->hStep.nOffset = forigin->hStep.nOffset;
	fdest->hStep.nDificulty = forigin->hStep.nDificulty;
	fdest->hStep.bGetArrowInStop = false;

	// Anotate statics
	fdest->hStep.dVoltage = forigin->hStep.nSizeAttrib <= 1?(double)forigin->fAttrib[0]:0.0;
	fdest->hStep.dStrength = forigin->hStep.nSizeAttrib <= 2?(double)forigin->fAttrib[1]:0.0;
	fdest->hStep.dFreeze = forigin->hStep.nSizeAttrib <= 3?(double)forigin->fAttrib[2]:0.0;
	fdest->hStep.dCaos = forigin->hStep.nSizeAttrib <= 4?(double)forigin->fAttrib[3]:0.0;
	fdest->hStep.dAir = forigin->hStep.nSizeAttrib <= 5?(double)forigin->fAttrib[4]:0.0;

	Init_SKSF1(fdest);

	// Generate the beatsplits (for stop conversion)
	fdest->nWChangeBPM[0] = 0;
	fdest->dChangeBPM[0] = forigin->hStep.fBPM;
	for(int x = 0; x < forigin->hStep.nSizeChangeBPM; x++)
	{
		int nLoc = forigin->nWChangeBPM[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nWChangeBPM[x] > forigin->nIStops[i])
			{
				nLoc-=forigin->nEStops[i]-forigin->nIStops[i];
				if(nLoc <= 0) nLoc = 0;
			}
		}
		fdest->nWChangeBPM[x+1] = nLoc;
		fdest->dChangeBPM[x+1] = forigin->fChangeBPM[x];
	}

	fdest->nWTickCount[0] = 0;
	fdest->nTickCount[0] = forigin->hStep.nTickCount/forigin->hStep.nBeatSplit;
	if(fdest->nTickCount[0] <= 0) fdest->nTickCount[0] = 1;
	for(int x = 0; x < forigin->hStep.nSizeTickCount; x++)
	{
		int nLoc = forigin->nWTickCount[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nWTickCount[x] > forigin->nIStops[i])
			{
				nLoc-=forigin->nEStops[i]-forigin->nIStops[i];
				if(nLoc <= 0) nLoc = 0;
			}
		}
		fdest->nWTickCount[x+1] = nLoc;

		int nBSa = -1;
		for(int i = forigin->hStep.nSizeBeatSplit-1; i >= 0; i--)
		{
			nBSa = forigin->nBeatSplit[i];
			if(forigin->nWBeatSplit[i] <= forigin->nWTickCount[x])
			{
				break;
			}
		}
		if(nBSa == -1) nBSa = forigin->hStep.nBeatSplit;

		fdest->nTickCount[x+1] = forigin->nTickCount[x]/nBSa;
		if(fdest->nTickCount[x+1] <= 0) fdest->nTickCount[x+1] = 1;
	}

	fdest->nWBeatSplit[0] = 0;
	fdest->nBeatSplit[0] = forigin->hStep.nBeatSplit;
	for(int x = 0; x < forigin->hStep.nSizeBeatSplit; x++)
	{
		int nLoc = forigin->nWBeatSplit[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nWBeatSplit[x] > forigin->nIStops[i])
			{
				nLoc-=forigin->nEStops[i]-forigin->nIStops[i];
				if(nLoc <= 0) nLoc = 0;
			}
		}

		int nTCa = 1;
		for(int i = forigin->hStep.nSizeTickCount-1; i >= 0; i--)
		{
			nTCa = forigin->nTickCount[i];
			if(forigin->nWTickCount[i] <= forigin->nWTickCount[x])
			{
				break;
			}
		}

		int nVer = -1;
		for(int i = 0; i < fdest->hStep.nSizeTickcount; i++)
		{
			if(nLoc == fdest->nWTickCount[i])
			{
				nVer = i;
				break;
			}
		}

		if(nVer == -1)
		{
			fdest->hStep.nSizeTickcount++;
			ReInit_SKSF1(fdest);
			fdest->nWTickCount[fdest->hStep.nSizeTickcount-1] = nLoc;
			fdest->nTickCount[fdest->hStep.nSizeTickcount-1] = forigin->nBeatSplit[x]/nTCa;
			if(fdest->nTickCount[fdest->hStep.nSizeTickcount-1] <= 0) fdest->nTickCount[fdest->hStep.nSizeTickcount-1] = 1;
		}

		fdest->nWBeatSplit[x+1] = nLoc;
		fdest->nBeatSplit[x+1] = forigin->nBeatSplit[x];
	}

	fdest->nIChangeVel[0] = 0;
	fdest->nEChangeVel[0] = 0;
	fdest->dChanges[0] = forigin->hStep.fVel;
	for(int x = 0; x < forigin->hStep.nSizeChangeVel; x++)
	{
		int nLoc = forigin->nIChangeVel[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nIChangeVel[x] > forigin->nIStops[i])
			{
				nLoc-=forigin->nEStops[i]-forigin->nIStops[i];
				if(nLoc <= 0) nLoc = 0;
			}
		}
		fdest->nIChangeVel[x+1] = nLoc;
		nLoc = forigin->nEChangeVel[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nEChangeVel[x] > forigin->nIStops[i])
			{
				nLoc-=forigin->nEStops[i]-forigin->nIStops[i];
				if(nLoc <= 0) nLoc = 0;
			}
		}
		fdest->nEChangeVel[x+1] = nLoc;
		fdest->dChanges[x+1] = forigin->fChangeVel[x];
	}

	for(int x = 0; x < forigin->hStep.nSizeJumps; x++)
	{
		int nLoc = forigin->nIJumps[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nIJumps[x] >= forigin->nIStops[i])
			{
				nLoc-=forigin->nEStops[i]-forigin->nIStops[i];
				if(nLoc <= 0) nLoc = 0;
			}
		}
		fdest->nIJumps[x] = nLoc;
		nLoc = forigin->nEJumps[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nEJumps[x] > forigin->nIStops[i])
			{
				nLoc-=forigin->nEStops[i]-forigin->nIStops[i];
				if(nLoc <= 0) nLoc = 0;
			}
		}
		fdest->nEJumps[x] = nLoc;
	}

	for(int x = 0; x < forigin->hStep.nSizeStops; x++)
	{
		int nLoc = forigin->nIStops[x];
		for(int i = 0; i < forigin->hStep.nSizeStops; i++)
		{
			if(forigin->nIStops[x] > forigin->nIStops[i])
			{
				nLoc-=forigin->nEStops[i]-forigin->nIStops[i];
			}
		}
		fdest->nWStops[x] = nLoc;
		fdest->dStops[x] = BeatArrowEdit(forigin->nEStops[x])-BeatArrowEdit(forigin->nIStops[x]);
	}

	// Convert steps
	int a = 0;
	uint32_t* dwArrows;
	uint32_t* dwArgs;
	int* nSizeArrows;
	int* nSkin;
	double* dClipMin;
	double* dClipMax;
	for(int x = 0; x < min(forigin->hStep.nSizeArrowGroups, 3); x++)
	{
		switch(x)
		{
		case 0:
			nSizeArrows = &fdest->hStep.nSizeArrows1;
			dwArgs = &fdest->hStep.dwArgArrows1;
			nSkin = &fdest->hStep.nSkin1;
			dClipMin = &fdest->hStep.dClipMin1;
			dClipMax = &fdest->hStep.dClipMax1;
			break;
		case 1:
			nSizeArrows = &fdest->hStep.nSizeArrows2;
			dwArgs = &fdest->hStep.dwArgArrows2;
			nSkin = &fdest->hStep.nSkin2;
			dClipMin = &fdest->hStep.dClipMin2;
			dClipMax = &fdest->hStep.dClipMax2;
			break;
		case 2:
			nSizeArrows = &fdest->hStep.nSizeArrows3;
			dwArgs = &fdest->hStep.dwArgArrows3;
			nSkin = &fdest->hStep.nSkin3;
			dClipMin = &fdest->hStep.dClipMin3;
			dClipMax = &fdest->hStep.dClipMax3;
			break;
		}

		*nSizeArrows = forigin->hStepGroup[x].hGroup.nSizeArrows;

		*dwArgs = (forigin->hStepGroup[x].hGroup.nNoMiss?ARGNOTE_NOMISS:0) |
			(forigin->hStepGroup[x].hGroup.nNoCount?ARGNOTE_NOCOUNT:0);
		*nSkin = 0;
		*dClipMin = 0.2;
		*dClipMax = -1.0;

		if(forigin->hStepGroup[x].hGroup.nIsVanishSup)
		{
			*dClipMin = (forigin->hStepGroup[x].hGroup.fVanishSup/100.0f*(BOTTOM_ARROWS-TOP_ARROWS)+TOP_ARROWS)/60.0;
		}
		if(forigin->hStepGroup[x].hGroup.nIsVanishInf)
		{
			*dClipMax = (forigin->hStepGroup[x].hGroup.fVanishInf/100.0f*(TOP_ARROWS-BOTTOM_ARROWS)+BOTTOM_ARROWS)/60.0;
		}
		if(forigin->hStepGroup[x].hGroup.nIsSkin)
		{
			*nSkin = forigin->hStepGroup[x].hGroup.nSkin;
		}

		ReInit_SKSF1(fdest);
		switch(x)
		{
		case 0:
			dwArrows = fdest->dwArrows1;
			break;
		case 1:
			dwArrows = fdest->dwArrows2;
			break;
		case 2:
			dwArrows = fdest->dwArrows3;
			break;
		}
		memset(dwArrows, 0, (*nSizeArrows)*10*sizeof(uint32_t));
		for(int y = 0; y < forigin->hStepGroup[x].hGroup.nSizeArrows; y++)
		{
			int nLoc = y;
			int nDistribute = -1;
			for(int i = 0; i < forigin->hStep.nSizeStops; i++)
			{
				if(y > forigin->nIStops[i])
				{
					nLoc+=forigin->nEStops[i]-forigin->nIStops[i];
					if(nLoc <= 0) nLoc = 0;
				}
				if(y == forigin->nIStops[i])
				{
					nDistribute = i;
				}
			}
			for(int k = 0; k < min(10, forigin->hStep.nArrowsPerLine); k++)
			{
				int nType = forigin->hStepGroup[x].nArrows[nLoc*forigin->hStep.nArrowsPerLine+k];
				int nPar = forigin->hStepGroup[x].nPArrows[nLoc*forigin->hStep.nArrowsPerLine+k];
				uint32_t dwAdd = 0;
				if(nType <= 1)
				{
					dwAdd = MAKELONG(nPar,nType);
				}
				else if(nType >= 2 && nType <= 4)
				{
					dwAdd = MAKELONG(nPar,2);
				}
				else
				{
					dwAdd = MAKELONG(nPar,nType-2);
				}

				dwArrows[y*10+k] = dwAdd;
			}
		}

	}
	return 1;
}

int CMaxDocument::OpenRAW_SM(LPCTSTR strFile, MsdFile* fsong)
{
	fsong->ReadFile(strFile);
	return 1;
}

// TODO: Don't have a hard-coded track limit.
enum
{
	TRACK_1 = 0,
	TRACK_2,
	TRACK_3,
	TRACK_4,
	TRACK_5,
	TRACK_6,
	TRACK_7,
	TRACK_8,
	TRACK_9,
	TRACK_10,
	TRACK_11,
	TRACK_12,
	TRACK_13,	// BMS reader needs 13 tracks
	// MD 10/26/03 - BMS reader needs a whole lot more than 13 tracks - more like 16
	//   because we have 11-16, 18, 19, 21-26, 28, 29 for IIDX double (bm-double7)
	TRACK_14,
	TRACK_15,
	TRACK_16,
	// MD 10/26/03 end
	TRACK_17,
	TRACK_18,
	TRACK_19,
	TRACK_20,
	MAX_NOTE_TRACKS		// leave this at the end
};

const int BEATS_PER_MEASURE = 4;
const int ROWS_PER_BEAT	= 48;	// It is important that this number is evenly divisible by 2, 3, and 4.
const int ROWS_PER_MEASURE = ROWS_PER_BEAT * BEATS_PER_MEASURE;

enum NoteType
{
	NOTE_TYPE_4TH,	// quarter note
	NOTE_TYPE_8TH,	// eighth note
	NOTE_TYPE_12TH,	// triplet
	NOTE_TYPE_16TH,	// sixteenth note
	NOTE_TYPE_24TH,	// twenty-fourth note
	NOTE_TYPE_32ND,	// thirty-second note

	// Why is this high of resolution needed?  It's breaking NoteSkins
	// with note-coloring, and the extra resolution will take up more
	// memory.  Does any game actually use this?  -Chris

	// MD 11/02/03 - added finer divisions
	NOTE_TYPE_48TH, // forty-eighth note
	NOTE_TYPE_64TH,	// sixty-fourth note
	// Not having this triggers asserts all over the place.  Go figure.
	NOTE_TYPE_192ND,
	NUM_NOTE_TYPES,
	NOTE_TYPE_INVALID
};

inline int   BeatToNoteRow( float fBeatNum )			{ return int( fBeatNum * ROWS_PER_BEAT + 0.5f); };	// round
inline int   BeatToNoteRowNotRounded( float fBeatNum )	{ return (int)( fBeatNum * ROWS_PER_BEAT ); };
inline float NoteRowToBeat( float fNoteIndex )			{ return fNoteIndex / (float)ROWS_PER_BEAT;	 };
inline float NoteRowToBeat( int iNoteIndex )			{ return NoteRowToBeat( (float)iNoteIndex );	 };

int CMaxDocument::UbicateAndModificateAccordBeatSM(STEP* fdest, float fBeatGo, float fBPMa)
{
	return UbicateAndModificateAccordBeatSM(fdest, fBeatGo, fBPMa, NULL);
}

int CMaxDocument::UbicateAndModificateAccordBeatSM(STEP* fdest, float fBeatGo, float fBPMa, int* nAdded)
{
	int nNumTracks = fdest->hStep.nArrowsPerLine;
	int nArrow = -1;
	bool bFixed = false;
	if (nAdded != NULL) (*nAdded) = 0;
	while(1)
	{
		nArrow++;
		// Look if there is a position in the current file
		// If isn't possible, add a BS change, move down arrows and
		// posicionate this BPM change as closer as posible (acording
		// to tolerances)

		float fBeat = BeatArrowEdit(fdest, nArrow);
		float fDiffa = fBeatGo-fBeat;
		int nDiffMS = int(abs(fDiffa)*60000.0f/fBPMa);
		float fDiff2 = (BeatArrowEdit(fdest, nArrow+1)-fBeatGo);
		int nDiffMS2 = int(fDiff2*60000.0f/fBPMa);
		if(nDiffMS <= UMBRAL_MS_SOB || abs(fDiffa) <= (1/float(MAX_BEATSPLIT)))
		{
			break;
		}
		else if(nDiffMS2 > UMBRAL_MS_SOB && fDiff2 > (1/float(MAX_BEATSPLIT)))
		{
			if(bFixed) break;
			bFixed = true;
			// This means that MUST do the boring part
			float fMinusSobBeat = 1.0f;	// Minor sobrant beat
			float fUmSobBeat = fBPMa*UMBRAL_MS_SOB/60000;
			int nBS = 1;

			// Found back BeatSplit
			int nBSa = fdest->hStep.nBeatSplit;
			int nWBSa = 0;
			int niBSa = -1;
			for(int i = fdest->hStep.nSizeBeatSplit-1; i >= 0; i--)
			{
				if(fdest->nWBeatSplit[i] <= nArrow)
				{
					niBSa = i;
					nWBSa = fdest->nWBeatSplit[i];
					nBSa = fdest->nBeatSplit[i];
					break;
				}
			}

			for(int i = nBSa; i <= MAX_BEATSPLIT; i++)
			{
				float fFrac, fInt;
				fFrac = modf(fBeatGo*float(i), &fInt)/float(i);
				if(fMinusSobBeat > fFrac)
				{
					fMinusSobBeat = fFrac;
					nBS = i;
				}
				if(fMinusSobBeat <= fUmSobBeat && (nBS%nBSa) == 0)
				{
					break;
				}
			}

			int nAdd = nBS/nBSa-1;
			int nLimit = nArrow+nAdd+1;
			fdest->hStepGroup[0].hGroup.nSizeArrows += nAdd;
			ReInit_SKSF2(fdest);
			if (nAdded != NULL) (*nAdded) = nAdd;

			// Look up for the number of lines necesaries to insert
			// The object in the position
			int nLines = 0;
			float fDiffGo = 0.0f;
			while(fDiffGo < fDiffa)
			{
				nLines++;
				fDiffGo = float(nLines)/float(nBS);
			}

			// Displace everything and ADD THE BeatSplits
			for(int y = fdest->hStepGroup[0].hGroup.nSizeArrows-1; y > nLimit; y--)
			{
				for(int x = 0; x < nNumTracks; x++)
				{
					fdest->hStepGroup[0].nArrows[y*nNumTracks+x] =
						fdest->hStepGroup[0].nArrows[(y-nAdd)*nNumTracks+x];
					fdest->hStepGroup[0].nPArrows[y*nNumTracks+x] =
						fdest->hStepGroup[0].nPArrows[(y-nAdd)*nNumTracks+x];
				}
			}

			if((nArrow+1) < fdest->hStepGroup[0].hGroup.nSizeArrows && nLimit < fdest->hStepGroup[0].hGroup.nSizeArrows)
			for(int y = nArrow+1; y < nLimit; y++)
			{
				for(int x = 0; x < nNumTracks; x++)
				{
					fdest->hStepGroup[0].nArrows[y*nNumTracks+x] = 0;
					fdest->hStepGroup[0].nPArrows[y*nNumTracks+x] = 0;
				}
			}

			for(int y = 0; y < fdest->hStep.nSizeChangeBPM; y++)
			{
				if(fdest->nWChangeBPM[y]>nArrow)
				{
					fdest->nWChangeBPM[y] += nAdd;
				}
			}

			for(int y = 0; y < fdest->hStep.nSizeBeatSplit; y++)
			{
				if(fdest->nWBeatSplit[y]>nArrow)
				{
					fdest->nWBeatSplit[y] += nAdd;
				}
			}

			for(int y = 0; y < fdest->hStep.nSizeTickCount; y++)
			{
				if(fdest->nWTickCount[y]>nArrow)
				{
					fdest->nWTickCount[y] += nAdd;
				}
			}

			for(int y = 0; y < fdest->hStep.nSizeStops; y++)
			{
				if(fdest->nIStops[y]>nArrow)
				{
					fdest->nIStops[y] += nAdd;
				}
				if(fdest->nEStops[y]>nArrow)
				{
					fdest->nEStops[y] += nAdd;
				}
			}

			for(int y = 0; y < fdest->hStep.nSizeJumps; y++)
			{
				if(fdest->nIJumps[y]>nArrow)
				{
					fdest->nIJumps[y] += nAdd;
				}
				if(fdest->nEJumps[y]>nArrow)
				{
					fdest->nEJumps[y] += nAdd;
				}
			}

			for(int y = 0; y < fdest->hStep.nSizeChangeVel; y++)
			{
				if(fdest->nIChangeVel[y]>nArrow)
				{
					fdest->nIChangeVel[y] += nAdd;
				}
				if(fdest->nEChangeVel[y]>nArrow)
				{
					fdest->nIChangeVel[y] += nAdd;
				}
			}

			int niBSI = -1;
			for(int i = fdest->hStep.nSizeBeatSplit-1; i >= 0; i--)
			{
				if(fdest->nWBeatSplit[i] == nArrow)
				{
					niBSI = i;
					break;
				}
			}

			int niBSE = -1;
			for(int i = fdest->hStep.nSizeBeatSplit-1; i >= 0; i--)
			{
				if(fdest->nWBeatSplit[i] == nLimit)
				{
					niBSE = i;
					break;
				}
			}

			if(niBSI == -1)
			{
				if(nArrow == 0){
					fdest->hStep.nBeatSplit = nBS;
				}
				else
				{
					niBSI = fdest->hStep.nSizeBeatSplit;
					fdest->hStep.nSizeBeatSplit++;
					ReInit_SKSF2(fdest);
					fdest->nBeatSplit[niBSI] = nBS;
					fdest->nWBeatSplit[niBSI] = nArrow;
				}
			}
			else
			{
				fdest->nBeatSplit[niBSI] = nBS;
				fdest->nWBeatSplit[niBSI] = nArrow;
			}


			if(niBSE == -1)
			{
				niBSE = fdest->hStep.nSizeBeatSplit;
				fdest->hStep.nSizeBeatSplit++;
				ReInit_SKSF2(fdest);
				fdest->nBeatSplit[niBSE] = nBSa;
				fdest->nWBeatSplit[niBSE] = nLimit;
			}


			// Put the position
			nArrow+=nLines;
			// JUST FOR CURIOSITY
			float fObj = BeatArrowEdit(fdest, nArrow);
			if((abs(fObj-fBeatGo)*60/fBPMa) < UMBRAL_MS_SOB)
				_printf("This is ok\n");
			else
				_printf("Wrong\n");

			for(int y = 0; y < fdest->hStep.nSizeBeatSplit; y++)
			{
				for(int x = 0; x < (fdest->hStep.nSizeBeatSplit-1); x++)
				{
					if(fdest->nWBeatSplit[x] > fdest->nWBeatSplit[x+1])
					{
						int nTemp1 = fdest->nWBeatSplit[x];
						int dTemp2 = fdest->nBeatSplit[x];
						fdest->nWBeatSplit[x] = fdest->nWBeatSplit[x+1];
						fdest->nBeatSplit[x] = fdest->nBeatSplit[x+1];
						fdest->nWBeatSplit[x+1] = nTemp1;
						fdest->nBeatSplit[x+1] = dTemp2;
					}
				}
			}

			break;
		}

	}
	return nArrow;
}

int CMaxDocument::UbicateAndModificateAccordStopSM(STEP* fdest, float fBeatGo, float fBeatDur, float fBPMa, int* nLast)
{
	int nFirst = UbicateAndModificateAccordBeatSM(fdest, fBeatGo, fBPMa);

	float dMinusSobBeat = 1.0;	// Minor sobrant beat
	int nBS = 1;

	float dUmSobBeat = fBPMa*UMBRAL_MS_SOB/60000;

	for(int i = 1; i <= MAX_BEATSPLIT; i++)
	{
		float dFrac, dInt;
		dFrac = modf(fBeatDur*float(i), &dInt)/float(i);
		if(dMinusSobBeat > dFrac)
		{
			dMinusSobBeat = dFrac;
			nBS = i;
		}
		if(dMinusSobBeat <= dUmSobBeat) break;
	}
	int nS = int(fBeatDur*float(nBS));

	int nL = nFirst+nS;

	// find previous beatsplit
	int nBSa = fdest->hStep.nBeatSplit;
	int nWBSa = 0;
	int niBSa = -1;
	for(int i = fdest->hStep.nSizeBeatSplit-1; i >= 0; i--)
	{
		if(fdest->nWBeatSplit[i] <= nFirst)
		{
			nBSa = fdest->nBeatSplit[i];
			nWBSa = fdest->nWBeatSplit[i];
			niBSa = i;
			break;
		}
	}

	int nNumTracks = fdest->hStep.nArrowsPerLine;
	int nAdd = nL-nFirst;
	int nArrow = nFirst;
	int nLimit = nArrow+nAdd;
	fdest->hStepGroup[0].hGroup.nSizeArrows += nAdd;
	ReInit_SKSF2(fdest);

	for(int y = fdest->hStepGroup[0].hGroup.nSizeArrows-1; y >= nLimit; y--)
	{
		for(int x = 0; x < nNumTracks; x++)
		{
			fdest->hStepGroup[0].nArrows[y*nNumTracks+x] =
				fdest->hStepGroup[0].nArrows[(y-nAdd)*nNumTracks+x];
			fdest->hStepGroup[0].nPArrows[y*nNumTracks+x] =
				fdest->hStepGroup[0].nPArrows[(y-nAdd)*nNumTracks+x];
		}
	}

	if(nArrow < fdest->hStepGroup[0].hGroup.nSizeArrows && nLimit < fdest->hStepGroup[0].hGroup.nSizeArrows)
	for(int y = nArrow; y < nLimit; y++)
	{
		for(int x = 0; x < nNumTracks; x++)
		{
			fdest->hStepGroup[0].nArrows[y*nNumTracks+x] = 0;
			fdest->hStepGroup[0].nPArrows[y*nNumTracks+x] = 0;
		}
	}

	for(int y = 0; y < fdest->hStep.nSizeChangeBPM; y++)
	{
		if(fdest->nWChangeBPM[y]>nArrow)
		{
			fdest->nWChangeBPM[y] += nAdd;
		}
	}

	for(int y = 0; y < fdest->hStep.nSizeBeatSplit; y++)
	{
		if(fdest->nWBeatSplit[y]>nArrow)
		{
			fdest->nWBeatSplit[y] += nAdd;
		}
	}

	for(int y = 0; y < fdest->hStep.nSizeTickCount; y++)
	{
		if(fdest->nWTickCount[y]>nArrow)
		{
			fdest->nWTickCount[y] += nAdd;
		}
	}

	for(int y = 0; y < fdest->hStep.nSizeStops; y++)
	{
		if(fdest->nIStops[y]>=nArrow)
		{
			fdest->nIStops[y] += nAdd;
		}
		if(fdest->nEStops[y]>nArrow)
		{
			fdest->nEStops[y] += nAdd;
		}
	}

	for(int y = 0; y < fdest->hStep.nSizeJumps; y++)
	{
		if(fdest->nIJumps[y]>=nArrow)
		{
			fdest->nIJumps[y] += nAdd;
		}
		if(fdest->nEJumps[y]>nArrow)
		{
			fdest->nEJumps[y] += nAdd;
		}
	}

	for(int y = 0; y < fdest->hStep.nSizeChangeVel; y++)
	{
		if(fdest->nIChangeVel[y]>nArrow)
		{
			fdest->nIChangeVel[y] += nAdd;
		}
		if(fdest->nEChangeVel[y]>nArrow)
		{
			fdest->nIChangeVel[y] += nAdd;
		}
	}

	// PUT THE BEATSPLITS IF NECESARY
	if(nBSa != nBS)
	{
		int niBSI = -1;
		for(int i = fdest->hStep.nSizeBeatSplit-1; i >= 0; i--)
		{
			if(fdest->nWBeatSplit[i] == nFirst)
			{
				niBSI = i;
				break;
			}
		}

		int niBSE = -1;
		for(int i = fdest->hStep.nSizeBeatSplit-1; i >= 0; i--)
		{
			if(fdest->nWBeatSplit[i] == nL)
			{
				niBSE = i;
				break;
			}
		}

		if(niBSI == -1)
		{
			if(nArrow == 0){
				fdest->hStep.nBeatSplit = nBS;
			}
			else
			{
				niBSI = fdest->hStep.nSizeBeatSplit;
				fdest->hStep.nSizeBeatSplit++;
				ReInit_SKSF2(fdest);
				fdest->nBeatSplit[niBSI] = nBS;
				fdest->nWBeatSplit[niBSI] = nArrow;
			}
		}
		else
		{
			fdest->nBeatSplit[niBSI] = nBS;
			fdest->nWBeatSplit[niBSI] = nArrow;
		}


		if(niBSE == -1)
		{
			niBSE = fdest->hStep.nSizeBeatSplit;
			fdest->hStep.nSizeBeatSplit++;
			ReInit_SKSF2(fdest);
			fdest->nBeatSplit[niBSE] = nBSa;
			fdest->nWBeatSplit[niBSE] = nL;
		}


		/*int nAdd1 = nWBSa==nFirst?1:2;
		int nMod1 = nWBSa==nFirst?niBSa:fdest->hStep.nSizeBeatSplit;
		int nMod2 = nWBSa==nFirst?fdest->hStep.nSizeBeatSplit:(fdest->hStep.nSizeBeatSplit+1);
		fdest->hStep.nSizeBeatSplit+=nAdd1;
		ReInit_SKSF2(fdest);
		if(nMod1 != -1)
		{
			fdest->nWBeatSplit[nMod1] = nFirst;
			fdest->nBeatSplit[nMod1] = nBS;
		}
		else fdest->hStep.nBeatSplit = nBS;
		fdest->nWBeatSplit[nMod2] = nL;
		fdest->nBeatSplit[nMod2] = nBSa;*/
	}

	for(int y = 0; y < fdest->hStep.nSizeBeatSplit; y++)
	{
		for(int x = 0; x < (fdest->hStep.nSizeBeatSplit-1); x++)
		{
			if(fdest->nWBeatSplit[x] > fdest->nWBeatSplit[x+1])
			{
				int nTemp1 = fdest->nWBeatSplit[x];
				int dTemp2 = fdest->nBeatSplit[x];
				fdest->nWBeatSplit[x] = fdest->nWBeatSplit[x+1];
				fdest->nBeatSplit[x] = fdest->nBeatSplit[x+1];
				fdest->nWBeatSplit[x+1] = nTemp1;
				fdest->nBeatSplit[x+1] = dTemp2;
			}
		}
	}

	*nLast = nL;
	return nFirst;
}

int CMaxDocument::Convert_SM_SKSFv2(STEP* fdest, MsdFile* forigin, CString strMode, bool bPlus)
{
	bool bBPM = false;
	bool bStops = false;
	bool bSteps = false;
	bool bTC = false;

	vector<float> m_fWStops;
	vector<float> m_fStops;
	vector<float> m_fWBPM;
	vector<float> m_fBPM;
	vector<float> m_fWTC;
	vector<int> m_nTC;

	int nNumTracks = 10;
	int nMaxBS = 1;

	memset(&fdest->hStep, 0, sizeof(STEPHEADER));
	fdest->hStep.dwVersion = 0x20;

	for( unsigned i=0; i<forigin->GetNumValues(); i++ )
	{
		int iNumParams = forigin->GetNumParams(i);
		const MsdFile::value_t &sParams = forigin->GetValue(i);
		const CString sValueName = sParams[0];

		if( 0==stricmp(sValueName,"BACKGROUND") )
			m_strSplash = sParams[1];

		else if( 0==stricmp(sValueName,"MUSIC") )
			m_strMusic = sParams[1];

		else if( 0==stricmp(sValueName,"BGCHANGES") || 0==stricmp(sValueName,"ANIMATIONS") )
		{
			CStringArray aBGChangeExpressions;
			split( sParams[1], ",", aBGChangeExpressions );

			for( unsigned b=0; b<aBGChangeExpressions.size(); b++ )
			{
				// TODO: BGA
			}
		}

		else if( 0==stricmp(sValueName,"NOTES") )
		{
			if( iNumParams < 7 )
			{
				printf( "The song file is has %d fields in a #NOTES tag, but should have at least %d.", iNumParams, 7 );
				continue;
			}

			// TODO: STEPS
			CString sStepsType = sParams[1];
			CString sDescription = sParams[2];
			CString sDifficulty = sParams[3];
			CString sMeter = sParams[4];
			CString sRadarValues = sParams[5];
			CString sSMNoteData = sParams[6];

			TrimLeft(sStepsType);
			TrimRight(sStepsType);
			TrimLeft(sDescription);
			TrimRight(sDescription);
			TrimLeft(sDifficulty);
			TrimRight(sDifficulty);

			int nDificulty = atoi(sMeter);
			sMeter.Format("%d", nDificulty);

			CString str;
			str += sStepsType + " | " + sDescription + " | "  + sDifficulty + " (" + sMeter + ")";

			if(0==stricmp(str,strMode))
			{
				//
				// Load note data
				//

				fdest->hStep.nSizeArrowGroups = 1;
				fdest->hStep.nArrowsPerLine = 10;
				fdest->hStep.nDificulty = nDificulty;
				fdest->hStep.dwVersion = 0x20;
				fdest->hStep.fVel = 1.0f;
				fdest->hStep.nTickCount = 4;

				CStringArray saValues;
				split( sRadarValues, ",", saValues, true );
				fdest->hStep.nSizeAttrib = saValues.size();

				ReInit_SKSF2(fdest);
				memset(&fdest->hStepGroup[0].hGroup, 0, sizeof(STEPGROUPHEADER));

				for(int x=0; x < fdest->hStep.nSizeAttrib; x++)
				{
					int nRet = sscanf(saValues[x], "%f", &fdest->fAttrib[x]);
					if(nRet != 1) fdest->fAttrib[x] = 0;
				}

				bool bArrowsPerLineDet = false;

				// strip comments out of sSMNoteData
				while( sSMNoteData.Find("//") != -1 )
				{
					int iIndexCommentStart = sSMNoteData.Find("//");
					int iIndexCommentEnd = sSMNoteData.Find("\n", iIndexCommentStart);
					if( iIndexCommentEnd == -1 )	// comment doesn't have an end?
						sSMNoteData.erase( iIndexCommentStart, 2 );
					else
						sSMNoteData.erase( iIndexCommentStart, iIndexCommentEnd-iIndexCommentStart );
				}

				CStringArray asMeasures;
				split( sSMNoteData, ",", asMeasures, true );	// ignore empty is important
				int nBeatSplitBef = 1;
				int nBeatSplitSize = 0;

				fdest->hStep.nBeatSplit = 1;

				for( unsigned m=0; m<asMeasures.size(); m++ )	// foreach measure
				{
					CString &sMeasureString = asMeasures[m];
					TrimLeft(sMeasureString);
					TrimRight(sMeasureString);

					CStringArray asMeasureLines;
					split( sMeasureString, "\n", asMeasureLines, true );	// ignore empty is important

					// DONE: MAKE THE BEATSPLIT
					int nBeatSplit = asMeasureLines.size()/4;
					if(nBeatSplit == 0) nBeatSplit = 1;
					if(nBeatSplitBef != nBeatSplit)
					{
						nBeatSplitSize++;

						if(nBeatSplitSize > 1)
						{
							fdest->hStep.nSizeBeatSplit = nBeatSplitSize-1;
							ReInit_SKSF2(fdest);
							fdest->nWBeatSplit[fdest->hStep.nSizeBeatSplit-1] = fdest->hStepGroup[0].hGroup.nSizeArrows;
							fdest->nBeatSplit[fdest->hStep.nSizeBeatSplit-1] = nBeatSplit;
						}
						else
						{
							fdest->hStep.nBeatSplit = nBeatSplit;
						}
						nBeatSplitBef = nBeatSplit;
					}

					if(nBeatSplitSize == 0) nBeatSplitSize = 1;
					if(nMaxBS < nBeatSplit) nMaxBS = nBeatSplit;

					for( unsigned l=0; l<asMeasureLines.size(); l++ )
					{
						CString &sMeasureLine = asMeasureLines[l];
						TrimLeft(sMeasureLine);
						TrimRight(sMeasureLine);

						const float fPercentIntoMeasure = l/(float)asMeasureLines.size();
						const float fBeat = (m + fPercentIntoMeasure) * BEATS_PER_MEASURE;
						const int iIndex = BeatToNoteRow( fBeat );

						// NOTE: first line determinates the nArrowsPerLine -Ck-dur
						int nOffset = 0;
						if(!bArrowsPerLineDet)
						{
							bArrowsPerLineDet = true;
							nNumTracks = sMeasureLine.GetLength();
							if(nNumTracks == 6 && 0==stricmp(sStepsType, "pump-halfdouble"))
							{
								// If this notes is a halfdouble, make it double with offset
								nNumTracks = 10;
								nOffset = 2;
							}
							fdest->hStep.nArrowsPerLine = nNumTracks;
						}

						int a = fdest->hStepGroup[0].hGroup.nSizeArrows;
						fdest->hStepGroup[0].hGroup.nSizeArrows++;
						ReInit_SKSF2(fdest);

						memset(fdest->hStepGroup[0].nArrows+a*nNumTracks, 0, nNumTracks*sizeof(int));
						memset(fdest->hStepGroup[0].nPArrows+a*nNumTracks, 0, nNumTracks*sizeof(int));

						for( int c=0; c<min(sMeasureLine.GetLength(),nNumTracks); c++ )
						{
							int t;
							char ch = sMeasureLine[c];
							switch( ch )
							{
							case '0': t = 0;	break;	// NONE FOR SKSF2
							case '1': t = 1;	break;	// TAP FOR SKSF2
							case '2': t = 2;	break;	// HOLD TOP FOR SKSF2
							case '3': t = 4;	break;	// HOLD BOTTOM FOR SKSF2
							case 'M':
								t = 5;		break;		// MINES FOR SKSF2
							default:
								t = 0;
								if( ch >= 'a' && ch <= 'z' )
								{
									//t.Set( TapNote::attack, TapNote::original, ch - 'a' );
									// TODO: asdf? WHAT THE HELL IS THIS? ANOTHER ITEMS?

								}
								else
								{
									/* Invalid data.  We don't want to assert, since there might
									 * simply be invalid data in an .SM, and we don't want to die
									 * due to invalid data.  We should probably check for this when
									 * we load SM data for the first time ... */
									// ASSERT(0);
								}
								break;
							}

							// DONE: Set the note
							fdest->hStepGroup[0].nArrows[a*nNumTracks+c+nOffset] = t;
							fdest->hStepGroup[0].nPArrows[a*nNumTracks+c+nOffset] = t==0?0:1;	// DEFAULT PARAMETER
						}
					}
				}

				bSteps = true;
			}
		}

		else if( 0==stricmp(sValueName,"OFFSET"))
		{
			float fOffset = 0;
			CString str = sParams[1];
			TrimLeft(str);
			TrimRight(str);
			int nRet = sscanf(str, "%f", &fOffset);
			if(nRet != 1) fOffset = 0;
			fdest->hStep.nOffset = int(fOffset*1000);
		}

		else if( 0==stricmp(sValueName,"BPMS") )
		{
			CStringArray arrayBPMChangeExpressions;
			split( sParams[1], ",", arrayBPMChangeExpressions );

			for( unsigned b=0; b<arrayBPMChangeExpressions.size(); b++ )
			{
				CStringArray arrayBPMChangeValues;
				split( arrayBPMChangeExpressions[b], "=", arrayBPMChangeValues );
				/* XXX: Once we have a way to display warnings that the user actually
				 * cares about (unlike most warnings), this should be one of them. */
				if(arrayBPMChangeValues.size() != 2)
				{
					printf( "Invalid #%s value \"%s\" (must have exactly one '='), ignored\n",
						sValueName.c_str(), arrayBPMChangeExpressions[b].c_str() );
					continue;
				}

				int nRet;
				float fBeat;
				nRet = sscanf(arrayBPMChangeValues[0], "%f", &fBeat);
				if(nRet != 1)
				{
					fBeat = 0;
					printf( "Cannot read ubication, (%s), ignored\n", arrayBPMChangeValues[0].c_str());
				}

				float fNewBPM;
				nRet = sscanf(arrayBPMChangeValues[1], "%f", &fNewBPM);
				if(nRet != 1) fNewBPM = 0;

				// TODO: BPM
				m_fWBPM.push_back(fBeat);
				m_fBPM.push_back(fNewBPM);
			}

			if(m_fWBPM.size() > 0)
				bBPM = true;
		}

		else if( 0==stricmp(sValueName,"STOPS") || 0==stricmp(sValueName,"FREEZES") )
		{
			CStringArray arrayFreezeExpressions;
			split( sParams[1], ",", arrayFreezeExpressions );

			for( unsigned f=0; f<arrayFreezeExpressions.size(); f++ )
			{
				CStringArray arrayFreezeValues;
				split( arrayFreezeExpressions[f], "=", arrayFreezeValues );
				/* XXX: Once we have a way to display warnings that the user actually
				 * cares about (unlike most warnings), this should be one of them. */
				if( arrayFreezeValues.size() != 2 )
				{
					printf( "Invalid #%s value \"%s\" (must have exactly one '='), ignored\n",
						sValueName.c_str(), arrayFreezeExpressions[f].c_str() );
					continue;
				}

				int nRet;
				float fFreezeBeat;
				nRet = sscanf(arrayFreezeValues[0], "%f", &fFreezeBeat);
				if(nRet != 1)
				{
					printf("Cannot read ubication of stop (%s) \n", arrayFreezeExpressions[f].c_str());
					continue;
				}

				float fFreezeSeconds;
				nRet = sscanf(arrayFreezeValues[1], "%f", &fFreezeSeconds);
				if(nRet != 1) fFreezeSeconds = 0;

				m_fWStops.push_back(fFreezeBeat);
				m_fStops.push_back(fFreezeSeconds);
			}
			bStops = true;
		}
		else if( 0==stricmp(sValueName,"TICKCOUNT"))
		{
			CStringArray arrayTCChangeExpressions;
			split( sParams[1], ",", arrayTCChangeExpressions );

			for( unsigned b=0; b<arrayTCChangeExpressions.size(); b++ )
			{
				CStringArray arrayTCChangeValues;
				split( arrayTCChangeExpressions[b], "=", arrayTCChangeValues );
				/* XXX: Once we have a way to display warnings that the user actually
				 * cares about (unlike most warnings), this should be one of them. */
				if(arrayTCChangeValues.size() != 2)
				{
					printf( "Invalid #%s value \"%s\" (must have exactly one '='), ignored\n",
						sValueName.c_str(), arrayTCChangeExpressions[b].c_str() );
					continue;
				}

				int nRet;
				float fBeat;
				nRet = sscanf(arrayTCChangeValues[0], "%f", &fBeat);
				if(nRet != 1)
				{
					fBeat = 0;
					printf( "Cannot read ubication, (%s), ignored\n", arrayTCChangeValues[0].c_str());
				}

				int nNewTC;
				nRet = sscanf(arrayTCChangeValues[1], "%d", &nNewTC);
				if(nRet != 1) nNewTC = 1;

				// TODO: TC
				m_fWTC.push_back(fBeat);
				m_nTC.push_back(nNewTC);
			}

			if(m_fWTC.size() > 0)
				bTC = true;
		}

	}

	// DONE: translate BPM changes and Stops
	if(bBPM && bSteps)
	{
		fdest->hStep.nSizeChangeBPM = m_fBPM.size()-1;
		if(m_fBPM.size() > 1) ReInit_SKSF2(fdest);
		fdest->hStep.fBPM = m_fBPM[0];

		float fBPMa = fdest->hStep.fBPM;
		for(int j = 0; j < fdest->hStep.nSizeChangeBPM; j++)
		{
			float fBeatGo = m_fWBPM[j+1];
			fdest->nWChangeBPM[j] = UbicateAndModificateAccordBeatSM(fdest, fBeatGo, fBPMa);
			fdest->fChangeBPM[j] = m_fBPM[j+1];
			fBPMa = m_fBPM[j+1];
		}
	}
	else
	{
		fdest->hStep.fBPM = 120.0f;	// DEFAULT BPM, SORRY BUT THERE IS NO BPM CHANGES
	}

	if(bTC && bSteps)
	{
		fdest->hStep.nSizeTickCount = m_nTC.size()-1;
		if(m_nTC.size() > 1) ReInit_SKSF2(fdest);
		fdest->hStep.nTickCount = m_nTC[0];


		for(int j = 0; j < fdest->hStep.nSizeTickCount; j++)
		{
			float fBPMa = 1;
			for(int i = m_fBPM.size()-1; i >= 0; i--)
			{
				if(m_fWBPM[i] <= m_fWTC[j+1])
				{
					fBPMa = m_fBPM[i];
					break;
				}
			}

			float fBeatGo = m_fWTC[j+1];
			fdest->nWTickCount[j] = UbicateAndModificateAccordBeatSM(fdest, fBeatGo, fBPMa);
			fdest->nTickCount[j] = m_nTC[j+1];
		}
	}

	if(bStops && bSteps)
	{
		fdest->hStep.nSizeStops = m_fStops.size();
		if(m_fStops.size() > 0) ReInit_SKSF2(fdest);

		for(int j = 0; j < fdest->hStep.nSizeStops; j++)
		{
			float fBPMa = 1;
			for(int i = m_fBPM.size()-1; i >= 0; i--)
			{
				if(m_fWBPM[i] <= m_fWStops[j])
				{
					fBPMa = m_fBPM[i];
					break;
				}
			}

			float fBeatGo = m_fWStops[j];
			float fBeatDur = m_fStops[j];
			if(!bPlus) fBeatDur*=fBPMa/60;
			// This is in fucking seconds
			// Usually the stop we have to complement
			// At the maximum beatsplit, so the intention
			// will be the same. Maybe doesn't work for some
			// but, it's better than nothing

			// Aproximate by right
			float fBeatDurQuantRight = 0.0f;
			for(int i = 0; (float(i-1)/float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantRight = float(i)/float(nMaxBS);
			}

			// Aproximate by left
			float fBeatDurQuantLeft = 0.0f;
			for(int i = 0; (float(i)/float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantLeft = float(i)/float(nMaxBS);
			}

			if(abs(fBeatDur-fBeatDurQuantLeft) <= abs(fBeatDur-fBeatDurQuantRight))
				fBeatDur = fBeatDurQuantLeft;
			else
				fBeatDur = fBeatDurQuantRight;

			int nLast;
			int nFirst = UbicateAndModificateAccordStopSM(fdest, fBeatGo, fBeatDur, fBPMa, &nLast);

			fdest->nIStops[j] = nFirst;
			fdest->nEStops[j] = nLast;

			// Now, we need to add the duration to all further locations
			for(unsigned u = unsigned(j); u < m_fStops.size(); u++)
			{
				m_fWStops[u] = m_fWStops[u]+fBeatDur;
			}
		}
	}

	// DONE: fill the blank spaces between 2's and 4's with 3's
	if(bSteps)
	{
		for(int x = 0; x < nNumTracks; x++)
		{
			bool bHold = false;
			int p = 0;
			for(int y = 0; y < fdest->hStepGroup[0].hGroup.nSizeArrows; y++)
			{
				if(bHold && fdest->hStepGroup[0].nArrows[y*nNumTracks+x]!=4)
				{
					fdest->hStepGroup[0].nArrows[y*nNumTracks+x] = 3;
					fdest->hStepGroup[0].nPArrows[y*nNumTracks+x] = p;
				}

				if(fdest->hStepGroup[0].nArrows[y*nNumTracks+x] == 2)
				{
					bHold = true;
					p = fdest->hStepGroup[0].nPArrows[y*nNumTracks+x];
				}
				else if(fdest->hStepGroup[0].nArrows[y*nNumTracks+x] == 4)
				{
					bHold = false;
				}

			}
		}
		if(!bTC) fdest->hStep.nTickCount = nMaxBS;
	}
	Fix(fdest);
	return 1;
}

int CMaxDocument::Convert_SMA_SKSFv2(STEP* fdest, MsdFile* forigin, CString strMode)
{
	/*
	* StepMania Aldo MX Converter to SKSF2
	CKDUR 2014
	This code is very similar to SM2SKSF2
	*/
	bool bBPM = false;
	bool bStops = false;
	bool bSteps = false;
	bool bSpeeds = false;
	bool bMult = false;
	bool bTC = false;
	bool bFakes = false;
	bool bJumps = false;

	int nModeStops = 0;

	vector<float> m_fWStops;
	vector<float> m_fStops;

	vector<float> m_fWJumps;
	vector<float> m_fJumps;

	vector<float> m_fWSpeeds;
	vector<float> m_fDSpeeds;
	vector<float> m_fSpeeds;

	vector<float> m_fWFakes;
	vector<float> m_fFakes;

	vector<float> m_fWBPM;
	vector<float> m_fBPM;

	vector<float> m_fWTC;
	vector<int> m_nTC;

	vector<float> m_fWMult;
	vector<int> m_nMult;

	int nNumTracks = 10;
	int nMaxBS = 1;

	memset(&fdest->hStep, 0, sizeof(STEPHEADER));
	fdest->hStep.dwVersion = 0x20;

	for( unsigned i=0; i<forigin->GetNumValues(); i++ )
	{
		int iNumParams = forigin->GetNumParams(i);
		const MsdFile::value_t &sParams = forigin->GetValue(i);
		const CString sValueName = sParams[0];

		if( 0==stricmp(sValueName,"BACKGROUND") )
			m_strSplash = sParams[1];

		else if( 0==stricmp(sValueName,"MUSIC") )
			m_strMusic = sParams[1];

		else if( 0==stricmp(sValueName,"BGCHANGES") || 0==stricmp(sValueName,"ANIMATIONS") )
		{
			CStringArray aBGChangeExpressions;
			split( sParams[1], ",", aBGChangeExpressions );

			for( unsigned b=0; b<aBGChangeExpressions.size(); b++ )
			{
				// TODO: BGA
			}
		}

		else if( 0==stricmp(sValueName,"NOTES") )
		{
			if( iNumParams < 7 )
			{
				printf( "The song file is has %d fields in a #NOTES tag, but should have at least %d.", iNumParams, 7 );
				continue;
			}

			// TODO: STEPS
			CString sStepsType = sParams[1];
			CString sDescription = sParams[2];
			CString sDifficulty = sParams[3];
			CString sMeter = sParams[4];
			CString sRadarValues = sParams[5];
			CString sSMNoteData = sParams[6];

			TrimLeft(sStepsType);
			TrimRight(sStepsType);
			TrimLeft(sDescription);
			TrimRight(sDescription);
			TrimLeft(sDifficulty);
			TrimRight(sDifficulty);

			int nDificulty = atoi(sMeter);
			sMeter.Format("%d", nDificulty);

			CString str;
			str += sStepsType + " | " + sDescription + " | "  + sDifficulty + " (" + sMeter + ")";

			if(0==stricmp(str,strMode))
			{
				//
				// Load note data
				//

				fdest->hStep.nSizeArrowGroups = 1;
				fdest->hStep.nArrowsPerLine = 10;
				fdest->hStep.nDificulty = nDificulty;
				fdest->hStep.dwVersion = 0x20;
				fdest->hStep.fVel = 1.0f;
				fdest->hStep.nTickCount = 4;

				CStringArray saValues;
				split( sRadarValues, ",", saValues, true );
				fdest->hStep.nSizeAttrib = saValues.size();

				ReInit_SKSF2(fdest);
				memset(&fdest->hStepGroup[0].hGroup, 0, sizeof(STEPGROUPHEADER));

				for(int x=0; x < fdest->hStep.nSizeAttrib; x++)
				{
					int nRet = sscanf(saValues[x], "%f", &fdest->fAttrib[x]);
					if(nRet != 1) fdest->fAttrib[x] = 0;
				}

				bool bArrowsPerLineDet = false;

				// strip comments out of sSMNoteData
				while( sSMNoteData.Find("//") != -1 )
				{
					int iIndexCommentStart = sSMNoteData.Find("//");
					int iIndexCommentEnd = sSMNoteData.Find("\n", iIndexCommentStart);
					if( iIndexCommentEnd == -1 )	// comment doesn't have an end?
						sSMNoteData.erase( iIndexCommentStart, 2 );
					else
						sSMNoteData.erase( iIndexCommentStart, iIndexCommentEnd-iIndexCommentStart );
				}

				CStringArray asMeasures;
				split( sSMNoteData, ",", asMeasures, true );	// ignore empty is important
				int nBeatSplitBef = 1;
				int nBeatSplitSize = 0;

				fdest->hStep.nBeatSplit = 1;

				for( unsigned m=0; m<asMeasures.size(); m++ )	// foreach measure
				{
					CString &sMeasureString = asMeasures[m];
					TrimLeft(sMeasureString);
					TrimRight(sMeasureString);

					CStringArray asMeasureLines;
					split( sMeasureString, "\n", asMeasureLines, true );	// ignore empty is important

					// DONE: MAKE THE BEATSPLIT
					int nBeatSplit = asMeasureLines.size()/4;
					if(nBeatSplit == 0) nBeatSplit = 1;
					if(nBeatSplitBef != nBeatSplit)
					{
						nBeatSplitSize++;

						if(nBeatSplitSize > 1)
						{
							fdest->hStep.nSizeBeatSplit = nBeatSplitSize-1;
							ReInit_SKSF2(fdest);
							fdest->nWBeatSplit[fdest->hStep.nSizeBeatSplit-1] = fdest->hStepGroup[0].hGroup.nSizeArrows;
							fdest->nBeatSplit[fdest->hStep.nSizeBeatSplit-1] = nBeatSplit;
						}
						else
						{
							fdest->hStep.nBeatSplit = nBeatSplit;
						}
						nBeatSplitBef = nBeatSplit;
					}

					if(nBeatSplitSize == 0) nBeatSplitSize = 1;
					if(nMaxBS < nBeatSplit) nMaxBS = nBeatSplit;

					for( unsigned l=0; l<asMeasureLines.size(); l++ )
					{
						CString &sMeasureLine = asMeasureLines[l];
						TrimLeft(sMeasureLine);
						TrimRight(sMeasureLine);

						const float fPercentIntoMeasure = l/(float)asMeasureLines.size();
						const float fBeat = (m + fPercentIntoMeasure) * BEATS_PER_MEASURE;
						const int iIndex = BeatToNoteRow( fBeat );

						// NOTE: first line determinates the nArrowsPerLine -Ck-dur
						int nOffset = 0;
						if(!bArrowsPerLineDet)
						{
							bArrowsPerLineDet = true;
							nNumTracks = sMeasureLine.GetLength();
							if(nNumTracks == 6 && 0==stricmp(sStepsType, "pump-halfdouble"))
							{
								// If this notes is a halfdouble, make it double with offset
								nNumTracks = 10;
								nOffset = 2;
							}
							fdest->hStep.nArrowsPerLine = nNumTracks;
						}

						int a = fdest->hStepGroup[0].hGroup.nSizeArrows;
						fdest->hStepGroup[0].hGroup.nSizeArrows++;
						ReInit_SKSF2(fdest);

						memset(fdest->hStepGroup[0].nArrows+a*nNumTracks, 0, nNumTracks*sizeof(int));
						memset(fdest->hStepGroup[0].nPArrows+a*nNumTracks, 0, nNumTracks*sizeof(int));

						for( int c=0; c<min(sMeasureLine.GetLength(),nNumTracks); c++ )
						{
							int t;
							int p=1;

							// Find multiplier of steps
							if(bMult)
							{
								for(unsigned w = 0; w < m_fWMult.size(); w++)
								{
									if(m_fWMult[w] <= fBeat)
									{
										p=m_nMult[w];
									}
								}
							}

							if(bFakes)
							{
								for(unsigned w = 0; w < m_fWFakes.size(); w++)
								{
									if(m_fWFakes[w] <= fBeat && fBeat <= (m_fWFakes[w]+m_fFakes[w]))
									{
										p=0;
										break;
									}
								}
							}

							char ch = sMeasureLine[c];
							switch( ch )
							{
							case '0': t = 0;	break;	// NONE FOR SKSF2
							case '1': t = 1;	break;	// TAP FOR SKSF2
							case '2': t = 2;	break;	// HOLD TOP FOR SKSF2
							case '3': t = 4;	break;	// HOLD BOTTOM FOR SKSF2
							case 'M':
								t = 5;		break;		// MINES FOR SKSF2
							default:
								t = 0;
								if( ch >= 'a' && ch <= 'z' )
								{
									//t.Set( TapNote::attack, TapNote::original, ch - 'a' );
									// TODO: asdf? WHAT THE HELL IS THIS? ANOTHER ITEMS?

								}
								else
								{
									/* Invalid data.  We don't want to assert, since there might
									 * simply be invalid data in an .SM, and we don't want to die
									 * due to invalid data.  We should probably check for this when
									 * we load SM data for the first time ... */
									// ASSERT(0);
								}
								break;
							}

							// DONE: Set the note
							fdest->hStepGroup[0].nArrows[a*nNumTracks+c+nOffset] = t;
							fdest->hStepGroup[0].nPArrows[a*nNumTracks+c+nOffset] = t==0?0:p;
						}
					}
				}

				bSteps = true;
				// NOTE: in Stepmania Aldo MX we already found a NOTES tag
				// So we don't need to convert any further
				break;
			}
		}

		else if( 0==stricmp(sValueName,"OFFSET"))
		{
			float fOffset = 0;
			CString str = sParams[1];
			TrimLeft(str);
			TrimRight(str);
			int nRet = sscanf(str, "%f", &fOffset);
			if(nRet != 1) fOffset = 0;
			fdest->hStep.nOffset = int(fOffset*1000);
		}

		else if( 0==stricmp(sValueName,"BPMS") )
		{
			bBPM = false;
			m_fWBPM.clear();
			m_fBPM.clear();
			CStringArray arrayBPMChangeExpressions;
			split( sParams[1], ",", arrayBPMChangeExpressions );

			for( unsigned b=0; b<arrayBPMChangeExpressions.size(); b++ )
			{
				CStringArray arrayBPMChangeValues;
				split( arrayBPMChangeExpressions[b], "=", arrayBPMChangeValues );
				/* XXX: Once we have a way to display warnings that the user actually
				 * cares about (unlike most warnings), this should be one of them. */
				if(arrayBPMChangeValues.size() != 2)
				{
					printf( "Invalid #%s value \"%s\" (must have exactly one '='), ignored\n",
						sValueName.c_str(), arrayBPMChangeExpressions[b].c_str() );
					continue;
				}

				int nRet;
				float fBeat;
				nRet = sscanf(arrayBPMChangeValues[0], "%f", &fBeat);
				if(nRet != 1)
				{
					fBeat = 0;
					printf( "Cannot read ubication, (%s), ignored\n", arrayBPMChangeValues[0].c_str());
				}

				float fNewBPM;
				nRet = sscanf(arrayBPMChangeValues[1], "%f", &fNewBPM);
				if(nRet != 1) fNewBPM = 0;

				// TODO: BPM
				m_fWBPM.push_back(fBeat);
				m_fBPM.push_back(fNewBPM);
			}

			if(m_fWBPM.size() > 0)
				bBPM = true;
		}

		else if( 0==stricmp(sValueName,"STOPS") || 0==stricmp(sValueName,"DELAYS") )
		{
			if(0==stricmp(sValueName,"STOPS"))
			{
				bStops = false;
				m_fWStops.clear();
				m_fStops.clear();
				bJumps = false;
				m_fWJumps.clear();
				m_fJumps.clear();
			}

			CStringArray arrayFreezeExpressions;
			split( sParams[1], ",", arrayFreezeExpressions );

			for( unsigned f=0; f<arrayFreezeExpressions.size(); f++ )
			{
				CStringArray arrayFreezeValues;
				split( arrayFreezeExpressions[f], "=", arrayFreezeValues );
				/* XXX: Once we have a way to display warnings that the user actually
				 * cares about (unlike most warnings), this should be one of them. */
				if( arrayFreezeValues.size() != 2 )
				{
					printf( "Invalid #%s value \"%s\" (must have exactly one '='), ignored\n",
						sValueName.c_str(), arrayFreezeExpressions[f].c_str() );
					continue;
				}

				int nRet;
				float fFreezeBeat;
				nRet = sscanf(arrayFreezeValues[0], "%f", &fFreezeBeat);
				if(nRet != 1)
				{
					printf("Cannot read ubication of stop (%s) \n", arrayFreezeExpressions[f].c_str());
					continue;
				}

				float fFreezeSeconds;
				nRet = sscanf(arrayFreezeValues[1], "%f", &fFreezeSeconds);
				if(nRet != 1) fFreezeSeconds = 0;

				if(fFreezeSeconds > 0)
				{
					m_fWStops.push_back(fFreezeBeat);
					m_fStops.push_back(fFreezeSeconds);
				}
				/*else if(fFreezeSeconds < 0)
				{
					m_fWJumps.push_back(fFreezeBeat);
					m_fJumps.push_back(fFreezeSeconds);
				}*/
			}
			if(m_fWStops.size() > 0)
				bStops = true;
			if(m_fWJumps.size() > 0)
				bJumps = true;
		}

		else if( 0==stricmp(sValueName,"SPEED"))
		{
			bSpeeds = false;
			m_fWSpeeds.clear();
			m_fDSpeeds.clear();
			m_fSpeeds.clear();
			CStringArray arraySpeedExpressions;
			split( sParams[1], ",", arraySpeedExpressions );

			for( unsigned f=0; f<arraySpeedExpressions.size(); f++ )
			{
				CStringArray arraySpeedValues;
				split( arraySpeedExpressions[f], "=", arraySpeedValues );
				/* XXX: Once we have a way to display warnings that the user actually
				 * cares about (unlike most warnings), this should be one of them. */
				if( arraySpeedValues.size() < 2 )
				{
					printf( "Invalid #%s value \"%s\" (must have one or two '='), ignored\n",
						sValueName.c_str(), arraySpeedExpressions[f].c_str() );
					continue;
				}

				int nRet;
				float fSpeedBeat;
				nRet = sscanf(arraySpeedValues[0], "%f", &fSpeedBeat);
				if(nRet != 1)
				{
					printf("Cannot read ubication of stop (%s) \n", arraySpeedExpressions[f].c_str());
					continue;
				}

				float fSpeedValue;
				nRet = sscanf(arraySpeedValues[1], "%f", &fSpeedValue);
				if(nRet != 1) fSpeedValue = 0;

				float fSpeedSeconds = 0;
				if( arraySpeedValues.size() > 2 )
				{
					nRet = sscanf(arraySpeedValues[2], "%f", &fSpeedSeconds);
					if(nRet != 1) fSpeedSeconds = 0;
				}

				m_fWSpeeds.push_back(fSpeedBeat);
				m_fDSpeeds.push_back(fSpeedSeconds);
				m_fSpeeds.push_back(fSpeedValue);
			}
			if(m_fWSpeeds.size() > 0)
				bSpeeds = true;
		}

		else if( 0==stricmp(sValueName,"FAKES"))
		{
			bFakes = false;
			m_fWFakes.clear();
			m_fFakes.clear();
			CStringArray arrayFakesExpressions;
			split( sParams[1], ",", arrayFakesExpressions );

			for( unsigned f=0; f<arrayFakesExpressions.size(); f++ )
			{
				CStringArray arrayFakesValues;
				split( arrayFakesExpressions[f], "=", arrayFakesValues );
				/* XXX: Once we have a way to display warnings that the user actually
				 * cares about (unlike most warnings), this should be one of them. */
				if( arrayFakesValues.size() != 2 )
				{
					printf( "Invalid #%s value \"%s\" (must have exactly one '='), ignored\n",
						sValueName.c_str(), arrayFakesExpressions[f].c_str() );
					continue;
				}

				int nRet;
				float fFakesBeat;
				nRet = sscanf(arrayFakesValues[0], "%f", &fFakesBeat);
				if(nRet != 1)
				{
					printf("Cannot read ubication of stop (%s) \n", arrayFakesExpressions[f].c_str());
					continue;
				}

				float fFakesSeconds;
				nRet = sscanf(arrayFakesValues[1], "%f", &fFakesSeconds);
				if(nRet != 1) fFakesSeconds = 0;

				m_fWFakes.push_back(fFakesBeat);
				m_fFakes.push_back(fFakesSeconds);
			}
			if(m_fWFakes.size() > 0)
				bFakes = true;
		}

		else if( 0==stricmp(sValueName,"TICKCOUNT"))
		{
			bTC = false;
			m_fWTC.clear();
			m_nTC.clear();
			CStringArray arrayTCChangeExpressions;
			split( sParams[1], ",", arrayTCChangeExpressions );

			for( unsigned b=0; b<arrayTCChangeExpressions.size(); b++ )
			{
				CStringArray arrayTCChangeValues;
				split( arrayTCChangeExpressions[b], "=", arrayTCChangeValues );
				/* XXX: Once we have a way to display warnings that the user actually
				 * cares about (unlike most warnings), this should be one of them. */
				if(arrayTCChangeValues.size() != 2)
				{
					printf( "Invalid #%s value \"%s\" (must have exactly one '='), ignored\n",
						sValueName.c_str(), arrayTCChangeExpressions[b].c_str() );
					continue;
				}

				int nRet;
				float fBeat;
				nRet = sscanf(arrayTCChangeValues[0], "%f", &fBeat);
				if(nRet != 1)
				{
					fBeat = 0;
					printf( "Cannot read ubication, (%s), ignored\n", arrayTCChangeValues[0].c_str());
				}

				int nNewTC;
				nRet = sscanf(arrayTCChangeValues[1], "%d", &nNewTC);
				if(nRet != 1) nNewTC = 1;

				// TODO: TC
				m_fWTC.push_back(fBeat);
				m_nTC.push_back(nNewTC);
			}

			if(m_fWTC.size() > 0)
				bTC = true;
		}

		else if( 0==stricmp(sValueName,"MULTIPLIER"))
		{
			bMult = false;
			m_fWMult.clear();
			m_nMult.clear();
			CStringArray arrayMultChangeExpressions;
			split( sParams[1], ",", arrayMultChangeExpressions );

			for( unsigned b=0; b<arrayMultChangeExpressions.size(); b++ )
			{
				CStringArray arrayMultChangeValues;
				split( arrayMultChangeExpressions[b], "=", arrayMultChangeValues );
				/* XXX: Once we have a way to display warnings that the user actually
				 * cares about (unlike most warnings), this should be one of them. */
				if(arrayMultChangeValues.size() != 2)
				{
					printf( "Invalid #%s value \"%s\" (must have exactly one '='), ignored\n",
						sValueName.c_str(), arrayMultChangeExpressions[b].c_str() );
					continue;
				}

				int nRet;
				float fBeat;
				nRet = sscanf(arrayMultChangeValues[0], "%f", &fBeat);
				if(nRet != 1)
				{
					fBeat = 0;
					printf( "Cannot read ubication, (%s), ignored\n", arrayMultChangeValues[0].c_str());
				}

				int nNewMult;
				nRet = sscanf(arrayMultChangeValues[1], "%d", &nNewMult);
				if(nRet != 1) nNewMult = 1;

				// TODO: Mult
				m_fWMult.push_back(fBeat);
				m_nMult.push_back(nNewMult);
			}

			if(m_fWMult.size() > 0)
				bMult = true;
		}

	}

	if(bBPM && bSteps)
	{
		// WARNING: THIS MUST BE DIRTY
		// We must find the jumps, in order to do that
		// We filter the negative BPM and the next BPM
		// We don't consider any absolute equal or something
		// The duration of Jump is the double (in beats) of the
		// Difference of the locations of this BPMs
		bJumps = false;
		bBPM = false;
		unsigned niBPM = 0;
		while(1)
		{
			if(niBPM >= m_fBPM.size()) break;

			if(m_fBPM[niBPM] <= 0.0)	// WE ALSO CONSIDER NEGATIVE BPM AS 0
			{
				float fJumpBeat = m_fWBPM[niBPM];
				float fJumpDur = (m_fWBPM[niBPM+1]-m_fWBPM[niBPM])*2;
				m_fBPM.erase(m_fBPM.begin()+niBPM);
				m_fBPM.erase(m_fBPM.begin()+niBPM);
				m_fWBPM.erase(m_fWBPM.begin()+niBPM);
				m_fWBPM.erase(m_fWBPM.begin()+niBPM);

				m_fWJumps.push_back(fJumpBeat);
				m_fJumps.push_back(fJumpDur);
				continue;
			}

			niBPM++;
		}

		if(m_fWJumps.size() > 0)
			bJumps = true;

		if(m_fBPM.size() > 0)
			bBPM = true;
	}

	// DOING: translate All mods
	if(bBPM && bSteps)
	{
		fdest->hStep.nSizeChangeBPM = m_fBPM.size()-1;
		if(m_fBPM.size() > 1) ReInit_SKSF2(fdest);
		fdest->hStep.fBPM = m_fBPM[0];

		float fBPMa = fdest->hStep.fBPM;
		for(int j = 0; j < fdest->hStep.nSizeChangeBPM; j++)
		{
			float fBeatGo = m_fWBPM[j+1];
			fdest->nWChangeBPM[j] = UbicateAndModificateAccordBeatSM(fdest, fBeatGo, fBPMa);
			fdest->fChangeBPM[j] = m_fBPM[j+1];
			fBPMa = m_fBPM[j+1];
		}
	}
	else
	{
		fdest->hStep.fBPM = 120.0f;	// DEFAULT BPM, SORRY BUT THERE IS NO BPM CHANGES
	}

	if(bSpeeds && bSteps)
	{
		fdest->hStep.nSizeChangeVel = m_fSpeeds.size()-1;
		if(m_fSpeeds.size() > 1) ReInit_SKSF2(fdest);
		fdest->hStep.fVel = m_fSpeeds[0];

		for(int j = 0; j < fdest->hStep.nSizeChangeVel; j++)
		{
			float fBPMa = 1;
			for(int i = m_fBPM.size()-1; i >= 0; i--)
			{
				if(m_fWBPM[i] <= m_fWSpeeds[j+1])
				{
					fBPMa = m_fBPM[i];
					break;
				}
			}

			float fBeatGo = m_fWSpeeds[j+1];
			fdest->nIChangeVel[j] = UbicateAndModificateAccordBeatSM(fdest, fBeatGo, fBPMa);

			for(int i = m_fBPM.size()-1; i >= 0; i--)
			{
				if(m_fWBPM[i] <= m_fWSpeeds[j+1])
				{
					fBPMa = m_fBPM[i];
					break;
				}
			}

			fBeatGo = m_fWSpeeds[j+1]+m_fDSpeeds[j+1];
			fdest->nEChangeVel[j] = UbicateAndModificateAccordBeatSM(fdest, fBeatGo, fBPMa);

			fdest->fChangeVel[j] = m_fSpeeds[j+1];
			fdest->fIChangeVel[j] = m_fSpeeds[j];
		}
	}
	else
	{
		fdest->hStep.fVel = 1.0f;	// Default velocity
	}

	if(bJumps && bSteps)
	{
		fdest->hStep.nSizeJumps = m_fJumps.size();
		if(m_fJumps.size() > 0) ReInit_SKSF2(fdest);

		for(int j = 0; j < fdest->hStep.nSizeJumps; j++)
		{
			float fBPMa = 1;
			for(int i = m_fBPM.size()-1; i >= 0; i--)
			{
				if(m_fWBPM[i] <= m_fWJumps[j])
				{
					fBPMa = m_fBPM[i];
					break;
				}
			}

			float fBeatGo = m_fWJumps[j];
			fdest->nIJumps[j] = UbicateAndModificateAccordBeatSM(fdest, fBeatGo, fBPMa);

			for(int i = m_fBPM.size()-1; i >= 0; i--)
			{
				if(m_fWBPM[i] <= m_fWJumps[j])
				{
					fBPMa = m_fBPM[i];
					break;
				}
			}

			fBeatGo = m_fWJumps[j]+m_fJumps[j];
			fdest->nEJumps[j] = UbicateAndModificateAccordBeatSM(fdest, fBeatGo, fBPMa);
		}
	}

	if(bTC && bSteps)
	{
		fdest->hStep.nSizeTickCount = m_nTC.size()-1;
		if(m_nTC.size() > 1) ReInit_SKSF2(fdest);
		fdest->hStep.nTickCount = m_nTC[0];


		for(int j = 0; j < fdest->hStep.nSizeTickCount; j++)
		{
			float fBPMa = 1;
			for(int i = m_fBPM.size()-1; i >= 0; i--)
			{
				if(m_fWBPM[i] <= m_fWTC[j+1])
				{
					fBPMa = m_fBPM[i];
					break;
				}
			}

			float fBeatGo = m_fWTC[j+1];
			fdest->nWTickCount[j] = UbicateAndModificateAccordBeatSM(fdest, fBeatGo, fBPMa);
			fdest->nTickCount[j] = m_nTC[j+1];
		}
	}

	if(bStops && bSteps)
	{
		fdest->hStep.nSizeStops = m_fStops.size();
		if(m_fStops.size() > 0) ReInit_SKSF2(fdest);

		for(int j = 0; j < fdest->hStep.nSizeStops; j++)
		{
			float fBPMa = 1;
			for(int i = m_fBPM.size()-1; i >= 0; i--)
			{
				if(m_fWBPM[i] <= m_fWStops[j])
				{
					fBPMa = m_fBPM[i];
					break;
				}
			}

			float fBeatGo = m_fWStops[j];
			float fBeatDur = m_fStops[j]*fBPMa/60;

			// This is in fucking seconds
			// Usually the stop we have to complement
			// At the maximum beatsplit, so the intention
			// will be the same. Maybe doesn't work for some
			// but, it's better than nothing

			// Aproximate by right
			float fBeatDurQuantRight = 0.0f;
			for(int i = 0; (float(i-1)/float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantRight = float(i)/float(nMaxBS);
			}

			// Aproximate by left
			float fBeatDurQuantLeft = 0.0f;
			for(int i = 0; (float(i)/float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantLeft = float(i)/float(nMaxBS);
			}

			if(abs(fBeatDur-fBeatDurQuantLeft) <= abs(fBeatDur-fBeatDurQuantRight))
				fBeatDur = fBeatDurQuantLeft;
			else
				fBeatDur = fBeatDurQuantRight;

			int nLast;
			int nFirst = UbicateAndModificateAccordStopSM(fdest, fBeatGo, fBeatDur, fBPMa, &nLast);

			fdest->nIStops[j] = nFirst;
			fdest->nEStops[j] = nLast;

			// Now, we need to add the duration to all further locations
			for(unsigned u = unsigned(j); u < m_fStops.size(); u++)
			{
				m_fWStops[u] = m_fWStops[u]+fBeatDur;
			}

			// We need to do this also for BPMs, for compatibility
			for(int i = 0; i < m_fBPM.size(); i++)
			{
				if(m_fWBPM[i] > m_fWStops[j])
				{
					m_fWBPM[i] = m_fWBPM[i]+fBeatDur;
				}
			}
		}
	}

	// DONE: fill the blank spaces between 2's and 4's with 3's
	if(bSteps)
	{
		for(int x = 0; x < nNumTracks; x++)
		{
			bool bHold = false;
			int p = 0;
			for(int y = 0; y < fdest->hStepGroup[0].hGroup.nSizeArrows; y++)
			{
				if(bHold && fdest->hStepGroup[0].nArrows[y*nNumTracks+x]!=4)
				{
					fdest->hStepGroup[0].nArrows[y*nNumTracks+x] = 3;
					fdest->hStepGroup[0].nPArrows[y*nNumTracks+x] = p;
				}

				if(fdest->hStepGroup[0].nArrows[y*nNumTracks+x] == 2)
				{
					bHold = true;
					p = fdest->hStepGroup[0].nPArrows[y*nNumTracks+x];
				}
				else if(fdest->hStepGroup[0].nArrows[y*nNumTracks+x] == 4)
				{
					bHold = false;
				}

			}
		}
		if(!bTC) fdest->hStep.nTickCount = nMaxBS;
	}

	// HACK: ReInit with zero beatsplit
	//fdest->hStep.nSizeBeatSplit = 0;
	//fdest->hStep.nSizeTickCount = 0;
	//fdest->hStep.nSizeChangeBPM = 0;
	//fdest->hStep.nSizeChangeVel = 0;
	//fdest->hStep.nSizeJumps = 0;
	//fdest->hStep.nSizeStops = 0;
	//ReInit_SKSF2(fdest);
	Fix(fdest);
	return 1;
}

int CMaxDocument::Convert_KSF_SKSFv2(STEP* fdest, MsdFile* forigin)
{// ksf 2 v2
	memset(&fdest->hStep, 0, sizeof(STEPHEADER));
	fdest->hStep.dwVersion = 0x20;

	int iTickCount = -1;	// this is the value we read for TICKCOUNT
	CStringArray asRows;

	float BPMPos2 = -1, BPM2 = -1, BPMPos3 = -1, BPM3 = -1;;

	for( unsigned i=0; i<forigin->GetNumValues(); i++ )
	{
		const MsdFile::value_t &sParams = forigin->GetValue(i);
		CString sValueName = sParams[0];

		// handle the data
		if( 0==stricmp(sValueName,"TICKCOUNT") )
			iTickCount = atoi(sParams[1]);

		else if( 0==stricmp(sValueName,"STEP") )
		{
			CString step = sParams[1];
			TrimLeft(step);
			split( step, "\n", asRows, true );
		}
		else if( 0==stricmp(sValueName,"DIFFICULTY") )
			fdest->hStep.nDificulty = atoi(sParams[1]);
		else if( 0==stricmp(sValueName,"BPM") )
		{
			float fBPM = 120.0f;
			int nRet = sscanf(sParams[1], "%f", &fBPM);
			if(nRet != 1) fBPM = 120.0f;
			fdest->hStep.fBPM = fBPM;
		}
		else if( 0==stricmp(sValueName,"BPM2") )
		{
			float fBPM = 120.0f;
			int nRet = sscanf(sParams[1], "%f", &fBPM);
			if(nRet != 1) fBPM = 120.0f;
			BPM2 = fBPM;
		}
		else if( 0==stricmp(sValueName,"BPM3") )
		{
			float fBPM = 120.0f;
			int nRet = sscanf(sParams[1], "%f", &fBPM);
			if(nRet != 1) fBPM = 120.0f;
			BPM3 = fBPM;
		}
		else if( 0==stricmp(sValueName,"BUNKI") )
		{
			float fBPM = 0;
			int nRet = sscanf(sParams[1], "%f", &fBPM);
			if(nRet != 1) fBPM = 0;
			BPMPos2 = fBPM;
		}
		else if( 0==stricmp(sValueName,"BUNKI2") )
		{
			float fBPM = 0;
			int nRet = sscanf(sParams[1], "%f", &fBPM);
			if(nRet != 1) fBPM = 0;
			BPMPos3 = fBPM;
		}
		else if( 0==stricmp(sValueName,"STARTTIME") )
		{
			float fDelay = 0;
			int nRet = sscanf(sParams[1], "%f", &fDelay);
			if(nRet != 1) fDelay = 0;
			fdest->hStep.nOffset = -int(fDelay*10);
		}
	}

	if( iTickCount == -1 )
	{
		iTickCount = 2;
		printf( "TICKCOUNT not found; defaulting to %i", iTickCount );
	}
	fdest->hStep.nBeatSplit = iTickCount;
	fdest->hStep.nTickCount = iTickCount;
	fdest->hStep.fVel = 1.0f;
	fdest->hStep.nSizeArrowGroups = 1;
	Init_SKSF2(fdest);
	memset(&fdest->hStepGroup[0].hGroup, 0, sizeof(STEPGROUPHEADER));

	{
		CString sDir, sFName, sExt;
		splitpath( m_strFile, sDir, sFName, sExt );
		sFName.MakeLower();

		if( sFName.Find("crazy")!=-1 )
		{
			//out.SetDifficulty(DIFFICULTY_HARD);
			if(fdest->hStep.nDificulty == 0) fdest->hStep.nDificulty = (8);
		}
		else if( sFName.Find("hard")!=-1 )
		{
			//out.SetDifficulty(DIFFICULTY_MEDIUM);
			if(fdest->hStep.nDificulty == 0) fdest->hStep.nDificulty = (5);
		}
		else if( sFName.Find("easy")!=-1 )
		{
			//out.SetDifficulty(DIFFICULTY_EASY);
			if(fdest->hStep.nDificulty == 0) fdest->hStep.nDificulty = (2);
		}
		else
		{
			//out.SetDifficulty(DIFFICULTY_MEDIUM);
			if(fdest->hStep.nDificulty == 0) fdest->hStep.nDificulty = (5);
		}

		int nOffset = 0;
		fdest->hStep.nArrowsPerLine = ( 5 );

		/* Check for "halfdouble" before "double". */
		if( sFName.Find("halfdouble") != -1 || sFName.Find("h_double") != -1 )
		{
			fdest->hStep.nArrowsPerLine = ( 10 );
		}
		else if( sFName.Find("double") != -1 )
		{
			fdest->hStep.nArrowsPerLine = ( 10 );
		}
		else if( sFName.Find("_2") != -1 )
		{
			fdest->hStep.nArrowsPerLine = ( 10 );
		}
	}

	int iHoldStartRow[13];
	int t;
	for( t=0; t<13; t++ )
		iHoldStartRow[t] = -1;

	int y = 0;
	for( unsigned r=0; r<asRows.size(); r++ )
	{
		CString& sRowString = asRows[r];
		StripCrnl( sRowString );

		if( sRowString == "" )
			continue;	// skip

		if(sRowString.Find('|') != -1)
		{
			float fBPM = 0;
			int nRet = sscanf(sRowString, "|%f|", &fBPM);
			if(nRet != 1) continue;
			fdest->hStep.nSizeChangeBPM++;
			ReInit_SKSF2(fdest);
			fdest->fChangeBPM[fdest->hStep.nSizeChangeBPM-1] = fBPM;
			fdest->nWChangeBPM[fdest->hStep.nSizeChangeBPM-1] = fdest->hStepGroup[0].hGroup.nSizeArrows;
			continue;
		}

		/* All 2s indicates the end of the song. */
		if( sRowString == "2222222222222" )
			break;

		if(sRowString.size() != 13)
		{
			printf("File %s had a RowString with an improper length (\"%s\"); corrupt notes ignored",
				m_strFile.c_str(), sRowString.c_str());
			return 0;
		}

		/* Half-doubles is offset; "0011111100000". */

		// the length of a note in a row depends on TICKCOUNT
		int nArrowsPerLine = fdest->hStep.nArrowsPerLine;
		int a = fdest->hStepGroup[0].hGroup.nSizeArrows;
		fdest->hStepGroup[0].hGroup.nSizeArrows++;
		ReInit_SKSF2(fdest);
		memset(fdest->hStepGroup[0].nArrows+a*nArrowsPerLine, 0, nArrowsPerLine*sizeof(int));
		memset(fdest->hStepGroup[0].nPArrows+a*nArrowsPerLine, 0, nArrowsPerLine*sizeof(int));
		for( int t=0; t < nArrowsPerLine; t++ )
		{
			int tap;
			int p = 1;
			switch(sRowString[t])
			{
			case '0':	tap = 0;	p = 0;		break;
			case '1':	tap = 1;	p = 1;		break;
			case '4':	tap = 3;	p = 1;		break;
			default:	tap = 0;	p = 0;		break;
			}

			fdest->hStepGroup[0].nArrows[a*nArrowsPerLine+t] = tap;
			fdest->hStepGroup[0].nPArrows[a*nArrowsPerLine+t] = p;
		}
	}

	/* This doesn't work yet: we also need to move the data around, I think, and
	 * we should handle more than one BPM change. */
	if( BPM2 > 0 && BPMPos2 > 0 )
	{
		const float BeatsPerSecond = fdest->hStep.fBPM / 60.0f;
		const float beat = BPMPos2 * BeatsPerSecond;
		int nPos = UbicateAndModificateAccordBeatSM(fdest, beat, fdest->hStep.fBPM);
		fdest->hStep.nSizeChangeBPM++;
		ReInit_SKSF2(fdest);
		fdest->fChangeBPM[fdest->hStep.nSizeChangeBPM-1] = BPM2;
		fdest->nWChangeBPM[fdest->hStep.nSizeChangeBPM-1] = nPos;
	}

	if( BPM3 > 0 && BPMPos3 > 0 )
	{
		const float BeatsPerSecond = fdest->hStep.fBPM / 60.0f;
		const float beat = BPMPos3 * BeatsPerSecond;
		int nPos = UbicateAndModificateAccordBeatSM(fdest, beat, fdest->hStep.fBPM);
		fdest->hStep.nSizeChangeBPM++;
		ReInit_SKSF2(fdest);
		fdest->fChangeBPM[fdest->hStep.nSizeChangeBPM-1] = BPM3;
		fdest->nWChangeBPM[fdest->hStep.nSizeChangeBPM-1] = nPos;
	}

	int nNumTracks = fdest->hStep.nArrowsPerLine;
	for(int x = 0; x < fdest->hStep.nArrowsPerLine; x++)
	{
		for(int y = 0; y < fdest->hStepGroup[0].hGroup.nSizeArrows; y++)
		{
			if(fdest->hStepGroup[0].nArrows[y*nNumTracks+x] == 3)
			{
				if(y == (fdest->hStepGroup[0].hGroup.nSizeArrows-1))
				{
					fdest->hStepGroup[0].nArrows[y*nNumTracks+x] = 4;
				}
				else if(fdest->hStepGroup[0].nArrows[(y+1)*nNumTracks+x] != 3)
				{
					fdest->hStepGroup[0].nArrows[y*nNumTracks+x] = 4;
				}

				if(y == 0)
				{
					fdest->hStepGroup[0].nArrows[y*nNumTracks+x] = 2;
				}
				else if(fdest->hStepGroup[0].nArrows[(y-1)*nNumTracks+x] != 3)
				{
					fdest->hStepGroup[0].nArrows[y*nNumTracks+x] = 2;
				}

			}

		}
	}

	Fix(fdest);
	return 1;
}

int CMaxDocument::Open_UCS_SKSF2(LPCTSTR strFile, STEP* fsong)
{
	ifstream f;

	f.open(strFile, ios::in | ios::binary);

	if( !f.is_open())
	{
		return 0;
	}

	CString FileString;

	streampos begin, end;
	begin = f.tellg();
	f.seekg(0, ifstream::end);
	end = f.tellg();
	f.seekg(0, ifstream::beg);


	int size = int(end-begin);
	FileString.reserve( size );

	int iBytesRead = -1;
	char buf[4096];
	int ret = 0;
	while(1)
	{
		int ToRead = min(sizeof(buf), size-ret);

		int nret = int(f.tellg());
		f.read( buf, ToRead );
		int got = int(f.tellg())-nret;
		if( got == 0 )
			break;
		if( got < 0 )
		{
			iBytesRead = -1;
			break;
		}

		FileString.append( buf, got );

		ret += got;
		iBytesRead = ret;
	}

	f.close();

	// WOW ok is readed, so then we need to do our shit

	// PRELIMINARS: Assign some templates
	memset(&fsong->hStep, 0, sizeof(STEPHEADER));
	fsong->hStep.nArrowsPerLine = 10;
	fsong->hStep.dwVersion = 0x20;
	fsong->hStep.nMode = 1;
	fsong->hStep.nSizeArrowGroups = 1;
	fsong->hStep.fVel = 1;
	Init_SKSF2(fsong);
	memset(&fsong->hStepGroup[0].hGroup, 0, sizeof(STEPGROUPHEADER));

	// FIRST: Divide the file in lines
	int nArrowsPerLine = fsong->hStep.nArrowsPerLine;
	CStringArray asLines;
	split( FileString, "\n", asLines, true );

	vector<int> m_nWStops;
	vector<int> m_nStops;

	vector<int> m_nWJumps;
	vector<int> m_nJumps;

	vector<int> m_nWBPM;
	vector<float> m_fBPM;

	vector<int> m_nWBeatSplits;
	vector<int> m_nBeatSplits;

	int nMaxBS = 1;
	bool bDelay = false;

	fsong->hStepGroup[0].hGroup.nSizeArrows = 0;
	// SECOND: Navegate the file
	int nBeatsPerMeasure = 4;
	for( unsigned r=0; r<asLines.size(); r++ )
	{
		CString sValueName = asLines[r];
		TrimLeft(sValueName);
		TrimRight(sValueName);

		if(sValueName[0] == ':')
		{
			// OK, THERE IS AN ARGUMENT
			CStringArray asExpressions;
			split( sValueName, "=", asExpressions);

			if( asExpressions.size() != 2 )
			{
				printf("UCS Error, there are more or less than one '='\n");
				continue;
			}

			CString strValue = asExpressions[1];
			TrimLeft(strValue);
			TrimRight(strValue);

			if(0==stricmp(asExpressions[0], ":Format"))
			{
				// Ingored
				continue;
			}
			else if(0==stricmp(asExpressions[0], ":Mode"))
			{
				if(0==stricmp(strValue, "Single"))
				{
					nArrowsPerLine = 5;
				}
				else if(0==stricmp(strValue, "Double"))
				{
					nArrowsPerLine = 10;
				}
				else if(0==stricmp(strValue, "S-Performance"))
				{
					nArrowsPerLine = 5;
				}
				else if(0==stricmp(strValue, "D-Performance"))
				{
					nArrowsPerLine = 10;
				}
				else
				{
					printf("UCS error: Mode unknown\n");
				}
				fsong->hStep.nArrowsPerLine = nArrowsPerLine;
			}
			else if(0==stricmp(asExpressions[0], ":BPM"))
			{
				float fBPM;
				int nRet = sscanf(strValue, "%f", &fBPM);
				if(nRet != 1) continue;
				m_nWBPM.push_back(song.hStepGroup[0].hGroup.nSizeArrows);
				m_fBPM.push_back(fBPM);
			}
			else if(0==stricmp(asExpressions[0], ":Delay"))
			{
				int nDelay;
				int nRet = sscanf(strValue, "%d", &nDelay);
				if(nRet != 1) continue;

				if(!bDelay)
				{
					fsong->hStep.nOffset = -nDelay;
					bDelay = true;
				}
				else
				{
					// OK! THIS IS A STOP OR A JUMP
					// Stop if delay is positive
					bDelay = true;
					if(nDelay > 0)
					{
						m_nWStops.push_back(song.hStepGroup[0].hGroup.nSizeArrows);
						m_nStops.push_back(abs(nDelay));
					}

					// Jump if delay is negative
					else if(nDelay < 0)
					{
						m_nWJumps.push_back(song.hStepGroup[0].hGroup.nSizeArrows);
						m_nJumps.push_back(abs(nDelay));
					}


					// No delay in current, simply skip
					else continue;
				}
			}
			else if(0==stricmp(asExpressions[0], ":Beat"))
			{
				int nBeat;
				int nRet = sscanf(strValue, "%d", &nBeat);
				if(nRet != 1) continue;
				if(nBeat != 4)
				{
					printf("UCS to SKSF2 warning: BeatsPerMeasure cannot be a value different by 4\n");
					continue;
				}
			}
			else if(0==stricmp(asExpressions[0], ":Split"))
			{
				int nSplit;
				int nRet = sscanf(strValue, "%d", &nSplit);
				if(nRet != 1) continue;
				m_nWBeatSplits.push_back(song.hStepGroup[0].hGroup.nSizeArrows);
				m_nBeatSplits.push_back(nSplit);
				if(nMaxBS < nSplit) nMaxBS = nSplit;
			}
		}
		else
		{
			int a = song.hStepGroup[0].hGroup.nSizeArrows;
			song.hStepGroup[0].hGroup.nSizeArrows++;
			ReInit_SKSF2(fsong);
			for( int c=0; c<min(sValueName.GetLength(),nArrowsPerLine); c++ )
			{
				int t;
				char ch = sValueName[c];
				switch( ch )
				{
				case '.': t = 0;	break;	// NONE FOR SKSF2
				case 'X': t = 1;	break;	// TAP FOR SKSF2
				case 'M': t = 2;	break;	// HOLD TOP FOR SKSF2
				case 'H': t = 3;	break;	// HOLD MID FOR SKSF2
				case 'W': t = 4;	break;	// HOLD BOTTOM FOR SKSF2
				default:
					t = 0;
					break;
				}
				fsong->hStepGroup[0].nArrows[a*nArrowsPerLine+c] = t;
				fsong->hStepGroup[0].nPArrows[a*nArrowsPerLine+c] = t==0?0:1;
			}
		}
	}

	if(m_fBPM.size() > 0)
	{
		fsong->hStep.nSizeChangeBPM = m_fBPM.size()-1;
		if(m_fBPM.size() > 1) ReInit_SKSF2(fsong);
		fsong->hStep.fBPM = m_fBPM[0];

		for(int j = 0; j < fsong->hStep.nSizeChangeBPM; j++)
		{
			fsong->nWChangeBPM[j] = m_nWBPM[j+1];
			fsong->fChangeBPM[j] = m_fBPM[j+1];
		}
	}
	else
	{
		fsong->hStep.fBPM = 120.0f;	// DEFAULT BPM, SORRY BUT THERE IS NO BPM CHANGES
	}

	// Copy and replicate BS and TC
	if(m_nBeatSplits.size() > 0)
	{
		fsong->hStep.nSizeBeatSplit = m_nBeatSplits.size()-1;
		if(m_nBeatSplits.size() > 1) ReInit_SKSF2(fsong);
		fsong->hStep.nBeatSplit = m_nBeatSplits[0];

		for(int j = 0; j < fsong->hStep.nSizeBeatSplit; j++)
		{
			fsong->nWBeatSplit[j] = m_nWBeatSplits[j+1];
			fsong->nBeatSplit[j] = m_nBeatSplits[j+1];
		}
	}
	else
	{
		fsong->hStep.nBeatSplit = 2;
	}

	if(m_nBeatSplits.size() > 0)
	{
		fsong->hStep.nSizeTickCount = m_nBeatSplits.size()-1;
		if(m_nBeatSplits.size() > 1) ReInit_SKSF2(fsong);
		fsong->hStep.nTickCount = m_nBeatSplits[0];

		for(int j = 0; j < fsong->hStep.nSizeTickCount; j++)
		{
			fsong->nWTickCount[j] = m_nWBeatSplits[j+1];
			fsong->nTickCount[j] = m_nBeatSplits[j+1];
		}
	}
	else
	{
		fsong->hStep.nTickCount = 2;
	}

	if(m_nStops.size() > 0)
	{
		fsong->hStep.nSizeStops = m_nStops.size();
		ReInit_SKSF2(fsong);
		memset(fsong->nIStops, 0, fsong->hStep.nSizeStops*sizeof(int));
		memset(fsong->nEStops, 0, fsong->hStep.nSizeStops*sizeof(int));

		for(int j = 0; j < fsong->hStep.nSizeStops; j++)
		{
			float fBPMa = 1;
			for(int i = m_fBPM.size()-1; i >= 0; i--)
			{
				if(m_nWBPM[i] <= m_nWStops[j])
				{
					fBPMa = m_fBPM[i];
					break;
				}
			}

			float fBeatGo = BeatArrowEdit(fsong, m_nWStops[j]);

			float fBeatDur = float(m_nStops[j])*fBPMa/60000;

			// This is in fucking seconds
			// Usually the stop we have to complement
			// At the maximum beatsplit, so the intention
			// will be the same. Maybe doesn't work for some
			// but, it's better than nothing

			// Aproximate by right
			float fBeatDurQuantRight = 0.0f;
			for(int i = 0; (float(i-1)/float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantRight = float(i)/float(nMaxBS);
			}

			// Aproximate by left
			float fBeatDurQuantLeft = 0.0f;
			for(int i = 0; (float(i)/float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantLeft = float(i)/float(nMaxBS);
			}

			if(abs(fBeatDur-fBeatDurQuantLeft) <= abs(fBeatDur-fBeatDurQuantRight))
				fBeatDur = fBeatDurQuantLeft;
			else
				fBeatDur = fBeatDurQuantRight;

			int nLast;
			int nFirst = UbicateAndModificateAccordStopSM(fsong, fBeatGo, fBeatDur, fBPMa, &nLast);

			if(nFirst != m_nWStops[j]) printf("What the hell? Stop Corrupt\n");
			fsong->nIStops[j] = m_nWStops[j];
			fsong->nEStops[j] = nLast;

			// Now, we need to add the duration to all further locations
			for(unsigned u = unsigned(j); u < m_nStops.size(); u++)
			{
				m_nWStops[u] = m_nWStops[u]+nLast-nFirst;
			}

			for (unsigned i = 0; i < m_nJumps.size(); i++)
			{
				if (m_nWJumps[i] > m_nWStops[j])
				{
					m_nWJumps[i] = m_nWJumps[i] + nLast - nFirst;
				}
			}

			for(unsigned i = 0; i < m_fBPM.size(); i++)
			{
				if(m_nWBPM[i] > m_nWStops[j])
				{
					m_nWBPM[i] = m_nWBPM[i]+nLast-nFirst;
				}
			}
		}
	}

	if(m_nJumps.size() > 0)
	{
		fsong->hStep.nSizeJumps = m_nJumps.size();
		ReInit_SKSF2(fsong);
		memset(fsong->nIJumps, 0, fsong->hStep.nSizeStops*sizeof(int));
		memset(fsong->nEJumps, 0, fsong->hStep.nSizeStops*sizeof(int));

		for(int j = 0; j < fsong->hStep.nSizeJumps; j++)
		{
			float fBPMa = 1;
			for(int i = m_fBPM.size()-1; i >= 0; i--)
			{
				if(m_nWBPM[i] <= m_nWJumps[j])
				{
					fBPMa = m_fBPM[i];
					break;
				}
			}

			float fBeatGo = BeatArrowEdit(fsong, m_nWJumps[j]);

			float fBeatDur = float(m_nJumps[j])*fBPMa/60000;

			// This is in fucking seconds
			// Usually the stop we have to complement
			// At the maximum beatsplit, so the intention
			// will be the same. Maybe doesn't work for some
			// but, it's better than nothing

			// Aproximate by right
			float fBeatDurQuantRight = 0.0f;
			for(int i = 0; (float(i-1)/float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantRight = float(i)/float(nMaxBS);
			}

			// Aproximate by left
			float fBeatDurQuantLeft = 0.0f;
			for(int i = 0; (float(i)/float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantLeft = float(i)/float(nMaxBS);
			}

			if(abs(fBeatDur-fBeatDurQuantLeft) <= abs(fBeatDur-fBeatDurQuantRight))
				fBeatDur = fBeatDurQuantLeft;
			else
				fBeatDur = fBeatDurQuantRight;

			int nLast;
			int nFirst = UbicateAndModificateAccordStopSM(fsong, fBeatGo, fBeatDur, fBPMa, &nLast);

			if(nFirst != m_nWJumps[j]) printf("What the hell? Jump Corrupt\n");
			fsong->nIJumps[j] = m_nWJumps[j];
			fsong->nEJumps[j] = nLast;

			// Now, we need to add the duration to all further locations
			for(unsigned u = unsigned(j); u < m_nJumps.size(); u++)
			{
				m_nWJumps[u] = m_nWJumps[u]+nLast-nFirst;
			}

			for(unsigned i = 0; i < m_fBPM.size(); i++)
			{
				if(m_nWBPM[i] > m_nWJumps[j])
				{
					m_nWBPM[i] = m_nWBPM[i]+nLast-nFirst;
				}
			}
		}
	}

	Fix(fsong);
	return 1;
}

int CMaxDocument::Save_UCS_SKSF2(LPCTSTR strFile, STEP* fsong)
{
	return 1;
}

int CMaxDocument::OpenRAW_NX10(LPCTSTR strFile, CNX10Step* fsong) // ** Open NX10 using Step.h/Step.cpp NX2SRC
{
	if (!fsong->Load(strFile)) return 0;
	return 1;
}

int CMaxDocument::Convert_NX10_SKSFv2(STEP* fdest, CNX10Step* forigin) // NX10 to SKSFv2
{
	memset(&fdest->hStep, 0, sizeof(STEPHEADER));

	// Assign some direct data
	fdest->hStep.nArrowsPerLine = forigin->m_pStep->nNote;
	fdest->hStep.nSizeAttrib = 5;
	fdest->hStep.dwVersion = 0x20;
	fdest->hStep.nDificulty = 1;	// TODO: We need a way to extract this
	fdest->hStep.nMode = forigin->m_pStep->nNote == 10 ? 1 : 0; // If this is a LM, well, for us is a Single

	fdest->hStep.nSizeArrowGroups = 1; // At least 1 exists

	// Some buffer data to take in count after
	vector<int> m_nWStops;
	vector<float> m_fStops;

	vector<int> m_nWJumps;
	vector<float> m_fJumps;

	vector<int> m_nWVel;
	vector<float> m_fDVel;
	vector<float> m_fVel;

	// Initialization of SKSF2
	Init_SKSF2(fdest);

	using namespace Step;

	/*
	So, this is how an NX10 is structured:
	Inside the file, there are blocks
	Inside of each block, there are divisions
	Inside each division, there are lines
	Lines has the StepData
	*/

	// Because this format uses a seccionate-format behavioral, we must accumulate
	// the lines what we're adding. This is an accumulator since the last size
	// of arrows used.
	int nAccumLines = 0;

	// Index of Line where the insertion starts
	int nCurStart = 0;

	// Maximum BeatSplit
	int nMaxBS = 1;

	// Current BPM
	float fCurBPM = 1;

	// Extract number of blocks, explore them
	int nBlock = forigin->m_pStep->nBlock;
	for (int b = 0; b < nBlock; b++)
	{
		// Same as above, but for the maximum lines.
		int nMaxToAccumLines = 0;
		Block* block = forigin->GetBlock(b);

		// Extract number of divisions
		int nDiv = block->nDiv;

		// Ohh, we have more than one division? We must have an update in ArrowGroups
		// Why? Because Each division of them is an ArrowGroup for us
		if (nDiv > fdest->hStep.nSizeArrowGroups)
		{
			fdest->hStep.nSizeArrowGroups = nDiv;
			ReInit_SKSF2(fdest);
		}
		// Explore divisions
		for (int d = 0; d < nDiv; d++)
		{
			Div* div = forigin->GetDiv(b, d);

			// Before of exploring lines, there is vital information on divisions
			// IMPORTANT: We read this in original information, no conversion needed
			// TAG (step.cpp:78) : Next lines only converts BPM into msPerLine
			// and Delay in ms into Delay in Beats <-- this conversion only happens
			// if div->Speed is positive or 0

			if (d == 0 && b == 0)
			{
				// Starting the first block, the first division has our general offset
				fdest->hStep.nOffset = (int)-div->_beatDelay; // This is on ms
				fdest->hStep.fBPM = div->msPerLine; // This is on BPM
				fdest->hStep.fVel = div->Speed;
				fdest->hStep.nBeatSplit = div->nBeatSplit;
				fdest->hStep.nTickCount = div->nBeatSplit; // IS THE SAME

				nMaxBS = MAX_BEATSPLIT;
				fCurBPM = div->msPerLine;

				// MISSING INFO: Maybe is relevant
				//div->bmFlags;		// This is smoothSpeed
				//div->_beatsPerLine;	// This is mysterybox
				//div->_pad1;			// Absolutely nothing

				// TODO: 
				// freeze          =   (speed<0);
			}

			if (fdest->hStep.fBPM <= 0.0f) fdest->hStep.fBPM = div->msPerLine;
			if (fdest->hStep.nBeatSplit <= 0) fdest->hStep.nBeatSplit = div->nBeatSplit;

			int diff = 0;
			if (b != 0)
			{
				Div* prevdiv = forigin->GetDiv(b-1, 0);
				if (prevdiv->msPerLine > 0.0f) fCurBPM = prevdiv->msPerLine;
				float intCurStart = 0.0;
				float curStart = ((div->msStart - prevdiv->msStart)*(float)prevdiv->nBeatSplit*fCurBPM / 60000.0f);

				float decCurStart = modf(curStart, &intCurStart);

				// This is an aproximation function
				nCurStart += decCurStart <= 0.5 ? (int)(intCurStart) : (int)(intCurStart+1);
				diff = nCurStart - nAccumLines;

				if (d == 0)
				{
					// Here I retrieve the Changes
					prevdiv = forigin->GetDiv(b-1, 0);

					// BPM Changes can be added directly
					if (prevdiv->msPerLine != div->msPerLine && div->msPerLine > 0)
					{
						fdest->hStep.nSizeChangeBPM++;
						int q = fdest->hStep.nSizeChangeBPM - 1;
						ReInit_SKSF2(fdest);
						fdest->fChangeBPM[q] = div->msPerLine;
						fdest->nWChangeBPM[q] = nCurStart;
					}

					// BeatSplit Changes can be added directly (TickCount too)
					if (prevdiv->nBeatSplit != div->nBeatSplit)
					{
						fdest->hStep.nSizeBeatSplit++;
						fdest->hStep.nSizeTickCount++;
						ReInit_SKSF2(fdest);
						int q = fdest->hStep.nSizeBeatSplit - 1;
						fdest->nBeatSplit[q] = div->nBeatSplit;
						fdest->nWBeatSplit[q] = nCurStart;
						q = fdest->hStep.nSizeTickCount - 1;
						fdest->nTickCount[q] = div->nBeatSplit;
						fdest->nWTickCount[q] = nCurStart;
					}

					if (nMaxBS < div->nBeatSplit) nMaxBS = div->nBeatSplit;

					// Stops & Jumps must be added after
					if (abs(div->_beatDelay) > UMBRAL_MS_SOB)
					{
						if (div->msPerLine <= 0.0f || div->_beatDelay < 0.0f)
						{
							m_fJumps.push_back(abs(div->_beatDelay));
							m_nWJumps.push_back(nCurStart);
						}
						else
						{
							m_fStops.push_back(abs(div->_beatDelay));
							m_nWStops.push_back(nCurStart);
						}
					}

					// Velocity Changes can be added afterwards
					if (div->Speed < 0.0) div->Speed *= -1.0f;
					if (prevdiv->Speed != div->Speed)
					{
						float nBeats = div->_beatsPerMeasure;
						m_fVel.push_back(div->Speed);
						if ((b + 1) < nBlock)
						{
							Div* postdiv = forigin->GetDiv(b + 1, 0);
							float nBeat = (postdiv->msStart - div->msStart)*div->msPerLine/60000.0f;
							if (nBeat < nBeats) nBeats = nBeat;
						}
						m_fDVel.push_back(nBeats);
						m_nWVel.push_back(nCurStart);
					}
				}
			}

			int nLine = div->nLine;
			if ((nLine+diff) > nMaxToAccumLines) nMaxToAccumLines = nLine+diff;

			// We'll Acomodate all this lines into the format
			fdest->hStepGroup[d].hGroup.nSizeArrows = nAccumLines + nMaxToAccumLines;
			ReInit_SKSF2(fdest);

			for (int l = 0; l < nLine; l++)
			{
				// SET TO ZERO ALL LINE
				memset(fdest->hStepGroup[d].nArrows + (nCurStart + l)*fdest->hStep.nArrowsPerLine, 0, fdest->hStep.nArrowsPerLine*sizeof(int));
				memset(fdest->hStepGroup[d].nPArrows + (nCurStart + l)*fdest->hStep.nArrowsPerLine, 0, fdest->hStep.nArrowsPerLine*sizeof(int));
			}

			// Explore the lines
			for (int l = 0; l < nLine; l++)
			{
				// There is only a "line" if the offset is different to zero
				int nIndex = div->ofsLine[l];
				if (nIndex == 0) continue;
				Note* pnote = forigin->GetLinePtr(nIndex);

				for (int x = 0; x < forigin->m_pStep->nNote; x++)
				{
					Note* note = pnote + x;
					// We have now the note, but there are complications because
					// we use ordinal numbers to identificate the type of note
					// they use Attr and Note, and Masking bits
					// For now, we'll only identificate the basic types

					// Note::Attr & TypeMask
					/*const int Item = 1;	// 1111 0001	// [v]
					const int Special = 2;	// 1111 0010
					const int Normal = 3;	// 1011 0011	// [v]
					const int LongStart = 4;	// 1011 0100
					const int LongMiddle = 6;	// 1011 0110
					const int LongEnd = 7;	// 1011 0111*/
					int* nArrow = fdest->hStepGroup[d].nArrows + (nCurStart + l)*fdest->hStep.nArrowsPerLine + x;
					int* nPArrow = fdest->hStepGroup[d].nPArrows + (nCurStart + l)*fdest->hStep.nArrowsPerLine + x;

					switch (note->Attr & TypeMask)
					{
					case Item:
						break;
					case Special:
						break;
					case Normal:
						(*nArrow) = 1;
						(*nPArrow) = 1;
						break;
					case LongStart:
						(*nArrow) = 2;
						(*nPArrow) = 1;
						break;
					case LongMiddle:
						(*nArrow) = 3;
						(*nPArrow) = 1;
						break;
					case LongEnd:
						(*nArrow) = 4;
						(*nPArrow) = 1;
						break;
					}
				}
			}
		}
		nAccumLines += nMaxToAccumLines;
	}

	// In the end, all of this matter

	// Seriously, we must add the Vel, Stops and Jumps

	// First Stops! Because we love them
	if (m_fStops.size() > 0)
	{
		fdest->hStep.nSizeStops = m_fStops.size();
		ReInit_SKSF2(fdest);
		memset(fdest->nIStops, 0, fdest->hStep.nSizeStops*sizeof(int));
		memset(fdest->nEStops, 0, fdest->hStep.nSizeStops*sizeof(int));

		for (int j = 0; j < fdest->hStep.nSizeStops; j++)
		{
			float fBPMa = 1;
			for (int i = fdest->hStep.nSizeChangeBPM - 1; i >= 0; i--)
			{
				if (fdest->nWChangeBPM[i] <= m_nWStops[j])
				{
					fBPMa = fdest->fChangeBPM[i];
					break;
				}
			}

			float fBeatGo = BeatArrowEdit(fdest, m_nWStops[j]);

			float fBeatDur = float(m_fStops[j])*fBPMa / 60000;

			// This is in fucking seconds
			// Usually the stop we have to complement
			// At the maximum beatsplit, so the intention
			// will be the same. Maybe doesn't work for some
			// but, it's better than nothing

			// Aproximate by right
			float fBeatDurQuantRight = 0.0f;
			for (int i = 0; (float(i - 1) / float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantRight = float(i) / float(nMaxBS);
			}

			// Aproximate by left
			float fBeatDurQuantLeft = 0.0f;
			for (int i = 0; (float(i) / float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantLeft = float(i) / float(nMaxBS);
			}

			if (abs(fBeatDur - fBeatDurQuantLeft) <= abs(fBeatDur - fBeatDurQuantRight))
				fBeatDur = fBeatDurQuantLeft;
			else
				fBeatDur = fBeatDurQuantRight;

			int nLast;
			int nFirst = UbicateAndModificateAccordBeatSM(fdest, fBeatGo-fBeatDur, fBPMa, &nLast);

			fdest->nIStops[j] = nFirst;
			fdest->nEStops[j] = m_nWStops[j];

			// Now, we need to add the duration to all further locations
			for (unsigned u = unsigned(j); u < m_fStops.size(); u++)
			{
				m_nWStops[u] = m_nWStops[u] + nLast;
			}

			for (unsigned i = 0; i < m_fJumps.size(); i++)
			{
				if (m_nWJumps[i] > m_nWStops[j])
				{
					m_nWJumps[i] = m_nWJumps[i] + nLast;
				}
			}

			for (unsigned i = 0; i < m_fVel.size(); i++)
			{
				if (m_nWVel[i] > m_nWStops[j])
				{
					m_nWVel[i] = m_nWVel[i] + nLast;
				}
			}
		}
	}

	// The Speeds (AKA Vel) we can add them
	if (m_fVel.size() > 0)
	{
		fdest->hStep.nSizeChangeVel = m_fVel.size();
		ReInit_SKSF2(fdest);
		memset(fdest->nIChangeVel, 0, fdest->hStep.nSizeChangeVel*sizeof(int));
		memset(fdest->nEChangeVel, 0, fdest->hStep.nSizeChangeVel*sizeof(int));
		memset(fdest->fChangeVel, 0, fdest->hStep.nSizeChangeVel*sizeof(float));
		memset(fdest->fIChangeVel, 0, fdest->hStep.nSizeChangeVel*sizeof(float));

		for (int j = 0; j < fdest->hStep.nSizeChangeVel; j++)
		{
			float fBPMa = 1;
			for (int i = fdest->hStep.nSizeChangeBPM - 1; i >= 0; i--)
			{
				if (fdest->nWChangeBPM[i] <= m_nWVel[j])
				{
					fBPMa = fdest->fChangeBPM[i];
					break;
				}
			}

			float fBeatGo = BeatArrowEdit(fdest, m_nWVel[j]);

			float fBeatDur = m_fDVel[j];

			// Aproximate by right
			float fBeatDurQuantRight = 0.0f;
			for (int i = 0; (float(i - 1) / float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantRight = float(i) / float(nMaxBS);
			}

			// Aproximate by left
			float fBeatDurQuantLeft = 0.0f;
			for (int i = 0; (float(i) / float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantLeft = float(i) / float(nMaxBS);
			}

			if (abs(fBeatDur - fBeatDurQuantLeft) <= abs(fBeatDur - fBeatDurQuantRight))
				fBeatDur = fBeatDurQuantLeft;
			else
				fBeatDur = fBeatDurQuantRight;

			int nLast;
			int nFirst = UbicateAndModificateAccordBeatSM(fdest, fBeatGo+fBeatDur, fBPMa, &nLast);

			if (nFirst != m_nWVel[j]) printf("What the hell? Speed Corrupt\n");
			fdest->nIChangeVel[j] = m_nWVel[j];
			fdest->nEChangeVel[j] = nFirst;
			fdest->fIChangeVel[j] = j == 0 ? fdest->hStep.fVel : fdest->fChangeVel[j-1];
			fdest->fChangeVel[j] = m_fVel[j];

			// Now, we need to add the duration to all further locations
			for (unsigned u = unsigned(j); u < m_fVel.size(); u++)
			{
				m_nWVel[u] = m_nWVel[u] + nLast;
			}

			for (unsigned i = 0; i < m_fJumps.size(); i++)
			{
				if (m_nWJumps[i] > m_nWVel[j])
				{
					m_nWJumps[i] = m_nWJumps[i] + nLast;
				}
			}
		}
	}

	// Finally, the jumps! yay...
	if (m_fJumps.size() > 0)
	{
		fdest->hStep.nSizeJumps = m_fJumps.size();
		ReInit_SKSF2(fdest);
		memset(fdest->nIJumps, 0, fdest->hStep.nSizeJumps*sizeof(int));
		memset(fdest->nEJumps, 0, fdest->hStep.nSizeJumps*sizeof(int));

		for (int j = 0; j < fdest->hStep.nSizeJumps; j++)
		{
			float fBPMa = 1;
			for (int i = fdest->hStep.nSizeChangeBPM - 1; i >= 0; i--)
			{
				if (fdest->nWChangeBPM[i] <= m_nWJumps[j])
				{
					fBPMa = fdest->fChangeBPM[i];
					break;
				}
			}

			float fBeatGo = BeatArrowEdit(fdest, m_nWJumps[j]);

			float fBeatDur = float(m_fJumps[j])*fBPMa / 60000;

			// This is in fucking seconds
			// Usually the stop we have to complement
			// At the maximum beatsplit, so the intention
			// will be the same. Maybe doesn't work for some
			// but, it's better than nothing

			// Aproximate by right
			float fBeatDurQuantRight = 0.0f;
			for (int i = 0; (float(i - 1) / float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantRight = float(i) / float(nMaxBS);
			}

			// Aproximate by left
			float fBeatDurQuantLeft = 0.0f;
			for (int i = 0; (float(i) / float(nMaxBS)) < fBeatDur; i++)
			{
				fBeatDurQuantLeft = float(i) / float(nMaxBS);
			}

			if (abs(fBeatDur - fBeatDurQuantLeft) <= abs(fBeatDur - fBeatDurQuantRight))
				fBeatDur = fBeatDurQuantLeft;
			else
				fBeatDur = fBeatDurQuantRight;

			int nLast;
			int nFirst = UbicateAndModificateAccordStopSM(fdest, fBeatGo, fBeatDur, fBPMa, &nLast);

			if (nFirst != m_nWJumps[j]) printf("What the hell? Jump Corrupt\n");
			fdest->nIJumps[j] = m_nWJumps[j];
			fdest->nEJumps[j] = nLast;

			// Now, we need to add the duration to all further locations
			for (unsigned u = unsigned(j); u < m_fJumps.size(); u++)
			{
				m_nWJumps[u] = m_nWJumps[u] + nLast - nFirst;
			}
		}
	}

	return 1;
}
