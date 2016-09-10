
#include "MaxMenu.h"
#include "AccReader.h"
#include "basutil.h"

CMaxMenu::CMaxMenu(void)
{
	m_bShow = false;
	m_bEnabled = false;
	m_nActive = 0;
	m_nStCat = 0;
	for(int i = 0; i < MAXMENU_EVENT_SIZE; i++)
	{m_info.nDurEvent[i] = 0; m_info.nTimeEvent[i] = 0;}
	m_info.nPlayer = -1;
	m_info.bLoopSelection = false;
	m_info.nSizeShow = 0;
	m_info.nIndCenShow = 0;
	m_info.bResI = false;
	m_info.bResT = false;
}

CMaxMenu::~CMaxMenu(void)
{
	Delete();
}

// ** Auxiliar: Reading ACC for menu
enum MAXMENUACC_READER
{
	MAXMENUACC_ITEMS,
	MAXMENUACC_CATALOG,
	MAXMENUACC_CATALOG_INDEX,
	MAXMENUACC_CATALOG_JUMPS,
	MAXMENUACC_LOOPSELECTION,
	MAXMENUACC_SIZESHOW,
	MAXMENUACC_INDEXCENTRALSHOW,
	MAXMENUACC_SCREENS,
	MAXMENUACC_SCREENSHOW,
	MAXMENUACC_INPUT,
	MAXMENUACC_INPUT_MOD,
	MAXMENUACC_INPUT_ACTION,
	MAXMENUACC_INPUT_ARG,
	MAXMENUACC_INPUT_TIMEREP,
	MAXMENUACC_COMMAND,
	MAXMENUACC_COMMAND_ACTION,
	MAXMENUACC_COMMAND_ARG,
	MAXMENUACC_EVENTTIME,
	MAXMENUACC_SIZE
};

#define crMAXMENUACC_ITEMS_ID				0
#define crMAXMENUACC_ITEMS_SUBID			1
#define crMAXMENUACC_ITEMS_ARGID			2
#define crMAXMENUACC_ITEMS_ARGSUBID			3
#define crMAXMENUACC_ITEMS_INDEX_IMG		4
#define crMAXMENUACC_ITEMS_SUBINDEX_IMG		5
#define crMAXMENUACC_ITEMS_INDEX_TXT		6
#define crMAXMENUACC_ITEMS_SUBINDEX_TXT		7
#define crMAXMENUACC_ITEMS_SIZE				8

#define crMAXMENUACC_CATALOG_INDEX_IMG		0
#define crMAXMENUACC_CATALOG_SUBINDEX_IMG	1
#define crMAXMENUACC_CATALOG_INDEX_TXT		2
#define crMAXMENUACC_CATALOG_SUBINDEX_TXT	3
#define crMAXMENUACC_CATALOG_SIZE			4

#define crMAXMENUACC_CATALOG_INDEX_TYPE		0
#define crMAXMENUACC_CATALOG_INDEX_INDEX	1
#define crMAXMENUACC_CATALOG_INDEX_SIZE		2

#define crMAXMENUACC_CATALOG_JUMPS		0
#define crMAXMENUACC_CATALOG_JUMPS_SIZE	1

#define crMAXMENUACC_LOOPSELECTION 0
#define crMAXMENUACC_LOOPSELECTION_SIZE 1

#define crMAXMENUACC_SIZESHOW	0
#define crMAXMENUACC_SIZESHOW_SIZE	1

#define crMAXMENUACC_INDEXCENTRALSHOW	0
#define crMAXMENUACC_INDEXCENTRALSHOW_SIZE	1

#define crMAXMENUACC_SCREENS		0
#define crMAXMENUACC_SCREENS_SIZE	1

#define crMAXMENUACC_SCREENSHOW			0
#define crMAXMENUACC_SCREENSHOW_SIZE	1

#define crMAXMENUACC_INPUT_ID		0
#define crMAXMENUACC_INPUT_SUBID	1
#define crMAXMENUACC_INPUT_SIZE		2

#define crMAXMENUACC_INPUT_MOD_ID		0
#define crMAXMENUACC_INPUT_MOD_SUBID	1
#define crMAXMENUACC_INPUT_MOD_SIZE		2

#define crMAXMENUACC_INPUT_ACTION_ID		0
#define crMAXMENUACC_INPUT_ACTION_SUBID		1
#define crMAXMENUACC_INPUT_ACTION_SIZE		2

#define crMAXMENUACC_INPUT_ARG_ID		0
#define crMAXMENUACC_INPUT_ARG_SUBID	1
#define crMAXMENUACC_INPUT_ARG_SIZE		2

#define crMAXMENUACC_INPUT_TIMEREP	0
#define crMAXMENUACC_INPUT_TIMEREP_SIZE	1

#define crMAXMENUACC_COMMAND_ID		0
#define crMAXMENUACC_COMMAND_SUBID	1
#define crMAXMENUACC_COMMAND_SIZE	2

#define crMAXMENUACC_COMMAND_ACTION_ID		0
#define crMAXMENUACC_COMMAND_ACTION_SUBID	1
#define crMAXMENUACC_COMMAND_ACTION_SIZE	2

#define crMAXMENUACC_COMMAND_ARG_ID		0
#define crMAXMENUACC_COMMAND_ARG_SUBID	1
#define crMAXMENUACC_COMMAND_ARG_SIZE	2

#define crMAXMENUACC_EVENTTIME_EVENT	0
#define crMAXMENUACC_EVENTTIME_TIME		1
#define crMAXMENUACC_EVENTTIME_SIZE		2

