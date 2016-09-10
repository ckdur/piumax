
#include "VideoRes.h"

#include "cmnpiu.h"

CVideoRes::CVideoRes(void)
{
}

CVideoRes::~CVideoRes(void)
{
	Clear();
}

int CVideoRes::AddVideo(CMovieStream2* video)
{
	m_video.push_back(video);
	return m_video.size();
}

int CVideoRes::AddVideo(LPCTSTR strName, bool bLoop)
{
	CMovieStream2* video;
	video = new CMovieStream2();
	(*video).OpenAVI(strName);
	(*video).bLoop = bLoop;
	m_video.push_back(video);
	return m_video.size();
}

int CVideoRes::GetSize(void)
{
	return m_video.size();
}

int CVideoRes::InsertVideo(CMovieStream2* video, int nPos)
{
	if(nPos < 0 || (size_t)nPos > m_video.size()) return 0;
	vector< CMovieStream2* >::iterator it = m_video.begin()+(size_t)nPos;
	m_video.insert(it, video);
	return 1;
}

int CVideoRes::InsertVideo(LPCTSTR strName, bool bLoop, int nPos)
{
	if(nPos < 0 || (size_t)nPos > m_video.size()) return 0;
	CMovieStream2* video;
	video = new CMovieStream2();
	(*video).OpenAVI(strName);
	(*video).bLoop = bLoop;
	vector< CMovieStream2*>::iterator it = m_video.begin()+(size_t)nPos;
	m_video.insert(it, video);
	return 1;
}
void CVideoRes::DeleteVideo(int nPos)
{
	if(nPos < 0 || (size_t)nPos >= m_video.size()) return;
	vector< CMovieStream2* >::iterator it = m_video.begin()+(size_t)nPos;
	if((*it) != NULL)
	{
		if((**it).bLoad)
			(**it).CloseAVI();
		delete (*it);
	}
	m_video.erase(it);
}

void CVideoRes::Clear(void)
{
	vector< CMovieStream2* >::iterator it;
	for(it = m_video.begin(); it != m_video.end(); it++)
	{
		if((*it) != NULL)
		{
			if((**it).bLoad)
				(**it).CloseAVI();
			delete (*it);
		}
	}
	m_video.clear();
}

CMovieStream2* CVideoRes::At(int nPos)
{
	if(nPos < 0 || (size_t)nPos >= m_video.size()) return NULL;
	return m_video.at((size_t)nPos);
}

void CVideoRes::UpdateAll(DWORD dwMiliseconds)
{
	vector< CMovieStream2* >::iterator it;
	for(it = m_video.begin(); it != m_video.end(); it++)
	{
		if((*it) != NULL) if((**it).bLoad) (**it).Update(dwMiliseconds);
	}
}

void CVideoRes::Update(int nPos, DWORD dwMiliseconds)
{
	if(nPos < 0 || (size_t)nPos >= m_video.size()) return;
	if(!(*m_video[nPos]).bLoad) return;
	if(m_video[nPos] != NULL)
	{
		m_video[nPos]->Update(dwMiliseconds);
		m_video[nPos]->GrabAVIFrame();
	}
}

int CVideoRes::AtTex(int nPos, CGraphTexture** tex)
{
	if(nPos < 0 || (size_t)nPos >= m_video.size()) return 0;
	// No video?
	if(!(*m_video.at((size_t)nPos)).bLoad) return 0;
	// Cannot create texture?
	/*if(!g_gmMain->CreateTexture(1, PMT_RGBA, (*m_video.at((size_t)nPos)).nwidth, (*m_video.at((size_t)nPos)).nheight, tex)) return 0;
	// Cannot Bind texture?
	if(!(*tex).TexImage2D(1, PMT_RGBA, 0, (*m_video.at((size_t)nPos)).nwidth, (*m_video.at((size_t)nPos)).nheight, (*m_video.at((size_t)nPos)).data)) return 0;*/
	*tex = m_video.at((size_t)nPos)->tex;
	//(**tex).m_bFlipY = true;	// Because FFMPEG flips Y
	return 1;
}
