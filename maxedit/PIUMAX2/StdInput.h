#ifndef STD_INPUT_PIUMAX
#define STD_INPUT_PIUMAX

#include "cmnpiu.h"

#define KEY_QUIT -0xFFFE

class CStdInput
{
public:
	CStdInput(void);
	~CStdInput(void);
	static int m_nDone;

	// For input
	static int m_nJoystick;
	static SDL_Joystick** m_joystick;
	static int* m_nJoystickHat;
	static int** m_nValueJoystickHat;
	static int* m_nJoystickButtons;
	static int** m_nValueJoystickButtons;

	// For events
	static int m_nSizeEvent;
	static int* m_nKeyEvent;
	static int* m_nEventEvent;

	// For data access
	// No time data (no __int64 nTimes[10])
	// This value must be asigned in each interface
	static int m_nArrows[10];
	static int m_nExtButtons[4];	// 0 = Coin1, 1 = Coin2, 3 = service, 4 = test
	static int m_nChoice[12];		// left1p, right1p, up1p, down1p, fire1p, fire1p, left2p, right2p, up2p, down2p, fire2p, fire2p

	// For thread
	static SDL_Thread *m_eventThread;
	static SDL_mutex *m_lockEvent;

	// Functions
	static int LaunchInput(void);
	static void StopInput(void);
	static int EventThread(void* data);
	static int PollEvent(int* nKey, int* nEvent);
	static int PollEventThread(int* nKey, int* nEvent);
	static int PollEventSDL(SDL_Event keyevent, int* nKey, int* nEvent);
};

#endif // STD_INPUT_PIUMAX