int CMaxMenu::OpenFromAcc(LPCTSTR strFile)
{
	CAccReader reader;
	int nDep = 0;

	// Set the format
	// ** CMaxMenu public parameters
	reader.AddAttrib(_T("ITEMS"), ACCFORMAT_NORMAL, true);

	reader.AddAttrib(_T("CATALOG"), ACCFORMAT_NORMAL, false);
	reader.AddAttrib(_T("CATALOG_INDEX"), ACCFORMAT_NORMAL, true, MAXMENUACC_CATALOG);
	reader.AddAttrib(_T("CATALOG_JUMPS"), ACCFORMAT_NORMAL, true, MAXMENUACC_CATALOG);

	reader.AddAttrib(_T("LOOPSELECTION"), ACCFORMAT_NORMAL, false);

	reader.AddAttrib(_T("SIZESHOW"), ACCFORMAT_NORMAL, false);
	reader.AddAttrib(_T("INDEXCENTRALSHOW"), ACCFORMAT_NORMAL, false);

	reader.AddAttrib(_T("SCREENS"), ACCFORMAT_NORMAL, true);
	reader.AddAttrib(_T("SCREENSSHOW"), ACCFORMAT_NORMAL, true);

	// ** INPUT public parameters
	reader.AddAttrib(_T("INPUT"), ACCFORMAT_NORMAL, false);
	reader.AddAttrib(_T("INPUT_MOD"), ACCFORMAT_NORMAL, false, MAXMENUACC_INPUT);
	reader.AddAttrib(_T("INPUT_ACTION"), ACCFORMAT_NORMAL, false, MAXMENUACC_INPUT);
	reader.AddAttrib(_T("INPUT_ARG"), ACCFORMAT_NORMAL, false, MAXMENUACC_INPUT);
	reader.AddAttrib(_T("INPUT_TIMEREP"), ACCFORMAT_NORMAL, false, MAXMENUACC_INPUT);

	// ** COMMAND public parameters
	reader.AddAttrib(_T("COMMAND"), ACCFORMAT_NORMAL, true);
	reader.AddAttrib(_T("COMMAND_ACTION"), ACCFORMAT_NORMAL, false, MAXMENUACC_COMMAND);
	reader.AddAttrib(_T("COMMAND_ARG"), ACCFORMAT_NORMAL, false, MAXMENUACC_COMMAND);

	reader.AddAttrib(_T("EVENTTIME"), ACCFORMAT_NORMAL, false);

	// ** Asegurate good atachment
	if(reader.m_nSizeAttrib != MAXMENUACC_SIZE) return 0;

	// ** Read the acc
	if(!reader.Read(strFile)) return 0;

	// ** ITEMS ** READING
	for(size_t j = 0; j < reader.m_strData[MAXMENUACC_ITEMS].size(); j++)
	{
		for(size_t i = 0; i < reader.m_strData[MAXMENUACC_ITEMS][j].size(); i++)
		{
			PITEM item;
			if(reader.m_strData[MAXMENUACC_ITEMS][j][i].size() < crMAXMENUACC_ITEMS_SIZE) continue;
			// Obligatory data
			if(!tx_int(reader.m_strData[MAXMENUACC_ITEMS][j][i][crMAXMENUACC_ITEMS_ID], &item.id.nId)) continue;
			if(!tx_int(reader.m_strData[MAXMENUACC_ITEMS][j][i][crMAXMENUACC_ITEMS_SUBID], &item.id.nSubId)) continue;
			if(!tx_int(reader.m_strData[MAXMENUACC_ITEMS][j][i][crMAXMENUACC_ITEMS_ARGID], &item.arg.nId)) continue;
			if(!tx_int(reader.m_strData[MAXMENUACC_ITEMS][j][i][crMAXMENUACC_ITEMS_ARGSUBID], &item.arg.nSubId)) continue;
			if(!tx_int(reader.m_strData[MAXMENUACC_ITEMS][j][i][crMAXMENUACC_ITEMS_INDEX_IMG], &item.ind_i.nIndex)) continue;
			if(!tx_int(reader.m_strData[MAXMENUACC_ITEMS][j][i][crMAXMENUACC_ITEMS_SUBINDEX_IMG], &item.ind_i.nSubIndex)) continue;
			if(!tx_int(reader.m_strData[MAXMENUACC_ITEMS][j][i][crMAXMENUACC_ITEMS_INDEX_TXT], &item.ind_t.nIndex)) continue;
			if(!tx_int(reader.m_strData[MAXMENUACC_ITEMS][j][i][crMAXMENUACC_ITEMS_SUBINDEX_TXT], &item.ind_t.nSubIndex)) continue;
			item.ind_i.nType = MIND_TYPE_IMAGE;
			item.ind_t.nType = MIND_TYPE_TEXT;
			m_pitem.push_back(item);
		}
	}
	if(m_pitem.size() <= 0) return 0;

	// ** CATALOG ** READING
	for(size_t j = 0; j < reader.m_strData[MAXMENUACC_CATALOG].size(); j++)
	{
		MCATALOG cat;
		if(reader.m_strData[MAXMENUACC_CATALOG][j].size() < 1) continue;
		if(reader.m_strData[MAXMENUACC_CATALOG][j][0].size() < crMAXMENUACC_CATALOG_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXMENUACC_CATALOG][j][0][crMAXMENUACC_CATALOG_INDEX_IMG], &cat.ind_i.nIndex)) continue;
		if(!tx_int(reader.m_strData[MAXMENUACC_CATALOG][j][0][crMAXMENUACC_CATALOG_SUBINDEX_IMG], &cat.ind_i.nSubIndex)) continue;
		if(!tx_int(reader.m_strData[MAXMENUACC_CATALOG][j][0][crMAXMENUACC_CATALOG_INDEX_TXT], &cat.ind_t.nIndex)) continue;
		if(!tx_int(reader.m_strData[MAXMENUACC_CATALOG][j][0][crMAXMENUACC_CATALOG_SUBINDEX_TXT], &cat.ind_t.nSubIndex)) continue;
		cat.ind_i.nType = MIND_TYPE_IMAGE;
		cat.ind_t.nType = MIND_TYPE_TEXT;

		// ** CATALOG_INDEX ** READING
		for(size_t i = 0; i < reader.m_strData[MAXMENUACC_CATALOG_INDEX][j].size(); i++)
		{
			MITEM item;
			if(reader.m_strData[MAXMENUACC_CATALOG_INDEX][j][i].size() < crMAXMENUACC_CATALOG_INDEX_SIZE) continue;
			// Obligatory data
			if(!tx_int(reader.m_strData[MAXMENUACC_CATALOG_INDEX][j][i][crMAXMENUACC_CATALOG_INDEX_TYPE], &item.type)) continue;
			if(!tx_int(reader.m_strData[MAXMENUACC_CATALOG_INDEX][j][i][crMAXMENUACC_CATALOG_INDEX_INDEX], &item.in)) continue;
			cat.items.push_back(item);
		}
		if(cat.items.size() <= 0) continue;

		// ** CATALOG_JUMPS ** READING
		for(size_t i = 0; i < reader.m_strData[MAXMENUACC_CATALOG_JUMPS][j].size(); i++)
		{
			int jump = 0;
			if(reader.m_strData[MAXMENUACC_CATALOG_JUMPS][j][i].size() < crMAXMENUACC_CATALOG_JUMPS_SIZE) continue;
			// Obligatory data
			if(!tx_int(reader.m_strData[MAXMENUACC_CATALOG_JUMPS][j][i][crMAXMENUACC_CATALOG_INDEX_TYPE], &jump)) continue;
			if(jump < (int)cat.items.size()) cat.nJumps.push_back(jump);
		}
		sort(cat.nJumps.begin(), cat.nJumps.end());

		m_cats.push_back(cat);
	}
	if(m_cats.size() <= 0) return 0;

	// ** LOOPSELECTION ** READING
	if(reader.m_strData[MAXMENUACC_LOOPSELECTION].size() < 1) return 0;
	if(reader.m_strData[MAXMENUACC_LOOPSELECTION][0].size() < 1) return 0;
	if(reader.m_strData[MAXMENUACC_LOOPSELECTION][0][0].size() < crMAXMENUACC_LOOPSELECTION_SIZE) return 0;
	if(!tx_bool(reader.m_strData[MAXMENUACC_LOOPSELECTION][0][0][crMAXMENUACC_LOOPSELECTION], &m_info.bLoopSelection)) return 0;

	// ** SIZESHOW ** READING
	if(reader.m_strData[MAXMENUACC_SIZESHOW].size() < 1) return 0;
	if(reader.m_strData[MAXMENUACC_SIZESHOW][0].size() < 1) return 0;
	if(reader.m_strData[MAXMENUACC_SIZESHOW][0][0].size() < crMAXMENUACC_SIZESHOW_SIZE) return 0;
	if(!tx_int(reader.m_strData[MAXMENUACC_SIZESHOW][0][0][crMAXMENUACC_SIZESHOW], &m_info.nSizeShow)) return 0;

	// ** INDEXCENTRALSHOW ** READING
	if(reader.m_strData[MAXMENUACC_INDEXCENTRALSHOW].size() < 1) return 0;
	if(reader.m_strData[MAXMENUACC_INDEXCENTRALSHOW][0].size() < 1) return 0;
	if(reader.m_strData[MAXMENUACC_INDEXCENTRALSHOW][0][0].size() < crMAXMENUACC_INDEXCENTRALSHOW_SIZE) return 0;
	if(!tx_int(reader.m_strData[MAXMENUACC_INDEXCENTRALSHOW][0][0][crMAXMENUACC_INDEXCENTRALSHOW], &m_info.nIndCenShow)) return 0;

	// ** SCREENS ** READING
	for(size_t j = 0; j < reader.m_strData[MAXMENUACC_SCREENS].size(); j++)
	{
		for(size_t i = 0; i < reader.m_strData[MAXMENUACC_SCREENS][j].size(); i++)
		{
			if(reader.m_strData[MAXMENUACC_SCREENS][j][i].size() < crMAXMENUACC_SCREENS_SIZE) continue;

			CMaxScreen* screen;
			screen = new CMaxScreen();

			//TCHAR strDri[260];
			CString strDir;
			/*TCHAR strFil[260];
			TCHAR strExt[260];
			_tsplitpath(strFile, strDri, strDir, strFil, strExt);*/

			GetDirStr(strFile, &strDir);

			CString strScrFile;
			strScrFile.Format(_T("%ls%ls"), strDir,
				reader.m_strData[MAXMENUACC_SCREENS][j][i][crMAXMENUACC_SCREENS]);

			if(!screen->OpenFromAcc(strScrFile))
			{delete screen; continue;}

			m_screen.push_back(screen);
		}
	}

	// ** SCREENSHOW ** READING
	for(size_t j = 0; j < reader.m_strData[MAXMENUACC_SCREENSHOW].size(); j++)
	{
		vector< CMaxScreen* > screens;
		for(size_t i = 0; i < reader.m_strData[MAXMENUACC_SCREENSHOW][j].size(); i++)
		{
			if(reader.m_strData[MAXMENUACC_SCREENSHOW][j][i].size() < crMAXMENUACC_SCREENSHOW_SIZE) continue;

			CMaxScreen* screen;
			screen = new CMaxScreen();

			//TCHAR strDri[260];
			CString strDir;
			/*TCHAR strFil[260];
			TCHAR strExt[260];
			_tsplitpath(strFile, strDri, strDir, strFil, strExt);*/

			GetDirStr(strFile, &strDir);

			CString strScrFile;
			strScrFile.Format(_T("%ls%ls"), strDir,
				reader.m_strData[MAXMENUACC_SCREENSHOW][j][i][crMAXMENUACC_SCREENSHOW]);

			if(!screen->OpenFromAcc(strScrFile))
			{delete screen; continue;}

			screens.push_back(screen);
		}
		// Simple add because we need to diferenciate "generations" of showing
		m_screenshow.push_back(screens);
	}

	// ** INPUT ** READING
	for(size_t j = 0; j < reader.m_strData[MAXMENUACC_INPUT].size(); j++)
	{
		MINPUT input;
		if(reader.m_strData[MAXMENUACC_INPUT][j].size() < 1) continue;
		if(reader.m_strData[MAXMENUACC_INPUT][j][0].size() < crMAXMENUACC_INPUT_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXMENUACC_INPUT][j][0][crMAXMENUACC_INPUT_ID], &input.id.nId)) continue;
		if(!tx_int(reader.m_strData[MAXMENUACC_INPUT][j][0][crMAXMENUACC_INPUT_SUBID], &input.id.nSubId)) continue;

		// ** INPUT_MOD ** READING
		if(reader.m_strData[MAXMENUACC_INPUT_MOD][j].size() < 1) continue;
		if(reader.m_strData[MAXMENUACC_INPUT_MOD][j][0].size() < crMAXMENUACC_INPUT_MOD_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXMENUACC_INPUT_MOD][j][0][crMAXMENUACC_INPUT_MOD_ID], &input.mod.nId)) continue;
		if(!tx_int(reader.m_strData[MAXMENUACC_INPUT_MOD][j][0][crMAXMENUACC_INPUT_MOD_SUBID], &input.mod.nSubId)) continue;

		// ** INPUT_ACTION ** READING
		if(reader.m_strData[MAXMENUACC_INPUT_ACTION][j].size() < 1) continue;
		if(reader.m_strData[MAXMENUACC_INPUT_ACTION][j][0].size() < crMAXMENUACC_INPUT_ACTION_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXMENUACC_INPUT_ACTION][j][0][crMAXMENUACC_INPUT_ACTION_ID], &input.action.nId)) continue;
		if(!tx_int(reader.m_strData[MAXMENUACC_INPUT_ACTION][j][0][crMAXMENUACC_INPUT_ACTION_SUBID], &input.action.nSubId)) continue;

		// ** INPUT_ARG ** READING
		if(reader.m_strData[MAXMENUACC_INPUT_ARG][j].size() < 1) continue;
		if(reader.m_strData[MAXMENUACC_INPUT_ARG][j][0].size() < crMAXMENUACC_INPUT_ARG_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXMENUACC_INPUT_ARG][j][0][crMAXMENUACC_INPUT_ARG_ID], &input.arg.nId)) continue;
		if(!tx_int(reader.m_strData[MAXMENUACC_INPUT_ARG][j][0][crMAXMENUACC_INPUT_ARG_SUBID], &input.arg.nSubId)) continue;

		// ** INPUT_TIMEREP ** READING
		if(reader.m_strData[MAXMENUACC_INPUT_TIMEREP][j].size() < 1) continue;
		if(reader.m_strData[MAXMENUACC_INPUT_TIMEREP][j][0].size() < crMAXMENUACC_INPUT_TIMEREP_SIZE) continue;
		// Obligatory data
		if(!tx_int64(reader.m_strData[MAXMENUACC_INPUT_TIMEREP][j][0][crMAXMENUACC_INPUT_TIMEREP], &input.nTimeRep)) continue;

		m_info.input.push_back(input);
	}

	// ** COMMAND ** READING

	size_t nMaxSizeCommands = 0;	// Auxiliar used to "size" m_comm
	for(size_t j = 0; j < reader.m_strData[MAXMENUACC_COMMAND].size(); j++)
	{
		MCOMMAND comm;
		for(size_t i = 0; i < reader.m_strData[MAXMENUACC_COMMAND][j].size(); i++)
		{
			MID id;
			if(reader.m_strData[MAXMENUACC_COMMAND][j][i].size() < crMAXMENUACC_COMMAND_SIZE) continue;
			// Obligatory data
			if(!tx_int(reader.m_strData[MAXMENUACC_COMMAND][j][i][crMAXMENUACC_COMMAND_ID], &id.nId)) continue;
			if(!tx_int(reader.m_strData[MAXMENUACC_COMMAND][j][i][crMAXMENUACC_COMMAND_SUBID], &id.nSubId)) continue;
			comm.ids.push_back(id);
		}
		if(comm.ids.size() <= 0) continue;

		// ** COMMAND_ACTION ** READING
		if(reader.m_strData[MAXMENUACC_COMMAND_ACTION][j].size() < 1) continue;
		if(reader.m_strData[MAXMENUACC_COMMAND_ACTION][j][0].size() < crMAXMENUACC_COMMAND_ACTION_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXMENUACC_COMMAND_ACTION][j][0][crMAXMENUACC_COMMAND_ACTION_ID], &comm.action.nId)) continue;
		if(!tx_int(reader.m_strData[MAXMENUACC_COMMAND_ACTION][j][0][crMAXMENUACC_COMMAND_ACTION_SUBID], &comm.action.nSubId)) continue;

		// ** COMMAND_ARG ** READING
		if(reader.m_strData[MAXMENUACC_COMMAND_ARG][j].size() < 1) continue;
		if(reader.m_strData[MAXMENUACC_COMMAND_ARG][j][0].size() < crMAXMENUACC_COMMAND_ARG_SIZE) continue;
		// Obligatory data
		if(!tx_int(reader.m_strData[MAXMENUACC_COMMAND_ARG][j][0][crMAXMENUACC_COMMAND_ARG_ID], &comm.arg.nId)) continue;
		if(!tx_int(reader.m_strData[MAXMENUACC_COMMAND_ARG][j][0][crMAXMENUACC_COMMAND_ARG_SUBID], &comm.arg.nSubId)) continue;
		if(comm.ids.size() > nMaxSizeCommands) nMaxSizeCommands = comm.ids.size();

		m_info.command.push_back(comm);
	}
	for(size_t i = 0; i < m_comm.size(); i++)
		m_comm[i].resize(nMaxSizeCommands, MID());

	// ** EVENTTIME ** READING
	for(size_t j = 0; j < reader.m_strData[MAXMENUACC_EVENTTIME].size(); j++)
	{
		int nIndex;
		__int64 time;
		if(reader.m_strData[MAXMENUACC_EVENTTIME][j].size() < 1) continue;
		if(reader.m_strData[MAXMENUACC_EVENTTIME][j][0].size() < crMAXMENUACC_EVENTTIME_SIZE) continue;
		// Obligatory data
		if(!tx_int_cat(reader.m_strData[MAXMENUACC_EVENTTIME][j][0][crMAXMENUACC_EVENTTIME_EVENT], &nIndex, tc_MAXMENU_EVENT, MAXMENU_EVENT_SIZE)) continue;
		nIndex = clamp(nIndex, 0, MAXMENU_EVENT_SIZE-1);
		if(!tx_int64(reader.m_strData[MAXMENUACC_EVENTTIME][j][0][crMAXMENUACC_EVENTTIME_TIME], &time)) continue;
		m_info.nDurEvent[nIndex] = time;
	}
	return 1;
}

