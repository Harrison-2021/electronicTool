#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <sys/time.h>

// 对NULL的定义
#ifndef NULL
#define NULL (void *)0
#endif

#define INPUT_TYPE_TOUCH 1 // 输入类型宏：点击事件
#define INPUT_TYPE_NET   2 // 输入类型宏：网络传输事件

// 1.输入事件定义
typedef struct InputEvent {
	struct timeval	tTime; 
	int iType;	// 输入方式类似
	// 点击输入事件属性：坐标位置，压力值
	int iX;
	int iY;
	int iPressure;
	// 网络传输输入事件属性，字符串内容
	char str[1024];
}InputEvent, *PInputEvent;

// 2.输入设备定义
typedef struct InputDevice {
	char *name;
	int (*GetInputEvent)(PInputEvent ptInputEvent); // 获取输入数据，数据保存在输入事件结构体指针中
	int (*DeviceInit)(void);	// 初始化设备文件
	int (*DeviceExit)(void);	// 退出设备文件
	struct InputDevice *ptNext; // 管理输入设备的链表
}InputDevice, *PInputDevice;

// 初始化中间管理层-注册各个底层输入设备
void InputInit(void);
// 注册方法
void RegisterInputDevice(PInputDevice ptInputDev);
// 初始化底层输入设备
void IntpuDeviceInit(void);
// 获取输入设备输入的数据
int GetInputEvent(PInputEvent ptInputEvent);


#endif




