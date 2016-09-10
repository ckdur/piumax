#include "eventdef.h"

TCHAR* tc_EV_KEY[] = 
{_T("NULL"), 
_T("P1_CENTER"), _T("P1_ARROW1"), _T("P1_ARROW3"), _T("P1_ARROW7"), _T("P1_ARROW9"),
_T("P2_CENTER"), _T("P2_ARROW1"), _T("P2_ARROW3"), _T("P2_ARROW7"), _T("P2_ARROW9"),
_T("CREDIT1"), _T("CREDIT2"), _T("SERVICE"), _T("CONFIG"),
_T("P1_LEFT"), _T("P1_RIGHT"), _T("P1_UP"), _T("P1_DOWN"), _T("P1_FIRE1"), _T("P1_FIRE2"),
_T("P2_LEFT"), _T("P2_RIGHT"), _T("P2_UP"), _T("P2_DOWN"), _T("P2_FIRE1"), _T("P2_FIRE2"),
_T("P_CENTER"), _T("P_ARROW1"), _T("P_ARROW3"), _T("P_ARROW7"), _T("P_ARROW9"),
_T("P_LEFT"), _T("P_RIGHT"), _T("P_UP"), _T("P_DOWN"), _T("P_FIRE1"), _T("P_FIRE2")
};

TCHAR* tc_MAXMENU_EVENT[] = 
{_T("NULL"),
_T("CREATE"), _T("EXIT"), _T("ACTIVATE"),
_T("DISABLE"), _T("INCATALOG"), _T("OUTCATALOG"), _T("NOOUTCATALOG"),
_T("SETITEM"), _T("NEXTITEM"), _T("PREVITEM"),
_T("JUMPNEXTITEM"), _T("JUMPPREVITEM"), _T("REPLACESTACK"),
_T("NONEXTITEM"), _T("NOPREVITEM"), 
_T("NOJUMPNEXTITEM"), _T("NOJUMPPREVITEM")};

TCHAR* tc_MAXINT_EVENT[] = 
{_T("NULL"),
_T("CREATE"), _T("EXIT"), _T("ACTIVATE"),
_T("DISABLE")};

int GetEvPlayerA(int i) // Gets a P event about P1, P2... event, returns original if is not P~ event
{
	if(i >= EV_P1_CENTER && i <= EV_P1_ARROW9) return i-EV_P1_CENTER+EV_P_CENTER;
	if(i >= EV_P2_CENTER && i <= EV_P2_ARROW9) return i-EV_P2_CENTER+EV_P_CENTER;
	if(i >= EV_P1_LEFT && i <= EV_P1_FIRE2) return i-EV_P1_LEFT+EV_P_LEFT;
	if(i >= EV_P2_LEFT && i <= EV_P2_FIRE2) return i-EV_P2_LEFT+EV_P_LEFT;
	return i;
}

int GetEvPlayer(int i)	// Gets specific player about event (-1 if shared)
{
	if(i >= EV_P1_CENTER && i <= EV_P1_ARROW9) return 0;
	if(i >= EV_P1_LEFT && i <= EV_P1_FIRE2) return 0;
	if(i >= EV_P2_CENTER && i <= EV_P2_ARROW9) return 1;
	if(i >= EV_P2_LEFT && i <= EV_P2_FIRE2) return 1;
	return -1;
}

