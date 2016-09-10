#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "HistoryLoader.h"

CHistoryLoader::CHistoryLoader(void)
{
	m_nSizeAnu = 0;
	m_nSizeStops = 0;
	m_nSizeWaitCenter = 0;
	m_nSizeChangeTPL = 0;
	m_nSizeShowAnu = 0;
	m_nSizeSounds = 0;
	m_nSizeLoopSounds = 0;
	m_nSizePlaySound = 0;
	m_nSizePlayLoopSound = 0;
	m_nPlayingLoop = -1;
	m_nPlaying = -1;
	m_uTexText = 0;
	m_nWStops = (int*)calloc(0, sizeof(int));
	m_nStops = (int*)calloc(0, sizeof(int));
	m_nWaitCenter = (int*)calloc(0, sizeof(int));
	m_nWChangeTPL = (int*)calloc(0, sizeof(int));
	m_nChangeTPL = (int*)calloc(0, sizeof(int));
	m_nWShowAnu = (int*)calloc(0, sizeof(int));
	m_nShowAnu = (int*)calloc(0, sizeof(int));
	m_nWPlaySound = (int*)calloc(0, sizeof(int));
	m_nPlaySound = (int*)calloc(0, sizeof(int));
	m_nWPlayLoopSound = (int*)calloc(0, sizeof(int));
	m_nPlayLoopSound = (int*)calloc(0, sizeof(int));
}

CHistoryLoader::~CHistoryLoader(void)
{
	Destroy();
}

