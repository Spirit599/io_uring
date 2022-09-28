#ifndef __EVENT_TYPE_H__
#define __EVENT_TYPE_H__

#define EVENT_READ (1<<0)
#define EVENT_WRITE (1<<1)
#define EVENT_ACCEPT (1<<2)
#define EVENT_CLOSE (1<<3)
#define EVENT_PROVBUF (1<<4)

enum {
    IOURING_ACCEPT,
    IOURING_READ,
    IOURING_WRITE,
    IOURING_PROV_BUF,
};

enum {
    CHANNEL_NEW,
    CHANNEL_ADDED,
    CHANNEL_DELETED,
};

enum {
    CHANNEL_ACCEPT,
    CHANNEL_CONNECTION,
};

typedef struct conn_info {
    unsigned int fd;
    unsigned short type;
    unsigned short bid;
} conn_info;

enum {
    ACCEPT,
    READ,
    WRITE,
    PROV_BUF,
};

enum {
    CONNECTING,
    CONNECTED,
};

#endif