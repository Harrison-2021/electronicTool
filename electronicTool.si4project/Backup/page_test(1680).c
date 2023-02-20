#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#include <page_manager.h>

int main(int argc, char **argv)
{
	PagesRegister(); // 注册
	Page("main")->Run(NULL); // 运行主页面
	return 0;	
}


