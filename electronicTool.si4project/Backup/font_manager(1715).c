
#include <font_manager.h>
#include <string.h>

static PFontOpr g_ptFonts = NULL;	// 链表头
static PFontOpr g_ptDefaulFontOpr = NULL; // 选择的字符设备

/* 注册方法*/
void RegisterFont(PFontOpr ptFontOpr)
{
	ptFontOpr->ptNext = g_ptFonts;
	g_ptFonts = ptFontOpr;
}
/* 初始化注册操作*/
void FontsInit(void)
{
	extern void FreetypeRegister(void);
	FreetypeRegister();
}

/* 选择字符设备，并根据aFontFileName格式初始化字符设备*/
int SelectAndInitFont(char *aFontOprName, char *aFontFileName)
{
	PFontOpr ptTmp = g_ptFonts;
	while (ptTmp)
	{
		if (strcmp(ptTmp->name, aFontOprName) == 0)
			break;
		ptTmp = ptTmp->ptNext;
	}

	if (!ptTmp)
		return -1;

	g_ptDefaulFontOpr = ptTmp;
	return ptTmp->FontInit(aFontFileName); // 初始化字符设备
}

/* 设置字体大小*/
int SetFontSize(int iFontSize)
{
	return g_ptDefaulFontOpr->SetFontSize(iFontSize);
}

/* 获取字符设备的位图数据*/
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	return g_ptDefaulFontOpr->GetFontBitMap(dwCode, ptFontBitMap);
}


