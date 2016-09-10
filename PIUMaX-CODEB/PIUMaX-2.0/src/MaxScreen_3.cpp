#include "MaxScreen.h"
#include "AccReader.h"
#include "basutil.h"

// ** Add a SpriteRes catalog
int CMaxScreen::AddSpriteRes(CSpriteRes* res)
{
	m_ires.push_back(res);
	return m_ires.size();
}

// ** Inserts a SpriteRes catalog
int CMaxScreen::InsertSpriteRes(int nPos, CSpriteRes* res)
{
	if(nPos < 0 || (size_t)nPos > m_ires.size()) return 0;
	vector< CSpriteRes* >::iterator it = m_ires.begin()+(size_t)nPos;
	m_ires.insert(it, res);
	return 1;
}

// ** Delete a SpriteRes catalog
void CMaxScreen::DeleteSpriteRes(int nPos)
{
	if(nPos < 0 || (size_t)nPos >= m_ires.size()) return;
	vector< CSpriteRes* >::iterator it = m_ires.begin()+(size_t)nPos;
	m_ires.erase(it);
}

// ** Clear all the SpriteRes catalog
void CMaxScreen::ClearSpriteRes(void)
{
	m_ires.clear();
}

// ** Add a VideoRes catalog
int CMaxScreen::AddVideoRes(CVideoRes* res)
{
	m_vres.push_back(res);
	return m_vres.size();
}

// ** Inserts a VideoRes catalog
int CMaxScreen::InsertVideoRes(int nPos, CVideoRes* res)
{
	if(nPos < 0 || (size_t)nPos > m_vres.size()) return 0;
	vector< CVideoRes* >::iterator it = m_vres.begin()+(size_t)nPos;
	m_vres.insert(it, res);
	return 1;
}

// ** Delete a VideoRes catalog
void CMaxScreen::DeleteVideoRes(int nPos)
{
	if(nPos < 0 || (size_t)nPos >= m_vres.size()) return;
	vector< CVideoRes* >::iterator it = m_vres.begin()+(size_t)nPos;
	m_vres.erase(it);
}

// ** Clear all the VideoRes catalog
void CMaxScreen::ClearVideoRes(void)
{
	m_vres.clear();
}

// ** Add a SoundRes catalog
int CMaxScreen::AddSoundRes(CSoundRes* res)
{
	m_sres.push_back(res);
	return m_sres.size();
}

// ** Inserts a SoundRes catalog
int CMaxScreen::InsertSoundRes(int nPos, CSoundRes* res)
{
	if(nPos < 0 || (size_t)nPos > m_sres.size()) return 0;
	vector< CSoundRes* >::iterator it = m_sres.begin()+(size_t)nPos;
	m_sres.insert(it, res);
	return 1;
}

// ** Delete a SoundRes catalog
void CMaxScreen::DeleteSoundRes(int nPos)
{
	if(nPos < 0 || (size_t)nPos >= m_sres.size()) return;
	vector< CSoundRes* >::iterator it = m_sres.begin()+(size_t)nPos;
	m_sres.erase(it);
}

// ** Clear all the SoundRes catalog
void CMaxScreen::ClearSoundRes(void)
{
	m_sres.clear();
}

// ** Add a String catalog
int CMaxScreen::AddStringCat(vector< CString* > txt, vector< int > ift)
{
	m_txt.push_back(txt);
	m_ift.push_back(ift);
	return m_txt.size();
}

// ** Inserts a String catalog
int CMaxScreen::InsertStringCat(int nPos, vector< CString* > txt, vector< int > ift)
{
	if(nPos < 0 || (size_t)nPos > m_txt.size()) return 0;
	vector< vector< CString* > >::iterator it = m_txt.begin()+(size_t)nPos;
	vector< vector< int > >::iterator it2 = m_ift.begin()+(size_t)nPos;
	m_txt.insert(it, txt);
	m_ift.insert(it2, ift);
	return 1;
}

// ** Delete a String catalog
void CMaxScreen::DeleteStringCat(int nPos)
{
	if(nPos < 0 || (size_t)nPos > m_txt.size()) return;
	vector< vector< CString* > >::iterator it = m_txt.begin()+(size_t)nPos;
	vector< vector< int > >::iterator it2 = m_ift.begin()+(size_t)nPos;
	m_txt.erase(it);
	m_ift.erase(it2);
}

// ** Clear all the String catalog
void CMaxScreen::ClearStringCat(void)
{
	for(size_t i = 0; i < m_txt.size(); i++)
		m_txt.clear();
	m_txt.clear();
	m_ift.clear();
}

// ** Add a Font
int CMaxScreen::AddFont(CFont2* res)
{
	m_fnt.push_back(res);
	return m_fnt.size();
}

// ** Inserts a Font
int CMaxScreen::InsertFont(int nPos, CFont2* res)
{
	if(nPos < 0 || (size_t)nPos > m_fnt.size()) return 0;
	vector< CFont2* >::iterator it = m_fnt.begin()+(size_t)nPos;
	m_fnt.insert(it, res);
	return 1;
}

// ** Delete a Font
void CMaxScreen::DeleteFont(int nPos)
{
	if(nPos < 0 || (size_t)nPos >= m_fnt.size()) return;
	vector< CFont2* >::iterator it = m_fnt.begin()+(size_t)nPos;
	m_fnt.erase(it);
}

// ** Clear all the Font
void CMaxScreen::ClearFont(void)
{
	m_fnt.clear();
}

TCHAR* tc_OBJTYPE[] =
{_T("IMAGE"), _T("VIDEO"), _T("TEXT")};

TCHAR* tc_TRANSEF[] =
{_T("NULL"), _T("NODRAW"), _T("APPEAR"), _T("XAPPEAR"), _T("YAPPEAR"), _T("DEGAPPEAR")};

TCHAR* tc_TRANSMOD[] =
{_T("LINEAL"), _T("HSTEP"), _T("POW"), _T("SIN"), _T("PLINEAL"),
_T("PTRIANGLE"), _T("PRECT"), _T("SIN_R"), _T("PTRIANGLE_R")};

TCHAR* tc_TRANSSPSTAT[] =
{_T("PLAY"), _T("PAUSE"), _T("STOP"), _T("PLAYLOOP"), _T("PLAYONCE"),
_T("PLAYONCEBEG")};

TCHAR* tc_SOUNDSMOD[] =
{_T("ONCE"), _T("LOOP"), _T("STOP"), _T("PAUSE"), _T("PLAY")};
