#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#include <input_manager.h>

static PInputDevice g_InputDevs  = NULL; // 链表头

static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER; // 初始化互斥量
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER; // 初始化条件变量


/* start of 实现环形buffer */
#define BUFFER_LEN 20	// 缓冲区大小
static int g_iRead  = 0;// 读指针
static int g_iWrite = 0;// 写指针
static InputEvent g_atInputEvents[BUFFER_LEN]; // buffer数组类型是输入事件结构体

// 判断buffer是否依据满了
static int isInputBufferFull(void)
{
	return (g_iRead == ((g_iWrite + 1) % BUFFER_LEN));
}

// 判断buffere是否为空
static int isInputBufferEmpty(void)
{
	return (g_iRead == g_iWrite);
}

// 提供向buffer中放入数据的接口
static void PutInputEventToBuffer(PInputEvent ptInputEvent)
{
	if (!isInputBufferFull()) // 没有满才放数据，满了的话，丢掉 这个数据
	{
		g_atInputEvents[g_iWrite] = *ptInputEvent; // 写指针指向的地址放入数据
		g_iWrite = (g_iWrite + 1) % BUFFER_LEN;	// 更新写指针
	}
}

// 提供获得缓冲区数据的接口-能获取数据返回1，否则返回0
static int GetInputEventFromBuffer(PInputEvent ptInputEvent)
{
	if (!isInputBufferEmpty())
	{
		*ptInputEvent = g_atInputEvents[g_iRead]; // 读
		g_iRead = (g_iRead + 1) % BUFFER_LEN;	 // 位置更新
		return 1;
	}
	else
	{
		return 0;
	}
}


/* end of 实现环形buffer */


/* 1. 初始化中间管理层-底层的输入设备进行注册 */
void InputInit(void)
{
	/* regiseter touchscreen */
	extern void TouchscreenRegister(void);
	TouchscreenRegister();

	/* regiseter netinput */
	extern void NetInputRegister(void);
	NetInputRegister();
}

// 2. 注册方法：加入链表
void RegisterInputDevice(PInputDevice ptInputDev)
{
	ptInputDev->ptNext = g_InputDevs;
	g_InputDevs = ptInputDev;
}

// 创建的线程要执行的操作函数-在线程中读取输入设备输入的数据
static void *input_recv_thread_func (void *data)
{
	PInputDevice ptInputDev = (PInputDevice)data; // 类型恢复为设备结构体
	InputEvent tEvent;
	int ret;
	
	while (1)
	{
		/* 读数据 */
		ret = ptInputDev->GetInputEvent(&tEvent);

		if (!ret)
		{	
			/* 保存数据 */
			pthread_mutex_lock(&g_tMutex); // 加锁
			PutInputEventToBuffer(&tEvent);// 向环形buffer中添加数据

			/* 唤醒等待数据的线程 */
			pthread_cond_signal(&g_tConVar); /* 通知注册的管理层，唤醒等待这把锁的线程 */
			pthread_mutex_unlock(&g_tMutex);// 解锁
		}
	}

	return NULL;
}

// 3. 初始化每个输入设备，并为每个设备创建单独线程（不是选择一种，而是都要管理）
void IntpuDeviceInit(void)
{
	int ret;
	pthread_t tid;
	
	/* for each inputdevice, init, pthread_create */
	PInputDevice ptTmp = g_InputDevs;
	while (ptTmp)
	{
		/* 初始化设备信息 */
		ret = ptTmp->DeviceInit();

		/* 初始化设备成功后，为当前设备创建单独线程 */
		if (!ret)
		{
			// 第四个参数，将当前输入设备作为参数传入线程处理函数中
			ret = pthread_create(&tid, NULL, input_recv_thread_func, ptTmp);
		}

		ptTmp= ptTmp->ptNext;
	}
}


// 4. 获取输入设备输入的数据-供上层代码调用
int GetInputEvent(PInputEvent ptInputEvent)
{
	InputEvent tEvent;
	int ret;
	pthread_mutex_lock(&g_tMutex); // 访问共享资源，要先加锁
	/* 有数据就从环形buffer中获取数据，放到输入事件结构体中 */
	if (GetInputEventFromBuffer(&tEvent))
	{
		*ptInputEvent = tEvent;
		pthread_mutex_unlock(&g_tMutex);
		return 0;
	}
	else
	{
	/* 无数据就休眠等待，直到被输入设备输入数据事件唤醒 */
		pthread_cond_wait(&g_tConVar, &g_tMutex);	
		// 监听的输入设备有事件时，会唤醒此线程去读取数据
		if (GetInputEventFromBuffer(&tEvent))
		{
			*ptInputEvent = tEvent;
			ret = 0;
		}
		else
		{
			ret = -1;
		}
		pthread_mutex_unlock(&g_tMutex);		
	}
	return ret;

}




