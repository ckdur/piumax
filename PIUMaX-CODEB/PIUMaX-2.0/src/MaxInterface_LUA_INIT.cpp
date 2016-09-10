// MaxInterface_LUA_INIT.cpp
// Made by: CK-Dur
/*
This file includes definitions about common loading functions IN LUA
*/

#include "MaxInterface.h"
#include "AccReader.h"
#include "basutil.h"
#include "SoundSys.h"

CMaxInterface* g_MaxIntHandle;

int __stdcall MaxIntCallback(MID id, MID arg)
{return g_MaxIntHandle->MenuCallback(id, arg);}

int MaxIntSetCallbackHandle(CMaxInterface* maxIntHandle)
{if((g_MaxIntHandle = maxIntHandle) == NULL) return 0; return 1;}

int MaxIntAddSprite(lua_State* L) // MaxIntAddSprite(string strFile, number nX, number nY, number duration)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 4) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddSprite, 4 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddSprite, argument 1 must be STRING\n")); return 0;}
	if(!lua_isnumber(L, 2))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddSprite, argument 2 must be NUMBER\n")); return 0;}
	if(!lua_isnumber(L, 3))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddSprite, argument 3 must be NUMBER\n")); return 0;}
	if(!lua_isnumber(L, 4))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddSprite, argument 4 must be NUMBER\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif
	
	int nX = (int)lua_tonumber(L, 2), nY = (int)lua_tonumber(L, 3);
	DWORD dwDuration = (DWORD)lua_tonumber(L, 4);

	// ** Load the Sprite and AddIt
	g_MaxIntHandle->m_ires[0]->AddSprite(tcsFile, nX, nY);
	g_MaxIntHandle->m_ires[0]->At(g_MaxIntHandle->m_ires[0]->GetSize())->SetDuration((DWORD)dwDuration);
	
	return 0;
}

int MaxIntInsertSprite(lua_State* L) // MaxIntInsertSprite(string strFile, number nX, number nY, number duration, number nPos)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 5) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSprite, 4 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSprite, argument 1 must be STRING\n")); return 0;}
	if(!lua_isnumber(L, 2))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSprite, argument 2 must be NUMBER\n")); return 0;}
	if(!lua_isnumber(L, 3))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSprite, argument 3 must be NUMBER\n")); return 0;}
	if(!lua_isnumber(L, 4))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSprite, argument 4 must be NUMBER\n")); return 0;}
	if(!lua_isnumber(L, 5))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSprite, argument 5 must be NUMBER\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif
	
	int nX = (int)lua_tonumber(L, 2), nY = (int)lua_tonumber(L, 3);
	DWORD dwDuration = (DWORD)lua_tonumber(L, 4);
	int nPos = (int)lua_tonumber(L, 5);

	// ** Load the Sprite and Insert
	if(!g_MaxIntHandle->m_ires[0]->InsertSprite(tcsFile, nX, nY, nPos))
	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSprite, CSpriteRes has returned ERROR\n")); return 0;}
	
	g_MaxIntHandle->m_ires[0]->At(nPos)->SetDuration((DWORD)dwDuration);
	return 0;
}

