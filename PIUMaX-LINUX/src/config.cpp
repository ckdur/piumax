#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "config.h"

int nCurSelConfig;
int nCurTotalConfig;
int nCurPageConfig;
bool bModPressConfig[4];
bool bPressEx = false;
int nPressEx[3];
bool bSet = false;
int nModSet = 0;

// ** Creditos PIU MaX
///////////////////////////////////////////////////
#define PIU_CREDITS _T("PIU MaX Credits:\n\nProgrammed and directed by:\n\nCKDUR\n\n2009-2010 (C)")
///////////////////////////////////////////////////

///////////////////////////////////////////////////

int CreateContextConfig()
{
#ifdef PIUMAX_FOR_HOS
	FILE* hFile = _fopen(_T("/SAVE/PIUMAX/config.bin"), _T("rb")/*b*/);
#else
    FILE* hFile = _fopen(_T("config.bin"), _T("rb")/*b*/);
#endif
	if(hFile == NULL)
	{
		ConfigGetDefaults(0);
		ConfigGetDefaults(1);
		ConfigGetDefaults(2);
#ifdef PIUMAX_FOR_HOS
        hFile = _fopen(_T("/SAVE/PIUMAX/config.bin"), _T("wb")/*b*/);
#else
        hFile = _fopen(_T("config.bin"), _T("wb")/*b*/);
#endif
		if(hFile == NULL) return 0;
		fwrite(&piuconfig, sizeof(PIUCONFIG), 1, hFile);
		fclose(hFile);
	}
	else
	{
		fread(&piuconfig, sizeof(PIUCONFIG), 1, hFile);
		fclose(hFile);
	}
#ifdef PIUMAX_FOR_HOS
	hFile = _fopen(_T("/SAVE/PIUMAX/credit.bin"), _T("rb")/*b*/);
#else
    hFile = _fopen(_T("credit.bin"), _T("rb")/*b*/);
#endif
	if(hFile == NULL)
	{
		nTotalCredit = 0;
#ifdef PIUMAX_FOR_HOS
        hFile = _fopen(_T("/SAVE/PIUMAX/credit.bin"), _T("wb")/*b*/);
#else
        hFile = _fopen(_T("credit.bin"), _T("wb")/*b*/);
#endif
		if(hFile == NULL) return 0;
		fwrite(&nTotalCredit, sizeof(int), 1, hFile);
		fclose(hFile);
	}
	else
	{
		fread(&nTotalCredit, sizeof(int), 1, hFile);
		fclose(hFile);
	}
	SetVolume(piuconfig.nSoundPrev?0:1);
	return 1;
}

void DestroyContextConfig()
{
}

#define PAGE0ITEMS_CONFIG	5
#define PAGE1ITEMS_CONFIG	2
#define PAGE2ITEMS_CONFIG	7
#define PAGE3ITEMS_CONFIG	7
#define PAGE4ITEMS_CONFIG	1
#define PAGE5ITEMS_CONFIG	1
#define PAGE6ITEMS_CONFIG	1
#define PAGE7ITEMS_CONFIG	17
#define SPACING_CONFIG 2.5
#define SIZETEXT_CONFIG 2.0
#define LSPACING_CONFIG -15.0
#define RSPACING_CONFIG 10.0

void ActualizateContextConfig()
{
	nCurSelConfig = 0;
	nCurTotalConfig = PAGE0ITEMS_CONFIG;
	nCurPageConfig = 0;
	for(int x = 0; x < 4; x++) bModPressConfig[x] = false;
	bPressEx = false;
	nPressEx[0] = -2;
	nPressEx[1] = 0;
}

