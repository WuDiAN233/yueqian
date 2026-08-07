#include "myhead.h"


//聊天发送按钮
void send_chat_cb(lv_event_t * e)
{
    const char *msg = lv_textarea_get_text(ta1);
    if(strlen(msg) == 0)
        return;
    //tcp_chat("192.168.131.130", 10000, msg); // 示例IP和端口
}

//群聊发送按钮
void send_room_cb(lv_event_t * e)
{
    const char *msg = lv_textarea_get_text(ta1);
    if(strlen(msg) == 0)
        return;

    //stp接入
}

//搜索聊天记录
void search_chat_record_cb(lv_event_t * e)
{
    //stp接入
}

//清空聊天记录
void clear_chat_record_cb(lv_event_t * e)
{
    if(chatmsg != NULL)
        lv_textarea_set_text(chatmsg, "");
    if(roommsg != NULL)
        lv_textarea_set_text(roommsg, "");
    //stp接入
}

//删除好友
void delete_friend_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    //stp接入
    show_frd();
    lv_obj_del(oldwin);
}

//关闭更多功能弹框
void close_more_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *current = lv_event_get_current_target(e);

    if(target != current)
        return;

    if(morewin != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del(morewin);
        morewin = NULL;
    }
}

//聊天界面更多功能按钮
void chat_more_cb(lv_event_t * e)
{
    if(morewin != NULL)
        return;

    morewin = lv_obj_create(setwin);
    lv_obj_set_pos(morewin, 510, 55);
    lv_obj_set_size(morewin, 270, 350);
    lv_obj_move_foreground(morewin);

    ta2 = lv_textarea_create(morewin);
    lv_obj_set_pos(ta2, 10, 10);
    lv_obj_set_size(ta2, 230, 50);
    lv_textarea_set_placeholder_text(ta2, "search records");
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t *bt1 = lv_btn_create(morewin);
    lv_obj_t *bt2 = lv_btn_create(morewin);
    lv_obj_t *bt3 = lv_btn_create(morewin);
    lv_obj_t *bt4 = lv_btn_create(morewin);

    lv_obj_set_pos(bt1, 10, 75);
    lv_obj_set_pos(bt2, 10, 135);
    lv_obj_set_pos(bt3, 10, 195);
    lv_obj_set_pos(bt4, 10, 255);

    lv_obj_set_size(bt1, 230, 45);
    lv_obj_set_size(bt2, 230, 45);
    lv_obj_set_size(bt3, 230, 45);
    lv_obj_set_size(bt4, 230, 45);

    lv_obj_t *lb1 = lv_label_create(bt1);
    lv_obj_t *lb2 = lv_label_create(bt2);
    lv_obj_t *lb3 = lv_label_create(bt3);
    lv_obj_t *lb4 = lv_label_create(bt4);

    lv_obj_add_style(lb1,mystyle,0);
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);
    lv_obj_add_style(lb4,mystyle,0);

    lv_label_set_text(lb1, "搜索");
    lv_label_set_text(lb2, "清空聊天记录");
    lv_label_set_text(lb3, "删除好友");
    lv_label_set_text(lb4, "关闭");

    lv_obj_center(lb1);
    lv_obj_center(lb2);
    lv_obj_center(lb3);
    lv_obj_center(lb4);

    lv_obj_add_event_cb(bt1, search_chat_record_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, clear_chat_record_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, delete_friend_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt4, close_more_cb, LV_EVENT_CLICKED, NULL);
}

//群聊更多功能按钮
void room_more_cb(lv_event_t * e)
{
    if(morewin != NULL)
        return;

    morewin = lv_obj_create(setwin);
    lv_obj_set_pos(morewin, 510, 55);
    lv_obj_set_size(morewin, 270, 350);
    lv_obj_move_foreground(morewin);

    ta2 = lv_textarea_create(morewin);
    lv_obj_set_pos(ta2, 10, 10);
    lv_obj_set_size(ta2, 230, 50);
    lv_textarea_set_placeholder_text(ta2, "search records");
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t *bt1 = lv_btn_create(morewin);
    lv_obj_t *bt2 = lv_btn_create(morewin);
    lv_obj_t *bt3 = lv_btn_create(morewin);
    lv_obj_t *bt4 = lv_btn_create(morewin);

    lv_obj_set_pos(bt1, 10, 75);
    lv_obj_set_pos(bt2, 10, 135);
    lv_obj_set_pos(bt3, 10, 195);
    lv_obj_set_pos(bt4, 10, 255);

    lv_obj_set_size(bt1, 230, 45);
    lv_obj_set_size(bt2, 230, 45);
    lv_obj_set_size(bt3, 230, 45);
    lv_obj_set_size(bt4, 230, 45);

    lv_obj_t *lb1 = lv_label_create(bt1);
    lv_obj_t *lb2 = lv_label_create(bt2);
    lv_obj_t *lb3 = lv_label_create(bt3);
    lv_obj_t *lb4 = lv_label_create(bt4);

    lv_obj_add_style(lb1,mystyle,0);
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);
    lv_obj_add_style(lb4,mystyle,0);

    lv_label_set_text(lb1, "搜索");
    lv_label_set_text(lb2, "清空聊天记录");
    lv_label_set_text(lb3, "退出群聊");
    lv_label_set_text(lb4, "关闭");

    lv_obj_center(lb1);
    lv_obj_center(lb2);
    lv_obj_center(lb3);
    lv_obj_center(lb4);

    lv_obj_add_event_cb(bt1, search_chat_record_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, clear_chat_record_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, set_back_main, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt4, close_more_cb, LV_EVENT_CLICKED, NULL);
}