void CMaxMenu::Delete(void)
{
	// Really ERASE all menu data
	m_curItem.clear();
	m_curItemSel.clear();
	m_cats.clear();
	m_pitem.clear();
	// Clear screens
	for(size_t j = 0; j < m_screenshow.size(); j++)
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
			delete m_screenshow[j][i];
	for(size_t j = 0; j < m_screen.size(); j++)
			delete m_screen[j];
	m_screenshow.clear();
	m_screen.clear();
	m_callback.clear();
	m_events.clear();
	for(size_t i = 0; i < m_comm.size(); i++)
		m_comm[i].clear();
	m_comm.clear();
	m_pev.clear();
}

void CMaxMenu::Update(DWORD dwMilliseconds)
{
	if(!m_bShow) return;
	m_nTime += dwMilliseconds;
	for(size_t j = 0; j < m_screenshow.size(); j++)
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
			m_screenshow[j][i]->Update(dwMilliseconds);
	for(size_t i = 0; i < m_screen.size(); i++)
		m_screen[i]->Update(dwMilliseconds);
}

int CMaxMenu::SetPlayers(int nPlayers, int nFocus)
{
	if(nFocus >= nPlayers || nPlayers <= 0) return 0;	// Are you kidding me?
	m_info.nPlayer = nFocus;
	m_comm.resize(nPlayers, vector< MID >(0));
	return 1;
}

