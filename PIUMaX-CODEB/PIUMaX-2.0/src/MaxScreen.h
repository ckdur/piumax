/*
Basic Screen PIU MaX
This programm uses reference resources
and animate it acoording to event's notification

Transition:
Array x4 of coord's from each
*/
#ifndef PIUMAX_MAXSCREEN
#define PIUMAX_MAXSCREEN

#include "GLFont2.h"
#include "GLFont.h"
#include "SpriteRes.h"
#include "VideoRes.h"
#include "SoundRes.h"

// ** Reading constants for objects

#define crSCR_OBJECT_P1 0
#define crSCR_OBJECT_P2 3
#define crSCR_OBJECT_P3 6
#define crSCR_OBJECT_P4 9
#define crSCR_OBJECT_V1 12
#define crSCR_OBJECT_V2 14
#define crSCR_OBJECT_ID 16
#define crSCR_OBJECT_SUBID 17
#define crSCR_OBJECT_TYPE 18
#define crSCR_OBJECT_TOTAL 19

// ** Object struct
struct SCR_OBJECT
{
	double p[4][3];	// 4 coord rect (WARNING, MUST BE A RECT)
	double v[2][2]; // 2 coord text
	unsigned int id[2];	// ID, subID about resource
	int type;	// Type about resource
};

enum SCR_OBJTYPE
{
	SCR_OBJTYPE_IMAGE,		// Image resource
	SCR_OBJTYPE_VIDEO,		// Video resource
	SCR_OBJTYPE_TEXT,		// Text resource
	SCR_OBJTYPE_SIZE		// SIZE OF THIS ENUM
};

extern TCHAR* tc_OBJTYPE[]; /* =
	{_T("IMAGE"), _T("VIDEO"), _T("TEXT")};*/

// ** Reading constants for transitions

#define crSCR_TRANSITION_TR 0
#define crSCR_TRANSITION_ROT 6
#define crSCR_TRANSITION_SC 14
#define crSCR_TRANSITION_EF 20
#define crSCR_TRANSITION_EFARG 21
#define crSCR_TRANSITION_DUR 29
#define crSCR_TRANSITION_MOD 30
#define crSCR_TRANSITION_MODARG 31
#define crSCR_TRANSITION_SPSTAT 35
#define crSCR_TRANSITION_TOTAL 36

struct SCR_TRANSITION
{
	double tr[6];	// XYZ1 to XYZ2 translation
	double rot[8];	// XYZ basepoint, UxUyUz vector and Angle1 to Angle2 rotation (in degress)
	double sc[6];	// SxSySz1 to SxSySz2 scale
	int ef;			// ID efect (see list)
	double aef[8];	// ABCD1 to ABCD2 argument
	__int64 dur;	// Duration (in milliseconds. If negative, it's loop)
	int mod;		// Modificator (see list)
	double amod[4];	// Modificator arguments
	int spstat;		// Sprite State (see list)
};

enum SCR_TRANSEF
{
	SCR_TRANSEF_NULL,		// Simple draw
	SCR_TRANSEF_NODRAW,		// No draw
	SCR_TRANSEF_APPEAR,		// Simple draw with alpha
	SCR_TRANSEF_XAPPEAR,	// left-right appear using degrade
	SCR_TRANSEF_YAPPEAR,	// up-down appear using degrade
	SCR_TRANSEF_DEGAPPEAR,	// X-degrees appear using degrade
	SCR_TRANSEF_SIZE		// SIZE OF THIS ENUM
};

extern TCHAR* tc_TRANSEF[];/*
	= {_T("NULL"), _T("NODRAW"), _T("APPEAR"), _T("XAPPEAR"), _T("YAPPEAR"), _T("DEGAPPEAR")};*/

