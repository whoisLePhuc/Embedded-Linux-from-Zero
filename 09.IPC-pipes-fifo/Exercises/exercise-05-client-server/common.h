#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>

#define SERVER_FIFO "/tmp/seqnum_server"
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl_%d"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

struct request {
    pid_t pid;      /* PID of client */
    int seq_len;    /* Length of sequence desired */
};

struct response {
    int seq_num;    /* Start of sequence */
};

#endif