int CHistoryLoader::LoadText(LPCTSTR chName)
{
	m_uTexText = GenTexImage(_T("History/TextBox.png"));
	FILE* hFile = _fopen(chName, _T("r"));
	if(hFile == NULL) return 0;
	CString strPar;
	CString strArg;
	TCHAR ch[260];
	int nCurAnu = -1;
	int nCurSound = -1;
	int nCurLoopSound = -1;
	int nCur = 0;
	SetZeroCount();
	while(_fgets(ch, 260, hFile) != NULL)
	{
		strPar = ( ch);
		int nCode = 0;
		if(strPar.Find(_T('#')) == -1 && strPar.Find(_T("//")) == -1)
		{
			m_strText += strPar;
			nCur = m_strText.GetLength();
		}
		else
		{
			strPar.Remove(_T('\n'));
			if((nCode = strPar.Find(_T("#SIZEANU"))) != -1)
			{
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
				strArg.Remove(_T(' '));
				strArg.Remove(_T('\t'));
				if(_sscanf(strArg, _T("%d"), &m_nSizeAnu) != 1) {fclose(hFile); return 0;}

				m_anu = new CAnu*[m_nSizeAnu];
				for(int x = 0; x < m_nSizeAnu; x++)
				{
					m_anu[x] = new CAnu();
				}
			}
			if((nCode = strPar.Find(_T("#ANU"))) != -1)
			{
				nCurAnu++;
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				m_anu[nCurAnu]->Open(strPar.Mid(nCode+1, strPar.GetLength()-nCode-1));
			}

			if((nCode = strPar.Find(_T("#SIZESOUNDS"))) != -1)
			{
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
				strArg.Remove(_T(' '));
				strArg.Remove(_T('\t'));
				if(_sscanf(strArg, _T("%d"), &m_nSizeSounds) != 1) {fclose(hFile); return 0;}

				m_soundSounds = new FMOD_SOUND*[m_nSizeSounds];
				for(int x = 0; x < m_nSizeSounds; x++)
				{
					m_soundSounds[x] = 0;
				}
			}
			if((nCode = strPar.Find(_T("#SOUND"))) != -1)
			{
				nCurSound++;
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
	#ifdef UNICODE
				char chFile[260];
				UnicodeToAnsi(strPar.Mid(nCode+1, strPar.GetLength()-nCode-1), chFile);
				FMOD_System_CreateStream(sys, chFile, FMOD_HARDWARE | FMOD_2D, 0, &m_soundSounds[nCurSound]);
	#else
				FMOD_System_CreateStream(sys, strPar.Mid(nCode+1, strPar.GetLength()-nCode-1), FMOD_HARDWARE | FMOD_2D, 0, &m_soundSounds[nCurSound]);
	#endif
			}

			if((nCode = strPar.Find(_T("#SIZELOOPSOUNDS"))) != -1)
			{
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
				strArg.Remove(_T(' '));
				strArg.Remove(_T('\t'));
				if(_sscanf(strArg, _T("%d"), &m_nSizeLoopSounds) != 1) {fclose(hFile); return 0;}

				m_soundLoopSounds = new FMOD_SOUND*[m_nSizeLoopSounds];
				for(int x = 0; x < m_nSizeLoopSounds; x++)
				{
					m_soundLoopSounds[x] = 0;
				}
			}
			if((nCode = strPar.Find(_T("#LOOPSOUND"))) != -1)
			{
				nCurLoopSound++;
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
	#ifdef UNICODE
				char chFile[260];
				UnicodeToAnsi(strPar.Mid(nCode+1, strPar.GetLength()-nCode-1), chFile);
				FMOD_System_CreateStream(sys, chFile, FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_NORMAL, 0, &m_soundLoopSounds[nCurLoopSound]);
	#else
				FMOD_System_CreateStream(sys, strPar.Mid(nCode+1, strPar.GetLength()-nCode-1), FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_NORMAL, 0, &m_soundLoopSounds[nCurLoopSound]);
	#endif
			}

			if((nCode = strPar.Find(_T("#TIMEPERLETTER"))) != -1)
			{
				m_nSizeChangeTPL++;
				m_nWChangeTPL = (int*)realloc(m_nWChangeTPL, m_nSizeChangeTPL*sizeof(int));
				m_nChangeTPL = (int*)realloc(m_nChangeTPL, m_nSizeChangeTPL*sizeof(int));
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
				strArg.Remove(_T(' '));
				strArg.Remove(_T('\t'));
				if(_sscanf(strArg, _T("%d"), &m_nChangeTPL[m_nSizeChangeTPL-1]) != 1) {fclose(hFile); return 0;}
				m_nWChangeTPL[m_nSizeChangeTPL-1] = nCur;
			}
			if((nCode = strPar.Find(_T("#STOPS"))) != -1)
			{
				m_nSizeStops++;
				m_nWStops = (int*)realloc(m_nWStops, m_nSizeStops*sizeof(int));
				m_nStops = (int*)realloc(m_nStops, m_nSizeStops*sizeof(int));
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
				strArg.Remove(_T(' '));
				strArg.Remove(_T('\t'));
				if(_sscanf(strArg, _T("%d"), &m_nStops[m_nSizeStops-1]) != 1) {fclose(hFile); return 0;}
				m_nWStops[m_nSizeStops-1] = nCur;
			}
			if((nCode = strPar.Find(_T("#SHOWANU"))) != -1)
			{
				m_nSizeShowAnu++;
				m_nWShowAnu = (int*)realloc(m_nWShowAnu, m_nSizeShowAnu*sizeof(int));
				m_nShowAnu = (int*)realloc(m_nShowAnu, m_nSizeShowAnu*sizeof(int));
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
				strArg.Remove(_T(' '));
				strArg.Remove(_T('\t'));
				if(_sscanf(strArg, _T("%d"), &m_nShowAnu[m_nSizeShowAnu-1]) != 1) {fclose(hFile); return 0;}
				m_nWShowAnu[m_nSizeShowAnu-1] = nCur;
			}
			if((nCode = strPar.Find(_T("#WAITCENTER"))) != -1)
			{
				m_nSizeWaitCenter++;
				m_nWaitCenter = (int*)realloc(m_nWaitCenter, m_nSizeWaitCenter*sizeof(int));
				m_nWaitCenter[m_nSizeWaitCenter-1] = nCur;
			}
			if((nCode = strPar.Find(_T("#PLAYSOUND"))) != -1)
			{
				m_nSizePlaySound++;
				m_nWPlaySound = (int*)realloc(m_nWPlaySound, m_nSizePlaySound*sizeof(int));
				m_nPlaySound = (int*)realloc(m_nPlaySound, m_nSizePlaySound*sizeof(int));
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
				strArg.Remove(_T(' '));
				strArg.Remove(_T('\t'));
				if(_sscanf(strArg, _T("%d"), &m_nPlaySound[m_nSizePlaySound-1]) != 1) {fclose(hFile); return 0;}
				m_nWPlaySound[m_nSizePlaySound-1] = nCur;
			}
			if((nCode = strPar.Find(_T("#PLAYLOOPSOUND"))) != -1)
			{
				m_nSizePlayLoopSound++;
				m_nWPlayLoopSound = (int*)realloc(m_nWPlayLoopSound, m_nSizePlayLoopSound*sizeof(int));
				m_nPlayLoopSound = (int*)realloc(m_nPlayLoopSound, m_nSizePlayLoopSound*sizeof(int));
				nCode = strPar.Find(_T(':'));
				if(nCode == -1) {fclose(hFile); return 0;}
				strArg = strPar.Mid(nCode+1, strPar.GetLength()-nCode-1);
				strArg.Remove(_T(' '));
				strArg.Remove(_T('\t'));
				if(_sscanf(strArg, _T("%d"), &m_nPlayLoopSound[m_nSizePlayLoopSound-1]) != 1) {fclose(hFile); return 0;}
				m_nWPlayLoopSound[m_nSizePlayLoopSound-1] = nCur;
			}
		}
	}
	fclose(hFile);
	return 1;
}

