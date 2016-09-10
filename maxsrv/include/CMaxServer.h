#ifndef CMAXSERVER_H
#define CMAXSERVER_H

#include "stdafx.h"

class CMaxServer
{
public:
    CMaxServer(boost::asio::io_service& io_service);
    virtual ~CMaxServer();
protected:
private:
    void start_accept_tcp();
    void start_receive_udp();

    udp::socket socket_udp;
    udp::endpoint remote_endpoint_udp;
    tcp::acceptor acceptor_tcp;
    boost::array<char, 128> recv_buffer_;
};

#endif // CMAXSERVER_H
