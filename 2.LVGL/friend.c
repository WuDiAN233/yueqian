#include "myhead.h"


//好友界面 -> 聊天界面
void fri_chat_cb(lv_event_t * e)
{
    //stp接入
    lv_obj_t *bt=lv_event_get_target(e);
    lv_obj_t *list=lv_obj_get_parent(bt);
    const char *info=lv_list_get_btn_text(list,bt);
    char buf[50]={0};
    strcpy(buf,info);
    char *ip=strtok(buf,"@");
    char *port=strtok(NULL,"@");
    extern char chatip[20];
    extern unsigned short chatport;
    if(ip!=NULL && port!=NULL)
    {
        bzero(chatip,20);
        strcpy(chatip,ip);
        chatport=atoi(port);
    }
    show_chat();
}

//创建好友界面 用来展示有多少好友 ,同时点击好友可以进入聊天界面
void show_frd()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    if(frd_page != NULL)
    {
        if(chat_page != NULL)
            lv_obj_add_flag(chat_page, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(frd_page, LV_OBJ_FLAG_HIDDEN);
        if(set_page != NULL)
            lv_obj_add_flag(set_page, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    frd_page = lv_obj_create(mainwin);
    lv_obj_set_size(frd_page, 800, 480);
    lv_obj_set_style_pad_all(frd_page, 0, 0);
    lv_obj_set_style_border_width(frd_page, 0, 0);

    //中间好友列表界面
    lv_obj_t * middlewin = lv_obj_create(frd_page);
    lv_obj_set_pos(middlewin, 70, 0);
    lv_obj_set_size(middlewin, 230, 480);
    lv_obj_set_style_pad_all(middlewin, 0, 0);
    lv_obj_set_style_border_width(middlewin, 0, 0);
    lv_obj_set_style_radius(middlewin, 0, 0);
    lv_obj_set_style_bg_color(middlewin, lv_color_hex(0xF2F2F2), 0);

    //右边好友信息界面
    lv_obj_t * rightwin = lv_obj_create(frd_page);
    lv_obj_set_pos(rightwin, 300, 0);
    lv_obj_set_size(rightwin, 500, 480);
    lv_obj_set_style_pad_all(rightwin, 0, 0);
    lv_obj_set_style_border_width(rightwin, 0, 0);
    lv_obj_set_style_radius(rightwin, 0, 0);
    lv_obj_t* img = lv_img_create(rightwin);
    LV_IMG_DECLARE(img1_map);
    lv_img_set_src(img, &img1_map);

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

    // 创建好友列表区域
    lv_obj_t * list = lv_list_create(middlewin);
    lv_obj_set_pos(list, 0, 110);
    lv_obj_set_size(list, 230, 370);
    lv_obj_set_style_radius(list, 0, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    //stp接入
    extern lv_obj_t *tcp_list;
    tcp_list=list;
    tcp_getlist();
    // 获取好友之后使用lv_list_add_btn创建好友按钮
    // 好友按钮点击事件使用fri_chat_cb

    lv_obj_t * title = lv_label_create(rightwin);
    lv_obj_set_pos(title, 50, 185);
    lv_obj_set_size(title, 400, 80);
    lv_obj_add_style(title,mystyle,0);
    lv_label_set_text(title, "personal");

    if(chat_page != NULL)
        lv_obj_add_flag(chat_page, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(frd_page, LV_OBJ_FLAG_HIDDEN);
    if(set_page != NULL)
        lv_obj_add_flag(set_page, LV_OBJ_FLAG_HIDDEN);
}
