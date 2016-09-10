// MaxInterface_3.cpp
// Made by: CK-Dur
/*
Auxiliar, includes extended about basic (only
MaxInterface.cpp: This file includes definitions about common loading functions
MaxInterface_2.cpp: Includes definitions about common functionality about common interface
*/

#include "MaxInterface.h"
#include "AccReader.h"
#include "basutil.h"

int CMaxInterface::Init(LPCTSTR strName) {return InitCommon(strName);}
void CMaxInterface::Reset(void) {ResetCommon();}
void CMaxInterface::Release(void) {ReleaseCommon();}
void CMaxInterface::Draw(void) {DrawCommon();}
void CMaxInterface::Update(DWORD dwMilliseconds) {UpdateCommon(dwMilliseconds);}
int CMaxInterface::Event(MID ev) {return EventCommon(ev);}
int CMaxInterface::MenuCallback(MID id, MID arg) {return MenuCallbackCommon(id, arg);}

int CMaxInterface::LoadResources(LPCTSTR strName) {return LoadResourcesCommon(strName);}
int CMaxInterface::LoadScreens(LPCTSTR strName) {return LoadScreensCommon(strName);}
int CMaxInterface::LoadMenus(LPCTSTR strName) {return LoadMenusCommon(strName);}

MID CMaxInterface::InterfaceStatus(void) {return InterfaceStatusCommon();}

int CMaxInterface::LuaInit(LPCTSTR strName) {return LuaInitCommon(strName);}
int CMaxInterface::LuaEvent(MID	ev) {return LuaEventCommon(ev);}

int CMaxInterface::LookAction(MID ev) {return LookActionCommon(ev);}