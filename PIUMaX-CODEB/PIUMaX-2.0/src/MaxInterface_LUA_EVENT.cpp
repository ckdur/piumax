// MaxInterface_LUA_INIT.cpp
// Made by: CK-Dur
/*
This file includes definitions about common event functions IN LUA
*/

#include "MaxInterface.h"
#include "AccReader.h"
#include "basutil.h"

int MaxLuaEvent(lua_State* L) // MaxLuaEvent(MID ev);
{
	int nArgs = lua_gettop(L);
	if(nArgs < 1) { _printf(_T("(ERROR) MAXINT_LUA_EVENT: MaxLuaEvent, 1 argument required\n")); return 0;	}
	if(!lua_istable(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_EVENT: MaxLuaEvent, argument 1 must be TABLE\n")); return 0;}

	MID ev;
	MLUA_readMID(L, 1, &ev);

	g_MaxIntHandle->Event(ev);
	
	return 0;				
}

int MaxLuaEventTo(lua_State* L) // MaxLuaEventTo(int nTypeSend, int nSend, DWORD dwTime, MID ev);
{
	int nArgs = lua_gettop(L);
	if(nArgs < 4) { _printf(_T("(ERROR) MAXINT_LUA_EVENT: MaxLuaEventTo, 4 argument required\n")); return 0;	}
	if(!lua_isnumber(L, 1))	{_printf(_T("(ERROR) MAXINT_LUA_EVENT: MaxLuaEventTo, argument 1 must be NUMBER\n")); return 0;}
	if(!lua_isnumber(L, 2))	{_printf(_T("(ERROR) MAXINT_LUA_EVENT: MaxLuaEventTo, argument 2 must be NUMBER\n")); return 0;}
	if(!lua_isnumber(L, 3))	{_printf(_T("(ERROR) MAXINT_LUA_EVENT: MaxLuaEventTo, argument 3 must be NUMBER\n")); return 0;}
	if(!lua_istable(L, 4))	{_printf(_T("(ERROR) MAXINT_LUA_EVENT: MaxLuaEventTo, argument 4 must be TABLE\n")); return 0;}

	INT_ACTION_EVENT send;
	send.nTypeSend = (int)lua_tonumber(L, 1);
	send.nSend = (int)lua_tonumber(L, 2);
	send.dwTime = (DWORD)lua_tonumber(L, 3);

	MID ev;
	MLUA_readMID(L, 4, &ev);

	g_MaxIntHandle->m_evToSend.push_back(send);
	
	return 0;				
}

int MaxIntRegister_LUA_EVENT(CMaxLua* lua)
{
	lua_State* L = lua->L;

	LUA_SETFUNCTION_RELATIVE(L, MaxLuaEvent);
	LUA_SETFUNCTION_RELATIVE(L, MaxLuaEventTo);
	return 1;
}
