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
    定时器的使用
       1.定时器：如果你的代码需要重复不断地做某件事情，就可以使用定时器
                比如：LVGL每隔1秒钟显示一张图片(电子相册)
       2.好处
           定时器不会干扰你的主函数运行，定时器单独重复做某件事
*/
 lv_timer_t *timer1;
 lv_timer_t *timer2;

 //定时器1的事件响应函数
 void timer1_cb(lv_timer_t *e)
 {
    printf("hello world\n");
 }
 
 //定时器2的事件响应函数
 void timer2_cb(lv_timer_t *e)
 {
    printf("你好粤嵌\n");
 }

 //按钮的事件响应函数
 void bt1_cb(lv_event_t *e)
 {
    //把两个定时器暂停
    lv_timer_pause(timer1);
    lv_timer_pause(timer2);
 }
void bt2_cb(lv_event_t *e)
 {
    //把两个定时器恢复,继续
    lv_timer_resume(timer1);
    lv_timer_resume(timer2);
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

    //创建两个定时器对象
    timer1=lv_timer_create(timer1_cb,1000,NULL); //1000ms就是1秒钟
    timer2=lv_timer_create(timer2_cb,1000,NULL);
    
    //创建按钮
    lv_obj_t *bt1=lv_btn_create(lv_scr_act());
    lv_obj_t *bt2=lv_btn_create(lv_scr_act());
    lv_obj_set_pos(bt1,400,150);
    lv_obj_set_pos(bt2,400,250);
    lv_obj_set_size(bt1,80,40);
    lv_obj_set_size(bt2,80,40);
    //添加事件响应函数
    lv_obj_add_event_cb(bt1,bt1_cb,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(bt2,bt2_cb,LV_EVENT_CLICKED,NULL);

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
