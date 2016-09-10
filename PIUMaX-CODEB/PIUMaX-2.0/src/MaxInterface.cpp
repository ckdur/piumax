// MaxInterface.cpp
// Made by: CK-Dur
/*
This file includes definitions about common loading functions
MaxInterface_2.cpp: Includes definitions about common functionality about common interface
MaxInterface_3.cpp: Auxiliar, includes extended about basic (only
*/

#include "MaxInterface.h"
#include "AccReader.h"
#include "basutil.h"
#include "eventdef.h"
#include "SoundSys.h"

CMaxInterface::CMaxInterface(void)
{
	m_nTime = 0;
	m_lua = NULL;
	m_bLuaEvent = false;
}

CMaxInterface::~CMaxInterface(void)
{
	Release();
}

enum MAXINTINITACC_READER
{
	MAXINTINITACC_FILERESOURCES,
	MAXINTINITACC_FILESCREENS,
	MAXINTINITACC_FILEMENUS,
	MAXINTINITACC_ACTION,
	MAXINTINITACC_ACTION_CONFIRM,
	MAXINTINITACC_ACTION_CONFIRM_EVENT,
	MAXINTINITACC_LUA_INIT,
	MAXINTINITACC_LUA_EVENT,
	MAXINTINITACC_SIZE
};

#define crMAXINTINITACC_FILERESOURCES		0
#define crMAXINTINITACC_FILERESOURCES_SIZE	1

#define crMAXINTINITACC_FILESCREENS		0
#define crMAXINTINITACC_FILESCREENS_SIZE	1

#define crMAXINTINITACC_FILEMENUS		0
#define crMAXINTINITACC_FILEMENUS_SIZE	1

#define crMAXINTINITACC_ACTION_ID				0
#define crMAXINTINITACC_ACTION_SUBID			1
#define crMAXINTINITACC_ACTION_ACTIONID			2
#define crMAXINTINITACC_ACTION_ACTIONSUBID		3
#define crMAXINTINITACC_ACTION_ARGID			4
#define crMAXINTINITACC_ACTION_ARGSUBID			5
#define crMAXINTINITACC_ACTION_SIZE				6

#define crMAXINTINITACC_ACTION_CONFIRM		0
#define crMAXINTINITACC_ACTION_CONFIRM_SIZE	1

#define crMAXINTINITACC_ACTION_CONFIRM_EVENT_TYPESEND		0
#define crMAXINTINITACC_ACTION_CONFIRM_EVENT_INDEXSEND		1
#define crMAXINTINITACC_ACTION_CONFIRM_EVENT_SENDID			2
#define crMAXINTINITACC_ACTION_CONFIRM_EVENT_SENDSUBID		3
#define crMAXINTINITACC_ACTION_CONFIRM_EVENT_TIME			4
#define crMAXINTINITACC_ACTION_CONFIRM_EVENT_SIZE			5

#define crMAXINTINITACC_LUA_INIT_FILE	0
#define crMAXINTINITACC_LUA_INIT_SIZE	1

#define crMAXINTINITACC_LUA_EVENT_FILE	0
#define crMAXINTINITACC_LUA_EVENT_SIZE	1

