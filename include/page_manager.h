#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

/* 页面形式结构体*/
typedef struct PageAction {
	char *name;			// 页面名称
	void (*Run)(void *pParams); // 页面运行函数
	struct PageAction *ptNext;  // 页面管理链表
}PageAction, *PPageAction;

// 注册页面操作和注册方法
void PageRegister(PPageAction ptPageAction);
void PagesRegister(void);
// 获取页面
PPageAction Page(char *name);

#endif




