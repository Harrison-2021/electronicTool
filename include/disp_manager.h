#ifndef _DISP_MANAGER_H
#define _DISP_MANAGER_H

// 对NULL的定义
#ifndef NULL
#define NULL (void *)0
#endif

// 表示区域的结构体
typedef struct Region { 
	int iLeftUpX;
	int iLeftUpY;
	int iWidth;
	int iHeigh;
}Region, *PRegion;// 结构体用指针表示

// 定义映射内存的buffer结构体
typedef struct DispBuff {
	int iXres; // x像素
	int iYres;
	int iBpp;  // bpp是32位还是16位
	char *buff;// 缓冲区地址
}DispBuff, *PDispBuff;


// 显示设备模块结构体
typedef struct DispOpr { 
	char *name;
	int (*DeviceInit)(void); // 初始化设备-函数指针
	int (*DeviceExit)(void); // 退出设备
	int (*GetBuffer)(PDispBuff ptDispBuff);// 获取buffer信息：分辨率，每个像素使用多少位
	int (*FlushRegion)(PRegion ptRegion,   PDispBuff ptDispBuff);// 刷新区域，数据在buffer中
	struct DispOpr *ptNext; // 定义链表，将多个输出方式的结构体链接起来
}DispOpr, *PDispOpr;

// 初始化中间管理层
void DisplayInit(void);
// 注册模块
void RegisterDisplay(PDispOpr ptDispOpr);
// 选择模块
int SelectDefaultDisplay(char *name);
// 初始化选择的模块，获得映射的内存信息
int InitDefaultDisplay(void);
// 绘制像素的基本操作函数，中间层提供，用户层可以调用使用
int PutPixel(int x, int y, unsigned int dwColor);
// 将底层模块的buffer内容刷新到显示设备上
int FlushDisplayRegion(PRegion ptRegion, PDispBuff ptDispBuff);
// 获取底层映射的的内存地址
PDispBuff GetDisplayBuffer(void);


#endif




