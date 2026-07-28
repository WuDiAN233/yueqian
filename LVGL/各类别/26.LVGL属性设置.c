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
    LVGL组件属性设置，样式设计
*/
lv_obj_t * bt1;
lv_obj_t * bt2;

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

    // 创建两个按钮
    bt1 = lv_btn_create(lv_scr_act()); // 按钮就会在开发板液晶屏上显示
    bt2 = lv_btn_create(lv_scr_act());
    // 修改按钮大小
    lv_obj_set_size(bt1, 100, 50);
    lv_obj_set_size(bt2, 100, 50);

    //设置按钮对齐  LV_ALIGN_TOP_MID表示屏幕顶部居中位置
    lv_obj_set_align(bt1,LV_ALIGN_TOP_MID);
    //更精细化的设置对齐，可以调整x，y坐标偏移
    //lv_obj_align(bt1,LV_ALIGN_TOP_MID,0,50);

    //设置两个组件之间的相对位置
    //bt2在bt1的右侧居中的位置
    lv_obj_align_to(bt2,bt1,LV_ALIGN_OUT_RIGHT_MID,10,10);

    //给按钮(其他组件也是可以设置)设置背景色：默认是蓝色背景色
    //0xff0000是红色
    lv_obj_set_style_bg_color(bt1, lv_color_hex(0xff0000), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(bt2, lv_color_hex(0x00ff00), LV_STATE_DEFAULT);
    //设置透明度: 透明度0--255  值越大，颜色越深
    lv_obj_set_style_bg_opa(bt1, 255, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(bt2, 255, LV_STATE_DEFAULT);

    //给按钮设置边框：  0表示无边框
    lv_obj_set_style_border_width(bt1, 0,LV_PART_MAIN);
    //5表示按钮2的边框宽度是5个像素点宽度
    lv_obj_set_style_border_width(bt2, 5,LV_PART_MAIN);

    //创建标签
    lv_obj_t *lb1=lv_label_create(bt1);
    
    //设置标签的文本内容
    lv_label_set_text(lb1,"login");
    //设置前景色--》文字的颜色
    lv_obj_set_style_text_color(lb1, lv_color_hex(0x0000ff), LV_STATE_DEFAULT);

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
