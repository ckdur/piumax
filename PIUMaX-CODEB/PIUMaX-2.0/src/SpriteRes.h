#ifndef PIUMAX_SPRITERES
#define PIUMAX_SPRITERES

#include "OGLSprite.h"

class CSpriteRes
{
public:
	CSpriteRes(void);
	~CSpriteRes(void);
	vector< CSprite* > m_sprite;
	int AddSprite(CSprite* sprite);
	int AddSprite(LPCTSTR strName, int nDivX, int nDivY);
	int GetSize(void);
	int InsertSprite(CSprite* sprite, int nPos);
	int InsertSprite(LPCTSTR strName, int nDivX, int nDivY, int nPos);
	void DeleteSprite(int nPos);
	void Clear(void);
	CSprite* At(int nPos);
	int AtTex(int nPos, CGraphTexture** tex);
	void UpdateAll(DWORD dwMiliseconds);
	void Update(int nPos, DWORD dwMiliseconds);
};

#endif // PIUMAX_SPRITERES