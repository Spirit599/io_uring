#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include <memory>
#include "Event_loop.h"
#include <arpa/inet.h>
#include <string>
#include <functional>

class Socket;


class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
#define MAX_MESSAGE_LEN 2048

    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::function<void (const TcpConnectionPtr&)> TcpConnCallback;
    typedef std::function<void (const TcpConnectionPtr&, char*)> MessageCallback;
private:
    EventLoop* loop_;
    sockaddr_in clientAddr_;
    std::string ipPort_;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    int status_;

    char* IOUringBuf_;
    char* IOUringBufBid;

    TcpConnCallback connectionCallback_;
    MessageCallback messageCallback_;
    TcpConnCallback writeCompleteCallback_;
    TcpConnCallback closeCallback_;

    std::string inputBuffer_;
    std::string outputBuffer_;

    void handleRead();
    void handleWrite();
    void handleClose();
    void handleProvBuf();


public:
    TcpConnection(EventLoop* loop,
                int sockfd,
                const std::string& ipPort,
                const sockaddr_in& clientAddr,
                int channelType);
    ~TcpConnection();

    void setConnectionCallback(const TcpConnCallback& cb);
    void setMessageCallback(const MessageCallback& cb);
    void setWriteCompleteCallback(const TcpConnCallback& cb);
    void setCloseCallback(const TcpConnCallback& cb);
    void send(const char* sendMessage);

    void connectEstablished();

    std::string ipPort();
};



#endif