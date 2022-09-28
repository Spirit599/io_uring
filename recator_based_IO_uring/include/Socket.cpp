#include "Socket.h"
#include "Sockets.h"
#include <cstring>
#include<arpa/inet.h>

Socket::Socket(int fd)
    : sockfd_(fd)
{

}

Socket::~Socket()
{
    ::close(sockfd_);
}

int Socket::fd() const
{
    return sockfd_;
}

void Socket::bindAddress(unsigned short port, unsigned int ip)
{
    if(ip != 0)
    {
        printf("set ip = 0\n");
        exit(-1);
    }
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bindSocket(sockfd_, (struct sockaddr*)&serv_addr);
}

void Socket::listen()
{
    listenSocket(sockfd_);
}


void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}