#ifndef PIUMAX_MAXMENU
#define PIUMAX_MAXMENU

#include "basutil.h"
#include "MaxScreen.h"
#include "eventdef.h"

struct PITEM	// This is Event information for each index
{
	MID id;				// ID and subID (Describes action)
	MID arg;			// Especial argument (Describes argument)
	MIND ind_i;			// Index to resource image
	MIND ind_t;			// Index to resource text

	// Status
	CSprite* ires;		// IMG Resource asociated
	CString* tres;		// Text Resource asociated
	int nift_res;		// Index of font text
};

/****************************
PITEM CONSIDERATIONS

Common identificators -> PITEM::id is 0 to up
This is a common identificator, Event "press" will send whit this id

Special identificators ->
values for PITEM::id
** They are the same than CMaxMenu considerations
PITEM::arg -> Item Asociated

Jump identificators -> PITEM::id is ID(0 OR 1, 9 OR 10)
PITEM::arg -> Item To Jump (if this item is not in the current catalog, jumps at the same item)
*****************************/

struct MITEM	// This is reference to MCATALOG or MITEM
{
	int type;	// 0 for PITEM, 1 for MCATALOG
	int in;		// Index for vector
};

struct MCATALOG
{
	vector< MITEM > items;
	vector< int > nJumps;	// Index to jump
	MIND ind_i;			// Index to resource image
	MIND ind_t;			// Index to resource text

	// Status
	CSprite* ires;		// IMG Resource asociated
	CString* tres;		// Text Resource asociated
	int nift_res;		// Index of font text
};

struct MINPUT
{
	// Parameters
	MID id;				// ID and subID that identifies using about this action
	MID mod;				// ID and subID about modificator, if mod isn't pressed, this input doesn't work
	MID action;			// ID and subID used in action
	MID arg;			// ID and subID argument
	__int64 nTimeRep;	// Repetitive time between actions if bHold (Holding) [if 0, no repetitive action]

	// Status
	int nIs;			// Do action? (a number indicates actions left)
	bool bHold;			// Is hold?
	bool bMod;			// Is Mod Active?
	__int64 nTimePress;	// Last time pressed (about Base Time)
};

struct MCOMMAND
{
	// Parameters
	vector< MID > ids;	// vector of ID describing command (This is a list of MINPUT id's that coincides with CMaxMenu::m_comm)
	MID action;			// ID and subID used in action
	MID arg;			// ID and subID argument

	// Status
	int nIs;			// Do action? (a number indicates actions left, should be one if action is required)
	__int64 nTimeCom;	// Last time commanded (about Base Time)
};

struct MINFO	// Administrates Menu info
{
	// Timing variables
	__int64 nDurEvent[MAXMENU_EVENT_SIZE];		// Duration of event (parameter)
	__int64 nTimeEvent[MAXMENU_EVENT_SIZE];		// Time last event (status)

	// Controling user variables
	vector< MINPUT > input;		// Input vector (parameter)
	int nPlayer;				// Player that controles this menu (proporcionated by interface)
	bool bLoopSelection;		// Indicates looping
	int nSizeShow;				// Size of showing
	int nIndCenShow;			// Index about showing that uses central (or selected)

	// Controling fast-commands (sequence)
	vector< MCOMMAND > command;	// Command vector (parameter)

	// Status about loading
	bool bResI;
	bool bResT;
};

struct MPOSTEVENT
{
	MID id;			// ID about event
	__int64 nTime;	// Time to be sended

	MPOSTEVENT()
	{id = MID(0,0); nTime = 0;};
	MPOSTEVENT(MID mid, __int64 time)
	{id = mid; nTime = time;};
};

class CMaxMenu
{
public:
	CMaxMenu(void);
	~CMaxMenu(void);
	// Current
	bool m_bShow;
	bool m_bEnabled;
	int m_nActive;							// Current Active Set-of-items
	vector< vector< MITEM > > m_curItem;	// Current Sets of items
	vector< int > m_curItemSel;				// Current Items selectionated for each Set
	vector< int > m_curCat;					// Current Catalog filling

