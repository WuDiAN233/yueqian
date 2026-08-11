#include "myhead.h"


//好友界面 -> 聊天界面
void fri_chat_cb(lv_event_t * e)
{
    //读取按钮里的IP和端口，进入对应聊天界面
    lv_obj_t *oldwin=lv_scr_act();
    lv_obj_t *bt=lv_event_get_current_target(e);
    lv_obj_t *list=lv_obj_get_parent(bt);
    const char *info=lv_list_get_btn_text(list,bt);
    char name[64]={0};
    char ip[20]={0};
    unsigned short port=0;

    if(info==NULL)
        return;

    if(sscanf(info,"%63[^@]@%19[^@]@%hu",name,ip,&port)!=3)
    {
        bzero(name,sizeof(name));
        if(sscanf(info,"%19[^@]@%hu",ip,&port)!=2)
            return;
    }

    bzero(chatip,20);
    snprintf(chatip,20,"%s",ip);
    chatport=port;
    bzero(chatname,64);
    snprintf(chatname,64,"%s",name);
    show_chat();

    if(oldwin!=mainwin)
        lv_obj_del_async(oldwin);
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
        tcp_getlist();
        if(chat_page != NULL)
            lv_obj_add_flag(chat_page, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(frd_page, LV_OBJ_FLAG_HIDDEN);
        if(set_page != NULL)
            lv_obj_add_flag(set_page, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    frd_page = lv_obj_create(mainwin);
    lv_obj_move_background(frd_page);
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
    //获取服务器上的在线客户端
    frd_list=list;
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
