#include "Acceptor.h"
#include <cstring>

Acceptor::Acceptor(EventLoop* loop, unsigned short port)
    : loop_(loop)
    , acceptSocket_(socketNonblocking())
    , acceptChannel_(loop, acceptSocket_.fd(), CHANNEL_ACCEPT)
    , listening_(false)
    , clientAddrLen_((sizeof(struct sockaddr)))
{
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bindAddress(port);
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));

    setAcceptorToPoller();
}

Acceptor::~Acceptor()
{

}

void Acceptor::listen()
{
  listening_ = true;
  acceptSocket_.listen();
  acceptChannel_.enableReading();
}

void Acceptor::handleRead()
{
    acceptChannel_.enableReading();

    int sock_conn_fd = acceptChannel_.cqe()->res;
    if(sock_conn_fd < 0)
        return ;

    if(newConnectionCallback_)
    {
        newConnectionCallback_(sock_conn_fd, clientAddr_);
    }

    
}

bool Acceptor::listening() const
{
    return listening_;
}

void Acceptor::setNewConnectionCallback(const NewConnectionCallback& cb)
{
    newConnectionCallback_ = cb;
}

void Acceptor::setAcceptorToPoller()
{
    loop_->setAcceptorToPoller(this);
}