int FrameConfig()
{
	if(bPressEx && bSet)
	{
		if(nCurPageConfig == 7)
		{
			if(nCurSelConfig >= 1 && nCurSelConfig <= 10)
			{
				piuconfig.nWhereButtons[nCurSelConfig-1][nModSet] = nPressEx[0];
				piuconfig.nButtons[nCurSelConfig-1][nModSet] = nPressEx[1];
				piuconfig.nModeButtons[nCurSelConfig-1][nModSet] = nPressEx[2];
			}
			if(nCurSelConfig == 11)
			{
				piuconfig.nCoinButton1[0] = nPressEx[0];
				piuconfig.nCoinButton1[1] = nPressEx[1];
				piuconfig.nCoinButton1[2] = nPressEx[2];
			}
			if(nCurSelConfig == 12)
			{
				piuconfig.nCoinButton2[0] = nPressEx[0];
				piuconfig.nCoinButton2[1] = nPressEx[1];
				piuconfig.nCoinButton2[2] = nPressEx[2];
			}
			if(nCurSelConfig == 13)
			{
				piuconfig.nConfigButton[0] = nPressEx[0];
				piuconfig.nConfigButton[1] = nPressEx[1];
				piuconfig.nConfigButton[2] = nPressEx[2];
			}
			if(nCurSelConfig == 14)
			{
				piuconfig.nServiceButton[0] = nPressEx[0];
				piuconfig.nServiceButton[1] = nPressEx[1];
				piuconfig.nServiceButton[2] = nPressEx[2];
			}
		}
		bSet = false;
	}
	if(bModPressConfig[2] == true && !bSet)
	{
		bModPressConfig[2] = false;
		nCurSelConfig++;
		if(nCurSelConfig >= nCurTotalConfig) nCurSelConfig = 0;
	}
	if(bModPressConfig[3] == true && !bSet)
	{
		bModPressConfig[3] = false;
		if(nCurPageConfig == 0)
		{
			nCurPageConfig = nCurSelConfig+1;
			switch(nCurSelConfig)
			{
			case 0: nCurTotalConfig = PAGE1ITEMS_CONFIG; break;
			case 1: nCurTotalConfig = PAGE2ITEMS_CONFIG; break;
			case 2: nCurTotalConfig = PAGE3ITEMS_CONFIG; break;
			case 3: nCurTotalConfig = PAGE4ITEMS_CONFIG; break;
			case 5: nCurTotalConfig = PAGE5ITEMS_CONFIG; break;
			case 6: nCurTotalConfig = PAGE6ITEMS_CONFIG; break;
			case 4:
				{
#ifdef PIUMAX_FOR_HOS
                    FILE* hFile = _fopen(_T("/SAVE/PIUMAX/config.bin"), _T("w+b"));
#else
                    FILE* hFile = _fopen(_T("config.bin"), _T("w+b"));
#endif
					if(hFile == NULL) return INT_WELCOME;
					fwrite(&piuconfig, sizeof(PIUCONFIG), 1, hFile);
					fclose(hFile);
					SetVolume(piuconfig.nSoundPrev?1:0);
					return INT_WELCOME;
				}break;
			}
			nCurSelConfig = 0;
		}
		else if(nCurPageConfig == 1)
		{
			nCurPageConfig = nCurSelConfig==0?7:0;
			switch(nCurSelConfig)
			{
			case 0: nCurTotalConfig = PAGE7ITEMS_CONFIG; break;
			case 1: nCurTotalConfig = PAGE0ITEMS_CONFIG; break;
			}
			nCurSelConfig = 0;
		}
		else if(nCurPageConfig == 2)
		{
			switch(nCurSelConfig)
			{
			case 0:
				piuconfig.nLevelMach++;
				if(piuconfig.nLevelMach >= 3) piuconfig.nLevelMach = 0;
				break;
			case 1:
				piuconfig.nBarLevel++;
				if(piuconfig.nBarLevel >= 7) piuconfig.nBarLevel = 0;
				break;
			case 2:
				piuconfig.nStageBreak++;
				if(piuconfig.nStageBreak >= 5) piuconfig.nStageBreak = 0;
				break;
			case 3:
				piuconfig.nLevelJud++;
				if(piuconfig.nLevelJud >= 7) piuconfig.nLevelJud = 0;
				break;
			case 4:
				piuconfig.nTimePlay++;
				if(piuconfig.nTimePlay >= 3) piuconfig.nTimePlay = 0;
				break;
			case 5:
				ConfigGetDefaults(1);
				break;
			case 6:
				nCurTotalConfig = PAGE0ITEMS_CONFIG;
				nCurPageConfig = 0;
				nCurSelConfig = 0;
				break;
			}
		}
		else if(nCurPageConfig == 3)
		{
			switch(nCurSelConfig)
			{
			case 0:
				piuconfig.nCoinsCred1++;
				if(piuconfig.nCoinsCred1 >= 10) piuconfig.nCoinsCred1 = 0;
				break;
			case 1:
				piuconfig.nCoinsCred2++;
				if(piuconfig.nCoinsCred2 >= 10) piuconfig.nCoinsCred2 = 0;
				break;
			case 2:
				piuconfig.nModeMach++;
				if(piuconfig.nModeMach >= 2) piuconfig.nModeMach = 0;
				break;
			case 3:
				if(!piuconfig.nModeMach)
				{
					piuconfig.nStageLimit++;
					if(piuconfig.nStageLimit >= 4) piuconfig.nStageLimit = 0;
				}
				break;
			case 4:
				piuconfig.nSoundPrev++;
				if(piuconfig.nSoundPrev >= 2) piuconfig.nSoundPrev = 0;
				break;
			case 5:
				ConfigGetDefaults(2);
				break;
			case 6:
				nCurTotalConfig = PAGE0ITEMS_CONFIG;
				nCurPageConfig = 0;
				nCurSelConfig = 0;
				break;
			}
		}
		else if(nCurPageConfig == 7)
		{
			switch(nCurSelConfig)
			{
			case 0:
				nModSet++;
				if(nModSet >= 4) nModSet = 0;
				break;
			case 1: case 2: case 3: case 4: case 5: case 6: case 7:
			case 8: case 9: case 10: case 11: case 12: case 13: case 14:
				if(!bSet) bSet = true;
				break;
			case 15:
				ConfigGetDefaults(3);
				break;
			case 16:
				nCurTotalConfig = PAGE1ITEMS_CONFIG;
				nCurPageConfig = 1;
				nCurSelConfig = 0;
				break;
			}
		}
		else// if(nCurPageConfig == 3)
		{
			nCurPageConfig = 0;
			nCurTotalConfig = PAGE0ITEMS_CONFIG;
			nCurSelConfig = 0;
		}
	}
	if(nCurPageConfig == 0)
	{
		for(int x = 0; x < PAGE0ITEMS_CONFIG; x++)
		{
			if(x == nCurSelConfig) glColor4d(1.0, 0.0, 0.0, 1.0);
			else glColor4d(1.0, 1.0, 1.0, 1.0);
			CString str;
			switch(x)
			{
			case 0: str = _T("Input Test"); break;
			case 1: str = _T("Game Setting"); break;
			case 2: str = _T("Coin Setting"); break;
			case 3: str = _T("About PIU MaX"); break;
			case 4: str = _T("Exit"); break;
			default: str = _T("(Null)"); break;
			}
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, LSPACING_CONFIG, -double(x)*SPACING_CONFIG+PAGE0ITEMS_CONFIG*SPACING_CONFIG/2, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER);
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Config Button: Next Option")), DT_VCENTER|DT_CENTER);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0-SPACING_CONFIG, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Service Button: Enter")), DT_VCENTER|DT_CENTER);
	}
	if(nCurPageConfig == 1)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		for(int x = 0; x < 10; x++)
		{
			if(bArrows[x]) glColor4d(1.0, 0.0, 0.0, 1.0);
			else glColor4d(1.0, 1.0, 1.0, 1.0);
			glPushMatrix();
			switch(x)
			{
			case 0:
				glTranslated(-33.3333, -3.3333, 0.0);
				break;
			case 1:
				glTranslated(-33.3333, 23.3333, 0.0);
				break;
			case 2:
				glTranslated(-20.0, 10.0, 0.0);
				break;
			case 3:
				glTranslated(-6.6666, 23.3333, 0.0);
				break;
			case 4:
				glTranslated(-6.6666, -3.3333, 0.0);
				break;
			case 5:
				glTranslated(6.6666, -3.3333, 0.0);
				break;
			case 6:
				glTranslated(6.6666, 23.3333, 0.0);
				break;
			case 7:
				glTranslated(20.0, 10.0, 0.0);
				break;
			case 8:
				glTranslated(33.3333, 23.3333, 0.0);
				break;
			case 9:
				glTranslated(33.3333, -3.3333, 0.0);
				break;
			}
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-5.5, -5.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 5.5, -5.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 5.5,  5.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-5.5,  5.5, -50.0);
			glEnd();
			glPopMatrix();
		}
		glColor4d(1.0, 1.0, 1.0, 1.0);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, -20.0, -11.3333, -50.0, 1.0, SIZETEXT_CONFIG, bModPressConfig[0]?CString(_T("Coin 1: ON")):CString(_T("Coin 1: OFF")), DT_VCENTER|DT_CENTER);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 20.0, -11.3333, -50.0, 1.0, SIZETEXT_CONFIG, bModPressConfig[1]?CString(_T("Coin 2: ON")):CString(_T("Coin 2: OFF")), DT_VCENTER|DT_CENTER);
		for(int x = 0; x < PAGE1ITEMS_CONFIG; x++)
		{
			if(x == nCurSelConfig) glColor4d(1.0, 0.0, 0.0, 1.0);
			else glColor4d(1.0, 1.0, 1.0, 1.0);
			CString str;
			switch(x)
			{
			case 0: str = _T("Input Setting"); break;
			case 1: str = _T("Back"); break;
			default: str = _T("(Null)"); break;
			}
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, LSPACING_CONFIG, -double(x)*SPACING_CONFIG-17.0, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER);
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -17.0-SPACING_CONFIG*2, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Config Button: Next Option")), DT_VCENTER|DT_CENTER);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -17.0-SPACING_CONFIG*3, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Service Button: Enter")), DT_VCENTER|DT_CENTER);
	}
	if(nCurPageConfig == 2)
	{
		for(int x = 0; x < PAGE2ITEMS_CONFIG; x++)
		{
			if(x == nCurSelConfig) glColor4d(1.0, 0.0, 0.0, 1.0);
			else glColor4d(1.0, 1.0, 1.0, 1.0);
			CString str;
			switch(x)
			{
			case 0: str = _T("Difficulty Level"); break;
			case 1: str = _T("Arcade Bar Level"); break;
			case 2: str = _T("Stage Break"); break;
			case 3: str = _T("Judgament Level"); break;
			case 4: str = _T("Time Playing"); break;
			case 5: str = _T("Reset Defaultis"); break;
			case 6: str = _T("Exit"); break;
			default: str = _T("(Null)"); break;
			}
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, LSPACING_CONFIG, -double(x)*SPACING_CONFIG+PAGE2ITEMS_CONFIG*SPACING_CONFIG/2, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER);
			switch(x)
			{
			case 0:
				switch(piuconfig.nLevelMach)
				{
				case 0:
					str = _T("Easy (SQRT timing)");
					break;
				case 1:
					str = _T("Normal (Lineal timing) DEF");
					break;
				case 2:
					str = _T("Hard (Cuadratic timimg)");
					break;
				default:
					str = _T("Unknown");
					break;
				}
				break;
			case 1:
				switch(piuconfig.nBarLevel)
				{
				case 0:
					str = _T("1");
					break;
				case 1:
					str = _T("2");
					break;
				case 2:
					str = _T("3 DEF");
					break;
				case 3:
					str = _T("4");
					break;
				case 4:
					str = _T("5");
					break;
				case 5:
					str = _T("6");
					break;
				case 6:
					str = _T("7");
					break;
				default:
					str = _T("Unknown");
					break;
				}
				break;
			case 2:
				switch(piuconfig.nStageBreak)
				{
				case 1:
					str = _T("1st Stage");
					break;
				case 2:
					str = _T("2nd Stage DEF");
					break;
				case 3:
					str = _T("3rd Stage");
					break;
				case 4:
					str = _T("4th Stage");
					break;
				case 0:
					str = _T("Never");
					break;
				default:
					str = _T("Unknown");
					break;
				}
				break;
			case 3:
				switch(piuconfig.nLevelJud)
				{
				case 0:
					str = _T("1 (85%A 75%B 65%C 55%D)");
					break;
				case 1:
					str = _T("2 (90%A 80%B 70%C 60%D)");
					break;
				case 2:
					str = _T("3 (90%A 82%B 74%C 66%D) DEF");
					break;
				case 3:
					str = _T("4 (92%A 84%B 76%C 68%D)");
					break;
				case 4:
					str = _T("5 (94%A 86%B 78%C 70%D)");
					break;
				case 5:
					str = _T("6 (94%A 87%B 80%C 73%D)");
					break;
				case 6:
					str = _T("7 (95%A 88%B 83%C 75%D)");
					break;
				default:
					str = _T("Unknown");
					break;
				}
				break;
			case 4:
				switch(piuconfig.nTimePlay)
				{
				case 0:
					str = _T("120 sec");
					break;
				case 1:
					str = _T("90 sec DEF");
					break;
				case 2:
					str = _T("60 sec");
					break;
				default:
					str = _T("Unknown");
					break;
				}
				break;
			case 5: str = _T(""); break;
			case 6: str = _T(""); break;
			default: str = _T(""); break;
			}
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, RSPACING_CONFIG, -double(x)*SPACING_CONFIG+PAGE2ITEMS_CONFIG*SPACING_CONFIG/2, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER);
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Config Button: Next Option")), DT_VCENTER|DT_CENTER);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0-SPACING_CONFIG, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Service Button: Enter")), DT_VCENTER|DT_CENTER);
	}
	if(nCurPageConfig == 3)
	{
		CString strc;
		strc.Format(_T("Coin counter: %d"), nTotalCredit);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, -35.0, 25.0, -50.0, 1.0, SIZETEXT_CONFIG, strc, 0);
		for(int x = 0; x < PAGE3ITEMS_CONFIG; x++)
		{
			if(x == nCurSelConfig) glColor4d(1.0, 0.0, 0.0, 1.0);
			else glColor4d(1.0, 1.0, 1.0, 1.0);
			CString str;
			switch(x)
			{
			case 0: str = _T("Coin Option 1"); break;
			case 1: str = _T("Coin Option 2"); break;
			case 2: str = _T("Machine Mode"); break;
			case 3: str = _T("Stage Limit"); break;
			case 4: str = _T("Demo sound"); break;
			case 5: str = _T("Reset Defaultis"); break;
			case 6: str = _T("Exit"); break;
			default: str = _T("(Null)"); break;
			}
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, LSPACING_CONFIG, -double(x)*SPACING_CONFIG+PAGE3ITEMS_CONFIG*SPACING_CONFIG/2, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER);
			switch(x)
			{
			case 0:
				if(piuconfig.nCoinsCred1!=0) str.Format(_T("1 Credit / %d Coins"), piuconfig.nCoinsCred1);
				else str = _T("FreePlay");
				break;
			case 1:
				if(piuconfig.nCoinsCred2!=0) str.Format(_T("1 Credit / %d Coins"), piuconfig.nCoinsCred2);
				else str = _T("FreePlay");
				break;
			case 2:
				switch(piuconfig.nModeMach)
				{
				case 0:
					str = _T("NORMAL DEF");
					break;
				case 1:
					str = _T("EVENT");
					break;
				default:
					str = _T("Unknown");
					break;
				}
				break;
			case 3:
				if(piuconfig.nModeMach)
				{
					str = _T("No configurable");
				}
				else
				{
					switch(piuconfig.nStageLimit)
					{
					case 0:
						str = _T("8 PIU Life (4 Stage) DEF");
						break;
					case 1:
						str = _T("6 PIU Life (3 Stage)");
						break;
					case 2:
						str = _T("4 PIU Life (2 Stage)");
						break;
					case 3:
						str = _T("2 PIU Life (1 Stage)");
						break;
					default:
						str = _T("Unknown");
						break;
					}
				}
				break;
			case 4:
				if(piuconfig.nSoundPrev)
				{
					str = _T("Yes");
				}
				else
				{
					str = _T("No");
				}
				break;
			default: str = _T(""); break;
			}
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, RSPACING_CONFIG, -double(x)*SPACING_CONFIG+PAGE3ITEMS_CONFIG*SPACING_CONFIG/2, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER);
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Config Button: Next Option")), DT_VCENTER|DT_CENTER);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0-SPACING_CONFIG, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Service Button: Enter")), DT_VCENTER|DT_CENTER);
	}
	if(nCurPageConfig == 4)	// About
	{
		CString str;
		for(int x = 0; x < PAGE4ITEMS_CONFIG; x++)
		{
			if(x == nCurSelConfig) glColor4d(1.0, 0.0, 0.0, 1.0);
			else glColor4d(1.0, 1.0, 1.0, 1.0);
			switch(x)
			{
			case 0: str = _T("Exit"); break;
			default: str = _T("(Null)"); break;
			}
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, LSPACING_CONFIG, -double(x-1)*SPACING_CONFIG-22.0, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER);
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		str = PIU_CREDITS;
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, 0.0, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_CENTER|DT_VCENTER);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Config Button: Next Option")), DT_VCENTER|DT_CENTER);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0-SPACING_CONFIG, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Service Button: Enter")), DT_VCENTER|DT_CENTER);
	}
	if(nCurPageConfig == 5)
	{
		for(int x = 0; x < PAGE5ITEMS_CONFIG; x++)
		{
			if(x == nCurSelConfig) glColor4d(1.0, 0.0, 0.0, 1.0);
			else glColor4d(1.0, 1.0, 1.0, 1.0);
			CString str;
			switch(x)
			{
			case 0: str = _T("Exit"); break;
			default: str = _T("(Null)"); break;
			}
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, LSPACING_CONFIG, -double(x)*SPACING_CONFIG+PAGE5ITEMS_CONFIG*SPACING_CONFIG/2, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER);
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Config Button: Next Option")), DT_VCENTER|DT_CENTER);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0-SPACING_CONFIG, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Service Button: Enter")), DT_VCENTER|DT_CENTER);
	}
	if(nCurPageConfig == 6)
	{
		for(int x = 0; x < PAGE6ITEMS_CONFIG; x++)
		{
			if(x == nCurSelConfig) glColor4d(1.0, 0.0, 0.0, 1.0);
			else glColor4d(1.0, 1.0, 1.0, 1.0);
			CString str;
			switch(x)
			{
			case 0: str = _T("Exit"); break;
			default: str = _T("(Null)"); break;
			}
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, LSPACING_CONFIG, -double(x)*SPACING_CONFIG+PAGE6ITEMS_CONFIG*SPACING_CONFIG/2, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER);
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Config Button: Next Option")), DT_VCENTER|DT_CENTER);
		font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -22.0-SPACING_CONFIG, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Service Button: Enter")), DT_VCENTER|DT_CENTER);
	}
	if(nCurPageConfig == 7)
	{
		for(int x = 0; x < PAGE7ITEMS_CONFIG; x++)
		{
			if(x == nCurSelConfig) glColor4d(1.0, 0.0, 0.0, 1.0);
			else glColor4d(1.0, 1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, 0);
			if(x >= 1 && x <= 10)
			{
				glPushMatrix();
				switch(x-1)
				{
				case 0:
					glTranslated(-33.3333, -3.3333, 0.0);
					break;
				case 1:
					glTranslated(-33.3333, 23.3333, 0.0);
					break;
				case 2:
					glTranslated(-20.0, 10.0, 0.0);
					break;
				case 3:
					glTranslated(-6.6666, 23.3333, 0.0);
					break;
				case 4:
					glTranslated(-6.6666, -3.3333, 0.0);
					break;
				case 5:
					glTranslated(6.6666, -3.3333, 0.0);
					break;
				case 6:
					glTranslated(6.6666, 23.3333, 0.0);
					break;
				case 7:
					glTranslated(20.0, 10.0, 0.0);
					break;
				case 8:
					glTranslated(33.3333, 23.3333, 0.0);
					break;
				case 9:
					glTranslated(33.3333, -3.3333, 0.0);
					break;
				}
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-5.5, -5.5, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 5.5, -5.5, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 5.5,  5.5, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-5.5,  5.5, -50.0);
				glEnd();
				CString str;
				if(piuconfig.nWhereButtons[x-1][nModSet] == -2)
				{
					str = _T("NO KEY");
				}
				else if(piuconfig.nWhereButtons[x-1][nModSet] == -1)
				{
					str.Format(_T("KeyBoard ID: %d"), piuconfig.nButtons[x-1][nModSet]);
				}
				else
				{
					str.Format(_T("Joy%d ID: %d"), piuconfig.nWhereButtons[x-1][nModSet], piuconfig.nButtons[x-1][nModSet]);
				}
				glColor4d(0.0, 1.0, 0.0, 1.0);
				font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, 0.0, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER|DT_CENTER);
				glPopMatrix();
			}
			if(x == 11)
			{
				CString str;
				if(piuconfig.nCoinButton1[0] == -2) str = _T("Coin 1: NO KEY");
				else if(piuconfig.nCoinButton1[0] == -1) str.Format(_T("Coin 1: KeyBoard ID: %d"), piuconfig.nCoinButton1[1]);
				else str.Format(_T("Coin 1: Joy%d ID: %d"), piuconfig.nCoinButton1[0], piuconfig.nCoinButton1[1]);
				font_System->glDrawText(30.0, -40.0, -30.0, 40.0, -20.0, -11.3333, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER|DT_CENTER);
			}
			if(x == 12)
			{
				CString str;
				if(piuconfig.nCoinButton2[0] == -2) str = _T("Coin 2: NO KEY");
				else if(piuconfig.nCoinButton2[0] == -1) str.Format(_T("Coin 2: KeyBoard ID: %d"), piuconfig.nCoinButton2[1]);
				else str.Format(_T("Coin 2: Joy%d ID: %d"), piuconfig.nCoinButton2[0], piuconfig.nCoinButton2[1]);
				font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 20.0, -11.3333, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER|DT_CENTER);
			}
			if(x == 13)
			{
				CString str;
				if(piuconfig.nConfigButton[0] == -2) str = _T("Config: NO KEY");
				else if(piuconfig.nConfigButton[0] == -1) str.Format(_T("Config: KeyBoard ID: %d"), piuconfig.nConfigButton[1]);
				else str.Format(_T("Config: Joy%d ID: %d"), piuconfig.nConfigButton[0], piuconfig.nConfigButton[1]);
				font_System->glDrawText(30.0, -40.0, -30.0, 40.0, -20.0, -11.3333-SPACING_CONFIG, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER|DT_CENTER);
			}
			if(x == 14)
			{
				CString str;
				if(piuconfig.nServiceButton[0] == -2) str = _T("Service: NO KEY");
				else if(piuconfig.nServiceButton[0] == -1) str.Format(_T("Service: KeyBoard ID: %d"), piuconfig.nServiceButton[1]);
				else str.Format(_T("Service: Joy%d ID: %d"), piuconfig.nServiceButton[0], piuconfig.nServiceButton[1]);
				font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 20.0, -11.3333-SPACING_CONFIG, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER|DT_CENTER);
			}
			if(x == 0 || x == 15 || x == 16 || x > 16)
			{
				CString str;
				switch(x)
				{
				case 0: str.Format(_T("Current Set: %d"), nModSet);; break;
				case 15: str = _T("Reset Defaults"); break;
				case 16: str = _T("Back"); break;
				default: str = _T("(Null)"); break;
				}
				font_System->glDrawText(30.0, -40.0, -30.0, 40.0, LSPACING_CONFIG, -double(x==0?0:(x-14))*SPACING_CONFIG-17.0, -50.0, 1.0, SIZETEXT_CONFIG, str, DT_VCENTER);
			}
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		if(!bSet)
		{
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -17.0-SPACING_CONFIG*2, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Config Button: Next Option")), DT_VCENTER|DT_CENTER);
			font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -17.0-SPACING_CONFIG*3, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Service Button: Enter")), DT_VCENTER|DT_CENTER);
		}
		else font_System->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, -17.0-SPACING_CONFIG*2, -50.0, 1.0, SIZETEXT_CONFIG, CString(_T("Press Key for Set")), DT_VCENTER|DT_CENTER);
	}
	return INT_CONFIG;
}

