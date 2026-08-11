#include "myhead.h"
#include <stdint.h>

/*
    TCP客户端通信
    网络线程只接收数据，LVGL界面统一在主线程更新
*/
int tcpsock=-1;
char chatip[20]={0};
unsigned short chatport=0;
char chatname[64]={0};
char roomtargets[1024]={0};
char roommemberbuf[20][80];
int roommembernum=0;
char search_account_name[64]={0};
lv_obj_t *tcp_list=NULL;

extern void fri_chat_cb(lv_event_t * e);

static pthread_mutex_t sendlock=PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t datalock=PTHREAD_MUTEX_INITIALIZER;
static int connected=0;

static char onlinebuf[20][80];
static int onlinenum=0;
static int onlineflag=0;

static char friendbuf[20][80];
static int friendnum=0;
static int friendflag=0;

static char chatbuf[2048]={0};
static int chatflag=0;
static char roombuf[2048]={0};
static int roomflag=0;

static char resultcmd[20]={0};
static char resultstate[20]={0};
static char resulttext[256]={0};
static int resultflag=0;
static int disconnectflag=0;

//完整发送指定长度的数据
static int send_all(int sock,const void *data,int size)
{
    int sent=0;
    while(sent<size)
    {
        int ret=send(sock,(const char *)data+sent,size-sent,0);
        if(ret<0 && errno==EINTR)
            continue;
        if(ret<=0)
            return -1;
        sent+=ret;
    }
    return 0;
}

//完整接收指定长度的数据
static int recv_all(int sock,void *data,int size)
{
    int received=0;
    while(received<size)
    {
        int ret=recv(sock,(char *)data+received,size-received,0);
        if(ret<0 && errno==EINTR)
            continue;
        if(ret<=0)
            return -1;
        received+=ret;
    }
    return 0;
}

//发送一条完整协议数据，避免粘包和拆包
static int tcp_send_command(const char *msg)
{
    uint32_t len;
    uint32_t netlen;
    int ret=-1;

    if(msg==NULL)
        return -1;
    len=(uint32_t)strlen(msg);
    if(len==0 || len>4095)
        return -1;

    pthread_mutex_lock(&sendlock);
    if(connected && tcpsock>=0)
    {
        netlen=htonl(len);
        if(send_all(tcpsock,&netlen,sizeof(netlen))==0 &&
           send_all(tcpsock,msg,(int)len)==0)
            ret=0;
    }
    pthread_mutex_unlock(&sendlock);
    return ret;
}

//保存列表数据
static int split_list(const char *body,char listbuf[20][80])
{
    int num=0;
    const char *start=body;
    while(start!=NULL && *start!='\0' && num<20)
    {
        const char *end=strchr(start,'#');
        int len=end==NULL?(int)strlen(start):(int)(end-start);
        if(len>0)
        {
            if(len>79)
                len=79;
            bzero(listbuf[num],80);
            memcpy(listbuf[num],start,len);
            num++;
        }
        if(end==NULL)
            break;
        start=end+1;
    }
    return num;
}

//解析服务器返回的数据
static void parse_server_message(char *rbuf)
{
    char *p1=strchr(rbuf,'#');
    char *p2;
    if(p1==NULL)
        return;
    *p1='\0';
    p1++;

    pthread_mutex_lock(&datalock);
    if(strcmp(rbuf,"getlist")==0)
    {
        onlinenum=split_list(p1,onlinebuf);
        onlineflag=1;
    }
    else if(strcmp(rbuf,"getfriends")==0)
    {
        friendnum=split_list(p1,friendbuf);
        friendflag=1;
    }
    else if(strcmp(rbuf,"chat")==0 || strcmp(rbuf,"room")==0)
    {
        p2=strchr(p1,'#');
        if(p2!=NULL)
        {
            *p2='\0';
            p2++;
            if(strcmp(rbuf,"chat")==0)
            {
                snprintf(chatbuf,sizeof(chatbuf),"%s\n%s\n",p1,p2);
                chatflag=1;
            }
            else
            {
                snprintf(roombuf,sizeof(roombuf),"%s\n%s\n",p1,p2);
                roomflag=1;
            }
        }
    }
    else
    {
        p2=strchr(p1,'#');
        if(p2!=NULL)
        {
            *p2='\0';
            p2++;
        }
        else
            p2="";
        snprintf(resultcmd,sizeof(resultcmd),"%s",rbuf);
        snprintf(resultstate,sizeof(resultstate),"%s",p1);
        snprintf(resulttext,sizeof(resulttext),"%s",p2);
        resultflag=1;
    }
    pthread_mutex_unlock(&datalock);
}

