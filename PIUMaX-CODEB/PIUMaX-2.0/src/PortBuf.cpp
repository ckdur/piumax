#include "PortBuf.h"

CPortBuf::CPortBuf(void)
{
	hFile = NULL;
	PortOut = NULL;
}

CPortBuf::~CPortBuf(void)
{
	WriteVal(0);
	if(hFile != NULL) FreeLibrary(hFile);
}

// Inicializa el puerto
int CPortBuf::digitalio(int nPortId)
{
	hFile = LoadLibrary(_T("IO.DLL"));
	if(hFile == NULL) return FALSE;
	else
	{
		PortOut = (PORTOUT)GetProcAddress(hFile, "PortOut");
		if(PortOut == NULL) return FALSE;
		else
		{
			// Aqui se inicializa
			PortOut(nPortId, 0);
			m_nPortId = nPortId;
		}
	}

	return TRUE;
}

int CPortBuf::Read(int nBit)
{
	int nTemp = 1<<nBit;
	
	return nTemp & m_nReadBuf;
}

void CPortBuf::Write(int nBit, bool bRef)
{
	if(PortOut == NULL) return;
	int nTemp = 1<<nBit;
	if(bRef) m_nWriteBuf |= nTemp;
	else m_nWriteBuf &= ~nTemp;
	PortOut(m_nPortId, m_nWriteBuf);
}

void CPortBuf::WriteVal(int nVal)
{
	if(PortOut == NULL) return;
	m_nWriteBuf = nVal;
	PortOut(m_nPortId, m_nWriteBuf);
}

void CPortBuf::ResetWrite(void)
{
	if(PortOut == NULL) return;
	PortOut(m_nPortId, 0);
	m_nWriteBuf = 0;
}
