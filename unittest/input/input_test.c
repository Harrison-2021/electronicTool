#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <input_manager.h>

int main(int argc, char **argv)
{
	int ret;
	InputEvent event;
	
	InputInit(); // 初始化中间层-注册所有输入设备
	IntpuDeviceInit(); // 初始化所有设备，并为所有设备创建单独线程，
					   //线程中循环接收输入数据，并将数据放入缓冲区中

	while (1)
	{
		// 调试程序：打印哪个文件，哪个函数，哪一行
		//printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__); 
		// app调用获取输入事件数据接口
		ret = GetInputEvent(&event);

		//printf("%s %s %d, ret = %d\n", __FILE__, __FUNCTION__, __LINE__, ret);
		if (ret) {
			printf("GetInputEvent err!\n");
			return -1;
		}
		else
		{
			//printf("%s %s %d, event.iType = %d\n", __FILE__, __FUNCTION__, __LINE__, event.iType );
			// 根据数据类型判断：如果是触摸屏，打印触摸屏相关数据
			if (event.iType == INPUT_TYPE_TOUCH)
			{
				printf("Type      : %d\n", event.iType);
				printf("iX        : %d\n", event.iX);
				printf("iY        : %d\n", event.iY);
				printf("iPressure : %d\n", event.iPressure);
			}
			// 根据数据类型判断：如果是网络传输，打印网络传输相关数据
			else if (event.iType == INPUT_TYPE_NET)
			{
				printf("Type      : %d\n", event.iType);
				printf("str       : %s\n", event.str);
			}
		}
	}
	return 0;	
}


