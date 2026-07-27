#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#define DISP_BUF_SIZE (1024 * 900)

/*
    讨论事件响应函数的使用
       1.参数的使用
         lv_event_t是个结构体类型
         {
              target; //存放导致这个事件响应函数被调用那个对象的地址
              code;   //保存事件类型
              user_data; //保存外界传递过来的参数
                           lv_obj_add_event_cb(bt1, bt_cb, LV_EVENT_CLICKED, 第四个参数用来传递信息给响应函数);
         }
       2.LVGL允许多个组件共用同一个事件响应函数
         之前我写代码，每个按钮单独写个事件响应函数
         LVGL可以偷懒，让多个按钮共用一个事件响应函数(简化事件响应函数的数量)
*/
lv_obj_t * bt1;
lv_obj_t * bt2;
lv_obj_t * bt3;
lv_obj_t * bt4;
// 四个按钮共用这个事件响应函数
void bt_cb(lv_event_t * e)
{
    // 判断用户究竟点击的是哪个按钮
    if(e->target == bt1) {
        printf("用户点击的是按钮11111\n");
        printf("按钮1传递的参数是: %d\n", *((int *)(e->user_data)));
    }
    if(e->target == bt2) {
        printf("用户点击的是按钮22222\n");
        printf("按钮2传递的参数是: %f\n", *((float *)(e->user_data)));
    }
    if(e->target == bt3) {
        printf("用户点击的是按钮33333\n");
        printf("按钮3传递的参数是: %s\n", (char *)(e->user_data));
    }
    if(e->target == bt4) {
        printf("用户点击的是按钮44444\n");
    }

    // 判断导致这个响应函数被调用的事件类型
    if(e->code == LV_EVENT_CLICKED) printf("用户触发了点击事件(按钮按下去然后松开)\n");
    if(e->code == LV_EVENT_PRESSED) printf("用户触发了按下事件\n");
    if(e->code == LV_EVENT_RELEASED) printf("用户触发了松开事件\n");
}

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

    // 创建四个个按钮
    bt1 = lv_btn_create(lv_scr_act()); // 按钮就会在开发板液晶屏上显示
    bt2 = lv_btn_create(lv_scr_act());
    bt3 = lv_btn_create(lv_scr_act());
    bt4 = lv_btn_create(lv_scr_act());
    // 默认情况下：bt1会在开发板(0,0)位置显示
    // 修改按钮的坐标位置
    lv_obj_set_pos(bt1, 100, 140);
    lv_obj_set_pos(bt2, 650, 140);
    lv_obj_set_pos(bt3, 100, 340);
    lv_obj_set_pos(bt4, 650, 340);
    // 修改按钮大小
    lv_obj_set_size(bt1, 100, 50);
    lv_obj_set_size(bt2, 100, 50);
    lv_obj_set_size(bt3, 100, 50);
    lv_obj_set_size(bt4, 100, 50);

    // 给按钮添加事件响应函数:  LV_EVENT_CLICKED枚举值表示按钮点击事件
    // 四个按钮共用bt_cb
    int n1      = 78;
    float n2    = 56.89;
    char n3[10] = "gec";
    lv_obj_add_event_cb(bt1, bt_cb, LV_EVENT_CLICKED, &n1);
    lv_obj_add_event_cb(bt2, bt_cb, LV_EVENT_PRESSED, &n2);
    lv_obj_add_event_cb(bt3, bt_cb, LV_EVENT_CLICKED, n3);
    lv_obj_add_event_cb(bt4, bt_cb, LV_EVENT_RELEASED, NULL);

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
