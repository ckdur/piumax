#ifndef CMAXCHECKSUM_H
#define CMAXCHECKSUM_H

#include "stdafx.h"

enum MAXCHECKSUM_ENUM
{
    MAXCHECKSUM_NULL = 0,
    MAXCHECKSUM_CRC32,
    MAXCHECKSUM_CRC16,
    MAXCHECKSUM_MD5SUM,
    MAXCHECKSUM_SHA1SUM,
    MAXCHECKSUM_SIZE,
};

class CMaxChecksum
{
    public:
        CMaxChecksum(unsigned int id_checksum);
        virtual ~CMaxChecksum();
        size_t GetSizeOfChecksum();
        void DoCheckSum(void* data, size_t s, char* checksum);
    protected:
    private:
        unsigned int m_checksum;

};

#endif // CMAXCHECKSUM_H