void CHistoryLoader::Destroy(void)
{
	SetZeroCount();
	glDeleteTextures(1, &m_uTexText);
	m_uTexText = 0;
	m_strText = _T("");
	if(m_nSizeAnu)
	{
		m_nSizeAnu = 0;
		delete[] m_anu;
	}
	if(m_nSizeStops)
	{
		m_nSizeStops = 0;
		m_nWStops = (int*)realloc(m_nWStops, m_nSizeStops*sizeof(int));
		m_nStops = (int*)realloc(m_nStops, m_nSizeStops*sizeof(int));
	}
	if(m_nSizeWaitCenter)
	{
		m_nSizeWaitCenter = 0;
		m_nWaitCenter = (int*)realloc(m_nWaitCenter, m_nSizeWaitCenter*sizeof(int));
	}
	if(m_nSizeChangeTPL)
	{
		m_nSizeChangeTPL = 0;
		m_nWChangeTPL = (int*)realloc(m_nWChangeTPL, m_nSizeChangeTPL*sizeof(int));
		m_nChangeTPL = (int*)realloc(m_nChangeTPL, m_nSizeChangeTPL*sizeof(int));
	}
	if(m_nSizeShowAnu)
	{
		m_nSizeShowAnu = 0;
		m_nWShowAnu = (int*)realloc(m_nWShowAnu, m_nSizeShowAnu*sizeof(int));
		m_nShowAnu = (int*)realloc(m_nShowAnu, m_nSizeShowAnu*sizeof(int));
	}
	if(m_nSizePlaySound)
	{
		m_nSizePlaySound = 0;
		m_nWPlaySound = (int*)realloc(m_nWPlaySound, m_nSizePlaySound*sizeof(int));
		m_nPlaySound = (int*)realloc(m_nPlaySound, m_nSizePlaySound*sizeof(int));
	}
	if(m_nSizePlayLoopSound)
	{
		m_nSizePlayLoopSound = 0;
		m_nWPlayLoopSound = (int*)realloc(m_nWPlayLoopSound, m_nSizePlayLoopSound*sizeof(int));
		m_nPlayLoopSound = (int*)realloc(m_nPlayLoopSound, m_nSizePlayLoopSound*sizeof(int));
	}
	if(m_nSizeSounds)
	{
		for(int x = 0; x < m_nSizeSounds; x++)
		{
			FMOD_Sound_Release(m_soundSounds[x]);
		}
		delete[] m_soundSounds;
		m_nSizeSounds = 0;
	}
	if(m_nSizeLoopSounds)
	{
		for(int x = 0; x < m_nSizeLoopSounds; x++)
		{
			FMOD_Sound_Release(m_soundLoopSounds[x]);
		}
		delete[] m_soundLoopSounds;
		m_nSizeLoopSounds = 0;
	}

	free(m_nWStops);
	free(m_nStops); 
	free(m_nWaitCenter); 
	free(m_nWChangeTPL); 
	free(m_nChangeTPL); 
	free(m_nWShowAnu); 
	free(m_nShowAnu); 
	free(m_nWPlaySound); 
	free(m_nPlaySound); 
	free(m_nWPlayLoopSound); 
	free(m_nPlayLoopSound); 
}