//创建聊天界面
void show_chat()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    morewin = NULL;
    roommsg = NULL;
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_set_style_pad_all(setwin, 0, 0);
    lv_obj_set_style_border_width(setwin, 0, 0);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);

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

    //右边聊天界面
    lv_obj_t * rightwin = lv_obj_create(setwin);
    lv_obj_set_pos(rightwin, 300, 0);
    lv_obj_set_size(rightwin, 500, 480);
    lv_obj_set_style_pad_all(rightwin, 0, 0);
    lv_obj_set_style_border_width(rightwin, 0, 0);
    lv_obj_set_style_radius(rightwin, 0, 0);
    lv_obj_set_style_bg_color(rightwin, lv_color_hex(0xFAFAFA), 0);

    //返回按钮
    lv_obj_t * backbt = lv_btn_create(leftwin);
    lv_obj_set_pos(backbt, 5, 405);
    lv_obj_set_size(backbt, 60, 55);
    lv_obj_t * backlb = lv_label_create(backbt);
    lv_label_set_text(backlb, "return");
    lv_obj_center(backlb);
    lv_obj_add_event_cb(backbt, set_back_main, LV_EVENT_CLICKED, NULL);

    //好友搜索输入框
    ta2 = lv_textarea_create(middlewin);
    lv_obj_set_pos(ta2, 10, 15);
    lv_obj_set_size(ta2, 210, 45);
    lv_textarea_set_placeholder_text(ta2, "Search");
    lv_textarea_set_one_line(ta2, true);
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    //好友列表
    lv_obj_t * list = lv_list_create(middlewin);
    lv_obj_set_pos(list, 0, 75);
    lv_obj_set_size(list, 230, 405);
    lv_obj_set_style_radius(list, 0, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    //stp接入
    //获取好友之后使用lv_list_add_btn创建好友按钮
    //好友按钮点击事件使用fri_chat_cb

    //聊天标题 显示好友信息
    lv_obj_t * lb1 = lv_label_create(rightwin);
    lv_obj_set_pos(lb1, 20, 10);
    lv_obj_set_size(lb1, 400, 45);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "chat with friend");
    //stp接入

    // 用来显示聊天界面的额外功能 里面有一个文本框 用来搜索聊天记录 有一个按钮用来清空聊天记录 有一个按钮 用来删除好友 
    lv_obj_t * bt1 = lv_btn_create(rightwin); 
    lv_obj_set_pos(bt1, 430, 8);
    lv_obj_set_size(bt1, 55, 45);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_label_set_text(lb2, "...");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, chat_more_cb, LV_EVENT_CLICKED, NULL);

    // 创建消息显示框
    chatmsg = lv_textarea_create(rightwin);
    lv_obj_set_pos(chatmsg, 0, 60);
    lv_obj_set_size(chatmsg, 500, 300);
    lv_textarea_set_text(chatmsg, "");
    lv_textarea_set_cursor_click_pos(chatmsg, false);
    lv_obj_set_style_radius(chatmsg, 0, 0);
    lv_obj_set_style_border_width(chatmsg, 0, 0);
    lv_obj_set_style_bg_color(chatmsg, lv_color_hex(0xF5F5F5), 0);
    //stp接入
    //tcp_chat("192.168.131.130", 10000, msg); // 示例IP和端口
    // 创建输入框：输入要聊天的信息 
    ta1 = lv_textarea_create(rightwin);
    lv_obj_set_pos(ta1, 15, 375);
    lv_obj_set_size(ta1, 370, 85);
    lv_textarea_set_placeholder_text(ta1, "send message");
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);

    // 发送按钮
    lv_obj_t * sendbt = lv_btn_create(rightwin);
    lv_obj_set_pos(sendbt, 400, 392);
    lv_obj_set_size(sendbt, 85, 50);
    lv_obj_t * sendlb = lv_label_create(sendbt);
    lv_label_set_text(sendlb, "send");
    lv_obj_center(sendlb);
    lv_obj_add_event_cb(sendbt, send_chat_cb, LV_EVENT_CLICKED, NULL);

    //自己主动加载聊天界面
    lv_scr_load(setwin);
}

