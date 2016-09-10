
#include "StdInput.h"

int CStdInput::m_nDone;

// For input
int CStdInput::m_nJoystick;
SDL_Joystick** CStdInput::m_joystick;
int* CStdInput::m_nJoystickHat;
int** CStdInput::m_nValueJoystickHat;
int* CStdInput::m_nJoystickButtons;
int** CStdInput::m_nValueJoystickButtons;

// For events
int CStdInput::m_nSizeEvent;
int* CStdInput::m_nKeyEvent;
int* CStdInput::m_nEventEvent;

// For data access
// No time data (no __int64 nTimes[10])
// This value must be asigned in each interface
int CStdInput::m_nArrows[10];
int CStdInput::m_nExtButtons[4];	// 0 = Coin1, 1 = Coin2, 3 = service, 4 = test
int CStdInput::m_nChoice[12];		// left1p, right1p, up1p, down1p, fire1p, fire1p, left2p, right2p, up2p, down2p, fire2p, fire2p

// For thread
SDL_Thread *CStdInput::m_eventThread;
SDL_mutex *CStdInput::m_lockEvent;

CStdInput::CStdInput(void)
{
	m_nDone = 0;
	m_nJoystick = 0;
}

CStdInput::~CStdInput(void)
{
	StopInput();
}

int CStdInput::LaunchInput(void)
{
	m_nDone = 0;
	// ** The number of Joysticks
	m_nJoystick = SDL_NumJoysticks();

	if(m_nJoystick != 0)
	{
		m_joystick = new SDL_Joystick*[m_nJoystick];
		m_nJoystickHat = new int[m_nJoystick];
		m_nJoystickButtons = new int[m_nJoystick];
		m_nValueJoystickHat = new int*[m_nJoystick];
		m_nValueJoystickButtons = new int*[m_nJoystick];
		if(m_joystick == NULL) return 0;
		if(m_nJoystickHat == NULL) return 0;
		if(m_nJoystickButtons == NULL) return 0;
		if(m_nValueJoystickHat == NULL) return 0;
		if(m_nValueJoystickButtons == NULL) return 0;

		// Now, we'll open each m_joystick
		// And, for data m_joystick, we'll asign default
		for(int x = 0; x < m_nJoystick; x++)
		{
			m_joystick[x] = SDL_JoystickOpen(x);
			m_nJoystickHat[x] = SDL_JoystickNumHats(m_joystick[x]);
			m_nJoystickButtons[x] = SDL_JoystickNumButtons(m_joystick[x]);
			m_nValueJoystickHat[x] = new int[m_nJoystickHat[x]];
			m_nValueJoystickButtons[x] = new int[m_nJoystickButtons[x]];
			for(int y = 0; y < m_nJoystickHat[x]; y++)
				m_nValueJoystickHat[x][y] = 0x00;
			for(int y = 0; y < m_nJoystickButtons[x]; y++)
				m_nValueJoystickButtons[x][y] = 0;
		}
		for(int i = 0; i < 10; i++) m_nArrows[i] = 0;
		for(int i = 0; i < 4; i++) m_nExtButtons[i] = 0;
		for(int i = 0; i < 12; i++) m_nChoice[i] = 0;
		m_nSizeEvent = 0;
		m_nKeyEvent=(int*)calloc(0, sizeof(int));
		m_nEventEvent=(int*)calloc(0, sizeof(int));

		// Crear el hilo para los eventos
		m_lockEvent = SDL_CreateMutex();
		m_eventThread = SDL_CreateThread(EventThread, NULL, m_lockEvent);
		if(m_eventThread == NULL)
		{
			_printf(_T("(ERROR): Hilo de eventos: no se pudo crear\n"));
			return 0;
		}
	}
	return 1;
}

void CStdInput::StopInput(void)
{
	m_nDone = 1;
	SDL_WaitThread(m_eventThread, NULL);
	SDL_DestroyMutex(m_lockEvent);
	if(m_nJoystick != 0)
	{
		for(int x = 0; x < m_nJoystick; x++)
		{
			SDL_JoystickClose(m_joystick[x]);
		}
		delete[] m_joystick;
		delete m_nJoystickHat;
		delete[] m_nValueJoystickHat;
		delete m_nJoystickButtons;
		delete[] m_nValueJoystickButtons;
	}
}

