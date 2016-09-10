#include "piu.h"

bool bFull = false;
int g_nGestor = 1;			// The global gestor
int nResWidth = 800;
int nResHeight = 600;
DWORD PIUFPS = 60;

DWORD dwWait = 0;

int done = 0;

CMainTheme* g_mtTheme;

// ** optional
CFont2* fmtArial;
CMaxInterface *inter;
////////////////////

#ifdef _WIN32
#ifdef _DEBUG
int _tmain(int argc, _TCHAR* argv[])
#else
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR tComm, int nCmd)
#endif
#elif defined(UNIX) || defined(__linux__)
int main(int argc, char* argv[])
#else
int _tmain(int argc, _TCHAR* argv[])
#endif
{
	CString res_path = _T("");
	#if (defined(UNIX) || defined(__linux__))
	//res_path = _T("../PIU2/");
	#endif
	SetCurDir();
	SetCurMethod(_T("piumain"), _T("init"));
	// ** Inicializacion para video
	av_register_all();
	///////////////////////////////

	// ** Inicializacion para modo
	nResWidth = 800; nResHeight = 600;
#ifdef _XBOX	// XBOX ONLY RUNS 640 x 480
	nResWidth = 640;
	nResHeight = 480;
#else
	if(!ReadModeFromACC(_T("Mode.acc")))
	{
		_printf(_T("(WARNING): piumain: cannot read the 'Mode.acc' file, program will continue at 800x600 with 60 FPS"));
	}

#endif // XBOX
	//////////////////////////////

	// ** Inicialización para el puntero global de luces

	if(!ReadLightsConfigFromACC(_T("plights.acc")))
	{
		_printf(_T("(WARNING): piumain: cannot read the 'plights.acc' file, program will continue at 0x378 standard LPT out"));
	}

	////////////////////////////////////////////////////

	_printf(_T("Inicializando el sistema de video\n"));

	g_gmMain = new CGraphMain();
	g_gmMain->Set(g_nGestor);

    if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0 )
	{
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	g_gmMain = new CGraphMain();
	g_gmMain->Set(1);

	//bool bFull = m_bFull;
	if(bFull) _printf(_T("Colocando el modo de video a %dx%d FullScreen\n"), nResWidth, nResHeight);
	else _printf(_T("Colocando el modo de video a %dx%d No FullScreen\n"), nResWidth, nResHeight);

	g_gmMain->Initialize(nResWidth, nResHeight, 0, bFull);
	g_gmMain->Clear(0.0, 0.0, 0.0, 1.0);
	g_gmMain->SwapBuffers();

	g_gtNoTex = new CGraphTexture();

	_printf(_T("Inicializando Sistema de sonido\n"));
	g_ssMain = new CSoundSys();
	if(!g_ssMain->Initialize()) return 0;

	for(int x = 0; x < MAX_CHANNELS; x++)
	{
		channels[x] = new CSoundChannel();
		g_ssMain->GetChannel(x, channels[x]);
	}

	_printf(_T("Inicializando Motor de entrada\n"));
	if(!CStdInput::LaunchInput())
	{
		printf("Unable to create Input \n");
        SDL_Quit();
        return 3;
	}

	_printf(_T("Inicializando Recursos\n"));

	// ** CÓDIGO DE PRUEBA PARA INTERFACES
	inter = new CMaxInterface();
	inter->Init(_T("testinterface/test.acc"));

	g_gmMain->Frustum(-40.0,40.0,-30.0,30.0,50.0,100.0);

	fmtArial = new CFont2();
	fmtArial->LoadFont(res_path + _T("Arial.fnt"));

	// Input init example
	memset(&piuconfig, 0, sizeof(PIUCONFIG));
	for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++) piuconfig.nWhereButtons[x][y] = -2;

	// Asignation
	for(int x = 0; x < 10; x++)
	{
		piuconfig.nWhereButtons[x][0] = -1; // All first set to keyboard
		piuconfig.nWhereButtons[x][1] = -1; // All first set to keyboard
	}

	piuconfig.nButtons[0][0] = SDLK_z; // DL 1
	piuconfig.nButtons[1][0] = SDLK_q; // UL 1
	piuconfig.nButtons[2][0] = SDLK_s; // CE 1
	piuconfig.nButtons[3][0] = SDLK_e; // UR 1
	piuconfig.nButtons[4][0] = SDLK_c; // DL 1
	piuconfig.nButtons[5][0] = SDLK_v; // DL 1
	piuconfig.nButtons[6][0] = SDLK_r; // UL 1
	piuconfig.nButtons[7][0] = SDLK_g; // CE 1
	piuconfig.nButtons[8][0] = SDLK_y; // UR 1
	piuconfig.nButtons[9][0] = SDLK_n; // DL 1

	piuconfig.nButtons[0][1] = SDLK_KP_1; // DL 2
	piuconfig.nButtons[1][1] = SDLK_KP_7; // UL 2
	piuconfig.nButtons[2][1] = SDLK_KP_5; // CE 2
	piuconfig.nButtons[3][1] = SDLK_KP_9; // UR 2
	piuconfig.nButtons[4][1] = SDLK_KP_3; // DL 2
	piuconfig.nButtons[5][1] = SDLK_KP_1; // DL 2
	piuconfig.nButtons[6][1] = SDLK_KP_7; // UL 2
	piuconfig.nButtons[7][1] = SDLK_KP_5; // CE 2
	piuconfig.nButtons[8][1] = SDLK_KP_9; // UR 2
	piuconfig.nButtons[9][1] = SDLK_KP_3; // DL 2
	//////////////////////////////////////////////////////////

	_printf(_T("Hecho! Disfrute PIU MaX\n"));
	dwTick = dwLastTick = SDL_GetTicks();
