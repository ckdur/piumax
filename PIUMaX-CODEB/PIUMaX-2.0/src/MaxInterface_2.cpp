// MaxInterface_2.cpp
// Made by: CK-Dur
/*
Includes definitions about common functionality about common interface
MaxInterface.cpp: This file includes definitions about common loading functions
MaxInterface_3.cpp: Auxiliar, includes extended about basic (only
*/

#include "MaxInterface.h"
#include "AccReader.h"
#include "basutil.h"

void CMaxInterface::DrawCommon(void)
{
	DrawScreensCommon();
	DrawMenusCommon();

	UpdateActions();

}

void CMaxInterface::DrawScreensCommon(void)
{
	for(size_t i = 0; i < m_screen.size(); i++)
	{
		m_screen[i]->Draw();
	}
}

void CMaxInterface::DrawMenusCommon(void)
{
	for(size_t i = 0; i < m_menu.size(); i++)
	{
		m_menu[i]->Draw();
	}
}

void CMaxInterface::UpdateCommon(DWORD dwMilliseconds)
{
	m_nTime += dwMilliseconds;
	/*for(size_t j = 0; j < m_ires.size(); j++)
		m_ires[j]->UpdateAll(dwMilliseconds);
	for(size_t j = 0; j < m_vres.size(); j++)
		m_vres[j]->UpdateAll(dwMilliseconds);*/
	for(size_t i = 0; i < m_screen.size(); i++)
	{
		m_screen[i]->Update(dwMilliseconds);
	}
	for(size_t i = 0; i < m_menu.size(); i++)
	{
		m_menu[i]->Update(dwMilliseconds);
	}
}

int CMaxInterface::EventCommon(MID ev)
{
	// This function receives Event data, from the main
	// We send this event to all menus and screens

	// Send this event to all screens and menus
	for(size_t i = 0; i < m_screen.size(); i++)
	{
		m_screen[i]->SetEvent(ev);
	}

	for(size_t i = 0; i < m_menu.size(); i++)
	{
		m_menu[i]->Event(ev);
	}

	LookActionCommon(ev);

	// ** Call our lua event func if exists
	MaxIntSetCallbackHandle(this);

	// ** PUT ADITIONAL EVENT POLLING RESPONSE HERE
	return 1;
}

int CMaxInterface::MenuCallbackCommon(MID id, MID arg)
{
	// This function receives Event data, from menus and etc
	// Send this event to all screens
	for(size_t i = 0; i < m_screen.size(); i++)
	{
		m_screen[i]->SetEvent(id);
	}

	LookActionCommon(id);

	// ** PUT ADITIONAL EVENT POLLING RESPONSE HERE
	return 1;
}

MID CMaxInterface::InterfaceStatusCommon(void)
{
	return MID();
}

int CMaxInterface::SendEventCallback(MID id, MID arg)
{
	// Sends to all asociated callbacks some menu event
	for(size_t i = 0; i < m_callback.size(); i++)
		(*m_callback[i])(id, arg);
	return 1;
}

int CMaxInterface::LookActionCommon(MID ev)
{
	// Explore the actions, send aditional events if necesary
	for(size_t i = 0; i < m_action.size(); i++) if(m_action[i].id == ev)
	{
		if(m_action[i].nConfirmStat < m_action[i].nConfirm)
		{
			m_action[i].nConfirmStat++;
			m_action[i].nTimeConfirm = m_nTime;
		}
		else
		{
			m_action[i].nConfirmStat = 0;
			m_action[i].nTimeConfirm = 0;

			m_evToSend.insert(m_evToSend.end(), m_action[i].events.begin(), m_action[i].events.end());
			for(size_t j = 0; j < m_action[i].events.size(); j++)
				m_evToSend[m_evToSend.size()-m_action[i].events.size()+j].dwTime
				+= (DWORD)m_nTime;
		}
	}

	UpdateActions();

	return 1;
}

