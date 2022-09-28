#ifndef __ACCEPTER_H__
#define __ACCEPTER_H__

#include "Event_loop.h"
#include "Socket.h"
#include "Sockets.h"
#include "liburing.h"
#include <arpa/inet.h>


class Acceptor
{
    typedef std::function<void (int, const sockaddr_in&)> NewConnectionCallback;
private:
    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listening_;

    void handleRead();
    void setAcceptorToPoller();

public:
    Acceptor(EventLoop* loop, unsigned short port);
    ~Acceptor();
    void listen();
    bool listening() const;
    void setNewConnectionCallback(const NewConnectionCallback& cb);

    struct sockaddr_in clientAddr_;
    socklen_t clientAddrLen_;
};

#endif