int CMaxInterface::InitCommon(LPCTSTR strName)
{
	CAccReader reader;
	//int nDep = 0;

	m_lua = new CMaxLua();

	CString strDir;
	GetDirStr(CString(strName), &strDir);

	// Set the format
	// ** CMaxInterface Init public parameters
	reader.AddAttrib(_T("FILERESOURCES"), ACCFORMAT_NORMAL, false);
	reader.AddAttrib(_T("FILESCREENS"), ACCFORMAT_NORMAL, false);
	reader.AddAttrib(_T("FILEMENUS"), ACCFORMAT_NORMAL, false);
	reader.AddAttrib(_T("ACTION"), ACCFORMAT_NORMAL, false);
	reader.AddAttrib(_T("ACTION_CONFIRM"), ACCFORMAT_NORMAL, false, MAXINTINITACC_ACTION);
	reader.AddAttrib(_T("ACTION_CONFIRM_EVENT"), ACCFORMAT_NORMAL, true, MAXINTINITACC_ACTION);

	reader.AddAttrib(_T("LUA_INIT"), ACCFORMAT_NORMAL, false);
	reader.AddAttrib(_T("LUA_EVENT"), ACCFORMAT_NORMAL, false);

	// ** Asegurate good atachment
	if(reader.m_nSizeAttrib != MAXINTINITACC_SIZE) return 0;

	// ** Read the acc
	if(!reader.Read(strName)) return 0;

	// ** FILERESOURCES ** READING
	for(size_t j = 0; j < reader.m_strData[MAXINTINITACC_FILERESOURCES].size(); j++)
	{
		if(reader.m_strData[MAXINTINITACC_FILERESOURCES][j].size() < 1) continue;
		if(reader.m_strData[MAXINTINITACC_FILERESOURCES][j][0].size() < crMAXINTINITACC_FILERESOURCES_SIZE) continue;

		m_strAuxFileRes.push_back(strDir
			+reader.m_strData[MAXINTINITACC_FILERESOURCES][j][0][crMAXINTINITACC_FILERESOURCES]);
	}

	// ** FILESCREENS ** READING
	for(size_t j = 0; j < reader.m_strData[MAXINTINITACC_FILESCREENS].size(); j++)
	{
		if(reader.m_strData[MAXINTINITACC_FILESCREENS][j].size() < 1) continue;
		if(reader.m_strData[MAXINTINITACC_FILESCREENS][j][0].size() < crMAXINTINITACC_FILESCREENS_SIZE) continue;

		m_strAuxFileScreens.push_back(strDir
			+reader.m_strData[MAXINTINITACC_FILESCREENS][j][0][crMAXINTINITACC_FILESCREENS]);
	}

	// ** FILEMENUS ** READING
	for(size_t j = 0; j < reader.m_strData[MAXINTINITACC_FILEMENUS].size(); j++)
	{
		if(reader.m_strData[MAXINTINITACC_FILEMENUS][j].size() < 1) continue;
		if(reader.m_strData[MAXINTINITACC_FILEMENUS][j][0].size() < crMAXINTINITACC_FILEMENUS_SIZE) continue;

		m_strAuxFileMenus.push_back(strDir
			+reader.m_strData[MAXINTINITACC_FILEMENUS][j][0][crMAXINTINITACC_FILEMENUS]);
	}

	// ** ACTION ** READING
	for(size_t j = 0; j < reader.m_strData[MAXINTINITACC_ACTION].size(); j++)
	{
		INT_ACTION action;
		if(reader.m_strData[MAXINTINITACC_ACTION][j].size() < 1) continue;
		if(reader.m_strData[MAXINTINITACC_ACTION][j][0].size() < crMAXINTINITACC_ACTION_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION][j][0][crMAXINTINITACC_ACTION_ID], &action.id.nId)) continue;
		if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION][j][0][crMAXINTINITACC_ACTION_SUBID], &action.id.nSubId)) continue;
		if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION][j][0][crMAXINTINITACC_ACTION_ACTIONID], &action.action.nId)) continue;
		if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION][j][0][crMAXINTINITACC_ACTION_ACTIONSUBID], &action.action.nSubId)) continue;
		if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION][j][0][crMAXINTINITACC_ACTION_ARGID], &action.arg.nId)) continue;
		if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION][j][0][crMAXINTINITACC_ACTION_ARGSUBID], &action.arg.nSubId)) continue;

		// ** ACTION_CONFIRM ** READING
		if(reader.m_strData[MAXINTINITACC_ACTION_CONFIRM][j].size() < 1) continue;
		if(reader.m_strData[MAXINTINITACC_ACTION_CONFIRM][j][0].size() < crMAXINTINITACC_ACTION_CONFIRM_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION_CONFIRM][j][0][crMAXINTINITACC_ACTION_CONFIRM], &action.nConfirm)) continue;
		action.nConfirm = max(action.nConfirm,1);	// MINIMUM 1

		// ** ACTION_CONFIRM_EVENT ** READING
		for(size_t i = 0; i < reader.m_strData[MAXINTINITACC_ACTION_CONFIRM_EVENT][j].size(); i++)
		{
			INT_ACTION_EVENT ev;
			if(reader.m_strData[MAXINTINITACC_ACTION_CONFIRM_EVENT][j][i].size() < crMAXINTINITACC_ACTION_CONFIRM_EVENT_SIZE) continue;
			// Obligatory data
			if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION_CONFIRM_EVENT][j][i][crMAXINTINITACC_ACTION_CONFIRM_EVENT_TYPESEND], &ev.nTypeSend)) continue;
			if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION_CONFIRM_EVENT][j][i][crMAXINTINITACC_ACTION_CONFIRM_EVENT_INDEXSEND], &ev.nSend)) continue;
			if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION_CONFIRM_EVENT][j][i][crMAXINTINITACC_ACTION_CONFIRM_EVENT_SENDID], &ev.send.nId)) continue;
			if(!tx_int(reader.m_strData[MAXINTINITACC_ACTION_CONFIRM_EVENT][j][i][crMAXINTINITACC_ACTION_CONFIRM_EVENT_SENDSUBID], &ev.send.nSubId)) continue;

			__int64 t;
			if(!tx_int64(reader.m_strData[MAXINTINITACC_ACTION_CONFIRM_EVENT][j][i][crMAXINTINITACC_ACTION_CONFIRM_EVENT_TIME], &t)) continue;

			ev.dwTime = (DWORD)t;
			action.events.push_back(ev);
		}
		m_action.push_back(action);
	}

	CString strFileLuaInit;

	// ** LUA_INIT ** READING
	if(reader.m_strData[MAXINTINITACC_LUA_INIT].size() < 1) goto omit_lua_init_reading;
	if(reader.m_strData[MAXINTINITACC_LUA_INIT][0].size() < 1) goto omit_lua_init_reading;
	if(reader.m_strData[MAXINTINITACC_LUA_INIT][0][0].size() < crMAXINTINITACC_LUA_INIT_SIZE)  goto omit_lua_init_reading;

	strFileLuaInit = strDir
		+reader.m_strData[MAXINTINITACC_LUA_INIT][0][0][crMAXINTINITACC_LUA_INIT_FILE];

