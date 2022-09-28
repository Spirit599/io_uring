#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "Event_loop.h"
#include <map>
#include <string>
#include<arpa/inet.h>

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;


class TcpServer
{
    
    typedef std::function<void (const TcpConnectionPtr&)> TcpConnCallback;
    typedef std::function<void (const TcpConnectionPtr&, char*)> MessageCallback;
private:
    EventLoop* loop_;
    unsigned short port_;
    std::unique_ptr<Acceptor> acceptor_;
    TcpConnCallback connectionCallback_;
    MessageCallback messageCallback_;
    TcpConnCallback writeCompleteCallback_;
    TcpConnCallback closeCallback_;
    std::map<std::string, TcpConnectionPtr> connections_;
    

public:
    TcpServer(EventLoop* loop, unsigned short port);
    ~TcpServer();
    EventLoop* getLoop() const;
    void listen();

    void newConnection(int sockfd, const sockaddr_in& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);

    void setConnectionCallback(const TcpConnCallback& cb);
    void setMessageCallback(const MessageCallback& cb);
    void setWriteCompleteCallback(const TcpConnCallback& cb);
};

#endif