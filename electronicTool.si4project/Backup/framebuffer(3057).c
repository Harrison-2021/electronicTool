#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "disp_manager.h"

static int fd_fb;
static struct fb_var_screeninfo var;	/* Current var */
static int screen_size;
static unsigned char *fb_base;	// 在内存中映射buffer的地址起始位置
static unsigned int line_width;
static unsigned int pixel_width;

// 初始化设备信息函数
static int FbDeviceInit(void)
{
	// 1.打开设备节点，可读可写权限
	fd_fb = open("/dev/fb0", O_RDWR); 
	if (fd_fb < 0)
	{
		printf("can't open /dev/fb0\n");
		return -1;
	}
	// 2.获得设备信息-放进var结构体中
	// 可变的参数 struct fb_var_screeninfo var
	// FBIOGET_VSCREENINFO，它表示 get var screeninfo，获得屏幕的可变信息
	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))  
	{
		printf("can't get var\n");
		return -1;
	}

	// 3.映射 Framebuffer
	line_width  = var.xres * var.bits_per_pixel / 8;
	pixel_width = var.bits_per_pixel / 8;
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8; // 整个framebuffer大小	
	// PROT_READ |PROT_WRITE 表示该区域可读、可写； MAP_SHARED 表示该区域是共享的
	fb_base = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fb_base == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}

	return 0;
}

// 退出，释放资源
static int FbDeviceExit(void)
{
	munmap(fb_base, screen_size);
	close(fd_fb);
	return 0;
}


/* 可以返回LCD的framebuffer, 以后上层APP可以直接操作LCD, 可以不用FbFlushRegion
 * 也可以malloc返回一块无关的buffer, 要使用FbFlushRegion
 */
 // 本例中直接返回LCD的framefuffer
static int FbGetBuffer(PDispBuff ptDispBuff)
{
	ptDispBuff->iXres = var.xres;
	ptDispBuff->iYres = var.yres;
	ptDispBuff->iBpp  = var.bits_per_pixel;
	ptDispBuff->buff  = (char *)fb_base;
	return 0;
}

static int FbFlushRegion(PRegion ptRegion, PDispBuff ptDispBuff)
{
	return 0;
}

// 结构体的实现
static DispOpr g_tFramebufferOpr = {
	.name        = "fb",
	.DeviceInit  = FbDeviceInit,
	.DeviceExit  = FbDeviceExit,  
	.GetBuffer   = FbGetBuffer, // 自定义获取绘画按钮的buffer函数
	.FlushRegion = FbFlushRegion,
};

// 将构造的结构体进行注册，注册到上面的管理层
void FramebufferInit(void)
{
	RegisterDisplay(&g_tFramebufferOpr);
}


