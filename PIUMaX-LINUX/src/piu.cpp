#include "stdafx.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "USBsys.h"
#include "welcome.h"
#include "game1.h"
#include "game2.h"
#include "Game3.h"
#include "Game4.h"
#include "Game5.h"
#include "UnlockSys.h"
#include "stepeval.h"
#include "History.h"
#include "config.h"
#include "libc_max_common.h"

#define MAX_LOADSTRING 100

DWORD nInterface = INT_WELCOME;								// Interfase actual
DWORD nBackInterface = INT_WELCOME;
bool bConfigCh = false;
int nCredit = 0;								// Creditos
int nSubCredit1 = 0;							// Creditos
int nSubCredit2 = 0;							// Creditos
DWORD dwTick, dwLastTick;

/*DWORD dwTimeAVFrame=0;
DWORD dwTimeAVScale=0;
DWORD dwTimeProcess=0;
DWORD dwTimeSwap=0;
DWORD dwTimeShow=0;
DWORD dwTimeInput=0;
int nPIUIOEvent = 0;
CString chProcess;
int nBeforeStateAcc=0;
int nStateAcc=0;*/
GLboolean bIsDirect;

#ifdef PIUMAX_FOR_HOS
bool bFull = false;
#else
bool bFull = true;
#endif

/* Branch optimizations: */
#if defined(__GNUC__)
#define likely(x) (__builtin_expect(!!(x), 1))
#define unlikely(x) (__builtin_expect(!!(x), 0))
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

template<class T>
inline bool IsBitSet( const T &data, int bit )
{
	// -1, since we have 32 bits indexed as 0..31
	int bits = sizeof(T) * 8 - 1;
	return data & ((T)1 << (bits-bit));
}

template<class T>
inline void SetBit( T &data, int bit, bool on = true )
{
	// -1, since we have 32 bits indexed as 0..31
	int bits = sizeof(T) * 8 - 1;
	int bitval = ((T)1 << (bits-bit));

	data = on ? data | bitval : data & ~bitval;
}

int nResWidth = 800;
int nResHeight = 600;
DWORD PIUFPS = 60;
FMOD_RESULT       result;
unsigned int      version;
FMOD_SYSTEM      *sys = 0;
FMOD_SOUND		*sound_credit = 0;
FMOD_SOUND		*sound_subcred = 0;
FMOD_SOUND *sound_preStart = 0;
FMOD_SOUND *sound_start = 0;
FMOD_SOUND *sound_pulse = 0;
FMOD_SOUND *sound_change1 = 0;
FMOD_SOUND *sound_change2 = 0;
FMOD_SOUND *sound_change3 = 0;
FMOD_SOUND *sound_switch = 0;
FMOD_SOUND *sound_lock = 0;
FMOD_SOUND *sound_lockoff = 0;
FMOD_SOUND *sound_option = 0;
TCHAR chLang[3];
CGLFont2* font_Micro;
CGLFont2 *font_Micro2;
CGLFont2 *font_Micro3;
CGLFont2* font_Car;
CGLFont2 *font_System;
//HDC memdc;
TCHAR chStdModes[10][5];
TCHAR chSpeModes[5][5];
TCHAR chBraModes[5][5];
PIUCONFIG piuconfig;
DWORD dwWait = 0;

bool bArrows[10];
int np[2];
int nGame;
int nInterAfterUsb = 0;

int ProcEvent(SDL_Event keyevent, int ad);
GLuint m_uNullTex;

int nSizeTotalGame1 = 0;
int nSizeTotalGame2 = 0;
int nSizeTotalGame3 = 0;
int nSizeTotalGame4 = 0;
int nSizeTotalGame5 = 0;

int nSizeTotalGame1Ex = 0;
int nSizeTotalGame2Ex = 0;
int nSizeTotalGame3Ex = 0;
int nSizeTotalGame4Ex = 0;
int nSizeTotalGame5Ex = 0;

int nCurStage = 0;

int nJoystick = 0;
SDL_Joystick** joystick;
int* nJoystickHat;
int** nValueJoystickHat;
int* nJoystickButtons;
int** nValueJoystickButtons;

void DrawInterface();

// ** Utilidades para eventos
#define KEY_QUIT -0xFFFE
int nSizeEvent;
int* nKeyEvent;
int* nEventEvent;
int EventThread(void* data);
int PollEventThread(int* nKey, int* nEvent);
int done = 0;

// Variables de evento
bool bShowGeneralOffset = false;
__int64 nTimeGeneralOffset = 0;

// ** Ayuda para el OpenGL
void InitGL(int Width, int Height)
{
    glViewport(0, 0, Width, Height);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	/*glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);*/

	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	glFrustum(-40.0,40.0,-30.0,30.0,50.0,100.0);

    glMatrixMode(GL_MODELVIEW);
}
//////////////////////////
#if defined(UNICODE) && defined(_WIN32) && !defined(_DEBUG)
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR tComm, int nCmd)
#elif defined(UNICODE) && defined(_WIN32)
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	SetCurDir();
	_strcpy(chStdModes[0], _T("EZ"));
	_strcpy(chStdModes[1], _T("HD"));
	_strcpy(chStdModes[2], _T("CZ"));
	_strcpy(chStdModes[3], _T("FS"));
	_strcpy(chStdModes[4], _T("NM"));
	_strcpy(chStdModes[5], _T("AEZ"));
	_strcpy(chStdModes[6], _T("AHD"));
	_strcpy(chStdModes[7], _T("ACZ"));
	_strcpy(chStdModes[8], _T("AFS"));
	_strcpy(chStdModes[9], _T("ANM"));
	_strcpy(chSpeModes[0], _T("AEZ"));
	_strcpy(chSpeModes[1], _T("AHD"));
	_strcpy(chSpeModes[2], _T("ACZ"));
	_strcpy(chSpeModes[3], _T("AFS"));
	_strcpy(chSpeModes[4], _T("ANM"));
	_strcpy(chBraModes[0], _T("Mem"));
	_strcpy(chBraModes[1], _T("Obs"));
	_strcpy(chBraModes[2], _T("Mat"));
	_strcpy(chBraModes[3], _T("Ana"));
	_strcpy(chBraModes[4], _T("Com"));
	for(int x = 0; x < 10; x++)
	{
		bArrows[x] = false;
	}
	nGeneralCount = 0;

	for(int x = 0; x < 2; x++)
	{
		nTimeShowSesionTray[x] = 0;
		nTimeShowSesionTrayMileage[x] = 0;
		nTimeShowSesionTrayExp[x] = 0;
		bShowSesionTray[x] = false;
		bShowSesionTrayMileage[x] = false;
		bShowSesionTrayExp[x] = false;
	}
	// ** Inicializacion para video
	av_register_all();
	///////////////////////////////
	Max_init_iconv();

	// ** Inicializacion para modo
	FILE* hFileMode = _fopen(_T("mode.acc"), _T("r"));
	if(hFileMode != NULL)
	{
		if(_fscanf(hFileMode, _T("%d\n"), &nResWidth) != 1)
		{
			_printf(_T("ERROR: mal formato de mode.acc\n"));
			////_getch();
			return 6;
		}
		if(_fscanf(hFileMode, _T("%d\n"), &nResHeight) != 1)
		{
			_printf(_T("ERROR: mal formato de mode.acc\n"));
			//_getch();
			return 6;
		}
		int fps = 60;
		if(_fscanf(hFileMode, _T("%d\n"), &fps) != 1)
		{
			fps = 60;
			_printf(_T("ERROR: mal formato de mode.acc [FPS]\n"));
		}
		PIUFPS = (DWORD)fps;
		fclose(hFileMode);
	}
	//////////////////////////////

	// ** Inicialización para el puntero global de luces

	int nPortID = 0x378;
	hFileMode = _fopen(_T("plights.acc"), _T("r"));
	if(hFileMode != NULL)
	{
		if(_fscanf(hFileMode, _T("%x\n"), &nPortID) != 1)
		{
			_printf(_T("ERROR: mal formato de mode.acc\n"));
			//_getch();
			return 7;
		}
		fclose(hFileMode);
	}
	////////////////////////////////////////////////////

	//nSizeMovies = 0;
	machine_sesion = new MACHINE_PROFILE();
	player_sesion[0] = new PLAYER();
	player_sesion[1] = new PLAYER();
	np[0] = 0;
	np[1] = 0;
	_strcpy(chLang, _T("ES"));
	nGame = 0;
	font_Micro = new CGLFont2();
	font_Car = new CGLFont2();
	font_Micro2 = new CGLFont2();
	font_Micro3 = new CGLFont2();
	font_System = new CGLFont2();
	_printf(_T("Inicializando SDL\n"));
    if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0 )
    {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
		//_getch();
        return 1;
    }

    //SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &nBeforeStateAcc);
    /*nStateAcc = */SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    if(bFull) _printf(_T("Colocando el modo de video a %dx%d FullScreen\n"), nResWidth, nResHeight);
	else _printf(_T("Colocando el modo de video a %dx%d No FullScreen\n"), nResWidth, nResHeight);
	if ( SDL_SetVideoMode(nResWidth, nResHeight, 0, SDL_OPENGL|(bFull?SDL_FULLSCREEN:0)) == NULL )
    {
        printf("Unable to create OpenGL screen: %s\n", SDL_GetError());
		//_getch();
        SDL_Quit();
        return 2;
    }

    SDL_WM_SetCaption("Afix Systems(R) PIU", NULL);
	nJoystick = SDL_NumJoysticks();

	if(nJoystick != 0)
	{
		joystick = new SDL_Joystick*[nJoystick];
		nJoystickHat = new int[nJoystick];
		nJoystickButtons = new int[nJoystick];
		nValueJoystickHat = new int*[nJoystick];
		nValueJoystickButtons = new int*[nJoystick];
		for(int x = 0; x < nJoystick; x++)
		{
			joystick[x] = SDL_JoystickOpen(x);
			nJoystickHat[x] = SDL_JoystickNumHats(joystick[x]);
			nJoystickButtons[x] = SDL_JoystickNumButtons(joystick[x]);
			nValueJoystickHat[x] = new int[nJoystickHat[x]];
			nValueJoystickButtons[x] = new int[nJoystickButtons[x]];
			for(int y = 0; y < nJoystickHat[x]; y++)
				nValueJoystickHat[x][y] = 0x00;
			for(int y = 0; y < nJoystickButtons[x]; y++)
				nValueJoystickButtons[x][y] = 0;
		}
	}

	// Init PIUIO
	g_ihPIUIO = new InputHandler_PIUIO();
	if(!g_ihPIUIO->m_bFoundDevice) printf("PIUIO: Failed\n");

	// Init portlights
	portLights = new CPortBuf();
	if(!portLights->digitalio(nPortID)) return 10;
		portLights->WriteVal(0);

    InitGL(nResWidth, nResHeight);