int CStdInput::EventThread(void* data)
{
	SDL_mutex *lock = (SDL_mutex *)data;
	while(!m_nDone)
	{
		SDL_mutexP(lock);
		int nMode = 0;
		int nKey = 0;
		int nEvent = 0;
		SDL_JoystickUpdate();
		for(int i = 0; i < m_nJoystick; i++)
		{
			for(int j = 0; j < m_nJoystickHat[i]; j++)
			{
				bool bCont = true;
				while(bCont)
				{
					nEvent = 0;
					Uint8 value = SDL_JoystickGetHat(m_joystick[i], j);
					if((m_nValueJoystickHat[i][j] & SDL_HAT_UP)
						&& !(value & SDL_HAT_UP))
					{
						m_nValueJoystickHat[i][j] &= ~SDL_HAT_UP;
						nEvent = EV_KEYUP;
						nMode = 1;
					}
					else if((m_nValueJoystickHat[i][j] & SDL_HAT_LEFT)
						&& !(value & SDL_HAT_LEFT))
					{
						m_nValueJoystickHat[i][j] &= ~SDL_HAT_LEFT;
						nEvent = EV_KEYUP;
						nMode = 2;
					}
					else if((m_nValueJoystickHat[i][j] & SDL_HAT_DOWN)
						&& !(value & SDL_HAT_DOWN))
					{
						m_nValueJoystickHat[i][j] &= ~SDL_HAT_DOWN;
						nEvent = EV_KEYUP;
						nMode = 3;
					}
					else if((m_nValueJoystickHat[i][j] & SDL_HAT_RIGHT)
						&& !(value & SDL_HAT_RIGHT))
					{
						m_nValueJoystickHat[i][j] &= ~SDL_HAT_RIGHT;
						nEvent = EV_KEYUP;
						nMode = 4;
					}
					// ev_keydown
					else if(!(m_nValueJoystickHat[i][j] & SDL_HAT_UP)
						&& (value & SDL_HAT_UP))
					{
						m_nValueJoystickHat[i][j] |= SDL_HAT_UP;
						nEvent = EV_KEYDOWN;
						nMode = 1;
					}
					else if(!(m_nValueJoystickHat[i][j] & SDL_HAT_LEFT)
						&& (value & SDL_HAT_LEFT))
					{
						m_nValueJoystickHat[i][j] |= SDL_HAT_LEFT;
						nEvent = EV_KEYDOWN;
						nMode = 2;
					}
					else if(!(m_nValueJoystickHat[i][j] & SDL_HAT_DOWN)
						&& (value & SDL_HAT_DOWN))
					{
						m_nValueJoystickHat[i][j] |= SDL_HAT_DOWN;
						nEvent = EV_KEYDOWN;
						nMode = 3;
					}
					else if(!(m_nValueJoystickHat[i][j] & SDL_HAT_RIGHT)
						&& (value & SDL_HAT_RIGHT))
					{
						m_nValueJoystickHat[i][j] |= SDL_HAT_RIGHT;
						nEvent = EV_KEYDOWN;
						nMode = 4;
					}
					if(m_nValueJoystickHat[i][j] == value) bCont = false;

					if(nEvent)
					{
						if(piuconfig.nCoinButton1[0] == i
						&& piuconfig.nCoinButton1[1] == j
						&& piuconfig.nCoinButton1[2] == nMode)
						{
							if(nEvent == EV_KEYDOWN) m_nExtButtons[0]++;
							else if(nEvent == EV_KEYUP) m_nExtButtons[0]--;
							nKey = EV_CREDIT1;
						}
						if(piuconfig.nCoinButton2[0] == i
						&& piuconfig.nCoinButton2[1] == j
						&& piuconfig.nCoinButton2[2] == nMode)
						{
							if(nEvent == EV_KEYDOWN) m_nExtButtons[1]++;
							else if(nEvent == EV_KEYUP) m_nExtButtons[1]--;
							nKey = EV_CREDIT2;
						}
						if(piuconfig.nConfigButton[0] == i
						&& piuconfig.nConfigButton[1] == j
						&& piuconfig.nConfigButton[2] == nMode)
						{
							if(nEvent == EV_KEYDOWN) m_nExtButtons[2]++;
							else if(nEvent == EV_KEYUP) m_nExtButtons[2]--;
							nKey = EV_CONFIG;
						}
						if(piuconfig.nServiceButton[0] == i
						&& piuconfig.nServiceButton[1] == j
						&& piuconfig.nServiceButton[2] == nMode)
						{
							if(nEvent == EV_KEYDOWN) m_nExtButtons[3]++;
							else if(nEvent == EV_KEYUP) m_nExtButtons[3]--;
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
									if(m_nArrows[x] == 1)
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
									m_nArrows[x]--;
								}
								if(nEvent == EV_KEYDOWN)
								{
									if(!m_nArrows[x])
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
									m_nArrows[x]++;
								}
							}
						}
						m_nSizeEvent++;
						m_nKeyEvent=(int*)max_realloc(m_nKeyEvent, m_nSizeEvent*sizeof(int));
						m_nEventEvent=(int*)max_realloc(m_nEventEvent, m_nSizeEvent*sizeof(int));
						m_nKeyEvent[m_nSizeEvent-1] = nKey;
						m_nEventEvent[m_nSizeEvent-1] = nEvent;
					}
				}
			}
			for(int j = 0; j < m_nJoystickButtons[i]; j++)
			{
				nEvent = 0;
				Uint8 value = SDL_JoystickGetButton(m_joystick[i], j);
				if(value != m_nValueJoystickButtons[i][j])
				{
					m_nValueJoystickButtons[i][j] = value;
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
								if(m_nArrows[x] == 1)
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
								m_nArrows[x]--;
							}
							if(nEvent == EV_KEYDOWN)
							{
								if(!m_nArrows[x])
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
								m_nArrows[x]++;
							}
						}
					}
					if(piuconfig.nCoinButton1[0] == i
					&& piuconfig.nCoinButton1[1] == j
					&& piuconfig.nCoinButton1[2] == 0)
					{
						if(nEvent == EV_KEYDOWN) m_nExtButtons[0]++;
						else if(nEvent == EV_KEYUP) m_nExtButtons[0]--;
						nKey = EV_CREDIT1;
					}
					if(piuconfig.nCoinButton2[0] == i
					&& piuconfig.nCoinButton2[1] == j
					&& piuconfig.nCoinButton2[2] == 0)
					{
						if(nEvent == EV_KEYDOWN) m_nExtButtons[1]++;
						else if(nEvent == EV_KEYUP) m_nExtButtons[1]--;
						nKey = EV_CREDIT2;
					}
					if(piuconfig.nConfigButton[0] == i
					&& piuconfig.nConfigButton[1] == j
					&& piuconfig.nConfigButton[2] == 0)
					{
						if(nEvent == EV_KEYDOWN) m_nExtButtons[2]++;
						else if(nEvent == EV_KEYUP) m_nExtButtons[2]--;
						nKey = EV_CONFIG;
					}
					if(piuconfig.nServiceButton[0] == i
					&& piuconfig.nServiceButton[1] == j
					&& piuconfig.nServiceButton[2] == 0)
					{
						if(nEvent == EV_KEYDOWN) m_nExtButtons[3]++;
						else if(nEvent == EV_KEYUP) m_nExtButtons[3]--;
						nKey = EV_SERVICE;
					}
					m_nSizeEvent++;
					m_nKeyEvent=(int*)max_realloc(m_nKeyEvent, m_nSizeEvent*sizeof(int));
					m_nEventEvent=(int*)max_realloc(m_nEventEvent, m_nSizeEvent*sizeof(int));
					m_nKeyEvent[m_nSizeEvent-1] = nKey;
					m_nEventEvent[m_nSizeEvent-1] = nEvent;
				}
			}
		}
		SDL_mutexV(lock);
	}
	return 0;
}

