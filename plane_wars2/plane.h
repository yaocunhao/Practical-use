#ifndef _PLANE_H_
#define _PLANE_H_

#include <stdio.h>
#include <graphics.h>
#include <time.h>

#define  SIZE 800 //背景大小
#define  PSIZE 80 //飞机大小
#define  BSIZE 40 //子弹大小
#define  MAX 10//子弹数目
#define  FMAX 10//敌机数目
typedef struct Member
{
	double x;
	double y;
	bool lable;
}mem;

void Game();
void Load();//图片加载
void DataInit();//数据初始化
void PlaneCtr();//通过输入上下左右来改变飞机的坐标
void Show();//画面呈现
void BullterCre();//按了空格，调用函数，创建子弹
void BullterMove();//子弹移动
void FplaneCre();//敌机创建
void FplaneMove();//敌机移动
void Hit();//消灭敌机




#endif