int CMaxMenu::Event(MID ev)
{
	// Check if active and showing
	if(!m_bShow || !m_bEnabled) return 0;

	// find player event limitations
	if(m_info.nPlayer > -1 && (ev.nId == EV_KEYDOWN || ev.nId == EV_KEYUP))
	{
		int p = GetEvPlayer(ev.nSubId);
		if(m_info.nPlayer != p && p != -1) return 0;
	}

	// Put in the stack the MID about event
	// Draw does actions for event-processing
	m_events.push_back(ev);
	return 1;
}

int CMaxMenu::SendEventCallback(MID id, MID arg)
{
	// Sends to all asociated callbacks some menu event
	for(size_t i = 0; i < m_callback.size(); i++)
		(*m_callback[i])(id, arg);
	return 1;
}

int CMaxMenu::SendEventScreens(MID id)
{
	if(!m_bShow) return 0;
	size_t uMinItems = min(m_curItem.size(), m_screenshow.size());
	for(size_t j = 0; j < uMinItems; j++)
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
			m_screenshow[j][i]->SetEvent(id);
	for(size_t i = 0; i < m_screen.size(); i++)
		m_screen[i]->SetEvent(id);
	return 1;
}

void CMaxMenu::Draw(void)
{
	if(!m_bShow) return;
	// Begin with events
	// Explore all input ID and poll event at menu and asociated callbacks
	for(size_t k = 0; k < m_events.size(); k++)
	{
		MID ev = m_events[k];
		for(size_t i = 0; i < m_info.input.size(); i++)
		{
			// Key mod
			if(ev.nId == EV_KEYDOWN &&
				(ev.nSubId == m_info.input[i].mod.nSubId ||
				GetEvPlayerA(ev.nSubId) == m_info.input[i].mod.nSubId) &&
				m_info.input[i].mod != MID(0,0))	// Input keydown
			{
				m_info.input[i].bMod = true;			// Mod Status
			}
			if(ev.nId == EV_KEYUP &&
				(ev.nSubId == m_info.input[i].mod.nSubId ||
				GetEvPlayerA(ev.nSubId) == m_info.input[i].mod.nSubId) &&
				m_info.input[i].mod != MID(0,0))	// Input keydown
			{
				m_info.input[i].bMod = false;			// No Mod Status
			}

			// Current key
			if(ev.nId == EV_KEYDOWN &&
				(ev.nSubId == m_info.input[i].id.nSubId ||
				GetEvPlayerA(ev.nSubId) == m_info.input[i].id.nSubId) && // Input keydown
				(m_info.input[i].bMod || m_info.input[i].mod == MID(0,0)))		// Mod NULL or mod active
			{
				m_info.input[i].nTimePress = m_nTime;	// Time pressed.. NOW!
				m_info.input[i].bHold = true;			// Hold Status
				m_info.input[i].nIs++;					// One action more
			}
			if((ev.nId == EV_KEYUP &&
				(ev.nSubId == m_info.input[i].id.nSubId ||
				GetEvPlayerA(ev.nSubId) == m_info.input[i].id.nSubId)) || // Input keyup
				(!m_info.input[i].bMod && m_info.input[i].mod != MID(0,0)))		// Mod no NULL and mod unactive
			{
				m_info.input[i].bHold = false;			// No hold Status
			}

			// Do HOLD REP support, we modificate nTimePress for no repeating actions
			// And increment nIs for actions left
			if(m_info.input[i].nTimeRep != 0 && (m_info.input[i].nTimePress+m_info.input[i].nTimeRep) <= m_nTime && m_info.input[i].bHold)
			{
				int nTimes = (int)((m_nTime-m_info.input[i].nTimePress)/m_info.input[i].nTimeRep);
				m_info.input[i].nTimePress += ((__int64)(nTimes))*m_info.input[i].nTimeRep;
				m_info.input[i].nIs += nTimes;
			}

			// Now, do the respective ACTION "nIs" Times
			for(int j = 0; j < m_info.input[i].nIs; j++)
			{
				DoAction(m_info.input[i].action, m_info.input[i].arg);
			}
			m_info.input[i].nIs = 0;	// No actions left
		}

		if(ev.nId != EV_KEYDOWN && ev.nId != EV_KEYUP)
		{
			DoAction(ev, MID());
		}
		else
		{
			int j = GetEvPlayer(ev.nSubId);
			if(j != 2)
			{
				// Move before commands one hit
				for(size_t i = m_comm[j].size(); i > 0; i--)
					m_comm[j][i] = m_comm[j][i-1];
				m_comm[j][0].nId = EV_KEYDOWN;
				m_comm[j][0].nSubId = GetEvPlayerA(ev.nSubId);
				// Check for commands
				for(size_t i = 0; i < m_info.command.size(); i++)
				{
					size_t nMin = min(m_info.command[i].ids.size(), m_comm[j].size());
					bool bMatch = true;
					for(size_t x = 0; x < nMin; x++)
					{
						if(m_info.command[i].ids[x] != m_comm[j][x])
							{bMatch = false; break;}
					}
					if(bMatch) DoAction(m_info.command[i].action, m_info.command[i].arg);
				}
			}
		}
	}
	// Clear event stack
	m_events.clear();

	// Now, menu actions are clear, we must check menu post-event
postevent_recheck:
	vector< MPOSTEVENT >::iterator it;
	for(it = m_pev.begin(); it != m_pev.end(); it++)
	{
		if(m_nTime >= (*it).nTime)	// Send!!!
		{
			if((*it).id.nId == MAXMENU_EVENT_END &&
				(*it).id.nSubId == MAXMENU_EVENT_EXIT)
			{
				Exit();
				return;
			}
			if((*it).id.nId == MAXMENU_EVENT_END &&
				(*it).id.nSubId == MAXMENU_EVENT_OUTCATALOG)
			{
				OutCatalog();
			}
			SendEventScreens((*it).id);
			SendEventCallback((*it).id, MID(0,0));
			m_pev.erase(it);
			goto postevent_recheck;
		}
	}

	// Draw all screens!
	for(size_t j = 0; j < m_screenshow.size(); j++) if(j < m_curItem.size())
	{
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
		{
			int nInd = int(i)-m_info.nIndCenShow+m_curItemSel[j];
			if((nInd >= 0 && nInd < (int)m_cats[m_curCat[j]].items.size()) || m_info.bLoopSelection)
				m_screenshow[j][i]->Draw();
		}
	}
	for(size_t i = 0; i < m_screen.size(); i++)
		m_screen[i]->Draw();
}


