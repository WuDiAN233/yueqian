
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "lv_drv_conf.h"
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#if USE_SDL
  #define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
  #include <SDL2/SDL.h>
  #include "lv_drivers/sdl/sdl.h"
#elif USE_X11
  #include "lv_drivers/x11/x11.h"
#endif
// #include "lv_drivers/display/monitor.h"
// #include "lv_drivers/indev/mouse.h"
// #include "lv_drivers/indev/keyboard.h"
// #include "lv_drivers/indev/mousewheel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>  //文件IO有关的头文件
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>  //目录有关的头文件
#include <errno.h> //perror有关  errno有关的头文件
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/wait.h> //跟wait，waitpid有关的头文件
#include <signal.h> //跟信号有关的头文件
#include <pthread.h>  //跟线程有关的头文件
#include <sys/socket.h>  //跟网络编程有关的头文件
#include <netinet/in.h>  //跟网络编程有关的头文件
#include <arpa/inet.h>   //跟网络编程有关的头文件
#include <netdb.h>  //跟gethostbyname有关的头文件
#include <sys/select.h>  //跟select多路复用有关的头文件

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static void hal_deinit(void);
static void* tick_thread(void *data);

/**********************
 *  STATIC VARIABLES
 **********************/
static pthread_t thr_tick;    /* thread */
static bool end_tick = false; /* flag to terminate thread */

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
#if 0
static void user_image_demo()
{
  lv_obj_t * img = lv_gif_create(lv_scr_act());
  lv_gif_set_src(img, "A:lvgl/examples/libs/gif/bulb.gif");
  lv_obj_align(img, LV_ALIGN_BOTTOM_RIGHT, -20, -20);

  lv_color_t bg_color = lv_palette_lighten(LV_PALETTE_LIGHT_BLUE, 5);
    lv_color_t fg_color = lv_palette_darken(LV_PALETTE_BLUE, 4);

    lv_obj_t * qr = lv_qrcode_create(lv_scr_act(), 150, fg_color, bg_color);

    /*Set data*/
    const char * data = "https://lvgl.io";
    lv_qrcode_update(qr, data, strlen(data));
    lv_obj_center(qr);

    /*Add a border with bg_color*/
    lv_obj_set_style_border_color(qr, bg_color, 0);
    lv_obj_set_style_border_width(qr, 5, 0);

    /*Create a font*/
    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    info.name = "./lvgl/examples/libs/freetype/Lato-Regular.ttf";
    info.weight = 24;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    /*Create style with the new font*/
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, info.font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    /*Create a label with the new style*/
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_obj_add_style(label, &style, 0);
    lv_label_set_text(label, "Hello world\nI'm a font created with FreeType");
    lv_obj_set_pos(label, 10, 10);

    lv_obj_t *  img1 = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_img_set_src(img1, "A:lvgl/examples/libs/png/wink.png");
    lv_obj_align(img1, LV_ALIGN_LEFT_MID, 20, 0);

    lv_obj_t * wp;

    wp = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_img_set_src(wp, "A:lvgl/examples/libs/sjpg/small_image.sjpg");
    lv_obj_align(wp, LV_ALIGN_RIGHT_MID, -20, 0);

    lv_obj_t * img2 = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_img_set_src(img2, "A:lvgl/examples/libs/sjpg/lv_example_jpg.jpg");
    //lv_obj_center(img);
    lv_obj_align(img2, LV_ALIGN_TOP_RIGHT, -20, 20);

    lv_obj_t * img3 = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
#if LV_COLOR_DEPTH == 32
    lv_img_set_src(img3, "A:lvgl/examples/libs/bmp/example_32bit.bmp");
#elif LV_COLOR_DEPTH == 16
    lv_img_set_src(img, "A:lvgl/examples/libs/bmp/example_16bit.bmp");
#endif
    lv_obj_align(img3, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t * img4 = lv_img_create(lv_scr_act());
    lv_img_set_src(img4, "A:lvgl/examples/libs/ffmpeg/ffmpeg.png");
    lv_obj_align(img4, LV_ALIGN_BOTTOM_LEFT, 20, -20);

    lv_obj_t * player = lv_ffmpeg_player_create(lv_scr_act());
    lv_ffmpeg_player_set_src(player, "./lvgl/examples/libs/ffmpeg/birds.mp4");
    lv_ffmpeg_player_set_auto_restart(player, true);
    lv_ffmpeg_player_set_cmd(player, LV_FFMPEG_PLAYER_CMD_START);
    lv_obj_align(player, LV_ALIGN_TOP_MID, 0, 20);
}
#endif

int tcpsock;

//线程接收服务器的信息
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
			exit(-1); //结束整个进程
		}
		//切割字符串，判断服务器给客户端究竟发的是什么信息
		/*
			聊天信息：  chat#真实信息
			文件信息:   sendfile#文件类型#文件大小  文件内容
			表情包：    emoji#表情包大小  表情包内容
			所有在线客户端信息  getlist#在线客户端信息
		*/
		char *p1=strtok(rbuf,"#");
		if(strcmp(p1,"getlist")==0) //说明服务器给客户端发过来的是好友列表
		{
			char *p2;
			printf("所有在线的好友信息如下:\n");
			while((p2=strtok(NULL,"#"))!=NULL)
			{
				printf("%s\n",p2);
			}
		}
		if(strcmp(p1,"chat")==0)  //说明服务器给客户端发过来的是聊天信息
		{
			//继续切割得到真实信息
			char *p2=strtok(NULL,"#"); //对方的ip和端口号
			char *p3=strtok(NULL,"#"); //真实的信息
			printf("%s发给我的信息: %s\n",p2,p3);
		}
	}
}
//线程发送信息给服务器
void *send_fun(void *arg)
{
	char sbuf[100];
	char allmsg[2048];
	char ipbuf[20];
	unsigned short portnum;
	int n;
	int ret;

	while(1)
	{
		printf("请选择你想要的功能!\n");
		printf("1.跟其他客户端聊天!\n");
		printf("2.发送文件给其他客户端!\n");
		printf("3.发送表情包给其他客户端!\n");
		printf("4.获取服务器上存放的在线客户端信息!\n");
		scanf("%d",&n);
		switch(n)
		{
			case 1:  //聊天 
				bzero(ipbuf,20);
				bzero(sbuf,100);
				bzero(allmsg,2048);
				printf("请输入对方的ip!\n");
				scanf("%s",ipbuf);
				printf("请输入对方的端口号！\n");
				scanf("%hu",&portnum);
				printf("请输入你要发给对方的信息!\n");
				scanf("%s",sbuf);
				//按照制定好的协议拼接字符串
				sprintf(allmsg,"chat@%s@%hu@%s",ipbuf,portnum,sbuf);
				//发送给服务器
				send(tcpsock,allmsg,strlen(allmsg),0);
				break;
			case 2:  //发送文件 
				bzero(ipbuf,20);
				bzero(sbuf,100);
				bzero(allmsg,2048);
				printf("请输入对方的ip!\n");
				scanf("%s",ipbuf);
				printf("请输入对方的端口号！\n");
				scanf("%hu",&portnum);
				printf("请输入你要发给对方的文件路径名!\n");
				scanf("%s",sbuf);
				//求出文件大小
				struct stat mystat;
				stat(sbuf,&mystat);
				//获取文件后缀名
				char *p1=strstr(sbuf,".");  
				
				//按照制定好的协议(sendfile@对方ip@对方端口@文件大小@文件类型)拼接字符串
				sprintf(allmsg,"sendfile@%s@%hu@%ld@%s",ipbuf,portnum,mystat.st_size,p1);
				//先发送文件基本信息发送给服务器
				send(tcpsock,allmsg,strlen(allmsg),0);
				
				int fd;
				fd=open(sbuf,O_RDWR);
				if(fd==-1)
				{
					perror("打开文件失败了!\n");
					return -1;
				}
				//循环读取文件内容，再发文件内容
				while(1)
				{
					bzero(allmsg,2048);
					ret=read(fd,allmsg,2048);
					if(ret==0)
						break;
					//发送给服务器
					send(tcpsock,allmsg,ret,0);
				}
				close(fd);
				break;
			case 3:
				break;
			case 4:
				send(tcpsock,"getlist",7,0);
				break;
		}
	}
}