int CHistoryLoader::Draw(void)
{
#define TIME_TRANSI_TEXTEXT_HISLOAD 300
	for(int x = 0; x < m_nSizeShowAnu; x++)
	{
		if(m_nTPL >= m_nWShowAnu[x] && ((x+1) >= m_nSizeShowAnu || m_nTPL < m_nWShowAnu[x+1]))
		{
			if(m_nShowAnu[x] < m_nSizeAnu)
				m_anu[m_nShowAnu[x]]->Draw();
		}
	}
	for(int x = 0; x < m_nSizePlaySound; x++)
	{
		if(m_nTPL >= m_nWPlaySound[x] && ((x+1) >= m_nSizePlaySound || m_nTPL < m_nWPlaySound[x+1]) && m_nPlaying != m_nPlaySound[x])
		{
			m_nPlaying = m_nPlaySound[x];
			if(m_nPlaySound[x] < m_nSizeSounds)
				FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)10, m_soundSounds[m_nPlaySound[x]], 0, &channels[10]);
		}
	}
	for(int x = 0; x < m_nSizePlayLoopSound; x++)
	{
		if(m_nTPL >= m_nWPlayLoopSound[x] && ((x+1) >= m_nSizePlayLoopSound || m_nTPL < m_nWPlayLoopSound[x+1]) && m_nPlayingLoop != m_nPlayLoopSound[x])
		{
			m_nPlayingLoop = m_nPlayLoopSound[x];
			if(m_nPlayLoopSound[x] < m_nSizeLoopSounds)
				FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, m_soundLoopSounds[m_nPlayLoopSound[x]], 0, &channels[11]);
		}
	}
	glPushMatrix();
	if(m_nNext >= 0 && m_nNext < TIME_TRANSI_TEXTEXT_HISLOAD)
	{
		double dApt = double(m_nNext)/TIME_TRANSI_TEXTEXT_HISLOAD;
		glColor4d(1.0, 1.0, 1.0, dApt);
		glTranslated(0.0, -20.0, 0.0);
		glScaled(0.5+0.5*dApt, 0.5+0.5*dApt, 1.0);
		glBindTexture(GL_TEXTURE_2D, m_uTexText);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0, -10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0, -10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0,  10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0,  10.0, -50.0);
		glEnd();
		glColor4d(1.0, 1.0, 1.0, 1.0);
	}
	if(m_nNext >= TIME_TRANSI_TEXTEXT_HISLOAD)
	{
		glTranslated(0.0, -20.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, m_uTexText);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0, -10.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0, -10.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0,  10.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0,  10.0, -50.0);
		glEnd();
	}
	glPopMatrix();
	int nBegin = m_nPressCenter==0?0:m_nWaitCenter[m_nPressCenter-1];
	int nEnd = min(m_nTPL, m_strText.GetLength());
	CString strShow = m_strText.Mid(nBegin, nEnd-nBegin);
	font_Micro->glDrawText(-10.0, -40.0, -30.0, 40.0, -35.0, -15.0, -50.0, 1.0, 1.4, strShow, 0);
	if(m_nTPL >= m_strText.GetLength()) return 0;
	return 1;
}

void CHistoryLoader::PressCenter(void)
{
	if(m_nPressCenter < m_nSizeWaitCenter)
	{
		if(m_nTPL < m_nWaitCenter[m_nPressCenter])
		{
			m_nTPL = m_nWaitCenter[m_nPressCenter];
			m_nNext = TPLToTime(m_nTPL);
		}
		else
		{
			m_nPressCenter++;
		}
	}
}