enum SCR_TRANSMOD
{
	SCR_TRANSMOD_LINEAL,		// Lineal transition (default) [a1 = pendient, a2 = Axis Y]
	SCR_TRANSMOD_HSTEP,			// Heaviside Step [a1 = timeoff, a2 = Amplitude, a3 = timemod, a4 = Axis Y]
	SCR_TRANSMOD_POW,			// Power-of modifier transition [a1 = pow, a2 = timeoff, a3 = Amplitude, a4 = Axis Y]
	SCR_TRANSMOD_SIN,			// Periodical Sinusoidal transition [a1 = 1/4 period, a2 = timeoff, a3 = Amplitude, a4 = Axis Y]
	SCR_TRANSMOD_PLINEAL,		// Periodical Lineal transition [a1 = period, a2 = timeoff, a3 = Amplitude, a4 = Axis Y]
	SCR_TRANSMOD_PTRIANGLE,		// Periodical Triangle transition [a1 = 1/4 period, a2 = timeoff, a3 = Amplitude, a4 = Axis Y]
	SCR_TRANSMOD_PRECT,			// Periodical Rectangular transition [a1 = 1/2 period, a2 = timeoff, a3 = Amplitude, a4 = Axis Y]
	SCR_TRANSMOD_SIN_R,			// Periodical Rectificated Sinusoidal transition [a1 = 1/2 period, a2 = timeoff, a3 = Amplitude, a4 = Axis Y]
	SCR_TRANSMOD_PTRIANGLE_R,	// Periodical Rectificated Triangle transition [a1 = 1/2 period, a2 = timeoff, a3 = Amplitude, a4 = Axis Y]
	SCR_TRANSMOD_SIZE			// SIZE OF THIS ENUM
};

extern TCHAR* tc_TRANSMOD[];/* =
{_T("LINEAL"), _T("HSTEP"), _T("POW"), _T("SIN"), _T("PLINEAL"),
_T("PTRIANGLE"), _T("PRECT"), _T("SIN_R"), _T("PTRIANGLE_R")};*/

enum SCR_TRANSSPSTAT
{
	SCR_TRANSSPSTAT_PLAY,			// Play (if was playing, nothing pass)
	SCR_TRANSSPSTAT_PAUSE,			// Pause
	SCR_TRANSSPSTAT_STOP,			// Stop (Pauses and set it to begin)
	SCR_TRANSSPSTAT_PLAYLOOP,		// Play indefinitely
	SCR_TRANSSPSTAT_PLAYONCE,		// Play once (forces begin)
	SCR_TRANSSPSTAT_PLAYONCEBEG,	// Play once, at end shows begining (forces begin)
	SCR_TRANSSPSTAT_SIZE			// SIZE OF THIS ENUM
};

extern TCHAR* tc_TRANSSPSTAT[];/* =
{_T("PLAY"), _T("PAUSE"), _T("STOP"), _T("PLAYLOOP"), _T("PLAYONCE"),
_T("PLAYONCEBEG")};*/

// ** Reading constants for events

#define crSCR_EVENT_ID 0
#define crSCR_EVENT_TOTAL 2
#define crSCR_EVENT_TRANSITION_OBJ 0
#define crSCR_EVENT_TRANSITION_TRANSITIONS 1
#define crSCR_EVENT_TRANSITION_TOTAL 1
#define crSCR_EVENT_SOUND_SOUND 0
#define crSCR_EVENT_SOUND_SMOD 3
#define crSCR_EVENT_SOUND_BEG 4
#define crSCR_EVENT_SOUND_DUR 5
#define crSCR_EVENT_SOUND_TOTAL 6
#if (crSCR_EVENT_TRANSITION_TRANSITIONS > crSCR_EVENT_TRANSITION_TOTAL)
#error "You can't define the item TRANS out of the TOTAL"
#endif

enum SCR_SOUNDSMOD
{
	SCR_SOUNDSMOD_ONCE,			// Play it once
	SCR_SOUNDSMOD_LOOP,			// Play it looping
	SCR_SOUNDSMOD_STOP,			// Stop Current channel
	SCR_SOUNDSMOD_PAUSE,		// Pause Current channel
	SCR_SOUNDSMOD_PLAY,			// Play Current channel (?)
	SCR_SOUNDSMOD_SIZE			// SIZE OF THIS ENUM
};

extern TCHAR* tc_SOUNDSMOD[];/* =
{_T("ONCE"), _T("LOOP"), _T("STOP"), _T("PAUSE"), _T("PLAY")};*/