int main(int argc, char **argv)
{
	//LVGL初始化
	lv_init();

	//硬件初始化
	hal_init();

	pthread_t id,id1;
	int ret;
	
	//定义ipv4地址结构体存放需要绑定的ip和端口号
	struct sockaddr_in bindaddr;
	bzero(&bindaddr,sizeof(bindaddr));
	bindaddr.sin_family=AF_INET;
	bindaddr.sin_addr.s_addr=htonl(INADDR_ANY); //绑定客户端自己的ip地址
	bindaddr.sin_port=htons(10086);  //绑定客户端的端口号
	
	//定义ipv4地址结构体存放服务器的ip和端口号
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=inet_addr("192.168.72.3"); //服务器的ip地址
	serveraddr.sin_port=htons(10000);  //服务器的端口号
	
	//创建tcp套接字
	tcpsock=socket(AF_INET,SOCK_STREAM,0);
	if(tcpsock==-1)
	{
		perror("创建tcp套接字失败了!\n");
		return -1;
	}
	
	//设置取消端口绑定限制
	int on=1;  //开关
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
	
	//创建线程接收信息
	pthread_create(&id,NULL,recv_fun,NULL);
	
	//创建线程发送信息给服务器
	pthread_create(&id1,NULL,send_fun,NULL);

	while(1) {
	/* Periodically call the lv_task handler.
	 * It could be done in a timer interrupt or an OS task too.*/
	lv_timer_handler();  //轮询的方式，进入事件循环，不断监测LVGL产生的各种事件，响应事件
	usleep(5 * 1000);
	}

	hal_deinit();
	return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static void hal_init(void)
{
  /* mouse input device */
  static lv_indev_drv_t indev_drv_1;
  lv_indev_drv_init(&indev_drv_1);
  indev_drv_1.type = LV_INDEV_TYPE_POINTER;

  /* keyboard input device */
  static lv_indev_drv_t indev_drv_2;
  lv_indev_drv_init(&indev_drv_2);
  indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;

  /* mouse scroll wheel input device */
  static lv_indev_drv_t indev_drv_3;
  lv_indev_drv_init(&indev_drv_3);
  indev_drv_3.type = LV_INDEV_TYPE_ENCODER;

  lv_group_t *g = lv_group_create();
  lv_group_set_default(g);

  lv_disp_t *disp = NULL;

#if USE_SDL
  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  sdl_init();

  /*Create a display buffer*/
  static lv_disp_draw_buf_t disp_buf1;
  static lv_color_t buf1_1[MONITOR_HOR_RES * 100];
  static lv_color_t buf1_2[MONITOR_HOR_RES * 100];
  lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, MONITOR_HOR_RES * 100);

  /*Create a display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  disp_drv.draw_buf = &disp_buf1;
  disp_drv.flush_cb = sdl_display_flush;
  disp_drv.hor_res = MONITOR_HOR_RES;
  disp_drv.ver_res = MONITOR_VER_RES;
  disp_drv.antialiasing = 1;

  disp = lv_disp_drv_register(&disp_drv);

  /* Add the input device driver */
  // mouse_init();
  indev_drv_1.read_cb = sdl_mouse_read;

  // keyboard_init();
  indev_drv_2.read_cb = sdl_keyboard_read;

  // mousewheel_init();
  indev_drv_3.read_cb = sdl_mousewheel_read;

#elif USE_X11
  lv_x11_init("LVGL Simulator Demo", DISP_HOR_RES, DISP_VER_RES);

  /*Create a display buffer*/
  static lv_disp_draw_buf_t disp_buf1;
  static lv_color_t buf1_1[DISP_HOR_RES * 100];
  static lv_color_t buf1_2[DISP_HOR_RES * 100];
  lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, DISP_HOR_RES * 100);

  /*Create a display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.draw_buf = &disp_buf1;
  disp_drv.flush_cb = lv_x11_flush;
  disp_drv.hor_res = DISP_HOR_RES;
  disp_drv.ver_res = DISP_VER_RES;
  disp_drv.antialiasing = 1;

  disp = lv_disp_drv_register(&disp_drv);

  /* Add the input device driver */
  indev_drv_1.read_cb = lv_x11_get_pointer;
  indev_drv_2.read_cb = lv_x11_get_keyboard;
  indev_drv_3.read_cb = lv_x11_get_mousewheel;