//接收服务器信息的线程
static void *recv_fun(void *arg)
{
    while(1)
    {
        uint32_t netlen=0;
        uint32_t len;
        char rbuf[4096];
        if(recv_all(tcpsock,&netlen,sizeof(netlen))!=0)
            break;
        len=ntohl(netlen);
        if(len==0 || len>=sizeof(rbuf))
            break;
        bzero(rbuf,sizeof(rbuf));
        if(recv_all(tcpsock,rbuf,(int)len)!=0)
            break;
        rbuf[len]='\0';
        parse_server_message(rbuf);
    }

    pthread_mutex_lock(&sendlock);
    connected=0;
    pthread_mutex_unlock(&sendlock);
    pthread_mutex_lock(&datalock);
    disconnectflag=1;
    pthread_mutex_unlock(&datalock);
    printf("服务器断开了\n");
    return NULL;
}

//给列表创建客户端按钮
static void update_btn_list(lv_obj_t *list,char data[20][80],int num)
{
    int i;
    if(list==NULL || !lv_obj_is_valid(list))
        return;
    lv_obj_clean(list);
    for(i=0;i<num;i++)
    {
        lv_obj_t *bt=lv_list_add_btn(list,NULL,data[i]);
        lv_obj_add_event_cb(bt,fri_chat_cb,LV_EVENT_CLICKED,NULL);
    }
}

//给群聊界面创建好友复选框
static void update_room_list(lv_obj_t *list,char data[20][80],int num)
{
    int i;
    if(list==NULL || !lv_obj_is_valid(list))
        return;
    lv_obj_clean(list);
    for(i=0;i<num;i++)
    {
        lv_obj_t *checkbox=lv_checkbox_create(list);
        lv_checkbox_set_text(checkbox,data[i]);
        lv_obj_set_width(checkbox,640);
    }
}

