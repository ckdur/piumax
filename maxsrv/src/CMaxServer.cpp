#include "CMaxServer.h"

#include "CMaxTcpCon.h"

CMaxServer::CMaxServer(boost::asio::io_service& io_service)
    : acceptor_tcp(io_service, tcp::endpoint(tcp::v4(), 13))
    : socket_udp(io_service, udp::endpoint(udp::v4(), 13))
{
    start_accept_tcp(); // For TCP
    start_receive_udp(); // For UDP
}

CMaxServer::~CMaxServer()
{
    //dtor
}

void CMaxServer::start_receive_udp()
{
    socket_udp.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&CMaxServer::handle_receive_udp, this, recv_buffer_
          boost::asio::placeholders::error));
}

void CMaxServer::handle_receive_udp(boost::shared_ptr<std::string> recv_buffer,
                                    const boost::system::error_code& error)
{
    // This UDP receive udp is only for 128bytes commands
    if (!error || error == boost::asio::error::message_size)
    {
        // TODO: do something with the data
        // TODO: do something with the message
        /*boost::shared_ptr<std::string> message(
          new std::string(make_daytime_string()));

        socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
          boost::bind(&udp_server::handle_send, this, message));*/

      start_receive_udp();
    }
}

void CMaxServer::handle_send_udp(boost::shared_ptr<std::string> message)
{
    // TODO: Maybe there is something to do here
}

void CMaxServer::start_accept_tcp()
{
    tcp_connection::pointer new_connection =
      tcp_connection::create(acceptor_tcp.get_io_service());

    acceptor_tcp.async_accept(new_connection->socket(),
        boost::bind(&CMaxServer::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
}
