#include "MaxStepSmu.h"

int CMaxStepSmu::CompileBS(void)
{
	// Compilador de Brain shower
	if(pDoc->song.hStep.nSizeArrowGroups <= 0) return 1;
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	for(int y = 0; y < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; y++)
	{
		if(pDoc->SomeBSQuestion(y, 0))
		{
			// Extraer la información
			bsc.nSizeBSQuestions++;
			bsc.dWIBSQuestions = (float*)max_realloc(bsc.dWIBSQuestions, bsc.nSizeBSQuestions*sizeof(float));
			bsc.dWEBSQuestions = (float*)max_realloc(bsc.dWEBSQuestions, bsc.nSizeBSQuestions*sizeof(float));
			bsc.dWABSQuestions = (float*)max_realloc(bsc.dWABSQuestions, bsc.nSizeBSQuestions*sizeof(float));
			bsc.dwBSQuestions = (DWORD*)max_realloc(bsc.dwBSQuestions, bsc.nSizeBSQuestions*sizeof(DWORD));
			bsc.dwDBSQuestions = (DWORD*)max_realloc(bsc.dwDBSQuestions, bsc.nSizeBSQuestions*30*sizeof(DWORD));
			bsc.dwDBSQuestionsEx = (DWORD*)max_realloc(bsc.dwDBSQuestionsEx, bsc.nSizeBSQuestions*40*sizeof(DWORD));
			bsc.dWIBSQuestions[bsc.nSizeBSQuestions-1] = 0.0;
			bsc.dWEBSQuestions[bsc.nSizeBSQuestions-1] = 0.0;
			bsc.dWABSQuestions[bsc.nSizeBSQuestions-1] = 0.0;
			bsc.dwBSQuestions[bsc.nSizeBSQuestions-1] = 0;
			for(int x = 0; x < 30; x++) bsc.dwDBSQuestions[(bsc.nSizeBSQuestions-1)*30+x] = 0;
			for(int x = 0; x < 40; x++) bsc.dwDBSQuestionsEx[(bsc.nSizeBSQuestions-1)*40+x] = 0;

			int nInfo = 0x0;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, y, 0)==21)
				{
					nInfo = pDoc->GetArrowPar(x, y, 0);
					break;
				}
			}
			int nArg1 = nInfo & 0x1F;
			int nArg2 = (nInfo >> 5) & 0x1F;
			int nType = (nInfo >> 10) & 0x3F;
			// Averiguar el numero de respuestas y procesar
			int nResp = 0;
			switch(nType)
			{
			case 0: nResp = nArg2+1;
				GQMemo1(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 1: nResp = 5;	// Las 4 respuestas + 1
				GQMemo2(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 2: nResp = 6;	// Las 5 Respuestas + 1
				GQMemo3(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 3: nResp = nArg1*2+1; // Respuestas + 1
				GQMemo4(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 4: nResp = nArg1;
				GQMemo5(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 5: nResp = 2; // La respuesta + el nivel de muestreo
				GQObse1(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 6: nResp = 2;
				GQObse2(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 7: nResp = 5;
				GQObse3(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 8: nResp = 1;
				GQObse4(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 9: nResp = 2;
				GQObse5(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 10: nResp = 1;
				GQMath1(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 11: nResp = 1;
				GQMath2(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 12: nResp = 1;
				GQMath3(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 13: nResp = 5;
				GQMath4(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 14: nResp = 1;
				GQMath5(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 15: nResp = 1;
				GQAnal1(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 16: nResp = 1;
				GQAnal2(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 17: nResp = 2;
				GQAnal3(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 18: nResp = 1;
				GQAnal4(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			case 19: nResp = nArg1*2+1;
				GQAnal5(y, bsc.nSizeBSQuestions-1, nResp, nArg1, nArg2);
				break;
			}
		}

		if(pDoc->SomeBSPopUp(y, 0))
		{
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, y, 0) == 23)
				{
					bsc.nSizeBSPopUp++;
					bsc.dWBSPopUp = (float*)max_realloc(bsc.dWBSPopUp, bsc.nSizeBSPopUp*sizeof(float));
					bsc.dwBSPopUp = (DWORD*)max_realloc(bsc.dwBSPopUp, bsc.nSizeBSPopUp*sizeof(DWORD));
					int nIndex = bsc.nSizeBSPopUp-1;
					bsc.dWBSPopUp[nIndex] = pDoc->BeatArrowTime(y);
					bsc.dwBSPopUp[nIndex] = pDoc->GetArrowPar(x, y, 0);
				}
			}
		}
	}
	FILE* hFile;
	hFile = _tfopen(_T("skins/BS/Messages.acc"), _T("r+"));
	if(hFile == NULL)
	{
		for(int x = 0; x < 50; x++) strBSDisp[x] = _T("");
	}
	else
	{
	    for(int x = 0; x < 50; x++)
        {
            TCHAR chMsg[500];
            _fgets(chMsg, 500, hFile);
            if(chMsg == NULL) strBSDisp[x] = _T("");
            else
            {
                strBSDisp[x].Format(_T("%s"), chMsg);
                strBSDisp[x].Remove(_T('\n'));
                strBSDisp[x].Replace(_T('#'), _T('\n'));
            }
        }
        fclose(hFile);
	}

	return 1;
}

void CMaxStepSmu::DeleteBS(void)
{
	if(bsc.nSizeBSQuestions)
	{
		bsc.nSizeBSQuestions=0;
		bsc.dWIBSQuestions = (float*)max_realloc(bsc.dWIBSQuestions, bsc.nSizeBSQuestions*sizeof(float));
		bsc.dWEBSQuestions = (float*)max_realloc(bsc.dWEBSQuestions, bsc.nSizeBSQuestions*sizeof(float));
		bsc.dWABSQuestions = (float*)max_realloc(bsc.dWABSQuestions, bsc.nSizeBSQuestions*sizeof(float));
		bsc.dwBSQuestions = (DWORD*)max_realloc(bsc.dwBSQuestions, bsc.nSizeBSQuestions*sizeof(DWORD));
		bsc.dwDBSQuestions = (DWORD*)max_realloc(bsc.dwDBSQuestions, bsc.nSizeBSQuestions*30*sizeof(DWORD));
		bsc.dwDBSQuestionsEx = (DWORD*)max_realloc(bsc.dwDBSQuestionsEx, bsc.nSizeBSQuestions*40*sizeof(DWORD));
	}
	if(bsc.nSizeBSAnswers)
	{
		bsc.nSizeBSAnswers=0;
		bsc.dWBSAnswers = (float*)max_realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(float));
		bsc.nWWBSAnswers = (__int64*)max_realloc(bsc.nWWBSAnswers, bsc.nSizeBSAnswers*sizeof(__int64));
		bsc.dwBSAnswers = (DWORD*)max_realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*10*sizeof(DWORD));
		bsc.nBSAnswers = (int*)max_realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*10*sizeof(int));
		bsc.nDBSAnswers = (int*)max_realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
	}
	if(bsc.nSizeBSPopUp)
	{
		bsc.nSizeBSPopUp = 0;
		bsc.dWBSPopUp = (float*)max_realloc(bsc.dWBSPopUp, bsc.nSizeBSPopUp*sizeof(float));
		bsc.dwBSPopUp = (DWORD*)max_realloc(bsc.dwBSPopUp, bsc.nSizeBSPopUp*sizeof(DWORD));
	}
}

void CMaxStepSmu::GQMemo1(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha ocurrido un error:\n"
			"Incongruencia de datos\n"
			"Se supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n"
			"No se garantiza el funcionamiento correcto de Brain Shower\n"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str);
		delete nCoord;
		delete nArrayCoord;
		return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha ocurrido un error:\n"
			"Error de sintaxis\n"
			"En la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n"
			"No se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg2);
		_printf(str);
		delete nCoord;
		delete nArrayCoord;
		return;
	}
	DWORD *dwOrdAnsw = new DWORD[nResp*ARROWSLINE_SINGLE];
	int nDist=nArg1/10;
	int nMode=nArg1-nDist*10;
	int nSize=nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	for(int x = 0; x < nSize; x++)
	{
		int kMode;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 10;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		nNum = rand()%nLimit;
		dwOrdAnsw[x*ARROWSLINE_SINGLE]=MAKELONG(nNum, nType);
		for(int z = 1; z < 5; z++)
		{
			if(nMode <= 5) kMode = nMode;
			else
			{
				if(nMode == 6)
				{
					int nPosib[] = {0, 1, 3, 5};
					kMode = nPosib[rand()%4];
				}
				else
				{
					int nPosib[] = {0, 2, 4, 5};
					kMode = nPosib[rand()%4];
				}
			}
			switch(kMode)
			{
			case 0:nType = 1;nLimit = 10;break;
			case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
			case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
			case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
			case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
			case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
			}
			bool bCond = false;
			do
			{
				nNum = rand()%nLimit;
				DWORD dwAdd = MAKELONG(nNum, nType);
				bCond = false;
				for(int w = 0; w < ARROWSLINE_SINGLE; w++)
				{
					if(dwOrdAnsw[x*ARROWSLINE_SINGLE+w] == dwAdd)
					{
						bCond = true;
					}
				}
			} while(bCond);
			dwOrdAnsw[x*ARROWSLINE_SINGLE+z]=MAKELONG(nNum, nType);
		}
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=0;	// Tipo de pregunta: Memoria 1
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40]=MAKELONG(nDist+1,0);	// Distribución (1 o 2 veces lo normal)
	bsc.dwDBSQuestionsEx[nIndex*40+1]=MAKELONG(nSize,0);	// Tamaño (para ahorrar procesador)
	for(int x = 0; x < nSize; x++)
	{
		bsc.dwDBSQuestions[nIndex*30+x] = dwOrdAnsw[x*ARROWSLINE_SINGLE];
	}
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = ARROWSLINE_SINGLE;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);

		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(nArrowsPerLine-nFirst);
			bID = false;
			for(int t = 0; t < ARROWSLINE_SINGLE; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = dwOrdAnsw[(i-1)*ARROWSLINE_SINGLE];
		bsc.nDBSAnswers[nInd] = 1;	// Mostrar numeros como tal
		for(int x = 1; x < nDis; x++)
		{
			do
			{
				nDisp = rand()%(nArrowsPerLine-nFirst);
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = dwOrdAnsw[(i-1)*ARROWSLINE_SINGLE+x];
			bsc.nDBSAnswers[nInd] = 1;	// Mostrar numeros como tal
		}
	}
	delete dwOrdAnsw;
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQMemo2(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\n"
			"Incongruencia de datos\n"
			"Se supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n"
			"No se garantiza el funcionamiento correcto de Brain Shower\n"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	DWORD dwCaret [ARROWSLINE_SINGLE];
	int nPos1[ARROWSLINE_SINGLE];
	int nPos2[ARROWSLINE_SINGLE];
	int nOrd[ARROWSLINE_SINGLE];
	for(int x = 0; x < ARROWSLINE_SINGLE; x++)
	{
		dwCaret[x] = 0xFFFFFFFF;
		nPos1[x] = -1;
		nPos2[x] = -1;
		nOrd[x] = -1;
	}
	int nMode=nArg1;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	for(int x = 0; x < ARROWSLINE_SINGLE; x++)
	{
		DWORD dwObj;
		int nsPos1;
		int nsPos2;
		int kMode;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 10;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwObj=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < ARROWSLINE_SINGLE; w++)
			{
				if(dwCaret[w] == dwObj)
				{
					bCond = true;
				}
			}
		} while(bCond);
		dwCaret[x] = dwObj;
		do
		{
			nsPos1 = rand()%ARROWSLINE_SINGLE;
			bCond = false;
			for(int w = 0; w < ARROWSLINE_SINGLE; w++)
			{
				if(nPos1[w] == nsPos1)
				{
					bCond = true;
				}
			}
		} while(bCond);
		nPos1[x] = nsPos1;
		if(x != (ARROWSLINE_SINGLE-1))
		{
			do
			{
				nsPos2 = rand()%(ARROWSLINE_SINGLE-1);
				bCond = false;
				for(int w = 0; w < ARROWSLINE_SINGLE; w++)
				{
					if(nPos2[w] == nsPos2)
					{
						bCond = true;
					}
				}
			} while(bCond);
		}
		else
		{
			nsPos2 = ARROWSLINE_SINGLE;
		}
		nPos2[x] = nsPos2;
	}
	bsc.dwBSQuestions[nIndex]=1;	// Tipo de pregunta: Memoria 2
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nCount==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nCount==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);

	int nOtherPos[ARROWSLINE_SINGLE];
	for(int x = 0; x < ARROWSLINE_SINGLE; x++)
	{
		switch(nPos1[x])
		{
		case 0: nOtherPos[x]=6; break;
		case 1: nOtherPos[x]=0; break;
		case 2: nOtherPos[x]=4; break;
		case 3: nOtherPos[x]=2; break;
		case 4: nOtherPos[x]=8; break;
		default: nOtherPos[x]=0; break;
		}
		// Item Display (Answers)
		bsc.dwDBSQuestionsEx[nIndex*40+nOtherPos[x]] = dwCaret[x];
		// Item Display (Questions)
		bsc.dwDBSQuestionsEx[nIndex*40+(nPos2[x]*2+1)] = dwCaret[x];
	}
	// Diseñar las respuestas y las preguntas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = ARROWSLINE_SINGLE;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);

		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = nFirst/ARROWSLINE_SINGLE*ARROWSLINE_SINGLE;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		nDisAr[0] = nPos1[i-1];
		bsc.nBSAnswers[nInd*nArrowsPerLine+nPos1[i-1]+nFirst] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nPos1[i-1]+nFirst] = MAKELONG(17, 1);
		// Appear Time (Answers)
		bsc.dwDBSQuestionsEx[nIndex*40+10+nOtherPos[i-1]] = (DWORD)bsc.nWWBSAnswers[nInd];
		// Dissapear Time (Answers)
		bsc.dwDBSQuestionsEx[nIndex*40+20+nOtherPos[i-1]] = (DWORD)(bsc.nWWBSAnswers[nInd])+500;
		// Appear Time (Questions)
		bsc.dwDBSQuestionsEx[nIndex*40+10+(nPos2[i-1]*2+1)] = (DWORD)BeatToTime(pDoc->BeatArrowTime(nCoord[i-1]));
		// Dissapear Time (Questions)
		bsc.dwDBSQuestionsEx[nIndex*40+20+(nPos2[i-1]*2+1)] = (DWORD)(bsc.nWWBSAnswers[nInd]+500);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%ARROWSLINE_SINGLE;
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = MAKELONG(17, 1);
		}
	}
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQMemo3(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	DWORD dwOrdAnsw[ARROWSLINE_SINGLE];
	int nRespAr[ARROWSLINE_SINGLE];
	for(int x = 0; x < ARROWSLINE_SINGLE; x++)
	{
		dwOrdAnsw[x] = 0xFFFFFFFF;
		nRespAr[x] = -1;
	}
	int nMode=nArg1;
	int nSize=ARROWSLINE_SINGLE;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	for(int x = 0; x < nSize; x++)
	{
		int kMode;
		DWORD dwAdd;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 10;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < ARROWSLINE_SINGLE; w++)
			{
				if(dwOrdAnsw[w] == dwAdd)
				{
					bCond = true;
				}
			}
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%ARROWSLINE_SINGLE;
			bID = false;
			for(int t = 0; t < ARROWSLINE_SINGLE; t++)
				if(nDisp == nRespAr[t])
					bID = true;
		} while (bID);
		nRespAr[x] = nDisp;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=2;	// Tipo de pregunta: Memoria 1
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	for(int x = 0; x < nSize; x++)
	{
		bsc.dwDBSQuestions[nIndex*30+x] = dwOrdAnsw[x];
		bsc.dwDBSQuestionsEx[nIndex*40+nRespAr[x]] = dwOrdAnsw[x];
	}
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = ARROWSLINE_SINGLE;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);

		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = nFirst/ARROWSLINE_SINGLE*ARROWSLINE_SINGLE;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		nDisAr[0] = nRespAr[i-1];
		bsc.nBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = MAKELONG(17,1);
		bsc.dwDBSQuestionsEx[nIndex*40+nDisAr[0]+ARROWSLINE_SINGLE] = (DWORD)bsc.nWWBSAnswers[nInd];
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%ARROWSLINE_SINGLE;
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = MAKELONG(17, 1);
		}
	}
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQMemo4(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg1 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg1);
		_printf(str); return;
	}
	int nSize = min(nArg1, nCount);
	int *RespAr = new int[nSize];
	for(int x = 0; x < nSize; x++)
	{
		do
		{
			RespAr[x] = rand()%nArrowsPerLine;
		} while (nArrayCoord[x*nArrowsPerLine+RespAr[x]] == 0);
		bsc.dwDBSQuestions[nIndex*30+x] = MAKELONG(RespAr[x],4);
		bsc.dwDBSQuestionsEx[nIndex*40+x] = (DWORD)BeatToTime(pDoc->BeatArrowTime(nCoord[x]));
	}

	for(int i = 0; i < nArg1; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);

		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr = RespAr[i];
		bsc.nBSAnswers[nInd*nArrowsPerLine+nDisAr] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisAr] = MAKELONG(1,0);
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=3;	// Tipo de pregunta: Memoria 4
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Media Respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[min(nArg1,nCount-1)]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40+30] = nSize;

	nDis = ARROWSLINE_SINGLE;
	for(int i = (nArg1+1); i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);

		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = RespAr[i-nArg1-1]/ARROWSLINE_SINGLE*ARROWSLINE_SINGLE;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		nDisAr[0] = RespAr[i-nArg1-1]-nFirst;
		bsc.nBSAnswers[nInd*nArrowsPerLine+RespAr[i-nArg1-1]] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+RespAr[i-nArg1-1]] = MAKELONG(17,1);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%ARROWSLINE_SINGLE;
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = MAKELONG(17, 1);
		}
	}
	delete RespAr;
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQMemo5(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg1 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg1);
		_printf(str); return;
	}
	if(nArg2 > 10)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser mayor que 10\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg2);
		_printf(str); return;
	}
	int nSize = min(nArg1, nCount);
	int *RespAr = new int[nSize];
	for(int x = 0; x < nSize; x++)
	{
		do
		{
			RespAr[x] = rand()%nArrowsPerLine;
		} while (nArrayCoord[x*nArrowsPerLine+RespAr[x]] == 0);
		bsc.dwDBSQuestions[nIndex*30+x] = MAKELONG(RespAr[x],4);
		bsc.dwDBSQuestionsEx[nIndex*40+x] = (DWORD)BeatToTime(pDoc->BeatArrowTime(nCoord[x]));
	}

	bsc.dwBSQuestions[nIndex]=MAKELONG(4,0);	// Tipo de pregunta: Memoria 5
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Media Respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[min(nArg1,nCount-1)]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40+30] = nSize;

	nDis = 5;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);

		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = RespAr[i-nArg1-1]/ARROWSLINE_SINGLE*ARROWSLINE_SINGLE;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		nDisAr[0] = RespAr[i]-nFirst;
		bsc.nBSAnswers[nInd*nArrowsPerLine+RespAr[i]] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+RespAr[i]] = MAKELONG(nArg2,18);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%5;
				bID = false;
				for(int t = 0; t < 5; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = MAKELONG(nArg2,18);
		}
	}
	delete RespAr;
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQObse1(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg2);
		_printf(str); return;
	}
	int nDist=nArg1/10;
	int nMode=nArg1-nDist*10;
	int nSize=nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	DWORD *dwOrdAnsw = new DWORD[nSize+1];
	for(int x = 0; x <= nSize ; x++)
	{
		dwOrdAnsw[x] = 0xFFFFFFFF;
	}
	for(int x = 0; x <= nSize ; x++)
	{
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 20;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < ARROWSLINE_SINGLE; w++)
				if(dwOrdAnsw[w] == dwAdd)
					bCond = true;
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=5;	// Tipo de pregunta: Observación 1
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40]=MAKELONG(nDist+1,0);	// Distribución (1 o 2 veces lo normal)
	bsc.dwDBSQuestionsEx[nIndex*40+1]=MAKELONG(nSize,0);	// Tamaño (para ahorrar procesador)
	for(int x = 0; x < nSize; x++)
	{
		bsc.dwDBSQuestions[nIndex*30+x] = dwOrdAnsw[x];
	}
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = ARROWSLINE_SINGLE;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);

		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		nDis = min(nSize,nDis);
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		int nCaretAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nCaretAr[x] = -1;
		/////////////////////////
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(nArrowsPerLine-nFirst);
			bID = false;
			for(int t = 0; t < ARROWSLINE_SINGLE; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		/////////////////////////
		bsc.nBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = dwOrdAnsw[nSize];
		for(int x = 1; x < nDis; x++)
		{
			int nDir;
			do
			{
				nDisp = rand()%(nArrowsPerLine-nFirst);
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%nSize;
				bID = false;
				for(int t = 0; t < nSize; t++)
					if(nDir == nCaretAr[t])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = dwOrdAnsw[nDir];
		}
	}
	delete dwOrdAnsw;
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQObse2(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg2);
		_printf(str); return;
	}
	int nDist=nArg1/10;
	int nMode=nArg1-nDist*10;
	int nSize=nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int nPosRep = (rand()%(nSize-1))+1;
	DWORD *dwOrdAnsw = new DWORD[nSize];
	for(int x = 0; x < nSize ; x++)
	{
		dwOrdAnsw[x] = 0xFFFFFFFF;
	}
	for(int x = 0; x < nSize; x++)
	{
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 20;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		if(x != nPosRep)
		{
			do
			{
				nNum = rand()%nLimit;
				dwAdd=MAKELONG(nNum, nType);
				bCond = false;
				for(int w = 0; w < ARROWSLINE_SINGLE; w++)
					if(dwOrdAnsw[w] == dwAdd)
						bCond = true;
			} while(bCond);
		}
		else
		{
			nNum = rand()%x;
			dwAdd=dwOrdAnsw[nNum];
		}
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=6;	// Tipo de pregunta: Observación 1
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40]=MAKELONG(nDist+1,0);	// Distribución (1 o 2 veces lo normal)
	bsc.dwDBSQuestionsEx[nIndex*40+1]=MAKELONG(nSize,0);	// Tamaño (para ahorrar procesador)
	for(int x = 0; x < nSize; x++)
	{
		bsc.dwDBSQuestions[nIndex*30+x] = dwOrdAnsw[x];
	}
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = ARROWSLINE_SINGLE;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		nDis = min(nSize-2,nDis);
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		int nCaretAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nCaretAr[x] = -1;
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(nArrowsPerLine-nFirst);
			bID = false;
			for(int t = 0; t < ARROWSLINE_SINGLE; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = dwOrdAnsw[nPosRep];
		for(int x = 1; x < nDis; x++)
		{
			int nDir;
			do
			{
				nDisp = rand()%(nArrowsPerLine-nFirst);
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%nSize;
				bID = false;
				for(int t = 0; t < nSize; t++)
					if(nDir == nCaretAr[t] || dwOrdAnsw[nDir] == dwOrdAnsw[nPosRep])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = dwOrdAnsw[nDir];
		}
	}
	delete dwOrdAnsw;
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQObse3(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	int nSum = 1;
	nSum = max(1, nSum);
	int nMin = 0;
	int nMax = 20;
	int nResults[ARROWSLINE_SINGLE];
	for(int x = 0; x < ARROWSLINE_SINGLE; x++) nResults[x] = -1;
	if(nArg1)
	{
		int nCar = 0;
		for(int x = 0; x < ARROWSLINE_SINGLE; x++)
		{
			nResults[x] = (rand()%(17+x-nCar)) + nCar;
			nCar = nResults[x]+1;
		}
	}
	else
	{
		int nCar = 20;
		for(int x = 0; x < ARROWSLINE_SINGLE; x++)
		{
			nResults[x] = nCar - (rand()%(nCar-3+x));
			nCar = nResults[x]-1;
		}
	}
	int nCaretOpt[ARROWSLINE_SINGLE];
	int nCaretOrd[ARROWSLINE_SINGLE];
	for(int x = 0; x < ARROWSLINE_SINGLE; x++)
	{
		nCaretOpt[x] = x;
		nCaretOrd[x] = 0;
	}
	for(int x = 0; x < ARROWSLINE_SINGLE; x++)
	{
		int nIndResp;
		do
		{
			nIndResp = nCaretOpt[rand()%ARROWSLINE_SINGLE];
		} while(nIndResp == -1);
		nCaretOpt[nIndResp] = -1;
		nCaretOrd[x] = nIndResp;
		bsc.dwDBSQuestionsEx[nIndex*40+nIndResp*4] = MAKELONG(nResults[x], 1);
		bsc.dwDBSQuestionsEx[nIndex*40+20+x] = 0;
	}
	bsc.dWIBSQuestions[nIndex] = pDoc->BeatArrowTime(y);
	bsc.dwBSQuestions[nIndex] = 7;
	// ** Esto es juzgar terminos, por lo tanto solo deben haber 3 respuestas maximo
	// Diseñar la respuesta
	nDis = ARROWSLINE_SINGLE;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);

		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
		}
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = nFirst/ARROWSLINE_SINGLE*ARROWSLINE_SINGLE;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		nDisAr[0] = nCaretOrd[i];
		bsc.nBSAnswers[nInd*nArrowsPerLine+nCaretOrd[i]+nFirst] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nCaretOrd[i]+nFirst] = MAKELONG(17, 1);
		bsc.dwDBSQuestionsEx[nIndex*40+25+nCaretOrd[i]] = (DWORD)bsc.nWWBSAnswers[nInd];
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%ARROWSLINE_SINGLE;
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = MAKELONG(17, 1);
		}
	}
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQObse4(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg2);
		_printf(str); return;
	}

	int nIsMax=nArg1/10;
	int nMode=nArg1-nIsMax*10;
	int nSize= min(nArg2,7);
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int nCar = 7;
	int *nResults = new int[nSize];
	DWORD *dwOrdAnsw = new DWORD[nSize];
	for(int x = 0; x < nSize ; x++)
	{
		dwOrdAnsw[x] = 0xFFFFFFFF;
		nResults[x] = 0;
	}

	for(int x = 0; x < nSize ; x++)
	{
		nResults[x] = nCar - (rand()%(nCar-nSize+1+x));
		nCar = nResults[x]-1;
		//////////////////////////////////////////
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 20;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < ARROWSLINE_SINGLE; w++)
				if(dwOrdAnsw[w] == dwAdd)
					bCond = true;
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=8;	// Tipo de pregunta: Observación 4
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	for(int x = 0; x < nSize; x++)
	{
		for(int i = 0; i < nResults[x]; i++)
		{
			int nPos;
			bool bCond = false;
			do
			{
				nPos = rand()%40;
				if(bsc.dwDBSQuestionsEx[nIndex*40+nPos]) bCond = true;
				else
				{
					bsc.dwDBSQuestionsEx[nIndex*40+nPos] = dwOrdAnsw[x];
					bCond = false;
				}
			} while (bCond);
		}
	}
	DWORD dwBegin = (DWORD)BeatToTime(bsc.dWIBSQuestions[nIndex]);
	for(int x = 0; x < 12; x++)
	{
		int nPos;
		bool bCond = false;
		do
		{
			nPos = rand()%12;
			if(bsc.dwDBSQuestions[nIndex*30+nPos]) bCond = true;
			else
			{
				bsc.dwDBSQuestions[nIndex*30+nPos] = 75*x+dwBegin;
				bCond = false;
			}
		} while (bCond);
	}
	// Diseñar las respuestas
	nDis = ARROWSLINE_SINGLE;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		nDis = min(nSize,nDis);
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		int nCaretAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nCaretAr[x] = -1;
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(nArrowsPerLine-nFirst);
			bID = false;
			for(int t = 0; t < ARROWSLINE_SINGLE; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = 1;
		nCaretAr[0] = nIsMax?0:(nSize-1);
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = dwOrdAnsw[nCaretAr[0]];
		for(int x = 1; x < nDis; x++)
		{
			int nDir;
			do
			{
				nDisp = rand()%(nArrowsPerLine-nFirst);
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%nSize;
				bID = false;
				for(int t = 0; t < nSize; t++)
					if(nDir == nCaretAr[t])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = dwOrdAnsw[nDir];
		}
	}
	delete dwOrdAnsw;
	delete nResults;
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQObse5(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg2);
		_printf(str); return;
	}

	int nMode=nArg1;
	int nSize= min(nArg2,7);
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int nHidPos = rand()%nSize;

	int *nResults = new int[nSize];
	DWORD *dwOrdAnsw = new DWORD[nSize];
	for(int x = 0; x < nSize ; x++)
	{
		dwOrdAnsw[x] = 0xFFFFFFFF;
		nResults[x] = 0;
	}

	for(int x = 0; x < nSize ; x++)
	{
		nResults[x] = (rand()%(36/nSize-1))+1;
		//////////////////////////////////////////
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 20;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < ARROWSLINE_SINGLE; w++)
				if(dwOrdAnsw[w] == dwAdd)
					bCond = true;
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=9;	// Tipo de pregunta: Observación 4
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dwBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dwBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	for(int x = 0; x < nSize; x++)
	{
		for(int i = 0; i < nResults[x]; i++)
		{
			int nPos;
			bool bCond = false;
			do
			{
				nPos = rand()%40;
				if(bsc.dwDBSQuestionsEx[nIndex*40+nPos]) bCond = true;
				else
				{
					bsc.dwDBSQuestionsEx[nIndex*40+nPos] = dwOrdAnsw[x];
					bCond = false;
				}
			} while (bCond);
		}
	}
	DWORD dwBegin = (DWORD)BeatToTime(bsc.dWIBSQuestions[nIndex]);
	for(int x = 0; x < 12; x++)
	{
		int nPos;
		bool bCond = false;
		do
		{
			nPos = rand()%12;
			if(bsc.dwDBSQuestions[nIndex*30+nPos]) bCond = true;
			else
			{
				bsc.dwDBSQuestions[nIndex*30+nPos] = 75*x+dwBegin;
				bCond = false;
			}
		} while (bCond);
	}
	bsc.dwDBSQuestions[nIndex*30+13] = dwOrdAnsw[nHidPos];
	// Diseñar las respuestas
	nDis = ARROWSLINE_SINGLE;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		nDis = min(nSize,nDis);
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		int nCaretAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nCaretAr[x] = -1;
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(nArrowsPerLine-nFirst);
			bID = false;
			for(int t = 0; t < ARROWSLINE_SINGLE; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = 1;
		nCaretAr[0] = nHidPos;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = dwOrdAnsw[nCaretAr[0]];
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp, nDir;
			do
			{
				nDisp = rand()%(nArrowsPerLine-nFirst);
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%nSize;
				bID = false;
				for(int t = 0; t < nSize; t++)
					if(nDir == nCaretAr[t])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = dwOrdAnsw[nDir];
		}
	}
	delete dwOrdAnsw;
	delete nResults;
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQAnal1(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	/*if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg2);
		_printf(str); return;
	}*/
	DWORD dwOrdAnsw[ARROWSLINE_SINGLE+1];
	int nMode=nArg1;
	//int nSize=nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int kMode;
	if(nMode <= 5) kMode = nMode;
	else
	{
		if(nMode == 6)
		{
			int nPosib[] = {0, 1, 3, 5};
			kMode = nPosib[rand()%4];
		}
		else
		{
			int nPosib[] = {0, 2, 4, 5};
			kMode = nPosib[rand()%4];
		}
	}
	switch(kMode)
	{
	case 0:nType = 1;nLimit = 10;break;
	case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
	case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
	case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
	case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
	case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
	}
	nNum = rand()%nLimit;
	dwOrdAnsw[0]=MAKELONG(nNum, nType);
	dwOrdAnsw[ARROWSLINE_SINGLE]=MAKELONG(nNum, nType==1?1:(nType+3));
	for(int z = 1; z < ARROWSLINE_SINGLE; z++)
	{
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 10;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			DWORD dwAdd = MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < ARROWSLINE_SINGLE; w++)
			{
				if(dwOrdAnsw[w] == dwAdd)
				{
					bCond = true;
				}
			}
		} while(bCond);
		dwOrdAnsw[z]=MAKELONG(nNum, nType);
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=15;	// Tipo de pregunta: Analisys 1
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		pDoc->BeatArrowTime(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		pDoc->BeatArrowTime(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40] = dwOrdAnsw[ARROWSLINE_SINGLE];
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = ARROWSLINE_SINGLE;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);

		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(nArrowsPerLine-nFirst);
			bID = false;
			for(int t = 0; t < ARROWSLINE_SINGLE; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = dwOrdAnsw[0];
		bsc.nDBSAnswers[nInd] = 1;	// Mostrar numeros como tal
		for(int x = 1; x < nDis; x++)
		{
			do
			{
				nDisp = rand()%(nArrowsPerLine-nFirst);
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = dwOrdAnsw[x];
			bsc.nDBSAnswers[nInd];	// Mostrar numeros como tal
		}
	}
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQAnal2(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg2);
		_printf(str); return;
	}

	int nMode=nArg1;
	int nSize= nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int nCar = 7;

	DWORD *dwOrdAnsw = new DWORD[nSize];
	for(int x = 0; x < nSize ; x++)
	{
		dwOrdAnsw[x] = 0xFFFFFFFF;
	}

	for(int x = 0; x < nSize ; x++)
	{
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 20;break;
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < ARROWSLINE_SINGLE; w++)
				if(dwOrdAnsw[w] == dwAdd)
					bCond = true;
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=16;	// Tipo de pregunta: Analisis 2
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	int nData[28][16];
	FILE* hFile;
	hFile = _tfopen(_T("skins\\BS\\BS42.acc"), _T("r+"));
	if(hFile != NULL)
	{
		TCHAR ch[100];
		for(int x = 0; x < 28; x++)
		{
			for(int i = 0; i < 4; i++)
			{
				_fgets(ch, 100, hFile);
				if(_sscanf(ch, _T("%d %d %d %d"), &nData[x][i*4], &nData[x][i*4+1], &nData[x][i*4+2], &nData[x][i*4+3]) != 4)
					_printf(_T("El archivo BS42.acc no tiene el formato adecuado, no se garantiza el funcionamiento del Brain Shower"));
			}
		}
		fclose(hFile);
	}
	else _printf(_T("El archivo BS42.acc no se encuentra en la ruta, no se garantiza el funcionamiento del Brain Shower"));

	int nSizePerData = 0;
	int nPerData[28];
	int nSizePosPerData[28];
	int nPosPerDataX[28][36];
	int nPosPerDataY[28][36];
	int nNumItem = 0;
	for(int x = 0; x < nSize; x++)
	{
		nSizePerData = 0;
		for(int j = 0; j < 28; j++) nSizePosPerData[j] = 0;
		for(int i = 0; i < 28; i++)	// Recorrer los arreglos
		{
			for(int u = 0; u < 6; u++) for(int v = 0; v < 6; v++)	// Recorrer las posiciones
			{
				bool bInc = true;
				for(int a = 0; a < 4; a++)
				{
					for(int b = 0; b < 4; b++)
					{
						if((v+b) < 6 && (u+a) < 6)
						{
							if(bsc.dwDBSQuestionsEx[nIndex*40+(v+b)*6+(u+a)] && nData[i][b*4+a])
							{
								bInc = false;
								break;
							}
						}
						else if(nData[i][b*4+a])
						{
							bInc = false;
							break;
						}
					}
					if(!bInc) break;
				}
				if(bInc)
				{
					nSizePosPerData[i]++;
					nPosPerDataX[i][nSizePosPerData[i]-1] = u;
					nPosPerDataY[i][nSizePosPerData[i]-1] = v;
				}
			}
			if(nSizePosPerData[i] != 0)
			{
				nSizePerData++;
				nPerData[nSizePerData-1] = i;
			}
		}
		if(nSizePerData == 0) break;	// Para que si no podemos incluir mas?
		// Randomizar el arreglo
		int p = rand()%nSizePerData;
		p = nPerData[p];
		int q = rand()%nSizePosPerData[p];
		int u = nPosPerDataX[p][q], v = nPosPerDataY[p][q];
		for(int a = 0; a < 4; a++)
		{
			for(int b = 0; b < 4; b++)
			{
				if((v+b) < 6 && (u+a) < 6)
				{
					if(nData[p][b*4+a])
					{
						bsc.dwDBSQuestionsEx[nIndex*40+(v+b)*6+(u+a)] = dwOrdAnsw[x];
						nNumItem++;
					}
				}
			}
		}
	}
	DWORD dwBegin = (DWORD)BeatToTime(bsc.dWIBSQuestions[nIndex]);
	for(int x = 0; x < 12; x++)
	{
		int nPos;
		bool bCond = false;
		do
		{
			nPos = rand()%12;
			if(bsc.dwDBSQuestions[nIndex*30+nPos]) bCond = true;
			else
			{
				bsc.dwDBSQuestions[nIndex*30+nPos] = 75*x+dwBegin;
				bCond = false;
			}
		} while (bCond);
	}
	// Diseñar las respuestas
	nDis = ARROWSLINE_SINGLE;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		int nCaretAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nCaretAr[x] = -1;
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(nArrowsPerLine-nFirst);
			bID = false;
			for(int t = 0; t < ARROWSLINE_SINGLE; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		bsc.nBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = 1;
		nCaretAr[0] = nNumItem;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = MAKELONG(nNumItem,1);
		for(int x = 1; x < nDis; x++)
		{
			int nDir;
			do
			{
				nDisp = rand()%(nArrowsPerLine-nFirst);
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%36;
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDir == nCaretAr[t])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = MAKELONG(nDir,1);
		}
	}
	delete dwOrdAnsw;
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQAnal3(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg2);
		_printf(str); return;
	}
	// Encontrar el número de items
	int nCaretSizeCoord = 0;
	int nFirst = -1;
	if(nCount >= 2)
	{
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
	}

	int nDist=nArg1/10;
	int nMode=nArg1-nDist*10;
	int nSize=nArg2;
	int nType = 0;
	int nNum = 0;
	int nLimit = 1;
	int nPosQ = rand()%nCaretSizeCoord;
	int nPosS = rand()%nSize;

	DWORD *dwOrdAnsw = new DWORD[nCaretSizeCoord];
	for(int x = 0; x < nCaretSizeCoord ; x++)
	{
		dwOrdAnsw[x] = 0xFFFFFFFF;
	}

	for(int x = 0; x < nCaretSizeCoord; x++)
	{
		int kMode;
		DWORD dwAdd = 0x0;
		if(nMode <= 5) kMode = nMode;
		else
		{
			if(nMode == 6)
			{
				int nPosib[] = {0, 1, 3, 5};
				kMode = nPosib[rand()%4];
			}
			else
			{
				int nPosib[] = {0, 2, 4, 5};
				kMode = nPosib[rand()%4];
			}
		}
		switch(kMode)
		{
		case 0:nType = 1;nLimit = 17;break;	// No podemos incluir la pregunta ahi... ^^
		case 1:nType = 2;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 2:nType = 2;nLimit = bs->skin[nType]->m_nSize;break;
		case 3:nType = 3;nLimit = bs->skin[nType]->m_nSize/2;break;
		case 4:nType = 3;nLimit = bs->skin[nType]->m_nSize;break;
		case 5:nType = 4;nLimit = bs->skin[nType]->m_nSize; break;
		}
		bool bCond = false;
		do
		{
			nNum = rand()%nLimit;
			dwAdd=MAKELONG(nNum, nType);
			bCond = false;
			for(int w = 0; w < ARROWSLINE_SINGLE; w++)
				if(dwOrdAnsw[w] == dwAdd)
					bCond = true;
		} while(bCond);
		dwOrdAnsw[x]=dwAdd;
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=17;	// Tipo de pregunta: Analisys 3
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Primera respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[0]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40]=MAKELONG(nDist+1,0);	// Distribución (1 o 2 veces lo normal)
	bsc.dwDBSQuestionsEx[nIndex*40+1]=MAKELONG(nSize*nCaretSizeCoord,0);	// Tamaño (para ahorrar procesador)
	for(int x = 0; x < nSize; x++) for(int i = 0; i < nCaretSizeCoord; i++)
	{
		if((x*nCaretSizeCoord+i)<30)
		{
			if(i == nPosQ && x == nPosS)
				bsc.dwDBSQuestions[nIndex*30+x*nCaretSizeCoord+i] = MAKELONG(17,1);
			else
				bsc.dwDBSQuestions[nIndex*30+x*nCaretSizeCoord+i] = dwOrdAnsw[i];
		}
	}
	// Diseñar las respuestas
	// CUIDADO: la primera respuesta no se tiene en cuenta
	// La primera respuesta SOLO sirve para determinar la salida de datos
	nDis = ARROWSLINE_SINGLE;
	for(int i = 1; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_DOUBLE];
		for(int x = 0; x < ARROWSLINE_DOUBLE; x++) nDisAr[x] = -1;
		int nCaretAr[ARROWSLINE_DOUBLE];
		for(int x = 0; x < ARROWSLINE_DOUBLE; x++) nCaretAr[x] = -1;
		bool bID = true;
		int nDisp;
		do
		{
			nDisp = rand()%(nArrowsPerLine-nFirst);
			bID = false;
			for(int t = 0; t < ARROWSLINE_SINGLE; t++)
				if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
					bID = true;
		} while (bID);
		nDisAr[0] = nDisp;
		nCaretAr[0] = nPosQ;
		bsc.nBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nFirst+nDisAr[0]] = dwOrdAnsw[nPosQ];
		for(int x = 1; x < nDis; x++)
		{
			int nDir;
			do
			{
				nDisp = rand()%(nArrowsPerLine-nFirst);
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t] || !nArrayCoord[i*nArrowsPerLine+nFirst+nDisp])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			do
			{
				nDir = rand()%nCaretSizeCoord;
				bID = false;
				for(int t = 0; t < nCaretSizeCoord; t++)
					if(nDir == nCaretAr[t])
						bID = true;
			} while (bID);
			nCaretAr[x] = nDir;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = dwOrdAnsw[nDir];
		}
	}
	delete dwOrdAnsw;
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQAnal4(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	int RespAr;
	do
	{
		RespAr = rand()%nArrowsPerLine;
	} while (nArrayCoord[RespAr] == 0);
	bsc.dwDBSQuestionsEx[nIndex*40] = MAKELONG(RespAr,4);
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=18;	// Tipo de pregunta: Analisys 4
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Media Respuesta (omitida)
		nCount==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nCount==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);

	// Vamos entonces a girar la flecha!
	int nGir = rand()%3; // 0 = 90, 1 = 180, 2 = 270
	bsc.dwDBSQuestionsEx[nIndex*40+31] = nGir;
	int nDesp = nGir+1;
	int nFirst = RespAr/ARROWSLINE_SINGLE;
	int nArrow = RespAr-nFirst;
	if(nArrow != 2)
	{
		if(nArrow > 2) nArrow--;	// Normalizar la flecha (1245 a 1234)
		nArrow-=nDesp;				// Mover flecha
		while(nArrow > 4)
			nArrow-=4;
		while(nArrow < 0)
			nArrow+=4;
		if(nArrow > 1) nArrow++;	// Volver a liberar la flecha (1234 a 1245)
	}
	RespAr = nArrow+nFirst;
	nDis = ARROWSLINE_SINGLE;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = nFirst/ARROWSLINE_SINGLE*ARROWSLINE_SINGLE;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		nDisAr[0] = RespAr-nFirst;
		bsc.nBSAnswers[nInd*nArrowsPerLine+RespAr] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+RespAr] = MAKELONG(17,1);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%ARROWSLINE_SINGLE;
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = MAKELONG(17,1);
		}
	}
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQAnal5(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg1 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg1);
		_printf(str); return;
	}
	int nSize = min(nArg1, nCount);
	int *RespAr=new int[nSize];
	for(int x = 0; x < nSize; x++)
	{
		do
		{
			RespAr[x] = rand()%nArrowsPerLine;
		} while (nArrayCoord[x*nArrowsPerLine+RespAr[x]] == 0);
		bsc.dwDBSQuestions[nIndex*30+x] = MAKELONG(RespAr[x],4);
		bsc.dwDBSQuestionsEx[nIndex*40+x] = (DWORD)BeatToTime(pDoc->BeatArrowTime(nCoord[x]));
	}

	for(int i = 0; i < nArg1; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr = RespAr[i];
		bsc.nBSAnswers[nInd*nArrowsPerLine+nDisAr] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisAr] = MAKELONG(1,0);
	}
	// Argumentar la pregunta
	bsc.dwBSQuestions[nIndex]=19;	// Tipo de pregunta: Analisys 5
	bsc.dWIBSQuestions[nIndex]=pDoc->BeatArrowTime(y);	// Locación de la pregunta: donde se puso inicialmente
	bsc.dWABSQuestions[nIndex]=					// Locacion de la salida de datos: Media Respuesta (omitida)
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[min(nArg1,nCount-1)]);
	bsc.dWEBSQuestions[nIndex]=					// Locación de la finalización: Ultima respuesta
		nSize==0?bsc.dWIBSQuestions[nIndex]:pDoc->BeatArrowTime(nCoord[nCount-1]);
	bsc.dwDBSQuestionsEx[nIndex*40+30] = nSize;

	// Vamos entonces a girar las flechas!
	int nGir = rand()%3; // 0 = 90, 1 = 180, 2 = 270
	bsc.dwDBSQuestionsEx[nIndex*40+31] = nGir;
	int nDesp = nGir+1;
	for(int x = 0; x < nSize; x++)
	{
		int nFirst = RespAr[x]/ARROWSLINE_SINGLE;
		int nArrow = RespAr[x]-nFirst;
		if(nArrow != 2)
		{
			if(nArrow > 2) nArrow--;	// Normalizar la flecha (1245 a 1234)
			nArrow-=nDesp;				// Mover flecha
			while(nArrow > 4)
				nArrow-=4;
			while(nArrow < 0)
				nArrow+=4;
			if(nArrow > 1) nArrow++;	// Volver a liberar la flecha (1234 a 1245)
		}
		RespAr[x] = nArrow+nFirst;
	}
	nDis = ARROWSLINE_SINGLE;
	for(int i = (nArg1+1); i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = RespAr[i-nArg1-1]/ARROWSLINE_SINGLE;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		nDisAr[0] = RespAr[i-nArg1-1]-nFirst;
		bsc.nBSAnswers[nInd*nArrowsPerLine+RespAr[i-nArg1-1]] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+RespAr[i-nArg1-1]] = MAKELONG(17,1);
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%ARROWSLINE_SINGLE;
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = MAKELONG(17,1);
		}
	}
	delete RespAr;
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQMath1(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	bool bRight = (rand()%2)?true:false;
	int nMode = rand()%3;	// 0 =, 1 <, 2 >
	if(nArg1 <= nArg2 || nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, Arg2 = %d, estos dos no pueden ser iguales, ni el segundo puede ser mayor que el primero, o el segundo ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg1, nArg2);
		_printf(str); return;
	}

	int nResult = 0;
	int nNumber = 0;
	int nOper = 0;	// 0 +, 1 -, 2 x, 3 /
	bool bCond = true;
	while(bCond)
	{
		nCarry = 0;
		nResult = 0;
		for(int a = 0; a < nArg2; a++)
		{
			if(a == 0)
			{
				nOper = 0;
				nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
			}
			else
			{
				int nOperPer[] = {0,1,2,3};
				if(nResult <= 0) nOperPer[3] = rand()%2;
				if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
				if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
				if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
				nOper = nOperPer[rand()%4];
				// Ahora el siguiente numero
				// tener cuidado, cuando es division
				// debe ser dividido por algun Dividendo del numero
				int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
				int nPer[20];
				for(int q = 0; q < 20; q++)
				{
					nPer[q] = q+1;
				}
				if(nOper == 3)	// Division, tener cuidado
				{
					// Encontrar los divisores del numero anterior
					int nCountDiv = 0;
					for(int q = 1; q <= nResult; q++)
					{
						int nDiv = nResult%q;
						if(nDiv == 0)	// Este numero es divisible
						{
							nPer[nCountDiv] = q;
							nCountDiv++;
						}
					}
					nSizePer = nCountDiv;
				}
				nNumber = nPer[rand()%nSizePer];
			}
			switch(nOper)
			{
			case 0: // Suma
				nResult += nNumber;
				break;
			case 1: // Resta
				nResult -= nNumber;
				break;
			case 2: // Multiplicacion
				nResult *= nNumber;
				break;
			case 3: // Division
				nResult /= nNumber;
				break;
			}
			if(a != 0)
			{
				bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
				nCarry++;
			}
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
			nCarry++;
		}
		bCond = nResult >= 20 || nResult < 0;
		if(nMode == 1) bCond = nResult >= 20 || nResult <= 0;
	}
	int nBackResult = nResult;
	nResult = 0;
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nMode+14, 1);
	nCarry++;
	int nArg3 = nArg1-nArg2;
	for(int a = 0; a < nArg3; a++)
	{
		if(a == 0)
		{
			nOper = 0;
			nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
		}
		else if(a != (nArg3-1))
		{
			int nOperPer[] = {0,1,2,3};
			if(nResult <= 0) nOperPer[3] = rand()%2;
			if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
			if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
			if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
			nOper = nOperPer[rand()%4];

			int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
			int nPer[20];
			for(int q = 0; q < 20; q++)
			{
				nPer[q] = q+1;
			}
			if(nOper == 3)	// Division, tener cuidado
			{
				// Encontrar los divisores del numero anterior
				int nCountDiv = 0;
				for(int q = 1; q <= nResult; q++)
				{
					int nDiv = nResult%q;
					if(nDiv == 0)	// Este numero es divisible
					{
						nPer[nCountDiv] = q;
						nCountDiv++;
					}
				}
				nSizePer = nCountDiv;
			}
			nNumber = nPer[rand()%nSizePer];
		}
		if(a == (nArg3-1) && bRight)
		{
			int nLeft = nBackResult - nResult;
			if(abs(nLeft) != nLeft) nOper = 1;
			else nOper = 0;
			if(nLeft == 0) nNumber = 0;
			else
			{
				if(nMode == 0)	// Igual
					nNumber = abs(nLeft);
				if(nMode == 1)	// Menor que
				{
					if(nOper == 0)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
				if(nMode == 2)	// Mayor que
				{
					if(nOper == 1)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
			}
		}
		if(a == (nArg3-1) && !bRight)
		{
			int nLeft = nBackResult - nResult;
			if(abs(nLeft) != nLeft) nOper = 1;
			else nOper = 0;
			if(nLeft == 0)
			{
				nNumber = rand()%38-19;
				if(nNumber < 0) nOper = 1;
				else nOper = 0;
			}
			else
			{
				if(nMode == 0)	// Igual
				{
					do
					{
						nNumber = rand()%19;
					}
					while(nNumber == abs(nLeft));
				}
				if(nMode == 2)	// Mayor que
				{
					if(nOper == 0)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
				if(nMode == 1)	// Menor que
				{
					if(nOper == 1)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
			}
		}
		switch(nOper)
		{
		case 0: // Suma
			nResult += nNumber;
			break;
		case 1: // Resta
			nResult -= nNumber;
			break;
		case 2: // Multiplicacion
			nResult *= nNumber;
			break;
		case 3: // Division
			nResult /= nNumber;
			break;
		}
		if(a != 0)
		{
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
			nCarry++;
		}
		bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
		nCarry++;
	}
	bsc.dWIBSQuestions[nIndex] = pDoc->BeatArrowTime(y);
	bsc.dwBSQuestions[nIndex] = 10;
	// ** Esto es verdadero y falso, por lo tanto solo deben haber 2 respuestas maximo
	// Diseñar la respuesta
	nDis = 2;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
		}
		int nCaretSizeCoord = 0;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x]) nCaretSizeCoord++;
		}

		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		// Ahora... cual va a ser la verdadera?
		if(nDis != 0) nRight = rand()%(nDis);
		else nRight = 0;
		int nCarCount = 0;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x] == 1)
			{
				if(nRight == nCarCount)	// Aqui va la correcta
				{
					bsc.nBSAnswers[nInd*nArrowsPerLine+x] = 1;
					bsc.dwBSAnswers[nInd*nArrowsPerLine+x] = (DWORD)MAKELONG((int)(bRight), 8);
				}
				else			// Esto es un distractor
				{
					bsc.nBSAnswers[nInd*nArrowsPerLine+x] = 0;
					bsc.dwBSAnswers[nInd*nArrowsPerLine+x] = (DWORD)MAKELONG((int)(!bRight), 8);
				}
				nCarCount++;
			}
			if(nCarCount >= nDis) break;
		}
	}
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQMath2(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg1 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, no puede ser igual a 0 (debe haber al menos un operando)\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg1);
		_printf(str); return;
	}

	int nResult = 0;
	int nNumber = 0;
	int nOper = 0;	// 0 +, 1 -, 2 x, 3 /
	bool bCond = true;
	while(bCond)
	{
		nCarry = 0;
		nResult = 0;
		for(int a = 0; a < nArg1; a++)
		{
			if(a == 0)
			{
				nOper = 0;
				nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
			}
			else
			{
				int nOperPer[] = {0,1,2,3};
				if(nResult <= 0) nOperPer[3] = rand()%2;
				if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
				if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
				if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
				nOper = nOperPer[rand()%4];
				// Ahora el siguiente numero
				// tener cuidado, cuando es division
				// debe ser dividido por algun Dividendo del numero
				int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
				int nPer[20];
				for(int q = 0; q < 20; q++)
				{
					nPer[q] = q+1;
				}
				if(nOper == 3)	// Division, tener cuidado
				{
					// Encontrar los divisores del numero anterior
					int nCountDiv = 0;
					for(int q = 1; q <= nResult; q++)
					{
						int nDiv = nResult%q;
						if(nDiv == 0)	// Este numero es divisible
						{
							nPer[nCountDiv] = q;
							nCountDiv++;
						}
					}
					nSizePer = nCountDiv;
				}
				nNumber = nPer[rand()%nSizePer];
			}
			switch(nOper)
			{
			case 0: // Suma
				nResult += nNumber;
				break;
			case 1: // Resta
				nResult -= nNumber;
				break;
			case 2: // Multiplicacion
				nResult *= nNumber;
				break;
			case 3: // Division
				nResult /= nNumber;
				break;
			}
			if(a != 0)
			{
				bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
				nCarry++;
			}
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
			nCarry++;
		}
		bCond = nResult >= nArg2 || nResult < 0;
	}
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(14, 1);
	nCarry++;
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(17, 1);
	nCarry++;

	bsc.dWIBSQuestions[nIndex] = pDoc->BeatArrowTime(y);
	bsc.dwBSQuestions[nIndex] = 11;

	// ** Esto es encontrar la respuesta, por lo tanto solo deben haber 5 respuestas maximo
	// Diseñar la respuesta
	nDis = 5;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
		}
		int nCaretSizeCoord = 0;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x]) nCaretSizeCoord++;
		}

		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		// Ahora... cual va a ser la verdadera?
		if(nDis != 0) nRight = rand()%(nDis);
		else nRight = 0;
		int nCarCount = 0;
		int nCarryDis = 0;
		int nDistrac[ARROWSLINE_DOUBLE];
		for(int x = 0; x < ARROWSLINE_DOUBLE; x++) nDistrac[x] = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x] == 1)
			{
				if(nRight == nCarCount)	// Aqui va la correcta
				{
					bsc.nBSAnswers[nInd*nArrowsPerLine+x] = 1;
					bsc.dwBSAnswers[nInd*nArrowsPerLine+x] = (DWORD)MAKELONG(nResult, 1);
				}
				else			// Esto es un distractor
				{
					bsc.nBSAnswers[nInd*nArrowsPerLine+x] = 0;
					nNumber = rand()%max(10, nArg2);
					for(int t = 0; t < nArrowsPerLine; t++)
					{
						if(nNumber == nDistrac[t] || nNumber == nResult)
						{
							t = 0;
							nNumber = rand()%max(10, nArg2);
						}
					}
					nDistrac[nCarryDis] = nNumber;
					nCarryDis++;
					bsc.dwBSAnswers[nInd*nArrowsPerLine+x] = (DWORD)MAKELONG(nNumber, 1);
				}
				nCarCount++;
			}
			if(nCarCount >= nDis) break;
		}
	}
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQMath3(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg1 <= 1)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, no puede ser menor que 1 (debe haber al menos un operando para juzgar)\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg1);
		_printf(str); return;
	}

	int nResult = 0;
	int nNumber = 0;
	int nOper = 0;	// 0 +, 1 -, 2 x, 3 /
	int nOperSur = 0;
	int nOperDir = 0;
	if(nArg1 > 1)
		nOperSur = rand()%(nArg1-1);
	bool bCond = true;
	while(bCond)
	{
		nCarry = 0;
		nResult = 0;
		for(int a = 0; a < nArg1; a++)
		{
			if(a == 0)
			{
				nOper = 0;
				nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
			}
			else
			{
				int nOperPer[] = {0,1,2,3};
				if(nResult <= 0) nOperPer[3] = rand()%2;
				if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
				if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
				if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
				nOper = nOperPer[rand()%4];
				// Ahora el siguiente numero
				// tener cuidado, cuando es division
				// debe ser dividido por algun Dividendo del numero
				int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
				int nPer[20];
				for(int q = 0; q < 20; q++)
				{
					nPer[q] = q+1;
				}
				if(nOper == 3)	// Division, tener cuidado
				{
					// Encontrar los divisores del numero anterior
					int nCountDiv = 0;
					for(int q = 1; q <= nResult; q++)
					{
						int nDiv = nResult%q;
						if(nDiv == 0)	// Este numero es divisible
						{
							nPer[nCountDiv] = q;
							nCountDiv++;
						}
					}
					nSizePer = nCountDiv;
				}
				nNumber = nPer[rand()%nSizePer];
			}
			switch(nOper)
			{
			case 0: // Suma
				nResult += nNumber;
				break;
			case 1: // Resta
				nResult -= nNumber;
				break;
			case 2: // Multiplicacion
				nResult *= nNumber;
				break;
			case 3: // Division
				nResult /= nNumber;
				break;
			}
			if(a != 0)
			{
				if((a-1)==nOperSur)
				{
					bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(17, 1);
					nOperDir = nOper;
				}
				else
					bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
				nCarry++;
			}
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
			nCarry++;
		}
		bCond = nResult >= nArg2 || nResult < 0;
	}
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(14, 1);
	nCarry++;
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nResult, 1);
	nCarry++;

	bsc.dWIBSQuestions[nIndex] = pDoc->BeatArrowTime(y);
	bsc.dwBSQuestions[nIndex] = 12;

	// ** Esto es encontrar el operando perdido, por lo tanto solo deben haber 4 respuestas maximo
	// Diseñar la respuesta
	nDis = 4;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
		}
		int nCaretSizeCoord = 0;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x]) nCaretSizeCoord++;
		}

		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		// Ahora... cual va a ser la verdadera?
		if(nDis != 0) nRight = rand()%(nDis);
		else nRight = 0;
		int nCarCount = 0;
		int nOpers[] = {0, 1, 2, 3};
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x] == 1)
			{
				if(nRight == nCarCount)	// Aqui va la correcta
				{
					bsc.nBSAnswers[nInd*nArrowsPerLine+x] = 1;
					bsc.dwBSAnswers[nInd*nArrowsPerLine+x] = (DWORD)MAKELONG(nOperDir+10, 1);
					nOpers[nOperDir] = -1;
				}
				else			// Esto es un distractor
				{
					bsc.nBSAnswers[nInd*nArrowsPerLine+x] = 0;
					do
					{
						nOper = nOpers[rand()%4];
					}
					while (nOper == -1 || nOper == nOperDir);
					nOpers[nOper] = -1;
					bsc.dwBSAnswers[nInd*nArrowsPerLine+x] = (DWORD)MAKELONG(nOper+10, 1);
				}
				nCarCount++;
			}
			if(nCarCount >= nDis) break;
		}
	}
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQMath4(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	if(nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg2 = %d, Este argumento no puede ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg2);
		_printf(str); return;
	}
	int nSum = min(4, nArg2);
	nSum = max(1, nSum);
	int nMin = 0;
	int nMax = 20;
	int nResults[ARROWSLINE_SINGLE];
	for(int x = 0; x < ARROWSLINE_SINGLE; x++) nResults[x] = -1;
	if(nArg1)
	{
		int nCar = 0;
		for(int x = 0; x < ARROWSLINE_SINGLE; x++)
		{
			nResults[x] = (rand()%(17+x-nCar)) + nCar;
			nCar = nResults[x]+1;
		}
	}
	else
	{
		int nCar = 20;
		for(int x = 0; x < ARROWSLINE_SINGLE; x++)
		{
			nResults[x] = nCar - (rand()%(nCar-3+x));
			nCar = nResults[x]-1;
		}
	}
	int nCaretOpt[ARROWSLINE_SINGLE];
	int nCaretOrd[ARROWSLINE_SINGLE];
	for(int x = 0; x < ARROWSLINE_SINGLE; x++)
	{
		nCaretOpt[x] = x;
		nCaretOrd[x] = 0;
	}
	for(int x = 0; x < ARROWSLINE_SINGLE; x++)
	{
		int nSumand[] = {0,0,0,0};
		int nSizeSum = (rand()%nSum);
		int nResult = 0;
		int nIndResp;
		do
		{
			nIndResp = nCaretOpt[rand()%ARROWSLINE_SINGLE];
		} while(nIndResp == -1);
		nCaretOpt[nIndResp] = -1;
		nCaretOrd[x] = nIndResp;
		for(int z = 0; z <= nSizeSum; z++)
		{
			if(z != nSizeSum) nSumand[z] = (rand()%(nResults[x]-nResult+1));
			else nSumand[z] = nResults[x]-nResult;
			nResult+=nSumand[z];
			bsc.dwDBSQuestionsEx[nIndex*40+nIndResp*4+z] = MAKELONG(nSumand[z], 1);
		}
		bsc.dwDBSQuestionsEx[nIndex*40+20+x] = (rand()%3)+1;
	}
	bsc.dWIBSQuestions[nIndex] = pDoc->BeatArrowTime(y);
	bsc.dwBSQuestions[nIndex] = 13;
	// ** Esto es juzgar terminos, por lo tanto solo deben haber 3 respuestas maximo
	// Diseñar la respuesta
	nDis = ARROWSLINE_SINGLE;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
		}
		int nCaretSizeCoord = 0;
		int nFirst = -1;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x])
			{
				if(nFirst == -1) nFirst = x;
				nCaretSizeCoord++;
			}
		}
		nFirst = nFirst/ARROWSLINE_SINGLE*ARROWSLINE_SINGLE;
		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		int nDisAr[ARROWSLINE_SINGLE];
		for(int x = 0; x < ARROWSLINE_SINGLE; x++) nDisAr[x] = -1;
		nDisAr[0] = nCaretOrd[i];
		bsc.nBSAnswers[nInd*nArrowsPerLine+nCaretOrd[i]+nFirst] = 1;
		bsc.dwBSAnswers[nInd*nArrowsPerLine+nCaretOrd[i]+nFirst] = MAKELONG(17, 1);
		bsc.dwDBSQuestionsEx[nIndex*40+25+nCaretOrd[i]] = (DWORD)bsc.nWWBSAnswers[nInd];
		for(int x = 1; x < nDis; x++)
		{
			bool bID = true;
			int nDisp;
			do
			{
				nDisp = rand()%ARROWSLINE_SINGLE;
				bID = false;
				for(int t = 0; t < ARROWSLINE_SINGLE; t++)
					if(nDisp == nDisAr[t])
						bID = true;
			} while (bID);
			nDisAr[x] = nDisp;
			bsc.nBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = 0;
			bsc.dwBSAnswers[nInd*nArrowsPerLine+nDisp+nFirst] = MAKELONG(17, 1);
		}
	}
	delete nCoord;
	delete nArrayCoord;
}

