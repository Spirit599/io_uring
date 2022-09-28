#include <cstdio>
#include "Event_loop.h"
#include "Channel.h"
#include "IO_uring_Poller.h"
#include "Poller.h"
#include "Thread.h"
#include "Acceptor.h"
#include "TcpServer.h"
#include "TcpConnection.h"

void handleConnection(const TcpConnectionPtr& conn)
{
    printf("new conn is coming %s\n", conn->ipPort().c_str());
}

void handleMessage(const TcpConnectionPtr& conn, char* message)
{
    printf("new message from:%s recv:%s", conn->ipPort().c_str(), message);

    std::string sendMessage(message);
    conn->send(sendMessage.c_str());
}

int main(int argc, char const *argv[])
{
    EventLoop loop;

    TcpServer Server(&loop, 8888);
    Server.setConnectionCallback(handleConnection);
    Server.setMessageCallback(handleMessage);
    Server.listen();

    loop.loop();
    return 0;
}
