#include "snake.h"

void MapPrint(char(*arr)[LINE])
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			//第一行，最后一行，第一列，最后一列都是墙壁
			if (arr[i][j]==1)
				printf("■");
			else if (arr[i][j] == 2)
				printf("⊙");//蛇头
			else if (arr[i][j] == 3)
				printf("●");//打印食物
			else if (arr[i][j] == 4)
				printf("¤");//打印身体
			else
				printf("  ");//一个墙体占两个空格
		}
		printf("\n");
	}
}
void Move(int *row, int *line,char *move)//蛇头移动函数，通过读取输入的键盘方向键来改变蛇头方向
{
		if (GetAsyncKeyState(VK_UP))//上
		{
			*move = 'w';
		}
		else if (GetAsyncKeyState(VK_DOWN))//下
		{
			*move = 's';
		}
		else if (GetAsyncKeyState(VK_LEFT))//左
		{
			*move = 'a';
		}
		else if (GetAsyncKeyState(VK_RIGHT))//右
		{
			*move = 'd';
		}
		GetCoordinate(row, line, move);//将获得的方向键信息传输过去
}

void GetCoordinate(int *row, int *line, char *move)//蛇头自动移动，通过获取的方向信息，决定蛇头往哪个方向自动移
{
	if (*move == 'w')
		(*row)--;
	else if (*move == 's')
		(*row)++;
	else if (*move == 'a')
		(*line)--;
	else if (*move == 'd')
		(*line)++;
}
void food(char arr[][LINE],int *x,int *y)
{
	int flag = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < ROW; j++)
		{
			if (arr[i][j] == 3)//食物为3
				flag = 1;
		}
	}
	if (!flag)//没有食物就生成食物
	{
		*x = rand() % ROW;
		*y = rand() % LINE;
	}
}

void GetBody(char arr[][LINE],int food_x,int food_y,int row,int line,int temp_x,int temp_y)//获得蛇的身体
{
	if (food_x == row&&food_y == line)//吃到食物
	{
		arr[temp_x][temp_y] = 4;//身体
	}
}

void Game()
{
	char arr[ROW][LINE]=
	{
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },

	};//地图

	int row = 10, line = 10;//蛇头起始位置
	arr[row][10] = 2;

	char move = 'w';//蛇头开始往上移动

	int food_x = 0, food_y = 0;//食物的x,y坐标
	srand((unsigned)time(NULL));//随机数种子

	//保留一份蛇头移动前坐标
	int temp_x = row;
	int temp_y = line;

	while (1)
	{
		
		system("cls");

		food(arr, &food_x, &food_y);//生成食物
		arr[food_x][food_y] = 3;

		MapPrint(arr);//打印图形

		arr[row][line] = 0;//蛇头原来位置变为空白
		
		//保留蛇头移动前的位置
		temp_x = row;
		temp_y = line;

		Move(&row, &line,&move);//蛇头移动
		arr[row][line] = 2;//新位置变成蛇头

		GetBody(arr,food_x,food_y,row,line,temp_x,temp_y);//判断是否吃到食物

		Sleep(200);//通过控制休眠时间来控制蛇头移动速度


	}

	

}