omit_lua_init_reading:

	CString strFileLuaEvent;

	// ** LUA_EVENT ** READING
	if(reader.m_strData[MAXINTINITACC_LUA_EVENT].size() < 1) goto omit_lua_event_reading;
	if(reader.m_strData[MAXINTINITACC_LUA_EVENT][0].size() < 1) goto omit_lua_event_reading;
	if(reader.m_strData[MAXINTINITACC_LUA_EVENT][0][0].size() < crMAXINTINITACC_LUA_EVENT_SIZE)  goto omit_lua_event_reading;

	strFileLuaEvent = strDir
		+reader.m_strData[MAXINTINITACC_LUA_EVENT][0][0][crMAXINTINITACC_LUA_EVENT_FILE];

omit_lua_event_reading:

	// Add basic resource attachment
	m_ires.resize(1);
	m_ires[0] = new CSpriteRes();
	m_vres.resize(1);
	m_vres[0] = new CVideoRes();
	m_sres.resize(1);
	m_sres[0] = new CSoundRes();
	m_txt.resize(1);
	m_ift.resize(1);

	// Now, we'll execute the lua for init
	if(!LuaInit(strFileLuaInit))
		_printf(_T("(ERROR): CMaxInterface::InitCommon: Sorry, cannot find LUA init file: ")STROP, LPCTSTR(strFileLuaInit));

	// Load resources/menus/screens
	for(size_t j = 0; j < m_strAuxFileRes.size(); j++)
	{
		if(!LoadResources(m_strAuxFileRes[j]))
			_printf(_T("(ERROR): CMaxInterface::InitCommon: Sorry, cannot find resource ACC file: ")STROP, LPCTSTR(m_strAuxFileRes[j]));
	}

	for(size_t j = 0; j < m_strAuxFileMenus.size(); j++)
	{
		if(!LoadMenus(m_strAuxFileMenus[j]))
			_printf(_T("(ERROR): CMaxInterface::InitCommon: Sorry, cannot find menus ACC file: ")STROP, LPCTSTR(m_strAuxFileMenus[j]));
	}

	for(size_t j = 0; j < m_strAuxFileScreens.size(); j++)
	{
		if(!LoadScreens(m_strAuxFileScreens[j]))
			_printf(_T("Sorry, cannot find screens ACC file: ")STROP, LPCTSTR(m_strAuxFileScreens[j]));
	}

	m_lua->Reset();
	if(!m_lua->OpenFile(strFileLuaEvent))
		_printf(_T("(ERROR): CMaxInterface::InitCommon: Sorry, cannot find LUA event file: ")STROP, LPCTSTR(strFileLuaEvent));
	else
	{
		MaxIntRegister_LUA_EVENT(m_lua);
		m_bLuaEvent = true;
	}

	UpdateResources();
	Event(MID(MAXINT_EVENT_RECV, MAXINT_EVENT_CREATE));
	return 1;
}

