#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#include <disp_manager.h>
#include <font_manager.h>
#include <ui.h>

int main(int argc, char **argv)
{
	PDispBuff ptBuffer;
	int error;
	Button tButton;
	Region tRegion;

	if (argc != 2)
	{
		printf("Usage: %s <font_size>\n", argv[0]);
		return -1;
	}

	// 1.显示系统初始化
	DisplayInit();

	SelectDefaultDisplay("fb");

	InitDefaultDisplay();

	ptBuffer = GetDisplayBuffer();

	// 2. 初始化字符系统
	FontsRegister();
	
	error = SelectAndInitFont("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}
	// 指定默认显示位置
	tRegion.iLeftUpX = 200;
	tRegion.iLeftUpY = 200;
	tRegion.iWidth   = 300;
	tRegion.iHeigh   = 100;

	// 3. 初始化按钮UI系统
	InitButton(&tButton, "test", &tRegion, NULL, NULL);
	tButton.OnDraw(&tButton, ptBuffer); // 绘制默认按钮
	while (1)
	{	// 点击按钮事件-默认处理逻辑
		tButton.OnPressed(&tButton, ptBuffer, NULL);
		sleep(2);
	}
	
	return 0;	
}