//在主线程更新LVGL界面
void tcp_update_lvgl(void)
{
    char onlinecopy[20][80];
    char friendcopy[20][80];
    char chatcopy[2048]={0};
    char roomcopy[2048]={0};
    char cmdcopy[20]={0};
    char statecopy[20]={0};
    char textcopy[256]={0};
    int onlinecount=0;
    int friendcount=0;
    int haveonline=0;
    int havefriend=0;
    int havechat=0;
    int haveroom=0;
    int haveresult=0;
    int disconnected=0;

    pthread_mutex_lock(&datalock);
    if(onlineflag)
    {
        memcpy(onlinecopy,onlinebuf,sizeof(onlinecopy));
        onlinecount=onlinenum;
        onlineflag=0;
        haveonline=1;
    }
    if(friendflag)
    {
        memcpy(friendcopy,friendbuf,sizeof(friendcopy));
        friendcount=friendnum;
        friendflag=0;
        havefriend=1;
    }
    if(chatflag)
    {
        snprintf(chatcopy,sizeof(chatcopy),"%s",chatbuf);
        bzero(chatbuf,sizeof(chatbuf));
        chatflag=0;
        havechat=1;
    }
    if(roomflag)
    {
        snprintf(roomcopy,sizeof(roomcopy),"%s",roombuf);
        bzero(roombuf,sizeof(roombuf));
        roomflag=0;
        haveroom=1;
    }
    if(resultflag)
    {
        snprintf(cmdcopy,sizeof(cmdcopy),"%s",resultcmd);
        snprintf(statecopy,sizeof(statecopy),"%s",resultstate);
        snprintf(textcopy,sizeof(textcopy),"%s",resulttext);
        resultflag=0;
        haveresult=1;
    }
    if(disconnectflag)
    {
        disconnectflag=0;
        disconnected=1;
    }
    pthread_mutex_unlock(&datalock);

    if(haveonline)
        update_btn_list(frd_list,onlinecopy,onlinecount);

    if(havefriend)
    {
        update_btn_list(main_list,friendcopy,friendcount);
        update_btn_list(chat_list,friendcopy,friendcount);
        update_room_list(room_list,friendcopy,friendcount);
    }

    if(havechat)
    {
        if(chatmsg!=NULL && lv_obj_is_valid(chatmsg))
            lv_textarea_add_text(chatmsg,chatcopy);
        chat_record_append(0,chatcopy);
    }
    if(haveroom)
    {
        if(roommsg!=NULL && lv_obj_is_valid(roommsg))
            lv_textarea_add_text(roommsg,roomcopy);
        chat_record_append(1,roomcopy);
    }

    if(haveresult && (strcmp(cmdcopy,"login")==0 || strcmp(cmdcopy,"register")==0))
    {
        if(strcmp(statecopy,"ok")==0)
        {
            lv_obj_t *oldwin=lv_scr_act();
            show_main();
            if(oldwin!=mainwin)
                lv_obj_del_async(oldwin);
            loginwin=NULL;
            setwin=NULL;
        }
        else if(login_message!=NULL && lv_obj_is_valid(login_message))
            lv_label_set_text(login_message,textcopy);
    }
    else if(haveresult && strcmp(cmdcopy,"search")==0)
    {
        if(search_result!=NULL && lv_obj_is_valid(search_result))
        {
            lv_label_set_text(search_result,textcopy);
            if(strcmp(statecopy,"ok")==0)
            {
                snprintf(search_account_name,64,"%s",textcopy);
                if(search_add_btn!=NULL && lv_obj_is_valid(search_add_btn))
                    lv_obj_clear_flag(search_add_btn,LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
    else if(haveresult && (strcmp(cmdcopy,"addfriend")==0 || strcmp(cmdcopy,"delfriend")==0))
    {
        if(search_result!=NULL && lv_obj_is_valid(search_result))
            lv_label_set_text(search_result,textcopy);
        if(chatmsg!=NULL && lv_obj_is_valid(chatmsg))
        {
            lv_textarea_add_text(chatmsg,textcopy);
            lv_textarea_add_text(chatmsg,"\n");
        }
        if(strcmp(statecopy,"ok")==0)
            tcp_getfriends();
    }

    if(disconnected)
    {
        if(login_message!=NULL && lv_obj_is_valid(login_message))
            lv_label_set_text(login_message,"服务器已断开");
        if(chatmsg!=NULL && lv_obj_is_valid(chatmsg))
            lv_textarea_add_text(chatmsg,"服务器已断开\n");
    }
}

//连接服务器
int tcp_init(void)
{
    int ret;
    int on=1;
    int bindport=10087;
    const char *serverip=getenv("CHAT_SERVER_IP");
    const char *clientport=getenv("CHAT_CLIENT_PORT");
    pthread_t id;
    struct sockaddr_in bindaddr;
    struct sockaddr_in serveraddr;

    if(serverip==NULL || strlen(serverip)==0)
        serverip="192.168.3.42";
    if(clientport!=NULL && atoi(clientport)>=0 && atoi(clientport)<=65535)
        bindport=atoi(clientport);

    bzero(&bindaddr,sizeof(bindaddr));
    bindaddr.sin_family=AF_INET;
    bindaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    bindaddr.sin_port=htons(bindport);

    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr(serverip);
    serveraddr.sin_port=htons(10000);

    tcpsock=socket(AF_INET,SOCK_STREAM,0);
    if(tcpsock==-1)
    {
        perror("创建tcp套接字失败了");
        return -1;
    }
    setsockopt(tcpsock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    ret=bind(tcpsock,(struct sockaddr *)&bindaddr,sizeof(bindaddr));
    if(ret==-1)
    {
        perror("绑定ip和端口号失败了");
        close(tcpsock);
        tcpsock=-1;
        return -1;
    }

    ret=connect(tcpsock,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if(ret==-1)
    {
        perror("连接服务器失败了");
        close(tcpsock);
        tcpsock=-1;
        return -1;
    }

    pthread_mutex_lock(&sendlock);
    connected=1;
    pthread_mutex_unlock(&sendlock);
    if(pthread_create(&id,NULL,recv_fun,NULL)!=0)
    {
        tcp_close();
        return -1;
    }
    pthread_detach(id);
    return 0;
}

//账号密码不能带协议分隔符
static int account_command(const char *cmd,const char *username,const char *password)
{
    char allmsg[512]={0};
    if(username==NULL || password==NULL || strlen(username)==0 || strlen(password)==0)
        return -1;
    if(strchr(username,'@')!=NULL || strchr(username,'#')!=NULL ||
       strchr(password,'@')!=NULL || strchr(password,'#')!=NULL)
        return -1;
    snprintf(allmsg,sizeof(allmsg),"%s@%s@%s",cmd,username,password);
    return tcp_send_command(allmsg);
}

int tcp_login(const char *username,const char *password)
{
    return account_command("login",username,password);
}

int tcp_register(const char *username,const char *password)
{
    return account_command("register",username,password);
}

int tcp_logout(void)
{
    return tcp_send_command("logout");
}

//获取服务器上存放的在线客户端信息
int tcp_getlist(void)
{
    return tcp_send_command("getlist");
}

//获取自己的好友信息
int tcp_getfriends(void)
{
    return tcp_send_command("getfriends");
}

int tcp_search(const char *username)
{
    char allmsg[256]={0};
    if(username==NULL || strchr(username,'@')!=NULL || strchr(username,'#')!=NULL)
        return -1;
    snprintf(allmsg,sizeof(allmsg),"search@%s",username);
    return tcp_send_command(allmsg);
}

int tcp_addfriend(const char *username)
{
    char allmsg[256]={0};
    if(username==NULL || strchr(username,'@')!=NULL || strchr(username,'#')!=NULL)
        return -1;
    snprintf(allmsg,sizeof(allmsg),"addfriend@%s",username);
    return tcp_send_command(allmsg);
}

int tcp_delfriend(const char *username)
{
    char allmsg[256]={0};
    if(username==NULL || strchr(username,'@')!=NULL || strchr(username,'#')!=NULL)
        return -1;
    snprintf(allmsg,sizeof(allmsg),"delfriend@%s",username);
    return tcp_send_command(allmsg);
}

//跟指定客户端聊天
int tcp_chat(char *ip,unsigned short port,char *msg)
{
    char allmsg[4096]={0};
    if(ip==NULL || msg==NULL || strlen(msg)==0 || strlen(msg)>3500)
        return -1;
    snprintf(allmsg,sizeof(allmsg),"chat@%s@%hu@%s",ip,port,msg);
    return tcp_send_command(allmsg);
}

//发送群聊信息
int tcp_room(const char *targets,const char *msg)
{
    char allmsg[4096]={0};
    if(targets==NULL || msg==NULL || strlen(targets)==0 || strlen(msg)==0)
        return -1;
    snprintf(allmsg,sizeof(allmsg),"room@%s@%s",targets,msg);
    return tcp_send_command(allmsg);
}

//主界面本地搜索好友
void tcp_filter_friends(const char *keyword)
{
    char copybuf[20][80];
    int copynum;
    int i;
    int shownum=0;
    char showbuf[20][80];
    pthread_mutex_lock(&datalock);
    memcpy(copybuf,friendbuf,sizeof(copybuf));
    copynum=friendnum;
    pthread_mutex_unlock(&datalock);

    for(i=0;i<copynum;i++)
    {
        if(keyword==NULL || strlen(keyword)==0 || strstr(copybuf[i],keyword)!=NULL)
        {
            snprintf(showbuf[shownum],80,"%s",copybuf[i]);
            shownum++;
        }
    }
    update_btn_list(main_list,showbuf,shownum);
}

//测试发送消息给服务器
void tcp_test(char *msg)
{
    tcp_send_command(msg);
}

int tcp_close(void)
{
    pthread_mutex_lock(&sendlock);
    connected=0;
    if(tcpsock>=0)
    {
        shutdown(tcpsock,SHUT_RDWR);
        close(tcpsock);
        tcpsock=-1;
    }
    pthread_mutex_unlock(&sendlock);
    return 0;
}