int CMaxInterface::UpdateActions(void)
{
	// Iterate about the time-demand events
	for(size_t j = 0; j < m_evToSend.size(); j++)
	{
		if(m_nTime >= (__int64)m_evToSend[j].dwTime)
		{

			switch(m_evToSend[j].nTypeSend)
			{
			case -1:
				for(size_t i = 0; i < m_screen.size(); i++)
					m_screen[i]->SetEvent(m_evToSend[j].send);

				for(size_t i = 0; i < m_menu.size(); i++)
					m_menu[i]->Event(m_evToSend[j].send);

				MenuCallbackCommon(m_evToSend[j].send, MID(0,0));
				break;
			case 2:
				if(m_evToSend[j].nSend <= -1)
					MenuCallbackCommon(m_evToSend[j].send, MID(0,0));
				break;
			case 0:
				if(m_evToSend[j].nSend <= -1) for(size_t i = 0; i < m_screen.size(); i++)
					m_screen[i]->SetEvent(m_evToSend[j].send);
				else
				{
					if(m_evToSend[j].nSend >= int(m_callback.size()))
					{
						_printf(_T("(WARNING): MaxInterface: doesn't exist %d screen\n"), m_evToSend[j].nSend);
						break;
					}
					m_screen[m_evToSend[j].nSend]->SetEvent(m_evToSend[j].send);
				}
				break;
			case 1:
				if(m_evToSend[j].nSend <= -1) for(size_t i = 0; i < m_menu.size(); i++)
					m_menu[i]->Event(m_evToSend[j].send);
				else
				{
					if(m_evToSend[j].nSend  >= int(m_callback.size()))
					{
						_printf(_T("(WARNING): MaxInterface: doesn't exist %d menu\n"), m_evToSend[j].nSend);
						break;
					}
					m_menu[m_evToSend[j].nSend]->Event(m_evToSend[j].send);
				}
				break;
			default:
				_printf(_T("(WARNING): MaxInterface: No-enumarated argument\n"));
				break;
			}
			vector< INT_ACTION_EVENT >::iterator it = m_evToSend.begin()+j;
			m_evToSend.erase(it);
			j--;
		}
	}
	return 1;
}

int CMaxInterface::LuaEventCommon(MID ev)
{
	if(m_bLuaEvent)
	{
		lua_State* L = m_lua->L;
		lua_getglobal(L, "Event");  /* function to be called */
		lua_newtable(L);
		if(!MLUA_setMID(L, -1, ev)) return 0;

		int nReturn = 0;
		// Get the result
		if(!lua_isnumber(L, -1))
			nReturn = 1;

		nReturn = (int)lua_tonumber(L, -1);
		m_lua->ResetStack();
		return 1;
	}
	return 0;
}

int CMaxInterface::UpdateResources(void)
{
	for(size_t i = 0; i < m_screen.size(); i++)
	{
		m_screen[i]->ClearFont();
		m_screen[i]->ClearSoundRes();
		m_screen[i]->ClearSpriteRes();
		m_screen[i]->ClearStringCat();
		m_screen[i]->ClearVideoRes();
		for(size_t j = 0; j < m_ires.size(); j++)
			m_screen[i]->InsertSpriteRes(j, m_ires[j]);
		for(size_t j = 0; j < m_sres.size(); j++)
			m_screen[i]->InsertSoundRes(j, m_sres[j]);
		for(size_t j = 0; j < m_vres.size(); j++)
			m_screen[i]->InsertVideoRes(j, m_vres[j]);
		for(size_t j = 0; j < m_fnt.size(); j++)
			m_screen[i]->InsertFont(j, m_fnt[j]);
		for(size_t j = 0; j < m_txt.size(); j++)
			m_screen[i]->InsertStringCat(j, m_txt[j], m_ift[j]);
	}

	for(size_t i = 0; i < m_menu.size(); i++)
	{
		m_menu[i]->SetResourcesI(m_ires[0]);
		m_menu[i]->SetResourcesT(m_txt[0], m_ift[0]);
		m_menu[i]->SetFonts(m_fnt);
		for(size_t j = 0; j < m_sres.size(); j++)
			m_menu[i]->SetResourcesS(j, m_sres[j]);
		for(size_t j = 0; j < m_vres.size(); j++)
			m_menu[i]->SetResourcesV(j, m_vres[j]);
	}
	return 1;
}
