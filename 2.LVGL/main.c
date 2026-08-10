#include "myhead.h"

extern void tcp_update_lvgl();


//搜索好友
void search_main_cb(lv_event_t * e)
{
    const char *msg = lv_textarea_get_text(ta1);
    if(strlen(msg) == 0)
        return;

    //stp接入
}

//关闭添加功能弹框
void close_add_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *current = lv_event_get_current_target(e);

    if(target != current)
        return;

    if(addwin != NULL)
    {
        lv_obj_del(addwin);
        addwin = NULL;
    }
}

//+功能弹框 -> 输入账号添加好友界面
void add_account_cb(lv_event_t * e)
{
    addwin = NULL;
    show_add_account();
    lv_obj_del(mainwin);
}

//+功能弹框 -> IP连接界面
void add_ip_cb(lv_event_t * e)
{
    addwin = NULL;
    show_ip_connect();
    lv_obj_del(mainwin);
}

//+功能弹框 -> 发起群聊界面
void add_room_cb(lv_event_t * e)
{
    addwin = NULL;
    show_choose_room();
    lv_obj_del(mainwin);
}

// 发起群聊确认按钮
void choose_room_ok_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    //stp接入
    show_room();
    lv_obj_del(oldwin);
}

//点击加号弹出添加功能 
void add_friend_cb(lv_event_t * e)
{
    if(addwin != NULL)
        return;

    addwin = lv_obj_create(mainwin);
    lv_obj_set_pos(addwin, 240, 65);
    lv_obj_set_size(addwin, 220, 235);
    lv_obj_set_style_pad_all(addwin, 10, 0);
    lv_obj_move_foreground(addwin);

    lv_obj_t *bt1 = lv_btn_create(addwin);
    lv_obj_t *bt2 = lv_btn_create(addwin);
    lv_obj_t *bt3 = lv_btn_create(addwin);
    lv_obj_t *bt4 = lv_btn_create(addwin);

    lv_obj_set_pos(bt1, 10, 10);
    lv_obj_set_pos(bt2, 10, 65);
    lv_obj_set_pos(bt3, 10, 120);
    lv_obj_set_pos(bt4, 10, 175);

    lv_obj_set_size(bt1, 180, 45);
    lv_obj_set_size(bt2, 180, 45);
    lv_obj_set_size(bt3, 180, 45);
    lv_obj_set_size(bt4, 180, 35);

    lv_obj_t *lb1 = lv_label_create(bt1);
    lv_obj_t *lb2 = lv_label_create(bt2);
    lv_obj_t *lb3 = lv_label_create(bt3);
    lv_obj_t *lb4 = lv_label_create(bt4);

    lv_obj_add_style(lb1,mystyle,0);
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);
    lv_obj_add_style(lb4,mystyle,0);

    lv_label_set_text(lb1, "添加好友");
    lv_label_set_text(lb2, "IP连接");
    lv_label_set_text(lb3, "发起群聊");
    lv_label_set_text(lb4, "关闭");

    lv_obj_center(lb1);
    lv_obj_center(lb2);
    lv_obj_center(lb3);
    lv_obj_center(lb4);

    lv_obj_add_event_cb(bt1, add_account_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, add_ip_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, add_room_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt4, close_add_cb, LV_EVENT_CLICKED, NULL);
}

//使用IP连接按钮
void connect_ip_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    const char *ip = lv_textarea_get_text(ta1);
    const char *port = lv_textarea_get_text(ta2);
    if(strlen(ip) == 0 || strlen(port) == 0)
        return;

    extern char chatip[20];
    extern unsigned short chatport;
    bzero(chatip,20);
    strcpy(chatip,ip);
    chatport=atoi(port);
    show_chat();
    lv_obj_del(oldwin);
}

//搜索已经注册的账号添加好友按钮
void search_account_cb(lv_event_t * e)
{
    const char *msg = lv_textarea_get_text(ta1);
    if(strlen(msg) == 0)
        return;

}

//通过ip连接的界面
void show_ip_connect()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 20);
    lv_obj_set_size(lb1, 600, 70);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "IP连接");

    ta1 = lv_textarea_create(setwin);
    ta2 = lv_textarea_create(setwin);
    lv_obj_set_pos(ta1, 120, 120);
    lv_obj_set_pos(ta2, 120, 220);
    lv_obj_set_size(ta1, 560, 70);
    lv_obj_set_size(ta2, 560, 70);
    lv_textarea_set_placeholder_text(ta1, "请输入IP地址");
    lv_textarea_set_placeholder_text(ta2, "请输入端口号");
    lv_textarea_set_one_line(ta1, true);
    lv_textarea_set_one_line(ta2, true);
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 550, 330);
    lv_obj_set_size(bt1, 120, 60);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "连接");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, connect_ip_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * bt2 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt2, 30, 30);
    lv_obj_set_size(bt2, 100, 50);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_add_style(lb3,mystyle,0);
    lv_label_set_text(lb3, "返回");
    lv_obj_center(lb3);
    lv_obj_add_event_cb(bt2, set_back_main, LV_EVENT_CLICKED, NULL);

    lv_scr_load(setwin);
}

//创建添加好友界面
void show_add_account()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 30);
    lv_obj_set_size(lb1, 600, 70);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "添加好友");

    ta1 = lv_textarea_create(setwin);
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_size(ta1, 560, 70);
    lv_textarea_set_placeholder_text(ta1, "请输入好友账号");
    lv_textarea_set_one_line(ta1, true);
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 550, 260);
    lv_obj_set_size(bt1, 120, 60);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "搜索");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, search_account_cb, LV_EVENT_CLICKED, NULL);

    // 搜索结果显示区域
    lv_obj_t * result = lv_obj_create(setwin);
    lv_obj_set_pos(result, 120, 340);
    lv_obj_set_size(result, 560, 80);
    //stp接入

    lv_obj_t * bt2 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt2, 30, 30);
    lv_obj_set_size(bt2, 100, 50);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_add_style(lb3,mystyle,0);
    lv_label_set_text(lb3, "返回");
    lv_obj_center(lb3);
    lv_obj_add_event_cb(bt2, set_back_main, LV_EVENT_CLICKED, NULL);

    lv_scr_load(setwin);
}

