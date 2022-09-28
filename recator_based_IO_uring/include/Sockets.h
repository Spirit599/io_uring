#ifndef __SOCKETS_H__
#define __SOCKETS_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

int socketNonblocking();

void bindSocket(int sockfd, const struct sockaddr* sa);

void listenSocket(int sockfd);

#endif