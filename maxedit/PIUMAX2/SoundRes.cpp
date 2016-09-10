
#include "SoundRes.h"
#include "cmnpiu.h"

CSoundRes::CSoundRes(void)
{
}

CSoundRes::~CSoundRes(void)
{
	Clear();
}

int CSoundRes::AddSound(CSoundStream* stream)
{
	m_stream.push_back(stream);
	return m_stream.size();
}

int CSoundRes::AddSound(LPCTSTR strName, DWORD dwFlags)
{
	CSoundStream* stream = new CSoundStream();
	int nCore = g_ssMain->CreateStream(strName, dwFlags, stream);
	if(!nCore) 
	{
		delete stream;
		stream = NULL;
	}
	m_stream.push_back(stream);
	return m_stream.size();
}

int CSoundRes::GetSize(void)
{
	return m_stream.size();
}

int CSoundRes::InsertSound(CSoundStream* stream, int nPos)
{
	if(nPos < 0 || (size_t)nPos > m_stream.size()) return 0;
	vector< CSoundStream* >::iterator it = m_stream.begin()+(size_t)nPos;
	m_stream.insert(it, stream);
	return 1;
}

int CSoundRes::InsertSound(LPCTSTR strName, DWORD dwFlags, int nPos)
{
	if(nPos < 0 || (size_t)nPos > m_stream.size()) return 0;
	CSoundStream* stream = new CSoundStream();
	g_ssMain->CreateStream(strName, dwFlags, stream);
	vector< CSoundStream* >::iterator it = m_stream.begin()+(size_t)nPos;
	m_stream.insert(it, stream);
	return 0;
}
void CSoundRes::DeleteSound(int nPos)
{
	if(nPos < 0 || (size_t)nPos <= m_stream.size()) return;
	vector< CSoundStream* >::iterator it = m_stream.begin()+(size_t)nPos;
	if((*it) != NULL) 
	{
		(**it).Release();
		delete (*it);
		(*it) = NULL;
	}
	m_stream.erase(it);
}

void CSoundRes::Clear(void)
{
	vector< CSoundStream* >::iterator it;
	for(it = m_stream.begin(); it != m_stream.end(); it++)
	{
		if((*it) != NULL) 
		{
			(**it).Release();
			delete (*it);
			(*it) = NULL;
		}
	}
	m_stream.clear();
}

CSoundStream* CSoundRes::At(int nPos)
{
	if(nPos < 0 || (size_t)nPos >= m_stream.size()) return NULL;
	return m_stream.at((size_t)nPos);
}