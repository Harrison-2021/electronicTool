#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

/* socket
 * connect
 * send/recv
 */

#define SERVER_PORT 8888

int main(int argc, char **argv)
{
	int iSocketClient; // 客户端socket的文件描述符
	struct sockaddr_in tSocketServerAddr; // 服务端的地址信息
	
	int iRet;
	int iSendLen;
	int iAddrLen;

	if (argc != 3)
	{
		printf("Usage:\n");
		printf("%s <server_ip> <str>\n", argv[0]);
		return -1;
	}
	
	// 1. 调用socket函数，获取句柄
	iSocketClient = socket(AF_INET, SOCK_DGRAM, 0);

	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	//tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
 	// 2. 设置服务端地址，将参数传入的ip设定为目标服务端ip地址
 	if (0 == inet_aton(argv[1], &tSocketServerAddr.sin_addr))
 	{
		printf("invalid server_ip\n");
		return -1;
	}
	memset(tSocketServerAddr.sin_zero, 0, 8);

// udp连接，可以不用连接，直接用sendto发送数据
#if 0	
	iRet = connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));	
	if (-1 == iRet)
	{
		printf("connect error!\n");
		return -1;
	}
#endif

	iAddrLen = sizeof(struct sockaddr);
	// 发送的数据是传入的第三个参数，向目标服务端发送数据
	iSendLen = sendto(iSocketClient, argv[2], strlen(argv[2]), 0,
	              (const struct sockaddr *)&tSocketServerAddr, iAddrLen);

	close(iSocketClient);
	
	return 0;
}

