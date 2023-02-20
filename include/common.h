

#ifndef _COMMON_H
#define _COMMON_H

#ifndef NULL
#define NULL (void *)0
#endif

/* lcd坐标系的区域 */
typedef struct Region {
	int iLeftUpX;
	int iLeftUpY;
	int iWidth;
	int iHeigh;
}Region, *PRegion;

/* 笛卡尔坐标系的区域的定义*/
typedef struct RegionCartesian {
	int iLeftUpX;
	int iLeftUpY;
	int iWidth;
	int iHeigh;
}RegionCartesian, *PRegionCartesian;


#endif

