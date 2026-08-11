#include "myhead.h"


//登录界面 -> 注册界面 
void log_go_sig(lv_event_t *e)
{
    lv_obj_t *oldwin = loginwin;
    show_sign();
    lv_obj_del_async(oldwin);
    loginwin = NULL;
}

//登入界面 ->主界面
void log_go_main(lv_event_t *e) // 登录按钮
{
    const char *username = lv_textarea_get_text(ta1);
    const char *password = lv_textarea_get_text(ta2);
    if(strlen(username) == 0 || strlen(password) == 0)
    {
        lv_label_set_text(login_message, "用户名和密码不能为空");
        return;
    }

    if(tcp_login(username,password) == 0)
        lv_label_set_text(login_message, "正在登录...");
    else
        lv_label_set_text(login_message, "服务器未连接");
}

//注册界面 ->主菜单
void set_go_main(lv_event_t * e) // 注册按钮
{
    const char *username = lv_textarea_get_text(ta1);
    const char *password = lv_textarea_get_text(ta2);
    if(strlen(username) == 0 || strlen(password) == 0)
    {
        lv_label_set_text(login_message, "用户名和密码不能为空");
        return;
    }

    if(tcp_register(username,password) == 0)
        lv_label_set_text(login_message, "正在注册...");
    else
        lv_label_set_text(login_message, "服务器未连接");
}

// 注册界面 -> 登录界面
void set_back_log(lv_event_t * e)
{
    lv_obj_t *oldwin = setwin;
    show_login();
    lv_obj_del_async(oldwin);
    setwin = NULL;
}

void show_login()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    loginwin = lv_obj_create(NULL);
    login_message = NULL;
    lv_obj_set_size(loginwin, 800, 480);
    lv_obj_add_event_cb(loginwin, close_kb_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * lb1 = lv_label_create(loginwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb1, "欢迎来到英语聊天室");
    // 创建两个输入框：输入用户名和密码
    ta1 = lv_textarea_create(loginwin);
    ta2 = lv_textarea_create(loginwin);
    // 设置输入框坐标，大小
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_pos(ta2, 120, 250);
    lv_obj_set_size(ta1, 560, 80);
    lv_obj_set_size(ta2, 560, 80);
    // 让输入框使用中文样式
    lv_obj_add_style(ta1,mystyle,0);
    lv_obj_add_style(ta2,mystyle,0);
    // 设置文本输入默认提示文字
    lv_textarea_set_placeholder_text(ta1, "请输入用户名");
    lv_textarea_set_placeholder_text(ta2, "请输入密码");
    lv_textarea_set_password_mode(ta2, true);
    lv_textarea_set_one_line(ta1, true);
    lv_textarea_set_one_line(ta2, true);
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    // 按钮就会在开发板液晶屏上显示
    lv_obj_t * bt1 = lv_btn_create(loginwin); 
    lv_obj_t * bt2 = lv_btn_create(loginwin);

    // 修改按钮的坐标位置
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_pos(bt2, 600, 340);

    // 修改按钮大小
    lv_obj_set_size(bt1, 100, 50);
    lv_obj_set_size(bt2, 100, 50);

    // 新建标签，把按钮作为父窗口
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_t * lb3 = lv_label_create(bt2);

    // 标签设置中文样式
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);

    // 给标签设置文字
    lv_label_set_text(lb2, "登录");
    lv_label_set_text(lb3, "注册");
    lv_obj_center(lb2);
    lv_obj_center(lb3);
     // 给按钮添加事件响应函数:  LV_EVENT_CLICKED枚举值表示按钮点击事件
    lv_obj_add_event_cb(bt1, log_go_main, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, log_go_sig, LV_EVENT_CLICKED, NULL);

    login_message = lv_label_create(loginwin);
    lv_obj_set_pos(login_message, 210, 405);
    lv_obj_set_size(login_message, 380, 45);
    lv_obj_add_style(login_message,mystyle,0);
    lv_label_set_text(login_message, "");

    //自己主动加载登录界面
    lv_scr_load(loginwin);

}

//创建注册界面
void show_sign()
{
    if(kb1 != NULL)
    {
        lv_keyboard_set_textarea(kb1, NULL);
        lv_obj_add_flag(kb1, LV_OBJ_FLAG_HIDDEN);
    }
    setwin = lv_obj_create(NULL);
    login_message = NULL;
    lv_obj_set_size(setwin, 800, 480);
    lv_obj_add_event_cb(setwin, close_kb_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * lb1 = lv_label_create(setwin);
    lv_obj_set_pos(lb1, 100, 50);
    lv_obj_set_size(lb1, 600, 80);
    // 让标签使用中文样式
    lv_obj_add_style(lb1,mystyle,0);
    // 给标签设置文字
    lv_label_set_text(lb1, "注册界面");
   
    // 创建两个输入框：输入用户名和密码
    ta1 = lv_textarea_create(setwin);
    ta2 = lv_textarea_create(setwin);
    // 设置输入框坐标，大小
    lv_obj_set_pos(ta1, 120, 150);
    lv_obj_set_pos(ta2, 120, 250);
    lv_obj_set_size(ta1, 560, 80);
    lv_obj_set_size(ta2, 560, 80);
    // 让输入框使用中文样式
    lv_obj_add_style(ta1,mystyle,0);
    lv_obj_add_style(ta2,mystyle,0);
    // 设置文本输入默认提示文字
    lv_textarea_set_placeholder_text(ta1, "请输入用户名");
    lv_textarea_set_placeholder_text(ta2, "请输入密码");
    lv_textarea_set_password_mode(ta2, true);
    lv_textarea_set_one_line(ta1, true);
    lv_textarea_set_one_line(ta2, true);
    // 给输入框添加事件响应函数，事件类型--》光标进入事件LV_EVENT_FOCUSED
    lv_obj_add_event_cb(ta1, ta1_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(ta2, ta2_cb, LV_EVENT_FOCUSED, NULL);

    // 按钮就会在开发板液晶屏上显示
    lv_obj_t * bt1 = lv_btn_create(setwin); 
    lv_obj_t * bt2 = lv_btn_create(setwin);

    // 修改按钮的坐标位置
    lv_obj_set_pos(bt1, 100, 340);
    lv_obj_set_pos(bt2, 600, 340);

    // 修改按钮大小
    lv_obj_set_size(bt1, 100, 50);
    lv_obj_set_size(bt2, 100, 50);

    // 新建标签，把按钮作为父窗口
    lv_obj_t * lb2 = lv_label_create(bt1);
    lv_obj_t * lb3 = lv_label_create(bt2);

    // 标签设置中文样式
    lv_obj_add_style(lb2,mystyle,0);
    lv_obj_add_style(lb3,mystyle,0);

    // 给标签设置文字
    lv_label_set_text(lb2, "返回");
    lv_label_set_text(lb3, "注册");
    lv_obj_center(lb2);
    lv_obj_center(lb3);

     // 给按钮添加事件响应函数:  LV_EVENT_CLICKED枚举值表示按钮点击事件
    lv_obj_add_event_cb(bt1, set_back_log, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(bt2, set_go_main, LV_EVENT_CLICKED, NULL);

    login_message = lv_label_create(setwin);
    lv_obj_set_pos(login_message, 210, 405);
    lv_obj_set_size(login_message, 380, 45);
    lv_obj_add_style(login_message,mystyle,0);
    lv_label_set_text(login_message, "");
    //自己主动加载注册界面
    lv_scr_load(setwin);

}
