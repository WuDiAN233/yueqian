#include "myhead.h"

lv_obj_t *kb1 = NULL;
lv_obj_t *ta1 = NULL;
lv_obj_t *ta2 = NULL;

lv_obj_t *loginwin = NULL;
lv_obj_t *mainwin = NULL;
lv_obj_t *setwin = NULL;
lv_obj_t *chat_page = NULL;
lv_obj_t *frd_page = NULL;
lv_obj_t *set_page = NULL;
lv_obj_t *frd_list = NULL;
lv_obj_t *main_list = NULL;
lv_obj_t *chat_list = NULL;
lv_obj_t *room_list = NULL;
lv_obj_t *login_message = NULL;
lv_obj_t *search_result = NULL;
lv_obj_t *search_add_btn = NULL;

lv_obj_t *addwin = NULL;
lv_obj_t *morewin = NULL;

lv_obj_t *chatmsg = NULL;
lv_obj_t *roommsg = NULL;
lv_obj_t *feedback_ta = NULL;
lv_style_t *mystyle = NULL;

uint32_t custom_tick_get(void);
/* 使用freetype显示中文 */
lv_style_t *create_chinese_style(char *fontpath, int fontsize)
{
    static lv_ft_info_t info;
    info.name = fontpath;
    info.weight = fontsize;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;

    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, info.font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    return &style;
}

void clear_page_point( )
{
    setwin=NULL;
    morewin=NULL;
    chatmsg=NULL;
    roommsg=NULL;
    chat_list=NULL;
    room_list=NULL;
}

//这里凡是要聊天的输入框都要关联软键盘
void ta1_cb(lv_event_t * e)
{
    ta1 = lv_event_get_target(e);
    // 把软键盘跟第一个输入框关联
    lv_keyboard_set_textarea(kb1, ta1);
    lv_obj_clear_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(kb1);
}

// 输入框2的事件响应函数
void ta2_cb(lv_event_t * e)
{
    ta2 = lv_event_get_target(e);
    // 把软键盘跟第二个输入框关联
    lv_keyboard_set_textarea(kb1, ta2);
    lv_obj_clear_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(kb1);
}

// 点击空白处关闭软键盘
void close_kb_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *current = lv_event_get_current_target(e);

    if(target == current && kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
}

// 主界面 -> 聊天界面
void main_go_chat(lv_event_t * e)
{
    lv_obj_t *oldwin = lv_scr_act();
    if(setwin!=NULL && lv_obj_is_valid(setwin) && lv_obj_get_parent(setwin)==mainwin)
    {
        lv_obj_del_async(setwin);
        clear_page_point();
    }
    if(chat_page != NULL)
        lv_obj_clear_flag(chat_page, LV_OBJ_FLAG_HIDDEN);
    if(frd_page != NULL)
        lv_obj_add_flag(frd_page, LV_OBJ_FLAG_HIDDEN);
    if(set_page != NULL)
        lv_obj_add_flag(set_page, LV_OBJ_FLAG_HIDDEN);
    if(oldwin != mainwin)
    {
        lv_scr_load(mainwin);
        lv_obj_del_async(oldwin);
        clear_page_point();
    }
}

// 主界面 -> 好友界面
void main_go_frd(lv_event_t * e)
{
    lv_obj_t *oldwin = lv_scr_act();
    if(setwin!=NULL && lv_obj_is_valid(setwin) && lv_obj_get_parent(setwin)==mainwin)
    {
        lv_obj_del_async(setwin);
        clear_page_point();
    }
    show_frd();
    if(oldwin != mainwin)
    {
        lv_scr_load(mainwin);
        lv_obj_del_async(oldwin);
        clear_page_point();
    }
}

// 主界面 -> 设置界面
void main_go_set(lv_event_t * e)
{
    lv_obj_t *oldwin = lv_scr_act();
    if(setwin!=NULL && lv_obj_is_valid(setwin) && lv_obj_get_parent(setwin)==mainwin)
    {
        lv_obj_del_async(setwin);
        clear_page_point();
    }
    if(set_page == NULL)
        show_set();
    else
    {
        lv_obj_add_flag(chat_page, LV_OBJ_FLAG_HIDDEN);
        if(frd_page != NULL)
            lv_obj_add_flag(frd_page, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(set_page, LV_OBJ_FLAG_HIDDEN);
    }
    if(oldwin != mainwin)
    {
        lv_scr_load(mainwin);
        lv_obj_del_async(oldwin);
        clear_page_point();
    }
}

// 主界面 -> 登入界面  
void main_back_log(lv_event_t * e)
{
    lv_obj_t *oldwin = mainwin;
    tcp_logout();
    show_login();
    lv_obj_del_async(oldwin);
    mainwin = NULL;
    chat_page = NULL;
    frd_page = NULL;
    set_page = NULL;
    frd_list = NULL;
    main_list = NULL;
    chat_list = NULL;
    room_list = NULL;
    search_result = NULL;
    search_add_btn = NULL;
    tcp_list = NULL;
}

// 其他功能界面 -> 主界面
void set_back_main(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    lv_scr_load(mainwin);
    lv_obj_del_async(oldwin);
    setwin = NULL;
    morewin = NULL;
    chatmsg = NULL;
    roommsg = NULL;
    chat_list = NULL;
    room_list = NULL;
    search_result = NULL;
    search_add_btn = NULL;
}

lv_obj_t *create_left_menu(lv_obj_t *parent)
{
    lv_obj_t *leftwin = lv_obj_create(parent);
        //左边导航栏
    lv_obj_set_pos(leftwin, 0, 0);
    lv_obj_set_size(leftwin, 70, 480);
    lv_obj_set_style_pad_all(leftwin, 0, 0);
    lv_obj_set_style_border_width(leftwin, 0, 0);

     //创建三个按钮 用来弹出到不同界面
    lv_obj_t *bt1 = lv_btn_create(leftwin);//聊天场所
    lv_obj_t *bt2 = lv_btn_create(leftwin);//好友列表
    lv_obj_t *bt3 = lv_btn_create(leftwin);//设置界面
    lv_obj_set_pos(bt1, 5, 90);
    lv_obj_set_pos(bt2, 5, 190);
    lv_obj_set_pos(bt3, 5, 290);
    lv_obj_set_size(bt1, 60, 65);
    lv_obj_set_size(bt2, 60, 65);
    lv_obj_set_size(bt3, 60, 65);
    //增加标签
    lv_obj_t *lb1 = lv_label_create(bt1);
    lv_obj_t *lb2 = lv_label_create(bt2);
    lv_obj_t *lb3 = lv_label_create(bt3);
    lv_label_set_text(lb1, "chat");
    lv_label_set_text(lb2, "solo");
    lv_label_set_text(lb3, "sets");
    lv_obj_center(lb1);
    lv_obj_center(lb2);
    lv_obj_center(lb3);
    //增加事件
    lv_obj_add_event_cb(bt1, main_go_chat, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, main_go_frd , LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt3, main_go_set, LV_EVENT_CLICKED, NULL);

    return leftwin;
}
