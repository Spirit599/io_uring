#include "IO_uring_Poller.h"
#include "cstring"
#include "cstdio"
#include <assert.h>

IOUringPoller::IOUringPoller(EventLoop* loop)
    : Poller(loop)
    , groupId_(1357)
    , bufAmount_(4096)
    , queueSize_(1024)
    , sqe_(NULL)
    , cqe_(NULL)
{

    memset(&IOUringParams_, 0, sizeof(IOUringParams_));

    if (io_uring_queue_init_params(queueSize_, &IORing_, &IOUringParams_) < 0) {
        perror("io_uring_init_failed...\n");
        exit(1);
    }

    checkFastPoll();
    checkBufferSelection();

    sqe_ = io_uring_get_sqe(&IORing_);
    io_uring_prep_provide_buffers(sqe_, bufs_, 
                                MAX_MESSAGE_LEN, 
                                bufAmount_, 
                                groupId_,
                                0);

    io_uring_submit(&IORing_);
    io_uring_wait_cqe(&IORing_, &cqe_);
    if(cqe_->res < 0)
    {
        printf("cqe_->res = %d\n", cqe_->res);
        exit(1);
    }
    io_uring_cqe_seen(&IORing_, cqe_);

    printf("IOUringPoller init success\n");
}

IOUringPoller::~IOUringPoller()
{

}

void IOUringPoller::checkFastPoll()
{
    if(!(IOUringParams_.features & IORING_FEAT_FAST_POLL)) {
        printf("IORING_FEAT_FAST_POLL not available in the kernel, quiting...\n");
        exit(0);
    }
}

void IOUringPoller::checkBufferSelection()
{
    struct io_uring_probe *probe;
    probe = io_uring_get_probe_ring(&IORing_);
    if (!probe || !io_uring_opcode_supported(probe, IORING_OP_PROVIDE_BUFFERS)) {
        printf("Buffer select not supported, skipping...\n");
        exit(0);
    }
    free(probe);
}

void IOUringPoller::poll(std::vector<Channel*>* activeChannels)
{
    //printf("IOUringPoller::poll()\n");

    io_uring_submit_and_wait(&IORing_, 1);
    
    struct io_uring_cqe *cqe;
    unsigned int head;
    unsigned int count = 0;
    
    io_uring_for_each_cqe(&IORing_, head, cqe)
    {
        ++count;

        Channel* channel = (Channel*)cqe->user_data;
        int fd = channel->fd();

        auto it = channels_.find(fd);
        
        if(it == channels_.end())
            printf("don't find %d\n", fd);

        channel->set_cqe(cqe);
        activeChannels->push_back(channel);
    }
    
    io_uring_cq_advance(&IORing_, count);
}

void IOUringPoller::updateChannel(Channel* channel)
{

    int fd = channel->fd();
    int channelType = channel->channelType();
    int events = channel->events();


    
    if(channelType == CHANNEL_ACCEPT)
    {
        channels_[fd] = channel;
        addAccept(fd, 0, channel);
    }
    if(channelType == CHANNEL_CONNECTION)
    {
        channels_[fd] = channel;
        if(events == EVENT_READ)
        {
            addRead(fd, IOSQE_BUFFER_SELECT, channel);
        }
        else if(events == EVENT_WRITE)
        {
            addWrite(fd, 0, channel);
        }
        else if(events == EVENT_PROVBUF)
        {
            addProvideBuf(fd, IOSQE_BUFFER_SELECT, channel);
        }
        
    }
}

void IOUringPoller::removeChannel(Channel* channel)
{
    printf("IOUringPoller::removeChannel\n");
}

void IOUringPoller::addAccept(int fd, unsigned flags, Channel* channel)
{
    struct io_uring_sqe *sqe_once = io_uring_get_sqe(&IORing_);

    io_uring_prep_accept(sqe_once, fd, (struct sockaddr *)&((*acceptor_).clientAddr_), &(*acceptor_).clientAddrLen_, 0);
    io_uring_sqe_set_flags(sqe_once, flags);

    sqe_once->user_data = (unsigned long long)channel;
}

void IOUringPoller::addRead(int fd, unsigned flags, Channel* channel)
{
    struct io_uring_sqe *sqe_once = io_uring_get_sqe(&IORing_);
    io_uring_prep_recv(sqe_once, fd, NULL, MAX_MESSAGE_LEN, 0);
    io_uring_sqe_set_flags(sqe_once, flags);
    sqe_once->buf_group = groupId_;

    sqe_once->user_data = (unsigned long long)channel;
}

void IOUringPoller::addWrite(int fd, unsigned flags, Channel* channel)
{
    struct io_uring_sqe *sqe_once = io_uring_get_sqe(&IORing_);

    int bid = (channel->cqe()->flags) >> 16;
    int messageLen = strlen(bufs_[bid]) + 1;
    io_uring_prep_send(sqe_once, fd, &bufs_[bid], messageLen, 0);

    io_uring_sqe_set_flags(sqe_once, flags);

    sqe_once->user_data = (unsigned long long)channel;
}

void IOUringPoller::addProvideBuf(int fd, unsigned flags, Channel* channel)
{
    struct io_uring_sqe *sqe_once = io_uring_get_sqe(&IORing_);

    int bid = channel->bid();
    io_uring_prep_provide_buffers(sqe_once, bufs_, MAX_MESSAGE_LEN, bid + 1, groupId_, 0);

    sqe_once->user_data = (unsigned long long)channel;
}