#ifndef _WIN32
    Display *disp = NULL;
    disp = XOpenDisplay(NULL);
    bIsDirect = glXIsDirect( disp, glXGetCurrentContext() );
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	SDL_GL_SwapBuffers();
	SDL_ShowCursor(SDL_DISABLE);
	_printf(_T("Inicializando FMODEx\n"));
	result = FMOD_System_Create(&sys);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_GetVersion(sys, &version);
	if(!ERRCHECK(result)) return 0;

	if (version < FMOD_VERSION)
	{
		_printf(_T("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n"), version, FMOD_VERSION);
		//_getch();
		return 0;
	}
#define WAITFORFMODSOUND 10000
	DWORD dwCheck = SDL_GetTicks();
	if(!ERRCHECK(FMOD_System_Init(sys, MAX_CHANNELS, FMOD_INIT_NORMAL, 0)))
	{
		SDL_Delay(1000);
		if((SDL_GetTicks() - dwCheck) >= WAITFORFMODSOUND)
		{
			_printf(_T("Tiempo de espera de FMOD alcanzado, PIUMaX termina"));
			return 0;
		}
	}

	result = FMOD_System_CreateStream(sys, "credit.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_credit);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "subcred.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_subcred);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "Prestart.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_preStart);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "Start.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_start);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "pulse.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_pulse);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "change1.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_change1);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "change2.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_change2);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "change3.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_change3);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "switch.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_switch);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "Lock.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_lock);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "LockOff.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_lockoff);
	if(!ERRCHECK(result)) return 0;

	result = FMOD_System_CreateStream(sys, "option.wav", FMOD_HARDWARE | FMOD_2D, 0, &sound_option);
	if(!ERRCHECK(result)) return 0;


	_printf(_T("Inicializando Recursos\n"));
	m_uNullTex = 0;
	if(!LoadMachineProfile())
	{
		if(!GenDefMach()) return 5;
		if(!GenDefPlayer()) return 6;
	}
#ifdef PIUMAX_FOR_HOS
	FILE* hTest = _fopen(_T("/SAVE/PIUMAX/MAXsave.bin"), _T("r")/*b*/);
#else
    FILE* hTest = _fopen(_T("MAXsave.bin"), _T("r")/*b*/);
#endif
	if(hTest == NULL)
	{
		if(!GenDefPlayer()) return 6;
	}
	else fclose(hTest);
	if(!CreateContextConfig()) return 1;
	_printf(_T("Inicializando Welcome\n"));
	if(!CreateContextWelcome()) return 1;
	_printf(_T("Inicializando USB\n"));
	if(!CreateContextUsb()) return 1;
	_printf(_T("Inicializando Game1\n"));
	if(!CreateContextGame1()) return 1;
	_printf(_T("Inicializando Game2\n"));
	if(!CreateContextGame2()) return 1;
	//_printf(_T("Inicializando Game3\n"));
	//if(!CreateContextGame3()) return 1;
	_printf(_T("Inicializando Game4\n"));
	if(!CreateContextGame4()) return 1;
	_printf(_T("Inicializando Game5\n"));
	if(!CreateContextGame5()) return 1;
	_printf(_T("Inicializando StepEval\n"));
	if(!CreateContextStepEval()) return 1;
	_printf(_T("Inicializando Unlock\n"));
	if(!CreateContextUnlock()) return 1;
	_printf(_T("Inicializando History\n"));
	if(!CreateContextHistory()) return 1;
	_printf(_T("Inicializando Fuentes\n"));
	font_Car->LoadFont(_T("Arial.fnt"));
	font_Micro->LoadFont(_T("micrograma.fnt"));
	font_Micro2->LoadFont(_T("microgramaTrek.fnt"));
	font_Micro3->LoadFont(_T("MicrogramaTrek2.fnt"));
	font_System->LoadFont(_T("System.fnt"));

	ActualizateContextWelcome();

	/////////////////////////////////////////////////////////**** Experimental
	_printf(_T("Creando hilo de eventos para coins\n"));
	nSizeEvent = 0;
	nKeyEvent=(int*)calloc(0, sizeof(int));
	nEventEvent=(int*)calloc(0, sizeof(int));
	SDL_Thread *eventThread;
	SDL_mutex *lockEvent;
	lockEvent = SDL_CreateMutex();
	eventThread = SDL_CreateThread(EventThread, lockEvent);
	if(eventThread == NULL)
	{
		_printf(_T("Hilo de eventos: ERROR: no se pudo crear\n"));
		return 1;
	}
	/////////////////////////////////////////////////////////
	_printf(_T("Hecho! Disfrute PIU MaX\n"));
	dwTick = dwLastTick = SDL_GetTicks();
