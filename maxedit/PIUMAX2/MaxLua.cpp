
#include "MaxLua.h"

CMaxLua::CMaxLua(void)
{
	L = NULL;
}

CMaxLua::~CMaxLua(void)
{
	Destroy();
}

// Methods
int CMaxLua::Init(void)
{
	if(L == NULL)
	{
		L = lua_open();

		if(L == NULL) return 0;

		/* load various Lua libraries */
		luaL_openlibs(L);
	}
	return 1;
}

void CMaxLua::Destroy(void)
{
	if(L != NULL)
	{
		ResetStack();
		lua_close(L);
		L = NULL;
	}
}

int CMaxLua::Reset(void)
{
	Destroy();
	return Init();
}

int CMaxLua::OpenFile(LPCTSTR strFile)
{
	// Temporally, we use the internal "open file" of lua
#ifdef UNICODE
	LPSTR chAnsiFile;
	UnicodeToAnsi(strFile, &chAnsiFile);
	int status = luaL_loadfile(L, chAnsiFile);
	free(chAnsiFile);
	chAnsiFile = NULL;
#else
	int status = luaL_loadfile(L, strFile);
#endif
	
    if (status) {
        /* If something went wrong, error message is at the top of */
        /* the stack */
		_printf(_T("MAXLUA: Couldn't load file: %s\n"), lua_tostring(L, -1));
		lua_pop(L, 1);
        return 0;
    }
	return 1;
}

int CMaxLua::RunScript(void)
{
	int result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result) {
		_printf(_T("MAXLUA: Failed to run script: %s\n"), lua_tostring(L, -1));
        return 0;
    }
	return 1;
}

int CMaxLua::RunScript(int nArgs, int nResults)
{
	int result = lua_pcall(L, nArgs, nResults, 0);
    if (result) {
		_printf(_T("MAXLUA: Failed to run script: %s\n"), lua_tostring(L, -1));
        return 0;
    }
	return 1;
}

void CMaxLua::ResetStack(void)
{
	lua_settop(L, 0);
}
