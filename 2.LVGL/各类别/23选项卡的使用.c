#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "lv_100ask_pinyin_ime.h"  //百问网的那个头文件
#define DISP_BUF_SIZE (1024 * 900)

/*
    选项卡的使用
       1.默认：选项卡左右滑动切换子选项
              程序员可以调用lv_tabview_set_act(tabview, 0, LV_ANIM_OFF);设置要切换的子选项
                  第二个参数代表子选项的索引号，从0开始，三个子选项分别就是0  1  2
*/

lv_obj_t * tb1; // 选项卡的地址
//定义全局变量表示子选项的索引号
int index=0;

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

//上一个子选项
void prevbt_cb(lv_event_t *e)
{
    lv_tabview_set_act(tb1,index, LV_ANIM_OFF);
    //判断
    if(index>0)
        index--;
    else
        index=2;
}

//下一个子选项
void nextbt_cb(lv_event_t *e)
{
    lv_tabview_set_act(tb1,index, LV_ANIM_OFF);
    //判断:由于我总共只有三个子选项
    if(index<2)
        index++;
    else
        index=0;
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

    //创建中文汉字样式
    lv_style_t *mystyle=create_chinese_style("/font/simkai.ttf",38);

    // 创建选项卡  LV_DIR_TOP表示在屏幕的正上方显示选项卡
    tb1 = lv_tabview_create(lv_scr_act(),LV_DIR_TOP,50);

    //设置选项卡的大小
    lv_obj_set_size(tb1,800,400);

    //给选项卡添加中文样式
    lv_obj_add_style(tb1,mystyle,0);
    //往选项卡中添加子选项：返回值就是子选项的地址
    lv_obj_t* tab1 = lv_tabview_add_tab(tb1, "界面1");
    lv_obj_t* tab2 = lv_tabview_add_tab(tb1, "界面2");
    lv_obj_t* tab3 = lv_tabview_add_tab(tb1, "界面3");

    //三个子选项分别添加组件
    //第一个子选项：添加三个按钮
    lv_obj_t *bt1=lv_btn_create(tab1);
    lv_obj_t *bt2=lv_btn_create(tab1);
    lv_obj_t *bt3=lv_btn_create(tab1);
    lv_obj_set_pos(bt1,100,50);
    lv_obj_set_pos(bt2,200,50);
    lv_obj_set_pos(bt3,300,50);
    //第二个子选项：添加文本输入框
    lv_obj_t *ta1=lv_textarea_create(tab2);
    //第三个子选项：添加开关组件
    lv_obj_t *sw1=lv_switch_create(tab3);

    //创建两个按钮左右切换子选项
    lv_obj_t *nextbt=lv_btn_create(lv_scr_act());
    lv_obj_t *prevbt=lv_btn_create(lv_scr_act());
    lv_obj_set_pos(nextbt,400,400);
    lv_obj_set_pos(prevbt,0,400);
    lv_obj_set_size(nextbt,400,80);
    lv_obj_set_size(prevbt,400,80);
    //给按钮添加事件响应函数
    lv_obj_add_event_cb(nextbt,nextbt_cb,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(prevbt,prevbt_cb,LV_EVENT_CLICKED,NULL);

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
