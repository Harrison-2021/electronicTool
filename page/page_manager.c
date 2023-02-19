
#include <common.h>
#include <page_manager.h>
#include <string.h>

static PPageAction g_ptPages = NULL;

// 1. 注册页面
void PagesRegister(void)
{
	extern void MainPageRegister(void);
	MainPageRegister();
}
// 注册方法
void PageRegister(PPageAction ptPageAction)
{
	ptPageAction->ptNext = g_ptPages;
	g_ptPages = ptPageAction;
}

// 寻找并获得一个页面
PPageAction Page(char *name)
{
	PPageAction ptTmp = g_ptPages;

	while (ptTmp)
	{
		if (strcmp(name, ptTmp->name) == 0)
			return ptTmp;
		ptTmp = ptTmp->ptNext;
	}

	return NULL;
}



