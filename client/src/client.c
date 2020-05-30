#include "client.h"

static INT client;

int main(int argc, char **argv)
{
//#if defined(__linux__)
    if (signal(SIGINT, disconnect) == SIG_ERR)
    {
        perror("signal sigint error");
		exit(1);
    }
//#endif
#if defined(_WIN32)||defined(_WIN64)
    //printf("windows\r\n");
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        return 0;
    }
#endif
    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0)
    {
		perror("create socket error");
		exit(0);
	}

    struct sockaddr_in severaddr;
	memset(&severaddr, 0, sizeof(severaddr));

    severaddr.sin_family = AF_INET;
    severaddr.sin_port = htons(TEST_PORT);
    if(inet_pton(AF_INET, TEST_IP, &severaddr.sin_addr.s_addr) != 1)
    {	
		perror("inet_pton error");
		exit(0);
	}

    if(connect(client, (struct sockaddr *)&severaddr, sizeof(severaddr)) < 0)
    {
		perror("connect error");
		exit(0);
	}

    do_client(client);

    CLOSE(client);
    return 0;
}

void do_client(INT fd)
{
    struct file_info FileInfo;
    size_t	size;
    while(1)
    { 
        /* recv file name from server */
        memset(&FileInfo, 0, sizeof(FileInfo));
        //if((size = read(fd, &FileInfo, sizeof(FileInfo))) < 0)
        if((size = recv(fd, (char *)&FileInfo, sizeof(FileInfo), 0)) < 0)
        {
		    perror("protocal error");
            return;
        }

        if (FileInfo.mode == MODE_NAME)
        {
            get_file_info(&FileInfo);   /* get file info and send to server */
            //if(write(fd, &FileInfo, sizeof(FileInfo)) < 0)
            if(send(fd, (const char *)&FileInfo, sizeof(FileInfo), 0) < 0)
            {
	    	    perror("protocal error");
                return;
            }
        }
        else if (FileInfo.mode == MODE_INFO)
        {
            /* recv file info from server */
            printf("file_name:%s, size: %d, line_num: %d \nlast_data: %s\n", 
                FileInfo.file_name, (int)FileInfo.size, FileInfo.line_num, FileInfo.tail_data);
            printf("------------------------------------------------\n");
        }
    }   
}

void get_file_info(struct file_info* FileInfo)
{
    if (FileInfo == NULL)
    {
        return;
    }
	FILE* fp;
#if defined(__linux__)
	fp= fopen(FileInfo->file_name, "r");
#elif defined(_WIN32)||defined(_WIN64)
	fopen_s(&fp, FileInfo->file_name, "r");
#endif
    if(fp == NULL)
    {
        perror("open file error");
        return;
    }

    // get file size
    fseek(fp, 0L, SEEK_END);
    FileInfo->size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    int num = 0;
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    while(fgets(buff, sizeof(buff), fp) != NULL)
    {
        num++;
    }
    FileInfo->line_num = num;
#if defined(__linux__)
    strcpy(FileInfo->tail_data, buff);
#elif defined(_WIN32)||defined(_WIN64)
	strcpy_s(FileInfo->tail_data,sizeof(FileInfo->tail_data), buff);
#endif
    FileInfo->mode = MODE_INFO;
    fclose(fp);
}

void disconnect(int signo)
{
    if (signo == SIGINT)
    {
        struct file_info FileInfo;
        memset(&FileInfo, 0, sizeof(FileInfo));
        FileInfo.mode = MODE_OFF;
        //if(write(client, &FileInfo, sizeof(FileInfo)) < 0)
        if(send(client, (const char *)&FileInfo, sizeof(FileInfo), 0) < 0)
        {
	    	perror("protocal error");
            return;
        }
        CLOSE(client);
        exit(0);
    }
}
