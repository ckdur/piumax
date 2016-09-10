#include "MaxStepSmu.h"

void CMaxStepSmu::Draw()
{
	for(int i = 0; i < stepc.hStep.nSizeArrowGroups; i++)
	{
		skin[i]->Update(dwTick - dwLastTick);
	}
	common->Update(dwTick - dwLastTick);
	bs->Update(dwTick - dwLastTick);

	// ** Timing

	static float sub = 0.0f;
	float fAdd = (float(dwTick - dwLastTick)*m_fRush)+sub;
	sub = fAdd - (float)((__int64)fAdd);
	if (!m_bIsMusic) m_nTime += (__int64)fAdd;
	else //m_nTime = (__int64)m_nPos + (__int64)stepc.hStep.nOffset;
	{
		m_nTime += (__int64)fAdd;
        if(abs(m_nTime-((__int64)m_nPos + (__int64)stepc.hStep.nOffset)) > 100)
			m_nTime = (__int64)m_nPos + (__int64)stepc.hStep.nOffset;
	}
	m_dBeatTime = TimeToBeat(m_nTime);
	m_dBeatDraw = BeatT2BeatD(m_dBeatTime);
	//////////////////////////////

	// Actualizar velocidad
	UpdateVel(m_dBeatTime);

	if(evarrows.bVel && m_dBeatDraw >= evarrows.dWWVel)
	{
		if(m_dBeatDraw >= (evarrows.dWWVel+1.0))
			m_dUserVel = evarrows.dDestVel;
		else
			m_dUserVel = (evarrows.dDestVel-evarrows.dSourceVel)*(m_dBeatDraw-evarrows.dWWVel)+evarrows.dSourceVel;
	}

	m_dPassVel = 1.0;
	if(m_bEW)
	{
		int nPart = (int)m_dBeatTime;
		m_dPassVel = abs(m_dBeatTime-(float)nPart-0.5f)+0.5f;
	}

	if(m_bRV)
	{
		int nPart = (int)m_dBeatTime;
		nPart = nPart%RV_BEATS_EVENT;
		if(nPart == 0)
		{
			evarrows.dSourceVel = m_dUserVel;
			evarrows.dDestVel = (float)(rand()%(RV_MAX_VEL-RV_MIN_VEL+1)+RV_MIN_VEL);
			evarrows.dWWVel = m_dBeatDraw;
			evarrows.bVel = true;
		}
	}

	///////////////////////

	// Asist Tick
	DoAssistTick();

	int nHlp = m_nHlpAuto;
	if(m_bAutoPlay)
	{
		/*if(m_bCharacter)
		{
			for(int s = nHlp; s < crt.hCtr.nSizeArrowsAP; s++)
			{
				if(crt.dWWArrowsAP[s] <= m_dBeatDraw)
				{
					int nArr = LOWORD(crt.dwArrowsAP[s]);
					int nType = HIWORD(crt.dwArrowsAP[s]);
					if(nType == 1)
					{
						evarrows.nArrows[nArr] = m_nTime;
					}
					if(nType == 3)
					{
						evarrows.nArrows[nArr] = m_nTime;
						evarrows.bArrows[nArr] = true;
					}
					m_nHlpAuto = s+1;
				}
				else break;
			}
		}
		else*/
		if(stepc.hStep.nSizeArrowGroups > 0)
		{
			for(int s = nHlp; s < stepc.hStepGroup[0].hGroup.nSizeArrows; s++)
			{
				if(stepc.hStepGroup[0].nWTArrows[s] <= m_nTime)
				{
					for(int t = 0; t < stepc.hStep.nArrowsPerLine; t++)
					{
						if(stepc.hStepGroup[0].nArrows[s*stepc.hStep.nArrowsPerLine+t] == 1)
						{
							evarrows.nArrows[t] = m_nTime;
						}
						if(stepc.hStepGroup[0].nArrows[s*stepc.hStep.nArrowsPerLine+t] == 2)
						{
							evarrows.nArrows[t] = m_nTime;
							evarrows.bArrows[t] = true;
						}
						if(stepc.hStepGroup[0].nArrows[s*stepc.hStep.nArrowsPerLine+t] == 4)
						{
							//evarrows.nArrows[t] = m_nTime;
							evarrows.bArrows[t] = true;
						}
					}
					m_nHlpAuto = s+1;
				}
				else break;
			}
		}
	}

	double dOpMult = 1.0, dOpLen = VANISH_TRANSITION;

	// FLASH declaration
	if(m_bFlash)
	{
		int nPart = (int)m_dBeatTime;
		double ko = abs(m_dBeatTime-(double)nPart-0.5)*2;
		dOpMult = min(dOpMult, ko);
	}

	int* nMin = new int[stepc.hStep.nSizeArrowGroups], *nMax = new int[stepc.hStep.nSizeArrowGroups];
	for(int m = 0; m < stepc.hStep.nSizeArrowGroups; m++)
	{
		GetLimitsOfDrawing(m_dBeatDraw, m, &nMin[m], &nMax[m]);
		nMin[m] = max(nMin[m], 0);
		nMax[m] = max(nMax[m], 0);
	}

	m_dSizeArrows = float(SIZE_ARROWS);
	m_dDistArrows = float(DIST_ARROWS);
	double m_nDistArrows = 60.0/8.0;
	int nOrder[] = {0,4,1,3,2,5,9,6,8,7};
	int nLimitX = m_nModePlay==MODE_PLAYER_SINGLE?ARROWSLINE_SINGLE:ARROWSLINE_DOUBLE;
	int nLimitXA = min(nLimitX, stepc.hStep.nArrowsPerLine);


	g_gmMain->MatrixMode(PMG_PROJECTION);
	g_gmMain->PushMatrix();
	g_gmMain->MatrixMode(PMG_MODELVIEW);
	g_gmMain->PushMatrix();
	// ** Transformaciones aqui!

	if(m_bDrop) g_gmMain->Scale(1.0, -1.0, 1.0);
	if(m_bUnderAttack) g_gmMain->Rotate(180.0, 0.0, 0.0, 1.0);
	if(m_bLeftAttack && m_nModePlay==MODE_PLAYER_SINGLE)
	{
		g_gmMain->Rotate(90.0, 0.0, 0.0, 1.0);
		if(!m_nModeMach) g_gmMain->Translate(1.4*m_dSizeArrows+10.0, 0.0, 0.0);
	}
	if(m_bRightAttack && m_nModePlay==MODE_PLAYER_SINGLE)
	{
		g_gmMain->Rotate(270.0, 0.0, 0.0, 1.0);
		if(!m_nModeMach) g_gmMain->Translate(1.4*m_dSizeArrows+10.0, 0.0, 0.0);
	}

	if(m_bNXMode)
	{
		g_gmMain->MatrixMode(PMG_PROJECTION);
		g_gmMain->LoadIdentity();

		g_gmMain->Frustum(-15.0,15.0,-15.0,15.0,30.0,200.0);

		g_gmMain->MatrixMode(PMG_MODELVIEW);
		g_gmMain->LoadIdentity();
		g_gmMain->Translate(0.0, 30.0, -60.0);
		g_gmMain->Rotate(-60.0, 1.0, 0.0, 0.0);
	}
	// USER: Specify it externally with m_nModeMach
	if(m_nModePlay==MODE_PLAYER_SINGLE && !m_nModeMach) g_gmMain->Translate(TRANSLATE_INITIAL_PLAYER, 0.0, 0.0);
	if(m_nPlayer != 1 && m_nModePlay==MODE_PLAYER_SINGLE) g_gmMain->Translate(TRANSLATE_NEXT_PLAYER*double(m_nPlayer-1), 0.0, 0.0);

	////////////////////////////

	// Calculate beat order
	double o = pow(1+floor(m_dBeatTime)-m_dBeatTime, double(5));	// USER: THIS MAYBE IS WRONG

	// USER: MAKE A UPDATEATTACK METHOD

	// USER: WARNING FOR REMPLACING SetOffset Method
	g_gmMain->Translate(0.0, TRANSLATE_Y_PLAYER, 0.0);

	g_gmMain->BlendFunc(PMG_SRC_ALPHA,PMG_ONE_MINUS_SRC_ALPHA);
	if(!m_bFreedom)
	{
		DrawArrowGuide(nLimitX, nOrder, o);
	}

	int nLevelMach = m_nJudTimes[0];	// USER: DIRTY?
	for( int n = 0; n < m_nSizeJudTimes; n++)
	{
		if(nLevelMach < m_nJudTimes[n] && m_nJudTimes[n] > 0)
		{
			nLevelMach = m_nJudTimes[n];
		}
	}

	StepEvaluate(nLimitXA, nMin, nMax, nLevelMach);


	g_gmMain->BlendFunc(PMG_SRC_ALPHA,PMG_ONE_MINUS_SRC_ALPHA);

	double dYOpSup = TOP_ARROWS, dYOpInf = BOTTOM_ARROWS;

	// USER: drawing i go here
	for( int n = 0; n < stepc.hStep.nSizeArrowGroups; n++)
	{
		// VANISH declaration
		if(stepc.hStepGroup[n].hGroup.nIsVanishSup)
			dYOpSup = stepc.hStepGroup[n].hGroup.fVanishSup/100.0f*(BOTTOM_ARROWS-TOP_ARROWS)+TOP_ARROWS;
		else if(m_fEVanish!=0.0f)
			dYOpSup = m_fEVanish/100.0f*(BOTTOM_ARROWS-TOP_ARROWS)+TOP_ARROWS;

		if(stepc.hStepGroup[n].hGroup.nIsVanishInf)
			dYOpInf = stepc.hStepGroup[n].hGroup.fVanishSup/100.0f*(TOP_ARROWS-BOTTOM_ARROWS)+BOTTOM_ARROWS;
		else if(m_fIVanish!=0.0f)
			dYOpInf = m_fIVanish/100.0f*(TOP_ARROWS-BOTTOM_ARROWS)+BOTTOM_ARROWS;

		for(int i = 0; i < nLimitXA; i++)
		{
			int x = nOrder[i];

			bool bDrawingHold = false;
			for(int y = nMin[n]; y < nMax[n]; y++)
			{
				int nSkin = 0;
				int nArrow = GetArrow(x, y, n);
				int nStatus = GetArrowStatus(x, y, n);
				float dBeat = m_dBeatDraw-float(stepc.hStepGroup[n].fWDArrows[y]);
				float nUbi = UbiXArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, dBeat);
				float dUbi = UbiYArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, dBeat);
				if((nArrow != 3 && nArrow != 4 && nArrow != 0) || (nArrow == 3 && nStatus < JUDGAMENT_DRAW_LOSE && nStatus >= 0))
				{
					bDrawingHold = false;
				}
				if(nArrow == 1 && (nStatus >= JUDGAMENT_DRAW_LOSE || nStatus < 0 ||
					(nStatus < JUDGAMENT_DRAW_LOSE && dUbi < 0 && nStatus >= 0) ) )
				{
					DrawArrowOp(nUbi, dUbi, ZOFFSET_ARROWS, skin[n]->dScale, 0.0,
						dYOpSup, dYOpInf, dOpLen, dOpMult, skin[n]->skin[x%ARROWSLINE_SINGLE+SKIN_TDOWNLEFT]->GetCurrentTex());
				}
				if(nArrow >= 5 && nArrow <= 20 && (nStatus >= JUDGAMENT_DRAW_LOSE || nStatus < 0 ||
					(nStatus < JUDGAMENT_DRAW_LOSE && dUbi < 0 && nStatus >= 0) ) )
				{
					DrawArrowOp(nUbi, dUbi, ZOFFSET_ARROWS, 1.0, 0.0,
						dYOpSup, dYOpInf, dOpLen, dOpMult, common->skin[nArrow-5+SKINCOMMON_MINE]->GetCurrentTex());
				}
				if(nArrow >= 2 && nArrow <= 4 && !bDrawingHold && (nStatus >= JUDGAMENT_DRAW_LOSE || nStatus < 0 ||
					(nStatus < JUDGAMENT_DRAW_LOSE && dUbi < 0 && nStatus >= 0) ) )
				{
					bDrawingHold = true;

					float dUbiT = dUbi;
					// Find the finish for bottom
					int j = (y+1);
					int nArrowj = 0;
					int nStatusj = -1;
					for(j = y+1; j < nMax[n]; j++)
					{
						nArrowj = GetArrow(x, j, n);
						nStatusj = GetArrowStatus(x, j, n);
						if((nArrowj != 3 && nArrowj != 0) || nArrowj == 4 || (nArrowj == 3 && nStatusj < JUDGAMENT_DRAW_LOSE && nStatusj >= 0)) break;
					}
					if(j >= stepc.hStepGroup[n].hGroup.nSizeArrows)
					{
						j = stepc.hStepGroup[n].hGroup.nSizeArrows-1;
					}

					// If pressed, must be drawn in reference


					if(y != 0)
					{
						int nArrow_1 = GetArrow(x, y-1, n);
						int nStatus_1 = GetArrowStatus(x, y-1, n);
						if(nStatus_1 < JUDGAMENT_DRAW_LOSE && nStatus_1 >= 0 && dUbi < 0.0f && nArrow_1 >= 2 && nArrow_1 <= 3)
						{
							nUbi = UbiXArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, 0.0);
							dUbiT = UbiYArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, 0.0);
						}
					}


					// Code for bottom
					/*float dUbiT = dUbi+m_dSizeArrows/2*(float)skin[n]->dScale;

					if(y != 0)
					{
						int nArrow_1 = GetArrow(x, y-1, n);
						int nStatus_1 = GetArrowStatus(x, y-1, n);
						float dBeat_1 = m_dBeatDraw-float(stepc.hStepGroup[n].fWDArrows[y-1]);
						float dUbi_1 = UbiYArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, dBeat_1);

						if(nStatus_1 < JUDGAMENT_DRAW_LOSE && nStatus_1 >= 0 && dUbi < 0.0f && nArrow_1 >= 2 && nArrow_1 <= 4)
						{
							nUbi = UbiXArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, 0.0);
							dUbiT = UbiYArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, 0.0);
							DrawArrowOp(nUbi, 0.0, ZOFFSET_ARROWS, skin[n]->dScale, 0.0,
								dYOpSup, dYOpInf, dOpLen, dOpMult, skin[n]->skin[x%ARROWSLINE_SINGLE+SKIN_TDOWNLEFT]->GetCurrentTex());

						}
						else if(dUbiT > dUbi_1)
						{
							dUbiT = dUbi_1;
						}
					}*/



					float dBeatB = m_dBeatDraw-float(stepc.hStepGroup[n].fWDArrows[j]);
					float dUbiB = UbiYArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, dBeatB);

					if(nArrow!=4) DrawHoldOp(nUbi, dUbiT, dUbiB, ZOFFSET_ARROWS, skin[n]->dScale, 0.0,
						dYOpSup, dYOpInf, dOpLen, dOpMult, skin[n]->skin[x%ARROWSLINE_SINGLE+SKIN_FDOWNLEFT]->GetCurrentTex());
					if(nArrowj == 4 || nArrow==4) DrawBottomHoldOp(nUbi, dUbiT, nArrow== 4?dUbi:dUbiB, ZOFFSET_ARROWS, skin[n]->dScale, 0.0,
						dYOpSup, dYOpInf, dOpLen, dOpMult, skin[n]->skin[x%ARROWSLINE_SINGLE+SKIN_FDOWNLEFT]->GetCurrentTex(),
						skin[n]->skin[x%ARROWSLINE_SINGLE+SKIN_LFDOWNLEFT]->GetCurrentTex());
					if(nArrow!=4) DrawArrowOp(nUbi, dUbiT, ZOFFSET_ARROWS, skin[n]->dScale, 0.0,
						dYOpSup, dYOpInf, dOpLen, dOpMult, skin[n]->skin[x%ARROWSLINE_SINGLE+SKIN_TDOWNLEFT]->GetCurrentTex());
				}
				if(nArrow == 4 && !bDrawingHold && (nStatus >= JUDGAMENT_DRAW_LOSE || nStatus < 0 ||
					(nStatus < JUDGAMENT_DRAW_LOSE && dUbi < 0 && nStatus >= 0) ) )
				{


				}
			}
		}
	}	// Fin del dibujo y reconocimiento

	// Dibujo y reconocimiento de Brain Shower
	BSEvaluate(nLimitXA, nLevelMach);

	if(stepc.hStepGroup[0].hGroup.nIsVanishSup)
		dYOpSup = stepc.hStepGroup[0].hGroup.fVanishSup/100.0f*(BOTTOM_ARROWS-TOP_ARROWS)+TOP_ARROWS;
	else if(m_fEVanish!=0.0f)
		dYOpSup = m_fEVanish/100.0f*(BOTTOM_ARROWS-TOP_ARROWS)+TOP_ARROWS;

	if(stepc.hStepGroup[0].hGroup.nIsVanishInf)
		dYOpInf = stepc.hStepGroup[0].hGroup.fVanishSup/100.0f*(TOP_ARROWS-BOTTOM_ARROWS)+BOTTOM_ARROWS;
	else if(m_fIVanish!=0.0f)
		dYOpInf = m_fIVanish/100.0f*(TOP_ARROWS-BOTTOM_ARROWS)+BOTTOM_ARROWS;

	for(int y = 0; y < bsc.nSizeBSAnswers; y++)
	{
		for(int i = 0; i < nLimitXA; i++)
		{
			int nSkin = 0;
			int x = nOrder[i];
			if((status.nBSStatus[y*stepc.hStep.nArrowsPerLine+x] <= 0 || status.nBSStatus[y*stepc.hStep.nArrowsPerLine+x] > 2) && bsc.dwBSAnswers[y*stepc.hStep.nArrowsPerLine+x])
			{
				float dBeat = m_dBeatDraw-float(bsc.dWBSAnswers[y]);
				float nUbi = UbiXArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, dBeat);
				float dUbi = UbiYArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, dBeat);
				DWORD dwDArrow = LOWORD(bsc.dwBSAnswers[y*stepc.hStep.nArrowsPerLine+x]);
				DWORD dwGArrow = HIWORD(bsc.dwBSAnswers[y*stepc.hStep.nArrowsPerLine+x]);
				bool bActiveX = true;
				if(dUbi <= 0) bActiveX = false;

				/*if(m_bXMode && bActiveX)
				{
					if(x < MODE_PLAYER_SINGLE && (m_nPlayer == 1 || !m_nModePlay==MODE_PLAYER_SINGLE))
						l += (m_bDrop?1:-1)*t;
					else
						l -= (m_bDrop?1:-1)*t;
				}*/
				double t = m_dSizeArrows/2;
				double l = -m_dSizeArrows/2;
				double b = t-m_dSizeArrows;
				double r = l+m_dSizeArrows;
				if(bsc.nDBSAnswers[y] == 1 && dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
				{
					DrawObjectOp(t, l, b, r,
						nUbi, dUbi, ZOFFSET_ARROWS, 0.0,
						dYOpSup, dYOpInf, dOpLen, dOpMult, true, bs->skin[0]->GetCurrentTex());
					CString str;
					str.Format(_T("%d"), dwDArrow);
					double dMinL = (l+m_dSizeArrows/2)-m_dSizeArrows/4*str.GetLength();
					for(int z = 0; z < str.GetLength(); z++)
					{
						double ll = dMinL+m_dSizeArrows/2*(double)z;
						double rr = dMinL+m_dSizeArrows/2*(double)(z+1);
						int nAcc = 0;
						TCHAR chAcc = str.GetAt(z);
						if(chAcc >= '0' && chAcc <= '9')
							nAcc = chAcc - '0';
						else nAcc = 17;

						DrawObjectOp(t, ll, b, rr,
							nUbi, dUbi, ZOFFSET_ARROWS, 0.0,
							dYOpSup, dYOpInf, dOpLen, dOpMult, true, bs->skin[dwGArrow]->texture[nAcc]);
					}
				}
				else if(dwGArrow == 0)
				{
					double dScale = skin[0]->dScale;
					t = m_dSizeArrows/2*dScale;
					l = -m_dSizeArrows/2*dScale;
					b = t-m_dSizeArrows*dScale;
					r = l+m_dSizeArrows*dScale;
					DrawObjectOp(t, l, b, r,
						nUbi, dUbi, ZOFFSET_ARROWS, 0.0,
						dYOpSup, dYOpInf, dOpLen, dOpMult, true, skin[0]->skin[x%5+SKIN_TDOWNLEFT]->GetCurrentTex());
				}
				else if(dwGArrow == 18)
				{
					o = 1.0;
					double dClipMin = TOP_ARROWS;//0.2;
					double dClipMax = BOTTOM_ARROWS;//-1.0;
					// Primera asignacion
					dClipMin = -(double)dwDArrow/(TOP_ARROWS-BOTTOM_ARROWS)+TOP_ARROWS;
					if(dwDArrow == 0) dClipMin = TOP_ARROWS;
					if(dClipMax > dUbi && (dClipMax-VANISH_TRANSITION) < dUbi)
						o = 1-abs(dClipMax-t)/10;
					else if((dClipMax-VANISH_TRANSITION) >= t)
						o = 0.0;
					if(dClipMin < dUbi && (dClipMin+VANISH_TRANSITION) > dUbi)
						o = 1-abs(dClipMin-t)/10;
					else if((dClipMin+VANISH_TRANSITION) <= t)
						o = 0.0;
					if(bsc.nBSAnswers[y*stepc.hStep.nArrowsPerLine+x])
					{
						double dScale = skin[0]->dScale;
						t = m_dSizeArrows/2*dScale;
						l = -m_dSizeArrows/2*dScale;
						b = t-m_dSizeArrows*dScale;
						r = l+m_dSizeArrows*dScale;

						DrawObjectOp(t, l, b, r,
							nUbi, dUbi, ZOFFSET_ARROWS, 0.0,
							dYOpSup, dYOpInf, dOpLen, dOpMult*o, true, skin[0]->skin[x%5+SKIN_TDOWNLEFT]->GetCurrentTex());
					}
					// Segunda asignación
					o = 1.0-o;
					t = m_dSizeArrows/2;
					l = -m_dSizeArrows/2;
					b = t-m_dSizeArrows;
					r = l+m_dSizeArrows;
					DrawObjectOp(t, l, b, r,
							nUbi, dUbi, ZOFFSET_ARROWS, 0.0,
							dYOpSup, dYOpInf, dOpLen, dOpMult*o, true, bs->skin[0]->GetCurrentTex());

					double ll = l+m_dSizeArrows/4;
					double rr = r-m_dSizeArrows/4;

					DrawObjectOp(t, ll, b, rr,
							nUbi, dUbi, ZOFFSET_ARROWS, 0.0,
							dYOpSup, dYOpInf, dOpLen, dOpMult*o, true, bs->skin[1]->texture[17]);
				}
				else
				{
					DrawObjectOp(t, l, b, r,
							nUbi, dUbi, ZOFFSET_ARROWS, 0.0,
							dYOpSup, dYOpInf, dOpLen, dOpMult, true, bs->skin[0]->GetCurrentTex());
					double ll = l+(dwGArrow == 1?(m_dSizeArrows/4):0);
					double rr = r-(dwGArrow == 1?(m_dSizeArrows/4):0);
					DrawObjectOp(t, ll, b, rr,
							nUbi, dUbi, ZOFFSET_ARROWS, 0.0,
							dYOpSup, dYOpInf, dOpLen, dOpMult, true, bs->skin[dwGArrow]->texture[dwDArrow]);
				}
			}
		}
	}
	// Fin de Dibujo y reconocimiento de Brain shower

	DrawArrowGlow(nLimitX, nOrder);

	g_gmMain->MatrixMode(PMG_PROJECTION);
	g_gmMain->PopMatrix();
	g_gmMain->MatrixMode(PMG_MODELVIEW);
	g_gmMain->PopMatrix();

	g_gmMain->PushMatrix();
	if(m_nModePlay==MODE_PLAYER_SINGLE && !m_nModeMach) g_gmMain->Translate(TRANSLATE_INITIAL_PLAYER, 0.0, 0.0);
	if(m_nPlayer != 1 && m_nModePlay==MODE_PLAYER_SINGLE) g_gmMain->Translate(TRANSLATE_NEXT_PLAYER*double(m_nPlayer-1), 0.0, 0.0);
	if(m_bNXMode) g_gmMain->Translate(0.0, 15.0, 0.0);
	g_gmMain->Scale(0.6, 0.6, 1.0);

	__int64 nCaret = _abs64(m_nTime-evarrows.nLastEva);
	bool bMissCombo = evarrows.nTypeEva<=2;
	int nShowCombo = bMissCombo?(status.nMissComboTotal):(status.nComboTotal);
	if(evarrows.nTypeEva>0 && evarrows.nTypeEva<=5)
	{
		if(nCaret >= 0 && nCaret <= EVA_1_EVENT_TIME)
		{
			double dApt = double(nCaret) / EVA_1_EVENT_TIME;
			double l = -20.0;
			double r = 20.0;
			double t = 2.5;
			double b = -2.5;
			g_gmMain->Scale(1.0+0.3*(1.0-dApt), 1.0+0.3*(1.0-dApt), 1.0);

			g_gmMain->BindTexture( common->skin[evarrows.nTypeEva+SKINCOMMON_EVA1-1]->GetCurrentTex());
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
			g_gmMain->End();
			if(nShowCombo >= 4)
			{
				l = -7.6;
				r = 7.6;
				t = -2.5;
				b = -4.5;
				g_gmMain->BindTexture( common->skin[SKINCOMMON_COMBO+(bMissCombo?1:0)]->GetCurrentTex());
				g_gmMain->Begin(PMG_QUADS);
				g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
				g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
				g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
				g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
				g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
				g_gmMain->End();
				CString strCombo;
				strCombo.Format(_T("%d"), nShowCombo);
				int nDisplay = strCombo.GetLength();
				for(int x = nDisplay+1; x <= 3; x++)
				{
					strCombo = _T("0")+strCombo;
				}
				nDisplay = nDisplay<=3?3:nDisplay;
				double dLength = 9.0;
				double dl = -4.5*nDisplay;
				t = -4.5;
				b = -9.5;
				for(int e = 0; e < nDisplay; e++)
				{
					l = dl+dLength*(e);
					r = dl+dLength*(e+1);
					int nNum;
					if(e >= strCombo.GetLength()) nNum= 0;
					else nNum= strCombo[e]-'0';
					g_gmMain->BindTexture( common->skin[SKINCOMMON_FONT0+nNum]->GetCurrentTex());
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
					g_gmMain->End();
				}
			}
		}
		nCaret -= EVA_1_EVENT_TIME;
		if(nCaret > 0 && nCaret <= EVA_2_EVENT_TIME)
		{
			double l = -20.0;
			double r = 20.0;
			double t = 2.5;
			double b = -2.5;
			g_gmMain->BindTexture( common->skin[evarrows.nTypeEva+SKINCOMMON_EVA1-1]->GetCurrentTex());
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
			g_gmMain->End();
			if(nShowCombo >= 4)
			{
				l = -7.6;
				r = 7.6;
				t = -2.5;
				b = -4.5;
				g_gmMain->BindTexture( common->skin[SKINCOMMON_COMBO+(bMissCombo?1:0)]->GetCurrentTex());
				g_gmMain->Begin(PMG_QUADS);
				g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
				g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
				g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
				g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
				g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
				g_gmMain->End();
				CString strCombo;
				strCombo.Format(_T("%d"), nShowCombo);
				int nDisplay = strCombo.GetLength();
				for(int x = nDisplay+1; x <= 3; x++)
				{
					strCombo = _T("0")+strCombo;
				}
				nDisplay = nDisplay<=3?3:nDisplay;
				double dLength = 9.0;
				double dl = -4.5*nDisplay;
				t = -4.5;
				b = -9.5;
				for(int e = 0; e < nDisplay; e++)
				{
					l = dl+dLength*(e);
					r = dl+dLength*(e+1);
					int nNum;
					if(e >= strCombo.GetLength()) nNum= 0;
					else nNum= strCombo[e]-'0';
					g_gmMain->BindTexture( common->skin[SKINCOMMON_FONT0+nNum]->GetCurrentTex());
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
					g_gmMain->End();
				}
			}
		}
		nCaret -= EVA_2_EVENT_TIME;
		if(nCaret > 0 && nCaret <= EVA_3_EVENT_TIME)
		{
			double dCaret = ((double)nCaret)/EVA_3_EVENT_TIME;
			double l = -20.0-(5.0*dCaret);
			double r = 20.0+(5.0*dCaret);
			double t = 2.5-(1.25*dCaret);
			double b = -2.5+(1.25*dCaret);
			//glBlendFunc(PMG_SRC_ALPHA, PMG_ONE_MINUS_SRC_LPHA);
			g_gmMain->BindTexture( common->skin[evarrows.nTypeEva+SKINCOMMON_EVA1-1]->GetCurrentTex());
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0-dCaret);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
			g_gmMain->End();
			if(nShowCombo >= 4)
			{
				l = -7.6;
				r = 7.6;
				t = -2.5;
				b = -4.5;
				g_gmMain->BindTexture( common->skin[SKINCOMMON_COMBO+(bMissCombo?1:0)]->GetCurrentTex());
				g_gmMain->Begin(PMG_QUADS);
				g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0-dCaret);
				g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
				g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
				g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
				g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
				g_gmMain->End();
				CString strCombo;
				strCombo.Format(_T("%d"), nShowCombo);
				int nDisplay = strCombo.GetLength();
				for(int x = nDisplay+1; x <= 3; x++)
				{
					strCombo = _T("0")+strCombo;
				}
				nDisplay = nDisplay<=3?3:nDisplay;
				double dLength = 9.0;
				double dl = -4.5*nDisplay;
				t = -4.5;
				b = -9.5;
				for(int e = 0; e < nDisplay; e++)
				{
					l = dl+dLength*(e);
					r = dl+dLength*(e+1);
					int nNum;
					if(e >= strCombo.GetLength()) nNum= 0;
					else nNum= strCombo[e]-'0';
					g_gmMain->BindTexture( common->skin[SKINCOMMON_FONT0+nNum]->GetCurrentTex());
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0-dCaret);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
					g_gmMain->End();
				}
			}
		}
	}
	g_gmMain->PopMatrix();
	g_gmMain->PushMatrix();
	if(m_nModePlay==MODE_PLAYER_SINGLE && m_nPlayer == 2)
	{
		g_gmMain->Scale(-1.0, 1.0, 1.0);
	}
	if(status.dLife > MAX_BAR) status.dLife = MAX_BAR;
	if(status.dLife < 0.0) status.dLife = 0.0;
	double dLongBar = 33.0;
	double dMulti = status.dLife+pow((1-m_dBeatTime+(double)((int)m_dBeatTime)), double(5))/10-0.1;
	if(m_dBeatTime < 0) dMulti = 1.0;
	if(dMulti > 1.0 || status.dLife >= 1.0) dMulti = 1.0;
	if(dMulti < 0) dMulti = 0.0;
	double t = 28.5;
	double l = -39.0;
	double b = t-3.0;
	double r = l+dLongBar*dMulti;
	g_gmMain->BindTexture( common->skin[32]->GetCurrentTex());
	g_gmMain->Begin(PMG_QUADS);
	g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
	g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
	g_gmMain->TexCoord2d(dMulti, 1.0); g_gmMain->Vertex3d(r, t, -50);
	g_gmMain->TexCoord2d(dMulti, 0.0); g_gmMain->Vertex3d(r, b, -50);
	g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
	g_gmMain->End();
	double dApt = 0.25+double(dwTick%80)/160;
	if(status.dLife >= 0.04 && status.dLife <= 0.96)
	{
		t = 28.5;
		l = -39.0+dLongBar*status.dLife-0.5;
		b = t-3.0;
		r = -39.0+dLongBar*status.dLife+0.5;
		g_gmMain->BindTexture( 0);
		g_gmMain->Begin(PMG_QUADS);
		g_gmMain->Color4d(0.6, 0.6, 0.0, dApt);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
		g_gmMain->TexCoord2d(dMulti, 1.0); g_gmMain->Vertex3d(r, t, -50);
		g_gmMain->TexCoord2d(dMulti, 0.0); g_gmMain->Vertex3d(r, b, -50);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
		g_gmMain->End();
	}
	if(status.dLife <= 0.2)
	{
		t = 28.5;
		l = -39.0;
		b = t-3.0;
		r = l+dLongBar;
		g_gmMain->BindTexture( 0);
		g_gmMain->Begin(PMG_QUADS);
		g_gmMain->Color4d(1.0, 0.0, 0.0, dApt);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
		g_gmMain->TexCoord2d(dMulti, 1.0); g_gmMain->Vertex3d(r, t, -50);
		g_gmMain->TexCoord2d(dMulti, 0.0); g_gmMain->Vertex3d(r, b, -50);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
		g_gmMain->End();
	}
	if(status.dLife >= 1.0)
	{
		t = 28.5;
		l = -39.0;
		b = t-3.0;
		r = l+dLongBar;
		g_gmMain->BindTexture( 0);
		g_gmMain->Begin(PMG_QUADS);
		g_gmMain->Color4d(1.0, 1.0, 1.0, dApt);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
		g_gmMain->TexCoord2d(dMulti, 1.0); g_gmMain->Vertex3d(r, t, -50);
		g_gmMain->TexCoord2d(dMulti, 0.0); g_gmMain->Vertex3d(r, b, -50);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
		g_gmMain->End();
	}
	t = 29.0;
	l = -39.0;
	b = t-4.0;
	r = l+dLongBar;
	g_gmMain->BindTexture( common->skin[31]->GetCurrentTex());
	g_gmMain->Begin(PMG_QUADS);
	g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
	g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
	g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
	g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
	g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
	g_gmMain->End();

	g_gmMain->PopMatrix();
	// Dibujo de las preguntas de BS
	g_gmMain->PushMatrix();
	int nPQuest = -1;
	for(int y = 0; y < bsc.nSizeBSQuestions; y++)
	{
		double dE = bsc.dWEBSQuestions[y];
		double dI = bsc.dWIBSQuestions[y];

		if(dE > m_dBeatTime && m_dBeatTime>dI)
		{
			nPQuest = y;
		}
	}
	if(nPQuest != -1)
	{
		DWORD dwSubject = HIWORD(bsc.dwBSQuestions[nPQuest]);
		DWORD dwType = LOWORD(bsc.dwBSQuestions[nPQuest]);
		switch(dwSubject)
		{
		case 0:
			if(m_nModePlay==MODE_PLAYER_SINGLE && m_nModeMach) g_gmMain->Translate(21.5, 0.0, 0.0);
			switch(dwType)
			{
			case 0:
				DrawBS03(nPQuest);
				break;
			case 1:
				DrawBS04(nPQuest);
				break;
			case 2:
				DrawBS05(nPQuest);
				break;
			case 3:
				if(m_nPlayer == 2 && m_nModePlay==MODE_PLAYER_SINGLE && !m_nModeMach) g_gmMain->Translate(2*m_dSizeArrows/3, 0.0, 0.0);
				DrawBS07(nPQuest);
				break;
			}
			break;
		case 1:
			if(m_nModePlay==MODE_PLAYER_SINGLE && m_nModeMach) g_gmMain->Translate(21.5, 0.0, 0.0);
			switch(dwType)
			{
			case 0:
				DrawBS03(nPQuest);
				break;
			case 1:
				DrawBS03(nPQuest);
				break;
			case 2:
				DrawBS02(nPQuest);
				break;
			case 3:
				DrawBS06(nPQuest);
				break;
			case 4:
				DrawBS06(nPQuest);
				break;
			}
			break;
		case 2:
			switch(dwType)
			{
			case 0:
				DrawBS01(nPQuest);
				break;
			case 1:
				DrawBS01(nPQuest);
				break;
			case 2:
				DrawBS01(nPQuest);
				break;
			case 3:
				DrawBS02(nPQuest);
				break;
			case 4:
				DrawBS01(nPQuest);
				break;
			}
			break;
		case 3:
			if(m_nModePlay==MODE_PLAYER_SINGLE && m_nModeMach) g_gmMain->Translate(21.5, 0.0, 0.0);
			switch(dwType)
			{
			case 0:
				DrawBS08(nPQuest);
				break;
			case 1:
				DrawBS06(nPQuest);
				break;
			case 2:
				DrawBS03(nPQuest);
				break;
			case 3:
				DrawBS08(nPQuest);
				break;
			case 4:
				if(m_nPlayer == 2 && m_nModePlay==MODE_PLAYER_SINGLE && !m_nModeMach) g_gmMain->Translate(2*m_dSizeArrows/3, 0.0, 0.0);
				DrawBS07(nPQuest);
				break;
			}
			break;
		}
	}
	g_gmMain->PopMatrix();
	//////////////////////////////////
	// Dibujo PopUp BS
	for(int y = 0; y < bsc.nSizeBSPopUp; y++)
	{
		t = 15.0; b = -15.0; l = -35.0; r = 35.0;
		__int64 nWPopUp = BeatToTime(bsc.dWBSPopUp[y]);
		__int64 nOff = m_nTime-nWPopUp;
		if(nOff>=-200 && nOff<0)
		{
			double dCaret = abs((double)nOff/200);
			g_gmMain->BindTexture( bs->skin[11]->GetCurrentTex());
			g_gmMain->Color4d(1.0,1.0,1.0,1-dCaret);
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l-5*dCaret, t+5*dCaret, -50);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r+5*dCaret, t+5*dCaret, -50);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r+5*dCaret, b-5*dCaret, -50);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l-5*dCaret, b-5*dCaret, -50);
			g_gmMain->End();
		}
		if(nOff>=0 && nOff<3000)
		{
			g_gmMain->BindTexture( bs->skin[11]->GetCurrentTex());
			g_gmMain->Color4d(1.0,1.0,1.0,1.0);
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
			g_gmMain->End();
			bsf->glDrawText(t, l, b, r, 0.0, 0.0, -50.0, 2.0, 5.0, strBSDisp[min(bsc.dwBSPopUp[y], 49)], DT_VCENTER|DT_CENTER);
		}
		if(nOff>=3000 && nOff<3200)
		{
			double dCaret = abs((double)(nOff-3000)/200);
			g_gmMain->BindTexture( bs->skin[11]->GetCurrentTex());
			g_gmMain->Color4d(1.0,1.0,1.0,1.0);
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l+5*dCaret, t-5*dCaret, -50);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r-5*dCaret, t-5*dCaret, -50);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r-5*dCaret, b+5*dCaret, -50);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l+5*dCaret, b+5*dCaret, -50);
			g_gmMain->End();
			g_gmMain->Color4d(1.0,1.0,1.0,1-dCaret);
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l-5*dCaret, t+5*dCaret, -50);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r+5*dCaret, t+5*dCaret, -50);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r+5*dCaret, b-5*dCaret, -50);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l-5*dCaret, b-5*dCaret, -50);
			g_gmMain->End();
		}
		if(nOff>=3200 && nOff<3400)
		{
			double dCaret = abs((double)(nOff-3200)/200);
			g_gmMain->BindTexture( bs->skin[11]->GetCurrentTex());
			g_gmMain->Color4d(1.0,1.0,1.0,1-dCaret);
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l+5-10*dCaret, t-5+10*dCaret, -50);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r-5+10*dCaret, t-5+10*dCaret, -50);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r-5+10*dCaret, b+5-10*dCaret, -50);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l+5-10*dCaret, b+5-10*dCaret, -50);
			g_gmMain->End();
		}
	}
	//////////////////
	// Dibujo eva BS
	glPushMatrix();
	if (!(m_nModePlay==MODE_PLAYER_SINGLE && m_nModeMach))
	{
		if(m_nModePlay==MODE_PLAYER_SINGLE && m_nPlayer == 1) g_gmMain->Translate(-20.0, 0.0, 0.0);
		if(m_nModePlay==MODE_PLAYER_SINGLE && m_nPlayer == 2) g_gmMain->Translate(20.0, 0.0, 0.0);
	}
	nCaret = _abs64(m_nTime-evarrows.nLastBSEva);
	if(nCaret < 500 && evarrows.nTypeBSEva)
	{
		t = 15.0; b = 5.0; l = -10.0; r = 10.0;
		double dCaret = (double)nCaret/500;
		if(evarrows.nTypeBSEva == 1) g_gmMain->BindTexture( bs->skin[10]->GetCurrentTex());
		else g_gmMain->BindTexture( bs->skin[9]->GetCurrentTex());
		g_gmMain->Begin(PMG_QUADS);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0-dCaret);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l-5*dCaret, t+5*dCaret, -50);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r+5*dCaret, t+5*dCaret, -50);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r+5*dCaret, b-5*dCaret, -50);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l-5*dCaret, b-5*dCaret, -50);
		g_gmMain->End();
	}
	glPopMatrix();
	// Dibujo del hidden
	if(evarrows.bHidden && m_nTime >= evarrows.nHidden && m_nTime <= (evarrows.nHidden+500))
	{
		t = 30.0; b = -30.0; l = -40.0; r = 40.0;
		double dCaret = (double)_abs64(m_nTime-evarrows.nHidden)/500;
		g_gmMain->BindTexture( NULL);
		g_gmMain->Begin(PMG_QUADS);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0-dCaret);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50);
		g_gmMain->End();
	}
	if(m_bAutoPlay)
	{
		/*if(m_bCharacter)
		{
			for(int s = nHlp; s < crt.hCtr.nSizeArrowsAP; s++)
			{
				if(crt.dWWArrowsAP[s] <= m_dBeatDraw)
				{
					int nArr = LOWORD(crt.dwArrowsAP[s]);
					int nType = HIWORD(crt.dwArrowsAP[s]);
					if(nType == 4)
					{
						evarrows.bArrows[nArr] = false;
					}
					m_nHlpAuto = s+1;
				}
				else break;
			}
		}
		else*/
		if(stepc.hStep.nSizeArrowGroups > 0)
		{
			for(int s = nHlp; s < stepc.hStepGroup[0].hGroup.nSizeArrows; s++)
			{
				if(stepc.hStepGroup[0].nWTArrows[s] <= m_nTime)
				{
					for(int t = 0; t < stepc.hStep.nArrowsPerLine; t++)
					{
						if(stepc.hStepGroup[0].nArrows[s*stepc.hStep.nArrowsPerLine+t] == 4)
						{
							evarrows.bArrows[t] = false;
						}
					}
					m_nHlpAuto = s+1;
				}
				else break;
			}
		}
	}

	delete nMin;
	delete nMax;
}

