#include<stdio.h>
#include <string.h>
#include "disp_manager.h"

/* 管理底层的LCD、WEB */
static PDispOpr g_DispDevs = NULL; // 显示设备管理结构体-链表头
static PDispOpr g_DispDefault = NULL; // 选择的模块
static DispBuff g_tDispBuff; // buffer结构体
static int line_width; // 每一行的宽度
static int pixel_width;// 每个像素的宽度（字节）

// 1.中间层初始化时，调用底层的注册函数，将底层的结构体注册
void DisplayInit(void)
{
	extern void FramebufferInit(void); // 要先定义函数，再使用
	FramebufferInit(); // FramebufferLCD显示方式初始化并注册
}

// 注册方法-将结构体放入链表中
// 每次在链表头插入一个结构体
void RegisterDisplay(PDispOpr ptDispOpr) // 传入结构体指针
{
	ptDispOpr->ptNext = g_DispDevs;  // 底层结构体的next指向链表头
	g_DispDevs = ptDispOpr;          // 将底层的结构体设定为链表头
}


// 2.选择函数，选择使用哪个模块
int SelectDefaultDisplay(char *name)
{
	// 遍历链表，进行寻找
	PDispOpr pTmp = g_DispDevs;
	while (pTmp) 
	{
		if (strcmp(name, pTmp->name) == 0)
		{
			g_DispDefault = pTmp; // 找到了
			return 0;
		}

		pTmp = pTmp->ptNext;
	}

	return -1;
}

// 3.初始化选择的模块，获得映射的内存信息
int InitDefaultDisplay(void)
{
	int ret;
	
	ret = g_DispDefault->DeviceInit(); // 调用底层模块的设备初始化函数
	if (ret)
	{
		printf("DeviceInit err\n");
		return -1;
	}

	
	ret = g_DispDefault->GetBuffer(&g_tDispBuff); // 获取映射的起始地址
	if (ret)
	{
		printf("GetBuffer err\n");
		return -1;
	}

	line_width  = g_tDispBuff.iXres * g_tDispBuff.iBpp/8;
	pixel_width = g_tDispBuff.iBpp/8;

	return 0;
}

// 获取底层映射的的内存地址
PDispBuff GetDisplayBuffer(void)
{
	return &g_tDispBuff;
}


// 4.绘制像素的基本操作函数，中间层提供，用户层可以调用使用
int PutPixel(int x, int y, unsigned int dwColor) // 描点的位置，颜色，默认是32位RGB888格式
{
	// 计算像素对应的framebuffer地址
	unsigned char *pen_8 =(unsigned char *)(g_tDispBuff.buff+y*line_width+x*pixel_width);
	unsigned short *pen_16;	
	unsigned int *pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	// 当 LCD 是 16bpp 时，要把 color 变量中的 R、 G、 B 抽出来再合并成 RGB565 格式
	switch (g_tDispBuff.iBpp)
	{
		case 8:
		{
			*pen_8 = dwColor;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (dwColor >> 16) & 0xff;
			green = (dwColor >> 8) & 0xff;
			blue  = (dwColor >> 0) & 0xff;
			dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = dwColor;
			break;
		}
		case 32:
		{
			*pen_32 = dwColor;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", g_tDispBuff.iBpp);
			return -1;
			break;
		}
	}
	return 0;
}


// 5.将底层模块的buffer内容刷新到显示设备上
int FlushDisplayRegion(PRegion ptRegion, PDispBuff ptDispBuff)
{
	return g_DispDefault->FlushRegion(ptRegion, ptDispBuff);
}

// 6.绘制位图
void DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwColor)
{
    int i, j, p, q;
	int x = ptFontBitMap->tRegion.iLeftUpX;
	int y = ptFontBitMap->tRegion.iLeftUpY;
    int x_max = x + ptFontBitMap->tRegion.iWidth;
    int y_max = y + ptFontBitMap->tRegion.iHeigh;
	int width = ptFontBitMap->tRegion.iWidth;
	unsigned char *buffer = ptFontBitMap->pucBuffer;

    //printf("x = %d, y = %d\n", x, y);

    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
        	//超过了显示设备的分辨率，就不处理
            if ( i < 0      || j < 0       ||
                i >= g_tDispBuff.iXres || j >= g_tDispBuff.iYres )
            continue;

            //image[j][i] |= bitmap->buffer[q * bitmap->width + p];
            if (buffer[q * width + p]) // buffer内存中有值，就去绘制颜色，没有值不管
	            PutPixel(i, j, dwColor);
        }
    }
	
}


