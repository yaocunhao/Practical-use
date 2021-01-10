#include"plane.h"

//定义图片变量
IMAGE background;//背景图
IMAGE plane_back;//飞机背景图
IMAGE plane_mask;//飞机掩码图
IMAGE fplane_back;//敌机背景图
IMAGE fplane_mask;//敌机掩码图
IMAGE bullte_back;//子弹背景图
IMAGE bullte_mask;//子弹掩码图
IMAGE fdie_back;//击中敌机效果背景图
IMAGE fdie_mask;//击中敌机效果掩码图
IMAGE pdie_back;//被击中背景图
IMAGE pdie_mask;//被击中掩码图
IMAGE god_plane;//飞机复活时呈现状态

//定义全局变量
mem plane;//飞机结构体
mem bullte[MAX];//子弹结构体数组
mem fplane[FMAX];//敌机结构体
mem fdie[FMAX];//击中敌机


DWORD b_time_start, b_time_last; //子弹时间变化量
DWORD pf_time_start, pf_time_last; //敌机产生时间变化量
DWORD p_time_start, p_time_last; //被击中无敌时间变化量

int flag;//本机生命值
int boom;//爆炸标志
int count;//得分计数器


void Load()//图片加载
{
	loadimage(&background, "background.jpg", SIZE, SIZE);
	loadimage(&plane_back, "plane_back.jpg", PSIZE, PSIZE);
	loadimage(&plane_mask, "plane_mask.jpg", PSIZE, PSIZE);
	loadimage(&god_plane, "god_plane.jpg", PSIZE, PSIZE);
	loadimage(&fplane_back, "fplane_back.jpg", PSIZE, PSIZE);
	loadimage(&fplane_mask, "fplane_mask.jpg", PSIZE, PSIZE);
	loadimage(&bullte_back, "bullte_back.jpg", BSIZE, BSIZE);
	loadimage(&bullte_mask, "bullte_mask.jpg", BSIZE, BSIZE);
	loadimage(&fdie_back, "fdie_back.jpg", PSIZE, PSIZE);
	loadimage(&fdie_mask, "fdie_mask.jpg", PSIZE, PSIZE);
	loadimage(&pdie_back, "pdie_back.jpg", PSIZE, PSIZE);
	loadimage(&pdie_mask, "pdie_mask.jpg", PSIZE, PSIZE);
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

	//时间变化初始化
	b_time_start = b_time_last = GetTickCount();//开机到现在的毫秒数
	pf_time_start = pf_time_last = GetTickCount();
	p_time_start = p_time_last = GetTickCount();//被击中无敌时间变化量

	//敌机初始化
	for (int i = 0; i<FMAX; i++)
	{
		fplane[i].lable = false;
	}
	//击中敌机效果初始化
	for (int i = 0; i < FMAX; i++)
	{
		fdie[i].lable = false;
	}

	//生命值初始化
	flag = 5;

	//死亡爆炸效果控制
	boom = 0;

	//得分计数器
	count = 0;

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
	if (GetAsyncKeyState(VK_SPACE) && b_time_last - b_time_start>80)//发射子弹，每颗子弹间接为80毫秒
	{
		BullterCre();//空格发射子弹
		b_time_start = b_time_last;
	}
	b_time_last = GetTickCount();
}

void Show()//画面呈现
{	
	BeginBatchDraw();//双缓冲绘图，防闪烁

	putimage(0, 0, &background);
	Score();//分数和生命值显示

	if (plane.lable)//飞机存活就打印飞机
	{
		putimage((int)plane.x, (int)plane.y, &plane_mask, SRCAND);
		putimage((int)plane.x, (int)plane.y, &plane_back, SRCPAINT);
	}
	if (boom)
	{
		putimage((int)plane.x, (int)plane.y, &pdie_mask, SRCAND);
		putimage((int)plane.x, (int)plane.y, &pdie_back, SRCPAINT);
		if (rand() % 80 == 2)
		{
			boom = 0;
			//飞机复活
			plane.x = SIZE / 2 - PSIZE / 2; //飞机起始位置在背景图的中央
			plane.y = SIZE - PSIZE;
		}
	}
	if (plane.lable == false&&boom==0&&flag!=0)
	{
		if (p_time_last - p_time_start>=4000)//无敌时间3秒
		{
			plane.lable = true;
			p_time_start = p_time_last;
		}
		p_time_last = GetTickCount();

		putimage((int)plane.x, (int)plane.y, &plane_mask, SRCAND);
		putimage((int)plane.x, (int)plane.y, &god_plane, SRCPAINT);
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
		if (fdie[i].lable)//打印击中效果
		{
			putimage((int)fdie[i].x, (int)fdie[i].y, &fdie_mask, SRCAND);
			putimage((int)fdie[i].x, (int)fdie[i].y, &fdie_back, SRCPAINT);
		}

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
		if (!fplane[i].lable&&pf_time_last-pf_time_start>100)//没有敌机就进行构建，用时间函数控制产生速度
		{
			fplane[i].lable = true;
			fplane[i].x = rand() % SIZE - PSIZE;//不让敌出现在窗口外
			fplane[i].y = 0;
			pf_time_start = pf_time_last;
			break;//每次一个
		}
		pf_time_last = GetTickCount();//获取新的时间
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
				fdie[i].lable = true;//击中效果
				fdie[i].x = fplane[i].x;
				fdie[i].y = fplane[i].y;
				bullte[j].lable = false;
				fplane[i].lable = false;
				count++;
			}
		}
	}
}



void FdieRetain()//击中后效果保留一段时间
{
	for (int i = 0; i < FMAX; i++)
	{
		if (rand() % 40 == 2)
		{
			fdie[i].lable = false;
		}
	}

}

void PDie()//被敌机击中
{
	for (int i = 0; i < FMAX; i++)
	{
		if (!fplane[i].lable)
			continue;

		if (plane.x>fplane[i].x&&plane.x<fplane[i].x + PSIZE
			&&plane.y>fplane[i].y&&plane.y < fplane[i].y + PSIZE&&plane.lable)
		{
			plane.lable = false;
			flag-=1;//生命值-1

			boom = 1;//输出爆炸图片
			
			break;
		}
	}
}

void Score()//分数显示
{
	char myscore[MAX] = "当前得分为：";
	char num[20];
	sprintf(num, "%d", count);
	settextcolor(WHITE);
	outtextxy(0, 0, myscore);//输出分数
	outtextxy(80, 0, num);

	char _myscore[MAX] = "当前生命值：";
	char _num[20];
	sprintf(_num, "%d", flag);
	settextcolor(WHITE);
	outtextxy(0, 50, _myscore);
	outtextxy(90, 50, _num);//输出剩余生命值

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
		if (flag==0)
			break;
		PlaneCtr();//操控
		BullterMove();//子弹移动
		FplaneCre();//敌机创建		
		Hit();//判断是否打中敌机
		FplaneMove();//敌机往下走
		FdieRetain();//击中敌机后效果保留一段时间
		PDie();//被敌机击中
	}

}