void CMaxStepSmu::DrawArrow(double dX, double dY, double dZ, double dScale, double dRotate, CGraphTexture* tex)
{
	// centered position
	double t = m_dSizeArrows/2;
	double l = -m_dSizeArrows/2;
	double b = -m_dSizeArrows/2;
	double r = m_dSizeArrows/2;
	DrawObject(t, l, b, r, dX, dY, dZ, dScale, dRotate, tex);
}

void CMaxStepSmu::DrawObject(double t, double l, double b, double r,
					double dX, double dY, double dZ, double dScale, double dRotate,
					CGraphTexture* tex)
{
	g_gmMain->MatrixMode(PMG_MODELVIEW);
	g_gmMain->PushMatrix();
	g_gmMain->Translate(dX, dY, dZ);
	g_gmMain->Scale(dScale, dScale, 1.0);
	g_gmMain->Rotate(dRotate, 0.0, 0.0, 1.0);

	g_gmMain->BindTexture(tex);
	g_gmMain->Begin(PMG_QUADS);
	g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, 0.0);
	g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, 0.0);
	g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, 0.0);
	g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, 0.0);
	g_gmMain->End();

	g_gmMain->PopMatrix();
}

void CMaxStepSmu::DrawArrowOp(double dX, double dY, double dZ, double dScale, double dRotate,
		double dYOpSup, double dYOpInf, double dOpLen, double dOpMult, CGraphTexture* tex)
{
	// centered position
	double dSize = m_dSizeArrows*dScale;
	double t = m_dSizeArrows/2*dScale;
	double l = -m_dSizeArrows/2*dScale;
	double b = -m_dSizeArrows/2*dScale;
	double r = m_dSizeArrows/2*dScale;

	DrawObjectOp(t, l, b, r,
				 dX, dY, dZ, dRotate,
				 dYOpSup, dYOpInf, dOpLen, dOpMult, true, tex);
}

