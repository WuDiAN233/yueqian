#include "../../lv_examples.h"
#if LV_USE_IMG && LV_BUILD_EXAMPLES


void lv_example_img_1(void)
{
	//声明图片数组对应的.C文件中的数据来源
    LV_IMG_DECLARE(lv_img_peng);
    lv_obj_t * img1 = lv_img_create(lv_scr_act());
    lv_img_set_src(img1, &lv_img_peng);
	//设置图片居中
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_size(img1, 100, 80);
}

#endif
