#ifndef __POLLER_H__
#define __POLLER_H__

#include <vector>
#include <map>
#include "Channel.h"
#include "Event_loop.h"

class Acceptor;

class Poller
{
#define MAX_CONNECTIONS 4096
#define MAX_MESSAGE_LEN 2048

private:
    EventLoop* ownerLoop_;
protected:
    std::map<int, Channel*> channels_;
    Acceptor* acceptor_;
public:
    char bufs_[MAX_CONNECTIONS][MAX_MESSAGE_LEN];

    Poller(EventLoop* loop);
    virtual ~Poller() = 0;
    virtual void poll(std::vector<Channel*>* activeChannels) = 0;
    virtual void updateChannel(Channel* channel) = 0;
    void updateChanneltest(Channel* channel);
    virtual void removeChannel(Channel* channel) = 0;
    virtual bool hasChannel(Channel* channel) const;
    static Poller* newDefaultPoller(EventLoop* loop);
    void setAcceptorToPoller(Acceptor* acceptor);
};

#endif