void CMaxInterface::ResetCommon(void)
{
	// Reset Status data
	m_nTime = 0;
	m_evToSend.clear();

	for(size_t i = 0; i < m_menu.size(); i++)
		m_menu[i]->Exit();

	for(size_t i = 0; i < m_screen.size(); i++)
		m_screen[i]->Reset();

	Event(MID(MAXINT_EVENT_RECV, MAXINT_EVENT_RESET));
}

void CMaxInterface::ReleaseCommon(void)
{
	Event(MID(MAXINT_EVENT_RECV, MAXINT_EVENT_EXIT));
	// Release all
	for(size_t i = 0; i < m_ires.size(); i++)
	{m_ires[i]->Clear(); delete m_ires[i];}
	m_ires.clear();
	for(size_t i = 0; i < m_vres.size(); i++)
	{m_vres[i]->Clear(); delete m_vres[i];}
	m_vres.clear();
	for(size_t i = 0; i < m_sres.size(); i++)
	{m_sres[i]->Clear(); delete m_sres[i];}
	m_sres.clear();
	for(size_t i = 0; i < m_fnt.size(); i++)
	{m_fnt[i]->Delete(); delete m_fnt[i];}
	m_fnt.clear();
	for(size_t i = 0; i < m_screen.size(); i++)
	{m_screen[i]->Delete(); delete m_screen[i];}
	m_screen.clear();
	for(size_t i = 0; i < m_menu.size(); i++)
	{m_menu[i]->Delete(); delete m_menu[i];}
	m_menu.clear();
	for(size_t i = 0; i < m_txt.size(); i++)
	{
		for(size_t j = 0; j < m_txt[i].size(); j++) delete m_txt[i][j];
		m_txt[i].clear();
	}
	m_txt.clear();

	m_callback.clear();

	for(size_t i = 0; i < m_action.size(); i++)
	{m_action[i].events.clear();}
	m_action.clear();

	delete m_lua;
	m_lua = NULL;
}

enum MAXINTRESOURCE_READER
{
	MAXINTRESOURCE_SPRITE,
	MAXINTRESOURCE_VIDEO,
	MAXINTRESOURCE_SOUND,
	MAXINTRESOURCE_FONT,
	MAXINTRESOURCE_STRING,
	MAXINTRESOURCE_SIZE
};

enum crMAXINTRESOURCE_SPRITE_ITEMS
{
	crMAXINTRESOURCE_SPRITE_FILE,
	crMAXINTRESOURCE_SPRITE_XDIV,
	crMAXINTRESOURCE_SPRITE_YDIV,
	crMAXINTRESOURCE_SPRITE_DURATION,
	crMAXINTRESOURCE_SPRITE_SIZE
};

