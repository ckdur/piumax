#include "PlayerStatus.h"


CPlayerStatus::CPlayerStatus(void)
{
	np = 0;
	m_nIntFocus = -1;
}


CPlayerStatus::~CPlayerStatus(void)
{
	m_is.clear();
}
