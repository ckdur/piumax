#include "MaxStepSmu.h"

void CMaxStepSmu::DrawBS01(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	float dOffIQuest = bsc.dWIBSQuestions[y];
	float dOffAQuest = bsc.dWABSQuestions[y];
	float dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nTime;
	///////////////////////////////
	double t=3.5, b=-3.5, l=-40.0, r=40.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[15]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo
	{
		g_gmMain->BindTexture( bs->skin[15]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);
		glEnd();
		CString strDisp = _T("");
		int nSize = 0;
		for(int x = 0; x < 30; x++)
		{
			if(bsc.dwDBSQuestions[y*30+x] == 0) break;
			nSize++;
		}
		bool bMode = false;
		for(int x = 0; x < nSize; x++)
		{
			if(bMode)
			{
				int nNumber = LOWORD(bsc.dwDBSQuestions[y*30+x]);
				switch(nNumber)
				{
				case 10:
					strDisp += _T("+");
					break;
				case 11:
					strDisp += _T("-");
					break;
				case 12:
					strDisp += _T("*");
					break;
				case 13:
					strDisp += _T("/");
					break;
				case 14:
					strDisp += _T("=");
					break;
				case 15:
					strDisp += _T("<");
					break;
				case 16:
					strDisp += _T(">");
					break;
				default:
					strDisp += _T("?");
					break;
				}
			}
			else
			{
				int nNumber = LOWORD(bsc.dwDBSQuestions[y*30+x]);
				if(nNumber > 100 || nNumber < 0) nNumber = 0;
				CString str;
				str.Format(_T("%d"), nNumber);
				strDisp += str;
			}
			if(nType != 1 || (x+2) != nSize) bMode = !bMode;
		}
		int nSizeShow = strDisp.GetLength();
		double nMinL = -1.75*(double)nSizeShow;
		for(int x = 0; x < nSizeShow; x++)
		{
			int nInd = 0;
			TCHAR chCaret = strDisp.GetAt(x);
			if(chCaret >= '0' && chCaret <= '9')
				nInd = chCaret - '0';
			else if(chCaret == '+')
				nInd = 10;
			else if(chCaret == '-')
				nInd = 11;
			else if(chCaret == '*')
				nInd = 12;
			else if(chCaret == '/')
				nInd = 13;
			else if(chCaret == '=')
				nInd = 14;
			else if(chCaret == '<')
				nInd = 15;
			else if(chCaret == '>')
				nInd = 16;
			else nInd = 17;
			t=3.5; b=-3.5; l=nMinL+3.5*x; r=nMinL+3.5*(x+1);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
			g_gmMain->BindTexture( bs->skin[1]->texture[nInd]);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);
			glEnd();
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=3.5; b=-3.5; l=-40.0; r=40.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->BindTexture( bs->skin[15]->texture[0]);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[15]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);
		glEnd();
		glPopMatrix();
	}
}

