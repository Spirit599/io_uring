#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include <sys/types.h>
#include <vector>
#include <memory>
#include "Poller.h"

class Channel;
class Poller;
class Acceptor;

class EventLoop
{
private:
    bool looping_;
    bool quit_;
    bool eventHandling_;
    pid_t threadId_;
    //std::unique_ptr<Poller> poller_;
    Poller* poller_;
    std::vector<Channel*> activeChannels_;
    Channel* currentActiveChannel_;
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    void setAcceptorToPoller(Acceptor* acceptor);
    Poller* poller();
};


#endif