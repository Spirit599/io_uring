#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <functional>
#include "EventType.h"
#include "liburing.h"

class EventLoop;

class Channel
{
    typedef std::function<void()> EventCallback;

private:
    EventLoop* loop_;
    int fd_;
    int events_;
    int bid_;
    io_uring_cqe* cqe_;
    bool eventHandling_;
    int status_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback provBufCallback_;
    int channelType_;

    void update();
    void handleEventWithGuard();

public:
    Channel(EventLoop* loop, int fd, int channelType);
    ~Channel();
    void handleEvent();
    void setReadCallback(const EventCallback& cb);
    void setWriteCallback(const EventCallback& cb);
    void setProvBufCallback(const EventCallback& cb);
    void setCloseCallback(const EventCallback& cb);
    int channelType() const;
    int fd() const;
    int events() const;
    io_uring_cqe* cqe() const;
    void set_cqe(io_uring_cqe*);
    int status() const;
    void set_status(int sta);
    int bid() const;
    void set_bid(int bid);
    void remove();
    void enableReading();
    void disableReading();
    void enableWriting();
    void disableWriting();
    void enableProvBuf();
    void disableProvBuf();
    
};

#endif