void CMaxStepSmu::DrawBS02(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	float dOffIQuest = bsc.dWIBSQuestions[y];
	float dOffAQuest = bsc.dWABSQuestions[y];
	float dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nTime;
	///////////////////////////////
	double t=0.0, b=-30.0, l=-20.0, r=20.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo
	{
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);
		glEnd();
		for(int x = 0; x < 5; x++)
		{
			if(m_nTime <= bsc.dwDBSQuestionsEx[y*40+25+x] ||
				(m_nTime >= (bsc.dwDBSQuestionsEx[y*40+25+x]+500)
				&& m_nTime <= (bsc.dwDBSQuestionsEx[y*40+25+x]+1000)))
			{
				g_gmMain->PushMatrix();
				switch(x)
				{
				case 0:
					glTranslated(-18.83, -28.239, 0.0);
					break;
				case 4:
					glTranslated(6.276, -28.239, 0.0);
					break;
				case 1:
					glTranslated(-18.83, -10.293, 0.0);
					break;
				case 3:
					glTranslated(6.276, -10.293, 0.0);
					break;
				default:
					glTranslated(-6.277, -19.706, 0.0);
					break;
				}
				int nCount = 0;
				for(int z = 0; z < 4; z++)
				{
					if(bsc.dwDBSQuestionsEx[y*40+4*x+z])
					{
						nCount++;
					}
				}
				for(int z = 0; z < 4; z++)
				{
					if(bsc.dwDBSQuestionsEx[y*40+4*x+z])
					{
						if(nCount == 0)
						{
							b = 0.7065; t = 8.7065; l = 2.2765; r = 10.2765;
						}
						else
						{
							switch(bsc.dwDBSQuestionsEx[y*40+20+x])
							{
							case 0:
								b = 0.7065; t = 8.7065; l = 2.2765; r = 10.2765;
								break;
							case 1:
								switch(z)
								{
								case 0:
									b = 0.471; t = 4.471; l = 1.5176; r = 5.5176;
									break;
								case 1:
									b = 0.471; t = 4.471; l = 7.0352; r = 11.0352;
									break;
								case 2:
									b = 4.942; t = 8.942; l = 1.5176; r = 5.5176;
									break;
								case 3:
									b = 4.942; t = 8.942; l = 7.0352; r = 11.0352;
									break;
								}
								break;
							case 2:
								switch(z)
								{
								case 0:
									b = 0.471; t = 5.471; l = 3.7765; r = 8.7765;
									break;
								case 1:
									b = 0.2065; t = 3.2065; l = 9.2765; r = 12.2765;
									break;
								case 2:
									b = 0.2065; t = 3.2065; l = 0.2765; r = 3.2765;
									break;
								case 3:
									b = 6.2065; t = 9.2065; l = 9.2765; r = 12.2765;
									break;
								}
								break;
							case 3:
								switch(z)
								{
								case 0:
									b = 0.471; t = 5.471; l = 3.7765; r = 8.7765;
									break;
								case 1:
									b = 6.2065; t = 9.2065; l = 9.2765; r = 12.2765;
									break;
								case 2:
									b = 0.2065; t = 3.2065; l = 9.2765; r = 12.2765;
									break;
								case 3:
									b = 6.2065; t = 9.2065; l = 0.2765; r = 3.2765;
									break;
								}
								break;
							}
						}
						int nGroup = HIWORD(bsc.dwDBSQuestionsEx[y*40+x*4+z]);
						int nNumber = LOWORD(bsc.dwDBSQuestionsEx[y*40+x*4+z]);
						g_gmMain->BindTexture( bs->skin[0]->GetCurrentTex());
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//x==0?-50.004:-50.005);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//x==0?-50.004:-50.005);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//x==0?-50.004:-50.005);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//x==0?-50.004:-50.005);
						glEnd();
						CString str;
						str.Format(_T("%d"), nNumber);
						double dSizeCaret = r-l;
						double dMinL = (l+dSizeCaret/2)-dSizeCaret/4*str.GetLength();
						for(int w = 0; w < str.GetLength(); w++)
						{
							double ll = dMinL+dSizeCaret/2*(double)w;
							double rr = dMinL+dSizeCaret/2*(double)(w+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(w);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							g_gmMain->BindTexture( bs->skin[nGroup]->texture[nAcc]);
							g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
							g_gmMain->Begin(PMG_QUADS);
							g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//x==0?-50.002:-50.003);
							g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//x==0?-50.002:-50.003);
							g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//x==0?-50.002:-50.003);
							g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//x==0?-50.002:-50.003);
							glEnd();
						}
					}
				}
				glPopMatrix();
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=0.0; b=-30.0; l=-20.0; r=20.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->BindTexture( bs->skin[13]->texture[0]);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);
		glEnd();
		glPopMatrix();
	}
}

void CMaxStepSmu::DrawBS03(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	int nSubject = HIWORD(bsc.dwBSQuestions[y]);
	float dOffIQuest = bsc.dWIBSQuestions[y];
	float dOffAQuest = bsc.dWABSQuestions[y];
	float dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nTime;
	///////////////////////////////
	double t=40.0, b=-20.0, l=-3.5, r=3.5;
	double _t=-20.0, _b=-30.0, _l=-5.0, _r=5.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[12]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
		g_gmMain->PushMatrix();
		glTranslated(0.0, -10.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[17]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(_l, _t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(_r, _t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(_r, _b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(_l, _b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo
	{
		g_gmMain->BindTexture( bs->skin[12]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		g_gmMain->BindTexture( bs->skin[17]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(_l, _t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(_r, _t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(_r, _b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(_l, _b, -50.0);//-50.006);
		glEnd();
		int nSize = LOWORD(bsc.dwDBSQuestionsEx[y*40+1]);
		double dDist = (double)LOWORD(bsc.dwDBSQuestionsEx[y*40]);
		double dSizeObj = 6.0;
		double dDistObj = 7.0;
		double h = ((t-b)+((double)nSize-1)*dDistObj)*dDist;	// La altura del panel + NumerodeItems*Dist entre ellas
		for(int x = 0; x < nSize; x++)
		{
			double dMult = ((double)(nBeat-nOffIQuest))/((double)(nOffAQuest-nOffIQuest));
			b = ((double)x)*dDistObj*dDist;
			b -= h*dMult;
			b += 30.0;
			if(b <= 30.0 && b >= -20.0)	// Se encuentra en los límites
			{
				t = b+dSizeObj; l = -3.0; r = l+dSizeObj;
				if(bsc.dwDBSQuestions[y*30+x])
				{
					DWORD dwDArrow = LOWORD(bsc.dwDBSQuestions[y*30+x]);
					DWORD dwGArrow = HIWORD(bsc.dwDBSQuestions[y*30+x]);
					if(dwGArrow == 1)
					{
						g_gmMain->BindTexture( bs->skin[0]->GetCurrentTex());
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.005);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.005);
						glEnd();
					}
					if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
					{
						CString str;
						if(nType == 2 && nSubject == 3 && dwDArrow == 17) str = _T("?");
						else str.Format(_T("%d"), dwDArrow);
						double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
						for(int z = 0; z < str.GetLength(); z++)
						{
							double ll = dMinL+dSizeObj/2*(double)z;
							double rr = dMinL+dSizeObj/2*(double)(z+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(z);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[nAcc]);
							g_gmMain->Begin(PMG_QUADS);
							g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
							g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.003);
							g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.003);
							g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.004);
							g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.004);
							glEnd();
						}
					}
					else
					{
						/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
						double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
						g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[dwDArrow]);
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
						glEnd();
					}
				}
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=40.0; b=-20.0; l=-3.5; r=3.5;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->BindTexture( bs->skin[12]->texture[0]);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[12]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
		g_gmMain->PushMatrix();
		glTranslated(0.0, -10.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[17]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(_l, _t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(_r, _t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(_r, _b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(_l, _b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

void CMaxStepSmu::DrawBS04(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	float dOffIQuest = bsc.dWIBSQuestions[y];
	float dOffAQuest = bsc.dWABSQuestions[y];
	float dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nTime;
	///////////////////////////////
	double t=0.0, b=-30.0, l=-20.0, r=20.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffAQuest - nBeat) > 0)	// Nudo
	{
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		// Dibujo principal (mostrar todo)
		for(int x = 0; x < 9; x++)
		{
			b = 0.7065; t = 8.7065; l = 2.2765; r = 10.2765;
			double dSizeObj = r-l;
			g_gmMain->PushMatrix();
			switch(x)
			{
			case 0:
				glTranslated(-18.83, -10.293, 0.0);
				break;
			case 1:
				glTranslated(-6.277, -10.293, 0.0);
				break;
			case 2:
				glTranslated(6.276, -10.293, 0.0);
				break;
			case 3:
				glTranslated(-18.83, -19.706, 0.0);
				break;
			case 4:
				glTranslated(-6.277, -19.706, 0.0);
				break;
			case 5:
				glTranslated(6.276, -19.706, 0.0);
				break;
			case 6:
				glTranslated(-18.83, -28.239, 0.0);
				break;
			case 7:
				glTranslated(-6.277, -28.239, 0.0);
				break;
			case 8:
				glTranslated(6.276, -28.239, 0.0);
				break;
			}
			if(bsc.dwDBSQuestionsEx[y*40+x])
			{
				DWORD dwDArrow = LOWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				DWORD dwGArrow = HIWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				if(dwGArrow == 1)
				{
					g_gmMain->BindTexture( bs->skin[0]->GetCurrentTex());
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t,-50.0);// -50.004);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b,-50.0);// -50.005);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.005);
					glEnd();
				}
				if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
				{
					CString str;
					str.Format(_T("%d"), dwDArrow);
					double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
					for(int z = 0; z < str.GetLength(); z++)
					{
						double ll = dMinL+dSizeObj/2*(double)z;
						double rr = dMinL+dSizeObj/2*(double)(z+1);
						int nAcc = 0;
						TCHAR chAcc = str.GetAt(z);
						if(chAcc >= '0' && chAcc <= '9')
							nAcc = chAcc - '0';
						else nAcc = 17;
						g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[nAcc]);
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.004);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.004);
						glEnd();
					}
				}
				else
				{
					/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
					double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
					g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[dwDArrow]);
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.003);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.003);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
					glEnd();
				}
			}
			glPopMatrix();
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=0.0; b=-30.0; l=-20.0; r=20.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->BindTexture( bs->skin[13]->texture[0]);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffAQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo 2
	{
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		// Dibujo secundario (mostrar de aceurdo al display)
		for(int x = 0; x < 9; x++)
		{
			if(m_nTime >= bsc.dwDBSQuestionsEx[y*40+10+x] &&
				m_nTime <= bsc.dwDBSQuestionsEx[y*40+20+x])
			{
				b = 0.7065; t = 8.7065; l = 2.2765; r = 10.2765;
				double dSizeObj = r-l;
				g_gmMain->PushMatrix();
				switch(x)
				{
				case 0:
					glTranslated(-18.83, -10.293, 0.0);
					break;
				case 1:
					glTranslated(-6.277, -10.293, 0.0);
					break;
				case 2:
					glTranslated(6.276, -10.293, 0.0);
					break;
				case 3:
					glTranslated(-18.83, -19.706, 0.0);
					break;
				case 4:
					glTranslated(-6.277, -19.706, 0.0);
					break;
				case 5:
					glTranslated(6.276, -19.706, 0.0);
					break;
				case 6:
					glTranslated(-18.83, -28.239, 0.0);
					break;
				case 7:
					glTranslated(-6.277, -28.239, 0.0);
					break;
				case 8:
					glTranslated(6.276, -28.239, 0.0);
					break;
				}
				if(bsc.dwDBSQuestionsEx[y*40+x])
				{
					DWORD dwDArrow = LOWORD(bsc.dwDBSQuestionsEx[y*40+x]);
					DWORD dwGArrow = HIWORD(bsc.dwDBSQuestionsEx[y*40+x]);
					if(dwGArrow == 1)
					{
						g_gmMain->BindTexture( bs->skin[0]->GetCurrentTex());
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.005);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.005);
						glEnd();
					}
					if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
					{
						CString str;
						str.Format(_T("%d"), dwDArrow);
						double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
						for(int z = 0; z < str.GetLength(); z++)
						{
							double ll = dMinL+dSizeObj/2*(double)z;
							double rr = dMinL+dSizeObj/2*(double)(z+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(z);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[nAcc]);
							g_gmMain->Begin(PMG_QUADS);
							g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
							g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.003);
							g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.003);
							g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.004);
							g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.004);
							glEnd();
						}
					}
					else
					{
						/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
						double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
						g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[dwDArrow]);
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
						glEnd();
					}
				}
				glPopMatrix();
			}
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// End
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t,-50.0);// -50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

void CMaxStepSmu::DrawBS05(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	float dOffIQuest = bsc.dWIBSQuestions[y];
	float dOffAQuest = bsc.dWABSQuestions[y];
	float dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nTime;
	///////////////////////////////
	double t=0.0, b=-30.0, l=-20.0, r=20.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		t=0.0; b=-30.0; l=-20.0; r=20.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
		t=60.0; b=0.0; l=-3.5; r=3.5;
		g_gmMain->PushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[12]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffAQuest - nBeat) > 0)	// Nudo
	{
		t=0.0; b=-30.0; l=-20.0; r=20.0;
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		t=60.0; b=0.0; l=-3.5; r=3.5;
		g_gmMain->BindTexture( bs->skin[12]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		int nSize = 5;
		double dSizeObj = 6.0;
		double dDistObj = 7.0;
		double h = ((40)+((double)nSize-1)*dDistObj);	// La altura del panel + NumerodeItems*Dist entre ellas
		for(int x = 0; x < nSize; x++)
		{
			double dMult = ((double)(nBeat-nOffIQuest))/((double)(nOffAQuest-nOffIQuest));
			b = ((double)x)*dDistObj;
			b -= h*dMult;
			b += 30.0;
			if(b <=40.0 && b >= 0.0)	// Se encuentra en los límites
			{
				t = b+dSizeObj; l = -3.0; r = l+dSizeObj;
				if(bsc.dwDBSQuestions[y*30+x])
				{
					DWORD dwDArrow = LOWORD(bsc.dwDBSQuestions[y*30+x]);
					DWORD dwGArrow = HIWORD(bsc.dwDBSQuestions[y*30+x]);
					if(dwGArrow == 1)
					{
						g_gmMain->BindTexture( bs->skin[0]->GetCurrentTex());
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.005);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.005);
						glEnd();
					}
					if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
					{
						CString str;
						str.Format(_T("%d"), dwDArrow);
						double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
						for(int z = 0; z < str.GetLength(); z++)
						{
							double ll = dMinL+dSizeObj/2*(double)z;
							double rr = dMinL+dSizeObj/2*(double)(z+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(z);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[nAcc]);
							g_gmMain->Begin(PMG_QUADS);
							g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
							g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.003);
							g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.003);
							g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.004);
							g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.004);
							glEnd();
						}
					}
					else
					{
						/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
						double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
						g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[dwDArrow]);
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
						glEnd();
					}
				}
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=0.0; b=-30.0; l=-20.0; r=20.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->BindTexture( bs->skin[13]->texture[0]);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
			glEnd();
			glPopMatrix();
			t=60.0; b=0.0; l=-3.5; r=3.5;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->BindTexture( bs->skin[12]->texture[0]);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.003);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.003);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.003);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.003);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffAQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo 2
	{
		t=0.0; b=-30.0; l=-20.0; r=20.0;
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		t=60.0; b=0.0; l=-3.5; r=3.5;
		g_gmMain->BindTexture( bs->skin[12]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		// Dibujo secundario (mostrar de aceurdo al display)
		for(int x = 0; x < 9; x++)
		{
			if(m_nTime <= bsc.dwDBSQuestionsEx[y*40+5+x] ||
				(m_nTime >= (bsc.dwDBSQuestionsEx[y*40+5+x]+500)
				&& m_nTime <= (bsc.dwDBSQuestionsEx[y*40+5+x]+1000)))
			{
				b = 0.7065; t = 8.7065; l = 2.2765; r = 10.2765;
				double dSizeObj = r-l;
				g_gmMain->PushMatrix();
				switch(x)
				{
				case 0:
					glTranslated(-18.83, -28.239, 0.0);
					break;
				case 4:
					glTranslated(6.276, -28.239, 0.0);
					break;
				case 1:
					glTranslated(-18.83, -10.293, 0.0);
					break;
				case 3:
					glTranslated(6.276, -10.293, 0.0);
					break;
				default:
					glTranslated(-6.277, -19.706, 0.0);
					break;
				}
				if(bsc.dwDBSQuestionsEx[y*40+x])
				{
					DWORD dwDArrow = LOWORD(bsc.dwDBSQuestionsEx[y*40+x]);
					DWORD dwGArrow = HIWORD(bsc.dwDBSQuestionsEx[y*40+x]);
					if(dwGArrow == 1)
					{
						g_gmMain->BindTexture( bs->skin[0]->GetCurrentTex());
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.005);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.005);
						glEnd();
					}
					if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
					{
						CString str;
						str.Format(_T("%d"), dwDArrow);
						double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
						for(int z = 0; z < str.GetLength(); z++)
						{
							double ll = dMinL+dSizeObj/2*(double)z;
							double rr = dMinL+dSizeObj/2*(double)(z+1);
							int nAcc = 0;
							TCHAR chAcc = str.GetAt(z);
							if(chAcc >= '0' && chAcc <= '9')
								nAcc = chAcc - '0';
							else nAcc = 17;
							g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[nAcc]);
							g_gmMain->Begin(PMG_QUADS);
							g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
							g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.003);
							g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.003);
							g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.004);
							g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.004);
							glEnd();
						}
					}
					else
					{
						/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
						double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
						g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[dwDArrow]);
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
						glEnd();
					}
				}
				glPopMatrix();
			}
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// End
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		t=0.0; b=-30.0; l=-20.0; r=20.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
		t=60.0; b=0.0; l=-3.5; r=3.5;
		g_gmMain->PushMatrix();
		glTranslated(0.0, 50.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[12]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

void CMaxStepSmu::DrawBS06(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	int nSubject = HIWORD(bsc.dwBSQuestions[y]);
	float dOffIQuest = bsc.dWIBSQuestions[y];
	float dOffAQuest = bsc.dWABSQuestions[y];
	float dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nTime;
	///////////////////////////////
	double t=0.0, b=-30.0, l=-20.0, r=20.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffAQuest - nBeat) > 0)	// Nudo
	{
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		double dSizeObj = 4.0;
		double dDistObjX = 6.2766;
		double dDistObjY = 4.7066;
		for(int x = 0; x < 36; x++)
		{
			int ox = x%6;
			int oy = x/6;
			if(m_nTime >= bsc.dwDBSQuestions[y*30+ox] && m_nTime >= bsc.dwDBSQuestions[y*30+oy+6] && bsc.dwDBSQuestionsEx[y*40+x])
			{
				t = -dDistObjY*((double)oy)-1.351; b = t-dSizeObj;
				l = dDistObjX*((double)ox)-17.83; r = l+dSizeObj;
				DWORD dwDArrow = LOWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				DWORD dwGArrow = HIWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				if(dwGArrow == 1)
				{
					g_gmMain->BindTexture( bs->skin[0]->GetCurrentTex());
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.005);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.005);
					glEnd();
				}
				if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
				{
					CString str;
					str.Format(_T("%d"), dwDArrow);
					double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
					for(int z = 0; z < str.GetLength(); z++)
					{
						double ll = dMinL+dSizeObj/2*(double)z;
						double rr = dMinL+dSizeObj/2*(double)(z+1);
						int nAcc = 0;
						TCHAR chAcc = str.GetAt(z);
						if(chAcc >= '0' && chAcc <= '9')
							nAcc = chAcc - '0';
						else nAcc = 17;
						g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[nAcc]);
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.004);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.004);
						glEnd();
					}
				}
				else
				{
					/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
					double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
					g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[dwDArrow]);
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.003);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.003);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
					glEnd();
				}
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=0.0; b=-30.0; l=-20.0; r=20.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->BindTexture( bs->skin[13]->texture[0]);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffAQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo 2
	{
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		double dSizeObj = 4.0;
		double dDistObjX = 6.2766;
		double dDistObjY = 4.7066;
		for(int x = 0; x < 36; x++)
		{
			int ox = x%6;
			int oy = x/6;
			if(m_nTime >= bsc.dwDBSQuestions[y*30+ox] && m_nTime >= bsc.dwDBSQuestions[y*30+oy+6] && (bsc.dwDBSQuestionsEx[y*40+x] == bsc.dwDBSQuestions[y*30+13]))
			{
				t = -dDistObjY*((double)oy)-1.351; b = t-dSizeObj;
				l = dDistObjX*((double)ox)-17.83; r = l+dSizeObj;
				DWORD dwDArrow = 17;//LOWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				DWORD dwGArrow = 1;HIWORD(bsc.dwDBSQuestionsEx[y*40+x]);
				if(dwGArrow == 1)
				{
					g_gmMain->BindTexture( bs->skin[0]->GetCurrentTex());
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.005);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.005);
					glEnd();
				}
				if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
				{
					CString str;
					if(nType == 4 && nSubject == 1) str = _T("?");
					else str.Format(_T("%d"), dwDArrow);
					double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
					for(int z = 0; z < str.GetLength(); z++)
					{
						double ll = dMinL+dSizeObj/2*(double)z;
						double rr = dMinL+dSizeObj/2*(double)(z+1);
						int nAcc = 0;
						TCHAR chAcc = str.GetAt(z);
						if(chAcc >= '0' && chAcc <= '9')
							nAcc = chAcc - '0';
						else nAcc = 17;
						g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[nAcc]);
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.003);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.004);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.004);
						glEnd();
					}
				}
				else
				{
					/*double ll = l+(dwGArrow == 1?(dSizeObj/4):0);
					double rr = r-(dwGArrow == 1?(dSizeObj/4):0);*/
					g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[dwDArrow]);
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.003);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.003);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
					glEnd();
				}
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			t=0.0; b=-30.0; l=-20.0; r=20.0;
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->BindTexture( bs->skin[13]->texture[0]);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, -30.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[13]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