int CMaxMenu::Create(void)
{
	// Reset all status data
	m_nTime = 0;
	m_bShow = true;
	m_bEnabled = true;
	m_nActive = 0;
	m_curItem.clear();
	m_curItemSel.clear();
	m_curCat.clear();

	for(size_t i = 0; i < m_info.input.size(); i++)
	{
		m_info.input[i].nIs = 0;
		m_info.input[i].bHold = false;
		m_info.input[i].bMod = false;
		m_info.input[i].nTimePress = 0;
	}
	for(size_t i = 0; i < m_info.command.size(); i++)
	{
		m_info.command[i].nIs = 0;
		m_info.command[i].nTimeCom = 0;
	}

	// Reset all screens
	for(size_t j = 0; j < m_screenshow.size(); j++)
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
			m_screenshow[j][i]->Reset();
	for(size_t i = 0; i < m_screen.size(); i++)
		m_screen[i]->Reset();

	// Reset the event stamp
	m_events.clear();
	m_pev.clear();

	// Set commands to zero
	for(size_t i = 0; i < m_comm.size(); i++)
		m_comm[i].assign(m_comm[i].size(), MID(0,0));

	// Now, fill (or refill) the item-stock showing
	// ONLY one set-of-items
	m_curItem.resize(1, vector< MITEM >((size_t)m_info.nSizeShow));
	// If the current item was specified, not modify
	bool bMod = m_curItemSel.size()==0;
	m_curItemSel.resize(1);
	m_curCat.resize(1, m_nStCat);
	m_curCat[0] = m_nStCat;
	if(bMod) m_curItemSel[0] = 0;
	else if(m_curItemSel[0] >= int(m_cats[m_nStCat].items.size())) m_curItemSel[0] = 0;
	// Now, assign items
	m_curItem[0].resize((size_t)m_info.nSizeShow, MITEM());
	PopulateShow(0, -1);
	PopulateScreenSubResAll();

	// Pass to all screens event creation
	SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_CREATE));
	SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_CREATE), MID(0,0));
	// Post-potergate the end event
	m_pev.push_back(MPOSTEVENT(
		MID(MAXMENU_EVENT_END, MAXMENU_EVENT_CREATE),
		m_nTime+m_info.nDurEvent[MAXMENU_EVENT_CREATE]));
	return 1;
}

int CMaxMenu::Exit(void)
{
	m_bShow = false;
	m_bEnabled = false;

	// Quit all dependent resorces
	for(size_t j = 0; j < m_screenshow.size(); j++)
	{
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
		{
			m_screenshow[j][i]->m_ires.resize(1, NULL);
			m_screenshow[j][i]->m_txt.resize(1);
			m_screenshow[j][i]->m_ift.resize(1);
		}
	}
	for(size_t i = 0; i < m_screen.size(); i++)
	{
		m_screen[i]->m_ires.resize(1, NULL);
		m_screen[i]->m_txt.resize(1);
		m_screen[i]->m_ift.resize(1);
	}

	return 1;
}

int CMaxMenu::OutCatalog(void)
{
	if(m_curItem.size() <= 1) return 0;
	m_curItem.erase(m_curItem.begin());
	m_curItemSel.erase(m_curItemSel.begin());
	m_curCat.erase(m_curCat.begin());
	PopulateScreenSubResAll();
	return 1;
}

int CMaxMenu::SetResourcesI(CSpriteRes* res)
{
	// Insert resources to all screens
	for(size_t j = 0; j < m_screenshow.size(); j++)
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
		{
			m_screenshow[j][i]->DeleteSpriteRes(0);
			m_screenshow[j][i]->InsertSpriteRes(0, res);
		}
	for(size_t i = 0; i < m_screen.size(); i++)
	{
		m_screen[i]->DeleteSpriteRes(0);
		m_screen[i]->InsertSpriteRes(0, res);
	}

	// Extract Sprites from resource for each item
	// We asume than index its 0 and type is IMAGE
	for(size_t i = 0; i < m_pitem.size(); i++)
		if(m_pitem[i].ind_i.nSubIndex < int(res->GetSize()))
			m_pitem[i].ires = res->At(m_pitem[i].ind_i.nSubIndex);
	for(size_t i = 0; i < m_cats.size(); i++)
		if(m_cats[i].ind_i.nSubIndex < int(res->GetSize()))
			m_cats[i].ires = res->At(m_cats[i].ind_i.nSubIndex);
	return 1;
}