void CMaxStepSmu::GQMath5(int y, int nIndex, int nResp, int nArg1, int nArg2)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	int nCarry = 0;
	int nDis = 1;
	int nRight = 0;
	// Encontrar las respuestas que encajen
	int *nCoord = new int[nResp];
	int *nArrayCoord = new int[nResp*nArrowsPerLine];
	for(int x = 0; x < nResp; x++)
	{
		nCoord[x] = 0;
		for(int k = 0; k < nArrowsPerLine; k++) nArrayCoord[x*nArrowsPerLine+k] = 0;
	}
	int nCount = 0;
	for(int i = y+1; i < pDoc->song.hStepGroup[0].hGroup.nSizeArrows; i++)
	{
		if(pDoc->SomeBSAnswer(i, 0))
		{
			nCoord[nCount] = i;
			for(int x = 0; x < nArrowsPerLine; x++)
			{
				if(pDoc->GetArrow(x, i, 0) == 22)
				{
					nArrayCoord[nCount*nArrowsPerLine+x] = 1;
				}
			}
			nCount++;
			if(nCount >= nResp) break;
		}
		if(pDoc->SomeBSQuestion(i, 0)) break;
	}
	if(nCount != nResp)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nIncongruencia de datos\nSe supone que para la pregunta ubicada en el Beat %g (Measure %d) debe haber %d respuestas, pero solo se encontraron %d respuestas antes de la siguiente pregunta\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nResp, nCount);
		_printf(str); return;
	}
	// Diseñar la pregunta
	int nMode = rand()%3;	// 0 =, 1 <, 2 >
	if(nArg1 <= nArg2 || nArg2 == 0)
	{
		CString str;
		str.Format(_T("Ha salido un error:\nError de sintaxis\nEn la pregunta ubicada en el Beat %g (Measure %d), Arg1 = %d, Arg2 = %d, estos dos no pueden ser iguales, ni el segundo puede ser mayor que el primero, o el segundo ser igual a 0\n\nNo se garantiza el funcionamiento correcto de Brain Shower"), pDoc->BeatArrowEdit(y), (int)(pDoc->BeatArrowEdit(y)/4), nArg1, nArg2);
		_printf(str); return;
	}

	int nResult = 0;
	int nNumber = 0;
	int nOper = 0;	// 0 +, 1 -, 2 x, 3 /
	bool bCond = true;
	while(bCond)
	{
		nCarry = 0;
		nResult = 0;
		for(int a = 0; a < nArg2; a++)
		{
			if(a == 0)
			{
				nOper = 0;
				nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
			}
			else
			{
				int nOperPer[] = {0,1,2,3};
				if(nResult <= 0) nOperPer[3] = rand()%2;
				if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
				if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
				if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
				nOper = nOperPer[rand()%4];
				// Ahora el siguiente numero
				// tener cuidado, cuando es division
				// debe ser dividido por algun Dividendo del numero
				int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
				int nPer[20];
				for(int q = 0; q < 20; q++)
				{
					nPer[q] = q+1;
				}
				if(nOper == 3)	// Division, tener cuidado
				{
					// Encontrar los divisores del numero anterior
					int nCountDiv = 0;
					for(int q = 1; q <= nResult; q++)
					{
						int nDiv = nResult%q;
						if(nDiv == 0)	// Este numero es divisible
						{
							nPer[nCountDiv] = q;
							nCountDiv++;
						}
					}
					nSizePer = nCountDiv;
				}
				nNumber = nPer[rand()%nSizePer];
			}
			switch(nOper)
			{
			case 0: // Suma
				nResult += nNumber;
				break;
			case 1: // Resta
				nResult -= nNumber;
				break;
			case 2: // Multiplicacion
				nResult *= nNumber;
				break;
			case 3: // Division
				nResult /= nNumber;
				break;
			}
			if(a != 0)
			{
				bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
				nCarry++;
			}
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
			nCarry++;
		}
		bCond = nResult >= 20 || nResult < 0;
		if(nMode == 1) bCond = nResult >= 20 || nResult <= 0;
	}
	int nBackResult = nResult;
	nResult = 0;
	bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(17, 1);
	nCarry++;
	int nArg3 = nArg1-nArg2;
	for(int a = 0; a < nArg3; a++)
	{
		if(a == 0)
		{
			nOper = 0;
			nNumber = rand()%((nArg1*3)>20?19:(nArg1*3));
		}
		else if(a != (nArg3-1))
		{
			int nOperPer[] = {0,1,2,3};
			if(nResult <= 0) nOperPer[3] = rand()%2;
			if(nResult <= -10 || nResult >= 10) nOperPer[2] = rand()%2;
			if(nResult <= -20) for(int t = 0; t < 4; t++) nOperPer[t] = 0;
			if(nResult >= 20) for(int t = 0; t < 4; t++) nOperPer[t] = 1;
			nOper = nOperPer[rand()%4];

			int nSizePer = (nArg1*3+1)>20?20:(nArg1*3+1);
			int nPer[20];
			for(int q = 0; q < 20; q++)
			{
				nPer[q] = q+1;
			}
			if(nOper == 3)	// Division, tener cuidado
			{
				// Encontrar los divisores del numero anterior
				int nCountDiv = 0;
				for(int q = 1; q <= nNumber; q++)
				{
					int nDiv = nNumber%q;
					if(nDiv == 0)	// Este numero es divisible
					{
						nPer[nCountDiv] = q;
						nCountDiv++;
					}
				}
				nSizePer = nCountDiv;
			}
			nNumber = nPer[rand()%nSizePer];
		}
		if(a == (nArg3-1))
		{
			int nLeft = nBackResult - nResult;
			if(abs(nLeft) != nLeft) nOper = 1;
			else nOper = 0;
			if(nLeft == 0) nNumber = 0;
			else
			{
				if(nMode == 0)	// Igual
					nNumber = abs(nLeft);
				if(nMode == 1)	// Menor que
				{
					if(nOper == 0)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
				if(nMode == 2)	// Mayor que
				{
					if(nOper == 1)
					{
						nNumber = rand()%10;
						nNumber += abs(nLeft);
					}
					else
					{
						nNumber = rand()%abs(nLeft);
					}
				}
			}
		}
		switch(nOper)
		{
		case 0: // Suma
			nResult += nNumber;
			break;
		case 1: // Resta
			nResult -= nNumber;
			break;
		case 2: // Multiplicacion
			nResult *= nNumber;
			break;
		case 3: // Division
			nResult /= nNumber;
			break;
		}
		if(a != 0)
		{
			bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nOper+10, 1);
			nCarry++;
		}
		bsc.dwDBSQuestions[nIndex*30+nCarry] = (DWORD)MAKELONG(nNumber, 1);
		nCarry++;
	}
	bsc.dWIBSQuestions[nIndex] = pDoc->BeatArrowTime(y);
	bsc.dwBSQuestions[nIndex] = 14;
	// ** Esto es juzgar terminos, por lo tanto solo deben haber 3 respuestas maximo
	// Diseñar la respuesta
	nDis = 3;
	for(int i = 0; i < nCount; i++)
	{
		// Codigo estandar de Sezizeo para las respuestas
		int nInd = IncrBSAnswer(nCoord[i]);
		if(i == (nCount-1))
		{
			bsc.dWEBSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
			bsc.dWABSQuestions[nIndex] = pDoc->BeatArrowTime(nCoord[i]);
		}
		int nCaretSizeCoord = 0;
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x]) nCaretSizeCoord++;
		}

		nDis = nCaretSizeCoord>nDis?nDis:nCaretSizeCoord;
		// Ahora... cual va a ser la verdadera?
		if(nDis != 0) nRight = rand()%(nDis);
		else nRight = 0;
		int nCarCount = 0;
		int nModes[] = {0,1,2};
		for(int x = 0; x < nArrowsPerLine; x++)
		{
			if(nArrayCoord[i*nArrowsPerLine+x] == 1)
			{
				if(nRight == nCarCount)	// Aqui va la correcta
				{
					bsc.nBSAnswers[nInd*nArrowsPerLine+x] = 1;
					bsc.dwBSAnswers[nInd*nArrowsPerLine+x] = (DWORD)MAKELONG(nMode+14, 1);
					nModes[nMode] = -1;
				}
				else			// Esto es un distractor
				{
					bsc.nBSAnswers[nInd*nArrowsPerLine+x] = 0;
					int nCMode;
					do
					{
						nCMode = nModes[rand()%3];
					}
					while(nCMode == -1 || nMode == nCMode);
					nModes[nCMode] = -1;
					bsc.dwBSAnswers[nInd*nArrowsPerLine+x] = (DWORD)MAKELONG(nCMode+14, 1);
				}
				nCarCount++;
			}
			if(nCarCount >= nDis) break;
		}
	}
	delete nCoord;
	delete nArrayCoord;
}

