#ifndef PORT_BUF
#define PORT_BUF

#include "stdafx.h"

typedef void (CALLBACK* PORTOUT) (int, unsigned char);

class CPortBuf
{
public:
	CPortBuf(void);
	virtual ~CPortBuf(void);
	// Inicializa el puerto
	int digitalio(int nPortId);
	int m_nPortId;
	int m_nWriteBuf, m_nReadBuf, m_nControlBuf;
	int Read(int nBit);
	void Write(int nBit, bool bRef);
	void WriteVal(int nVal);
	void ResetWrite(void);

public:
	HMODULE hFile;
	PORTOUT PortOut;
};

#endif //PORT_BUF