void CHistoryLoader::Update(DWORD dwMilliseconds)
{
	static bool bAssign = false;
	if(m_nPressCenter >= m_nSizeWaitCenter || m_nTPL < m_nWaitCenter[m_nPressCenter])
	{
		m_nNext += dwMilliseconds;
		m_nTPL = TimeToTPL(m_nNext);
		if(bAssign) bAssign = false;
	}
	else
	{
		m_nTPL = m_nWaitCenter[m_nPressCenter];
		if(!bAssign)
		{
			m_nNext = TPLToTime(m_nTPL);
		}
	}
	for(int x = 0; x < m_nSizeShowAnu; x++)
	{
		if(m_nTPL >= m_nWShowAnu[x] && ((x+1) >= m_nSizeShowAnu || m_nTPL < m_nWShowAnu[x+1]))
		{
			if(m_nShowAnu[x] < m_nSizeAnu) m_anu[m_nShowAnu[x]]->Update(dwMilliseconds);
		}
		else
		{
			if(m_nShowAnu[x] < m_nSizeAnu) m_anu[m_nShowAnu[x]]->SetZeroCount();
		}
	}
}

void CHistoryLoader::SetZeroCount(void)
{
	m_nNext = 0;
	m_nTPL = 0;
	m_nPressCenter = 0;
	m_nPlayingLoop = -1;
	m_nPlaying = -1;
	FMOD_Channel_Stop(channels[10]);
	FMOD_Channel_Stop(channels[11]);
}

int CHistoryLoader::TimeToTPL(__int64 m_nMillisec)
{
	double dTPL = 0.0;
	__int64 nMiliseconds = m_nMillisec;
	int x;
	for(x = 0; x < m_nSizeChangeTPL; x++)
	{
		double dSTPL = dTPL+double(nMiliseconds)/double(m_nChangeTPL[x]);
		for(int y = 0; y < m_nSizeStops; y++)	// Stops
		{
			double dStTPL = double(m_nWStops[y]);
			if(dStTPL <= dSTPL && (dStTPL+double(m_nStops[y])) >= dSTPL)
			{
				dSTPL -= dSTPL - dStTPL;
			}
			else if((dStTPL+double(m_nStops[y])) < dSTPL && dStTPL >= dTPL)
			{
				dSTPL -= double(m_nStops[y]);
			}
		}
		if((x+1) >= m_nSizeChangeTPL)
		{
			return int(dSTPL);
		}
		else
		{
			double dTPLMil = double(m_nWChangeTPL[x+1]-m_nWChangeTPL[x])+dTPL;
			if(dTPLMil > dSTPL)
			{
				return int(dSTPL);
			}
			else
			{
				dTPL += double(m_nWChangeTPL[x+1]-m_nWChangeTPL[x]);
				nMiliseconds -= (__int64)((m_nWChangeTPL[x+1]-m_nWChangeTPL[x])*m_nChangeTPL[x]);
				for(int y = 0; y < m_nSizeStops; y++)
				{
					if(m_nWStops[y] >= m_nWChangeTPL[x] && // WARNING
						m_nWStops[y] < m_nWChangeTPL[x+1])
					{
						nMiliseconds -= (__int64)(m_nStops[y]*m_nChangeTPL[x]);
					}
				}
			}
		}
	}
	return int(dTPL);
}
__int64 CHistoryLoader::TPLToTime(int nTPL)
{
	__int64 nMil = 0;
	int nSTPL = nTPL;
	int x;
	for(x = 0; x < m_nSizeChangeTPL; x++)
	{
		__int64 dSMil = nMil+(__int64)(nSTPL*m_nChangeTPL[x]);
		__int64 nOffset = 0;
		int dMinB = m_nWChangeTPL[x];
		int dMaxB = (x+1) >= m_nSizeChangeTPL?nTPL:min(m_nWChangeTPL[x+1],nTPL);
		for(int y = 0; y < m_nSizeStops; y++)
		{
			if(m_nWStops[y] >= dMinB && m_nWStops[y] < dMaxB)
			{
				nOffset+=(__int64)(m_nStops[y]*m_nChangeTPL[x]);
			}
		}
		if((x+1) >= m_nSizeChangeTPL)
		{
			return dSMil+nOffset; // No existen mas cambios de BPM
		}
		else
		{
			if(m_nWChangeTPL[x+1] > nTPL)
			{
				// El beat actual no ha pasado por este cambio de BPM
				return dSMil+nOffset;
			}
			else
			{
				nSTPL -= m_nWChangeTPL[x+1]-m_nWChangeTPL[x];
				nMil += (__int64)((m_nWChangeTPL[x+1]-m_nWChangeTPL[x])*m_nChangeTPL[x]);
				nMil += nOffset;
			}
		}
	}
	return nMil;
}
