// CMaxChecksum.cpp
// Definitions for ckecksums
// Simple to use, you'll invoke the context using CMaxChecksum(ID)
// Depending of ID using any MAXCHECKSUM_ENUM member, you'll
// obtain any checksum for any specified data using DoChecksum(data, size, checksum)
// PLEASE: Use this only ONCE, this is not compatible if you recursively
// use this function. Also, if you want to know the size of checksum, you can use
// GetSizeOfChecksum() for preallocating this.

#include "CMaxChecksum.h"

CMaxChecksum::CMaxChecksum(unsigned int id_checksum)
{
    m_checksum = id_checksum;
    if (id_checksum >= MAXCHECKSUM_SIZE) m_checksum = MAXCHECKSUM_NULL;
}

CMaxChecksum::~CMaxChecksum()
{
    //dtor
    switch(m_checksum)
    {
    case MAXCHECKSUM_CRC32:
        break;
    case MAXCHECKSUM_MD5SUM:
        break;
    case MAXCHECKSUM_SHA1SUM:
        break;
    }
}

size_t CMaxChecksum::GetSizeOfChecksum()
{
    switch(m_checksum)
    {
    case MAXCHECKSUM_NULL:
        return 1;
        break;
    case MAXCHECKSUM_CRC32:
        return 4;
        break;
    case MAXCHECKSUM_CRC16:
        return 2;
        break;
    case MAXCHECKSUM_MD5SUM:
        return MD5_DIGEST_LENGTH;
        break;
    case MAXCHECKSUM_SHA1SUM:
        return SHA_DIGEST_LENGTH;
        break;
    }
    return 1;
}

void CMaxChecksum::DoCheckSum(void* data, size_t s, char* checksum)
{
    char* str = (char*)data;
    switch(m_checksum)
    {
    case MAXCHECKSUM_NULL:
        (*checksum) = 0;
        break;
    case MAXCHECKSUM_CRC32:
        try
        {
            boost::crc_32_type  result;
            result.process_bytes( data, s );
            uint32_t check = result.checksum();
        }
        catch ( std::exception &e )
        {
            std::cerr << "CMaxChecksum::DoCheckSum() with MAXCHECKSUM_CRC32: Found an exception with '" << e.what() << "'." << std::endl;
            return;
        }
        catch ( ... )
        {
            std::cerr << "CMaxChecksum::DoCheckSum() with MAXCHECKSUM_CRC32: Found an unknown exception." << std::endl;
            return;
        }
        break;
    case MAXCHECKSUM_CRC16:
        try
        {
            boost::crc_16_type  result;
            result.process_bytes( data, s );
            uint32_t check = result.checksum();
        }
        catch ( std::exception &e )
        {
            std::cerr << "CMaxChecksum::DoCheckSum() with MAXCHECKSUM_CRC16: Found an exception with '" << e.what() << "'." << std::endl;
            return;
        }
        catch ( ... )
        {
            std::cerr << "CMaxChecksum::DoCheckSum() with MAXCHECKSUM_CRC16: Found an unknown exception." << std::endl;
            return;
        }
        break;
    case MAXCHECKSUM_MD5SUM:
        {
            MD5_CTX c;
            MD5_Init(&c);
            size_t length = s;
            while (length > 0) {
                if (length > 512) {
                    MD5_Update(&c, str, 512);
                } else {
                    MD5_Update(&c, str, length);
                }
                length -= 512;
                str += 512;
            }
            MD5_Final(checksum, &c);
            break;
        }
    case MAXCHECKSUM_SHA1SUM:
        {
            SHA_CTX c;
            SHA1_Init(&c);
            size_t length = s;
            while (length > 0) {
                if (length > 512) {
                    SHA1_Update(&c, str, 512);
                } else {
                    SHA1_Update(&c, str, length);
                }
                length -= 512;
                str += 512;
            }
            SHA1_Final(checksum, &c);
            break;
        }
        break;
    }
}
