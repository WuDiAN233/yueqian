#include "myhead.h"

lv_obj_t *kb1 = NULL;
lv_obj_t *ta1 = NULL;
lv_obj_t *ta2 = NULL;

lv_obj_t *loginwin = NULL;
lv_obj_t *mainwin = NULL;
lv_obj_t *setwin = NULL;

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

// 主界面 -> 登入界面  
void main_back_log(lv_event_t * e)
{
    show_login();
    lv_obj_del(mainwin);
}

// 其他功能界面 -> 主界面
void set_back_main(lv_event_t * e)
{
    show_main();
    lv_obj_del(setwin);
}