int CStdInput::PollEvent(int* nKey, int* nEvent)
{
	SDL_mutexP(m_lockEvent);
	int nRet = PollEventThread(nKey, nEvent);
	SDL_mutexV(m_lockEvent);
	if(!nRet)
	{
		SDL_Event ev;
		nRet = SDL_PollEvent(&ev);
		if(!nRet)
		{
			(*nKey) = 0;
			(*nEvent) = 0;
			return 0;
		}
		PollEventSDL(ev, nKey, nEvent);
		return 2;
	}
	return 1;
}

int CStdInput::PollEventThread(int* nKey, int* nEvent)
{
	if(m_nSizeEvent <= 0)
	{
		(*nKey) = 0;
		(*nEvent) = 0;
		m_nSizeEvent = 0;
		return 0;
	}
	else
	{
		(*nKey) = m_nKeyEvent[0];
		(*nEvent) = m_nEventEvent[0];
		for(int x = 0; x < (m_nSizeEvent-1); x++)
		{
			m_nKeyEvent[x] = m_nKeyEvent[x+1];
			m_nEventEvent[x] = m_nEventEvent[x+1];
		}
		m_nSizeEvent--;
		m_nKeyEvent=(int*)max_realloc(m_nKeyEvent, m_nSizeEvent*sizeof(int));
		m_nEventEvent=(int*)max_realloc(m_nEventEvent, m_nSizeEvent*sizeof(int));
		int ret = 0;
		if(m_nSizeEvent <= 0) ret = 0;
		else ret = 1;
		return ret;
	}
	return 0;
}