enum crMAXINTRESOURCE_VIDEO_ITEMS
{
	crMAXINTRESOURCE_VIDEO_FILE,
	crMAXINTRESOURCE_VIDEO_LOOP,
	crMAXINTRESOURCE_VIDEO_SIZE
};

enum crMAXINTRESOURCE_SOUND_ITEMS
{
	crMAXINTRESOURCE_SOUND_FILE,
	crMAXINTRESOURCE_SOUND_LOOP,
	crMAXINTRESOURCE_SOUND_SIZE
};

enum crMAXINTRESOURCE_FONT_ITEMS
{
	crMAXINTRESOURCE_FONT_FILE,
	crMAXINTRESOURCE_FONT_SIZE
};

enum crMAXINTRESOURCE_STRING_ITEMS
{
	crMAXINTRESOURCE_STRING_STRING,
	crMAXINTRESOURCE_STRING_IFT,
	crMAXINTRESOURCE_STRING_SIZE
};

int CMaxInterface::LuaInitCommon(LPCTSTR strName)
{
	MaxIntSetCallbackHandle(this);
	if(!m_lua->Init()) return 0;
	MaxIntRegister_LUA_INIT(m_lua);
	if(!m_lua->OpenFile(strName)) return 0;
	if(!m_lua->RunScript()) return 0;
	return 1;
}

