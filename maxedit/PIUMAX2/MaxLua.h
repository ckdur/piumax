#ifndef PIUMAX_MAXLUA
#define PIUMAX_MAXLUA

#include "stdafx.h"
#include "defbas.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>

#ifdef __cplusplus
}
#endif

#ifdef WIN32
#pragma comment( lib, "lua51.lib")
#endif // WIN32

class CMaxLua
{
public:
	CMaxLua(void);
	~CMaxLua(void);
	lua_State* L;

	// Methods
	int Init(void);
	void Destroy(void);
	int Reset(void);

	int OpenFile(LPCTSTR strFile);

	int RunScript(void);
	int RunScript(int nArgs, int nResults);

	void ResetStack(void);
};

#define LUA_SETFUNCTION_RELATIVE(L, __func) lua_pushcfunction(L, __func); lua_setglobal(L, #__func)

enum MAXLUA_TYPES
{
	MAXLUA_TYPE_INT,
	MAXLUA_TYPE_INT64,
	MAXLUA_TYPE_FLOAT,
	MAXLUA_TYPE_DOUBLE,
	MAXLUA_TYPE_BOOL,
	MAXLUA_TYPE_STRING,
	MAXLUA_TYPE_MID
};

#endif // PIUMAX_MAXLUA