int CMaxMenu::SetResourcesT(vector< CString* > strs, vector< int > ifts)
{
	if(strs.size() != ifts.size()) return 0;

	// Insert resources to all screens
	for(size_t j = 0; j < m_screenshow.size(); j++)
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
			m_screenshow[j][i]->InsertStringCat(0, strs, ifts);
	for(size_t i = 0; i < m_screen.size(); i++)
		m_screen[i]->InsertStringCat(0, strs, ifts);

	// Extract Strings from resource for each item
	// We asume than index its 0 and type is TEXT
	for(size_t i = 0; i < m_pitem.size(); i++)
		if(m_pitem[i].ind_i.nSubIndex < int(strs.size()))
		{
			m_pitem[i].tres = strs[m_pitem[i].ind_i.nSubIndex];
			m_pitem[i].nift_res = ifts[m_pitem[i].ind_i.nSubIndex];
		}
	for(size_t i = 0; i < m_cats.size(); i++)
		if(m_cats[i].ind_i.nSubIndex < int(strs.size()))
		{
			m_cats[i].tres = strs[m_cats[i].ind_i.nSubIndex];
			m_cats[i].nift_res = ifts[m_cats[i].ind_i.nSubIndex];
		}
	return 1;
}

int CMaxMenu::SetFonts(vector< CFont2* > fnts)
{
	// Replace fonts to all screens
	for(size_t j = 0; j < m_screenshow.size(); j++)
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
			m_screenshow[j][i]->m_fnt.assign(fnts.begin(), fnts.end());
	for(size_t i = 0; i < m_screen.size(); i++)
		m_screen[i]->m_fnt.assign(fnts.begin(), fnts.end());
	return 0;
}

int CMaxMenu::SetResourcesV(int nPos, CVideoRes* res)
{
	// Set at specific VideoRes
	for(size_t j = 0; j < m_screenshow.size(); j++)
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
		{
			m_screenshow[j][i]->DeleteVideoRes(nPos);
			m_screenshow[j][i]->InsertVideoRes(nPos, res);
		}
	for(size_t i = 0; i < m_screen.size(); i++)
	{
		m_screen[i]->DeleteVideoRes(nPos);
		m_screen[i]->InsertVideoRes(nPos, res);
	}
	return 0;
}

int CMaxMenu::SetResourcesS(int nPos, CSoundRes* res)
{
	// Set at specific VideoRes
	for(size_t j = 0; j < m_screenshow.size(); j++)
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
		{
			m_screenshow[j][i]->DeleteSoundRes(nPos);
			m_screenshow[j][i]->InsertSoundRes(nPos, res);
		}
	for(size_t i = 0; i < m_screen.size(); i++)
	{
		m_screen[i]->DeleteSoundRes(nPos);
		m_screen[i]->InsertSoundRes(nPos, res);
	}
	return 0;
}

void CMaxMenu::PopulateShow(int nCat, int nCen)
{
	if(nCat < 0 || nCat > int(m_curItem.size())) return;
	if(nCen >= 0) m_curItemSel[nCat] = nCen;
	int nCur = m_curItemSel[nCat];
	for(size_t i = 0; i < m_curItem[nCat].size(); i++)
	{
		int nInd = wrapi(int(i)-m_info.nIndCenShow+nCur, 0, m_cats[m_nStCat].items.size()-1);
		// Update items
		m_curItem[nCat][i].type = m_cats[m_curCat[nCat]].items[nInd].type;
		m_curItem[nCat][i].in = m_cats[m_curCat[nCat]].items[nInd].in;
	}
}

// Llena (o actualiza) los recursos de cada screen independiente
void CMaxMenu::PopulateScreenSubRes(int nCat)
{
	if(nCat < 0 || nCat > int(m_curItem.size())) return;
	for(size_t j = 0; j < m_screenshow.size(); j++)
	{
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
		{
			if((nCat+1) >= int(m_screenshow[j][i]->m_ires.size())) continue;
			if((nCat+1) >= int(m_screenshow[j][i]->m_txt.size())) continue;
			m_screenshow[j][i]->m_ires[nCat+1]->m_sprite.resize(m_curItem[nCat].size());
			m_screenshow[j][i]->m_txt[nCat+1].resize(m_curItem[nCat].size());
			m_screenshow[j][i]->m_ift[nCat+1].resize(m_curItem[nCat].size());
			for(size_t k = 0; k < m_curItem[nCat].size(); k++)
			{
				m_screenshow[j][i]->m_ires[nCat+1]->m_sprite[k] =
				m_curItem[nCat][k].type==0?
				m_pitem[m_curItem[nCat][k].in].ires:
				m_cats[m_curItem[nCat][k].in].ires;

				m_screenshow[j][i]->m_txt[nCat+1][k] =
				m_curItem[nCat][k].type==0?
				m_pitem[m_curItem[nCat][k].in].tres:
				m_cats[m_curItem[nCat][k].in].tres;

				m_screenshow[j][i]->m_ift[nCat+1][k] =
				m_curItem[nCat][k].type==0?
				m_pitem[m_curItem[nCat][k].in].nift_res:
				m_cats[m_curItem[nCat][k].in].nift_res;
			}
		}
	}
	for(size_t i = 0; i < m_screen.size(); i++)
	{
		if((nCat+1) >= int(m_screen[i]->m_ires.size())) continue;
		if((nCat+1) >= int(m_screen[i]->m_txt.size())) continue;
		m_screen[i]->m_ires[nCat+1]->m_sprite.resize(m_curItem[nCat].size());
		m_screen[i]->m_txt[nCat+1].resize(m_curItem[nCat].size());
		m_screen[i]->m_ift[nCat+1].resize(m_curItem[nCat].size());
		for(size_t k = 0; k < m_curItem[nCat].size(); k++)
		{
			m_screen[i]->m_ires[nCat+1]->m_sprite[k] =
			m_curItem[nCat][k].type==0?
			m_pitem[m_curItem[nCat][k].in].ires:
			m_cats[m_curItem[nCat][k].in].ires;

			m_screen[i]->m_txt[nCat+1][k] =
			m_curItem[nCat][k].type==0?
			m_pitem[m_curItem[nCat][k].in].tres:
			m_cats[m_curItem[nCat][k].in].tres;

			m_screen[i]->m_ift[nCat+1][k] =
			m_curItem[nCat][k].type==0?
			m_pitem[m_curItem[nCat][k].in].nift_res:
			m_cats[m_curItem[nCat][k].in].nift_res;
		}
	}
}