#define PIUFPSINACTIVE PIUFPS
	DWORD dwTimePerFrame = 1000/PIUFPS;
    while ( ! done )
    {
		Uint8 st=SDL_GetAppState();
		if((st==SDL_APPACTIVE) || (! st)) dwTimePerFrame = 1000/PIUFPSINACTIVE;
		else dwTimePerFrame = 1000/PIUFPS;
		dwLastTick = dwTick;
		dwTick = SDL_GetTicks();
		DWORD dwAdit = dwTick - dwLastTick;
		__int64 nWait = __int64(dwTimePerFrame)-__int64(dwAdit);
		nWait = 0<nWait?nWait:0;
		dwWait = (DWORD)(nWait);
		if(dwWait != 0) SDL_Delay((DWORD)dwWait);
		dwTick = SDL_GetTicks();
		dwWait = dwTick - dwLastTick;

        //DWORD dwTime = SDL_GetTicks();
		SDL_mutexP(lockEvent);
		SDL_Event keyevent;
		while(nSizeEvent > 0)
		{
			memset(&keyevent, 0, sizeof(SDL_Event));
			if(!ProcEvent(keyevent, 1)) done = 1;
		}
		while ( SDL_PollEvent(&keyevent) )
		{
			if ( keyevent.type == SDL_QUIT )
			{
				done = 1;
			}
			if(!ProcEvent(keyevent, 0)) done = 1;
		}
		SDL_mutexV(lockEvent);
		//dwTime = SDL_GetTicks() - dwTime;
		//if(dwTime > dwTimeInput) dwTimeInput = dwTime;
		DrawInterface();
    }

	SDL_WaitThread(eventThread, NULL);
	SDL_DestroyMutex(lockEvent);

	DestroyContextConfig();
	DestroyContextWelcome();
	DestroyContextUsb();
	DestroyContextGame1();
	DestroyContextGame2();
	DestroyContextGame3();
	DestroyContextGame4();
	DestroyContextGame5();
	DestroyContextUnlock();
	DestroyContextStepEval();
	DestroyContextHistory();
	FMOD_Sound_Release(sound_credit);
	FMOD_Sound_Release(sound_preStart);
	FMOD_Sound_Release(sound_start);
	FMOD_Sound_Release(sound_pulse);
	FMOD_Sound_Release(sound_change1);
	FMOD_Sound_Release(sound_change2);
	FMOD_Sound_Release(sound_change3);
	FMOD_Sound_Release(sound_switch);
	FMOD_Sound_Release(sound_lock);
	FMOD_Sound_Release(sound_lockoff);
	FMOD_Sound_Release(sound_option);
	result = FMOD_System_Close(sys);
    result = FMOD_System_Release(sys);
	if(nJoystick != 0)
	{
		for(int x = 0; x < nJoystick; x++)
		{
			SDL_JoystickClose(joystick[x]);
		}
		delete[] joystick;
		delete nJoystickHat;
		delete[] nValueJoystickHat;
		delete nJoystickButtons;
		delete[] nValueJoystickButtons;
	}
    SDL_Quit();

    delete g_ihPIUIO;
    g_ihPIUIO = NULL;

	delete machine_sesion;
	delete player_sesion[0];
	delete player_sesion[1];

	delete portLights;

	delete font_Car;
	delete font_Micro;
	delete font_Micro2;
	delete font_Micro3;
	delete font_System;

	free(nKeyEvent);
	free(nEventEvent);

    return 0;
}
void DrawInterface()
{
    //DWORD dwTime = SDL_GetTicks();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	nBackInterface = nInterface;
	if(bConfigCh)
	{
		bConfigCh = false;
		if(nInterface != INT_USB)
		{
			switch(nInterface)
			{
			case INT_GAME_1: ClearContextGame1(); break;
			case INT_GAME_2: ClearContextGame2(); break;
			case INT_GAME_3: ClearContextGame3(); break;
			case INT_GAME_4: ClearContextGame4(); break;
			case INT_GAME_5: ClearContextGame5(); break;
			case INT_WELCOME: ClearContextWelcome(); FMOD_Channel_Stop(channels[8]); break;
			}
			ActualizateContextConfig();
			nBackInterface = nInterface = INT_CONFIG;
			np[0] = 0;
			np[1] = 0;
			DeleteSongCompile(1);
			DeleteSongCompile(2);
			if(g_bIsMusic) DeleteSong();
		}
	}
	if((piuconfig.nCoinsCred1 == 0 || piuconfig.nCoinsCred2 == 0))
	{
		if(nCredit == 0 && nInterface == INT_WELCOME)
		{
			nCredit = 1;
			EventWelcome(EV_KEYDOWN, EV_CREDIT1);
		}
		nCredit = 1;
	}
	if(nInterface == INT_GAME_1) nInterface = FrameGame1();
	else if(nInterface == INT_GAME_2) nInterface = FrameGame2();
	else if(nInterface == INT_GAME_3) nInterface = FrameGame3();
	else if(nInterface == INT_GAME_4) nInterface = FrameGame4();
	else if(nInterface == INT_GAME_5) nInterface = FrameGame5();
	else if(nInterface == INT_USB) nInterface = FrameUsb();
	else if(nInterface == INT_WELCOME) nInterface = FrameWelcome();
	else if(nInterface == INT_CONFIG) nInterface = FrameConfig();
	else if(nInterface == INT_GAMEOVER)
	{
		if(!FrameGameOverStepEval())
		{
			ActualizateContextWelcome();
			nInterface = nBackInterface = INT_WELCOME;
		}
	}
	if(nInterface != INT_WELCOME) nGeneralCount += dwTick - dwLastTick;
	glColor4d(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, welres->skin[10]->GetCurrentTex());
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 1.0); glVertex3d(-9.409, -29.2, -50.0);
	glTexCoord2d(1.0, 1.0); glVertex3d( 9.409, -29.2, -50.0);
	glTexCoord2d(1.0, 0.0); glVertex3d( 9.409, -27.4, -50.0);
	glTexCoord2d(0.0, 0.0); glVertex3d(-9.409, -27.4, -50.0);
	glEnd();
	CString str;
	if(piuconfig.nCoinsCred1 == 0 || piuconfig.nCoinsCred2 == 0) str = _T("FreePlay");
	else str.Format(_T("CREDITS: %d ( %d / %d )"), nCredit, nSubCredit1, piuconfig.nCoinsCred1);
	font_Micro3->glDrawText(-27.4, -9.409, -29.2, 9.409, 0.0, -28.1, -50.0, 1.0, 1.5, str, DT_CENTER|DT_VCENTER);
