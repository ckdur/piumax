
#include "SpriteRes.h"

CSpriteRes::CSpriteRes(void)
{
}

CSpriteRes::~CSpriteRes(void)
{
	Clear();
}

int CSpriteRes::AddSprite(CSprite* sprite)
{
	m_sprite.push_back(sprite);
	return m_sprite.size();
}

int CSpriteRes::AddSprite(LPCTSTR strName, int nDivX, int nDivY)
{
	CSprite* sprite = new CSprite();
	(*sprite).InitImage_mn(strName, nDivX, nDivY);
	m_sprite.push_back(sprite);
	return m_sprite.size();
}

int CSpriteRes::GetSize(void)
{
	return m_sprite.size();
}

int CSpriteRes::InsertSprite(CSprite* sprite, int nPos)
{
	if(nPos < 0 || (size_t)nPos > m_sprite.size()) {_printf(_T("CSpriteRes: OUT OF BOUNDS (pos: %d)"), nPos); return 0;}
	vector< CSprite* >::iterator it = m_sprite.begin()+(size_t)nPos;
	m_sprite.insert(it, sprite);
	return 1;
}

int CSpriteRes::InsertSprite(LPCTSTR strName, int nDivX, int nDivY, int nPos)
{
	if(nPos < 0 || (size_t)nPos > m_sprite.size()) return 0;
	CSprite* sprite = new CSprite();
	(*sprite).InitImage_mn(strName, nDivX, nDivY);
	vector< CSprite* >::iterator it = m_sprite.begin()+(size_t)nPos;
	m_sprite.insert(it, sprite);
	return 0;
}
void CSpriteRes::DeleteSprite(int nPos)
{
	if(nPos < 0 || (size_t)nPos >= m_sprite.size()) return;
	vector< CSprite* >::iterator it = m_sprite.begin()+(size_t)nPos;
	if((*it) != NULL)
	{
		(**it).DeleteImage();
		delete (*it);
	}
	m_sprite.erase(it);
}

void CSpriteRes::Clear(void)
{
	vector< CSprite* >::iterator it;
	for(it = m_sprite.begin(); it != m_sprite.end(); it++)
	{
		if((*it) != NULL)
		{
			(**it).DeleteImage();
			delete (*it);
		}
	}
	m_sprite.clear();
}

CSprite* CSpriteRes::At(int nPos)
{
	if(nPos < 0 || (size_t)nPos >= m_sprite.size()) return NULL;
	return m_sprite.at((size_t)nPos);
}

int CSpriteRes::AtTex(int nPos, CGraphTexture** tex)
{
	if(nPos < 0 || (size_t)nPos >= m_sprite.size()) return 0;
	*tex = m_sprite.at((size_t)nPos)->GetCurrentTex();
	return 1;
}

void CSpriteRes::UpdateAll(DWORD dwMiliseconds)
{
	vector< CSprite* >::iterator it;
	for(it = m_sprite.begin(); it != m_sprite.end(); it++)
	{
		if((*it) != NULL) (**it).Update(dwMiliseconds);
	}
}

void CSpriteRes::Update(int nPos, DWORD dwMiliseconds)
{
	if(nPos < 0 || (size_t)nPos >= m_sprite.size()) return;
	if(m_sprite[nPos] != NULL) (*m_sprite[nPos]).Update(dwMiliseconds);
}