#endif
  /* Set diplay theme */
  lv_theme_t * th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
  lv_disp_set_theme(disp, th);

  /* Tick init */
  end_tick = false;
  pthread_create(&thr_tick, NULL, tick_thread, NULL);

  /* register input devices */
  lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);
  lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
  lv_indev_t *enc_indev = lv_indev_drv_register(&indev_drv_3);
  lv_indev_set_group(kb_indev, g);
  lv_indev_set_group(enc_indev, g);

  /* Set a cursor for the mouse */
  LV_IMG_DECLARE(mouse_cursor_icon);                   /*Declare the image file.*/
  lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor*/
  lv_img_set_src(cursor_obj, &mouse_cursor_icon);      /*Set the image source*/
  lv_indev_set_cursor(mouse_indev, cursor_obj);        /*Connect the image  object to the driver*/
}

/**
 * Releases the Hardware Abstraction Layer (HAL) for the LVGL graphics library
 */
static void hal_deinit(void)
{
  end_tick = true;
  pthread_join(thr_tick, NULL);

#if USE_SDL
  // nop
#elif USE_X11
  lv_x11_deinit();
#endif
}

/**
 * A task to measure the elapsed time for LVGL
 * @param data unused
 * @return never return
 */
static void* tick_thread(void *data) {
  (void)data;

  while(!end_tick) {
    usleep(5000);
    lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
  }

  return NULL;
}
