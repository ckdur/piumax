#ifndef MAXSRV_STDAFX
#define MAXSRV_STDAFX

// Common
#include <iostream>
#include <ctime>
#include <string>

// For asio
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

// For checksums
#include <openssl/md5.h>
#include <boost/crc.hpp>
#include <openssl/sha.h>

#endif // MAXSRV_STDAFX
