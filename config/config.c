#include <config.h>
#include <stdio.h>
#include <string.h>

static ItemCfg g_tItemCfgs[ITEMCFG_MAX_NUM]; //模块结构体数组
static int g_iItemCfgCount = 0; // 当前页面的模块数量-也是数组下标
// 解析配置文件
int ParseConfigFile(void)
{
	FILE *fp;      // 打开的文件描述符
	char buf[100]; // 读取数据存放的buffer
	char *p = buf; // 指向buf的指针，便于操作和读取
	
	/* 1. 打开文件-默认配置文件 */
	fp = fopen(CFG_FILE, "r");
	if (!fp)
	{
		printf("can not open cfg file %s\n", CFG_FILE);
		return -1;
	}

	while (fgets(buf, 100, fp))
	{
		/* 2.1 读每一行 */
		buf[99] = '\0';		

		/* 2.2 吃掉开头的空格或TAB */
		p = buf;
		while (*p == ' ' || *p =='\t')
			p++;

		/* 2.3 忽略注释 */
		if (*p == '#')
			continue;

		/* 2.4 处理 */
		g_tItemCfgs[g_iItemCfgCount].command[0] = '\0'; // command默认值
		g_tItemCfgs[g_iItemCfgCount].index = g_iItemCfgCount;
		// 从p字符串中得到默写数据，存入后面的变量中(数组尾部插入数据)
		sscanf(p, "%s %d %s", g_tItemCfgs[g_iItemCfgCount].name, &g_tItemCfgs[g_iItemCfgCount].bCanBeTouched, \
		                      g_tItemCfgs[g_iItemCfgCount].command);
		g_iItemCfgCount++;		
	}
	return 0;
}


// 查询模块数量
int GetItemCfgCount(void)
{
	return g_iItemCfgCount;
}

// 通过索引查询某一个模块-返回指针地址
PItemCfg GetItemCfgByIndex(int index)
{
	if (index < g_iItemCfgCount)
		return &g_tItemCfgs[index];
	else
		return NULL;
}

// 通过名称查询某一个模块
PItemCfg GetItemCfgByName(char *name)
{
	int i;
	for (i = 0; i < g_iItemCfgCount; i++)
	{
		if (strcmp(name, g_tItemCfgs[i].name) == 0)
			return &g_tItemCfgs[i];
	}
	return NULL;
}


