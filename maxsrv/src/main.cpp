#include "stdafx.h"
#include "CMaxServer.h"

int main()
{
    std::cout << "Afix Systems (R) maxsrv for PIUMaX 2" << std::endl;
    std::cout << "2010 - 2015" << std::endl;

#ifdef DEBUG
    std::cout << "This is a Debug release!" << std::endl;
#endif

    try
    {
        boost::asio::io_service io_service;
        CMaxServer server(io_service);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
