#include "Poller.h"
#include "Channel.h"
#include "IO_uring_Poller.h"
#include <cstdio>

Poller::Poller(EventLoop* loop)
    : ownerLoop_(loop)
{

}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel* channel) const
{
  auto it = channels_.find(channel->fd());
  return it != channels_.end() && it->second == channel;
}

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    return new IOUringPoller(loop);
}

void Poller::setAcceptorToPoller(Acceptor* acceptor)
{
    acceptor_ = acceptor;
}