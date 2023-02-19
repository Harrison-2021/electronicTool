
#include <page_manager.h>
#include <stdio.h>

// 主页面处理逻辑：打印一段信息
static void MainPageRun(void *pParams)
{
	printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}

// 1.创建全局变量，页面管理结构
static PageAction g_tMainPage = {
	.name = "main",
	.Run  = MainPageRun,
};

// 2. 向页面管理器注册
void MainPageRegister(void)
{
	PageRegister(&g_tMainPage);
}


