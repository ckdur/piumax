#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "welcome.h"
//#include "stepsmu.h"

#define PIUMAX_WELCOME_RUNTIMESONG 30000

CMaxLights *lightsWelcome;
CMaxLights *lightsWelcomeLoop;

int DrawWelcome();
int DrawWelcomeLoop();
int DrawWelcomeBest();
int DrawDemo();

int nInter = 0, nCur = 0, g_nCount = 0;
__int64 nSec = 0, nDur = 0;
bool bBusy = false;
bool bChooseGame = false;
GLuint texture[1];
DWORD dwSizeTex = 0;
CMovieStream2* movie_welcome;
CMovieStream2* movie_loop;
CMovieStream2* movie_best;
CMovieStream2* movie_bg;
CWelcomeRes* welres;
__int64 nInitChooseGame = 0;
__int64 nInitChange = 0;
__int64 nCountLoop = 0;
__int64 nCountBest = 0;
__int64 nTimeChoose[2];
int nModeChange = 0;		// 0 ninguno, 1 izq, 2 der
int nChoose = 0;			// Interfase
int nChooses = 0;			// Veces Escogida

FMOD_SOUND		*music_welcome = 0;
FMOD_SOUND		*music_loop = 0;
FMOD_SOUND		*sound_wpiumax = 0;
FMOD_SOUND		*sound_chooseGameIntro = 0;
FMOD_SOUND		*sound_chooseGameChange = 0;
FMOD_RESULT res;

//HMODULE hInter = NULL;

struct WELCOMECONFIG
{
	int m_nLLoop;
	int m_nFPress;
	int m_nLPress;
	int m_nFICoin;
	int m_nLICoin;
	int m_nLDemo;
}
#ifdef __GNUC__
__attribute__((ms_struct))
#endif
;

WELCOMECONFIG w_config;

// ** Funciones Auxiliares

int GetConfig()
{
	TCHAR ch[100];
	FILE* hFile;
	if((hFile = _fopen(_T("welcome.cfg"), _T("r"))) == NULL)
	{
        _printf(_T("El archivo 'welcome.cfg' no existe o esta corrupto\n"));
		return 0;
	}
	_fgets(ch, 100, hFile);
	fclose(hFile);
	if(_sscanf(ch, _T("%d %d %d %d %d %d")
		, &w_config.m_nLLoop
		, &w_config.m_nFPress
		, &w_config.m_nLPress
		, &w_config.m_nFICoin
		, &w_config.m_nLICoin
		, &w_config.m_nLDemo)!= 6)
	{
        _printf(_T("El archivo 'welcome.cfg' se encuentra mal escrito, deben ser 6 numeros enteros\n"));
		return 0;
	}
	return 1;
}

void DeleteTextures()
{
	if(welres) delete welres;
	welres = NULL;
}

/////////////////////////////

// ** Funciones no exportadas
void LoadGLTextures(void)
{
	if (welres == NULL)
	{
		welres = new CWelcomeRes();
		welres->LoadSkin();
	}
};

