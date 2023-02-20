#include <config.h>
#include <stdio.h>
#include <ui.h>
#include <page_manager.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define X_GAP 5
#define Y_GAP 5

static Button g_tButtons[ITEMCFG_MAX_NUM]; // 按钮数组
static int g_tButtonCnt; // 按钮数量-数组下标

// 处理输入事件
static int MainPageOnPressed(struct Button *ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent)
{
	unsigned int dwColor = BUTTON_DEFAULT_COLOR; // 默认红色
	char name[100];
	char status[100];
	char *strButton; // button中的文字
	char *command_status[3] = {"err", "ok", "percent"};
	int command_status_index = 0;
	char command[1000];
	PItemCfg ptItemCfg;

	strButton = ptButton->name;
	
	/* 1. 对于触摸屏事件 */
	if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
	{
		/* 1.1 分辨能否被点击 */
		if (GetItemCfgByName(ptButton->name)->bCanBeTouched == 0)
			return -1;

		/* 1.2 修改颜色 */
		if(ptInputEvent->iPressure > 0) // 按压状态才改变状态，防止按键抖动问题
			ptButton->status = !ptButton->status;
		if (ptButton->status)
		{
			dwColor = BUTTON_PRESSED_COLOR;
			command_status_index = 1;
		}
			
	}
	else if (ptInputEvent->iType == INPUT_TYPE_NET)
	{
		/* 2. 对于网络事件 */
		
		/* 根据传入的字符串修改颜色 : wifi ok, wifi err, burn 70 */
		sscanf(ptInputEvent->str, "%s %s", name, status); // 截取出是否有ok
		if (strcmp(status, "ok") == 0)
		{
			command_status_index = 1;
			dwColor = BUTTON_PRESSED_COLOR;
		}
		else if (strcmp(status, "err") == 0)
		{
			command_status_index = 0;
			dwColor = BUTTON_DEFAULT_COLOR;
		}
		else if (status[0] >= '0' && status[0] <= '9')
		{			
			command_status_index = 2;
			dwColor = BUTTON_PERCENT_COLOR;
			strButton = status;			
		}
		else
			return -1;
	}
	else
	{
		return -1;
	}

	/* 绘制底色 */
	DrawRegion(&ptButton->tRegion, dwColor);

	/* 居中写文字 */
	DrawTextInRegionCentral(strButton, &ptButton->tRegion, BUTTON_TEXT_COLOR);

	/* flush to lcd/web */
	FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);
	return 0;

	/* 执行command */
	ptItemCfg = GetItemCfgByName(ptButton->name);
	if (ptItemCfg->command[0] != '\0')
	{
		// 打印命令信息
		sprintf(command, "%s %s", ptItemCfg->command, command_status[command_status_index]);
		// 调用命令
		system(command);
	}
	
}

// 判断输入事件的按钮是否在寻找的区域内
static int isTouchPointInRegion(int iX, int iY, PRegion ptRegion)
{
	if (iX < ptRegion->iLeftUpX || iX >= ptRegion->iLeftUpX + ptRegion->iWidth)
		return 0;

	if (iY < ptRegion->iLeftUpY || iY >= ptRegion->iLeftUpY + ptRegion->iHeigh)
		return 0;

	return 1;
}

// 获取button
static PButton GetButtonByName(char *name)
{
	int i;
	
	for (i = 0; i < g_tButtonCnt; i++)
	{
		if (strcmp(name, g_tButtons[i].name) == 0)
			return &g_tButtons[i];
	}

	return NULL;
}

static PButton GetButtonByInputEvent(PInputEvent ptInputEvent)
{
	int i;
	char name[100];
	
	if (ptInputEvent->iType == INPUT_TYPE_TOUCH) // 触屏事件类型
	{
		for (i = 0; i < g_tButtonCnt; i++)
		{	
			if (isTouchPointInRegion(ptInputEvent->iX, ptInputEvent->iY, &g_tButtons[i].tRegion))
				return &g_tButtons[i];
		}
	}
	else if (ptInputEvent->iType == INPUT_TYPE_NET) // 网络事件类型
	{
		sscanf(ptInputEvent->str, "%s", name); // 从输入事件中提取name
		return GetButtonByName(name);
	}
	else
	{
		return NULL;
	}
	return NULL;
}

