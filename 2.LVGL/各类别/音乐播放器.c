#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "lv_100ask_pinyin_ime.h" //百问网的那个头文件
#include "lv_indev.h"
#define DISP_BUF_SIZE (1024 * 900)

/*
    音乐播放器
*/

lv_obj_t * mystyle;

// 定义指针数组存放六个按钮的地址
lv_obj_t * btarray[6];
// 定义指针数组存放六个标签的地址
lv_obj_t * lbarray[6];
// 定义数组存放标签上要显示文字
char * msgarray[6] = {"播放", "暂停", "继续", "退出", "上一首", "下一首"};
// 定义全局变量，表示当前播放的歌曲下标
int n = 0;

// 定义二维数组--》当然复杂一点，你可以采用双向循环链表存放也行
// 偷懒了：不需要拼接字符串，直接把完整的播放命令存放起来了
char mp3path[5][100] = {"madplay /otherspace/1.mp3 &", "madplay /otherspace/2.mp3 &", "madplay /otherspace/3.mp3 &",
                        "madplay /otherspace/4.mp3 &", "madplay /otherspace/5.mp3 &"};

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

// 六个按钮共用响应函数
void bt_cb(lv_event_t * e)
{
    // 判断用户究竟点击的是哪个按钮
    if(e->target == btarray[0]) // 播放音乐
    {
        system(mp3path[n]); // 播放第一首
    }
    if(e->target == btarray[1]) // 暂停音乐
    {
        system("killall -STOP madplay");
    }
    if(e->target == btarray[2]) // 继续音乐
    {
        system("killall -CONT madplay");
    }
    if(e->target == btarray[3]) // 退出音乐
    {
        system("killall -KILL madplay");
    }
    if(e->target == btarray[4]) // 上一首音乐
    {
        // 关闭正在播放的歌曲
        system("killall -KILL madplay");
        // 播放上一首
        if(n > 0)
            n--;
        else
            n = 4; // 我总共五首歌曲
        system(mp3path[n]);
    }
    if(e->target == btarray[5]) // 下一首音乐
    {
        // 关闭正在播放的歌曲
        system("killall -KILL madplay");
        // 播放下一首
        if(n < 4)
            n++;
        else
            n = 0; // 我总共五首歌曲
        system(mp3path[n]);
    }
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

    // 创建中文汉字样式
    mystyle = create_chinese_style("/font/simkai.ttf", 24);

    // 创建六个按钮,六个标签
    int i;
    for(i = 0; i < 6; i++) {
        // 创建按钮
        btarray[i] = lv_btn_create(lv_scr_act());
        // 设置位置坐标
        lv_obj_set_pos(btarray[i], 35 + (100 + 25) * i, 300);
        // 设置大小
        lv_obj_set_size(btarray[i], 100, 60);
        // 创建标签
        lbarray[i] = lv_label_create(btarray[i]);
        // 给标签添加中文样式
        lv_obj_add_style(lbarray[i], mystyle, 0);
        // 给标签设置文字内容
        lv_label_set_text(lbarray[i], msgarray[i]);
        // 给按钮添加事件响应函数
        lv_obj_add_event_cb(btarray[i], bt_cb, LV_EVENT_CLICKED, NULL);
    }

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
