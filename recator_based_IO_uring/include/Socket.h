#ifndef __SOCKET_H__
#define __SOCKET_H__

class Socket
{
private:
    const int sockfd_;
public:
    explicit Socket(int sockfd);
    ~Socket();
    int fd() const;
    void bindAddress(unsigned short port, unsigned int ip = 0);
    void listen();
    void setReuseAddr(bool on);
};


#endif