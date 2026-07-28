#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define DISP_BUF_SIZE (1024 * 900)

/*
    复选框的使用

*/

lv_obj_t * cb1; //复选框1
lv_obj_t * cb2; //复选框2
lv_obj_t * cb3; //复选框3

//三个复选框公用一个事件响应函数
void cb_fun(lv_event_t * e)
{
    //判断用户是不是勾选了对应的复选框
    if(e->target==cb1) //复选框1被点击了
    {
        //进一步判断复选框1是勾上了，还是取消了勾选
        //方法：跟早上学习的开关组件的方法一模一样
        if(lv_obj_has_state(cb1, LV_STATE_CHECKED))
            printf("用户勾选了钱多\n");
        else
            printf("用户取消了勾选了钱多\n");
    }
    if(e->target==cb2) //复选框2被点击了
    {
        //进一步判断复选框2是勾上了，还是取消了勾选
        //方法：跟早上学习的开关组件的方法一模一样
        if(lv_obj_has_state(cb2, LV_STATE_CHECKED))
            printf("用户勾选了事少\n");
        else
            printf("用户取消了勾选了事少\n");
    }
    if(e->target==cb3) //复选框3被点击了
    {
        //进一步判断复选框3是勾上了，还是取消了勾选
        //方法：跟早上学习的开关组件的方法一模一样
        if(lv_obj_has_state(cb3, LV_STATE_CHECKED))
            printf("用户勾选了离家近\n");
        else
            printf("用户取消了勾选了离家近\n");
    }
};


int main(void)
{
    // LVGL的初始化
    lv_init(); // 分配了内存，分配了定时器(刷新界面)

    // 液晶屏设备的初始化
    fbdev_init(); // open打开/dev/fb0,然后ioctl发送命令获取到液晶屏的宽，高，色深，mmap映射得到液晶屏的首地址

    // 定义数组：存放液晶屏需要显示的画面数据
    static lv_color_t buf[DISP_BUF_SIZE];

    // 定义结构体变量
    static lv_disp_draw_buf_t disp_buf;
    // 初始化结构体和刚才定义的那个数组
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    // 定义结构体变量
    static lv_disp_drv_t disp_drv;
    // 初始化结构体变量
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;   // 指定图像界面显示需要用到的缓冲区
    disp_drv.flush_cb = fbdev_flush; // 重点：指定LVGL显示界面，需要用到的画点/画矩形区域，给矩形区域填充颜色函数
    disp_drv.hor_res  = 800;         // 液晶屏的宽
    disp_drv.ver_res  = 480;         // 液晶屏的高
    // 注册液晶屏
    lv_disp_drv_register(&disp_drv);

    // 触摸屏的初始化
    evdev_init(); // 打开触摸屏的驱动，但是/dev/input/event10,跟我们6818不符合，我修改成了/dev/input/event0
    // 定义结构体变量
    static lv_indev_drv_t indev_drv_1;
    // 初始化结构体变量
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER; // 输入设备类型，这个枚举值就表示触摸设备

    // 指定触摸屏读取坐标的函数
    indev_drv_1.read_cb = evdev_read; // 读取触摸屏坐标的函数
    // 注册触摸屏
    lv_indev_t * mouse_indev = lv_indev_drv_register(&indev_drv_1);

    //创建复选框:表示你找工作的条件
    cb1=lv_checkbox_create(lv_scr_act());
    cb2=lv_checkbox_create(lv_scr_act());
    cb3=lv_checkbox_create(lv_scr_act());

    //设置大小，坐标
    lv_obj_set_pos(cb1,300,100);
    lv_obj_set_pos(cb2,300,200);
    lv_obj_set_pos(cb3,300,300);
    lv_obj_set_size(cb1,100,50);
    lv_obj_set_size(cb2,100,50);
    lv_obj_set_size(cb3,100,50);   
    
    //给复选框设置文字内容
    lv_checkbox_set_text(cb1,"money duo");
    lv_checkbox_set_text(cb2,"shi shao");
    lv_checkbox_set_text(cb3,"home jin");

    //给复选框添加事件响应函数:用来判断用户是否点击了复选框
    lv_obj_add_event_cb(cb1,cb_fun,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(cb2,cb_fun,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(cb3,cb_fun,LV_EVENT_CLICKED,NULL);

    // 一定要保留，不可以删除
    while(1) {
        lv_timer_handler(); // 把你刷新ui界面，监测LVGL程序是否发生事件，响应事件
        usleep(5000);
    }

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
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