#define TIME_SHOW_GENERALOFFSET 3000
	if((SDL_GetTicks()-nTimeGeneralOffset)>=0 && (SDL_GetTicks()-nTimeGeneralOffset)<=TIME_SHOW_GENERALOFFSET && bShowGeneralOffset)
	{
		str.Format(_T("GENERAL OFFSET: %d ms"), piuconfig.nMachineOffset);
		font_Micro->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, 0.0, -50.0, 1.0, 2.0, str, DT_CENTER|DT_VCENTER);
	}
	for(int x = 0; x < 2; x++)
	{
		double dApt;
		if(bShowSesionTray[x])
		{
#define TIME_TRANSI_SESIONTRAY 500
			__int64 nTimeShow = nGeneralCount - nTimeShowSesionTray[x];
			if(nTimeShow >= 0 && nTimeShow < TIME_TRANSI_SESIONTRAY)
			{
				dApt = double(nTimeShow)/TIME_TRANSI_SESIONTRAY;
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, dApt);
				glTranslated(x?32.0:-32.0+(x?8.0:-8.0)*(1-dApt), -28.0, 0.0);
				glScaled(x?-1.0:1.0, 1.0, 1.0);
				glBindTexture(GL_TEXTURE_2D, welres->skin[11]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 8.0, -1.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-8.0, -1.0, -50.0);
				glEnd();
				glPopMatrix();
				glPushMatrix();
				glTranslated(x?22.0:-22.0, -28.0, 0.0);
				glScaled(dApt, dApt, 1.0);
				glBindTexture(GL_TEXTURE_2D, uTexPlayerSplash[x]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-2.0,  2.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 2.0,  2.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 2.0, -2.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-2.0, -2.0, -50.0);
				glEnd();
				glPopMatrix();
				glPushMatrix();
				glTranslated(x?32.0:-32.0+(x?8.0:-8.0)*(1-dApt), -28.0, 0.0);
				CString str;
				TCHAR chn[12];
				Utf16ToUtf32(player_sesion[x]->chName, chn, 12);
				str = chn;
				font_Micro->glDrawText(2.0, -8.0, -2.0, 8.0, 0.0, 0.0, -50.0, 1.0, 1.2, str, DT_CENTER|DT_VCENTER);
				glPopMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0);
			}
			if(nTimeShow >= TIME_TRANSI_SESIONTRAY)
			{
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPushMatrix();
				glTranslated(x?32.0:-32.0, -28.0, 0.0);
				glScaled(x?-1.0:1.0, 1.0, 1.0);
				glBindTexture(GL_TEXTURE_2D, welres->skin[11]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 8.0, -1.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-8.0, -1.0, -50.0);
				glEnd();
				glPopMatrix();
				glPushMatrix();
				glTranslated(x?22.0:-22.0, -28.0, 0.0);
				glBindTexture(GL_TEXTURE_2D, uTexPlayerSplash[x]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-2.0,  2.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 2.0,  2.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 2.0, -2.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-2.0, -2.0, -50.0);
				glEnd();
				glPopMatrix();
				glPushMatrix();
				glTranslated(x?32.0:-32.0, -28.0, 0.0);
				CString str;
				TCHAR chn[12];
				Utf16ToUtf32(player_sesion[x]->chName, chn, 12);
				str = chn;
				font_Micro->glDrawText(2.0, -8.0, -2.0, 8.0, 0.0, 0.0, -50.0, 1.0, 1.2, str, DT_CENTER|DT_VCENTER);
				glPopMatrix();
			}
		}
		if(bShowSesionTrayMileage[x])
		{
#define TIME_TRANSI_SESIONTRAYMILEAGE 500
			__int64 nTimeShow = nGeneralCount - nTimeShowSesionTrayMileage[x];
			if(nTimeShow >= 0 && nTimeShow < TIME_TRANSI_SESIONTRAYMILEAGE)
			{
				dApt = double(nTimeShow)/TIME_TRANSI_SESIONTRAYMILEAGE;
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, dApt);
				glTranslated(x?32.0:-32.0+(x?8.0:-8.0)*(1-dApt), -20.0, 0.0);
				glScaled(x?-1.0:1.0, 1.0, 1.0);
				glBindTexture(GL_TEXTURE_2D, welres->skin[12]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 8.0, -1.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-8.0, -1.0, -50.0);
				glEnd();
				glPopMatrix();
				glPushMatrix();
				glTranslated(x?32.0:-32.0+(x?8.0:-8.0)*(1-dApt), -20.0, 0.0);
				CString str;
				str.Format(_T("M: %d"), player_sesion[x]->nMileage);
				font_Micro->glDrawText(2.0, -8.0, -2.0, 8.0, 0.0, 0.0, -50.0, 1.0, 1.2, str, DT_CENTER|DT_VCENTER);
				glPopMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0);
			}
			if(nTimeShow >= TIME_TRANSI_SESIONTRAYMILEAGE)
			{
				glPushMatrix();
				glTranslated(x?32.0:-32.0, -20.0, 0.0);
				glScaled(x?-1.0:1.0, 1.0, 1.0);
				glBindTexture(GL_TEXTURE_2D, welres->skin[12]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 8.0, -1.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-8.0, -1.0, -50.0);
				glEnd();
				glPopMatrix();
				glPushMatrix();
				glTranslated(x?32.0:-32.0, -20.0, 0.0);
				CString str;
				str.Format(_T("M: %d"), player_sesion[x]->nMileage);
				font_Micro->glDrawText(2.0, -8.0, -2.0, 8.0, 0.0, 0.0, -50.0, 1.0, 1.2, str, DT_CENTER|DT_VCENTER);
				glPopMatrix();
			}
		}
		if(bShowSesionTrayExp[x])
		{
#define TIME_TRANSI_SESIONTRAYEXP 500
			__int64 nTimeShow = nGeneralCount - nTimeShowSesionTrayExp[x];
			if(nTimeShow >= 0 && nTimeShow < TIME_TRANSI_SESIONTRAYEXP)
			{
				dApt = double(nTimeShow)/TIME_TRANSI_SESIONTRAYEXP;
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, dApt);
				glTranslated(x?32.0:-32.0+(x?8.0:-8.0)*(1-dApt), -24.0, 0.0);
				glScaled(x?-1.0:1.0, 1.0, 1.0);
				glBindTexture(GL_TEXTURE_2D, welres->skin[13]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 8.0, -1.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-8.0, -1.0, -50.0);
				glEnd();
				glPopMatrix();
				glPushMatrix();
				glTranslated(x?32.0:-32.0+(x?8.0:-8.0)*(1-dApt), -24.0, 0.0);
				CString str;
				str.Format(_T("Lv.%d %d/%d"), int(player_sesion[x]->nLevel), player_sesion[x]->nExp-int(pow(2.0, double(player_sesion[x]->nLevel-1))),
					int(pow(2.0, double(player_sesion[x]->nLevel))-pow(2.0, double(player_sesion[x]->nLevel-1))));
				double dApt = (double(player_sesion[x]->nExp)-pow(2.0, double(player_sesion[x]->nLevel-1)))
					/(pow(2.0, double(player_sesion[x]->nLevel))-pow(2.0, double(player_sesion[x]->nLevel-1)));
				if(player_sesion[x]->nExp >= 1)
				{
					glBindTexture(GL_TEXTURE_2D, welres->skin[14]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-8.0			  ,  1.0, -50.0);
					glTexCoord2d(dApt, 1.0); glVertex3d(-8.0+dApt*16.0,  1.0, -50.0);
					glTexCoord2d(dApt, 0.0); glVertex3d(-8.0+dApt*16.0, -1.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-8.0			  , -1.0, -50.0);
					glEnd();
				}
				font_Micro2->glDrawText(1.0, -8.0, -1.0, 8.0, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);
				glPopMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0);
			}
			if(nTimeShow >= TIME_TRANSI_SESIONTRAYEXP)
			{
				glPushMatrix();
				glTranslated(x?32.0:-32.0, -24.0, 0.0);
				glScaled(x?-1.0:1.0, 1.0, 1.0);
				glBindTexture(GL_TEXTURE_2D, welres->skin[13]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 8.0,  1.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 8.0, -1.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-8.0, -1.0, -50.0);
				glEnd();
				glPopMatrix();
				glPushMatrix();
				glTranslated(x?32.0:-32.0, -24.0, 0.0);
				CString str;
				str.Format(_T("Lv.%d %d/%d"), int(player_sesion[x]->nLevel), player_sesion[x]->nExp-int(pow(2.0, double(player_sesion[x]->nLevel-1))),
					int(pow(2.0, double(player_sesion[x]->nLevel))-pow(2.0, double(player_sesion[x]->nLevel-1))));
				double dApt = (double(player_sesion[x]->nExp)-pow(2.0, double(player_sesion[x]->nLevel-1)))
					/(pow(2.0, double(player_sesion[x]->nLevel))-pow(2.0, double(player_sesion[x]->nLevel-1)));
				if(player_sesion[x]->nExp >= 1)
				{
					glBindTexture(GL_TEXTURE_2D, welres->skin[14]->GetCurrentTex());
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-8.0			  ,  1.0, -50.0);
					glTexCoord2d(dApt, 1.0); glVertex3d(-8.0+dApt*16.0,  1.0, -50.0);
					glTexCoord2d(dApt, 0.0); glVertex3d(-8.0+dApt*16.0, -1.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-8.0			  , -1.0, -50.0);
					glEnd();
				}
				font_Micro2->glDrawText(1.0, -8.0, -1.0, 8.0, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);
				glPopMatrix();
			}
		}
	}
	//dwTime = SDL_GetTicks() - dwTime;
	//if(dwTime > dwTimeProcess) dwTimeProcess = dwTime;
	/*dwTimeShow += dwTick - dwLastTick;
	if(dwTimeShow >= 1000)
	{
        chProcess.Format(_T("GVFrame: %lu\nAVScale: %lu\nProcess: %lu\nGLSwap:%lu\nTimeInput:%lu\nWasSetAcc: %d\nIsSuccessAcc: %d\nIsXDirect: %d\nPIUIOEv: %d"),
                dwTimeAVFrame, dwTimeAVScale, dwTimeProcess, dwTimeSwap, dwTimeInput, nBeforeStateAcc, nStateAcc, (int)(bIsDirect?1:0), nPIUIOEvent);
        dwTimeAVFrame=0;
        dwTimeAVScale=0;
        dwTimeProcess=0;
        dwTimeInput = 0;
        dwTimeSwap=0;
        dwTimeShow=0;
        nPIUIOEvent = 0;
	}*/
	//font_System->glDrawText(30, -40, -30, 40, -40, 30, -50, 1.0, 5.0, chProcess, DT_TOP|DT_LEFT);

    //dwTime = SDL_GetTicks();
	SDL_GL_SwapBuffers();
	//dwTime = SDL_GetTicks() - dwTime;
	//if(dwTime > dwTimeSwap) dwTimeSwap = dwTime;
	if(nBackInterface != nInterface)	// Cambió la interface
	{
		if(nBackInterface == INT_WELCOME)
		{
			ClearContextWelcome();
			bSaveUsb = false;
			ActualizateContextUsb();
			nInterAfterUsb = nInterface;
			nInterface = INT_USB;
			nGeneralCount = 0;
		}
		else if(nBackInterface == INT_USB)
		{
			nInterface = nInterAfterUsb;
			switch(nInterface)
			{
			case INT_GAME_1: ActualizateContextGame1(); break;
			case INT_GAME_2: ActualizateContextGame2(); break;
			case INT_GAME_3: ActualizateContextGame3(); break;
			case INT_GAME_4: ActualizateContextGame4(); break;
			case INT_GAME_5: ActualizateContextGame5(); break;
			}
		}
		else
		{
			switch(nInterface)
			{
			case INT_WELCOME: nInterface = nBackInterface = INT_GAMEOVER; break;
			}
		}
		/*ClearDraw();
		SDL_GL_SwapBuffers();*/
		nBackInterface = nInterface;
	}
}