void CMaxStepSmu::DrawBS07(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	int nSubject = HIWORD(bsc.dwBSQuestions[y]);
	float dOffIQuest = bsc.dWIBSQuestions[y];
	float dOffAQuest = bsc.dWABSQuestions[y];
	float dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nTime;
	///////////////////////////////
	double dSizeItem = 2*m_dSizeArrows/3;
	double t=24.0-dSizeItem, b=24.0-dSizeItem*16, l=-dSizeItem, r=0.0;
	if((nOffIQuest - nBeat) < 800 && (nOffIQuest - nBeat) > 0)	// Intro
	{
		double dCarUp = ((double)nOffIQuest - (double)nBeat)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, 80.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[14]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
	if((nOffIQuest - nBeat) <= 0 && (nOffAQuest - nBeat) > 0)	// Nudo
	{
		g_gmMain->BindTexture( bs->skin[14]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		int nSize = bsc.dwDBSQuestionsEx[y*40+30];
		for(int x = 0; x < nSize; x++)
		{
			if(m_nTime >= bsc.dwDBSQuestionsEx[y*40+x])
			{
				t = 24.0-dSizeItem-(dSizeItem*(double)x); b = t-dSizeItem; l=-dSizeItem, r=0.0;
				DWORD dwDArrow = LOWORD(bsc.dwDBSQuestions[y*30+x]);
				DWORD dwGArrow = HIWORD(bsc.dwDBSQuestions[y*30+x]);
				if(dwGArrow == 1)
				{
					g_gmMain->BindTexture( bs->skin[0]->GetCurrentTex());
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.005);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.005);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.005);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.005);
					glEnd();
				}
				if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
				{
					CString str;
					str.Format(_T("%d"), dwDArrow);
					double dMinL = (l+dSizeItem/2)-dSizeItem/4*str.GetLength();
					for(int z = 0; z < str.GetLength(); z++)
					{
						double ll = dMinL+dSizeItem/2*(double)z;
						double rr = dMinL+dSizeItem/2*(double)(z+1);
						int nAcc = 0;
						TCHAR chAcc = str.GetAt(z);
						if(chAcc >= '0' && chAcc <= '9')
							nAcc = chAcc - '0';
						else nAcc = 17;
						g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[nAcc]);
						g_gmMain->Begin(PMG_QUADS);
						g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
						g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.004);
						g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.004);
						g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.004);
						g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.004);
						glEnd();
					}
				}
				else
				{
					g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[dwDArrow]);
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
					glEnd();
				}
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			t=24.0-m_dSizeArrows; b=24.0-m_dSizeArrows*8; l=-m_dSizeArrows/2; r=0.0;
			g_gmMain->BindTexture( bs->skin[14]->texture[0]);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.003);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.003);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.003);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.003);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffAQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > 0)	// Nudo
	{
		g_gmMain->BindTexture( bs->skin[14]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		if(nSubject == 3 && nType == 4)
		{
			t=-23.0; b=-30.0; l=-40.0; r=40.0;
			g_gmMain->BindTexture( bs->skin[15]->texture[0]);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
			glEnd();
		}
		int nSize = bsc.dwDBSQuestionsEx[y*40+30];
		for(int x = 0; x < nSize; x++)
		{
			t = 24.0-dSizeItem-(dSizeItem*(double)x); b = t-dSizeItem; l=-dSizeItem, r=0.0;
			g_gmMain->BindTexture( bs->skin[18]->texture[0]);
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.005);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.005);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.005);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.005);
			glEnd();

			if(nSubject == 3 && nType == 4)
			{
				t=-23.0; b=-30.0; double dSizeObj = 7.0;
				CString str;
				int nAng = 0;
				switch(bsc.dwDBSQuestionsEx[y*40+31])
				{
				case 0:
					nAng = 90; break;
				case 1:
					nAng = 180; break;
				default:
					nAng = 270; break;
				}
				str.Format(_T("%d"), nAng);
				double dMinL = (l+dSizeObj/2)-dSizeObj/4*str.GetLength();
				for(int z = 0; z < str.GetLength(); z++)
				{
					double ll = dMinL+dSizeObj/2*(double)z;
					double rr = dMinL+dSizeObj/2*(double)(z+1);
					int nAcc = 0;
					TCHAR chAcc = str.GetAt(z);
					if(chAcc >= '0' && chAcc <= '9')
						nAcc = chAcc - '0';
					else nAcc = 17;
					g_gmMain->BindTexture( bs->skin[1]->texture[nAcc]);
					g_gmMain->Begin(PMG_QUADS);
					g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
					g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.003);
					g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.003);
					g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.003);
					g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.003);
					glEnd();
				}
			}
		}
		if((nOffAQuest - nBeat) >= -500 && nSubject == 3 && nType == 4)
		{
			t=-23.0; b=-30.0; l=-40.0; r=40.0;
			double dApGlout = abs((double)nOffAQuest - (double)nBeat)/500;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->BindTexture( bs->skin[15]->texture[0]);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.002);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.002);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.002);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.002);
			glEnd();
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		g_gmMain->PushMatrix();
		glTranslated(0.0, 80.0*dCarUp, 0.0);
		g_gmMain->BindTexture( bs->skin[14]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}

