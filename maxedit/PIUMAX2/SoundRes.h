#ifndef PIUMAX_SOUNDRES
#define PIUMAX_SOUNDRES

#include "SoundStream.h"

class CSoundRes
{
public:
	CSoundRes(void);
	~CSoundRes(void);
	vector< CSoundStream* > m_stream;
	int AddSound(CSoundStream* sprite);
	int AddSound(LPCTSTR strName, DWORD dwFlags);
	int GetSize(void);
	int InsertSound(CSoundStream* sprite, int nPos);
	int InsertSound(LPCTSTR strName, DWORD dwFlags, int nPos);
	void DeleteSound(int nPos);
	void Clear(void);
	CSoundStream* At(int nPos);
};

#endif // PIUMAX_SOUNDRES