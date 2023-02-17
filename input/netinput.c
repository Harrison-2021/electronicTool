#include <input_manager.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/* socket
 * bind
 * sendto/recvfrom
 */

#define SERVER_PORT 8888

static int g_iSocketServer; // socket套接字文件描述符

// 2. 获取设备数据（网络数据）
static int NetinputGetInputEvent(PInputEvent ptInputEvent)
{
	struct sockaddr_in tSocketClientAddr;
	int iRecvLen; // recvfrom返回接收数据的字节数
	char aRecvBuf[1000]; // 该缓冲区用来存放 recvfrom 函数接收到的数据
	
	unsigned int iAddrLen = sizeof(struct sockaddr);

	// 获取客户端的数据-udp连接-带上源地址的ip
	iRecvLen = recvfrom(g_iSocketServer, aRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
	if (iRecvLen > 0)
	{
		aRecvBuf[iRecvLen] = '\0';
		//printf("Get Msg From %s : %s\n", inet_ntoa(tSocketClientAddr.sin_addr), ucRecvBuf);
		// 将接收的数据转换成输入事件结构体来保存
		ptInputEvent->iType 	= INPUT_TYPE_NET;
		gettimeofday(&ptInputEvent->tTime, NULL); // 将当前时间放入ptInputEvent的结构体中
		strncpy(ptInputEvent->str, aRecvBuf, 1000);
		ptInputEvent->str[999] = '\0'; // 字符串要有结束符
		return 0;
	}
	else
		return -1;
}

// 1. 初始化设备文件
static int NetinputDeviceInit(void)
{
	struct sockaddr_in tSocketServerAddr; // 储存网络地址的结构体，#include <netinet/in.h>
	int iRet; // bing绑定socket的结果

	// 初始化socket，AF_INET：支持网络通信，SOCK_DGRAM：UDP
	g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == g_iSocketServer)
	{
		printf("socket error!\n");
		return -1;
	}
	
	// 设置网络地址
	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /*本地字节顺序转网络字节顺序 */
 	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY; // 保存的IP地址，本机地址
	memset(tSocketServerAddr.sin_zero, 0, 8);

	// bind绑定一个socket地址
	iRet = bind(g_iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}

	return 0;
}

// 3.关闭设备
static int NetinputDeviceExit(void)
{
	close(g_iSocketServer);	
	return 0;
}


static InputDevice g_tNetinputDev ={
	.name = "touchscreen",
	.GetInputEvent  = NetinputGetInputEvent,
	.DeviceInit     = NetinputDeviceInit,
	.DeviceExit     = NetinputDeviceExit,
};

// 向中间层注册输入设备信息
void NetInputRegister(void)
{
	RegisterInputDevice(&g_tNetinputDev);
}


#if 0

int main(int argc, char **argv)
{
	InputEvent event;
	int ret;

	// 1.初始化网络设备（服务端）
	g_tNetinputDev.DeviceInit();

	while (1)
	{
		// 2.获取从客服端向服务端发送的数据
		ret = g_tNetinputDev.GetInputEvent(&event);
		if (ret) {
			printf("GetInputEvent err!\n");
			return -1;
		}
		else
		{
			printf("Type      : %d\n", event.iType);
			printf("str       : %s\n", event.str);
		}
	}
	return 0;
}

#endif

