#include "PortBuf.h"
#include "cmnpiu.h"

#define FALSE 0
#define TRUE 1

CPortBuf::CPortBuf(void)
{
    m_bIsPort = false;
    m_nPortId = 0;
    m_nWriteBuf = 0xF803FB83;
    m_nReadBuf = 0;
    m_nControlBuf = 0;
#ifdef _WIN32
	hFile = NULL;
	PortOut = NULL;
#endif
}

CPortBuf::~CPortBuf(void)
{
#ifdef _WIN32
	WriteVal(0);
	if(hFile != NULL) FreeLibrary(hFile);
#endif
}

// Inicializa el puerto
int CPortBuf::digitalio(int nPortId)
{
#ifdef _WIN32
    if(nPortId == 0) return TRUE;
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
#else
    if(g_ihPIUIO == NULL) return FALSE;
    if(g_ihPIUIO->m_bFoundDevice)
    {
        g_ihPIUIO->m_iLightData = 0;
    }
    return TRUE;
#endif
}

int CPortBuf::Read(int nBit)
{
#ifdef _WIN32
	int nTemp = 1<<nBit;

	return nTemp & m_nReadBuf;
#else
    return 0; // NOTHING TO READ
#endif // _WIN32
}

void CPortBuf::Write(int nBit, bool bRef)
{
#ifdef _WIN32
	if(PortOut == NULL) return;
	int nTemp = 1<<nBit;
	if(bRef) m_nWriteBuf |= nTemp;
	else m_nWriteBuf &= ~nTemp;
	PortOut(m_nPortId, m_nWriteBuf);
#else
    if(nBit < 0) return;
    int32_t nTemp = 1<<nBit;
	if(bRef) m_nWriteBuf |= nTemp;
	else m_nWriteBuf &= ~nTemp;
	if(g_ihPIUIO->m_bFoundDevice)
    {
        g_ihPIUIO->m_iLightData = m_nWriteBuf;
    }
#endif
}

void CPortBuf::WriteVal(int32_t nVal)
{
#ifdef _WIN32
	if(PortOut == NULL) return;
	m_nWriteBuf = nVal;
	PortOut(m_nPortId, m_nWriteBuf);
#else
    for(unsigned i = 0; i < 3; i++)
    {
        int n1 = 0, n2 = 0;
        if(i == 0) {n1 = 10; n2 = 10;}
        if(i == 1) {n1 = 26; n2 = 25;}
        if(i == 2) {n1 = 24; n2 = 23;}
        if(nVal & (1 << i))
        {
            m_nWriteBuf |= ((int32_t)(1 << n1));
            m_nWriteBuf |= ((int32_t)(1 << n2));
        }
        else
        {
            m_nWriteBuf &= ~((int32_t)(1 << n1));
            m_nWriteBuf &= ~((int32_t)(1 << n2));
        }
    }
	if(g_ihPIUIO->m_bFoundDevice)
    {
        g_ihPIUIO->m_iLightData = m_nWriteBuf;
    }
#endif
}

void CPortBuf::ResetWrite(void)
{
#ifdef _WIN32
	if(PortOut == NULL) return;
	PortOut(m_nPortId, 0);
	m_nWriteBuf = 0;
#else
    m_nWriteBuf = 0;
	if(g_ihPIUIO->m_bFoundDevice)
    {
        g_ihPIUIO->m_iLightData = m_nWriteBuf;
    }
#endif
}
