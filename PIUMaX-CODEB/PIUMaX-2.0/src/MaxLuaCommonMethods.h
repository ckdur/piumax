// MaxLuaCommonMethods.h
// Declares all common methods for MaxLua and it's home of "RegisterCommonMethods"

#include "basutil.h"
#include "cmnpiu.h"
#include "MaxLua.h"

// Define common methods

// PIUMaX status
int GetCurrentInterface(lua_State* lua);
int GetCurrentDirectory(lua_State* lua);
int GetMachineConfig(lua_State* lua);
int GetMachineConfigVariable(lua_State* lua);

// Player status
int GetPlayers(lua_State* lua);
int GetPlayerVariable(lua_State* lua);
int IsPlayer(lua_State* lua);

// PIUMaX variable machine registration

// Our "RegisterCommonMethods"

int RegisterCommonMethods(CMaxLua* lua);