//创建群聊界面
void show_room()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    morewin = NULL;
    chatmsg = NULL;
    setwin = lv_obj_create(NULL);
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_set_style_pad_all(setwin, 0, 0);
    lv_obj_set_style_border_width(setwin, 0, 0);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);

    //左边导航栏
    lv_obj_t * leftwin = lv_obj_create(setwin);
    lv_obj_set_pos(leftwin, 0, 0);
    lv_obj_set_size(leftwin, 70, 480);
    lv_obj_set_style_pad_all(leftwin, 0, 0);
    lv_obj_set_style_border_width(leftwin, 0, 0);
    lv_obj_set_style_radius(leftwin, 0, 0);
    lv_obj_set_style_bg_color(leftwin, lv_color_hex(0x2E2E2E), 0);

    //中间群聊列表界面
    lv_obj_t * middlewin = lv_obj_create(setwin);
    lv_obj_set_pos(middlewin, 70, 0);
    lv_obj_set_size(middlewin, 230, 480);
    lv_obj_set_style_pad_all(middlewin, 0, 0);
    lv_obj_set_style_border_width(middlewin, 0, 0);
    lv_obj_set_style_radius(middlewin, 0, 0);
    lv_obj_set_style_bg_color(middlewin, lv_color_hex(0xF2F2F2), 0);

    //右边群聊界面
    lv_obj_t * rightwin = lv_obj_create(setwin);
    lv_obj_set_pos(rightwin, 300, 0);
    lv_obj_set_size(rightwin, 500, 480);
    lv_obj_set_style_pad_all(rightwin, 0, 0);
    lv_obj_set_style_border_width(rightwin, 0, 0);
    lv_obj_set_style_radius(rightwin, 0, 0);
    lv_obj_set_style_bg_color(rightwin, lv_color_hex(0xFAFAFA), 0);

    //返回按钮
    lv_obj_t * backbt = lv_btn_create(leftwin);
    lv_obj_set_pos(backbt, 5, 405);
    lv_obj_set_size(backbt, 60, 55);
    lv_obj_t * backlb = lv_label_create(backbt);
    lv_label_set_text(backlb, "return");
    lv_obj_center(backlb);
    lv_obj_add_event_cb(backbt, set_back_main, LV_EVENT_CLICKED, NULL);

    //群聊搜索输入框
    ta2 = lv_textarea_create(middlewin);
    lv_obj_set_pos(ta2, 10, 15);
    lv_obj_set_size(ta2, 210, 45);
    lv_textarea_set_placeholder_text(ta2, "Search");
    lv_textarea_set_one_line(ta2, true);
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    //群聊列表
    lv_obj_t * list = lv_list_create(middlewin);
    lv_obj_set_pos(list, 0, 75);
    lv_obj_set_size(list, 230, 405);
    lv_obj_set_style_radius(list, 0, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    //stp接入

    // 给标签设置文字 显示群聊信息 
    lv_obj_t * lb1 = lv_label_create(rightwin);
    lv_obj_set_pos(lb1, 20, 10);
    lv_obj_set_size(lb1, 400, 45);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "chat room");
    //stp接入

    // 用来显示聊天界面的额外功能 里面有一个文本框 用来搜索聊天记录 有一个按钮用来清空聊天记录 有一个按钮 用来退出群聊 
    lv_obj_t * bt1 = lv_btn_create(rightwin); 
    lv_obj_set_pos(bt1, 430, 8);
    lv_obj_set_size(bt1, 55, 45);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_label_set_text(lb2, "...");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, room_more_cb, LV_EVENT_CLICKED, NULL);

    // 创建群聊消息显示框
    roommsg = lv_textarea_create(rightwin);
    lv_obj_set_pos(roommsg, 0, 60);
    lv_obj_set_size(roommsg, 500, 300);
    lv_textarea_set_text(roommsg, "");
    lv_textarea_set_cursor_click_pos(roommsg, false);
    lv_obj_set_style_radius(roommsg, 0, 0);
    lv_obj_set_style_border_width(roommsg, 0, 0);
    lv_obj_set_style_bg_color(roommsg, lv_color_hex(0xF5F5F5), 0);
    //stp接入
   
    // 创建输入框：输入要聊天的信息 
    ta1 = lv_textarea_create(rightwin);
    lv_obj_set_pos(ta1, 15, 375);
    lv_obj_set_size(ta1, 370, 85);
    lv_textarea_set_placeholder_text(ta1, "send message");
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);

    // 发送按钮
    lv_obj_t * sendbt = lv_btn_create(rightwin);
    lv_obj_set_pos(sendbt, 400, 392);
    lv_obj_set_size(sendbt, 85, 50);
    lv_obj_t * sendlb = lv_label_create(sendbt);
    lv_label_set_text(sendlb, "发送");
    lv_obj_center(sendlb);
    lv_obj_add_event_cb(sendbt, send_room_cb, LV_EVENT_CLICKED, NULL);

    //自己主动加载群聊界面
    lv_scr_load(setwin);
}
