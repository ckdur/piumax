

#ifndef PORT_BUF
#define PORT_BUF

#include "stdafx.h"

#ifdef _WIN32
typedef void (CALLBACK* PORTOUT) (int, unsigned char);
#endif

class CPortBuf
{
public:
	CPortBuf(void);
	virtual ~CPortBuf(void);
	// Inicializa el puerto
	int digitalio(int nPortId);
	int m_nPortId;
	int32_t m_nWriteBuf, m_nReadBuf, m_nControlBuf;
	int Read(int nBit);
	void Write(int nBit, bool bRef);
	void WriteVal(int nVal);
	void ResetWrite(void);

public:
    bool m_bIsPort;
#ifdef _WIN32
	HMODULE hFile;
	PORTOUT PortOut;
#endif
};

#endif //PORT_BUF
