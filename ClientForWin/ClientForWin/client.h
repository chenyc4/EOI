# ifndef __MY_CLIENT_H_
# define __MY_CLIENT_H_

#include<signal.h>
#include<string.h>
#include<stdio.h>
#if defined(__linux__) 
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include<netdb.h>
    #include<sys/socket.h>
    #include<unistd.h>
    #include<stdlib.h>
    #include<errno.h>
    #include<fcntl.h>
    #define INT int
    #define CLOSE close
#elif defined(_WIN32)||defined(_WIN64) 
    #include <winsock2.h>
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")

    #define INT SOCKET
    #define CLOSE closesocket
#endif


#define TEST_PORT 9999
#define TEST_IP "127.0.0.1"

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

void do_client(INT fd);
void get_file_info(struct file_info *FileInfo);
void disconnect(int signo);

#endif