int CMaxInterface::LoadResourcesCommon(LPCTSTR strName)
{
	CAccReader reader;
	int nDep = 0;

	CString strDir;
	GetDirStr(CString(strName), &strDir);

	// Set the format
	// ** CMaxInterface Init public parameters
	reader.AddAttrib(_T("SPRITE"), ACCFORMAT_NORMAL, true);
	reader.AddAttrib(_T("VIDEO"), ACCFORMAT_NORMAL, true);
	reader.AddAttrib(_T("SOUND"), ACCFORMAT_NORMAL, true);
	reader.AddAttrib(_T("FONT"), ACCFORMAT_NORMAL, true);
	reader.AddAttrib(_T("STRING"), ACCFORMAT_NORMAL, true);

	// ** Asegurate good atachment
	if(reader.m_nSizeAttrib != MAXINTRESOURCE_SIZE) return 0;

	// ** Read the acc
	if(!reader.Read(strName)) return 0;

	// ** SPRITE **
	for(size_t j = 0; j < reader.m_strData[MAXINTRESOURCE_SPRITE].size(); j++)
	{
		for(size_t i = 0; i < reader.m_strData[MAXINTRESOURCE_SPRITE][j].size(); i++)
		{
			if(reader.m_strData[MAXINTRESOURCE_SPRITE][j][i].size() < crMAXINTRESOURCE_SPRITE_SIZE) continue;

			CString strNewFile = reader.m_strData[MAXINTRESOURCE_SPRITE][j][i][crMAXINTRESOURCE_SPRITE_FILE];

			int nX=1, nY=1;

			if(!tx_int(reader.m_strData[MAXINTRESOURCE_SPRITE][j][i][crMAXINTRESOURCE_SPRITE_XDIV], &nX)) continue;
			if(!tx_int(reader.m_strData[MAXINTRESOURCE_SPRITE][j][i][crMAXINTRESOURCE_SPRITE_YDIV], &nY)) continue;

			__int64 dwDuration;
			if(!tx_int64(reader.m_strData[MAXINTRESOURCE_SPRITE][j][i][crMAXINTRESOURCE_SPRITE_DURATION], &dwDuration)) continue;

			m_ires[0]->AddSprite(strDir+strNewFile, nX, nY);
			m_ires[0]->At(m_ires[0]->GetSize()-1)->SetDuration((DWORD)dwDuration);
		}
	}

	// ** VIDEO **
	for(size_t j = 0; j < reader.m_strData[MAXINTRESOURCE_VIDEO].size(); j++)
	{
		for(size_t i = 0; i < reader.m_strData[MAXINTRESOURCE_VIDEO][j].size(); i++)
		{
			if(reader.m_strData[MAXINTRESOURCE_VIDEO][j][i].size() < crMAXINTRESOURCE_VIDEO_SIZE) continue;

			CString strNewFile = reader.m_strData[MAXINTRESOURCE_VIDEO][j][i][crMAXINTRESOURCE_VIDEO_FILE];
			bool bLoop = false;
			if(!tx_bool(reader.m_strData[MAXINTRESOURCE_VIDEO][j][i][crMAXINTRESOURCE_VIDEO_LOOP], &bLoop)) continue;

			m_vres[0]->AddVideo(strDir+strNewFile, bLoop);
		}
	}

	// ** SOUND **
	for(size_t j = 0; j < reader.m_strData[MAXINTRESOURCE_SOUND].size(); j++)
	{
		for(size_t i = 0; i < reader.m_strData[MAXINTRESOURCE_SOUND][j].size(); i++)
		{
			if(reader.m_strData[MAXINTRESOURCE_SOUND][j][i].size() < crMAXINTRESOURCE_SOUND_SIZE) continue;

			CString strNewFile = reader.m_strData[MAXINTRESOURCE_SOUND][j][i][crMAXINTRESOURCE_SOUND_FILE];
			bool bLoop = false;
			if(!tx_bool(reader.m_strData[MAXINTRESOURCE_SOUND][j][i][crMAXINTRESOURCE_SOUND_LOOP], &bLoop)) continue;

			m_sres[0]->AddSound(strDir+strNewFile, bLoop?PMS_CREATESTREAM_LOOP:0);
		}
	}

	// ** FONT **
	for(size_t j = 0; j < reader.m_strData[MAXINTRESOURCE_FONT].size(); j++)
	{
		for(size_t i = 0; i < reader.m_strData[MAXINTRESOURCE_FONT][j].size(); i++)
		{
			if(reader.m_strData[MAXINTRESOURCE_FONT][j][i].size() < crMAXINTRESOURCE_FONT_SIZE) continue;

			CString strNewFile = reader.m_strData[MAXINTRESOURCE_FONT][j][i][crMAXINTRESOURCE_FONT_FILE];

			CFont2* font = new CFont2();
			CString strFileResult = strDir+strNewFile;
			if(!font->LoadFont(strFileResult))
			{delete font; continue;}

			m_fnt.push_back(font);
		}
	}

	// ** STRING **
	for(size_t j = 0; j < reader.m_strData[MAXINTRESOURCE_STRING].size(); j++)
	{
		for(size_t i = 0; i < reader.m_strData[MAXINTRESOURCE_STRING][j].size(); i++)
		{
			if(reader.m_strData[MAXINTRESOURCE_STRING][j][i].size() < crMAXINTRESOURCE_STRING_SIZE) continue;

			int nIft = 0;
			if(!tx_int(reader.m_strData[MAXINTRESOURCE_STRING][j][i][crMAXINTRESOURCE_STRING_IFT], &nIft)) continue;

			CString* str = new CString(reader.m_strData[MAXINTRESOURCE_STRING][j][i][crMAXINTRESOURCE_STRING_STRING]);

			m_txt[0].push_back(str);
			m_ift[0].push_back(nIft);
		}
	}

	return 1;
}

int CMaxInterface::LoadScreensCommon(LPCTSTR strName)
{
	CMaxScreen* screen = new CMaxScreen();
	if(!screen->OpenFromAcc(strName))
	{ delete screen; return 0;}
	m_screen.push_back(screen);
	return 1;
}

int CMaxInterface::LoadMenusCommon(LPCTSTR strName)
{
	CMaxMenu* menu = new CMaxMenu();
	if(!menu->OpenFromAcc(strName))
	{ delete menu; return 0;}

	// IMPORTANT: Put our callback function in each menu
	menu->m_callback.push_back(MaxIntCallback);
	m_menu.push_back(menu);
	return 1;
}