int MaxIntDeleteSprite(lua_State* L) // MaxIntDeleteSprite(number nPos)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSprite, 1 argument required\n")); return 0;	}
	if(!lua_isnumber(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSprite, argument 1 must be NUMBER\n")); return 0;}

	int nPos = (int)lua_tonumber(L, 1);

	// ** Delete the sprite at nPos
	g_MaxIntHandle->m_ires[0]->DeleteSprite(nPos);
	
	return 0;
}

int MaxIntClearSprite(lua_State* L) // MaxIntClearSprite()
{
	// ** Clear Sprites
	g_MaxIntHandle->m_ires[0]->Clear();
	
	return 0;
}

int MaxIntGetSizeSprite(lua_State* L) // number MaxIntGetSizeSprite()
{
	// ** Get Size of sprites
	int nCount = g_MaxIntHandle->m_ires[0]->GetSize();

	lua_pushinteger(L, nCount);
	
	return 1;
}

int MaxIntAddVideo(lua_State* L) // MaxIntAddVideo(string strFile, boolean bLoop)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 2) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddVideo, 2 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddVideo, argument 1 must be STRING\n")); return 0;}
	if(!lua_isboolean(L, 2)) {_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertVideo, argument 2 must be BOOLEAN\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif

	int nLoop = lua_toboolean(L, 2);
	// ** Load the Video and AddIt
	g_MaxIntHandle->m_vres[0]->AddVideo(tcsFile, nLoop?true:false);
	
	return 0;
}

int MaxIntInsertVideo(lua_State* L) // MaxIntInsertVideo(string strFile, boolean bLoop, number nPos)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 3) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertVideo, 3 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertVideo, argument 1 must be STRING\n")); return 0;}
	if(!lua_isboolean(L, 2)) {_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertVideo, argument 2 must be BOOLEAN\n")); return 0;}
	if(!lua_isnumber(L, 3)) {_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertVideo, argument 3 must be NUMBER\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif
	
	int nLoop = lua_toboolean(L, 2);
	int nPos = (int)lua_tonumber(L, 3);

	// ** Load the Video and Insert
	if(!g_MaxIntHandle->m_vres[0]->InsertVideo(tcsFile, nLoop?true:false, nPos))
	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertVideo, CVideoRes has returned ERROR\n")); return 0;}
	
	return 0;
}

int MaxIntDeleteVideo(lua_State* L) // MaxIntDeleteVideo(number nPos)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertVideo, 1 argument required\n")); return 0;	}
	if(!lua_isnumber(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertVideo, argument 1 must be NUMBER\n")); return 0;}

	int nPos = (int)lua_tonumber(L, 1);

	// ** Delete the sprite at nPos
	g_MaxIntHandle->m_vres[0]->DeleteVideo(nPos);
	
	return 0;
}

int MaxIntClearVideo(lua_State* L) // MaxIntClearVideo()
{
	// ** Clear Videos
	g_MaxIntHandle->m_vres[0]->Clear();
	
	return 0;
}

int MaxIntGetSizeVideo(lua_State* L) // number MaxIntGetSizeVideo()
{
	// ** Get Size of Videos
	int nCount = g_MaxIntHandle->m_vres[0]->GetSize();

	lua_pushinteger(L, nCount);
	
	return 1;
}

int MaxIntAddSound(lua_State* L) // MaxIntAddSound(string strFile, boolean bLoop)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 2) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddSound, 2 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddSound, argument 1 must be STRING\n")); return 0;}
	if(!lua_isboolean(L, 2)) {_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSound, argument 2 must be BOOLEAN\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif

	int nLoop = lua_toboolean(L, 2);
	// ** Load the Sound and AddIt
	g_MaxIntHandle->m_sres[0]->AddSound(tcsFile, nLoop?PMS_CREATESTREAM_LOOP:0);
	
	return 0;
}

int MaxIntInsertSound(lua_State* L) // MaxIntInsertSound(string strFile, boolean bLoop, number nPos)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 3) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSound, 3 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSound, argument 1 must be STRING\n")); return 0;}
	if(!lua_isboolean(L, 2)) {_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSound, argument 2 must be BOOLEAN\n")); return 0;}
	if(!lua_isnumber(L, 3)) {_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSound, argument 3 must be NUMBER\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif
	
	int nLoop = lua_toboolean(L, 2);
	int nPos = (int)lua_tonumber(L, 3);

	// ** Load the Sound and Insert
	if(!g_MaxIntHandle->m_sres[0]->InsertSound(tcsFile, nLoop?PMS_CREATESTREAM_LOOP:0, nPos))
	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSound, CSoundRes has returned ERROR\n")); return 0;}
	
	return 0;
}

int MaxIntDeleteSound(lua_State* L) // MaxIntAddSound(number nPos)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSound, 1 argument required\n")); return 0;	}
	if(!lua_isnumber(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertSound, argument 1 must be NUMBER\n")); return 0;}

	int nPos = (int)lua_tonumber(L, 1);

	// ** Delete the sprite at nPos
	g_MaxIntHandle->m_sres[0]->DeleteSound(nPos);
	
	return 0;
}

int MaxIntClearSound(lua_State* L) // MaxIntClearSound()
{
	// ** Clear Sounds
	g_MaxIntHandle->m_sres[0]->Clear();
	
	return 0;
}

int MaxIntGetSizeSound(lua_State* L) // number MaxIntGetSizeSound()
{
	// ** Get Size of Sounds
	int nCount = g_MaxIntHandle->m_sres[0]->GetSize();

	lua_pushinteger(L, nCount);
	
	return 1;
}

int MaxIntAddFont(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddFont, 1 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddFont, argument 1 must be STRING\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif

	// ** Load the Font and AddIt
	CFont2* font = new CFont2();
	if(!font->LoadFont(tcsFile))  { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddFont, especified file not found or not loaded properly\n")); return 0;	}
	g_MaxIntHandle->m_fnt.push_back(font);
	
	return 0;
}

int MaxIntInsertFont(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 2) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertFont, 2 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertFont, argument 1 must be STRING\n")); return 0;}
	if(!lua_isnumber(L, 2)) {_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertFont, argument 2 must be NUMBER\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif
	
	size_t nPos = (size_t)lua_tonumber(L, 2);
	if(nPos > g_MaxIntHandle->m_fnt.size())  { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddFont, try to insert in a invalid position (%d)"), nPos); return 0;	}

	// ** Load the Font and Insert
	CFont2* font = new CFont2();
	if(!font->LoadFont(tcsFile))  { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddFont, especified file not found or not loaded properly\n")); return 0;	}
	vector< CFont2* >::iterator it;
	it = g_MaxIntHandle->m_fnt.begin() + nPos;
	g_MaxIntHandle->m_fnt.insert(it, font);
	
	return 0;
}

int MaxIntDeleteFont(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertFont, 1 argument required\n")); return 0;	}
	if(!lua_isnumber(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertFont, argument 1 must be NUMBER\n")); return 0;}

	size_t nPos = (size_t)lua_tonumber(L, 1);

	// ** Delete the sprite at nPos
	vector< CFont2* >::iterator it;
	it = g_MaxIntHandle->m_fnt.begin() + nPos;

	(*it)->Delete();
	delete (*it);

	g_MaxIntHandle->m_fnt.erase(it);
	
	return 0;
}

int MaxIntClearFont(lua_State* L)
{
	// ** Clear Fonts
	vector< CFont2* >::iterator it;
	for(it = g_MaxIntHandle->m_fnt.begin(); it != g_MaxIntHandle->m_fnt.end(); it ++)
	{
		(*it)->Delete();
		delete (*it);
	}

	g_MaxIntHandle->m_fnt.clear();
	
	return 0;
}

int MaxIntGetSizeFont(lua_State* L)
{
	// ** Get Size of Fonts
	size_t nCount = g_MaxIntHandle->m_fnt.size();

	lua_pushinteger(L, (lua_Integer)nCount);
	
	return 1;
}

int MaxIntAddString(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 2) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddString, 2 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddString, argument 1 must be STRING\n")); return 0;}
	if(!lua_isnumber(L, 2))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddString, argument 2 must be NUMBER\n")); return 0;}

	TCHAR tcs[MAX_PATH];
	LPCSTR str = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(str, &tcs);
#else
	strcpy(tcs, str);
#endif

	int nIft = (int)lua_tonumber(L, 2);
	// ** Load the String and AddIt
	CString* string = new CString(tcs);
	g_MaxIntHandle->m_txt[0].push_back(string);
	g_MaxIntHandle->m_ift[0].push_back(nIft);
	
	return 0;
}

int MaxIntInsertString(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 3) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertString, 3 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertString, argument 1 must be STRING\n")); return 0;}
	if(!lua_isnumber(L, 2)) {_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertString, argument 2 must be NUMBER\n")); return 0;}
	if(!lua_isnumber(L, 3))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertString, argument 3 must be NUMBER\n")); return 0;}

	TCHAR tcs[MAX_PATH];
	LPCSTR str = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(str, &tcs);
#else
	strcpy(tcs, str);
#endif
	
	size_t nPos = (size_t)lua_tonumber(L, 2);
	if(nPos > g_MaxIntHandle->m_txt[0].size())  { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddString, try to insert in a invalid position (%d)"), nPos); return 0;	}

	// ** Load the String and Insert
	CString* string = new CString(tcs);
	vector< CString* >::iterator it;
	it = g_MaxIntHandle->m_txt[0].begin() + nPos;
	g_MaxIntHandle->m_txt[0].insert(it, string);

	int nIft = (int)lua_tonumber(L, 3);
	vector< int >::iterator it2;
	it2 = g_MaxIntHandle->m_ift[0].begin() + nPos;
	g_MaxIntHandle->m_ift[0].insert(it2, nIft);
	
	return 0;
}

int MaxIntDeleteString(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertString, 1 argument required\n")); return 0;	}
	if(!lua_isnumber(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertString, argument 1 must be NUMBER\n")); return 0;}

	size_t nPos = (size_t)lua_tonumber(L, 1);

	// ** Delete the String at nPos
	vector< CString* >::iterator it;
	it = g_MaxIntHandle->m_txt[0].begin() + nPos;
	delete (*it);
	g_MaxIntHandle->m_txt[0].erase(it);

	vector< int >::iterator it2;
	it2 = g_MaxIntHandle->m_ift[0].begin() + nPos;
	g_MaxIntHandle->m_ift[0].erase(it2);
	
	return 0;
}

int MaxIntClearString(lua_State* L)
{
	// ** Clear Strings
	vector< vector< CString* > >::iterator it;
	for(it = g_MaxIntHandle->m_txt.begin(); it != g_MaxIntHandle->m_txt.end(); it ++)
	{
		vector< CString* >::iterator it2;
		for(it2 = (*it).begin(); it2 != (*it).end(); it ++)
		{
			delete (*it2);
		}
		(*it).clear();
	}

	g_MaxIntHandle->m_txt[0].clear();
	g_MaxIntHandle->m_ift[0].clear();
	
	return 0;
}

int MaxIntGetSizeString(lua_State* L)
{
	// ** Get Size of Strings
	size_t nCount = g_MaxIntHandle->m_txt[0].size();

	lua_pushinteger(L, (lua_Integer)nCount);
	
	return 1;
}

int MaxIntAddScreen(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddScreen, 1 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddScreen, argument 1 must be STRING\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif

	// ** Load the Screen and AddIt
	CMaxScreen* screen = new CMaxScreen();
	if(!screen->OpenFromAcc(tcsFile))  { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddScreen, especified file not found or not loaded properly\n")); return 0;	}
	g_MaxIntHandle->m_screen.push_back(screen);
	
	return 0;
}

int MaxIntInsertScreen(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 2) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertScreen, 2 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertScreen, argument 1 must be STRING\n")); return 0;}
	if(!lua_isnumber(L, 2)) {_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertScreen, argument 2 must be NUMBER\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif
	
	size_t nPos = (size_t)lua_tonumber(L, 2);
	if(nPos > g_MaxIntHandle->m_screen.size())  { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddScreen, try to insert in a invalid position (%d)"), nPos); return 0;	}

	// ** Load the Screen and Insert
	CMaxScreen* screen = new CMaxScreen();
	if(!screen->OpenFromAcc(tcsFile))  { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddScreen, especified file not found or not loaded properly\n")); return 0;	}
	vector< CMaxScreen* >::iterator it;
	it = g_MaxIntHandle->m_screen.begin() + nPos;
	g_MaxIntHandle->m_screen.insert(it, screen);
	
	return 0;
}

int MaxIntDeleteScreen(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertScreen, 1 argument required\n")); return 0;	}
	if(!lua_isnumber(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertScreen, argument 1 must be NUMBER\n")); return 0;}

	size_t nPos = (size_t)lua_tonumber(L, 1);

	// ** Delete the sprite at nPos
	vector< CMaxScreen* >::iterator it;
	it = g_MaxIntHandle->m_screen.begin() + nPos;
	(*it)->Delete();
	delete (*it);
	g_MaxIntHandle->m_screen.erase(it);
	
	return 0;
}

int MaxIntClearScreen(lua_State* L)
{
	// ** Clear Screens
	vector< CMaxScreen* >::iterator it;
	for(it = g_MaxIntHandle->m_screen.begin(); it != g_MaxIntHandle->m_screen.end(); it ++)
	{
		(*it)->Delete();
		delete (*it);
	}

	g_MaxIntHandle->m_screen.clear();
	
	return 0;
}

int MaxIntGetSizeScreen(lua_State* L)
{
	// ** Get Size of Screens
	size_t nCount = g_MaxIntHandle->m_screen.size();

	lua_pushinteger(L, (lua_Integer)nCount);
	
	return 1;
}

int MaxIntAddMenu(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddMenu, 1 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddMenu, argument 1 must be STRING\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif

	// ** Load the Menu and AddIt
	CMaxMenu* menu = new CMaxMenu();
	if(!menu->OpenFromAcc(tcsFile))  { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddMenu, especified file not found or not loaded properly\n")); return 0;	}
	g_MaxIntHandle->m_menu.push_back(menu);
	
	return 0;
}

int MaxIntInsertMenu(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 2) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertMenu, 2 argument required\n")); return 0;	}
	if(!lua_isstring(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertMenu, argument 1 must be STRING\n")); return 0;}
	if(!lua_isnumber(L, 2)) {_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertMenu, argument 2 must be NUMBER\n")); return 0;}

	TCHAR tcsFile[MAX_PATH];
	LPCSTR strFile = lua_tostring(L, 1);
#ifdef UNICODE
	AnsiToUnicode(strFile, &tcsFile);
#else
	strcpy(tcsFile, strFile);
#endif
	
	size_t nPos = (size_t)lua_tonumber(L, 2);
	if(nPos > g_MaxIntHandle->m_menu.size())  { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddMenu, try to insert in a invalid position (%d)"), nPos); return 0;	}

	// ** Load the Menu and Insert
	CMaxMenu* menu = new CMaxMenu();
	if(!menu->OpenFromAcc(tcsFile))  { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntAddMenu, especified file not found or not loaded properly\n")); return 0;	}
	vector< CMaxMenu* >::iterator it;
	it = g_MaxIntHandle->m_menu.begin() + nPos;
	g_MaxIntHandle->m_menu.insert(it, menu);
	
	return 0;
}

int MaxIntDeleteMenu(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertMenu, 1 argument required\n")); return 0;	}
	if(!lua_isnumber(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntInsertMenu, argument 1 must be NUMBER\n")); return 0;}

	size_t nPos = (size_t)lua_tonumber(L, 1);

	// ** Delete the sprite at nPos
	vector< CMaxMenu* >::iterator it;
	it = g_MaxIntHandle->m_menu.begin() + nPos;
	(*it)->Delete();
	delete (*it);
	g_MaxIntHandle->m_menu.erase(it);
	
	return 0;
}

int MaxIntClearMenu(lua_State* L)
{
	// ** Clear Menus
	vector< CMaxMenu* >::iterator it;
	for(it = g_MaxIntHandle->m_menu.begin(); it != g_MaxIntHandle->m_menu.end(); it ++)
	{
		(*it)->Delete();
		delete (*it);
	}

	g_MaxIntHandle->m_menu.clear();
	
	return 0;
}

int MaxIntGetSizeMenu(lua_State* L)
{
	// ** Get Size of Menus
	size_t nCount = g_MaxIntHandle->m_menu.size();

	lua_pushinteger(L, (lua_Integer)nCount);
	
	return 1;
}

#define MaxIntDefineAction_aux_getMID(__name) lua_pushstring(L, #__name); lua_gettable(L, 1); \
	if(lua_isnil(L, -1)) \
	{\
	    _printf(_T("(WARNING) MAXINT_LUA_INIT: MaxIntDefineAction, TABLE of INT_ACTION must have \"")_T(#__name)_T("\" key, set this value to (0,0)\n\n"));\
		action.action = MID(0, 0);\
	}\
	else if (!MLUA_readMID(L, -1, &action.__name)) \
		_printf(_T("(WARNING) MAXINT_LUA_INIT: MaxIntDefineAction, MLUA_readMID returned bad value while modifying '")_T(#__name)_T("' (see below)\n\n"));\
	lua_pop(L, -1)

int MaxIntDefineAction(lua_State* L) // MaxIntDefineAction(INT_ACTION action)
{
	/*
	INT_ACTION IN LUA
	must have MID 'id, action, arg'
	a MID is a table with to int called 'id, subid'

	also, must have a vector of INT_ACTION_EVENT called 'events'
	INT_ACTION_EVENT events[]
	{
		int nTypeSend,		// Type of object to send (0 for MENU, 1 for screen)
		int nSend,			// Object to send (if -1, send to all)
		MID send			// Send info
	}
	*/

	INT_ACTION action;

	// ** Make secure the table
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntDefineAction, 1 argument required\n\n")); return 0;	}
	if(!lua_istable(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntDefineAction, argument 1 must be TABLE\n see the manual for this declaration\n\n")); return 0;}

	// ** Get the MIDs

	// Push 'id' as key, must be TABLE
	MaxIntDefineAction_aux_getMID(id);

	// Push 'action' as key, must be TABLE
	MaxIntDefineAction_aux_getMID(action);

	// Push 'arg' as key, must be TABLE
	MaxIntDefineAction_aux_getMID(arg);

	// Push 'events' as key, must be TABLE ARRAY
	lua_pushstring(L, "events"); lua_gettable(L, 1);

	int n = 0;

	if(!lua_istable(L, -1))
		_printf(_T("(WARNING) MAXINT_LUA_INIT: MaxIntDefineAction, the key 'actions' don't match TABLE\n\n"));
	else if((n = luaL_getn(L, -1)) <= 0)
		_printf(_T("(WARNING) MAXINT_LUA_INIT: MaxIntDefineAction, the key 'actions' don't have elements\n\n"));
	else
	{
		for (int i=1; i<=n; i++) 
		{
			lua_rawgeti (L, -1, i);
			if(!lua_istable(L, -1))
			{
				_printf(_T("(WARNING) MAXINT_LUA_INIT: MaxIntDefineAction, the key 'actions[%d]' don't match TABLE\n"), i);
				lua_pop(L, -1); break;
			}
			INT_ACTION_EVENT ev;
			lua_pushstring(L, "nTypeSend"); lua_gettable(L, -1);
			if(lua_isnumber(L, -1)) 
			{ 
				_printf(_T("(WARNING) MAXINT_LUA_INIT: MaxIntDefineAction, 'actions.nTypeSend' is not NUMBER\n\n")); 
				lua_pop(L, -1); break;
			}
			ev.nTypeSend = (int)lua_tonumber(L, -1);
			lua_pop(L, -1);

			lua_pushstring(L, "nSend"); lua_gettable(L, -1);
			if(lua_isnumber(L, -1)) 
			{ 
				_printf(_T("(WARNING) MAXINT_LUA_INIT: MaxIntDefineAction, 'actions.nSend' is not NUMBER\n\n")); 
				lua_pop(L, -1); break;
			}
			ev.nSend = (int)lua_tonumber(L, -1);
			lua_pop(L, -1);

			lua_pushstring(L, "send"); lua_gettable(L, -1);
			if(!MLUA_readMID(L, -1, &ev.send))
				_printf(_T("(WARNING) MAXINT_LUA_INIT: MaxIntDefineAction, MLUA_readMID returned bad value while modifying 'actions.send' (see below)\n\n"));
		}
	}
	lua_pop(L, -1);

	// SUCESS! we store tne action to the catalog
	g_MaxIntHandle->m_action.push_back(action);

	return 0;
}

int MaxIntUndefineAction(lua_State* L)
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntUndefineAction, 1 argument required\n\n")); return 0;	}
	if(!lua_isnumber(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_INIT: MaxIntUndefineAction, argument 1 must be NUMBER\n\n")); return 0;}

	size_t nPos = (size_t)lua_tonumber(L, 1);

	// ** Delete the action at nPos
	vector< INT_ACTION >::iterator it;
	it = g_MaxIntHandle->m_action.begin() + nPos;
	(*it).events.clear();
	g_MaxIntHandle->m_action.erase(it);
	
	return 0;
}

int MaxIntRegister_LUA_INIT(CMaxLua* lua)
{
	lua_State* L = lua->L;

	LUA_SETFUNCTION_RELATIVE(L, MaxIntAddSprite);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntInsertSprite);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntDeleteSprite);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntClearSprite);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntGetSizeSprite);

	LUA_SETFUNCTION_RELATIVE(L, MaxIntAddVideo);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntInsertVideo);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntDeleteVideo);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntClearVideo);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntGetSizeVideo);

	LUA_SETFUNCTION_RELATIVE(L, MaxIntAddSound);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntInsertSound);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntDeleteSound);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntClearSound);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntGetSizeSound);

	LUA_SETFUNCTION_RELATIVE(L, MaxIntAddFont);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntInsertFont);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntDeleteFont);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntClearFont);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntGetSizeFont);

	LUA_SETFUNCTION_RELATIVE(L, MaxIntAddString);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntInsertString);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntDeleteString);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntClearString);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntGetSizeString);

	LUA_SETFUNCTION_RELATIVE(L, MaxIntAddScreen);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntInsertScreen);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntDeleteScreen);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntClearScreen);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntGetSizeScreen);

	LUA_SETFUNCTION_RELATIVE(L, MaxIntAddMenu);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntInsertMenu);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntDeleteMenu);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntClearMenu);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntGetSizeMenu);

	LUA_SETFUNCTION_RELATIVE(L, MaxIntDefineAction);
	LUA_SETFUNCTION_RELATIVE(L, MaxIntUndefineAction);

	return 1;
}

// ** WARNING
/*
Definitions about "SetMaxInt" that contains the setting method for 
global g_MaxIntHandle Is defined in the Main Theme

Remember that you must use "SetMaxInt" for editing interfaces.
*/