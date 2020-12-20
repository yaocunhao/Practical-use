#include "plane.h"

char str[ROW][LINE]={ // 0代表边框(■)，1代表游戏区域(空格),2代表飞机(Ж)
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void Showmap(char str[][LINE])//地图展示
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			if (str[i][j] == 0)// 0代表边框(■)
				printf("■");
			else if (str[i][j] == 1)
				printf("  ");//1代表游戏区域(空格)
			else if (str[i][j] == 2)//2代表飞机(Ж)
				printf("Ж");
			else if (str[i][j] == 3)//3表示子弹●
				printf("●");
			else if (str[i][j] == 4)//4表示地方飞机Ψ
				printf("Ψ");
			else if (str[i][j] == 5)//5表示敌机子弹
				printf("¤");
		}
		printf("\n");
	}
}

void Move(char str[][ROW],int row,int line)//进行移动
{
	int count = 0;//统计步数生成敌机
	int newrow = row;
	int newline = line;
	while (1)
	{
		int quit = 0;
		while (!quit)
		{
			printf("w s a d 控制上下左右\n");
			int ch = 0;
			int move = 0;//接收移动命令
			move = getchar();
			while ((ch = getchar()) != '\n');//吃掉空格等多余的符号
			switch (move)
			{
			case 'w'://向上
				newrow--;
				quit = 1;//成功移动一次就退出，打印地图形成动态效果
				break;
			case 's'://下
				newrow++;
				quit = 1;
				break;
			case 'a'://左
				newline--;
				quit = 1;
				break;
			case 'd'://右
				newline++;
				quit = 1;
				break;
			default:
				printf("输入有误，请从新输入\n");
				break;
			}
		}

		if (str[newrow][newline] != 0)//不为墙壁,更新数据
		{
			int judge=Judge(str, newrow, newline);//进行判定
			if (judge)//判定被敌机子弹打中
				break;
			str[newrow][newline] = 2;
			str[row][line] = 1;
			row = newrow;
			line = newline;
		}
		else//否则数据交换回来
		{
			newrow = row;
			newline = line;
		}
		int size = Buttle(str, row, line);//生成子弹，返回size是给消除子弹函数
		FadeButtle(str, row, line, size);//消除子弹
		if (count % 2 == 0)//每移动四次生成一辆敌机
		{
			Plan(str);
		}
		count++;
	}
}
int Buttle(char str[][LINE],int row,int line)//子弹
{
	int size =1;//从1开始是要判断子弹前是否是墙壁
	while ((size != 6) && str[row - size][line]!=0)//飞机前面呈现5颗子弹,子弹不能碰墙
	{
		str[row-size][line]=3;//3表示子弹
		size++;
		system("cls");
		Showmap(str);//动态子弹
	}
	return size;
}
void FadeButtle(char str[][LINE], int row, int line,int size)
{
	size--;//生成子弹时，这里多加了一个
	while (size)
	{
		str[row - size][line] = 1;//消除子弹，即将子弹处变成空白
		size--;
	}
}
void Plan(char str[][LINE])//敌机生成
{
	srand((unsigned)time(NULL));//种下随机数种子
	//敌机坐标
	int foerow = 0;
	int foeline = 0;
	while (1)
	{
		foerow = (rand() %(ROW-2)) + 1;//+1是坐标不能为墙壁
		foeline = (rand() % (LINE-2)) + 1;
		if (str[foerow][foeline] != 2 && str[foerow][foeline] != 3)//随机出现的敌机不能一出来就与我方飞机碰撞
		{
			str[foerow][foeline] = 4;//地方飞机数据用4，图案用Ψ；
			FeoButtle(str, foerow, foeline);
			break;
		}
	}
}
void FeoButtle(char str[][LINE],int foerow,int foeline)//敌机子弹
{
	//敌机出生时子弹不能直接撞到本机，子弹不能到墙内部
	if (str[foerow - 1][foeline] != 0 && str[foerow - 1][foeline] != 2)//上方
		str[foerow - 1][foeline] = 5;
	if (str[foerow + 1][foeline] != 0 && str[foerow + 1][foeline] != 2)//下方
		str[foerow + 1][foeline] = 5;
	if (str[foerow][foeline-1] != 0 && str[foerow][foeline-1] != 2)//左方
		str[foerow][foeline-1] = 5;
	if (str[foerow][foeline + 1] != 0 && str[foerow][foeline + 1] != 2)//右方
		str[foerow][foeline + 1] = 5;
}
int Judge(char str[][LINE],int row,int line)//判断函数
{
	if (str[row][line] == 5)
	{
		printf("你被子弹击中了，游戏结束\n");
		return 1;
	}
	else
		return 0;
}
void game()
{
	int row = 18, line = 8;//飞机初始位置
	Plan(str);//先随机生成一架敌机
	Showmap(str);
	Move(str,row,line);
}