int DrawWelcome()
{
	if(movie_welcome->bLoad)
	{
		unsigned int nPos = 0;
		FMOD_Channel_GetPosition(channels[8], &nPos, FMOD_TIMEUNIT_MS);
		//movie_welcome->next = nPos;
		movie_welcome->Update(dwWait);
		movie_welcome->GrabAVIFrame();
		glBindTexture(GL_TEXTURE_2D, movie_welcome->tex);

		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0, -30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0,  30.0, -50.0);
		glEnd();
	}

	lightsWelcome->Update(dwWait);

	FMOD_BOOL nIsPlaying = 0;
	FMOD_Channel_IsPlaying(channels[8], &nIsPlaying);
	if(!nIsPlaying || channels[8] == NULL)
	{
		return 0;
	}
	return 1;
}
int DrawWelcomeLoop()
{
	nCountLoop += dwTick - dwLastTick;
	double dAdv = double(nCountLoop%500)/500;
	double dScal = abs((double)dAdv-(double)(int(dAdv))-0.5)*0.25+1.0;
	if(movie_loop->bLoad)
	{
		movie_loop->Update(dwWait);
		movie_loop->GrabAVIFrame();

		DWORD dwTime = SDL_GetTicks();
		glBindTexture(GL_TEXTURE_2D, movie_loop->tex);

		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
		glEnd();
	}

	lightsWelcomeLoop->Update(dwWait);

	if(!bChooseGame)
	{
		glPushMatrix();
		glTranslated(-20.0, -20.0, 0.0);
		glScaled(dScal,dScal,1.0);
		glBindTexture(GL_TEXTURE_2D, welres->skin[1]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-12.5, -2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 12.5, -2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 12.5,  2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-12.5,  2.5, -50.0);
		glEnd();
		if(nInter == 1)
		{
			glBindTexture(GL_TEXTURE_2D, welres->skin[2]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-10.0, 2.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 10.0, 2.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
			glEnd();
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, welres->skin[3]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-7.5, 2.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 12.5, 2.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 12.5, -2.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-7.5, -2.5, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, welres->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-12.5, 2.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(-7.5, 2.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(-7.5, -2.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-12.5, -2.5, -50.0);
			glEnd();
		}
		glPopMatrix();

		glPushMatrix();
		glTranslated(20.0, -20.0, 0.0);
		glScaled(dScal,dScal,1.0);
		glBindTexture(GL_TEXTURE_2D, welres->skin[1]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-12.5, -2.5, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 12.5, -2.5, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 12.5,  2.5, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-12.5,  2.5, -50.0);
		glEnd();
		if(nInter == 1)
		{
			glBindTexture(GL_TEXTURE_2D, welres->skin[2]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-10.0, 2.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 10.0, 2.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 10.0, -2.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-10.0, -2.5, -50.0);
			glEnd();
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, welres->skin[3]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-7.5, 2.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 12.5, 2.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 12.5, -2.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-7.5, -2.5, -50.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, welres->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-12.5, 2.5, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(-7.5, 2.5, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(-7.5, -2.5, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-12.5, -2.5, -50.0);
			glEnd();
		}
		glPopMatrix();
	}

	double dAngles[] = {180.0, 270.0, 0.0, 90.0, 180.0};
	double dAngle = -90.0*nChoose;
	double dRad = 20.0;
	if(nInter == 1)
	{
		if(nCountLoop >= nDur)
		{
			nCountLoop = 0;
			return 0;
		}
	}
	else
	{
#define TRANS_INIT_CHAIN_WELCOME 800	// Transición inicial de la cadena
#define TRANS_BRIGHT_CHAIN_WELCOME 100	// Transición del brillo
#define TIME_TRANSI_CHOOSE_WELCOME 800	// Transición de los menus
#define MULTI_SCALE_BRIGHT_WELCOME 0.5	// 1.0 +o- 0.5
#define TIME_LIMIT_CHOOSE_WELCOME 20000	// Tiempo para elejir
#define TRANSI_UPANDDOWN_WELCOME 500
#define TRANSI_GLOUTH_UPANDDOWN_WELCOME 300
#define TRANSI_UPANDDOWNEXIT_WELCOME 500
		if(bChooseGame)
		{
			__int64 nTrans = nCountLoop - nInitChooseGame;
			__int64 nTrans2 = nCountLoop - nInitChange;
			double dAmp = 0;
			if(nTrans >= 0 && nTrans < TRANS_INIT_CHAIN_WELCOME) // Init
			{
				dAmp = (1.0-pow((double(nTrans)/TRANS_INIT_CHAIN_WELCOME+1),-10*double(nTrans)/TRANS_INIT_CHAIN_WELCOME));
				dAngle += 2*360*(1.0-dAmp);
				glBindTexture(GL_TEXTURE_2D, m_uNullTex);
				glColor4d(0.0, 0.0, 0.0, dAmp*0.5);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				for(int x = 0; x < 4; x++)
				{
					glPushMatrix();
					glTranslated(((x%2)?-1:1)*(1.0-dAmp)*240.0,dRad*sin((dAngle+dAngles[x])*M_PI/180.0),-dRad*cos((dAngle+dAngles[x])*M_PI/180.0)-75.0);
					glScaled(1.0+5.0*(1.0-dAmp), 1.0+5.0*(1.0-dAmp), 1.0);
					glColor4d(1.0, 1.0, 1.0, (-cos((dAngle+dAngles[x])*M_PI/180.0)+1.0)/2.0);
					glBindTexture(GL_TEXTURE_2D, welres->skin[x+4]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, 0.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, 0.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, 0.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, 0.0);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
			}
			if(nTrans >= 0 && nTrans < TRANSI_UPANDDOWN_WELCOME)
			{
				dAmp = double(nTrans)/TRANSI_UPANDDOWN_WELCOME;
				glBindTexture(GL_TEXTURE_2D, welres->skin[17]->GetCurrentTex());
				glPushMatrix();
				glScaled(2.0-dAmp, 2.0-dAmp, 1.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-80.0,  60.0, -100.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 80.0,  60.0, -100.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 80.0, -60.0, -100.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-80.0, -60.0, -100.0);
				glEnd();
				glPopMatrix();
			}
			if(nTrans >= TRANSI_UPANDDOWN_WELCOME && nTrans < TRANS_INIT_CHAIN_WELCOME)
			{
				dAmp = double(nTrans)/TRANSI_UPANDDOWN_WELCOME;
				glBindTexture(GL_TEXTURE_2D, welres->skin[17]->GetCurrentTex());
				glPushMatrix();
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-80.0,  60.0, -100.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 80.0,  60.0, -100.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 80.0, -60.0, -100.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-80.0, -60.0, -100.0);
				glEnd();
				if(nTrans < (TRANSI_UPANDDOWN_WELCOME+TRANSI_GLOUTH_UPANDDOWN_WELCOME))
				{
					dAmp = double(nTrans-TRANSI_UPANDDOWN_WELCOME)/TRANSI_GLOUTH_UPANDDOWN_WELCOME;
					glBindTexture(GL_TEXTURE_2D, welres->skin[17]->GetCurrentTex());
					glPushMatrix();
					glScaled(1.0+dAmp*0.2, 1.0+dAmp*0.2, 1.0);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-80.0,  60.0, -100.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 80.0,  60.0, -100.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 80.0, -60.0, -100.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-80.0, -60.0, -100.0);
					glEnd();
					glPopMatrix();
				}
			}
#define TIME_EXIT_WELCOME 1000
#define TIME_FINAL_BRIGHT_WELCOME 300
#define CHOOSES_SELECT_WELCOME 2
#define TIME_EXPLOTION_BRIGHT_SELECTED_WELCOME 500
			if(nChooses >= CHOOSES_SELECT_WELCOME)	// Exit
			{
				__int64 nTransExit = nCountLoop - nTimeChoose[CHOOSES_SELECT_WELCOME-1];
				if(nTransExit >= 0 && nTransExit <= TIME_EXIT_WELCOME)
				{
					glBindTexture(GL_TEXTURE_2D, m_uNullTex);
					glColor4d(0.0, 0.0, 0.0, double(nTransExit)/TIME_EXIT_WELCOME*0.5+0.5);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);

					dAmp = pow(double(nTransExit)/TIME_EXIT_WELCOME - 0.25, 2)/0.125 + 0.5;
					glPushMatrix();
					glScaled(dAmp, dAmp, 1.0);
					glColor4d(1.0, 1.0, 1.0, 1.0-double(nTransExit)/TIME_EXIT_WELCOME);
					glBindTexture(GL_TEXTURE_2D, welres->skin[nChoose+4]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, -75.0+dRad);
					glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, -75.0+dRad);
					glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, -75.0+dRad);
					glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, -75.0+dRad);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();

					dAmp = (1.0-pow((double(nTransExit)/TIME_EXIT_WELCOME+1),-10*double(nTransExit)/TIME_EXIT_WELCOME));
					dAngle += 2*360*(dAmp);
					for(int x = 0; x < 4; x++) if(x != nChoose)
					{
						glPushMatrix();
						glTranslated(((x%2)?-1:1)*(dAmp)*240.0,dRad*sin((dAngle+dAngles[x])*M_PI/180.0),-dRad*cos((dAngle+dAngles[x])*M_PI/180.0)-75.0);
						//glScaled(1.0+5.0*(dAmp), 1.0+5.0*(dAmp), 1.0);
						glColor4d(1.0, 1.0, 1.0, (-cos((dAngle+dAngles[x])*M_PI/180.0)+1.0)/2.0);
						glBindTexture(GL_TEXTURE_2D, welres->skin[x+4]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, 0.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, 0.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, 0.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, 0.0);
						glEnd();
						glPopMatrix();
						glColor4d(1.0, 1.0, 1.0, 1.0);
					}
				}
				if(nTransExit >= 0 && nTransExit < TRANSI_UPANDDOWNEXIT_WELCOME)
				{
					dAmp = double(nTransExit)/TRANSI_UPANDDOWNEXIT_WELCOME;
					glBindTexture(GL_TEXTURE_2D, welres->skin[17]->GetCurrentTex());
					glPushMatrix();
					glScaled(1.0+dAmp, 1.0+dAmp, 1.0);
					glColor4d(1.0, 1.0, 1.0, 1.0-dAmp);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-80.0,  60.0, -100.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 80.0,  60.0, -100.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 80.0, -60.0, -100.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-80.0, -60.0, -100.0);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
				if(nTransExit >= 0 && nTransExit <= TIME_FINAL_BRIGHT_WELCOME)
				{
					dAmp = double(nTransExit)/TIME_EXIT_WELCOME;
					glPushMatrix();
					glScaled(1.0+2.0*dAmp, 1.0+2.0*dAmp, 1.0);
					glBindTexture(GL_TEXTURE_2D, welres->skin[9]->GetCurrentTex());
					glColor4d(1.0, 1.0, 1.0, 1.0-dAmp);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, -75.0+dRad);
					glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, -75.0+dRad);
					glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, -75.0+dRad);
					glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, -75.0+dRad);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
				if(nTransExit > TIME_EXIT_WELCOME)
				{
					nCountLoop = 0;
					glBindTexture(GL_TEXTURE_2D, m_uNullTex);
					glColor4d(0.0, 0.0, 0.0, 1.0);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-40.0, -30.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 40.0, -30.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 40.0,  30.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-40.0,  30.0, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					return 2;
				}
				// ** Proceed to exit
			}
			if(nTrans >= TRANS_INIT_CHAIN_WELCOME && nChooses < CHOOSES_SELECT_WELCOME)	//While
			{
				if(nTrans2 >= 0 && nTrans2 < TIME_TRANSI_CHOOSE_WELCOME && nModeChange)
				{
					dAmp = pow(double(nTrans2)/TIME_TRANSI_CHOOSE_WELCOME,0.5);
					dAngle += (nModeChange==1?-1:1)*(1.0-dAmp)*72.0;
				}
				glBindTexture(GL_TEXTURE_2D, m_uNullTex);
				glColor4d(0.0, 0.0, 0.0, 0.5);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				if(nTrans >= TRANSI_UPANDDOWN_WELCOME)
				{
					dAmp = double(nTrans)/TRANSI_UPANDDOWN_WELCOME;
					glBindTexture(GL_TEXTURE_2D, welres->skin[17]->GetCurrentTex());
					glPushMatrix();
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-80.0,  60.0, -100.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 80.0,  60.0, -100.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 80.0, -60.0, -100.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-80.0, -60.0, -100.0);
					glEnd();
					if(nTrans < (TRANSI_UPANDDOWN_WELCOME+TRANSI_GLOUTH_UPANDDOWN_WELCOME))
					{
						dAmp = double(nTrans-TRANSI_UPANDDOWN_WELCOME)/TRANSI_GLOUTH_UPANDDOWN_WELCOME;
						glBindTexture(GL_TEXTURE_2D, welres->skin[17]->GetCurrentTex());
						glPushMatrix();
						glScaled(1.0+dAmp*0.2, 1.0+dAmp*0.2, 1.0);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-80.0,  60.0, -100.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 80.0,  60.0, -100.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 80.0, -60.0, -100.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-80.0, -60.0, -100.0);
						glEnd();
						glPopMatrix();
					}
				}
				CString strTime;
				strTime.Format(_T("%.2d"), (TIME_LIMIT_CHOOSE_WELCOME-nTrans)/1000);
				font_Car->glDrawText(60.0, -80.0, -60.0, 80.0, -74.4, 54.4, -100.0, 1.0, 5.0, strTime, DT_CENTER|DT_VCENTER);
				for(int x = 0; x < 4; x++)
				{
					glPushMatrix();
					glTranslated(0.0,dRad*sin((dAngle+dAngles[x])*M_PI/180.0),-dRad*cos((dAngle+dAngles[x])*M_PI/180.0)-75.0);
					glColor4d(1.0, 1.0, 1.0, (-cos((dAngle+dAngles[x])*M_PI/180.0)+1.0)/2.0);
					glBindTexture(GL_TEXTURE_2D, welres->skin[x+4]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, 0.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, 0.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, 0.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, 0.0);
					glEnd();
					glPopMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0);
				}
				if(nTrans < (TRANS_INIT_CHAIN_WELCOME+TRANS_BRIGHT_CHAIN_WELCOME))	// Brillo de primer instancia
				{
					dAmp = pow((double(nTrans)-TRANS_INIT_CHAIN_WELCOME)/TRANS_BRIGHT_CHAIN_WELCOME,0.5);
					glPushMatrix();
					glScaled(1.0+MULTI_SCALE_BRIGHT_WELCOME*(1.0-dAmp), 1.0+MULTI_SCALE_BRIGHT_WELCOME*(1.0-dAmp), 1.0);
					glBindTexture(GL_TEXTURE_2D, welres->skin[9]->GetCurrentTex());
					glColor4d(1.0, 1.0, 1.0, dAmp);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, -75.0+dRad);
					glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, -75.0+dRad);
					glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, -75.0+dRad);
					glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, -75.0+dRad);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();
				}
				else if(!nModeChange)
				{
					glBindTexture(GL_TEXTURE_2D, welres->skin[9]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, -75.0+dRad);
					glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, -75.0+dRad);
					glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, -75.0+dRad);
					glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, -75.0+dRad);
					glEnd();
				}
				if(nModeChange)
				{
					if(nTrans2 >= 0 && nTrans2 < TRANS_BRIGHT_CHAIN_WELCOME)
					{
						dAmp = 1-pow((double(nTrans2))/TRANS_BRIGHT_CHAIN_WELCOME,0.5);
						glPushMatrix();
						glScaled(1.0+MULTI_SCALE_BRIGHT_WELCOME*(1.0-dAmp), 1.0+MULTI_SCALE_BRIGHT_WELCOME*(1.0-dAmp), 1.0);
						glBindTexture(GL_TEXTURE_2D, welres->skin[9]->GetCurrentTex());
						glColor4d(1.0, 1.0, 1.0, dAmp);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, -75.0+dRad);
						glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, -75.0+dRad);
						glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, -75.0+dRad);
						glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, -75.0+dRad);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nTrans2 >= TIME_TRANSI_CHOOSE_WELCOME && nTrans2 < (TIME_TRANSI_CHOOSE_WELCOME+TRANS_BRIGHT_CHAIN_WELCOME))
					{
						dAmp = pow((double(nTrans2)-TIME_TRANSI_CHOOSE_WELCOME)/TRANS_BRIGHT_CHAIN_WELCOME,0.5);
						glPushMatrix();
						glScaled(1.0+MULTI_SCALE_BRIGHT_WELCOME*(1.0-dAmp), 1.0+MULTI_SCALE_BRIGHT_WELCOME*(1.0-dAmp), 1.0);
						glBindTexture(GL_TEXTURE_2D, welres->skin[9]->GetCurrentTex());
						glColor4d(1.0, 1.0, 1.0, dAmp);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, -75.0+dRad);
						glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, -75.0+dRad);
						glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, -75.0+dRad);
						glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, -75.0+dRad);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nTrans2 >= (TIME_TRANSI_CHOOSE_WELCOME+TRANS_BRIGHT_CHAIN_WELCOME))
					{
						glBindTexture(GL_TEXTURE_2D, welres->skin[9]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, -75.0+dRad);
						glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, -75.0+dRad);
						glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, -75.0+dRad);
						glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, -75.0+dRad);
						glEnd();
					}
				}
				if(nChooses > 0 && nChooses < CHOOSES_SELECT_WELCOME)
				{
					__int64 nTransChoose = nCountLoop - nTimeChoose[nChooses-1];
					if(nTransChoose >= 0)
					{
						dAmp = double(nTransChoose%TIME_EXPLOTION_BRIGHT_SELECTED_WELCOME)/TIME_EXPLOTION_BRIGHT_SELECTED_WELCOME;
						glPushMatrix();
						glScaled(1.0+0.7*dAmp, 1.0+0.7*dAmp, 1.0);
						glBindTexture(GL_TEXTURE_2D, welres->skin[9]->GetCurrentTex());
						glColor4d(1.0, 1.0, 1.0, 1.0-dAmp);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, -75.0+dRad);
						glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, -75.0+dRad);
						glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, -75.0+dRad);
						glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, -75.0+dRad);
						glEnd();
						glLoadIdentity();
						glScaled(1.0+0.5*dAmp, 1.0+0.5*dAmp, 1.0);
						glBindTexture(GL_TEXTURE_2D, welres->skin[nChoose+4]->GetCurrentTex());
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-20.0, 5.0, -75.0+dRad);
						glTexCoord2d(1.0, 1.0); glVertex3d( 20.0, 5.0, -75.0+dRad);
						glTexCoord2d(1.0, 0.0); glVertex3d( 20.0, -5.0, -75.0+dRad);
						glTexCoord2d(0.0, 0.0); glVertex3d(-20.0, -5.0, -75.0+dRad);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
				}
				if(nTrans >= TIME_LIMIT_CHOOSE_WELCOME && (nChoose != 1 || !(np[0] && np[1])))
				{
					nChooses = CHOOSES_SELECT_WELCOME;
					nInitChange = nCountLoop-TIME_TRANSI_CHOOSE_WELCOME;
					nTimeChoose[nChooses-1] = nCountLoop;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
				}
				if(np[0])
				{
					if(bArrows[0] || bArrows [1])	// Move on left
					{
						nChoose--;
						if(nChoose < 0) nChoose = 3;
						nInitChange = nCountLoop;
						nModeChange = 1;
						bArrows[0] = false;
						bArrows[1] = false;
						nChooses = 0;
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_chooseGameChange, 0, &channels[11]);
					}
					if(bArrows[2])
					{
						if((nChoose != 1 || !(np[0] && np[1]) || nChooses != 1)) nChooses++;
						nInitChange = nCountLoop-TIME_TRANSI_CHOOSE_WELCOME;
						nTimeChoose[nChooses-1] = nCountLoop;
						bArrows[2] = false;
						if(nChooses > 0 && nChooses < CHOOSES_SELECT_WELCOME) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
						if(nChooses >= CHOOSES_SELECT_WELCOME) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					}
					if(bArrows[3] || bArrows [4])	// Move on right
					{
						nChoose++;
						if(nChoose > 3) nChoose = 0;
						nInitChange = nCountLoop;
						nModeChange = 2;
						bArrows[3] = false;
						bArrows[4] = false;
						nChooses = 0;
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_chooseGameChange, 0, &channels[11]);
					}
				}
				if(np[1])
				{
					if(bArrows[5] || bArrows [6])	// Move on right
					{
						nChoose--;
						if(nChoose < 0) nChoose = 3;
						nInitChange = nCountLoop;
						nModeChange = 1;
						bArrows[5] = false;
						bArrows[6] = false;
						nChooses = 0;
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_chooseGameChange, 0, &channels[11]);
					}
					if(bArrows[7])
					{
						if((nChoose != 1 || !(np[0] && np[1]) || nChooses != 1)) nChooses++;
						nInitChange = nCountLoop-TIME_TRANSI_CHOOSE_WELCOME;
						nTimeChoose[nChooses-1] = nCountLoop;
						bArrows[7] = false;
						if(nChooses > 0 && nChooses < CHOOSES_SELECT_WELCOME) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
						if(nChooses >= CHOOSES_SELECT_WELCOME) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					}
					if(bArrows[8] || bArrows [9])	// Move on left
					{
						nChoose++;
						if(nChoose > 3) nChoose = 0;
						nInitChange = nCountLoop;
						nModeChange = 2;
						bArrows[8] = false;
						bArrows[9] = false;
						nChooses = 0;
						FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)11, sound_chooseGameChange, 0, &channels[11]);
					}
				}
			}
		}
		if(bArrows[2] == true)
		{
			if(nCredit)
			{
				if(!bChooseGame)
				{
					bChooseGame = true;
					nInitChooseGame = nCountLoop;
					nChoose = 0;				// By default [ABDA_1]
					nInitChange = nCountLoop;
					nModeChange = 0;
					nChooses = 0;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)10, sound_chooseGameIntro, 0, &channels[10]);
				}
				else if(!np[0])
				{
					nInitChooseGame = nCountLoop-TRANS_INIT_CHAIN_WELCOME;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)4, sound_pulse, 0, &channels[4]);
				}
				if(!np[0])
				{
					PlusStagePlayer(0);
					nCredit--;
				}
			}
			bArrows[2] = false;
		}
		if(bArrows[7] == true)
		{
			if(nCredit)
			{
				if(!bChooseGame)
				{
					bChooseGame = true;
					nInitChooseGame = nCountLoop;
					nChoose = 0;				// By default [ABDA_1]
					nInitChange = nCountLoop;
					nModeChange = 0;
					nChooses = 0;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)10, sound_chooseGameIntro, 0, &channels[10]);
				}
				else if(!np[1])
				{
					nInitChooseGame = nCountLoop-TRANS_INIT_CHAIN_WELCOME;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)4, sound_pulse, 0, &channels[4]);
				}
				if(!np[1])
				{
					PlusStagePlayer(1);
					nCredit--;
				}
			}
			bArrows[7] = false;
		}
	}
	return 1;
}
int DrawWelcomeBest()
{
#define TIME_PERRECORDBEST_WELCOME 500
#define TIME_WAITINTRO_RECORDBEST_WELCOME 3000
#define TIME_WAITEXIT_RECORDBEST_WELCOME 3000
#define TIME_TRANSI_EXIT_RECORDBESTBEST 500
#define DIST_RECORDBEST_WELCOME 8.0
#define DISTADD_RECORDBEST_WELCOME 16.0
#define SIZE_RECORDBESTYD2_WELCOME 3.75
#define SIZE_RECORDBESTXD2_WELCOME 30.0
#define PASS_RECORDSCREEN 15
	nCountBest += dwTick - dwLastTick;
	if(nCountBest >= (TIME_WAITINTRO_RECORDBEST_WELCOME+TIME_PERRECORDBEST_WELCOME*PASS_RECORDSCREEN+TIME_WAITEXIT_RECORDBEST_WELCOME+TIME_TRANSI_EXIT_RECORDBESTBEST))
	{
		nCountBest = 0;
		return 0;
	}

	if(movie_best->bLoad)
	{
		movie_best->Update(dwWait);
		movie_best->GrabAVIFrame();
		glBindTexture(GL_TEXTURE_2D, movie_best->tex);

		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
		glEnd();
	}

	glPushMatrix();
	double dx;
	if(nCountBest <= TIME_WAITINTRO_RECORDBEST_WELCOME)
		dx = TIME_PERRECORDBEST_WELCOME*PASS_RECORDSCREEN/TIME_PERRECORDBEST_WELCOME*DIST_RECORDBEST_WELCOME-DIST_RECORDBEST_WELCOME/2+DISTADD_RECORDBEST_WELCOME;
	else if(nCountBest > TIME_WAITINTRO_RECORDBEST_WELCOME && nCountBest <= (TIME_PERRECORDBEST_WELCOME*PASS_RECORDSCREEN+TIME_WAITINTRO_RECORDBEST_WELCOME))
		dx = double(TIME_PERRECORDBEST_WELCOME*PASS_RECORDSCREEN-nCountBest+TIME_WAITINTRO_RECORDBEST_WELCOME)/TIME_PERRECORDBEST_WELCOME*DIST_RECORDBEST_WELCOME-DIST_RECORDBEST_WELCOME/2+DISTADD_RECORDBEST_WELCOME;
	else dx = -DIST_RECORDBEST_WELCOME/2+DISTADD_RECORDBEST_WELCOME;
	glTranslated(0.0, dx, 0.0);
	for(int x = 0; x < 20; x++)
	{
		glPushMatrix();
		glTranslated(0.0, -double(x)*DIST_RECORDBEST_WELCOME, 0.0);
		glBindTexture(GL_TEXTURE_2D, welres->skin[16]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d( -30.0,  3.75, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d(  30.0,  3.75, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d(  30.0, -3.75, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d( -30.0, -3.75, -50.0);
		glEnd();
		CString str;
		str = ( machine_sesion->chRecords[x]);
		font_Micro->glDrawText(3.75, -30.0, -3.75, 30.0, -15.0, 0.0, -50.0, 1.0, 3.0, str, DT_CENTER|DT_VCENTER);
		str.Format(_T("%.8d"), machine_sesion->nRecords[x]);
		font_Micro->glDrawText(3.75, -30.0, -3.75, 30.0, 15.0, 0.0, -50.0, 1.0, 3.0, str, DT_CENTER|DT_VCENTER);
		str.Format(_T("%d"), x+1);
		font_Micro->glDrawText(3.75, -30.0, -3.75, 30.0, -27.0, 0.0, -50.0, 1.0, 3.0, str, DT_CENTER|DT_VCENTER);
		glPopMatrix();
	}
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, welres->skin[15]->GetCurrentTex());
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
	glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
	glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
	glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
	glEnd();

	if(nCountBest >= (TIME_WAITINTRO_RECORDBEST_WELCOME+TIME_PERRECORDBEST_WELCOME*PASS_RECORDSCREEN+TIME_WAITEXIT_RECORDBEST_WELCOME))
	{
		double dApt = double(nCountBest-TIME_WAITINTRO_RECORDBEST_WELCOME-TIME_PERRECORDBEST_WELCOME*PASS_RECORDSCREEN-TIME_WAITEXIT_RECORDBEST_WELCOME)/TIME_TRANSI_EXIT_RECORDBESTBEST;
		glColor4d(0.0, 0.0, 0.0, dApt);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
		glColor4d(1.0, 1.0, 1.0, 1.0);
	}

	return 1;
}


SONG song;
int nSong = 0;
TCHAR chAccSongs[1000][100];
TCHAR chAccModes[1000][100];

int DrawDemo()	// Utilizar esta funcion para despues
{
	int nCode = Draw_All();
	if(!nCode || dwStepSmu >= PIUMAX_WELCOME_RUNTIMESONG) return 0;
	return 1;
}

int LoadSongList()
{
	FILE* hFile;
	if((hFile = _fopen(_T("songdemo.cfg"), _T("r"))) == NULL)
	{
		_printf(_T("El archivo 'songdemo.cfg' no existe o esta corrupto"));
		return 0;
	}
	TCHAR chArg[100];
	CString strArg;
	int nCount = 0;
	while(_fgets(chArg, 100, hFile) != NULL)
	{
		strArg = ( chArg);
		strArg.Remove('\n'); strArg.Remove('\r');
		strArg.Remove('\t');
		int nCode = strArg.Find(_T(' '));
		CString song;
		CString mode;
		if(nCode == -1)
		{
			song = strArg;
			mode = _T("EZ");
		}
		else
		{
			song = strArg.Mid(0, nCode);
			mode = strArg.Mid(nCode+1, strArg.GetLength() - nCode - 1);
		}
		if(!strArg.IsEmpty() && (strArg.Find(':') == -1))
		{
			_strcpy(chAccSongs[nCount], song);
			_strcpy(chAccModes[nCount], mode);
			nCount++;
		}
	}
	fclose(hFile);
	return nCount;
}

int nCurDemoSong = 0;

int LoadDemoSong()
{
	if(g_nCount == 0) return 0;
	ARGSTEP arg;
	arg.dUserVel = 2.0;
	arg.bIsSingle = true;
	arg.bAutoPlay = true;
	arg.nSkin = -1;
	arg.bLoadBS = false;
	arg.bTrainingMode = false;
	arg.bXMode = false;
	arg.bNXMode = false;
	arg.bHJ = false;
	arg.bRS = false;
	arg.bMirror = false;
	arg.bFreedom = false;
	arg.bRV = false;
	arg.bRandomSkin = false;
	arg.nVanish = 0;
	arg.bDC = false;
	arg.bAC = false;
	arg.bEW = false;
	arg.bFlash = false;
	arg.bUnderAttack = false;
	arg.bLeftAttack = false;
	arg.bRightAttack = false;
	arg.bSnake = false;
	arg.nCharacter = 0;
	arg.bCharacter = false;
	arg.nLevelCharacer = 0;
	switch(piuconfig.nBarLevel)
	{
	case 0:
		arg.dIncrBar = BAR_INCR_1;
		arg.dDecrBar = BAR_DECR_1;
		break;
	case 1:
		arg.dIncrBar = BAR_INCR_2;
		arg.dDecrBar = BAR_DECR_2;
		break;
	case 2:
		arg.dIncrBar = BAR_INCR_3;
		arg.dDecrBar = BAR_DECR_3;
		break;
	case 3:
		arg.dIncrBar = BAR_INCR_4;
		arg.dDecrBar = BAR_DECR_4;
		break;
	case 4:
		arg.dIncrBar = BAR_INCR_5;
		arg.dDecrBar = BAR_DECR_5;
		break;
	case 5:
		arg.dIncrBar = BAR_INCR_6;
		arg.dDecrBar = BAR_DECR_6;
		break;
	case 6:
		arg.dIncrBar = BAR_INCR_7;
		arg.dDecrBar = BAR_DECR_7;
		break;
	default:
		arg.dIncrBar = BAR_INCR_1;
		arg.dDecrBar = BAR_DECR_1;
		break;
	}
	switch(piuconfig.nLevelMach)
	{
	case 0:
		arg.dPowTimer = POW_TIMER_EASY;
		break;
	case 1:
		arg.dPowTimer = POW_TIMER_NORMAL;
		break;
	case 2:
		arg.dPowTimer = POW_TIMER_HARD;
		break;
	default:
		arg.dPowTimer = POW_TIMER_EASY;
		break;
	}
	arg.nLevelMach = TIMING_HARD;
	arg.bDrop = false;
	arg.strFile.Format(_T("song/%ls/%ls_%ls.sksf"), chAccSongs[nCurDemoSong], chAccSongs[nCurDemoSong], chAccModes[nCurDemoSong]);
	arg.strMusic.Format(_T("song/%ls/%ls.mp3"), chAccSongs[nCurDemoSong], chAccSongs[nCurDemoSong]);
	arg.strLights.Format(_T("song/%ls/%ls.lgt"), chAccSongs[nCurDemoSong], chAccSongs[nCurDemoSong]);
	CString str; TCHAR ch[260];
	str.Format(_T("song/%ls/%ls.acc"), chAccSongs[nCurDemoSong], chAccSongs[nCurDemoSong]);
	FILE* hFile = _fopen(str, _T("r"));
	if(hFile != NULL)
	{
		if( _fgets(ch, 260, hFile) != NULL)
		{
			arg.strBGA.Format(_T("song/%ls/%ls"), chAccSongs[nCurDemoSong], ch);
			if(arg.strBGA.Find(_T('[')) != -1)
				arg.strBGA.Format(_T("song/GNRBGA/%ls"), ch);
			arg.strBGA.Remove(_T('\n')); arg.strBGA.Remove(_T('\r'));
		}
		else arg.strBGA.Format(_T("song/%ls/%ls.avi"), chAccSongs[nCurDemoSong], chAccSongs[nCurDemoSong]);
		fclose(hFile);
	}
	else arg.strBGA.Format(_T("song/%ls/%ls.avi"), chAccSongs[nCurDemoSong], chAccSongs[nCurDemoSong]);
	arg.strSplash.Format(_T("song/%ls/%ls.png"), chAccSongs[nCurDemoSong], chAccSongs[nCurDemoSong]);
	nCurDemoSong++;
	if(nCurDemoSong >= g_nCount) nCurDemoSong = 0;
	nSec = 0;
	if(!LoadSong(arg)) return 0;
	if(!LoadSongCompile(arg, 1))
	{
		DeleteSong();
		return 0;
	}
	if(!LoadSongCompile(arg, 2))
	{
		DeleteSong();
		DeleteSongCompile(1);
		return 0;
	}
	Go_Step();
	return 1;
}

void DeleteDemoSong()
{
	DeleteSongCompile(1);
	DeleteSongCompile(2);
	DeleteSong();
}
/////////////////////////////////////////////////////////

int CreateContextWelcome()
{
	sys = sys;
	if(!GetConfig()) return 0;
	if(!(g_nCount = LoadSongList())) return 0;

	LoadGLTextures();

	lightsWelcome = new CMaxLights();
	lightsWelcome->Open(_T("welcome/intro_welcome.lgt"));
	lightsWelcomeLoop = new CMaxLights();
	lightsWelcomeLoop->Open(_T("welcome/loop_welcome.lgt"));
	lightsWelcomeLoop->bLoop = true;

	res = FMOD_System_CreateStream(sys, "welcome/loop_welcome.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &music_loop);
	if(!ERRCHECK(res)) return 0;

	res = FMOD_System_CreateStream(sys, "welcome/intro_welcome.mp3", FMOD_HARDWARE | FMOD_2D, 0, &music_welcome);
	if(!ERRCHECK(res)) return 0;

	res = FMOD_System_CreateStream(sys, "welcome/wpiumax.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_wpiumax);
	if(!ERRCHECK(res)) return 0;

	res = FMOD_System_CreateStream(sys, "welcome/ChooseGameChange.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_chooseGameChange);
	if(!ERRCHECK(res)) return 0;

	res = FMOD_System_CreateStream(sys, "welcome/ChooseGameIntro.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_chooseGameIntro);
	if(!ERRCHECK(res)) return 0;

	dwTick = SDL_GetTicks();
	dwLastTick = SDL_GetTicks();
    return 1;
}

void DestroyContextWelcome()
{
	delete lightsWelcome;
	delete lightsWelcomeLoop;

	ClearContextWelcome();
	DeleteTextures();
	FMOD_Channel_Stop(channels[8]);
	FMOD_Sound_Release(music_welcome); music_welcome = 0;
	FMOD_Sound_Release(music_loop); music_loop = 0;
    return;
}

void ActualizateContextWelcome()
{
	if (movie_welcome == NULL)
	{
		movie_welcome = new CMovieStream2();
		movie_welcome->OpenAVI(_T("welcome/welcome.avi"));
		movie_welcome->bLoop = false;
	}
	if (movie_loop == NULL)
	{
		movie_loop = new CMovieStream2();
		movie_loop->OpenAVI(_T("welcome/welcome_loop.avi"));
		movie_loop->bLoop = true;
	}
	if (movie_best == NULL)
	{
		movie_best = new CMovieStream2();
		movie_best->OpenAVI(_T("welcome/welcome_best.avi"));
		movie_best->bLoop = true;
	}


	for(int x = 0; x < 10; x++) {
        bArrows[x] = false;
    }

	//LoadGLTextures();

	nCountLoop = 0;
	bChooseGame = false;
	nChooses = 0;
	nCur = 0;
	nSec = 0; nDur = 0;
	nModeChange = 0;		// 0 ninguno, 1 izq, 2 der
	nChoose = 0;			// Interfase
	for(int x = 0; x < 2; x++)
	{
		nTimeChoose[x] = 0;
	}
	if(nCredit)
	{
		nInter = 4;
		nDur = -1;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)8, music_loop, 0, &channels[8]);
		FMOD_Channel_SetPosition(channels[8], 0, FMOD_TIMEUNIT_MS);
	}
	else
	{
		nInter = 0;
		unsigned int nDuration = 0;
		res = FMOD_Sound_GetLength(music_welcome, &nDuration, FMOD_TIMEUNIT_MS);
		nDur = nDuration;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)8, music_welcome, 0, &channels[8]);
		FMOD_Channel_SetPosition(channels[8], 0, FMOD_TIMEUNIT_MS);
	}
	movie_welcome->SetZeroCount();
	movie_loop->SetZeroCount();
	movie_best->SetZeroCount();
	if(!nCredit) SetVolume(piuconfig.nSoundPrev?0:1);
	dwTick = dwLastTick = SDL_GetTicks();
}

void ClearContextWelcome()
{
	if (movie_welcome)
	{
		if (movie_welcome->bLoad) movie_welcome->CloseAVI();
		delete movie_welcome;
		movie_welcome = NULL;
	}

	if (movie_loop)
	{
		if (movie_loop->bLoad) movie_loop->CloseAVI();
		delete movie_loop;
		movie_loop = NULL;
	}

	if (movie_best)
	{
		if (movie_best->bLoad) movie_best->CloseAVI();
		delete movie_best;
		movie_best = NULL;
	}
	//DeleteTextures();
}

int FrameWelcome()
{
	// Habiendo inicializado el OpenGL, ahora se va a graficar la pantalla de bienvenida
	welres->Update(dwWait);
	switch(nInter)
	{
	case 0:	// Pantalla de bienvenida
		if(!DrawWelcome())
		{
			SetCurDir();
			nInter = 1;
			movie_welcome->SetZeroCount();
			nDur = w_config.m_nLLoop;
			FMOD_Channel_Stop(channels[8]);
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)8, music_loop, 0, &channels[8]);
			FMOD_Channel_SetPosition(channels[8], 0, FMOD_TIMEUNIT_MS);
			lightsWelcomeLoop->SetZero();
			if(!nCredit) SetVolume(piuconfig.nSoundPrev?0:1);
		}
		break;
	case 1:	// Pantalla de bienvenida loop
		if(!DrawWelcomeLoop())
		{
			SetCurDir();
			nInter = 2;
			FMOD_Channel_Stop(channels[8]);
			movie_loop->SetZeroCount();
			if(!LoadDemoSong())nInter = 3;
			if(!nCredit) SetVolume(piuconfig.nSoundPrev?0:1);
			portLights->WriteVal(0);
		}
		break;
	case 2:
		if(!DrawDemo())
		{
			SetCurDir();
			nInter = 3;
			DeleteDemoSong();
			portLights->WriteVal(0);
		}
		break;
	case 3:	// Best Scores
		if(!DrawWelcomeBest())
		{
			SetCurDir();
			nInter = 0;
			movie_best->SetZeroCount();
			unsigned int nDuration = 0;
			res = FMOD_Sound_GetLength(music_welcome, &nDuration, FMOD_TIMEUNIT_MS);
			if(!ERRCHECK(res)) return 0;
			nDur = nDuration;
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)8, music_welcome, 0, &channels[8]);
			FMOD_Channel_SetPosition(channels[8], 0, FMOD_TIMEUNIT_MS);
			lightsWelcome->SetZero();
			if(!nCredit) SetVolume(piuconfig.nSoundPrev?0:1);
		}
		break;
	case 4:
		if(DrawWelcomeLoop() == 2)
		{
			SetCurDir();
			FMOD_Channel_Stop(channels[8]);
			portLights->WriteVal(0);
			switch (nChoose)
			{
			case 1:
				return INT_GAME_2;
				break;
			case 2:
				return INT_GAME_4;
				break;
			case 3:
				return INT_GAME_5;
				break;
			case 4:
				return INT_GAME_3;
				break;
			default:
				return INT_GAME_1;
				break;
			}
		}
		break;
	}
	return INT_WELCOME;
}

int EventWelcome(int nEvent, int nKey)
{
	if(nEvent == EV_KEYDOWN)
	{
		if(nKey == EV_CREDIT1 || nKey == EV_CREDIT2 || nKey == EV_SERVICE)
		{
			if(nCredit == 1 && nInter != 1 && nInter != 4)
			{
				if(nInter == 2)
				{
					DeleteDemoSong();
				}
				SetVolume(0);
				nInter = 4;
				FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)9, sound_wpiumax, 0, &channels[9]);
				FMOD_Channel_SetPosition(channels[9], 0, FMOD_TIMEUNIT_MS);
				FMOD_Channel_Stop(channels[8]);
				FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)8, music_loop, 0, &channels[8]);
				FMOD_Channel_SetPosition(channels[8], 0, FMOD_TIMEUNIT_MS);
				lightsWelcomeLoop->SetZero();
				nDur = -1;
			}
			if(nCredit == 1 && nInter == 1) nInter = 4;
		}
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
