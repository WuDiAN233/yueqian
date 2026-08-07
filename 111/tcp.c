#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
    TCP客户端通信
*/
int tcpsock;

void *recv_fun(void *arg)
{
    int ret;
    char rbuf[2048];
    while(1)
    {
        bzero(rbuf,2048);
        ret=recv(tcpsock,rbuf,2048,0);
        if(ret==0) //服务器断开了
        {
            printf("服务器断开了\n");
            pthread_exit(NULL);
        }

        char *p1=strtok(rbuf,"#");
        if(strcmp(p1,"getlist")==0) //服务器发过来的是在线客户端列表
        {
            char *p2;
            while((p2=strtok(NULL,"#"))!=NULL)
            {
                printf("在线的好友：%s\n",p2);
                //stp接入
            }
        }


        if(strcmp(p1,"chat")==0) //服务器发过来的是聊天信息
        {
            char *p2=strtok(NULL,"#"); //谁发过来的
            char *p3=strtok(NULL,"#"); //真实的信息
            printf("%s发给我的信息：%s\n",p2,p3);
            //stp接入
        }
    }
}


int tcp_init()
{
    int ret;
    pthread_t id;
    //定义ipv4地址结构体存放需要绑定的ip和端口号
    struct sockaddr_in bindaddr;
    bzero(&bindaddr,sizeof(bindaddr));
    bindaddr.sin_family=AF_INET;
    bindaddr.sin_addr.s_addr=htonl(INADDR_ANY); //绑定客户端自己的ip地址
    bindaddr.sin_port=htons(10086); //绑定客户端自己的端口号

    //定义ipv4地址结构体存放服务器的ip和端口号
    struct sockaddr_in serveraddr;
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr("192.168.131.130"); //服务器的ip地址
    serveraddr.sin_port=htons(10000); //服务器的端口号

    //创建tcp套接字
    tcpsock=socket(AF_INET,SOCK_STREAM,0);

    if(tcpsock==-1)
    {
        perror("创建tcp套接字失败了!\n");
        return -1;
    }

    //设置取消端口绑定限制
    int on=1;
    setsockopt(tcpsock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    //绑定ip和端口号
    ret=bind(tcpsock,(struct sockaddr *)&bindaddr,sizeof(struct sockaddr_in));

    if(ret==-1)
    {
        perror("绑定ip和端口号失败了!\n");
        return -1;
    }

    //连接服务器
    ret=connect(tcpsock,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));

    if(ret==-1)
    {
        perror("连接服务器失败!\n");
        return -1;
    }

    //创建线程接收服务器的信息
    pthread_create(&id,NULL,recv_fun,NULL);

    return 0;
}

//获取服务器上存放的在线客户端信息
int tcp_getlist()
{
    send(tcpsock,"getlist",7,0);

    return 0;
}

//跟指定客户端聊天
int tcp_chat(char *ip,unsigned short port,char *msg)
{
    char allmsg[2048];

    bzero(allmsg,2048);
    //按照原来的通信协议拼接字符串
    sprintf(allmsg,"chat@%s@%hu@%s",ip,port,msg);
    //发送给服务器
    send(tcpsock,allmsg,strlen(allmsg),0);
    return 0;
}

int tcp_close()
{
    close(tcpsock);
    return 0;
}
