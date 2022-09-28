#include "Sockets.h"

void perror_exit(const char* s)
{
	perror(s);
	exit(-1);
}

int socketNonblocking()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if(sockfd < 0)
    {
        perror_exit("socket error");
    }
    return sockfd;
}

void bindSocket(int sockfd, const struct sockaddr* sa)
{
    int ret = bind(sockfd, sa, sizeof(sockaddr));
    if(ret < 0)
    {
        perror_exit("bind error");
    }
}

void listenSocket(int sockfd)
{
    int ret = listen(sockfd, SOMAXCONN);
    if(ret < 0)
    {
        perror_exit("listen error");
    }
}