void CMaxStepSmu::DrawHoldOp(double dX, double dY_top, double dY_bot, double dZ, double dScale, double dRotate,
		double dYOpSup, double dYOpInf, double dOpLen, double dOpMult, CGraphTexture* tex)
{
	double dY = dY_top;

	// centered position
	double dSize = dY_top-dY_bot;
	double t = 0;
	double l = -m_dSizeArrows/2*dScale;
	double b = -dSize;
	double r = m_dSizeArrows/2*dScale;

	DrawObjectOp(t, l, b, r,
				 dX, dY, dZ, dRotate,
				 dYOpSup, dYOpInf, dOpLen, dOpMult, false, tex);
}

void CMaxStepSmu::DrawBottomHoldOp(double dX, double dY_top, double dY_bot, double dZ, double dScale, double dRotate,
		double dYOpSup, double dYOpInf, double dOpLen, double dOpMult, CGraphTexture* texHold, CGraphTexture* tex)
{
	double dY = dY_bot;

	// centered position
	double dSize = dY_top-dY_bot;
	double t = dSize;
	double l = -m_dSizeArrows/2*dScale;
	double b = m_dSizeArrows/2*dScale;
	double r = m_dSizeArrows/2*dScale;

	if(t > b) DrawObjectOp(t, l, b, r,
				 dX, dY, dZ, dRotate,
				 dYOpSup, dYOpInf, dOpLen, dOpMult, false, texHold);

	dSize = m_dSizeArrows*dScale;
	t = clamp(dY_top-dY_bot, 0.0, m_dSizeArrows/2*dScale);
	l = -m_dSizeArrows/2*dScale;
	b = -m_dSizeArrows/2*dScale;
	r = m_dSizeArrows/2*dScale;

	DrawObjectOp(t, l, b, r, (t-b)/dSize, 0.0,
				 dX, dY, dZ, dRotate,
				 dYOpSup, dYOpInf, dOpLen, dOpMult, true, tex);
}

