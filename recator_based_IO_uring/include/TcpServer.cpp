#include "TcpServer.h"
#include "Acceptor.h"
#include "TcpConnection.h"

#include <cstring>
#include <functional>

TcpServer::TcpServer(EventLoop* loop, unsigned short port)
    : loop_(loop)
    , port_(port)
    , acceptor_(new Acceptor(loop, port))
    , connectionCallback_(NULL)
    , messageCallback_(NULL)
{
    acceptor_->setNewConnectionCallback(
        std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
    //need handle connections
}

void TcpServer::newConnection(int sockfd, const sockaddr_in& clientAddr)
{
    char client_ip[16];
	memset(client_ip, 0, sizeof(client_ip));
    inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, client_ip, sizeof(client_ip));
    std::string ipPort(client_ip);
    ipPort.push_back(':');
    ipPort += std::to_string(ntohs(clientAddr.sin_port));
    //printf("new connection ipPort = %s fd = %d\n", ipPort.c_str(), sockfd);


    TcpConnectionPtr conn(new TcpConnection(loop_,
                                            sockfd,
                                            ipPort,
                                            clientAddr,
                                            CHANNEL_CONNECTION));

    connections_[ipPort] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);

    conn->connectEstablished();
}

void TcpServer::setConnectionCallback(const TcpConnCallback& cb)
{
    connectionCallback_ = cb;
}
void TcpServer::setMessageCallback(const MessageCallback& cb)
{
    messageCallback_ = cb;
}

void TcpServer::listen()
{
    acceptor_->listen();
}

