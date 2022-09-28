#ifndef __IO_URING_POLLER_H__
#define __IO_URING_POLLER_H__

#include "liburing.h"
#include "Channel.h"
#include "Poller.h"
#include "Acceptor.h"
#include <arpa/inet.h>
#include <errno.h>


class IOUringPoller : public Poller
{
private:
    
    
    struct io_uring_params IOUringParams_;
    struct io_uring IORing_;
    unsigned int groupId_;
    unsigned int bufAmount_;
    unsigned int queueSize_;
    struct io_uring_sqe *sqe_;
    struct io_uring_cqe *cqe_;

    void checkFastPoll();
    void checkBufferSelection();

    void addAccept(int fd, unsigned flags, Channel* channel);
    void addRead(int fd, unsigned flags, Channel* channel);
    void addWrite(int fd, unsigned flags, Channel* channel);
    void addProvideBuf(int fd, unsigned flags, Channel* channel);

    void fillActiveChannels(int numEvents, std::vector<Channel*>* activeChannels) const;
    void update(int operation, Channel* channel);
public:
    
    IOUringPoller(EventLoop* loop);
    ~IOUringPoller() override;
    virtual void poll(std::vector<Channel*>* activeChannels) override;
    virtual void updateChannel(Channel* channel) override;
    virtual void removeChannel(Channel* channel) override;
};




#endif