int nAse[] = {0,0,0,0,0,0,0,0,0,0};
bool bResetAse = false;

int ProcEvent(SDL_Event keyevent, int ad)
{
	DWORD re = 1;
	int nEvent = 0;
	int nKey = 0;
	if(ad == 1) PollEventThread(&nKey, &nEvent);
    if ( keyevent.type == SDL_KEYDOWN )
    {
		if(keyevent.key.keysym.sym == SDLK_F1)
		{
			if(nInterface != INT_CONFIG) PlusCredit(0);
			nEvent = EV_KEYDOWN;
			nKey = EV_CREDIT1;
		}
		if(keyevent.key.keysym.sym == SDLK_F2)
		{
			if(nInterface != INT_CONFIG) PlusCredit(1);
			nEvent = EV_KEYDOWN;
			nKey = EV_CREDIT2;
		}
		if(keyevent.key.keysym.sym == SDLK_F3)
		{
			if(nInterface != INT_CONFIG) bConfigCh = true;
			nEvent = EV_KEYDOWN;
			nKey = EV_CONFIG;
		}
		if(keyevent.key.keysym.sym == SDLK_F4)
		{
			if(nInterface != INT_CONFIG) PlusCredit(2);
			nEvent = EV_KEYDOWN;
			nKey = EV_SERVICE;
		}
		/*if(keyevent.key.keysym.sym == SDLK_F5)
		{
			bFull = !bFull;
			if(bFull) _printf(_T("Colocando el modo de video a 800x600 FullScreen\n"));
			else _printf(_T("Colocando el modo de video a 800x600 No FullScreen\n"));
			if ( SDL_SetVideoMode(800, 600, 0, SDL_OPENGL|(bFull?SDL_FULLSCREEN:0)) == NULL )
			{
				printf("Unable to create OpenGL screen: %ls\n", SDL_GetError());
				//_getch();
				//SDL_Quit();
				return 0;
			}
			InitGL(800, 600);
		}*/
		for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++)
		{
			if(piuconfig.nWhereButtons[x][y] == -1
				&& piuconfig.nButtons[x][y] == keyevent.key.keysym.sym)
			{
				if(!nAse[x])
				{
					nEvent = EV_KEYDOWN;
					switch(x)
					{
					case 0: nKey = EV_P1_ARROW1; break;
					case 1: nKey = EV_P1_ARROW7; break;
					case 2: nKey = EV_P1_CENTER; break;
					case 3: nKey = EV_P1_ARROW9; break;
					case 4: nKey = EV_P1_ARROW3; break;
					case 5: nKey = EV_P2_ARROW1; break;
					case 6: nKey = EV_P2_ARROW7; break;
					case 7: nKey = EV_P2_CENTER; break;
					case 8: nKey = EV_P2_ARROW9; break;
					case 9: nKey = EV_P2_ARROW3; break;
					}
				}
				nAse[x]++;
			}
		}
		if(piuconfig.nCoinButton1[0] == -1
		&& piuconfig.nCoinButton1[1] == keyevent.key.keysym.sym)
		{
			if(nInterface != INT_CONFIG) PlusCredit(0);
			nEvent = EV_KEYDOWN;
			nKey = EV_CREDIT1;
		}
		if(piuconfig.nCoinButton2[0] == -1
		&& piuconfig.nCoinButton2[1] == keyevent.key.keysym.sym)
		{
			if(nInterface != INT_CONFIG) PlusCredit(1);
			nEvent = EV_KEYDOWN;
			nKey = EV_CREDIT2;
		}
		if(piuconfig.nConfigButton[0] == -1
		&& piuconfig.nConfigButton[1] == keyevent.key.keysym.sym)
		{
			if(nInterface != INT_CONFIG) bConfigCh = true;
			nEvent = EV_KEYDOWN;
			nKey = EV_CONFIG;
		}
		if(piuconfig.nServiceButton[0] == -1
		&& piuconfig.nServiceButton[1] == keyevent.key.keysym.sym)
		{
			if(nInterface != INT_CONFIG) PlusCredit(0);
			nEvent = EV_KEYDOWN;
			nKey = EV_SERVICE;
		}
    }
	if(keyevent.type == SDL_KEYUP)
	{
		if(keyevent.key.keysym.sym == SDLK_F1)
		{
			nEvent = EV_KEYUP;
			nKey = EV_CREDIT1;
		}
		if(keyevent.key.keysym.sym == SDLK_F2)
		{
			nEvent = EV_KEYUP;
			nKey = EV_CREDIT2;
		}
		if(keyevent.key.keysym.sym == SDLK_F3)
		{
			nEvent = EV_KEYUP;
			nKey = EV_CONFIG;
		}
		if(keyevent.key.keysym.sym == SDLK_F4)
		{
			nEvent = EV_KEYUP;
			nKey = EV_SERVICE;
		}
		if(keyevent.key.keysym.sym == SDLK_F10)
		{
			nEvent = EV_KEYUP;
			nKey = 0;
			piuconfig.nMachineOffset=0;
			bShowGeneralOffset = true;
			nTimeGeneralOffset = SDL_GetTicks();
		}
		if(keyevent.key.keysym.sym == SDLK_F11)
		{
			nEvent = EV_KEYUP;
			nKey = 0;
			piuconfig.nMachineOffset-=10;
			bShowGeneralOffset = true;
			nTimeGeneralOffset = SDL_GetTicks();
		}
		if(keyevent.key.keysym.sym == SDLK_F12)
		{
			nEvent = EV_KEYUP;
			nKey = 0;
			piuconfig.nMachineOffset+=10;
			bShowGeneralOffset = true;
			nTimeGeneralOffset = SDL_GetTicks();
		}
		for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++)
		{
			if(piuconfig.nWhereButtons[x][y] == -1
				&& piuconfig.nButtons[x][y] == keyevent.key.keysym.sym)
			{
				if(nAse[x] == 1)
				{
					nEvent = EV_KEYUP;
					switch(x)
					{
					case 0: nKey = EV_P1_ARROW1; break;
					case 1: nKey = EV_P1_ARROW7; break;
					case 2: nKey = EV_P1_CENTER; break;
					case 3: nKey = EV_P1_ARROW9; break;
					case 4: nKey = EV_P1_ARROW3; break;
					case 5: nKey = EV_P2_ARROW1; break;
					case 6: nKey = EV_P2_ARROW7; break;
					case 7: nKey = EV_P2_CENTER; break;
					case 8: nKey = EV_P2_ARROW9; break;
					case 9: nKey = EV_P2_ARROW3; break;
					}
				}
				nAse[x]--;
			}
		}
		if(piuconfig.nCoinButton1[0] == -1
		&& piuconfig.nCoinButton1[1] == keyevent.key.keysym.sym)
		{
			nEvent = EV_KEYUP;
			nKey = EV_CREDIT1;
		}
		if(piuconfig.nCoinButton2[0] == -1
		&& piuconfig.nCoinButton2[1] == keyevent.key.keysym.sym)
		{
			nEvent = EV_KEYUP;
			nKey = EV_CREDIT2;
		}
		if(piuconfig.nConfigButton[0] == -1
		&& piuconfig.nConfigButton[1] == keyevent.key.keysym.sym)
		{
			nEvent = EV_KEYUP;
			nKey = EV_CONFIG;
		}
		if(piuconfig.nServiceButton[0] == -1
		&& piuconfig.nServiceButton[1] == keyevent.key.keysym.sym)
		{
			nEvent = EV_KEYUP;
			nKey = EV_SERVICE;
		}
	}
	/*if ( keyevent.type == SDL_JOYBUTTONDOWN )
    {
		for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++)
		{
			if(piuconfig.nWhereButtons[x][y] == keyevent.jbutton.which
				&& piuconfig.nButtons[x][y] == keyevent.jbutton.button
				&& piuconfig.nModeButtons[x][y] == 0)
			{
				if(!nAse[x])
				{
					nEvent = EV_KEYDOWN;
					switch(x)
					{
					case 0: nKey = EV_P1_ARROW1; break;
					case 1: nKey = EV_P1_ARROW7; break;
					case 2: nKey = EV_P1_CENTER; break;
					case 3: nKey = EV_P1_ARROW9; break;
					case 4: nKey = EV_P1_ARROW3; break;
					case 5: nKey = EV_P2_ARROW1; break;
					case 6: nKey = EV_P2_ARROW7; break;
					case 7: nKey = EV_P2_CENTER; break;
					case 8: nKey = EV_P2_ARROW9; break;
					case 9: nKey = EV_P2_ARROW3; break;
					}
				}
				nAse[x]++;
			}
		}
		if(piuconfig.nCoinButton1[0] == keyevent.jbutton.which
		&& piuconfig.nCoinButton1[1] == keyevent.jbutton.button
		&& piuconfig.nCoinButton1[2] == 0)
		{
			if(nInterface != INT_CONFIG) PlusCredit(0);
			nEvent = EV_KEYDOWN;
			nKey = EV_CREDIT1;
		}
		if(piuconfig.nCoinButton2[0] == keyevent.jbutton.which
		&& piuconfig.nCoinButton2[1] == keyevent.jbutton.button
		&& piuconfig.nCoinButton2[2] == 0)
		{
			if(nInterface != INT_CONFIG) PlusCredit(1);
			nEvent = EV_KEYDOWN;
			nKey = EV_CREDIT2;
		}
		if(piuconfig.nConfigButton[0] == keyevent.jbutton.which
		&& piuconfig.nConfigButton[1] == keyevent.jbutton.button
		&& piuconfig.nConfigButton[2] == 0)
		{
			if(nInterface != INT_CONFIG) bConfigCh = true;
			nEvent = EV_KEYDOWN;
			nKey = EV_CONFIG;
		}
		if(piuconfig.nServiceButton[0] == keyevent.jbutton.which
		&& piuconfig.nServiceButton[1] == keyevent.jbutton.button
		&& piuconfig.nServiceButton[2] == 0)
		{
			if(nInterface != INT_CONFIG) PlusCredit(2);
			nEvent = EV_KEYDOWN;
			nKey = EV_SERVICE;
		}
	}
	if ( keyevent.type == SDL_JOYBUTTONUP )
    {
		for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++)
		{
			if(piuconfig.nWhereButtons[x][y] == keyevent.jbutton.which
				&& piuconfig.nButtons[x][y] == keyevent.jbutton.button
				&& piuconfig.nModeButtons[x][y] == 0)
			{
				if(nAse[x] == 1)
				{
					nEvent = EV_KEYUP;
					switch(x)
					{
					case 0: nKey = EV_P1_ARROW1; break;
					case 1: nKey = EV_P1_ARROW7; break;
					case 2: nKey = EV_P1_CENTER; break;
					case 3: nKey = EV_P1_ARROW9; break;
					case 4: nKey = EV_P1_ARROW3; break;
					case 5: nKey = EV_P2_ARROW1; break;
					case 6: nKey = EV_P2_ARROW7; break;
					case 7: nKey = EV_P2_CENTER; break;
					case 8: nKey = EV_P2_ARROW9; break;
					case 9: nKey = EV_P2_ARROW3; break;
					}
				}
				nAse[x]--;
			}
		}
		if(piuconfig.nCoinButton1[0] == keyevent.jbutton.which
		&& piuconfig.nCoinButton1[1] == keyevent.jbutton.button
		&& piuconfig.nCoinButton1[2] == 0)
		{
			nEvent = EV_KEYUP;
			nKey = EV_CREDIT1;
		}
		if(piuconfig.nCoinButton2[0] == keyevent.jbutton.which
		&& piuconfig.nCoinButton2[1] == keyevent.jbutton.button
		&& piuconfig.nCoinButton2[2] == 0)
		{
			nEvent = EV_KEYUP;
			nKey = EV_CREDIT2;
		}
		if(piuconfig.nConfigButton[0] == keyevent.jbutton.which
		&& piuconfig.nConfigButton[1] == keyevent.jbutton.button
		&& piuconfig.nConfigButton[2] == 0)
		{
			nEvent = EV_KEYUP;
			nKey = EV_CONFIG;
		}
		if(piuconfig.nServiceButton[0] == keyevent.jbutton.which
		&& piuconfig.nServiceButton[1] == keyevent.jbutton.button
		&& piuconfig.nServiceButton[2] == 0)
		{
			nEvent = EV_KEYUP;
			nKey = EV_SERVICE;
		}
	}*/
	/*if ( keyevent.type == SDL_JOYHATMOTION )
    {
		bool bCont = true;
		while(bCont)
		{
			// EV_KEYUP
			int nMode = 0;
			if((nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] & SDL_HAT_UP)
				&& !(keyevent.jhat.value & SDL_HAT_UP))
			{
				nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] &= ~SDL_HAT_UP;
				nEvent = EV_KEYUP;
				nMode = 1;
			}
			else if((nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] & SDL_HAT_LEFT)
				&& !(keyevent.jhat.value & SDL_HAT_LEFT))
			{
				nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] &= ~SDL_HAT_LEFT;
				nEvent = EV_KEYUP;
				nMode = 2;
			}
			else if((nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] & SDL_HAT_DOWN)
				&& !(keyevent.jhat.value & SDL_HAT_DOWN))
			{
				nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] &= ~SDL_HAT_DOWN;
				nEvent = EV_KEYUP;
				nMode = 3;
			}
			else if((nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] & SDL_HAT_RIGHT)
				&& !(keyevent.jhat.value & SDL_HAT_RIGHT))
			{
				nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] &= ~SDL_HAT_RIGHT;
				nEvent = EV_KEYUP;
				nMode = 4;
			}
			// ev_keydown
			else if(!(nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] & SDL_HAT_UP)
				&& (keyevent.jhat.value & SDL_HAT_UP))
			{
				nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] |= SDL_HAT_UP;
				nEvent = EV_KEYDOWN;
				nMode = 1;
			}
			else if(!(nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] & SDL_HAT_LEFT)
				&& (keyevent.jhat.value & SDL_HAT_LEFT))
			{
				nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] |= SDL_HAT_LEFT;
				nEvent = EV_KEYDOWN;
				nMode = 2;
			}
			else if(!(nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] & SDL_HAT_DOWN)
				&& (keyevent.jhat.value & SDL_HAT_DOWN))
			{
				nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] |= SDL_HAT_DOWN;
				nEvent = EV_KEYDOWN;
				nMode = 3;
			}
			else if(!(nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] & SDL_HAT_RIGHT)
				&& (keyevent.jhat.value & SDL_HAT_RIGHT))
			{
				nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] |= SDL_HAT_RIGHT;
				nEvent = EV_KEYDOWN;
				nMode = 4;
			}
			if(nValueJoystickHat[keyevent.jhat.which][keyevent.jhat.hat] == keyevent.jhat.value) bCont = false;

			if(piuconfig.nCoinButton1[0] == keyevent.jhat.which
			&& piuconfig.nCoinButton1[1] == keyevent.jhat.hat
			&& piuconfig.nCoinButton1[2] == nMode)
			{
				if(nInterface != INT_CONFIG && nEvent == EV_KEYDOWN) PlusCredit(0);
				nKey = EV_CREDIT1;
			}
			if(piuconfig.nCoinButton2[0] == keyevent.jhat.which
			&& piuconfig.nCoinButton2[1] == keyevent.jhat.hat
			&& piuconfig.nCoinButton2[2] == nMode)
			{
				if(nInterface != INT_CONFIG && nEvent == EV_KEYDOWN) PlusCredit(1);
				nKey = EV_CREDIT2;
			}
			if(piuconfig.nConfigButton[0] == keyevent.jhat.which
			&& piuconfig.nConfigButton[1] == keyevent.jhat.hat
			&& piuconfig.nConfigButton[2] == nMode)
			{
				if(nInterface != INT_CONFIG && nEvent == EV_KEYDOWN) bConfigCh = true;
				nKey = EV_CONFIG;
			}
			if(piuconfig.nServiceButton[0] == keyevent.jhat.which
			&& piuconfig.nServiceButton[1] == keyevent.jhat.hat
			&& piuconfig.nServiceButton[2] == nMode)
			{
				if(nInterface != INT_CONFIG && nEvent == EV_KEYDOWN) PlusCredit(2);
				nKey = EV_SERVICE;
			}
			for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++)
			{
				if(piuconfig.nWhereButtons[x][y] == keyevent.jhat.which
					&& piuconfig.nButtons[x][y] == keyevent.jhat.hat
					&& piuconfig.nModeButtons[x][y] == nMode)
				{
					if(nEvent == EV_KEYUP)
					{
						if(nAse[x] == 1)
						{
							nEvent = EV_KEYUP;
							switch(x)
							{
							case 0: nKey = EV_P1_ARROW1; break;
							case 1: nKey = EV_P1_ARROW7; break;
							case 2: nKey = EV_P1_CENTER; break;
							case 3: nKey = EV_P1_ARROW9; break;
							case 4: nKey = EV_P1_ARROW3; break;
							case 5: nKey = EV_P2_ARROW1; break;
							case 6: nKey = EV_P2_ARROW7; break;
							case 7: nKey = EV_P2_CENTER; break;
							case 8: nKey = EV_P2_ARROW9; break;
							case 9: nKey = EV_P2_ARROW3; break;
							}
						}
						else nEvent = 0;
						nAse[x]--;
					}
					if(nEvent == EV_KEYDOWN)
					{
						if(!nAse[x])
						{
							nEvent = EV_KEYDOWN;
							switch(x)
							{
							case 0: nKey = EV_P1_ARROW1; break;
							case 1: nKey = EV_P1_ARROW7; break;
							case 2: nKey = EV_P1_CENTER; break;
							case 3: nKey = EV_P1_ARROW9; break;
							case 4: nKey = EV_P1_ARROW3; break;
							case 5: nKey = EV_P2_ARROW1; break;
							case 6: nKey = EV_P2_ARROW7; break;
							case 7: nKey = EV_P2_CENTER; break;
							case 8: nKey = EV_P2_ARROW9; break;
							case 9: nKey = EV_P2_ARROW3; break;
							}
						}
						else nEvent = 0;
						nAse[x]++;
					}
				}
			}
		}
	}*/
	if(nInterface == INT_WELCOME) EventWelcome(nEvent, nKey);
	if(nInterface == INT_USB) EventUsb(nEvent, nKey);
	if(nInterface == INT_GAME_1) EventGame1(nEvent, nKey);
	if(nInterface == INT_GAME_2) EventGame2(nEvent, nKey);
	if(nInterface == INT_GAME_3) EventGame3(nEvent, nKey);
	if(nInterface == INT_GAME_4) EventGame4(nEvent, nKey);
	if(nInterface == INT_GAME_5) EventGame5(nEvent, nKey);
	if(nInterface == INT_CONFIG)
	{
		bResetAse = true;
		EventConfig(nEvent, nKey);
		EventConfigEx(keyevent);
	}
	if(nInterface != INT_CONFIG && bResetAse)
	{
		for(int x = 0; x < 10; x++) nAse[x] = 0;
		bResetAse = false;
	}
	return re;
}