int CStdInput::PollEventSDL(SDL_Event keyevent, int* nKey, int* nEvent)
{
	(*nEvent) = 0;
	(*nKey) = 0;
    if ( keyevent.type == SDL_KEYDOWN )
    {
		if(keyevent.key.keysym.sym == SDLK_F1 || (piuconfig.nCoinButton1[0] == -1
		&& piuconfig.nCoinButton1[1] == keyevent.key.keysym.sym))
		{
			m_nExtButtons[0]++;
			(*nEvent) = EV_KEYDOWN;
			(*nKey) = EV_CREDIT1;
		}
		if(keyevent.key.keysym.sym == SDLK_F2 || (piuconfig.nCoinButton2[0] == -1
		&& piuconfig.nCoinButton2[1] == keyevent.key.keysym.sym))
		{
			m_nExtButtons[1]++;
			(*nEvent) = EV_KEYDOWN;
			(*nKey) = EV_CREDIT2;
		}
		if(keyevent.key.keysym.sym == SDLK_F3 || (piuconfig.nConfigButton[0] == -1
		&& piuconfig.nConfigButton[1] == keyevent.key.keysym.sym))
		{
			m_nExtButtons[2]++;
			(*nEvent) = EV_KEYDOWN;
			(*nKey) = EV_CONFIG;
		}
		if(keyevent.key.keysym.sym == SDLK_F4 || (piuconfig.nServiceButton[0] == -1
		&& piuconfig.nServiceButton[1] == keyevent.key.keysym.sym))
		{
			m_nExtButtons[3]++;
			(*nEvent) = EV_KEYDOWN;
			(*nKey) = EV_SERVICE;
		}
		for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++)
		{
			if(piuconfig.nWhereButtons[x][y] == -1
				&& piuconfig.nButtons[x][y] == keyevent.key.keysym.sym)
			{
				if(!(m_nArrows[x] & 0x10))
				{
					(*nEvent) = EV_KEYDOWN;
					switch(x)
					{
					case 0: (*nKey) = EV_P1_ARROW1; break;
					case 1: (*nKey) = EV_P1_ARROW7; break;
					case 2: (*nKey) = EV_P1_CENTER; break;
					case 3: (*nKey) = EV_P1_ARROW9; break;
					case 4: (*nKey) = EV_P1_ARROW3; break;
					case 5: (*nKey) = EV_P2_ARROW1; break;
					case 6: (*nKey) = EV_P2_ARROW7; break;
					case 7: (*nKey) = EV_P2_CENTER; break;
					case 8: (*nKey) = EV_P2_ARROW9; break;
					case 9: (*nKey) = EV_P2_ARROW3; break;
					}
					m_nArrows[x] |= 0x10;
				}
			}
		}
    }
	if(keyevent.type == SDL_KEYUP)
	{
		if(keyevent.key.keysym.sym == SDLK_F1 || (piuconfig.nCoinButton1[0] == -1
		&& piuconfig.nCoinButton1[1] == keyevent.key.keysym.sym))
		{
			m_nExtButtons[0]--;
			(*nEvent) = EV_KEYUP;
			(*nKey) = EV_CREDIT1;
		}
		if(keyevent.key.keysym.sym == SDLK_F2 || (piuconfig.nCoinButton2[0] == -1
		&& piuconfig.nCoinButton2[1] == keyevent.key.keysym.sym))
		{
			m_nExtButtons[1]--;
			(*nEvent) = EV_KEYUP;
			(*nKey) = EV_CREDIT2;
		}
		if(keyevent.key.keysym.sym == SDLK_F3 || (piuconfig.nConfigButton[0] == -1
		&& piuconfig.nConfigButton[1] == keyevent.key.keysym.sym))
		{
			m_nExtButtons[2]--;
			(*nEvent) = EV_KEYUP;
			(*nKey) = EV_CONFIG;
		}
		if(keyevent.key.keysym.sym == SDLK_F4 || (piuconfig.nServiceButton[0] == -1
		&& piuconfig.nServiceButton[1] == keyevent.key.keysym.sym))
		{
			m_nExtButtons[3]--;
			(*nEvent) = EV_KEYUP;
			(*nKey) = EV_SERVICE;
		}
		for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++)
		{
			if(piuconfig.nWhereButtons[x][y] == -1
				&& piuconfig.nButtons[x][y] == keyevent.key.keysym.sym)
			{
				if(m_nArrows[x] &= 0x10)
				{
					(*nEvent) = EV_KEYUP;
					switch(x)
					{
					case 0: (*nKey) = EV_P1_ARROW1; break;
					case 1: (*nKey) = EV_P1_ARROW7; break;
					case 2: (*nKey) = EV_P1_CENTER; break;
					case 3: (*nKey) = EV_P1_ARROW9; break;
					case 4: (*nKey) = EV_P1_ARROW3; break;
					case 5: (*nKey) = EV_P2_ARROW1; break;
					case 6: (*nKey) = EV_P2_ARROW7; break;
					case 7: (*nKey) = EV_P2_CENTER; break;
					case 8: (*nKey) = EV_P2_ARROW9; break;
					case 9: (*nKey) = EV_P2_ARROW3; break;
					}
					// is the keyboard, so only dimminute 1 if there is 1
					m_nArrows[x] &= ~0x10;
				}
			}
		}
	}
	if ( keyevent.type == SDL_QUIT )
	{
		(*nKey) = (*nEvent) = KEY_QUIT;
	}
	return 0;
}
