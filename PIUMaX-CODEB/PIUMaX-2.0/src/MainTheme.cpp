#include "MainTheme.h"
#include "AccReader.h"

CMainTheme::CMainTheme(void)
{
	m_nVersion[0] = 0;
	m_nVersion[1] = 0;
	m_nVersion[2] = 0;
	m_nVersion[3] = 1;
	m_dScreenWidth = 80.0;
	m_dScreenHeight = 60.0;
}

CMainTheme::~CMainTheme(void)
{
}

enum MAXTHEMEINITACC_READER
{
	MAXTHEMEINITACC_NAME,
	MAXTHEMEINITACC_DESCRIPTION,
	MAXTHEMEINITACC_VERSION,
	MAXTHEMEINITACC_SCREENPROPORTION,
	MAXTHEMEINITACC_INTERFACE,
	MAXTHEMEINITACC_INTERFACETYPE,
	MAXTHEMEINITACC_INTERFACEFILE,
	MAXTHEMEINITACC_ACTION,
	MAXTHEMEINITACC_ACTION_CONFIRM,
	MAXTHEMEINITACC_ACTION_CONFIRM_EVENT,
	MAXTHEMEINITACC_SIZE
};

#define crMAXTHEMEINITACC_NAME_NAME	0
#define crMAXTHEMEINITACC_NAME_SIZE	1

#define crMAXTHEMEINITACC_DESCRIPTION_DESCRIPTION	0
#define crMAXTHEMEINITACC_DESCRIPTION_SIZE	1

#define crMAXTHEMEINITACC_VERSION_VERSION	0
#define crMAXTHEMEINITACC_VERSION_SIZE	4

#define crMAXTHEMEINITACC_SCREENPROPORTION_WIDTH	0
#define crMAXTHEMEINITACC_SCREENPROPORTION_HEIGHT	1
#define crMAXTHEMEINITACC_SCREENPROPORTION_SIZE	2

#define crMAXTHEMEINITACC_INTERFACE_INTERFACE	0
#define crMAXTHEMEINITACC_INTERFACE_SIZE	1

#define crMAXTHEMEINITACC_INTERFACETYPE_INTERFACETYPE	0
#define crMAXTHEMEINITACC_INTERFACETYPE_SIZE	1

#define crMAXTHEMEINITACC_INTERFACEFILE_INTERFACEFILE	0
#define crMAXTHEMEINITACC_INTERFACEFILE_SIZE	1

#define crMAXTHEMEINITACC_ACTION_ID				0
#define crMAXTHEMEINITACC_ACTION_SUBID			1
#define crMAXTHEMEINITACC_ACTION_ACTIONID			2
#define crMAXTHEMEINITACC_ACTION_ACTIONSUBID		3
#define crMAXTHEMEINITACC_ACTION_ARGID			4
#define crMAXTHEMEINITACC_ACTION_ARGSUBID			5
#define crMAXTHEMEINITACC_ACTION_SIZE				6

#define crMAXTHEMEINITACC_ACTION_CONFIRM		0
#define crMAXTHEMEINITACC_ACTION_CONFIRM_SIZE	1

#define crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_TYPESEND		0
#define crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_INDEXSEND		1
#define crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_SENDID			2
#define crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_SENDSUBID		3
#define crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_TIME		4
#define crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_SIZE			5

TCHAR* tc_MAXTHEME_INTTYPE[] = {_T("BASIC"), _T("WELCOME")};

