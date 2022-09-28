#include "Event_loop.h"
#include "Channel.h"
#include "Thread.h"
#include "Acceptor.h"
#include <cstdio>
#include <assert.h>

EventLoop::EventLoop()
    : looping_(false)
    , quit_(false)
    , eventHandling_(false)
    , threadId_(gettid())
    , poller_(Poller::newDefaultPoller(this))
    , currentActiveChannel_(NULL)
{}

EventLoop::~EventLoop()
{}

void EventLoop::loop()
{
    assert(!looping_);
    looping_ = true;
    quit_ = false;

    while(!quit_)
    {
        
        activeChannels_.clear();
        
        poller_->poll(&activeChannels_);

        eventHandling_ = true;
        for(Channel* channel : activeChannels_)
        {
            currentActiveChannel_ = channel;

            currentActiveChannel_->handleEvent();
        }

        //sleep(1);
    }
}

void EventLoop::quit()
{
    quit_ = true;
}

void EventLoop::updateChannel(Channel* channel)
{
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
    poller_->removeChannel(channel);
}

void EventLoop::setAcceptorToPoller(Acceptor* acceptor)
{
    poller_->setAcceptorToPoller(acceptor);
}

Poller* EventLoop::poller()
{
    return poller_;
}