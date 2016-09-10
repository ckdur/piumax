#ifndef MAX_DIALOG
#define MAX_DIALOG

#include "MaxDataExchange.h"

class CMaxDialog
{
public:
	CMaxDialog(void);
	~CMaxDialog(void);

	virtual int DoModal(void);
	virtual int DoNoModal(void);
};

#endif // MAX_DIALOG