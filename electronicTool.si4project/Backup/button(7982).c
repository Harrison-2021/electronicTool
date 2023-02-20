

#include <ui.h>

// 默认绘制按钮函数
static int DefaultOnDraw(struct Button *ptButton, PDispBuff ptDispBuff)
{
	/* 绘制底色-红色 */
	DrawRegion(&ptButton->tRegion, BUTTON_DEFAULT_COLOR);

	/* 居中写文字 */
	DrawTextInRegionCentral(ptButton->name, &ptButton->tRegion, BUTTON_TEXT_COLOR);

	/* flush to lcd/web */
	FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);

	return 0;
}

// 默认处理按钮点击事件响应函数-每次点击红绿切换
static int DefaultOnPressed(struct Button *ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent)
{
	unsigned int dwColor = BUTTON_DEFAULT_COLOR;
	// 每次点击，状态翻转一次
	ptButton->status = !ptButton->status;
	if (ptButton->status) // 点击后，颜色改变
		dwColor = BUTTON_PRESSED_COLOR;

	/* 绘制底色 */
	DrawRegion(&ptButton->tRegion, dwColor);

	/* 居中写文字 */
	DrawTextInRegionCentral(ptButton->name, &ptButton->tRegion, BUTTON_TEXT_COLOR);

	/* flush to lcd/web */
	FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);
	return 0;
}

// 1.初始化按钮
void InitButton(PButton ptButton, char *name, PRegion ptRegion, ONDRAW_FUNC OnDraw, ONPRESSED_FUNC OnPressed)
{
	ptButton->status = 0;
	ptButton->name = name;
	if(ptRegion) 
		ptButton->tRegion = *ptRegion;
	ptButton->OnDraw    = OnDraw ? OnDraw : DefaultOnDraw; // 没有提供就使用默认
	ptButton->OnPressed = OnPressed ? OnPressed : DefaultOnPressed;
}


