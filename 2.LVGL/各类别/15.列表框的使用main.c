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
    列表框的使用
       1.如果文字内容太多，会自动显示滑动条
       2.列表框可以添加文字，也可以添加按钮(按钮可以点击)
       3.如果用户点击了按钮，代码如何判断用户点击了哪个按钮
            思路：给按钮添加事件响应函数
                 不是给列表框添加事件响应函数
                    char *lv_list_get_btn_text(list, 列表中的某个按钮组件的地址);
*/
lv_obj_t * list1; // 列表框对象的地址

// 五个按钮的事件响应函数
void bt_cb(lv_event_t * e)
{
    printf("你点击的按钮对象首地址是: %p\n", e->target);
    char * btmsg = lv_list_get_btn_text(list1, e->target);
    printf("你点击的按钮，文字内容是: %s\n", btmsg);
}

// 自己封装一个函数(受到官方例子的启发)，使用freetype显示中文
/*
    fontpath: 某个字体的路径名，开发板必须有，别写错
    fontsize： 字体的大小
*/
lv_style_t * create_chinese_style(char * fontpath, int fontsize)
{
    // 定义结构体变量，结构体代表你要使用的中文字体
    static lv_ft_info_t info;
    // 初始化结构体成员
    // 这是开发板的路径
    info.name   = fontpath; // 传递参数，把字体的路径名传递过来
    info.weight = fontsize; // 设置中文字体大小
    info.style  = FT_FONT_STYLE_NORMAL;
    info.mem    = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    // 新建样式，让样式跟中文字体绑定
    // 样式：跟计算机网页设计中的样式的概念类似，字体，前景色，背景色...都叫做样式
    static lv_style_t style;
    // 初始化样式对象
    lv_style_init(&style);
    // 样式跟字体绑定在一起，等一会就能使用你指定的字体(宋体)
    lv_style_set_text_font(&style, info.font);
    // 设置文字显示居中对齐
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    // 返回准备好的样式，让调用者使用
    return &style;
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

    // 准备中文样式
    lv_style_t * mystyle = create_chinese_style("/font/simkai.ttf", 34);

    // 创建列表框用来显示歌曲名字--》歌曲播放列表
    list1 = lv_list_create(lv_scr_act());

    // 设置大小
    lv_obj_set_size(list1, 400, 480);

    // 列表框跟中文样式绑定
    lv_obj_add_style(list1, mystyle, 0);
    // 设置文本内容
    lv_list_add_text(list1, "周杰伦-双截棍");
    lv_list_add_text(list1, "周杰伦-龙拳");
    lv_list_add_text(list1, "刘德华-忘情水");
    lv_list_add_text(list1, "张学友-饿狼传说");
    lv_list_add_text(list1, "陈奕迅-富士山下");

    // 添加按钮到列表框
    /*
        第二个参数：LV_SYMBOL_WIFI(wifi的图标)表示LVGL内部定义的按钮图标
                   开发板上暂时无法显示图标
                   传递NULL表示不使用图标
    */
    lv_obj_t * btn1 = lv_list_add_btn(list1, NULL, "WIFI");
    lv_obj_t * btn2 = lv_list_add_btn(list1, LV_SYMBOL_KEYBOARD, "键盘");
    lv_obj_t * btn3 = lv_list_add_btn(list1, LV_SYMBOL_BATTERY_FULL, "电量");
    lv_obj_t * btn4 = lv_list_add_btn(list1, LV_SYMBOL_FILE, "文件");
    lv_obj_t * btn5 = lv_list_add_btn(list1, LV_SYMBOL_BLUETOOTH, "蓝牙");

    printf("按钮地址: %p\n", btn1);
    printf("按钮地址: %p\n", btn2);
    printf("按钮地址: %p\n", btn3);
    printf("按钮地址: %p\n", btn4);
    printf("按钮地址: %p\n", btn5);
    // 给五个按钮添加事件响应函数
    /*
        新的事件类型：LV_EVENT_ALL表示跟这个组件有关的任何事件，都可以响应
                     LV_EVENT_CLICKED
    */
    lv_obj_add_event_cb(btn1, bt_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn2, bt_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn3, bt_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn4, bt_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn5, bt_cb, LV_EVENT_CLICKED, NULL);

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
