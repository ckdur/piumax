// MaxLuaCommonMethods.cpp
/*
This file contains definitions about common MaxLua Methods
*/

#include "MaxLuaCommonMethods.h"

// *****************************************************************************************
// ** Definition about "int RegisterCommonMethods(CMaxLua* lua)"

int RegisterCommonMethods(CMaxLua* lua)
{
	lua_State* L = lua->L;

	// PIUMaX status
	lua_pushcfunction(L, GetCurrentInterface);			lua_setglobal(L, "GetCurrentInterface");
	lua_pushcfunction(L, GetCurrentDirectory);			lua_setglobal(L, "GetCurrentDirectory");
	lua_pushcfunction(L, GetMachineConfig);				lua_setglobal(L, "GetMachineConfig");
	lua_pushcfunction(L, GetMachineConfigVariable);		lua_setglobal(L, "GetMachineConfigVariable");

	// Player status
	lua_pushcfunction(L, GetPlayers);					lua_setglobal(L, "GetPlayers");
	lua_pushcfunction(L, GetPlayerVariable);			lua_setglobal(L, "GetPlayerVariable");
	lua_pushcfunction(L, IsPlayer);						lua_setglobal(L, "IsPlayer");

	// PIUMaX variable machine registration
	return 1;
}

// PIUMaX status
int GetCurrentInterface(lua_State* lua)
{
	return 0;
}

int GetCurrentDirectory(lua_State* lua)
{
	return 0;
}

int GetMachineConfig(lua_State* lua)
{
	return 0;
}

int GetMachineConfigVariable(lua_State* lua)
{
	return 0;
}

// Player status
int GetPlayers(lua_State* lua)
{
	return 0;
}

int GetPlayerVariable(lua_State* lua)
{
	return 0;
}

int IsPlayer(lua_State* lua)
{
	return 0;
}