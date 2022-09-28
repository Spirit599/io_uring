#include "TcpConnection.h"
#include "Socket.h"
#include "EventType.h"
#include <assert.h>
#include <cstring>


TcpConnection::TcpConnection(EventLoop* loop,
                            int sockfd,
                            const std::string& ipPort,
                            const sockaddr_in& clientAddr,
                            int channelType)
    : loop_(loop)
    , clientAddr_(clientAddr)
    , ipPort_(ipPort)
    , socket_(new Socket(sockfd))
    , channel_(new Channel(loop, sockfd, CHANNEL_CONNECTION))
    , status_(CONNECTING)
    , IOUringBuf_((loop_->poller()->bufs_)[0])
{
    channel_->setReadCallback(
        std::bind(&TcpConnection::handleRead, this));
    channel_->setWriteCallback(
        std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(
        std::bind(&TcpConnection::handleClose, this));
    channel_->setProvBufCallback(
        std::bind(&TcpConnection::handleProvBuf, this));
}

TcpConnection::~TcpConnection()
{
    printf("~TcpConnection()\n");
}

void TcpConnection::handleRead()
{
    int bytes_read = channel_->cqe()->res;
    if(bytes_read <= 0)
    {
        shutdown(socket_->fd(), SHUT_RDWR);
        return ;
    }

    int bid = (channel_->cqe()->flags) >> 16;
    channel_->set_bid(bid);

    *(IOUringBuf_ + bid * MAX_MESSAGE_LEN + bytes_read) = '\0';

    IOUringBufBid = IOUringBuf_ + bid * MAX_MESSAGE_LEN;


    if(messageCallback_)
        messageCallback_(shared_from_this(), IOUringBufBid);

}

void TcpConnection::handleWrite()
{

    channel_->enableProvBuf();
}

void TcpConnection::handleClose()
{
    
}

void TcpConnection::handleProvBuf()
{

    channel_->enableReading();
}

void TcpConnection::setConnectionCallback(const TcpConnCallback& cb)
{
    connectionCallback_ = cb;
}

void TcpConnection::setMessageCallback(const MessageCallback& cb)
{
    messageCallback_ = cb;
}

void TcpConnection::connectEstablished()
{

    assert(status_ == CONNECTING);
    status_ = CONNECTED;

    channel_->enableReading();

    if(connectionCallback_)
        connectionCallback_(shared_from_this());
}

std::string TcpConnection::ipPort()
{
    return ipPort_;
}

void TcpConnection::send(const char* sendMessage)
{
    memset(IOUringBufBid, 0, sizeof(char) * MAX_MESSAGE_LEN);
    memcpy(IOUringBufBid, sendMessage, sizeof(char) * MAX_MESSAGE_LEN);
    

    channel_->enableWriting();
}