void CMaxStepSmu::DrawObjectOp(double t, double l, double b, double r, double v_t, double v_b,
							   double dX, double dY, double dZ, double dRotate,
							   double dYOpSup, double dYOpInf, double dOpLen, double dOpMult, bool bVRaster, CGraphTexture* tex)
{
	g_gmMain->MatrixMode(PMG_MODELVIEW);
	g_gmMain->PushMatrix();
	g_gmMain->Translate(dX, dY, dZ);
	g_gmMain->Rotate(dRotate, 0.0, 0.0, 1.0);

	g_gmMain->BindTexture(tex);

	// centered position
	double dSize = t-b;

	double dYSup = dYOpSup-dY;
	double dYSupF = dYSup+dOpLen;
	double dYInf = dYOpInf-dY;
	double dYInfF = dYInf-dOpLen;

	// List operations
	double dYUbi[] = {dYSup, dYSupF, dYInf, dYInfF};
	double dYOp[] = {1.0, 0.0, 1.0, 0.0};
	int nOp = sizeof(dYUbi)/sizeof(double);

	// Sort operations
	for(int i=0;i<nOp;i++)
    {
        for(int j=0;j<i;j++)
        {
            if(dYUbi[i]>dYUbi[j])
            {
                double temp=dYUbi[i]; //swap
                dYUbi[i]=dYUbi[j];
                dYUbi[j]=temp;

				temp=dYOp[i]; //swap
                dYOp[i]=dYOp[j];
                dYOp[j]=temp;
            }
        }
    }

	// Do operations
	for(int i=0;i<(nOp+1);i++)
    {
		int j = i-1;
		double dY1, dO1;
		double dY2, dO2;
		if(j < 0)
		{
			dY1 = TOP_ARROWS; dO1 = dYOp[0];
			dY2 = dYUbi[0]; dO2 = dYOp[0];
		}
		if((j+1) >= nOp)
		{
			dY1 = dYUbi[nOp-1]; dO1 = dYOp[nOp-1];
			dY2 = BOTTOM_ARROWS; dO2 = dYOp[nOp-1];
		}
		else
		{
			dY1 = dYUbi[j]; dO1 = dYOp[j];
			dY2 = dYUbi[j+1]; dO2 = dYOp[j+1];
		}
		// Dont draw if operations are zero, look if arrow is on area, there is area to do
		if((dO1 != 0.0 || dO2 != 0.0) && ((dY2 <= t && t <= dY1) || (dY2 <= b && b <= dY1) || (b <= dY1 && dY1 <= t) || (b <= dY2 && dY2 <= t)) && dY1 != dY2)
		{
			double ta = clamp(t, dY2, dY1);
			double ba = clamp(b, dY2, dY1);
			double vt = (bVRaster?((ta-b)/dSize):1.0)*(v_t-v_b)+v_b;
			double vb = (bVRaster?((ba-b)/dSize):0.0)*(v_t-v_b)+v_b;
			double ot = (ta-dY2)/(dY1-dY2)*(dO1-dO2)+dO2;
			double ob = (ba-dY2)/(dY1-dY2)*(dO1-dO2)+dO2;

			// Draw
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->Color4d(1.0, 1.0, 1.0, ot*dOpMult);
			g_gmMain->TexCoord2d(0.0, vt); g_gmMain->Vertex3d(l, ta, 0.0);
			g_gmMain->TexCoord2d(1.0, vt); g_gmMain->Vertex3d(r, ta, 0.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, ob*dOpMult);
			g_gmMain->TexCoord2d(1.0, vb); g_gmMain->Vertex3d(r, ba, 0.0);
			g_gmMain->TexCoord2d(0.0, vb); g_gmMain->Vertex3d(l, ba, 0.0);
			g_gmMain->End();
		}
	}

	g_gmMain->PopMatrix();
}

