
#ifndef _CONFIG_H
#define _CONFIG_H

#include <common.h>

#define ITEMCFG_MAX_NUM 30  // 模块数组大小
#define CFG_FILE  "/etc/test_gui/gui.conf" // 默认配置文件

/* 描述配置文件中每行(一个模块)的结构体*/
typedef struct ItemCfg {
	int index;	// 哪一项的索引
	char name[100]; // 名字
	int bCanBeTouched; // 是否能被点击
	char command[100]; // 测试不通过后的操作命令
}ItemCfg, *PItemCfg;

// 加载解析配置文件
int ParseConfigFile(void);
// 统计有多少行(多少个按钮)
int GetItemCfgCount(void);
// 查询某一项
PItemCfg GetItemCfgByIndex(int index);
PItemCfg GetItemCfgByName(char *name);


#endif

