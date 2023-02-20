#定义变量
src=$(wildcard ./*/*.c ./unittest/business/*.c) # 获取指定路径下的文件
objs=$(patsubst %.c, %.o, $(src)) # 将获取的文件名.c替换为.o文件
# 开发框架头文件路径。
PUBINCL = -I $(shell pwd)/include
# 链接的其他库
LDFLAGS = -lts -lpthread -lfreetype -lm
# 交叉编译
ARMBUILD = arm-buildroot-linux-gnueabihf-gcc
target=app # 生成的目标文件
$(target):$(objs)
	$(ARMBUILD) $(objs) -o $(target) $(LDFLAGS)

# $< :依赖第一个文件，$@: 目标完整文件名	
%.o:%.c # 通配符：依赖所有的.c文件生成.o文件
	$(ARMBUILD) -c $< -o $@ $(PUBINCL)

.PHONY:clean # 清理文件
clean:
	rm $(objs) -f