void CMaxStepSmu::DrawObjectOp(double t, double l, double b, double r,
							   double dX, double dY, double dZ, double dRotate,
							   double dYOpSup, double dYOpInf, double dOpLen, double dOpMult, bool bVRaster, CGraphTexture* tex)
{
	DrawObjectOp(t, l, b, r, 1.0, 0.0,
				 dX, dY, dZ, dRotate,
				 dYOpSup, dYOpInf, dOpLen, dOpMult, bVRaster, tex);
}

void CMaxStepSmu::DrawArrowGuide(int nLimitX, int* nOrder, double o)
{
	if(stepc.hStep.nSizeArrowGroups <= 0) return;
	g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
	for(int i = 0; i < nLimitX; i++)
	{
		int x = nOrder[i];
		double nUbi = UbiXArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, 0.0);
		DrawArrow(nUbi, 0.0, ZOFFSET_ARROWS, skin[0]->dScale, 0.0, skin[0]->skin[x%ARROWSLINE_SINGLE+SKIN_RUDOWNLEFT]->GetCurrentTex());
	}

	g_gmMain->Color4d(1.0, 1.0, 1.0, o);
	for(int i = 0; i < nLimitX; i++)
	{
		int x = nOrder[i];
		double nUbi = UbiXArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, 0.0);
		DrawArrow(nUbi, 0.0, ZOFFSET_ARROWS, skin[0]->dScale, 0.0, skin[0]->skin[x%ARROWSLINE_SINGLE+SKIN_RLDOWNLEFT]->GetCurrentTex());
	}
}

