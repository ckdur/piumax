#ifndef MAX_PLAYERSTATUS
#define MAX_PLAYERSTATUS

#include "stdafx.h"

struct PLAYER_INTERFACE_STATUS
{
	int nId;				// ID describing interface asociated
	int nIdMenuFocus;		// Wich menu ID are focussed
	int nInMenuFocus;		// Index of menu focus
};

class CPlayerStatus
{
public:
	CPlayerStatus(void);
	~CPlayerStatus(void);
	// GENERAL STATUS
	int np;			// Use for lives

	// INTERFACE STATUS
	int m_nIntFocus;			// Wich interface is focused (-1 for unknown)
	vector< PLAYER_INTERFACE_STATUS > m_is;	// Interface status asociated

	// Methods
};

#endif // MAX_PLAYERSTATUS
