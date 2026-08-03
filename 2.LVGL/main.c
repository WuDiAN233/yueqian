#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <sys/ioctl.h>

#define DISP_BUF_SIZE (1024 * 900)
lv_obj_t *mainwin;   // 主界面
lv_style_t *mystyle; // 中文字体样式

void show_main(void);
uint32_t custom_tick_get(void);
int num = 1;
int nu = 1;

const char *song_cmd[] =
{
    "madplay /WuDiAN/1.mp3 &",
    "madplay /WuDiAN/2.mp3 &",
    "madplay /WuDiAN/3.mp3 &",
    "madplay /WuDiAN/4.mp3 &",
    "madplay /WuDiAN/5.mp3 &",
};

lv_style_t *create_chinese_style(char *fontpath,int fontsize)
{
    //定义结构体变量，结构体代表你要使用的中文字体
    static lv_ft_info_t info;
    //初始化结构体成员
    //这是开发板的路径
    info.name = fontpath; //传递参数，把字体的路径名传递过来
    info.weight = fontsize; //设置中文字体大小
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    //新建样式，让样式跟中文字体绑定
    //样式：跟计算机网页设计中的样式的概念类似，字体，前景色，背景色...都叫做样式
    static lv_style_t style;
    //初始化样式对象
    lv_style_init(&style); 
    //样式跟字体绑定在一起，等一会就能使用你指定的字体(宋体)
    lv_style_set_text_font(&style, info.font);
    //设置文字显示居中对齐
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    //返回准备好的样式，让调用者使用
    return &style;
}
/* 先声明函数，下面才能互相调用 */


void next(lv_event_t * e)
{
    nu++;
    if(nu>4)nu=0;
    system("killall -KILL madplay");
    system(song_cmd[nu]);
}

void cost(lv_event_t * e)
{
 
    printf("LED2 ON\n");
    if(num==0){
        system("killall -CONT madplay");
        num = 1;
        printf("音乐继续\n");
    }
    else {
        system("killall -STOP madplay");
        num = 0;
        printf("音乐已暂停\n");
    }
}


void back(lv_event_t * e)
{
    nu--;
    if(nu<0)nu=4;
    system("killall -KILL madplay");
    system(song_cmd[nu]);
}

void show_main(void)
{
    mainwin = lv_obj_create(NULL);
    lv_obj_set_size(mainwin, 800, 480);

    lv_obj_t *title = lv_label_create(mainwin);
    lv_obj_set_pos(title, 80, 40);
    lv_obj_set_size(title, 640, 80);
    lv_obj_add_style(title, mystyle, 0);
    lv_label_set_text(title, "音乐播放界面");
    system("madplay /otherspace/1.mp3 &");

    lv_obj_t *bt1 = lv_btn_create(mainwin);
    lv_obj_t *bt2 = lv_btn_create(mainwin);
    lv_obj_t *bt3 = lv_btn_create(mainwin); 
    lv_obj_set_pos(bt1, 100, 180);
    lv_obj_set_pos(bt2, 300, 180);
    lv_obj_set_pos(bt3, 500, 180);
    lv_obj_set_size(bt1, 150, 70);
    lv_obj_set_size(bt2, 150, 70);
    lv_obj_set_size(bt3, 150, 70);
    lv_obj_add_event_cb(bt1, next, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, cost, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, back, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lb1 = lv_label_create(bt1);
    lv_obj_add_style(lb1, mystyle, 0);
    lv_label_set_text(lb1, "上一首");
    lv_obj_center(lb1);

    lv_obj_t * lb2 = lv_label_create(bt2);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2,"暂停/继续");
    lv_obj_center(lb2);

    lv_obj_t *lb3 = lv_label_create(bt3);
    lv_obj_add_style(lb3, mystyle, 0);
    lv_label_set_text(lb3, "下一首");
    lv_obj_center(lb3);

    lv_scr_load(mainwin);

}

int main(void)
{
    lv_init();

    fbdev_init();

    static lv_color_t buf[DISP_BUF_SIZE];

    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    disp_drv.hor_res = 800;
    disp_drv.ver_res = 480;
    lv_disp_drv_register(&disp_drv);

    evdev_init();

    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb = evdev_read;
    lv_indev_drv_register(&indev_drv_1);

    mystyle = create_chinese_style("/font/simkai.ttf", 34);

    show_main();

    while(1)
    {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

/* Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR` */
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0)
    {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