void CMaxStepSmu::DrawArrowGlow(int nLimitX, int* nOrder)
{
	if(stepc.hStep.nSizeArrowGroups <= 0) return;
	for(int i = 0; i < nLimitX; i++)
	{
		int x = nOrder[i];
		double nUbi = UbiXArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, 0.0);
		__int64 nCaret = abs(m_nTime-evarrows.nArrows[x]);
		double dCaret = (double)nCaret/DRAWGU_TIME;
		double dScale = (DRAWGU_SCALEEND-DRAWGU_SCALEBEGIN)*dCaret+DRAWGU_SCALEBEGIN;
		double dOp = (DRAWGU_OPACITYEND-DRAWGU_OPACITYBEGIN)*dCaret+DRAWGU_OPACITYBEGIN;

		if(nCaret >= 0 && nCaret <= DRAWGU_TIME)
		{
			g_gmMain->Color4d(1.0, 1.0, 1.0, dOp);
			DrawArrow(nUbi, 0.0, ZOFFSET_ARROWS, skin[0]->dScale*dScale, 0.0, skin[0]->skin[x%ARROWSLINE_SINGLE+SKIN_EBDOWNLEFT]->GetCurrentTex());
		}
	}

	for(int i = 0; i < nLimitX; i++)
	{
		int x = nOrder[i];
		double nUbi = UbiXArrow(x, nLimitX, m_dDistArrows, m_dSizeArrows, 0.0);
		__int64 nCaret = abs(m_nTime-evarrows.nGlowArrows[x]);
		double dCaret = (double)nCaret/DRAWGLOW_TIME;
		double dScale = (DRAWGLOW_SCALEEND-DRAWGLOW_SCALEBEGIN)*dCaret+DRAWGLOW_SCALEBEGIN;
		double dOp = (DRAWGLOW_OPACITYEND-DRAWGLOW_OPACITYBEGIN)*dCaret+DRAWGLOW_OPACITYBEGIN;

		if(nCaret >= 0 && nCaret <= DRAWGLOW_TIME)
		{
			g_gmMain->Color4d(1.0, 1.0, 1.0, dOp);
			DrawArrow(nUbi, 0.0, ZOFFSET_ARROWS, skin[0]->dScale, 0.0, skin[0]->skin[x%ARROWSLINE_SINGLE+SKIN_TDOWNLEFT]->GetCurrentTex());
			g_gmMain->BlendFunc(PMG_SRC_ALPHA, PMG_ONE);
			DrawArrow(nUbi, 0.0, ZOFFSET_ARROWS, skin[0]->dScale*dScale, 0.0, skin[0]->skin[x%ARROWSLINE_SINGLE+SKIN_GDOWNLEFT]->GetCurrentTex());
			g_gmMain->BlendFunc(PMG_SRC_ALPHA, PMG_ONE_MINUS_SRC_ALPHA);
		}
	}

}
