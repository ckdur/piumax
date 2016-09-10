#ifndef PIUMAX_INTERFACE
#define PIUMAX_INTERFACE

//#include "stdafx.h"
#include "GLFont2.h"
#include "MaxMenu.h"
#include "MaxLua.h"

struct INT_ACTION_EVENT
{
	int nTypeSend;		// Type of object to send (0 for MENU, 1 for screen)
	int nSend;			// Object to send (if -1, send to all)
	MID send;			// Send info
	DWORD dwTime;		// Time to be sent
};

struct INT_ACTION
{
	// Parameters
	MID id;					// ID in response
	MID action;				// Action to do
	MID arg;				// Argument relative
	int nConfirm;			// Number of confirms (default 1)

	// Event pushing acoord confirms
	vector< INT_ACTION_EVENT > events;

	// Status
	int nConfirmStat;	// Confirm status (if this >= nConfirm, ACTION takes effect)
	__int64 nTimeConfirm;	// Time that the las corfirm was polled
};

class CMaxInterface
{
public:
	CMaxInterface(void);
	~CMaxInterface(void);
	vector< CSpriteRes* > m_ires;			// Vector of pointers resources (images)
	vector< CVideoRes* > m_vres;			// Vector of pointers resources (video)
	vector< CSoundRes* > m_sres;			// Vector of pointers resources (sound)
	vector< CFont2* > m_fnt;				// Vector of pointers fonts
	vector< vector< CString* > > m_txt;		// Vector of pointers text
	vector< vector< int > > m_ift;			// Vector of Index Font Text (font used for each text)
	__int64 m_nTime;						// Base time

	vector< CMaxScreen* > m_screen;			// Base screens

	vector< CMaxMenu* > m_menu;				// Base menus
	vector< int > m_nMenuFocus;				// Menu is focused! in? (one for player, if -1 is no focus)

	vector< INT_ACTION > m_action;			// Actions about event reciving

	vector< INT_ACTION_EVENT > m_evToSend;	// Events to send

	// Auxiliars
	vector< CString > m_strAuxFileRes;		// Contents all names (path + name) about file resources
	vector< CString > m_strAuxFileScreens;		// Contents all names (path + name) about file screens
	vector< CString > m_strAuxFileMenus;		// Contents all names (path + name) about file menus

	vector< MAX_EVENT_CALLBACK > m_callback;	// List of callbacks about menu events (ONLY menu events)

	// The current LUA class
	CMaxLua* m_lua;

	// State about the lua reader
	bool m_bLuaEvent;

	// Common methods
	int InitCommon(LPCTSTR strName);			// Common initialization
	void ResetCommon(void);						// Common reseting
	void ReleaseCommon(void);					// Common release
	void DrawCommon(void);						// Common screens drawing
	void DrawScreensCommon(void);				// Common screens drawing
	void DrawMenusCommon(void);					// Common menu drawing
	void UpdateCommon(DWORD dwMilliseconds);	// Common Update
	int EventCommon(MID ev);					// Common Event polling
	int MenuCallbackCommon(MID id, MID arg);	// Common Menu callbacking
	int LookActionCommon(MID ev);				// Common Action seeking
	MID InterfaceStatusCommon(void);			// Common Interface Status reporting

	int LuaInitCommon(LPCTSTR strName);			// Common Lua Initialization
	int LuaEventCommon(MID ev);					// Common Lua Eventing

	int LoadResourcesCommon(LPCTSTR strName);
	int LoadScreensCommon(LPCTSTR strName);
	int LoadMenusCommon(LPCTSTR strName);

	int SendEventCallback(MID id, MID arg);
	int UpdateActions(void);
	int UpdateResources(void);

	// Virtual functions
	virtual int Init(LPCTSTR strName);
	virtual void Reset(void);
	virtual void Release(void);
	virtual void Draw(void);
	virtual void Update(DWORD dwMilliseconds);
	virtual int Event(MID ev);
	virtual int MenuCallback(MID id, MID arg);
	virtual int LookAction(MID ev);

	virtual int LoadResources(LPCTSTR strName);
	virtual int LoadScreens(LPCTSTR strName);
	virtual int LoadMenus(LPCTSTR strName);

	virtual MID InterfaceStatus(void);

	virtual int LuaInit(LPCTSTR strName);
	virtual int LuaEvent(MID ev);
};

// Definition about global object used for wrapper functions
extern CMaxInterface* g_MaxIntHandle;

// ****************MENU_CALLBACK*******************

// Definition about Wrapper function MAX_EVENT_CALLBACK
int __stdcall MaxIntCallback(MID id, MID arg);

// Definition about Holder global object
int MaxIntSetCallbackHandle(CMaxInterface* maxIntHandle);

// ********************LUA*************************
// Definitions of LUA about MaxInterface

// 1. Definitions about MaxInterface LUA_INIT
int MaxIntAddSprite(lua_State* L);
int MaxIntInsertSprite(lua_State* L);
int MaxIntDeleteSprite(lua_State* L);
int MaxIntClearSprite(lua_State* L);
int MaxIntGetSizeSprite(lua_State* L);

int MaxIntAddVideo(lua_State* L);
int MaxIntInsertVideo(lua_State* L);
int MaxIntDeleteVideo(lua_State* L);
int MaxIntClearVideo(lua_State* L);
int MaxIntGetSizeVideo(lua_State* L);

int MaxIntAddSound(lua_State* L);
int MaxIntInsertSound(lua_State* L);
int MaxIntDeleteSound(lua_State* L);
int MaxIntClearSound(lua_State* L);
int MaxIntGetSizeSound(lua_State* L);

int MaxIntAddFont(lua_State* L);
int MaxIntInsertFont(lua_State* L);
int MaxIntDeleteFont(lua_State* L);
int MaxIntClearFont(lua_State* L);
int MaxIntGetSizeFont(lua_State* L);

int MaxIntAddString(lua_State* L);
int MaxIntInsertString(lua_State* L);
int MaxIntDeleteString(lua_State* L);
int MaxIntClearString(lua_State* L);
int MaxIntGetSizeString(lua_State* L);

int MaxIntAddScreen(lua_State* L);
int MaxIntInsertScreen(lua_State* L);
int MaxIntDeleteScreen(lua_State* L);
int MaxIntClearScreen(lua_State* L);
int MaxIntGetSizeScreen(lua_State* L);

int MaxIntAddMenu(lua_State* L);
int MaxIntInsertMenu(lua_State* L);
int MaxIntDeleteMenu(lua_State* L);
int MaxIntClearMenu(lua_State* L);
int MaxIntGetSizeMenu(lua_State* L);

int MaxIntDefineAction(lua_State* L);
int MaxIntUndefineAction(lua_State* L);

// 2. Definitions about MaxInterface LUA_EVENT
int MaxLuaEvent(lua_State* L);
int MaxLuaEventTo(lua_State* L);

// 3. Common-used Registers
int MaxIntRegister_LUA_INIT(CMaxLua* lua);
int MaxIntRegister_LUA_EVENT(CMaxLua* lua);
#endif // PIUMAX_INTERFACE
