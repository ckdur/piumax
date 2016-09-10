#ifndef MAX_MAINTHEME
#define MAX_MAINTHEME

#include "stdafx.h"
#include "MaxInterface.h"

// This struct is used for store Interface info
struct INT_INFO
{
	CString strName;
	CString strFile;
	CString strInternalFile;
	int nType;
};

enum MAXTHEME_INTTYPE
{
	MAXTHEME_INTTYPE_BASIC,
	MAXTHEME_INTTYPE_WELCOME,
	MAXTHEME_INTTYPE_SIZE
};

extern TCHAR* tc_MAXTHEME_INTTYPE[];

class CMainTheme
{
public:
	CMainTheme(void);
	~CMainTheme(void);
	int LoadTheme(LPCTSTR strName);

	CString m_strName;			// Name of the theme
	CString m_strDescription;	// Description
	int m_nVersion[4];			// Version [0].[1].[2].[3]

	double m_dScreenWidth, m_dScreenHeight;	// Size of the screen

	vector< INT_INFO > m_int_info;

	vector< INT_ACTION > m_action;			// Actions about event reciving

	vector< CMaxInterface* > m_int;			// Active interfaces
	vector< int > m_nActiveInt;				// List of actived interfaces

	CMaxLua* m_lua;							// Lua interface

	// Definition of methods
	void Draw(void);
	int Event(MID ev);
};

#endif /*MAX_MAINTHEME*/