void CMaxMenu::PopulateScreenSubResAll(void)
{
	for(size_t j = 0; j < m_screenshow.size(); j++)
	{
		for(size_t i = 0; i < m_screenshow[j].size(); i++)
		{
			m_screenshow[j][i]->m_ires.resize(m_curItem.size()+1);
			m_screenshow[j][i]->m_txt.resize(m_curItem.size()+1);
			m_screenshow[j][i]->m_ift.resize(m_curItem.size()+1);
		}
	}

	for(size_t i = 0; i < m_screen.size(); i++)
	{
		m_screen[i]->m_ires.resize(m_curItem.size()+1);
		m_screen[i]->m_txt.resize(m_curItem.size()+1);
		m_screen[i]->m_ift.resize(m_curItem.size()+1);
	}

	for(size_t k = 0; k < m_curItem.size(); k++)
		PopulateScreenSubRes(k);
}

int CMaxMenu::DoAction(MID id, MID arg)
{
	switch(id.nSubId)
	{
	case MAXMENU_EVENT_CREATE:
		// Creation about event? That's ilogic
		// We'll send this to screens
		// But menu doesn't nothing
		SendEventScreens(id);
		break;
	case MAXMENU_EVENT_EXIT:
		DoExit();
		break;
	case MAXMENU_EVENT_ACTIVATE:
		DoActivate();
		break;
	case MAXMENU_EVENT_DISABLE:
		DoDisable();
		break;
	case MAXMENU_EVENT_OUTCATALOG:
		DoOutCatalog();
		break;
	case MAXMENU_EVENT_SETITEM:
		if(arg != MID(0,0))
		{
			m_pev.push_back(
			MPOSTEVENT(
				MID(MAXMENU_EVENT_END, MAXMENU_EVENT_SETITEM),
				m_nTime+m_info.nDurEvent[MAXMENU_EVENT_SETITEM]));
			SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_SETITEM));
			SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_SETITEM), arg);
		}
		else
		{
			DoSetItem();
		}
		break;
	case MAXMENU_EVENT_NEXTITEM:
		DoNextItem();
		break;
	case MAXMENU_EVENT_PREVITEM:
		DoPrevItem();
		break;
	case MAXMENU_EVENT_JUMPNEXTITEM:
		DoJumpNextItem();
		break;
	case MAXMENU_EVENT_JUMPPREVITEM:
		DoJumpPrevItem();
		break;
	case MAXMENU_EVENT_REPLACESTACK:
		DoReplaceStack(arg.nSubId);
		break;
	}
	return 1;
}

int CMaxMenu::ReplaceItems(int nPos, vector< PITEM > pitems)
{
	// We know our item, we must validate it and paste the info
	if(nPos >= (int)m_pitem.size()) return 0;
	for(size_t i = 0; i < pitems.size(); i++)
	{
		if(i == 0) m_pitem[nPos] = pitems[i];
		else
		{
			vector< PITEM >::iterator it = m_pitem.begin()+(size_t)(nPos)+i;
			m_pitem.insert(it, pitems[i]);
		}
	}
	// now, do the same with CATALOG
	// All MITEM equal to 0 and nPos, will be replaced
	if(pitems.size() > 1) for(size_t i = 0; i < m_cats.size(); i++)
	{
		// Items movement
		for(size_t j = 0; j < m_cats[i].items.size(); j++)
		{
			if(m_cats[i].items[j].type == 0 && m_cats[i].items[j].in == nPos)
			{
				for(size_t k = 1; k < pitems.size(); k++)
				{
					MITEM item;
					item.type = 0;
					item.in = nPos+k;
					vector< MITEM >::iterator it = m_cats[i].items.begin()+j+k;
					m_cats[i].items.insert(it, item);
				}
				// Jump re-assignament
				for(size_t k = 0; k < (int)m_cats[i].nJumps.size(); k++)
					if(m_cats[i].nJumps[k] > (int)j) m_cats[i].nJumps[k] += pitems.size()-1;
			}
		}
	}
	return 1;
}

int CMaxMenu::ReplaceItemsAtID(MID id, MID arg, vector< PITEM > pitems)
{
	for(size_t i = 0; i < m_pitem.size(); i++) if(id == m_pitem[i].id && arg == m_pitem[i].arg)
	{
		ReplaceItems((int)i, pitems);
	}
	return 1;
}

void CMaxMenu::DoExit(void)
{
	if(!m_bShow) return;
	m_pev.push_back(
		MPOSTEVENT(
		MID(MAXMENU_EVENT_END, MAXMENU_EVENT_EXIT),
		m_nTime+m_info.nDurEvent[MAXMENU_EVENT_EXIT]));
	SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_EXIT));
	SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_EXIT), MID(0,0));
}

void CMaxMenu::DoActivate(void)
{
	if(!m_bShow) return;
	m_nActive = 1;
	m_pev.push_back(
		MPOSTEVENT(
		MID(MAXMENU_EVENT_END, MAXMENU_EVENT_ACTIVATE),
		m_nTime+m_info.nDurEvent[MAXMENU_EVENT_ACTIVATE]));
	SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_ACTIVATE));
	SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_ACTIVATE), MID(0,0));
}

void CMaxMenu::DoDisable(void)
{
	if(!m_bShow) return;
	m_nActive = 0;
	m_pev.push_back(
		MPOSTEVENT(
		MID(MAXMENU_EVENT_END, MAXMENU_EVENT_DISABLE),
		m_nTime+m_info.nDurEvent[MAXMENU_EVENT_DISABLE]));
	SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_DISABLE));
	SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_DISABLE), MID(0,0));
}

void CMaxMenu::DoOutCatalog(void)
{
	if(!m_bShow) return;
	if(m_curItem.size() <= 1)
	{
		m_pev.push_back(
			MPOSTEVENT(
			MID(MAXMENU_EVENT_END, MAXMENU_EVENT_NOOUTCATALOG),
			m_nTime+m_info.nDurEvent[MAXMENU_EVENT_NOOUTCATALOG]));
		SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NOOUTCATALOG));
		SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NOOUTCATALOG), MID(0,0));
	}
	else
	{
		// The true OUT will be efected in post-event
		m_pev.push_back(
			MPOSTEVENT(
			MID(MAXMENU_EVENT_END, MAXMENU_EVENT_OUTCATALOG),
			m_nTime+m_info.nDurEvent[MAXMENU_EVENT_OUTCATALOG]));
		SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_OUTCATALOG));
		SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_OUTCATALOG), MID(0,0));
	}
}

void CMaxMenu::DoSetItem(void)
{
	if(!m_bShow || !m_nActive) return;
	// Get the MITEM
	MITEM item = m_cats[m_curCat.front()].items[m_curItemSel.front()];
	if(item.type == 0)
	{
		// Do the respective action
		MID id = m_pitem[item.in].id;
		MID arg = m_pitem[item.in].arg;
		DoAction(id, arg);
	}
	else
	{
		// Is In Catalog method, do the insertion
		m_curItem.insert(m_curItem.begin(), vector< MITEM >((size_t)m_info.nSizeShow));
		m_curItemSel.insert(m_curItemSel.begin(), 0);
		m_curCat.insert(m_curItemSel.begin(), item.in);
		PopulateShow(0, 0);
		PopulateScreenSubResAll();

		m_pev.push_back(
			MPOSTEVENT(
			MID(MAXMENU_EVENT_END, MAXMENU_EVENT_INCATALOG),
		m_nTime+m_info.nDurEvent[MAXMENU_EVENT_INCATALOG]));
		SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_INCATALOG));
		SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_INCATALOG), MID(0,0));
	}
}