//创建发起群聊选择好友界面
void show_choose_room()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);

    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 20);
    lv_obj_set_size(lb1, 600, 70);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "选择好友发起群聊");

    // 好友选择区域
    lv_obj_t * list = lv_obj_create(setwin);
    lv_obj_set_pos(list, 50, 100);
    lv_obj_set_size(list, 700, 280);
    //stp接入
    // 获取好友之后在这里创建复选框

    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 600, 400);
    lv_obj_set_size(bt1, 120, 55);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "确定");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, choose_room_ok_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * bt2 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt2, 50, 400);
    lv_obj_set_size(bt2, 120, 55);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_add_style(lb3,mystyle,0);
    lv_label_set_text(lb3, "返回");
    lv_obj_center(lb3);
    lv_obj_add_event_cb(bt2, set_back_main, LV_EVENT_CLICKED, NULL);

    lv_scr_load(setwin);
}

//显示主界面 
void show_main(void)
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    addwin = NULL;
    morewin = NULL;
    chatmsg = NULL;
    roommsg = NULL;
    mainwin = lv_obj_create(NULL);
    lv_obj_set_size(mainwin, 800, 480);
    lv_obj_set_style_pad_all(mainwin, 0, 0);
    lv_obj_set_style_border_width(mainwin, 0, 0);
    lv_obj_add_event_cb(mainwin, close_kb_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(mainwin, close_add_cb, LV_EVENT_CLICKED, NULL);

    //左边导航栏
    create_left_menu(setwin);
    //中间聊天列表界面
    lv_obj_t * middlewin = lv_obj_create(mainwin);
    lv_obj_set_pos(middlewin, 70, 0);
    lv_obj_set_size(middlewin, 230, 480);
    lv_obj_set_style_pad_all(middlewin, 0, 0);
    lv_obj_set_style_border_width(middlewin, 0, 0);
    lv_obj_set_style_radius(middlewin, 0, 0);
    lv_obj_set_style_bg_color(middlewin, lv_color_hex(0xF2F2F2), 0);

    //右边聊天内容界面
    lv_obj_t * rightwin = lv_obj_create(mainwin);
    lv_obj_set_pos(rightwin, 300, 0);
    lv_obj_set_size(rightwin, 500, 480);
    lv_obj_set_style_pad_all(rightwin, 0, 0);
    lv_obj_set_style_border_width(rightwin, 0, 0);
    lv_obj_set_style_radius(rightwin, 0, 0);
    lv_obj_t* img = lv_img_create(rightwin);
    LV_IMG_DECLARE(img1_map);
    lv_img_set_src(img, &img1_map);

    //搜索界面 
    ta1 = lv_textarea_create(middlewin);
    // 设置输入框坐标，大小
    lv_obj_set_pos(ta1, 10, 15);
    lv_obj_set_size(ta1, 165, 45);
    // 设置文本输入默认提示文字
    lv_textarea_set_placeholder_text(ta1, "Search");
    lv_textarea_set_one_line(ta1, true);
    //点击之后弹出软键盘
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);
    //输入后弹出相关人员信息
    //点击可以进入聊天界面
    //stp接入

    //创建添加好友按钮
    lv_obj_t *bt4 = lv_btn_create(middlewin);
    lv_obj_set_pos(bt4, 185, 15);
    lv_obj_set_size(bt4, 35, 45);
    lv_obj_t *lb4 = lv_label_create(bt4);
    lv_label_set_text(lb4, "+");
    lv_obj_center(lb4);
    //点击后弹出添加好友界面以弹框形式出现
    lv_obj_add_event_cb(bt4, add_friend_cb, LV_EVENT_CLICKED, NULL);
    //添加好友界面有三个功能
    //yi.输入好友账号，点击搜索，弹出相关信息
    //er.以IP连接，点击后进入ip连接界面 有两个输入框 一个输入框输入IP地址，另一个输入框输入端口号，点击连接后进入聊天界面
    //san.发起群聊 点击后弹出好友列表 用来勾选1个或多个好友，点击确定后进入群聊界面

    //创建聊天列表区域
    lv_obj_t * list = lv_list_create(middlewin);
    lv_obj_set_pos(list, 0, 75);
    lv_obj_set_size(list, 230, 405);
    lv_obj_set_style_radius(list, 0, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    //stp接入
    //获取聊天列表之后使用lv_list_add_btn创建聊天按钮


    lv_obj_t *title = lv_label_create(rightwin);
    lv_obj_set_pos(title, 40, 170);
    lv_obj_set_size(title, 420, 100);
    lv_obj_add_style(title, mystyle, 0);
    lv_label_set_text(title, "Welcome chat room");

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

    kb1 = lv_keyboard_create(lv_layer_top());
    lv_obj_set_size(kb1, 800, 200);
    lv_obj_align(kb1, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_keyboard_set_mode(kb1, LV_KEYBOARD_MODE_TEXT_LOWER);
    lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(kb1, close_kb_cb, LV_EVENT_CANCEL, NULL);
    lv_obj_add_event_cb(kb1, close_kb_cb, LV_EVENT_READY, NULL);

    tcp_init();
    show_login();

    while(1)
    {
        tcp_update_lvgl();
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