/* 修改文字大小-自适应按钮大小*/
static int GetFontSizeForAllButton(void)
{
	int i;
	int max_len = -1;
	int max_index = 0;
	int len;
	RegionCartesian tRegionCar;
	float k, kx, ky;
	
	/* 1. 找出name最长的Button */
	for (i = 0; i < g_tButtonCnt; i++)
	{
		len = strlen(g_tButtons[i].name);
		if (len > max_len)
		{
			max_len = len;
			max_index = i;
		}
	}

	/* 2. 以font_size =100, 算出它的外框 */
	SetFontSize(100);
	GetStringRegionCar(g_tButtons[max_index].name, &tRegionCar);

	/* 3. 把文字的外框缩放为Button的外框 */
	kx = (float)g_tButtons[max_index].tRegion.iWidth / tRegionCar.iWidth;
	ky = (float)g_tButtons[max_index].tRegion.iHeigh / tRegionCar.iHeigh;
	//printf("button width / str width   = %d/%d = %f\n", g_tButtons[max_index].tRegion.iWidth, tRegionCar.iWidth, kx);
	//printf("button height / str height = %d/%d = %f\n", g_tButtons[max_index].tRegion.iHeigh, tRegionCar.iHeigh, ky);
	if (kx < ky) // 设置缩放系数
		k = kx;
	else
		k = ky;

	//printf("font size = %d\n", (int)(k*100));
	/* 4. 反算出font size, 只取0.80, 避免文字过于接近边界 */
	return k * 100 * 0.8;
}


// 根据配置文件计算按钮大小，并绘制页面按钮
static void GenerateButtons(void)
{
	int width, height;   // 每个按钮的宽，高
	int n_per_line;		 // 每行多少个按钮
	int row, rows;       // 有多少行按钮
	int col;			 // 列
	int n;				 // 按钮个数
	PDispBuff pDispBuff; // 映射的buffer
	int xres, yres;		 // 像素分辨率
	int start_x, start_y;// 按钮起点位置
	int pre_start_x, pre_start_y;// 前一个按钮起点位置
	PButton pButton;     // 按钮对象
	int i = 0;
	int iFontSize;       // 调整字体大小
	
	/* 1.算出单个按钮的width/height */
	n = GetItemCfgCount(); // 计算有多少个按钮
	g_tButtonCnt = n;
	
	pDispBuff = GetDisplayBuffer(); // 获取分辨率-从buffer内存读取
	xres = pDispBuff->iXres;
	yres = pDispBuff->iYres;
	width = sqrt(1.0/0.618 *xres * yres / n);
	n_per_line = xres / width + 1; // 每行按钮向上取整，调整按钮宽度
	width  = xres / n_per_line; 
	height = 0.618 * width;	

	/* 2.居中显示:  计算起始按钮的region  */
	start_x = (xres - width * n_per_line) / 2;
	rows    = n / n_per_line; 
	if (rows * n_per_line < n) // 向上取整
		rows++;
	
	start_y = (yres - rows*height)/2;

	/* 3.计算每个按钮的region */
	for (row = 0; (row < rows) && (i < n); row++) // 边界总数<n
	{
		pre_start_y = start_y + row * height;
		pre_start_x = start_x - width; // 每行都要从左重新计算
		for (col = 0; (col < n_per_line) && (i < n); col++)
		{
			pButton = &g_tButtons[i];
			pButton->tRegion.iLeftUpX = pre_start_x + width;
			pButton->tRegion.iLeftUpY = pre_start_y;
			pButton->tRegion.iWidth   = width - X_GAP;// 防止连在一起
			pButton->tRegion.iHeigh   = height - Y_GAP;
			pre_start_x = pButton->tRegion.iLeftUpX; // 每次向右更新递进

			/* 初始化button */
			InitButton(pButton, GetItemCfgByIndex(i)->name, NULL, NULL, MainPageOnPressed);
			i++;
		}
	}
	// 根据按钮调整字体大小
	iFontSize = GetFontSizeForAllButton();
	/* 4.自定义绘制button */
	for (i = 0; i < n; i++) 
	{
		g_tButtons[i].iFontSize = iFontSize;
		g_tButtons[i].OnDraw(&g_tButtons[i], pDispBuff);// 使用默认的DefaultOnDraw
	}
		
}


// 主页面处理逻辑
static void MainPageRun(void *pParams)
{
	int error;
	InputEvent tInputEvent;
	PButton ptButton;
	PDispBuff ptDispBuff = GetDisplayBuffer();
	
	/* 读取配置文件-默认位置 */
	error = ParseConfigFile();
	if (error)
		return ;

	/* 根据配置文件生成按钮、界面 */
	GenerateButtons();

	while (1)
	{
		/* 读取输入事件 */
		error = GetInputEvent(&tInputEvent);
		if (error)
			continue;

		/* 根据输入事件找到按钮 */
		ptButton = GetButtonByInputEvent(&tInputEvent);
		if (!ptButton)
			continue;

		/* 调用按钮的OnPressed函数-会跳转到自定义的 MainPageOnPressed */
		ptButton->OnPressed(ptButton, ptDispBuff, &tInputEvent);
	}
}

// 1.创建全局变量，页面管理结构
static PageAction g_tMainPage = {
	.name = "main",
	.Run  = MainPageRun,
};

// 2. 向页面管理器注册
void MainPageRegister(void)
{
	PageRegister(&g_tMainPage);
}


