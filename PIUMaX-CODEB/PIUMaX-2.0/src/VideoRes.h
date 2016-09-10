#ifndef PIUMAX_VIDEORES
#define PIUMAX_VIDEORES

#include "MovieStream2.h"
#include "GraphTexture.h"

class CVideoRes
{
public:
	CVideoRes(void);
	~CVideoRes(void);
	vector< CMovieStream2* > m_video;
	int AddVideo(CMovieStream2* video);
	int AddVideo(LPCTSTR strName, bool bLoop);
	int GetSize(void);
	int InsertVideo(CMovieStream2* video, int nPos);
	int InsertVideo(LPCTSTR strName, bool bLoop, int nPos);
	void DeleteVideo(int nPos);
	void Clear(void);
	CMovieStream2* At(int nPos);
	void UpdateAll(DWORD dwMiliseconds);
	void Update(int nPos, DWORD dwMiliseconds);
	int AtTex(int nPos, CGraphTexture** tex);
};

#endif // PIUMAX_VIDEORES