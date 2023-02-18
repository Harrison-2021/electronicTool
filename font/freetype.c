#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <sys/ioctl.h>
#include <font_manager.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


static FT_Face g_tFace;				// 字符外形对象
static int g_iDefaultFontSize = 12;	// 默认字体大小

/* 初始化字符设备文件*/
static int FreeTypeFontInit(char *aFineName)
{
    FT_Library    library; // 初始化freetype库，将数据保存在库文件中
    int error;

	// ① 初始化 freetype 库
    error = FT_Init_FreeType( &library );                 /* initialize library */    
	if (error)
	{
		printf("FT_Init_FreeType err\n");
		return -1;
	}

	// ② 加载字体文件， 字体格式为aFineName，保存在&g_tFace 中
    error = FT_New_Face(library, aFineName, 0, &g_tFace ); /* create face object */
	if (error)
	{
		printf("FT_New_Face err\n");
		return -1;
	}

	// ③ 设置字体大小
    FT_Set_Pixel_Sizes(g_tFace, g_iDefaultFontSize, 0);

	return 0;
}

/* 设置字体大小接口*/
static int FreeTypeSetFontSize(int iFontSize)
{
    FT_Set_Pixel_Sizes(g_tFace, iFontSize, 0);
	return 0;
}

/* 通过字符编码值，获取字符的位图*/
static int FreeTypeGetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	int error;
    FT_Vector pen;		// 基点，对应Origin点，单位不同
    //FT_Glyph  glyph;	// 关键点，在slot插槽中保存
    FT_GlyphSlot slot = g_tFace->glyph; // 定义保存关键点的插槽

    pen.x = ptFontBitMap->iCurOriginX * 64; /* 单位: 1/64像素 */
    pen.y = ptFontBitMap->iCurOriginY * 64; /* 单位: 1/64像素 */

	/* 变形&设置原点-第二个参数可以传递变形矩阵，第三个参数设置基点，便于在直到位置显示 */
	FT_Set_Transform(g_tFace, 0, &pen);

	/* 加载位图: 根据dwCode编码值，得到新的关键点和位图，放在slot中 */
	error = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER);
	if (error)
	{
		printf("FT_Load_Char error\n");
		return -1;
	}

	/* 将位图数据保存在自定义的描述文字位图信息结构体中 */
	ptFontBitMap->pucBuffer = slot->bitmap.buffer; // 位图点阵信息

	ptFontBitMap->tRegion.iLeftUpX = slot->bitmap_left;
	// 坐标系转换：将笛卡尔坐标转为LCD坐标
	ptFontBitMap->tRegion.iLeftUpY = ptFontBitMap->iCurOriginY*2 - slot->bitmap_top;
	ptFontBitMap->tRegion.iWidth   = slot->bitmap.width;
	ptFontBitMap->tRegion.iHeigh   = slot->bitmap.rows;
	ptFontBitMap->iNextOriginX = ptFontBitMap->iCurOriginX + slot->advance.x / 64;
	ptFontBitMap->iNextOriginY = ptFontBitMap->iCurOriginY;

	return 0;
}

/* 实现操作字符类*/
static FontOpr g_tFreetypeOpr = {
	.name          = "freetype",
	.FontInit      = FreeTypeFontInit,
	.SetFontSize   = FreeTypeSetFontSize,
	.GetFontBitMap = FreeTypeGetFontBitMap,
};

/* 注册*/
void FreetypeRegister(void)
{
	RegisterFont(&g_tFreetypeOpr);
}

