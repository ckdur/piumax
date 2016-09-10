#ifndef PIUMAX_ANU
#define PIUMAX_ANU

#include "cmnpiu.h"



class CAnu
{
public:
	CAnu(void);
	~CAnu(void);
	int Open(LPCTSTR lpszPathName);
	void Destroy(void);
	void Update(DWORD dwMilliseconds);
	void SetZeroCount(void);
	ANUMAX* anu;
	void Draw(void);
	__int64 m_nNext;
	bool m_bLoop;
	bool m_bLoad;
};

#endif
