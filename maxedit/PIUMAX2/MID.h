#ifndef MAX_MID
#define MAX_MID

struct MID
{
	int nId;
	int nSubId;

	MID()
	{nId = 0; nSubId = 0;};
	MID(int id, int subid)
	{nId = id; nSubId = subid;};
	MID operator = (MID id)
	{return MID(id.nId, id.nSubId);}
};

bool operator == (MID id1, MID id2);
bool operator != (MID id1, MID id2);

struct MIND
{
	int nType;
	int nIndex;
	int nSubIndex;

	MIND()
	{nType = 0; nIndex = 0; nSubIndex = 0;};
	MIND(int type, int index, int subindex)
	{nType = type; nIndex = index; nSubIndex = subindex;};
	MIND operator = (MIND ind)
	{return MIND(ind.nType, ind.nIndex, ind.nSubIndex);}
};

enum MIND_TYPE
{
	MIND_TYPE_IMAGE,		// Image resource
	MIND_TYPE_VIDEO,		// Video resource
	MIND_TYPE_TEXT,			// Text resource
	MIND_TYPE_SIZE			// SIZE OF THIS ENUM
};

#include "defbas.h"

typedef int (__stdcall *MAX_EVENT_CALLBACK)(MID, MID);

#endif // MAX_MID