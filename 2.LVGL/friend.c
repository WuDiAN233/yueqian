#include "myhead.h"


//好友界面 -> 聊天界面
void fri_chat_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    //stp接入
    show_chat();
    lv_obj_del(oldwin);
}

//创建好友界面 用来展示有多少好友 ,同时点击好友可以进入聊天界面
void show_frd()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_set_style_pad_all(setwin, 0, 0);
    lv_obj_set_style_border_width(setwin, 0, 0);

    //左边导航栏
    lv_obj_t * leftwin = lv_obj_create(setwin);
    lv_obj_set_pos(leftwin, 0, 0);
    lv_obj_set_size(leftwin, 70, 480);
    lv_obj_set_style_pad_all(leftwin, 0, 0);
    lv_obj_set_style_border_width(leftwin, 0, 0);
    lv_obj_set_style_radius(leftwin, 0, 0);
    lv_obj_set_style_bg_color(leftwin, lv_color_hex(0x2E2E2E), 0);

    //中间好友列表界面
    lv_obj_t * middlewin = lv_obj_create(setwin);
    lv_obj_set_pos(middlewin, 70, 0);
    lv_obj_set_size(middlewin, 230, 480);
    lv_obj_set_style_pad_all(middlewin, 0, 0);
    lv_obj_set_style_border_width(middlewin, 0, 0);
    lv_obj_set_style_radius(middlewin, 0, 0);
    lv_obj_set_style_bg_color(middlewin, lv_color_hex(0xF2F2F2), 0);

    //右边好友信息界面
    lv_obj_t * rightwin = lv_obj_create(setwin);
    lv_obj_set_pos(rightwin, 300, 0);
    lv_obj_set_size(rightwin, 500, 480);
    lv_obj_set_style_pad_all(rightwin, 0, 0);
    lv_obj_set_style_border_width(rightwin, 0, 0);
    lv_obj_set_style_radius(rightwin, 0, 0);
    lv_obj_set_style_bg_color(rightwin, lv_color_hex(0xFAFAFA), 0);

    //返回按钮
    lv_obj_t * bt1 = lv_btn_create(leftwin); 
    lv_obj_set_pos(bt1, 5, 405);
    lv_obj_set_size(bt1, 60, 55);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_label_set_text(lb2, "return");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, set_back_main, LV_EVENT_CLICKED, NULL);

    // 给标签设置文字 显示好友信息 
    lv_obj_t * lb1 = lv_label_create(middlewin);
    lv_obj_set_pos(lb1, 10, 15);
    lv_obj_set_size(lb1, 210, 45);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "friend list");

    // 显示好友数量
    lv_obj_t * numlb = lv_label_create(middlewin);
    lv_obj_set_pos(numlb, 10, 65);
    lv_obj_set_size(numlb, 210, 40);
    lv_obj_add_style(numlb,mystyle,0);
    lv_label_set_text(numlb, "好友数量：");
    tcp_getlist();

    // 创建好友列表区域
    lv_obj_t * list = lv_list_create(middlewin);
    lv_obj_set_pos(list, 0, 110);
    lv_obj_set_size(list, 230, 370);
    lv_obj_set_style_radius(list, 0, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    //stp接入
    // 获取好友之后使用lv_list_add_btn创建好友按钮
    // 好友按钮点击事件使用fri_chat_cb

    lv_obj_t * title = lv_label_create(rightwin);
    lv_obj_set_pos(title, 50, 185);
    lv_obj_set_size(title, 400, 80);
    lv_obj_add_style(title,mystyle,0);
    lv_label_set_text(title, "personal");

    //自己主动加载好友界面
    lv_scr_load(setwin);
}