	// Parameters
	vector< MCATALOG > m_cats;				// All the catalogs
	vector< PITEM > m_pitem;				// All the items
	int m_nStCat;							// Which catalog starts filling current

	MINFO m_info;							// Basic menu info

	__int64 m_nTime;						// Time Base
	vector< vector< CMaxScreen* > > m_screenshow;		// Screens used in menu (for items)
	vector< CMaxScreen* > m_screen;			// Screens used in menu (common)

	vector< MAX_EVENT_CALLBACK > m_callback;	// List of callbacks about menu events (ONLY menu events)
	vector< MID > m_events;					// List of events passed by Event()

	// Commands (The size holding last commands is set by max_size about MCOMMANDS)
	// This will be filled with coincidences about MINPUT::id's introduced in menu
	// This won't be filled with actions
	vector< vector< MID > > m_comm;	

	// Post-events, events that will be send to screens and
	vector< MPOSTEVENT > m_pev;

	// Methods
	int OpenFromAcc(LPCTSTR strFile);
	void Delete(void);
	void Update(DWORD dwMilliseconds);
	int Event(MID ev);
	int SendEventCallback(MID id, MID arg);
	int SendEventScreens(MID  id);
	void Draw(void);
	int Create(void);
	int Exit(void);
	int DoAction(MID id, MID arg);
	int OutCatalog(void);

	// Action functions
	void DoExit(void);				// MAXMENU_EVENT_EXIT
	void DoActivate(void);			// MAXMENU_EVENT_ACTIVATE
	void DoDisable(void);			// MAXMENU_EVENT_DISABLE
	void DoOutCatalog(void);		// MAXMENU_EVENT_OUTCATALOG
	void DoSetItem(void);			// MAXMENU_EVENT_SETITEM
	void DoNextItem(void);			// MAXMENU_EVENT_NEXTITEM
	void DoPrevItem(void);			// MAXMENU_EVENT_PREVITEM
	void DoJumpNextItem(void);		// MAXMENU_EVENT_JUMPNEXTITEM
	void DoJumpPrevItem(void);		// MAXMENU_EVENT_JUMPPREVITEM
	void DoReplaceStack(int nCat);	// MAXMENU_EVENT_REPLACESTACK

	// Common resource claim
	int SetResourcesI(CSpriteRes* res);
	int SetResourcesT(vector< CString* > strs, vector< int > ifts);
	int SetResourcesV(int nPos, CVideoRes* res);
	int SetResourcesS(int nPos, CSoundRes* res);
	int SetFonts(vector< CFont2* > fnts);
	int SetPlayers(int nPlayers, int nFocus);	// Num of players and which player use it?

	// Refill operations
	int ReplaceItems(int nPos, vector< PITEM > pitems);
	int ReplaceItemsAtID(MID id, MID arg, vector< PITEM > pitems);

	// Current item stack populate
	void PopulateShow(int nCat, int nCen);
	// Llena (o actualiza) los recursos de cada screen independiente
	void PopulateScreenSubRes(int nCat);
	// Llena todos los screen con recursos dependientes
	void PopulateScreenSubResAll(void);
};

/***************************
CMaxMenu considerations.

- For each screen, will storage C*****Res as follows
Index 0 (id 0) for shared resources
Index 1 for MITEM showing (current showing)
Rest of catalog items are reserved

- This sends a ID and SUBID Event, with an argument, they are following
X	00:	Creation of menu (Begin 0, End 1)
X	01:	Exiting of menu (Begin 0, End 1)
X	02:	Activation of menu (Begin 0, End 1)
X	03:	Disablement of menu (Begin 0, End 1)
X	04:	In to catalog (Begin 0, End 1)
X	05:	Exit from catalog (Begin 0, End 1)
X	06:	Set Item (Begin 0, End 1)
X	07:	Next Item (Begin 0, End 1)
X	08:	Prev Item (Begin 0, End 1)
X	09:	Jump to nexts Items (Begin 0, End 1)
X	10:	Jump to Prevs Items (Begin 0, End 1)
****************************/

#endif // PIUMAX_MAXMENU