int CMaxStepSmu::IncrBSAnswer(int y)
{
	int nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;
	bsc.nSizeBSAnswers++;
	bsc.dWBSAnswers = (float*)max_realloc(bsc.dWBSAnswers, bsc.nSizeBSAnswers*sizeof(float));
	bsc.nWWBSAnswers = (__int64*)max_realloc(bsc.nWWBSAnswers, bsc.nSizeBSAnswers*sizeof(__int64));
	bsc.dwBSAnswers = (DWORD*)max_realloc(bsc.dwBSAnswers, bsc.nSizeBSAnswers*nArrowsPerLine*sizeof(DWORD));
	bsc.nBSAnswers = (int*)max_realloc(bsc.nBSAnswers, bsc.nSizeBSAnswers*nArrowsPerLine*sizeof(int));
	bsc.nDBSAnswers = (int*)max_realloc(bsc.nDBSAnswers, bsc.nSizeBSAnswers*sizeof(int));
	int nInd = bsc.nSizeBSAnswers-1;
	bsc.dWBSAnswers[bsc.nSizeBSAnswers-1] = 0.0;
	bsc.nWWBSAnswers[bsc.nSizeBSAnswers-1] = 0;
	bsc.nDBSAnswers[bsc.nSizeBSAnswers-1] = 0;
	for(int x = 0; x < nArrowsPerLine; x++)
	{
		bsc.dwBSAnswers[(bsc.nSizeBSAnswers-1)*nArrowsPerLine+x]=0;
		bsc.nBSAnswers[(bsc.nSizeBSAnswers-1)*nArrowsPerLine+x]=0;
	}

	bsc.dWBSAnswers[nInd] = pDoc->BeatArrowDraw(y);
	bsc.nWWBSAnswers[nInd] = BeatToTime(pDoc->BeatArrowTime(y));
	return nInd;
}
