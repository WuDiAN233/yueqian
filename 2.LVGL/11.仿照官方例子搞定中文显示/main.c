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
#include <fcntl.h>  //open的头文件
#include <unistd.h>  //read的头文件
#include <errno.h>  //perror的头文件
#include <linux/input.h>  //输入子系统模型有关的头文件
#include <sys/ioctl.h>   //ioctl的头文件
#define DISP_BUF_SIZE (1024 * 900)
#define TEST_MAGIC 'x'                    //定义幻数
#define LED1 _IO(TEST_MAGIC, 0)              
#define LED2 _IO(TEST_MAGIC, 1)
#define LED3 _IO(TEST_MAGIC, 2)
#define LED4 _IO(TEST_MAGIC, 3) 
#define LED_ON  	0	//灯亮
#define LED_OFF		1   //灯灭

lv_obj_t * loginwin; // 登录界面的地址
lv_obj_t * mainwin;  // 主界面的地址
lv_obj_t * funwin;  // 功能的地址
lv_style_t *mystyle; //保存你初始化好的中文字体样式
lv_obj_t * sw1;//led开关
lv_obj_t * sw2;
lv_obj_t * sw3;
lv_obj_t * sw4;
lv_obj_t * sw5;//蜂鸣器开关

//自己封装一个函数(受到官方例子的启发)，使用freetype显示中文
/*
    fontpath: 某个字体的路径名，开发板必须有，别写错
    fontsize： 字体的大小
*/
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

// 开关组件的事件响应函数
void bt1_cb(lv_event_t * e) // 关闭当前的登录界面
{
    // 由于现在没有做软键盘，直接跳到主界面
    // 显示主界面
    show_main();
    lv_obj_del(loginwin);
}

void bt2_cb(lv_event_t * e) //注册按钮
{
    printf("注册按钮被点击了888\n");
}

void bt3_cb(lv_event_t * e) // 关闭当前的控制界面
{
    // 由于现在没有做软键盘，直接跳到主界面
    // 显示主界面
    show_funt();
    lv_obj_del(loginwin);
}

void retbt_cb(lv_event_t * e)    //关闭当前的主界面
{
    //显示登录界面
    show_login();

    lv_obj_del(mainwin);

}

void retbt_cb1(lv_event_t * e)    //关闭当前的主界面
{
    //显示登录界面
    show_login();

    lv_obj_del(funwin);

}

static void event_cb(lv_event_t * e)//封装按钮
{
    int fd = open("/dev/Led",O_RDWR);

    if(lv_obj_has_state(sw1,LV_STATE_CHECKED))
    {
        printf("LED1 ON\n");
        ioctl(fd,LED1,LED_ON);
    }
    else
    {
        printf("LED1 off\n");
        ioctl(fd,LED1,LED_OFF);
    }
    

    if(lv_obj_has_state(sw2,LV_STATE_CHECKED))
    {
        printf("LED2 ON\n");
        ioctl(fd,LED2,LED_ON);
    }
    else
    {
        printf("LED2 off\n");
        ioctl(fd,LED2,LED_OFF);
    }

    if(lv_obj_has_state(sw3,LV_STATE_CHECKED))
    {
        printf("LED3 ON\n");
        ioctl(fd,LED3,LED_ON);
    }
    else
    {
        printf("LED3 off\n");
        ioctl(fd,LED3,LED_OFF);
    }

    if(lv_obj_has_state(sw4,LV_STATE_CHECKED))
    {
        printf("LED4 ON\n");
        ioctl(fd,LED4,LED_ON);
    }
    else
    {
        printf("LED4 off\n");
        ioctl(fd,LED4,LED_OFF);
    }

    if(lv_obj_has_state(sw5,LV_STATE_CHECKED))
    {
    printf("beep ON\n");
    beep_ctrl(1);
    }
    else
    {
    printf("beep OFF\n");
    beep_ctrl(0);
    }
    }

void beep_ctrl(int cmd)//封装蜂鸣器
{
    int fd1 = open("/dev/beep",O_RDWR);
    if(fd1 < 0)
    {
    perror("beep open");
    }
    if(cmd)
        ioctl(fd1,0,1);
    else
        ioctl(fd1,1,1);

    close(fd1);
}

int show_login()// 封装函数显示登录界面
{
    // 创建空白的界面作为登录界面
    loginwin = lv_obj_create(NULL);
    // 调整登录界面的大小-->跟开发板液晶屏大小一模一样
    lv_obj_set_size(loginwin, 800, 480);

    // 创建标签：显示标题
    lv_obj_t * lb1 = lv_label_create(loginwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb1, "欢迎登录粤嵌学生管理系统");
   

    // 创建两个输入框：输入用户名和密码
    lv_obj_t * ta1 = lv_textarea_create(loginwin);
    lv_obj_t * ta2 = lv_textarea_create(loginwin);
    // 设置输入框坐标，大小
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_pos(ta2, 120, 250);
    lv_obj_set_size(ta1, 560, 80);
    lv_obj_set_size(ta2, 560, 80);
    // 让输入框使用中文样式
    lv_obj_add_style(ta1,mystyle,0);
    lv_obj_add_style(ta2,mystyle,0);
    // 设置文本输入默认提示文字
    lv_textarea_set_placeholder_text(ta1, "请输入用户名");
    lv_textarea_set_placeholder_text(ta2, "请输入密码");

    // 创建两个按钮
    lv_obj_t * bt1 = lv_btn_create(loginwin); // 按钮就会在开发板液晶屏上显示
    lv_obj_t * bt2 = lv_btn_create(loginwin);
    lv_obj_t * bt3 = lv_btn_create(loginwin);
    // 默认情况下：bt1会在开发板(0,0)位置显示
    // 修改按钮的坐标位置
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_pos(bt2, 650, 340);
    lv_obj_set_pos(bt3, 750, 500);
    // 修改按钮大小
    lv_obj_set_size(bt1, 100, 50);
    lv_obj_set_size(bt2, 100, 50);
    lv_obj_set_size(bt3, 100, 50);
    // 给按钮设置文字:LVGL没有专门的函数来给按钮设置文字
    /*
        按钮显示文字的思路：
           创建一个标签，把标签的父窗口设置成按钮
           在标签上显示文字
    */
    // 新建标签，把按钮作为父窗口
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_t * lb4 = lv_label_create(bt3);
    // 标签设置中文样式
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);
    lv_obj_add_style(lb4,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb2, "登录");
    lv_label_set_text(lb3, "注册");
    lv_label_set_text(lb4, "控制");

    
    // 给按钮添加事件响应函数:  LV_EVENT_CLICKED枚举值表示按钮点击事件
    lv_obj_add_event_cb(bt1, bt1_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, bt2_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, bt3_cb, LV_EVENT_CLICKED, NULL);

    //自己主动加载登录界面
    lv_scr_load(loginwin);
}

