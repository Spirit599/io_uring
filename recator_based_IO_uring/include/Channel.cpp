#include "Channel.h"
#include "Event_loop.h"
#include <cstdio>

Channel::Channel(EventLoop* loop, int fd, int channelType)
    : loop_(loop)
    , fd_(fd)
    , events_(0)
    , bid_(0)
    , cqe_(NULL)
    , eventHandling_(false)
    , status_(CHANNEL_NEW)
    , channelType_(channelType)
{

}

Channel::~Channel()
{

}

void Channel::handleEvent()
{
    handleEventWithGuard();
}

void Channel::handleEventWithGuard()
{
    eventHandling_ = true;

    if(events_ == EVENT_ACCEPT)
    {
        readCallback_();
    }
    else if(events_ == EVENT_READ)
    {
        readCallback_();
    }
    else if(events_ == EVENT_WRITE)
    {
        writeCallback_();
    }
    else if(events_ == EVENT_PROVBUF)
    {
        provBufCallback_();
    }
    eventHandling_ = false;

}

void Channel::setReadCallback(const EventCallback& cb)
{
    readCallback_ = cb;
}

void Channel::setWriteCallback(const EventCallback& cb)
{
    writeCallback_ = cb;
}

void Channel::setCloseCallback(const EventCallback& cb)
{
    closeCallback_ = cb;
}

void Channel::setProvBufCallback(const EventCallback& cb)
{
    provBufCallback_ = cb;
}

int Channel::channelType() const
{
    return channelType_;
}

int Channel::fd() const
{
    return fd_;
}

int Channel::events() const
{
    return events_;
}

io_uring_cqe* Channel::cqe() const
{
    return cqe_;
}

void Channel::set_cqe(io_uring_cqe* cqe)
{
    cqe_ = cqe;
}

int Channel::status() const
{
    return status_;
}

void Channel::set_status(int sta)
{
    status_ = sta;
}

int Channel::bid() const
{
    return bid_;
}

void Channel::set_bid(int bid)
{
    bid_ = bid;
}


void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::remove()
{
    loop_->removeChannel(this);
}

void Channel::enableReading()
{
    if(channelType_ ==  CHANNEL_ACCEPT)
        events_ = EVENT_ACCEPT;
    if(channelType_ == CHANNEL_CONNECTION)
        events_ = EVENT_READ;

    update();
}

void Channel::disableReading()
{
    if(channelType_ ==  CHANNEL_ACCEPT)
        events_ &= ~EVENT_ACCEPT;
    if(channelType_ == CHANNEL_CONNECTION)
        events_ &= ~EVENT_READ;
    


    update();
}

void Channel::enableWriting()
{
    if(channelType_ == CHANNEL_CONNECTION)
        events_ = EVENT_WRITE;

    update();
}

void Channel::disableWriting()
{
    if(channelType_ == CHANNEL_CONNECTION)
        events_ &= ~EVENT_WRITE;


    update();
}

void Channel::enableProvBuf()
{
    if(channelType_ == CHANNEL_CONNECTION)
        events_ = EVENT_PROVBUF;

    update();
}

void Channel::disableProvBuf()
{
    if(channelType_ == CHANNEL_CONNECTION)
        events_ &= ~EVENT_PROVBUF;

    update();
}