#include"plane.h"

//定义图片变量
IMAGE background;//背景图
IMAGE plane_back;//飞机背景图
IMAGE plane_mask;//飞机掩码图
IMAGE fplane_back;//敌机背景图
IMAGE fplane_mask;//敌机掩码图
IMAGE bullte_back;//子弹背景图
IMAGE bullte_mask;//子弹掩码图

//定义全局变量
mem plane;//飞机结构体
mem bullte[MAX];//子弹结构体数组
mem fplane[FMAX];//敌机结构体

DWORD time_start, time_last; //时间变化量

void Load()//图片加载
{
	loadimage(&background, "background.jpg", SIZE, SIZE);
	loadimage(&plane_back, "plane_back.jpg", PSIZE, PSIZE);
	loadimage(&plane_mask, "plane_mask.jpg", PSIZE, PSIZE);
	loadimage(&fplane_back, "fplane_back.jpg", PSIZE, PSIZE);
	loadimage(&fplane_mask, "fplane_mask.jpg", PSIZE, PSIZE);
	loadimage(&bullte_back, "bullte_back.jpg", BSIZE, BSIZE);
	loadimage(&bullte_mask, "bullte_mask.jpg", BSIZE, BSIZE);
}
void DataInit()//数据初始化
{
	//飞机的数据初始化
	plane.x = SIZE / 2 - PSIZE / 2; //飞机起始位置在背景图的中央
	plane.y = SIZE - PSIZE;
	plane.lable = true; //飞机出生是存活的

	//子弹初始化
	for (int i = 0; i < MAX; i++)
	{
		bullte[i].lable = false;//子弹没按空间前是不存在的
	}

	time_start = time_last = GetTickCount();//开机到现在的毫秒数

	//敌机初始化
	for (int i = 0; i>FMAX; i++)
	{
		fplane[i].lable = false;
	}

}

void PlaneCtr()//通过输入上下左右来改变飞机的坐标
{
	if (GetAsyncKeyState(VK_UP))//向上移动
	{
		if (plane.y>0)//不能越界
		plane.y-=0.8;
	}
	if (GetAsyncKeyState(VK_DOWN))//向下移动
	{
		if (plane.y<SIZE-PSIZE)//y最大值600，飞机图片大小为80
		plane.y+=0.8;
	}
	if (GetAsyncKeyState(VK_LEFT))//向左移动
	{
		if (plane.x>-PSIZE/2)//可以缩入墙壁一半机体
		plane.x-=0.8;
	}
	if (GetAsyncKeyState(VK_RIGHT))//向右移动
	{
		if (plane.x<SIZE-PSIZE/2)
		plane.x+=0.8;
	}
	if (GetAsyncKeyState(VK_SPACE) &&time_last - time_start>80)//发射子弹，每颗子弹间接为80毫秒
	{
		BullterCre();//空格发射子弹
		time_start = time_last;
	}
	time_last = GetTickCount();
}

void Show()//画面呈现
{	
	BeginBatchDraw();//双缓冲绘图，防闪烁

	putimage(0, 0, &background);
	if (plane.lable)//飞机存活就打印飞机
	{
		putimage((int)plane.x, (int)plane.y, &plane_mask, SRCAND);
		putimage((int)plane.x, (int)plane.y, &plane_back, SRCPAINT);
	}
	for (int i = 0; i < MAX; i++)//循环判断子弹
	{
		if (bullte[i].lable)//子弹存在就打印子弹
		{
			putimage((int)bullte[i].x, (int)bullte[i].y, &bullte_mask, SRCAND);
			putimage((int)bullte[i].x, (int)bullte[i].y, &bullte_back, SRCPAINT);
		}
	}
	for (int i = 0; i < FMAX; i++)
	{
		if (fplane[i].lable)//打印敌机
		{
			putimage((int)fplane[i].x, (int)fplane[i].y, &fplane_mask, SRCAND);
			putimage((int)fplane[i].x, (int)fplane[i].y, &fplane_back, SRCPAINT);
		}
	}

	EndBatchDraw();
}

void BullterCre()//按了空格，调用函数，创建子弹
{
	for (int i = 0; i < MAX; i++)
	{
		if (!bullte[i].lable)//子弹不存在就创建子弹
		{
			//从飞机的正前方发出去
			bullte[i].x = plane.x + PSIZE / 2 - BSIZE / 2;
			bullte[i].y = plane.y - BSIZE;
			bullte[i].lable = true;
			break;//一次空格产生一颗子弹
		}
	}
}
void BullterMove()//子弹移动
{
	for (int i = 0; i < MAX; i++)
	{
		if (bullte[i].lable)//子弹存在，它的y坐标就减少
		{
			bullte[i].y--;//坐标减，产生移动

			if (bullte[i].y < 0)//子弹回收，否则下次全是发射出去的子弹
			{
				bullte[i].lable = false;
			}
		}
	}
}
void FplaneCre()//敌机创建
{
	for (int i = 0; i < FMAX; i++)
	{
		if (!fplane[i].lable&&time_last-time_start>100)//没有敌机就进行构建，用时间函数控制产生速度
		{
			fplane[i].lable = true;
			fplane[i].x = rand() % SIZE - PSIZE;//不让敌出现在窗口外
			fplane[i].y = 0;
			time_start = time_last;
			break;//每次一个
		}
		time_last = GetTickCount();//获取新的时间
	}
}
void FplaneMove()//敌机移动
{
	for (int i = 0; i < FMAX; i++)
	{
		if (fplane[i].lable)
		{
			fplane[i].y+=0.3;//敌机往下走

			if (fplane[i].y>SIZE)//敌机回收
				fplane[i].lable = false;
		}
	}
}
void Hit()//消灭敌机
{
	for (int i = 0; i < FMAX; i++)
	{
		if (!fplane[i].lable)//不存在则换下一个判断
			continue;

		for (int j = 0; j < MAX; j++)
		{
			if (!bullte[j].lable)
				continue;//子弹不存在也换下一个判断

			if (bullte[j].x>fplane[i].x&&  //子弹和敌机有交集则都消失
				bullte[j].x<fplane[i].x + PSIZE
				&&bullte[j].y>fplane[i].y&&
				bullte[j].y <fplane[i].y + PSIZE) 
			{
				bullte[j].lable = false;
				fplane[i].lable = false;
			}
		}
	}
}


void Game()
{
	//加载图片数和初始化数据
	Load();
	DataInit();
	srand((unsigned)time(NULL));//种下随机数种子

	while (1)
	{
		Show();//图像显示
		PlaneCtr();//操控
		BullterMove();//子弹移动
		FplaneCre();//敌机创建		
		Hit();//判断是否打中敌机
		FplaneMove();//敌机往下走
	}





}