int show_shop() //封装函数显示商品界面
{
    funwin = lv_obj_create(NULL);
    // 调整登录界面的大小-->跟开发板液晶屏大小一模一样
    lv_obj_set_size(funwin, 800, 480);
    // 创建标签：显示标题
    lv_obj_t * lb1 = lv_label_create(funwin);
    lv_obj_set_pos(lb1, 400, 400);
    lv_obj_set_size(lb1, 600, 80);
    // 标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb1, "商店");



    lv_scr_load(funwin);
    //创建按钮代表返回
    lv_obj_t * retbt = lv_btn_create(funwin); 
    //设置坐标和大小
    lv_obj_set_pos(retbt,800,300);
    lv_obj_set_size(retbt,100,50);
    //给返回按钮添加事件响应函数
    lv_obj_add_event_cb(retbt, retbt_cb1, LV_EVENT_CLICKED, NULL);
    //创建标签嵌套到返回按钮上
    lv_obj_t * lb2 = lv_label_create(retbt);
    // 标签使用中文样式
    lv_obj_add_style(lb2,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb2,"返回");

}

int show_funt() // 封装函数显示控制界面
{
    funwin = lv_obj_create(NULL);
    // 调整登录界面的大小-->跟开发板液晶屏大小一模一样
    lv_obj_set_size(funwin, 800, 480);
    // 创建标签：显示标题
    lv_obj_t * lb1 = lv_label_create(funwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb1, "控制面板");
    // 创建开关组件
    sw1 = lv_switch_create(funwin); // 按钮就会在开发板液晶屏上显示
    sw2 = lv_switch_create(funwin);
    sw3 = lv_switch_create(funwin);
    sw4 = lv_switch_create(funwin);
    sw5 = lv_switch_create(funwin);
    // 修改开关组件的坐标位置
    lv_obj_set_pos(sw1, 100, 40);
    lv_obj_set_pos(sw2, 100, 140);
    lv_obj_set_pos(sw3, 100, 240);
    lv_obj_set_pos(sw4, 100, 340);
    lv_obj_set_pos(sw5, 100, 440);
    // 修改开关组件大小
    lv_obj_set_size(sw1, 100, 50);
    lv_obj_set_size(sw2, 100, 50);
    lv_obj_set_size(sw3, 100, 50);
    lv_obj_set_size(sw4, 100, 50);
    lv_obj_set_size(sw5, 100, 50);
    // 给开关组件添加事件响应函数:  LV_EVENT_CLICKED枚举值表示按钮点击事件
    lv_obj_add_event_cb(sw1, event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(sw2, event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(sw3, event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(sw4, event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(sw5, event_cb, LV_EVENT_CLICKED, NULL);

    lv_scr_load(funwin);
    //创建按钮代表返回
    lv_obj_t * retbt = lv_btn_create(funwin); 
    //设置坐标和大小
    lv_obj_set_pos(retbt,450,300);
    lv_obj_set_size(retbt,100,50);
    //给返回按钮添加事件响应函数
    lv_obj_add_event_cb(retbt, retbt_cb1, LV_EVENT_CLICKED, NULL);
    //创建标签嵌套到返回按钮上
    lv_obj_t * lb2 = lv_label_create(retbt);
    // 标签使用中文样式
    lv_obj_add_style(lb2,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb2,"返回");
    
}

int show_main() // 封装函数显示主界面
{
    // 创建空白的界面作为主界面
    mainwin = lv_obj_create(NULL);
    // 调整登录界面的大小-->跟开发板液晶屏大小一模一样
    lv_obj_set_size(mainwin, 800, 480);
   
    // 创建标签：显示标题
    lv_obj_t * lb1 = lv_label_create(mainwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb1, "欢迎使用粤嵌学生管理系统");
   
    //创建按钮代表返回
    lv_obj_t * retbt = lv_btn_create(mainwin); 
    //设置坐标和大小
    lv_obj_set_pos(retbt,350,50);
    lv_obj_set_size(retbt,100,50);
    //给返回按钮添加事件响应函数
    lv_obj_add_event_cb(retbt, retbt_cb, LV_EVENT_CLICKED, NULL);
    //创建标签嵌套到返回按钮上
    lv_obj_t * lb2 = lv_label_create(retbt);
    // 标签使用中文样式
    lv_obj_add_style(lb2,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb2,"返回");

    //自己主动加载主界面
    lv_scr_load(mainwin);
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

    //调用自己封装的函数使用freetype显示中文
    mystyle=create_chinese_style("/font/simkai.ttf",34);

    // 显示登录界面
    show_login();

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
