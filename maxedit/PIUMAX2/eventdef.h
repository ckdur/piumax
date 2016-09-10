#ifndef DEF_MODULES_EVENT
#define DEF_MODULES_EVENT

#include "stdafx.h"

#define EV_KEYDOWN 1
#define EV_KEYUP 2
// Los demas comandos se darán desde el rango 1 al  (segun binario)
#define EV_NULL 0		// Reservado
#define EV_P1_CENTER 1	// Centro
#define EV_P1_ARROW1 2	// Diagonal abajo izquierda
#define EV_P1_ARROW3 3	// Diagonal abajo derecha
#define EV_P1_ARROW7 4	// Diagonal arriba izquierda
#define EV_P1_ARROW9 5	// Diagonal arriba derecha
#define EV_P2_CENTER 6	// Centro
#define EV_P2_ARROW1 7	// Diagonal abajo izquierda
#define EV_P2_ARROW3 8	// Diagonal abajo derecha
#define EV_P2_ARROW7 9	// Diagonal arriba izquierda
#define EV_P2_ARROW9 10	// Diagonal arriba derecha
#define EV_CREDIT1 11	// Nuevo credito
#define EV_CREDIT2 12	// Nuevo credito
#define EV_SERVICE 13	// Servicio
#define EV_CONFIG 14	// Modo configuracion
#define EV_P1_LEFT 15	// Frontal izquierda 1p
#define EV_P1_RIGHT 16	// Frontal derecha 1p
#define EV_P1_UP 17		// Frontal arriba 1p
#define EV_P1_DOWN 18	// Frontal abajo 1p
#define EV_P1_FIRE1 19	// Frontal choice 1p
#define EV_P1_FIRE2 20	// Frontal choice 1p
#define EV_P2_LEFT 21	// Frontal izquierda 2p
#define EV_P2_RIGHT 22	// Frontal derecha 2p
#define EV_P2_UP 23		// Frontal arriba 2p
#define EV_P2_DOWN 24	// Frontal abajo 2p
#define EV_P2_FIRE1 25	// Frontal choice 2p
#define EV_P2_FIRE2 26	// Frontal choice 2p
#define EV_P_CENTER 27	// Centro
#define EV_P_ARROW1 28	// Diagonal abajo izquierda
#define EV_P_ARROW3 29	// Diagonal abajo derecha
#define EV_P_ARROW7 30	// Diagonal arriba izquierda
#define EV_P_ARROW9 31	// Diagonal arriba derecha
#define EV_P_LEFT 32	// Frontal izquierda
#define EV_P_RIGHT 33	// Frontal derecha
#define EV_P_UP 34		// Frontal arriba
#define EV_P_DOWN 35	// Frontal abajo
#define EV_P_FIRE1 36	// Frontal choice
#define EV_P_FIRE2 37	// Frontal choice

#define EV_SIZE	38		// Tamaño de esta enumeración

extern TCHAR* tc_EV_KEY[];
int GetEvPlayerA(int i); // Gets a P event about P1, P2... event, returns original if is not P~ event
int GetEvPlayer(int i);	// Gets specific player about event (-1 if shared)

// Menu Events

#define MAXMENU_EVENT_BEGIN 3
#define MAXMENU_EVENT_END 4

enum MAXMENU_EVENT
{
	MAXMENU_EVENT_NULL,		// Reserved
	MAXMENU_EVENT_CREATE,
	MAXMENU_EVENT_EXIT,
	MAXMENU_EVENT_ACTIVATE,
	MAXMENU_EVENT_DISABLE,
	MAXMENU_EVENT_INCATALOG,
	MAXMENU_EVENT_OUTCATALOG,
	MAXMENU_EVENT_NOOUTCATALOG,
	MAXMENU_EVENT_SETITEM,
	MAXMENU_EVENT_NEXTITEM,
	MAXMENU_EVENT_PREVITEM,
	MAXMENU_EVENT_JUMPNEXTITEM,
	MAXMENU_EVENT_JUMPPREVITEM,
	MAXMENU_EVENT_REPLACESTACK,
	MAXMENU_EVENT_NONEXTITEM,
	MAXMENU_EVENT_NOPREVITEM,
	MAXMENU_EVENT_NOJUMPNEXTITEM,
	MAXMENU_EVENT_NOJUMPPREVITEM,
	MAXMENU_EVENT_SIZE
};
extern TCHAR* tc_MAXMENU_EVENT[];

// Interface base Events
#define MAXINT_EVENT_RECV 5
#define MAXINT_EVENT_DO 6

enum MAXINT_EVENT
{
	MAXINT_EVENT_NULL,		// Reserved
	MAXINT_EVENT_CREATE,
	MAXINT_EVENT_RESET,
	MAXINT_EVENT_EXIT,
	MAXINT_EVENT_ACTIVATE,
	MAXINT_EVENT_DISABLE,
	// MENU-OPERATION EVENTS
	MAXINT_EVENT_MENU_CREATE,
	MAXINT_EVENT_MENU_EXIT,
	MAXINT_EVENT_MENU_ACTIVATE,
	MAXINT_EVENT_MENU_DISABLE,
	MAXINT_EVENT_MENU_FOCUS,
	MAXINT_EVENT_SIZE
};

#endif // DEF_MODULES_EVENT
