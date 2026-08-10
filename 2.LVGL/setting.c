#include "myhead.h"


//设置界面 -> 登录界面
void set_back_login(lv_event_t * e)
{
    //stp接入
    show_login();
    lv_obj_del(setwin);
}

//设置界面 -> 反馈界面
void set_back_feedback_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    show_feedback();
    lv_obj_del(oldwin);
}

//反馈界面 -> 设置界面 
void feedback_back_set_cb(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    show_set();
    lv_obj_del(oldwin);
}

//保存反馈内容到记事本按钮
void save_feedback_cb(lv_event_t * e)
{
    const char *msg = lv_textarea_get_text(feedback_ta);
    if(strlen(msg) == 0)
        return;

    FILE *fp = fopen("./feedback.txt", "a+");
    if(fp == NULL)
    {
        perror("open feedback.txt");
        return;
    }

    fprintf(fp, "%s\n", msg);
    fclose(fp);
    lv_textarea_set_text(feedback_ta, "");
}

//创建设置界面 
void show_set()
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
    create_left_menu(setwin);

    //中间设置列表界面
    lv_obj_t * middlewin = lv_obj_create(setwin);
    lv_obj_set_pos(middlewin, 70, 0);
    lv_obj_set_size(middlewin, 230, 480);
    lv_obj_set_style_pad_all(middlewin, 0, 0);
    lv_obj_set_style_border_width(middlewin, 0, 0);
    lv_obj_set_style_radius(middlewin, 0, 0);
    lv_obj_set_style_bg_color(middlewin, lv_color_hex(0xF2F2F2), 0);

    //右边设置内容界面
    lv_obj_t * rightwin = lv_obj_create(setwin);
    lv_obj_set_pos(rightwin, 300, 0);
    lv_obj_set_size(rightwin, 500, 480);
    lv_obj_set_style_pad_all(rightwin, 0, 0);
    lv_obj_set_style_border_width(rightwin, 0, 0);
    lv_obj_set_style_radius(rightwin, 0, 0);
    lv_obj_set_style_bg_color(rightwin, lv_color_hex(0xFAFAFA), 0);

    lv_obj_t * lb1 = lv_label_create(middlewin);
    lv_obj_set_pos(lb1, 10, 20);
    lv_obj_set_size(lb1, 210, 50);
    lv_obj_add_style(lb1,mystyle,0);
    lv_label_set_text(lb1, "settings");

    // 退出登录按钮
    lv_obj_t * bt1 = lv_btn_create(middlewin); 
    lv_obj_set_pos(bt1, 10, 100);
    lv_obj_set_size(bt1, 210, 65);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "退出登录");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, set_back_login, LV_EVENT_CLICKED, NULL);

    // 反馈按钮
    lv_obj_t * bt2 = lv_btn_create(middlewin); 
    lv_obj_set_pos(bt2, 10, 185);
    lv_obj_set_size(bt2, 210, 65);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_add_style(lb3,mystyle,0);
    lv_label_set_text(lb3, "反馈");
    lv_obj_center(lb3);
    lv_obj_add_event_cb(bt2, set_back_feedback_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * title = lv_label_create(rightwin);
    lv_obj_set_pos(title, 50, 185);
    lv_obj_set_size(title, 400, 80);
    lv_obj_add_style(title,mystyle,0);
    lv_label_set_text(title, "settings");

    //自己主动加载设置界面
    lv_scr_load(setwin);
}

//创建反馈界面
void show_feedback()
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
    lv_label_set_text(lb1, "反馈");

    feedback_ta = lv_textarea_create(setwin);
    ta1 = feedback_ta;
    lv_obj_set_pos(feedback_ta, 80, 110);
    lv_obj_set_size(feedback_ta, 640, 230);
    lv_textarea_set_placeholder_text(feedback_ta, "请输入反馈内容");
    lv_obj_add_event_cb(feedback_ta, ta1_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt1, 600, 370);
    lv_obj_set_size(bt1, 120, 60);
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_add_style(lb2,mystyle,0);
    lv_label_set_text(lb2, "提交");
    lv_obj_center(lb2);
    lv_obj_add_event_cb(bt1, save_feedback_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * bt2 = lv_btn_create(setwin); 
    lv_obj_set_pos(bt2, 80, 370);
    lv_obj_set_size(bt2, 120, 60);
    lv_obj_t * lb3 = lv_label_create(bt2);
    lv_obj_add_style(lb3,mystyle,0);
    lv_label_set_text(lb3, "返回");
    lv_obj_center(lb3);
    lv_obj_add_event_cb(bt2, feedback_back_set_cb, LV_EVENT_CLICKED, NULL);

    lv_scr_load(setwin);
}