int EventThread(void* data)
{
	SDL_mutex *lock = (SDL_mutex *)data;
	while(!done)
	{
		int nMode = 0;
		int nKey = 0;
		int nEvent = 0;
		SDL_JoystickUpdate();
		for(int i = 0; i < nJoystick; i++)
		{
			for(int j = 0; j < nJoystickHat[i]; j++)
			{
				bool bCont = true;
				while(bCont)
				{
					nEvent = 0;
					Uint8 value = SDL_JoystickGetHat(joystick[i], j);
					if((nValueJoystickHat[i][j] & SDL_HAT_UP)
						&& !(value & SDL_HAT_UP))
					{
						nValueJoystickHat[i][j] &= ~SDL_HAT_UP;
						nEvent = EV_KEYUP;
						nMode = 1;
					}
					else if((nValueJoystickHat[i][j] & SDL_HAT_LEFT)
						&& !(value & SDL_HAT_LEFT))
					{
						nValueJoystickHat[i][j] &= ~SDL_HAT_LEFT;
						nEvent = EV_KEYUP;
						nMode = 2;
					}
					else if((nValueJoystickHat[i][j] & SDL_HAT_DOWN)
						&& !(value & SDL_HAT_DOWN))
					{
						nValueJoystickHat[i][j] &= ~SDL_HAT_DOWN;
						nEvent = EV_KEYUP;
						nMode = 3;
					}
					else if((nValueJoystickHat[i][j] & SDL_HAT_RIGHT)
						&& !(value & SDL_HAT_RIGHT))
					{
						nValueJoystickHat[i][j] &= ~SDL_HAT_RIGHT;
						nEvent = EV_KEYUP;
						nMode = 4;
					}
					// ev_keydown
					else if(!(nValueJoystickHat[i][j] & SDL_HAT_UP)
						&& (value & SDL_HAT_UP))
					{
						nValueJoystickHat[i][j] |= SDL_HAT_UP;
						nEvent = EV_KEYDOWN;
						nMode = 1;
					}
					else if(!(nValueJoystickHat[i][j] & SDL_HAT_LEFT)
						&& (value & SDL_HAT_LEFT))
					{
						nValueJoystickHat[i][j] |= SDL_HAT_LEFT;
						nEvent = EV_KEYDOWN;
						nMode = 2;
					}
					else if(!(nValueJoystickHat[i][j] & SDL_HAT_DOWN)
						&& (value & SDL_HAT_DOWN))
					{
						nValueJoystickHat[i][j] |= SDL_HAT_DOWN;
						nEvent = EV_KEYDOWN;
						nMode = 3;
					}
					else if(!(nValueJoystickHat[i][j] & SDL_HAT_RIGHT)
						&& (value & SDL_HAT_RIGHT))
					{
						nValueJoystickHat[i][j] |= SDL_HAT_RIGHT;
						nEvent = EV_KEYDOWN;
						nMode = 4;
					}
					if(nValueJoystickHat[i][j] == value) bCont = false;

					if(nEvent)
					{
						if(piuconfig.nCoinButton1[0] == i
						&& piuconfig.nCoinButton1[1] == j
						&& piuconfig.nCoinButton1[2] == nMode)
						{
							if(nInterface != INT_CONFIG && nEvent == EV_KEYDOWN) PlusCredit(0);
							nKey = EV_CREDIT1;
						}
						if(piuconfig.nCoinButton2[0] == i
						&& piuconfig.nCoinButton2[1] == j
						&& piuconfig.nCoinButton2[2] == nMode)
						{
							if(nInterface != INT_CONFIG && nEvent == EV_KEYDOWN) PlusCredit(1);
							nKey = EV_CREDIT2;
						}
						if(piuconfig.nConfigButton[0] == i
						&& piuconfig.nConfigButton[1] == j
						&& piuconfig.nConfigButton[2] == nMode)
						{
							if(nInterface != INT_CONFIG && nEvent == EV_KEYDOWN) bConfigCh = true;
							nKey = EV_CONFIG;
						}
						if(piuconfig.nServiceButton[0] == i
						&& piuconfig.nServiceButton[1] == j
						&& piuconfig.nServiceButton[2] == nMode)
						{
							if(nInterface != INT_CONFIG && nEvent == EV_KEYDOWN) PlusCredit(2);
							nKey = EV_SERVICE;
						}
						for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++)
						{
							if(piuconfig.nWhereButtons[x][y] == i
								&& piuconfig.nButtons[x][y] == j
								&& piuconfig.nModeButtons[x][y] == nMode)
							{
								if(nEvent == EV_KEYUP)
								{
									if(nAse[x] == 1)
									{
										nEvent = EV_KEYUP;
										switch(x)
										{
										case 0: nKey = EV_P1_ARROW1; break;
										case 1: nKey = EV_P1_ARROW7; break;
										case 2: nKey = EV_P1_CENTER; break;
										case 3: nKey = EV_P1_ARROW9; break;
										case 4: nKey = EV_P1_ARROW3; break;
										case 5: nKey = EV_P2_ARROW1; break;
										case 6: nKey = EV_P2_ARROW7; break;
										case 7: nKey = EV_P2_CENTER; break;
										case 8: nKey = EV_P2_ARROW9; break;
										case 9: nKey = EV_P2_ARROW3; break;
										}
									}
									else nEvent = 0;
									nAse[x]--;
								}
								if(nEvent == EV_KEYDOWN)
								{
									if(!nAse[x])
									{
										nEvent = EV_KEYDOWN;
										switch(x)
										{
										case 0: nKey = EV_P1_ARROW1; break;
										case 1: nKey = EV_P1_ARROW7; break;
										case 2: nKey = EV_P1_CENTER; break;
										case 3: nKey = EV_P1_ARROW9; break;
										case 4: nKey = EV_P1_ARROW3; break;
										case 5: nKey = EV_P2_ARROW1; break;
										case 6: nKey = EV_P2_ARROW7; break;
										case 7: nKey = EV_P2_CENTER; break;
										case 8: nKey = EV_P2_ARROW9; break;
										case 9: nKey = EV_P2_ARROW3; break;
										}
									}
									else nEvent = 0;
									nAse[x]++;
								}
							}
						}
						SDL_mutexP(lock);
						nSizeEvent++;
						nKeyEvent=(int*)realloc(nKeyEvent, nSizeEvent*sizeof(int));
						nEventEvent=(int*)realloc(nEventEvent, nSizeEvent*sizeof(int));
						nKeyEvent[nSizeEvent-1] = nKey;
						nEventEvent[nSizeEvent-1] = nEvent;
						SDL_mutexV(lock);
					}
				}
			}
			for(int j = 0; j < nJoystickButtons[i]; j++)
			{
				nEvent = 0;
				Uint8 value = SDL_JoystickGetButton(joystick[i], j);
				if(value != nValueJoystickButtons[i][j])
				{
					nValueJoystickButtons[i][j] = value;
					nEvent = value?EV_KEYDOWN:EV_KEYUP;
				}
				if(nEvent)
				{
					for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++)
					{
						if(piuconfig.nWhereButtons[x][y] == i
							&& piuconfig.nButtons[x][y] == j
							&& piuconfig.nModeButtons[x][y] == 0)
						{
							if(nEvent == EV_KEYUP)
							{
								if(nAse[x] == 1)
								{
									nEvent = EV_KEYUP;
									switch(x)
									{
									case 0: nKey = EV_P1_ARROW1; break;
									case 1: nKey = EV_P1_ARROW7; break;
									case 2: nKey = EV_P1_CENTER; break;
									case 3: nKey = EV_P1_ARROW9; break;
									case 4: nKey = EV_P1_ARROW3; break;
									case 5: nKey = EV_P2_ARROW1; break;
									case 6: nKey = EV_P2_ARROW7; break;
									case 7: nKey = EV_P2_CENTER; break;
									case 8: nKey = EV_P2_ARROW9; break;
									case 9: nKey = EV_P2_ARROW3; break;
									}
								}
								else nEvent = 0;
								nAse[x]--;
							}
							if(nEvent == EV_KEYDOWN)
							{
								if(!nAse[x])
								{
									nEvent = EV_KEYDOWN;
									switch(x)
									{
									case 0: nKey = EV_P1_ARROW1; break;
									case 1: nKey = EV_P1_ARROW7; break;
									case 2: nKey = EV_P1_CENTER; break;
									case 3: nKey = EV_P1_ARROW9; break;
									case 4: nKey = EV_P1_ARROW3; break;
									case 5: nKey = EV_P2_ARROW1; break;
									case 6: nKey = EV_P2_ARROW7; break;
									case 7: nKey = EV_P2_CENTER; break;
									case 8: nKey = EV_P2_ARROW9; break;
									case 9: nKey = EV_P2_ARROW3; break;
									}
								}
								else nEvent = 0;
								nAse[x]++;
							}
						}
					}
					if(piuconfig.nCoinButton1[0] == i
					&& piuconfig.nCoinButton1[1] == j
					&& piuconfig.nCoinButton1[2] == 0)
					{
						if(nInterface != INT_CONFIG && nEvent != EV_KEYUP) PlusCredit(0);
						//nEvent = EV_KEYDOWN;
						nKey = EV_CREDIT1;
					}
					if(piuconfig.nCoinButton2[0] == i
					&& piuconfig.nCoinButton2[1] == j
					&& piuconfig.nCoinButton2[2] == 0)
					{
						if(nInterface != INT_CONFIG && nEvent != EV_KEYUP) PlusCredit(1);
						//nEvent = EV_KEYDOWN;
						nKey = EV_CREDIT2;
					}
					if(piuconfig.nConfigButton[0] == i
					&& piuconfig.nConfigButton[1] == j
					&& piuconfig.nConfigButton[2] == 0)
					{
						if(nInterface != INT_CONFIG) bConfigCh = true;
						//nEvent = EV_KEYDOWN;
						nKey = EV_CONFIG;
					}
					if(piuconfig.nServiceButton[0] == i
					&& piuconfig.nServiceButton[1] == j
					&& piuconfig.nServiceButton[2] == 0)
					{
						if(nInterface != INT_CONFIG && nEvent != EV_KEYUP) PlusCredit(2);
						//nEvent = EV_KEYDOWN;
						nKey = EV_SERVICE;
					}
					SDL_mutexP(lock);
                    nSizeEvent++;
                    nKeyEvent=(int*)realloc(nKeyEvent, nSizeEvent*sizeof(int));
                    nEventEvent=(int*)realloc(nEventEvent, nSizeEvent*sizeof(int));
                    nKeyEvent[nSizeEvent-1] = nKey;
                    nEventEvent[nSizeEvent-1] = nEvent;
                    SDL_mutexV(lock);
				}
			}
		}
        if(g_ihPIUIO->m_bFoundDevice)
        {
            g_ihPIUIO->InputThreadMain();
            if(g_ihPIUIO->m_iChanged)
            {
                for( unsigned iBtn = 0; iBtn < 32; ++iBtn )
                {
                    nKey = 0;
                    nEvent = 0;
                    // if this button's status has changed, report it.
                    if( (!IsBitSet(g_ihPIUIO->m_iChanged, iBtn)) )
                        continue;
                    nEvent = IsBitSet(g_ihPIUIO->m_iInputField, iBtn)?EV_KEYDOWN:EV_KEYUP;
                    unsigned iBtnA = 31 - iBtn;
                    switch(iBtnA)
                    {
                        case 10: nKey = EV_CREDIT1; if(nInterface != INT_CONFIG && nEvent != EV_KEYUP) PlusCredit(0); break;
                        case 26: nKey = EV_CREDIT2; if(nInterface != INT_CONFIG && nEvent != EV_KEYUP) PlusCredit(1); break;
                        case 9: nKey = EV_CONFIG; if(nInterface != INT_CONFIG && nEvent != EV_KEYUP) bConfigCh = true; break;
                        case 14: nKey = EV_SERVICE; if(nInterface != INT_CONFIG && nEvent != EV_KEYUP) PlusCredit(2); break;
                        case 0: nKey = EV_P1_ARROW7; break;
                        case 1: nKey = EV_P1_ARROW9; break;
                        case 2: nKey = EV_P1_CENTER; break;
                        case 3: nKey = EV_P1_ARROW1; break;
                        case 4: nKey = EV_P1_ARROW3; break;
                        case 16: nKey = EV_P2_ARROW7; break;
                        case 17: nKey = EV_P2_ARROW9; break;
                        case 18: nKey = EV_P2_CENTER; break;
                        case 19: nKey = EV_P2_ARROW1; break;
                        case 20: nKey = EV_P2_ARROW3; break;
                        default: nKey = 0; break;
                    }
                    if(nKey != 0 && nEvent != 0)
                    {
                        const int BitBy[] = { 2,  3,  4,  5,  6, -1, -1, -1,
                                             -1, -1, -1, -1, -1, -1, -1, -1,
                                             18, 19, 20, 21, 22, -1, -1, -1,
                                             -1, -1, -1, -1, -1, -1, -1, -1};
                        if(BitBy[iBtnA] != -1)
                            portLights->Write(BitBy[iBtnA], IsBitSet(g_ihPIUIO->m_iInputField, iBtn)?1:0);
                        SDL_mutexP(lock);
						nSizeEvent++;
						nKeyEvent=(int*)realloc(nKeyEvent, nSizeEvent*sizeof(int));
						nEventEvent=(int*)realloc(nEventEvent, nSizeEvent*sizeof(int));
						nKeyEvent[nSizeEvent-1] = nKey;
						nEventEvent[nSizeEvent-1] = nEvent;
						//nPIUIOEvent++;
						SDL_mutexV(lock);
                    }
                }
            }
        }
	}
	return 0;
}

int PollEventThread(int* nKey, int* nEvent)
{
	if(nSizeEvent <= 0)
	{
		(*nKey) = 0;
		(*nEvent) = 0;
		nSizeEvent = 0;
		return 0;
	}
	else
	{
		(*nKey) = nKeyEvent[0];
		(*nEvent) = nEventEvent[0];
		for(int x = 0; x < (nSizeEvent-1); x++)
		{
			nKeyEvent[x] = nKeyEvent[x+1];
			nEventEvent[x] = nEventEvent[x+1];
		}
		nSizeEvent--;
		nKeyEvent=(int*)realloc(nKeyEvent, nSizeEvent*sizeof(int));
		nEventEvent=(int*)realloc(nEventEvent, nSizeEvent*sizeof(int));
		int ret = 0;
		if(nSizeEvent <= 0) ret = 0;
		else ret = 1;
		return ret;
	}
	return 0;
}