struct SCR_SOUND
{
	int id[2];
	int cha;
};

struct SCR_EVENT
{
	int id[2];									// ID and subID that describes event (parameter)

	vector< unsigned int > obj;					// List of object aplicating event (parameter)
	vector< __int64 > time;						// Current time-stamp for each object-aplicated (auxiliar)
	vector< vector< unsigned int > > trans;		// Transitions to play for each object (parameter)

	vector< SCR_SOUND > sound;					// Sounds that will be played (parameter)
	vector< int > smod;							// List of object aplicating event (see list) (parameter)
	vector< __int64 > sbeg;						// The sound is played at? (parameter)
	vector< __int64 > sdur;						// Duration of sounds (-1 is played until the end) (parameter)
};

class CMaxScreen
{
public:
	CMaxScreen(void);
	~CMaxScreen(void);
	vector< SCR_OBJECT > m_ob;		// Objects
	vector< SCR_TRANSITION > m_tr;	// Transitions
	vector< SCR_EVENT > m_ev;		// Events
	vector< CSpriteRes* > m_ires;	// Vector of pointers resources (images)
	vector< CVideoRes* > m_vres;	// Vector of pointers resources (video)
	vector< CSoundRes* > m_sres;	// Vector of pointers resources (sound)
	vector< CFont2* > m_fnt;		// Vector of pointers fonts
	vector< vector< CString* > > m_txt;		// Vector of pointers text
	vector< vector< int > > m_ift;			// Vector of Index Font Text (font used for each text)
	__int64 m_nTime;	// Base time
	int m_nCurev;		// Current event
	vector< int > m_sts;	// State for each sound

	int OpenFromAcc(LPCTSTR strFile);
	int Reset(void);
	void Update(DWORD dwMilliseconds);
	int SetEvent(MID id);
	int SetEventIndex(int nIndex);
	void Draw(void);
	void Delete(void);

	// Function about pointers to catalogs
	int AddSpriteRes(CSpriteRes* res);
	int InsertSpriteRes(int nPos, CSpriteRes* res);
	void DeleteSpriteRes(int nPos);
	void ClearSpriteRes(void);

	int AddVideoRes(CVideoRes* res);
	int InsertVideoRes(int nPos, CVideoRes* res);
	void DeleteVideoRes(int nPos);
	void ClearVideoRes(void);

	int AddSoundRes(CSoundRes* res);
	int InsertSoundRes(int nPos, CSoundRes* res);
	void DeleteSoundRes(int nPos);
	void ClearSoundRes(void);

	int AddStringCat(vector< CString* > txt, vector< int > ift);
	int InsertStringCat(int nPos, vector< CString* > txt, vector< int > ift);
	void DeleteStringCat(int nPos);
	void ClearStringCat(void);

	int AddFont(CFont2* res);
	int InsertFont(int nPos, CFont2* res);
	void DeleteFont(int nPos);
	void ClearFont(void);

	// Functions about Drawing effects
	void DrawNull(int obj, double* aef, double dApt, __int64 nTime, int nTra);
	void DrawNoDraw(int obj, double* aef, double dApt, __int64 nTime, int nTra);
	void DrawAppear(int obj, double* aef, double dApt, __int64 nTime, int nTra);
	void DrawXAppear(int obj, double* aef, double dApt, __int64 nTime, int nTra);
	void DrawYAppear(int obj, double* aef, double dApt, __int64 nTime, int nTra);
	void DrawXCont(int obj, double* aef, double dApt, __int64 nTime, int nTra);
	void DrawYCont(int obj, double* aef, double dApt, __int64 nTime, int nTra);
	void DrawDegAppear(int obj, double* aef, double dApt, __int64 nTime, int nTra);

	// Modificator function
	double Mod(double dApt, int mod, double* amod);

	// Get pure texture, and deletion issue
	CGraphTexture* GetTex(int obj, bool* bDel);

	// Trasformation function common
	void TransformTransitionCommon(int nTra, double dApt);
};

#endif // PIUMAX_MAXSCREEN