void CMaxMenu::DoNextItem(void)
{
	if(!m_bShow) return;
	// Do next item
	m_curItemSel[0]++;
	if(m_curItemSel[0] >= (int)m_cats[m_curCat[0]].items.size())
	{
		if(m_info.bLoopSelection)
		{
			m_curItemSel[0]=0;
			m_pev.push_back(
			MPOSTEVENT(
				MID(MAXMENU_EVENT_END, MAXMENU_EVENT_NEXTITEM),
				m_nTime+m_info.nDurEvent[MAXMENU_EVENT_NEXTITEM]));
			SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NEXTITEM));
			SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NEXTITEM), MID(0,0));
		}
		else
		{
			m_curItemSel[0]=m_cats[m_curCat[0]].items.size()-1;
			m_pev.push_back(
			MPOSTEVENT(
				MID(MAXMENU_EVENT_END, MAXMENU_EVENT_NONEXTITEM),
				m_nTime+m_info.nDurEvent[MAXMENU_EVENT_NONEXTITEM]));
			SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NONEXTITEM));
			SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NONEXTITEM), MID(0,0));
			return;
		}
	}
	PopulateShow(0, -1);
	PopulateScreenSubRes(0);
}

void CMaxMenu::DoPrevItem(void)
{
	if(!m_bShow) return;
	// Do next item
	m_curItemSel[0]--;
	if(m_curItemSel[0] < 0)
	{
		if(m_info.bLoopSelection)
		{
			m_curItemSel[0]=m_cats[m_curCat[0]].items.size()-1;
			m_pev.push_back(
			MPOSTEVENT(
				MID(MAXMENU_EVENT_END, MAXMENU_EVENT_PREVITEM),
				m_nTime+m_info.nDurEvent[MAXMENU_EVENT_PREVITEM]));
			SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_PREVITEM));
			SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_PREVITEM), MID(0,0));
		}
		else
		{
			m_curItemSel[0]=0;
			m_pev.push_back(
			MPOSTEVENT(
				MID(MAXMENU_EVENT_END, MAXMENU_EVENT_NOPREVITEM),
				m_nTime+m_info.nDurEvent[MAXMENU_EVENT_NOPREVITEM]));
			SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NOPREVITEM));
			SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NOPREVITEM), MID(0,0));
			return;
		}
	}
	PopulateShow(0, -1);
	PopulateScreenSubRes(0);
}

void CMaxMenu::DoJumpNextItem(void)
{
	if(!m_bShow) return;
	// Do jump next item
	if(m_curItemSel[0] <= m_cats[m_curCat[0]].nJumps.front() && !m_info.bLoopSelection)
	{
		m_pev.push_back(
		MPOSTEVENT(
			MID(MAXMENU_EVENT_END, MAXMENU_EVENT_NOJUMPNEXTITEM),
			m_nTime+m_info.nDurEvent[MAXMENU_EVENT_NOJUMPNEXTITEM]));
		SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NOJUMPNEXTITEM));
		SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NOJUMPNEXTITEM), MID(0,0));
		return;
	}
	if(m_cats[m_curCat[0]].nJumps.size() == 0) return;
	int nBefore = m_curItemSel[0];
	for(size_t i = (m_cats[m_curCat[0]].nJumps.size()-1); i >= 0; i--)
	{
		if(m_cats[m_curCat[0]].nJumps[i] < m_curItemSel[0])
		{
			m_curItemSel[0] = m_cats[m_curCat[0]].nJumps[i];
			break;
		}
		if (i==0) m_curItemSel[0] = m_cats[m_curCat[0]].nJumps.back();
	}
	if(nBefore == m_curItemSel[0]) return;	// No jump efectuated
	PopulateShow(0, -1);
	PopulateScreenSubRes(0);
	m_pev.push_back(
	MPOSTEVENT(
		MID(MAXMENU_EVENT_END, MAXMENU_EVENT_JUMPNEXTITEM),
		m_nTime+m_info.nDurEvent[MAXMENU_EVENT_JUMPNEXTITEM]));
	SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_JUMPNEXTITEM));
	SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_JUMPNEXTITEM), MID(0,0));
}

void CMaxMenu::DoJumpPrevItem(void)
{
	if(!m_bShow) return;
	// Do jump next item
	if(m_curItemSel[0] >= m_cats[m_curCat[0]].nJumps.back() && !m_info.bLoopSelection)
	{
		m_pev.push_back(
		MPOSTEVENT(
			MID(MAXMENU_EVENT_END, MAXMENU_EVENT_NOJUMPPREVITEM),
			m_nTime+m_info.nDurEvent[MAXMENU_EVENT_NOJUMPPREVITEM]));
		SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NOJUMPPREVITEM));
		SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_NOJUMPPREVITEM), MID(0,0));
		return;
	}
	if(m_cats[m_curCat[0]].nJumps.size() == 0) return;
	int nBefore = m_curItemSel[0];
	for(size_t i = 0; i < m_cats[m_curCat[0]].nJumps.size(); i)
	{
		if(m_cats[m_curCat[0]].nJumps[i] > m_curItemSel[0])
		{
			m_curItemSel[0] = m_cats[m_curCat[0]].nJumps[i];
			break;
		}
		if (i==(m_cats[m_curCat[0]].nJumps.size()-1)) m_curItemSel[0] = m_cats[m_curCat[0]].nJumps.front();
	}
	if(nBefore == m_curItemSel[0]) return;	// No jump efectuated
	PopulateShow(0, -1);
	PopulateScreenSubRes(0);
	m_pev.push_back(
	MPOSTEVENT(
		MID(MAXMENU_EVENT_END, MAXMENU_EVENT_JUMPPREVITEM),
		m_nTime+m_info.nDurEvent[MAXMENU_EVENT_JUMPPREVITEM]));
	SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_JUMPPREVITEM));
	SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_JUMPPREVITEM), MID(0,0));
}

void CMaxMenu::DoReplaceStack(int nCat)
{
	if(nCat < 0 || nCat >= int(m_cats.size())) return;
	if(!m_bShow) return;
	m_curItem.clear();
	m_curItemSel.clear();
	m_curCat.clear();

	// Now, fill (or refill) the item-stock showing
	// ONLY one set-of-items
	m_curItem.resize(1, vector< MITEM >((size_t)m_info.nSizeShow));
	m_curItemSel.resize(1, 0);
	m_curCat.resize(1, nCat);
	m_curCat[0] = nCat;
	m_curItemSel[0] = 0;
	// Now, assign items
	m_curItem[0].resize((size_t)m_info.nSizeShow, MITEM());
	PopulateShow(0, 0);
	PopulateScreenSubResAll();
	m_pev.push_back(
	MPOSTEVENT(
		MID(MAXMENU_EVENT_END, MAXMENU_EVENT_REPLACESTACK),
		m_nTime+m_info.nDurEvent[MAXMENU_EVENT_REPLACESTACK]));
	SendEventScreens(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_REPLACESTACK));
	SendEventCallback(MID(MAXMENU_EVENT_BEGIN, MAXMENU_EVENT_REPLACESTACK), MID(0,0));
}
