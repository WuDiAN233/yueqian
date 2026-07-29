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
    LVGL学习套路
      1.所有的组件，创建的函数统一叫做
         lv_xxx_create();  xxx是组件的英文单词
         比如：
            lv_btn_create()
            lv_label_create()
      2.LVGL的函数可以划分为两大类
          第一类：专门针对某个组件的函数
               lv_btn_create()
               lv_label_set_text()
          第二类：通用函数，所有的组件都可以使用这些通用函数
               lv_obj_set_pos()
               lv_obj_set_size()
               lv_obj_get_style_text_font(lb1,&lv_font_montserrat_30);
*/
int main(void)
{
    //LVGL的初始化
    lv_init(); //分配了内存，分配了定时器(刷新界面)

    //液晶屏设备的初始化
    fbdev_init();  //open打开/dev/fb0,然后ioctl发送命令获取到液晶屏的宽，高，色深，mmap映射得到液晶屏的首地址

    //定义数组：存放液晶屏需要显示的画面数据
    static lv_color_t buf[DISP_BUF_SIZE];

    //定义结构体变量
    static lv_disp_draw_buf_t disp_buf;
    //初始化结构体和刚才定义的那个数组
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    //定义结构体变量
    static lv_disp_drv_t disp_drv;
    //初始化结构体变量
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;  //指定图像界面显示需要用到的缓冲区
    disp_drv.flush_cb   = fbdev_flush; //重点：指定LVGL显示界面，需要用到的画点/画矩形区域，给矩形区域填充颜色函数
    disp_drv.hor_res    = 800; //液晶屏的宽
    disp_drv.ver_res    = 480; //液晶屏的高
	//注册液晶屏
    lv_disp_drv_register(&disp_drv);

	//触摸屏的初始化
    evdev_init(); //打开触摸屏的驱动，但是/dev/input/event10,跟我们6818不符合，我修改成了/dev/input/event0
    //定义结构体变量
    static lv_indev_drv_t indev_drv_1;
    //初始化结构体变量
    lv_indev_drv_init(&indev_drv_1); 
    indev_drv_1.type = LV_INDEV_TYPE_POINTER; //输入设备类型，这个枚举值就表示触摸设备

    //指定触摸屏读取坐标的函数
    indev_drv_1.read_cb = evdev_read; //读取触摸屏坐标的函数
    //注册触摸屏
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    //创建按钮
    //lv_btn_create(NULL); //错误的写法，父窗口不能传递NULL，传递NULL按钮无法显示在液晶屏上
    lv_obj_t *bt1=lv_btn_create(lv_scr_act()); //按钮就会在开发板液晶屏上显示
    
    //默认情况下：bt1会在开发板(0,0)位置显示
    //修改按钮的坐标位置
    lv_obj_set_pos(bt1,400,240);
    //修改按钮大小
    lv_obj_set_size(bt1,100,80);
    //给按钮设置文字:LVGL没有专门的函数来给按钮设置文字
    /*
        按钮显示文字的思路：
           创建一个标签，把标签的父窗口设置成按钮
           在标签上显示文字
    */
   //新建标签，把按钮作为父窗口
   lv_obj_t *lb2=lv_label_create(bt1);
   //给标签设置文字
   lv_label_set_text(lb2,"login");
   //标签的字体设置大一点,我要使用30号字体
    lv_obj_set_style_text_font(lb2,&lv_font_montserrat_30,LV_STATE_DEFAULT);

    //创建一个标签: 标签是LVGL用来显示文字的一个组件
    lv_obj_t *lb1=lv_label_create(lv_scr_act());
    lv_obj_set_pos(lb1,100,50);
    lv_obj_set_size(lb1,600,80); 
    //给标签设置文字: 今天暂时无法显示中文汉字，明天可以
    lv_label_set_text(lb1,"welcome to yueqian!");
    //标签的字体设置大一点,我要使用30号字体
    lv_obj_set_style_text_font(lb1,&lv_font_montserrat_30,LV_STATE_DEFAULT);


    //一定要保留，不可以删除
    while(1) {
        lv_timer_handler();//把你刷新ui界面，监测LVGL程序是否发生事件，响应事件
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
