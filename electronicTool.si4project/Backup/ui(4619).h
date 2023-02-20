
#ifndef _UI_H
#define _UI_H

#include <common.h>
#include <disp_manager.h>
#include <input_manager.h>

// 定义按钮不同点击事件的颜色
#define BUTTON_DEFAULT_COLOR 0xff0000 // 默认颜色：红
#define BUTTON_PRESSED_COLOR 0x00ff00 // 按下：绿
#define BUTTON_TEXT_COLOR    0x000000 // 文字颜色：黑

struct Button;

typedef int (*ONDRAW_FUNC)(struct Button *ptButton, PDispBuff ptDispBuff);
typedef int (*ONPRESSED_FUNC)(struct Button *ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent);

// 定义按钮的数据类型
typedef struct Button {
	char *name;		
	int status;
	Region tRegion;			 // 区域位置
	ONDRAW_FUNC OnDraw;		 // 绘制按钮-在映射buffer中
	ONPRESSED_FUNC OnPressed;// 按钮点击事件响应-第三个参数传递输入触屏事件
}Button, *PButton;

// 初始化按钮
void InitButton(PButton ptButton, char *name, PRegion ptRegion, ONDRAW_FUNC OnDraw, ONPRESSED_FUNC OnPressed);

#endif