void CMaxStepSmu::DrawBS08(int y)
{
	// Prototipo general de tiempos
	int nType = LOWORD(bsc.dwBSQuestions[y]);
	int nSubject = HIWORD(bsc.dwBSQuestions[y]);
	float dOffIQuest = bsc.dWIBSQuestions[y];
	float dOffAQuest = bsc.dWABSQuestions[y];
	float dOffEQuest = bsc.dWEBSQuestions[y];
	__int64 nOffIQuest = BeatToTime(dOffIQuest);
	__int64 nOffAQuest = BeatToTime(dOffAQuest);
	__int64 nOffEQuest = BeatToTime(dOffEQuest);
	__int64 nBeat = m_nTime;
	///////////////////////////////
	double t=15.0, b=-15.0, l=-15.0, r=15.0; double dSizeItem = t-b;
	if((nOffIQuest - nBeat) <= 0 && (nOffAQuest - nBeat) > 0)	// Nudo
	{
		g_gmMain->BindTexture( bs->skin[19]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		t=7.5; b=-7.5; l=-7.5; r=7.5;
		DWORD dwDArrow = LOWORD(bsc.dwDBSQuestionsEx[y*40]);
		DWORD dwGArrow = HIWORD(bsc.dwDBSQuestionsEx[y*40]);
		if(dwGArrow == 1)
		{
			g_gmMain->BindTexture( bs->skin[0]->GetCurrentTex());
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.005);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.005);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.005);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.005);
			glEnd();
		}
		if(dwGArrow == 1) // Es un número, y debemos mostrarlo como tal
		{
			CString str;
			str.Format(_T("%d"), dwDArrow);
			double dMinL = (l+dSizeItem/2)-dSizeItem/4*str.GetLength();
			for(int z = 0; z < str.GetLength(); z++)
			{
				double ll = dMinL+dSizeItem/2*(double)z;
				double rr = dMinL+dSizeItem/2*(double)(z+1);
				int nAcc = 0;
				TCHAR chAcc = str.GetAt(z);
				if(chAcc >= '0' && chAcc <= '9')
					nAcc = chAcc - '0';
				else nAcc = 17;
				g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[nAcc]);
				g_gmMain->Begin(PMG_QUADS);
				g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
				g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.004);
				g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.004);
				g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.004);
				g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.004);
				glEnd();
			}
		}
		else
		{
			g_gmMain->BindTexture( bs->skin[dwGArrow]->texture[dwDArrow]);
			g_gmMain->Begin(PMG_QUADS);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
			glEnd();
		}
		if(nSubject == 3 && nType == 3)
		{
			t=-15.0; b=-22.0; l=-40.0; r=40.0;
			g_gmMain->BindTexture( bs->skin[15]->texture[0]);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.004);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.004);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.004);
			glEnd();
			t=-15.0; b=-22.0; double dSizeObj = 7.0;
			CString str;
			int nAng = 0;
			switch(bsc.dwDBSQuestionsEx[y*40+31])
			{
			case 0:
				nAng = 90; break;
			case 1:
				nAng = 180; break;
			default:
				nAng = 270; break;
			}
			str.Format(_T("%d"), nAng);
			double dMinL = (dSizeObj/2)-dSizeObj/4*str.GetLength();
			for(int z = 0; z < str.GetLength(); z++)
			{
				double ll = dMinL+dSizeObj/2*(double)z;
				double rr = dMinL+dSizeObj/2*(double)(z+1);
				int nAcc = 0;
				TCHAR chAcc = str.GetAt(z);
				if(chAcc >= '0' && chAcc <= '9')
					nAcc = chAcc - '0';
				else nAcc = 17;
				g_gmMain->BindTexture( bs->skin[1]->texture[nAcc]);
				g_gmMain->Begin(PMG_QUADS);
				g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
				g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(ll, t, -50.0);//-50.003);
				g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(rr, t, -50.0);//-50.003);
				g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(rr, b, -50.0);//-50.003);
				g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(ll, b, -50.0);//-50.003);
				glEnd();
			}
		}
		if((nOffIQuest - nBeat) >= -500)
		{
			double dApGlout = abs((double)nOffIQuest - (double)nBeat)/500;
			g_gmMain->PushMatrix();
			g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
			t=15.0; b=-15.0; l=-15.0; r=15.0;
			g_gmMain->BindTexture( bs->skin[19]->texture[0]);
			g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
			g_gmMain->Begin(PMG_POLYGON);
			g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.003);
			g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.003);
			g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.003);
			g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.003);
			glEnd();
			if(nSubject == 3 && nType == 3)
			{
				t=-15.0; b=-22.0; l=-40.0; r=40.0;
				g_gmMain->Scale(1.0+dApGlout, 1.0+dApGlout, 1.0);
				g_gmMain->Color4d(1.0, 1.0, 1.0, 1-dApGlout);
				g_gmMain->BindTexture( bs->skin[15]->texture[0]);
				g_gmMain->Begin(PMG_POLYGON);
				g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.002);
				g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.002);
				g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.002);
				g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.002);
				glEnd();
			}
			glPopMatrix();
		}
	}
	if((nOffEQuest - nBeat) <= 0 && (nOffEQuest - nBeat) > -800 )	// Intro
	{
		double dCarUp = abs((double)nBeat -(double)nOffEQuest)/800.0;
		g_gmMain->PushMatrix();
		g_gmMain->Scale(1.0+dCarUp, 1.0+dCarUp, 1.0);
		g_gmMain->BindTexture( bs->skin[19]->texture[0]);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0-dCarUp);
		g_gmMain->Begin(PMG_POLYGON);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(l, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(r, t, -50.0);//-50.006);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(r, b, -50.0);//-50.006);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(l, b, -50.0);//-50.006);
		glEnd();
		glPopMatrix();
	}
}
