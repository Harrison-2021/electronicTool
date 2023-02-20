#ifndef _FONT_MANAGER_H
#define _FONT_MANAGER_H

#include <common.h>

/* 用来描述一个文字的位图信息 */
typedef struct FontBitMap {
	// 当前字符的位置区域信息(LCD坐标系)
	Region tRegion;
	// freetype当前字符基点和下个字符基点信息
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char *pucBuffer; // 存有字符的位图数据
}FontBitMap, *PFontBitMap;

/* 用来描述操作文件的操作结构体 */
typedef struct FontOpr {
	char *name;			// 类型名
	int (*FontInit)(char *aFineName); // 字体模块的初始化函数
	int (*SetFontSize)(int iFontSize);// 设置字体大小，不一定能设置，需要有返回值
	int (*GetFontBitMap)(unsigned int dwCode, PFontBitMap ptFontBitMap);// 根据编码值获取字符位图
	struct FontOpr *ptNext; // 管理模块的指针链表
}FontOpr, *PFontOpr;

/* 注册方法和初始化注册函数 */
void RegisterFont(PFontOpr ptFontOpr);

void FontsRegister(void);
/* 选择操作字体的方式 */
int SelectAndInitFont(char *aFontOprName, char *aFontFileName);
/* 设置字体大小 */
int SetFontSize(int iFontSize);
/* 获取文件信息 */
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap);


#endif