#define PIUFPSINACTIVE 1
	DWORD dwTimePerFrame = 1000/PIUFPS;
    while ( ! done )
    {
		/*Uint8 st=SDL_GetAppState();
		if((st==SDL_APPACTIVE) || (! st)) dwTimePerFrame = 1000/PIUFPSINACTIVE;
		else dwTimePerFrame = 1000/PIUFPS;*/
		dwLastTick = dwTick;
		dwTick = SDL_GetTicks();
		DWORD dwAdit = dwTick - dwLastTick;
		__int64 nWait = __int64(dwTimePerFrame)-__int64(dwAdit);
		dwWait = (DWORD)max(0, nWait);
		if(dwWait != 0) SDL_Delay((DWORD)dwWait);
		dwTick = SDL_GetTicks();
		dwWait = dwTick - dwLastTick;

		//printf("Delta: %lu\n", dwWait);

		int nKey, nEvent;
		while(CStdInput::PollEvent(&nKey, &nEvent))
		{
			if(nKey == 0 && nEvent == 0) continue;
			if(!ProcEvent(nKey, nEvent)) done = 1;
		}

		DrawInterface();
    }

    delete fmtArial;

	g_ssMain->Destroy();
	g_gmMain->Destroy();

	delete g_ssMain;
	delete g_gmMain;

    SDL_Quit();

    return 0;
}

int ReadModeFromACC(LPCTSTR strFile)
{
	FILE* hFileMode = _tfopen(strFile, _T("r+"));
	if(hFileMode != NULL)
	{
		if(_fscanf(hFileMode, _T("%d\n"), &nResWidth) != 1)
		{
			_printf(_T("ERROR: mal formato de mode.acc\n"));
			return 0;
		}
		if(_fscanf(hFileMode, _T("%d\n"), &nResHeight) != 1)
		{
			_printf(_T("ERROR: mal formato de mode.acc\n"));
			return 0;
		}
		int fps = 60;
		if(_fscanf(hFileMode, _T("%d\n"), &fps) != 1)
		{
			fps = 60;
			_printf(_T("ERROR: mal formato de mode.acc [FPS]\n"));
		}
		PIUFPS = (DWORD)fps;
		if(_fscanf(hFileMode, _T("%d\n"), &g_nGestor) != 1)
		{
			g_nGestor = 1;
//			_printf("ERROR: mal formato de mode.acc [Gestor], se colocará OpenGL\n");
		}
		fclose(hFileMode);
	}
	return 1;
}

int ReadLightsConfigFromACC(LPCTSTR strFile)
{
	int nPortID = 0x0;
	FILE* hFileMode = _tfopen(strFile, _T("r+"));
	if(hFileMode != NULL)
	{
		if(_fscanf(hFileMode, _T("%x\n"), &nPortID) != 1)
		{
			_printf(_T("ERROR: mal formato de ")STROP _T("\n"), strFile);
		}
		fclose(hFileMode);
	}
	//if(nPortID != 0x0)
	//{
	//	if(!portLights->digitalio(nPortID)) return 10;
		//portLights->WriteVal(0xFFFF);
	//}
	return 1;
}

void DrawInterface()
{
	g_gmMain->Clear(0.0, 0.0, 0.0, 1.0);

	// DRAWING CODE HERE

	if(!g_gmMain->BeginScene()) return;

	inter->Update(dwWait);
	inter->Draw();

    g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
	fmtArial->glDrawText(30.0, -40.0, -30.0, 40.0, 0.0, 0.0, -50.0, 1.0, 8.0,
		_T("Negros y judios"), DT_CENTER|DT_VCENTER);

	g_gmMain->EndScene();
	g_gmMain->SwapBuffers();
}

int ProcEvent(int nKey, int nEvent)
{
	if(nKey == KEY_QUIT) return 0;
	inter->Event(MID(nEvent, nKey));
	return 1;
}
