#include <input_manager.h>
#include <tslib.h>
#include <stdio.h>

static struct tsdev *g_ts; // 接收输入设备初始化后的数据

// 1.初始化输入设备文件
static int TouchscreenDeviceInit(void)
{
	// 打开设备文件，并配置，阻塞方式
	g_ts = ts_setup(NULL, 0);
	if (!g_ts)
	{
		printf("ts_setup err\n");
		return -1;
	}

	return 0;
}


// 2. 获取输入事件的数据
static int TouchscreenGetInputEvent(PInputEvent ptInputEvent)
{
	struct ts_sample samp; 
	int ret;

	// 读取设备数据，保存到结构体中，一个触摸点的采样数:1
	ret = ts_read(g_ts, &samp, 1);
	
	if (ret != 1)
		return -1;

	// 将ts_sample转为自定义的PInputEvent输入事件的结构体
	ptInputEvent->iType     = INPUT_TYPE_TOUCH; // 事件类型:触摸屏事件-头文件中定义的宏
	ptInputEvent->iX        = samp.x;
	ptInputEvent->iY        = samp.y;
	ptInputEvent->iPressure = samp.pressure;
	ptInputEvent->tTime     = samp.tv;

	return 0;
}

// 3.关闭设备
static int TouchscreenDeviceExit(void)
{
	ts_close(g_ts);
	return 0;
}

// 全局变量：触摸屏点击事件结构体的实现
static InputDevice g_tTouchscreenDev ={
	.name = "touchscreen",
	.GetInputEvent  = TouchscreenGetInputEvent,
	.DeviceInit     = TouchscreenDeviceInit,
	.DeviceExit     = TouchscreenDeviceExit,
};

// 向中间层注册
void TouchscreenRegister(void)
{
	RegisterInputDevice(&g_tTouchscreenDev);
}

// 代码测试
#if 0

int main(int argc, char **argv)
{
	InputEvent event; // 传入输入事件结构体：结构体访问属性用.,指针访问用->
	int ret;
	
	g_tTouchscreenDev.DeviceInit(); // 初始化输入设备

	while (1)
	{
		ret = g_tTouchscreenDev.GetInputEvent(&event); // 获取输入设备的输入事件中的数据
		if (ret) {
			printf("GetInputEvent err!\n");
			return -1;
		}
		else
		{
			printf("Type      : %d\n", event.iType);
			printf("iX        : %d\n", event.iX);
			printf("iY        : %d\n", event.iY);
			printf("iPressure : %d\n", event.iPressure);
		}
	}
	return 0;
}

#endif