int EventConfig(int nEvent, int nKey)
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
		if(nKey == EV_CREDIT1) bModPressConfig[0] = true;
		if(nKey == EV_CREDIT2) bModPressConfig[1] = true;
		if(nKey == EV_CONFIG) bModPressConfig[2] = true;
		if(nKey == EV_SERVICE) bModPressConfig[3] = true;
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
		if(nKey == EV_CREDIT1) bModPressConfig[0] = false;
		if(nKey == EV_CREDIT2) bModPressConfig[1] = false;
		if(nKey == EV_CONFIG) bModPressConfig[2] = false;
		if(nKey == EV_SERVICE) bModPressConfig[3] = false;
	}
	return 1;
}

int EventConfigEx(SDL_Event ev)
{
	if(bSet)
	{
		if ( ev.type == SDL_KEYDOWN )
		{
			bPressEx = true;
			nPressEx[0] = -1;
			nPressEx[1] = ev.key.keysym.sym;
			nPressEx[2] = 0;
		}
		if ( ev.type == SDL_JOYBUTTONDOWN )
		{
			bPressEx = true;
			nPressEx[0] = ev.jbutton.which;
			nPressEx[1] = ev.jbutton.button;
			nPressEx[2] = 0;
		}
		if ( ev.type == SDL_JOYHATMOTION )
		{
			bPressEx = true;
			nPressEx[0] = ev.jhat.which;
			nPressEx[1] = ev.jhat.hat;
			if(ev.jhat.value & SDL_HAT_UP) nPressEx[2] = 1;
			if(ev.jhat.value & SDL_HAT_LEFT) nPressEx[2] = 2;
			if(ev.jhat.value & SDL_HAT_DOWN) nPressEx[2] = 3;
			if(ev.jhat.value & SDL_HAT_RIGHT) nPressEx[2] = 4;
		}
	}
	if ( ev.type == SDL_JOYBUTTONUP || ev.type == SDL_KEYUP || (ev.type == SDL_JOYHATMOTION && ev.jhat.value == 0))
	{
		bPressEx = false;
	}
	return 1;
}
