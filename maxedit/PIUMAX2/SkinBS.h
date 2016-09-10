#ifndef MAX_SKINBS
#define MAX_SKINBS

#include "cmnpiu.h"

enum SKINBS
{
	SKINBS_STITEM,
	SKINBS_NUMBERS,
	SKINBS_CHARACTERS,
	SKINBS_FORMS,
	SKINBS_ARROWS,
	SKINBS_CHARACTERS_SHADOW,
	SKINBS_FORMS_SHADOW,
	SKINBS_ARROWS_SHADOW,
	SKINBS_XO,
	SKINBS_RIGHT,
	SKINBS_WRONG,
	SKINBS_POPUPBG,
	SKINBS_CENTERBAR,
	SKINBS_CENTERSPACE,
	SKINBS_LATERALBAR,
	SKINBS_FRONTBAR,
	SKINBS_CLOCK,
	SKINBS_BOTTOMQ,
	SKINBS_QUESTIONY,
	SKINBS_CIRCLE,
	SIZE_SPRITEBS
};

class CSkinBS
{
public:
	CSkinBS(void);
	~CSkinBS(void);
	CSprite* skin[SIZE_SPRITEBS];
	int LoadSkin(void);
	void Update(DWORD dwMiliseconds);
	void Delete();
};

#endif
