# include "server.h"

//#define SERV_TEST
#define SERV_BETA

Node *clients;
pthread_mutex_t mutex;

int main(int argc, char **argv)
{
    /*
    * step1: create socket
    */ 
    int server = socket(AF_INET, SOCK_STREAM, 0);
    
    if (server < 0)
    {
        perror("create socket error!");
        exit(1);
    }

    int opt = 0;
	if(setsockopt(server, SOL_SOCKET, SO_SNDBUF, &opt, sizeof(opt)) < 0)
    {
		perror("setsockopt error");
		exit(0);
	}

    /*
    * step2: bind ip & port on the socket
    * */
    struct sockaddr_in	serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(TEST_PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(server, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
    {
		perror("bind error!");
		exit(1);
	}

    /*
    * step3: start listen
    * */
    if(listen(server, BACK_LOG) < 0)
    {
		perror("listen error!");
		exit(1);
	}

    //set detached pthread attr
    pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (FAIL == InitLinkList(&clients))
    {
        perror("init linklist error");
        exit(1);
    }
    while(1)
    {
        /*
        * step4: accept new connect from client
        * */
        struct sockaddr_in	clientaddr;
        socklen_t clientaddr_len = sizeof(clientaddr);
        int client = accept(server, (struct sockaddr*)&clientaddr,&clientaddr_len);
		if(client < 0){
			perror("accept error!");
			continue;
		}

        /*
        * step5:start new thread to deal with business 
        * */
        int err;
        pthread_t th;
		if((err = pthread_create(&th, &attr, do_service, (void *)client)) != 0){		
			perror("pthread create error");
			exit(0);
		}

		pthread_attr_destroy(&attr);
    }

    return 0;
}


void do_service(void* arg)
{
    int fd = arg;
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);
    if (FAIL == AddLinkList(clients, fd))
    {
        pthread_mutex_unlock(&mutex);
        return;
    }
    pthread_mutex_unlock(&mutex);


    /* 01-send filename to client */
    struct file_info FileInfo;
    memset(&FileInfo, 0, sizeof(FileInfo));
    get_file_name(&FileInfo);
    FileInfo.mode = MODE_NAME;
    
    //if(write(fd, &FileInfo, sizeof(FileInfo)) < 0)
    if(send(fd, &FileInfo, sizeof(FileInfo), 0) < 0)
    {
		perror("protocal error");
        return;
    }

    /* 02-recv file info from client */
    size_t	size;
    while(1)
    {
        memset(&FileInfo, 0, sizeof(FileInfo));
        //if((size = read(fd, &FileInfo, sizeof(FileInfo))) < 0)
        if((size = recv(fd, &FileInfo, sizeof(FileInfo), 0)) < 0)
        {
	    	perror("protocal error");
            return;
        }
        
        if (FileInfo.mode == MODE_INFO)
        {
            printf("file_name:%s, size: %d, line_num: %d \nlast_data: %s\n", 
                FileInfo.file_name, FileInfo.size, FileInfo.line_num, FileInfo.tail_data);
            printf("------------------------------------------------\n");
            /* 03- broadcast file info to all clients */
            pthread_mutex_lock(&mutex);
            for (int i = 1; i < LinkListLength(clients); i++)
            {
                int client = (GetNode(clients, i))->data;
                //if(write(client, &FileInfo, sizeof(FileInfo)) < 0)
                if(send(client, &FileInfo, sizeof(FileInfo), 0) < 0)
                {
	            	perror("protocal error");
                    pthread_mutex_unlock(&mutex);
                    return;
                }
            }
            pthread_mutex_unlock(&mutex);
        }
        else if (FileInfo.mode == MODE_OFF)
        {
            //delete client socket from arrary
            pthread_mutex_lock(&mutex);
            if (FAIL == DelLinkList(clients, fd))
            {
                pthread_mutex_unlock(&mutex);
                return;
            }
            pthread_mutex_unlock(&mutex);
            break;
        }  
    }    
}

void get_file_name(struct file_info* FileInfo)
{
    char filename[512];
    memset(filename, 0, sizeof(filename));
#ifdef SERV_TEST
    srand(time(NULL));
    int id = rand() % 11 + 1;   //10 files, 11 random number, contains filename doesn't exsit. 
    sprintf(filename, "../../data/test%d.txt", id);
#endif
#ifdef SERV_BETA 
    printf("please input file name(contains path): ");
    scanf("%s", filename);
#endif
    strcpy(FileInfo->file_name, filename);
}
