# ifndef __MY_SERVER_H_
# define __MY_SERVER_H_

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#include "linklist.h"

#define TEST_PORT 9999
#define BACK_LOG   5

enum MODE
{
    MODE_NAME = 1,
    MODE_INFO,
    MODE_OFF
};

struct file_info
{
    char    file_name[256];
    size_t  size;
    int     line_num;
    char    tail_data[1024];
    int     mode;
};


void do_service(void *fd);


#endif