int CMainTheme::LoadTheme(LPCTSTR strName)
{
	// Basic Acc Reader
	CAccReader reader;

	//Now, we'll now set the format

	// * General

	// Name
	reader.AddAttrib(_T("NAME"), ACCFORMAT_NORMAL, false);

	// Description
	reader.AddAttrib(_T("DESCRIPTION"), ACCFORMAT_NORMAL, false);

	// Version
	reader.AddAttrib(_T("VERSION"), ACCFORMAT_VERSION, false);

	// Screen proportion
	reader.AddAttrib(_T("SCREENPROPORTION"), ACCFORMAT_COORD, false);

	// * Interfaces

	// Interface (par = #id)
	reader.AddAttrib(_T("INTERFACE"), ACCFORMAT_NORMAL, false);
	// Gets the last Attrib as dependency
	int nDep = reader.m_nSizeAttrib-1;

	// Type
	reader.AddAttrib(_T("INTERFACETYPE"), ACCFORMAT_NORMAL, false, nDep);

	// File
	reader.AddAttrib(_T("INTERFACEFILE"), ACCFORMAT_NORMAL, false, nDep);

	// Actions
	reader.AddAttrib(_T("ACTIONS"), ACCFORMAT_NORMAL, false);
	reader.AddAttrib(_T("ACTION_CONFIRM"), ACCFORMAT_NORMAL, false, MAXTHEMEINITACC_ACTION);
	reader.AddAttrib(_T("ACTION_CONFIRM_EVENT"), ACCFORMAT_NORMAL, true, MAXTHEMEINITACC_ACTION);

	// ** Asegurate good atachment
	if(reader.m_nSizeAttrib != MAXTHEMEINITACC_SIZE) return 0;

	// Read the file
	reader.Read(strName);

	// ** NAME ** READING

	if(reader.m_strData[MAXTHEMEINITACC_NAME].size() < 1) goto omit_name_reading;
	if(reader.m_strData[MAXTHEMEINITACC_NAME][0].size() < 1) goto omit_name_reading;
	if(reader.m_strData[MAXTHEMEINITACC_NAME][0][0].size() < crMAXTHEMEINITACC_NAME_SIZE)  goto omit_name_reading;
	
	m_strDescription = reader.m_strData[MAXTHEMEINITACC_NAME][0][0][crMAXTHEMEINITACC_NAME_NAME];

omit_name_reading:

	// ** DESCRIPTION ** READING

	if(reader.m_strData[MAXTHEMEINITACC_DESCRIPTION].size() < 1) goto omit_description_reading;
	if(reader.m_strData[MAXTHEMEINITACC_DESCRIPTION][0].size() < 1) goto omit_description_reading;
	if(reader.m_strData[MAXTHEMEINITACC_DESCRIPTION][0][0].size() < crMAXTHEMEINITACC_DESCRIPTION_SIZE)  goto omit_description_reading;
	
	m_strName = reader.m_strData[MAXTHEMEINITACC_DESCRIPTION][0][0][crMAXTHEMEINITACC_DESCRIPTION_DESCRIPTION];

omit_description_reading:

	// ** VERSION ** READING

	if(reader.m_strData[MAXTHEMEINITACC_VERSION].size() < 1) goto omit_version_reading;
	if(reader.m_strData[MAXTHEMEINITACC_VERSION][0].size() < 1) goto omit_version_reading;
	if(reader.m_strData[MAXTHEMEINITACC_VERSION][0][0].size() < crMAXTHEMEINITACC_VERSION_SIZE)  goto omit_version_reading;
	
	for(size_t i = 0; i < 4; i++)
		tx_int(reader.m_strData[MAXTHEMEINITACC_VERSION][0][0][crMAXTHEMEINITACC_VERSION_VERSION+i], &m_nVersion[i]);

omit_version_reading:

	// ** SCREENPROPORTION ** READING

	if(reader.m_strData[MAXTHEMEINITACC_SCREENPROPORTION].size() < 1) goto omit_screenproportion_reading;
	if(reader.m_strData[MAXTHEMEINITACC_SCREENPROPORTION][0].size() < 1) goto omit_screenproportion_reading;
	if(reader.m_strData[MAXTHEMEINITACC_SCREENPROPORTION][0][0].size() < crMAXTHEMEINITACC_SCREENPROPORTION_SIZE)  goto omit_screenproportion_reading;
	
	double dWidth, dHeight;
	if(!tx_double(reader.m_strData[MAXTHEMEINITACC_SCREENPROPORTION][0][0][crMAXTHEMEINITACC_SCREENPROPORTION_WIDTH], &dWidth)) goto omit_screenproportion_reading;
	if(!tx_double(reader.m_strData[MAXTHEMEINITACC_SCREENPROPORTION][0][0][crMAXTHEMEINITACC_SCREENPROPORTION_HEIGHT], &dHeight)) goto omit_screenproportion_reading;

	m_dScreenWidth = dWidth;
	m_dScreenHeight = dHeight;

omit_screenproportion_reading:

	// ** INTERFACE ** READING
	for(size_t j = 0; j < reader.m_strData[MAXTHEMEINITACC_INTERFACE].size(); j++)
	{
		INT_INFO info;
		if(reader.m_strData[MAXTHEMEINITACC_INTERFACE][j].size() < 1) continue;
		if(reader.m_strData[MAXTHEMEINITACC_INTERFACE][j][0].size() < crMAXTHEMEINITACC_INTERFACE_SIZE) continue;

		info.strName = reader.m_strData[MAXTHEMEINITACC_INTERFACE][j][0][crMAXTHEMEINITACC_INTERFACE_INTERFACE];

		if(reader.m_strData[MAXTHEMEINITACC_INTERFACETYPE].size() <= j) continue;
		if(reader.m_strData[MAXTHEMEINITACC_INTERFACETYPE][j].size() < 1) continue;
		if(reader.m_strData[MAXTHEMEINITACC_INTERFACETYPE][j][0].size() < crMAXTHEMEINITACC_INTERFACETYPE_SIZE) continue;

		if(!tx_int_cat(
			reader.m_strData[MAXTHEMEINITACC_INTERFACE][j][0][crMAXTHEMEINITACC_INTERFACETYPE_INTERFACETYPE], 
			&info.nType, tc_MAXTHEME_INTTYPE, MAXTHEME_INTTYPE_SIZE)) continue;

		if(reader.m_strData[MAXTHEMEINITACC_INTERFACEFILE].size() <= j) continue;
		if(reader.m_strData[MAXTHEMEINITACC_INTERFACEFILE][j].size() < 1) continue;
		if(reader.m_strData[MAXTHEMEINITACC_INTERFACEFILE][j][0].size() < crMAXTHEMEINITACC_INTERFACEFILE_SIZE) continue;

		info.strFile = info.strInternalFile = 
			reader.m_strData[MAXTHEMEINITACC_INTERFACE][j][0][crMAXTHEMEINITACC_INTERFACEFILE_INTERFACEFILE];

		m_int_info.push_back(info);
	}

	// ** ACTION ** READING
	for(size_t j = 0; j < reader.m_strData[MAXTHEMEINITACC_ACTION].size(); j++)
	{
		INT_ACTION action;
		if(reader.m_strData[MAXTHEMEINITACC_ACTION][j].size() < 1) continue;
		if(reader.m_strData[MAXTHEMEINITACC_ACTION][j][0].size() < crMAXTHEMEINITACC_ACTION_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION][j][0][crMAXTHEMEINITACC_ACTION_ID], &action.id.nId)) continue;
		if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION][j][0][crMAXTHEMEINITACC_ACTION_SUBID], &action.id.nSubId)) continue;
		if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION][j][0][crMAXTHEMEINITACC_ACTION_ACTIONID], &action.action.nId)) continue;
		if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION][j][0][crMAXTHEMEINITACC_ACTION_ACTIONSUBID], &action.action.nSubId)) continue;
		if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION][j][0][crMAXTHEMEINITACC_ACTION_ARGID], &action.arg.nId)) continue;
		if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION][j][0][crMAXTHEMEINITACC_ACTION_ARGSUBID], &action.arg.nSubId)) continue;

		// ** ACTION_CONFIRM ** READING
		if(reader.m_strData[MAXTHEMEINITACC_ACTION_CONFIRM][j].size() < 1) continue;
		if(reader.m_strData[MAXTHEMEINITACC_ACTION_CONFIRM][j][0].size() < crMAXTHEMEINITACC_ACTION_CONFIRM_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION_CONFIRM][j][0][crMAXTHEMEINITACC_ACTION_CONFIRM], &action.nConfirm)) continue;
		action.nConfirm = max(action.nConfirm,1);	// MINIMUM 1

		// ** ACTION_CONFIRM_EVENT ** READING
		for(size_t i = 0; i < reader.m_strData[MAXTHEMEINITACC_ACTION_CONFIRM_EVENT][j].size(); i++) 
		{
			INT_ACTION_EVENT ev;
			if(reader.m_strData[MAXTHEMEINITACC_ACTION_CONFIRM_EVENT][j][i].size() < crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_SIZE) continue;
			// Obligatory data
			if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION_CONFIRM_EVENT][j][i][crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_TYPESEND], &ev.nTypeSend)) continue;
			if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION_CONFIRM_EVENT][j][i][crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_INDEXSEND], &ev.nSend)) continue;
			if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION_CONFIRM_EVENT][j][i][crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_SENDID], &ev.send.nId)) continue;
			if(!tx_int(reader.m_strData[MAXTHEMEINITACC_ACTION_CONFIRM_EVENT][j][i][crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_SENDSUBID], &ev.send.nSubId)) continue;

			__int64 t;
			if(!tx_int64(reader.m_strData[MAXTHEMEINITACC_ACTION_CONFIRM_EVENT][j][i][crMAXTHEMEINITACC_ACTION_CONFIRM_EVENT_TIME], &t)) continue;

			ev.dwTime = (DWORD)t;
			ev.nSend = 0;
			action.events.push_back(ev);
		}
		m_action.push_back(action);
	}

	// ** Initialization about

	return 1;
}
