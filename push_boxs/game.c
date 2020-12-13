#include "game.h"

void ShowMap(char arr[][LINE])//打印地图
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			if (arr[i][j] == 0)
				printf("■");//打印墙壁
			else if (arr[i][j] == 1)//打印空白
				printf("  ");
			else if (arr[i][j] == 2)//打印箱子
				printf("●");
			else if (arr[i][j] == 3)//打印目的点
				printf("¤");
			else if (arr[i][j] == 4)
				printf("♀");
		}
		printf("\n");
	}
}
void Move(char arr[][LINE],int *row,int *line)//移动小人儿
{
	while (1)
	{
		int newrow = *row;
		int newline = *line;
		int quit = 0;
		printf("请通过w,s,a,d，控制上下左右\n");
		int move=0;
		int c = 0;
		while (!quit)
		{
			move = getchar();//从标准输入读取字符
			while((c=getchar())!='\n');//吃掉空格
			switch (move)
			{
			case 'w':
				newrow--;//向上移动
				quit = 1;
				break;
			case 's':
				newrow++;//向下移动
				quit = 1;
				break;
			case 'a':
				newline--;//向左移动
				quit = 1;
				break;
			case 'd':
				newline++;//向右移动
				quit = 1;
				break;
			default:
				printf("输入有误，请从新输入\n");
				break;
			}
		}

		if (arr[newrow][newline] == 0 || arr[newrow][newline]==3)//下一个坐标为墙壁或者目标点
		{
			printf("禁止前行\n");
		}
		
		else//前面为空白，箱子，或者目标点
		{
			if (arr[newrow][newline] == 1)//为空白
			{
				arr[newrow][newline]= 4;//将小人移动到坐标点
				arr[*row][*line]= 1;//小人的地方变为空白
				*row = newrow;
				*line = newline;
				break;
			}
			else if (arr[newrow][newline] == 2)//下一个目标点为箱子
			{
				int nextrow = newrow;
				int nextline = newline;
				if (move == 'w')//上
					nextrow = newrow - 1;
				else if (move == 's')//下
					nextrow = newrow + 1;
				else if (move == 'a')//左
					nextline = newline - 1;
				else//右
					nextline = newline + 1;
				if (arr[nextrow][nextline] == 0 || arr[nextrow][nextline] == 2)//箱子前面是墙壁或者箱子
					printf("移动不了,箱子前方有障碍\n");
				else//箱子前面不是墙壁
				{
					arr[nextrow][nextline]= 2;//箱子前挪
					arr[newrow][newline]= 4;//箱子位置变为人
					arr[*row][*line]= 1;//小人的地方变为空白
					*row = newrow;
					*line = newline;
					break;
				}

			}

		}
	}

}
void  Judge(char arr[][LINE],int *boxs)
{
	int temp = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			if (arr[i][j] == 3)//剩余目标点数
				temp++;
		}
	}
	*boxs = temp;
}
void Game()
{
	char arr[10][10] = {  //0位墙壁■，1位空白，2为箱子●，3为目的点¤，4为人♀
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 3, 1, 0, 0, 0, 1, 0, 0 },
		{ 0, 0, 1, 1, 1, 1, 1, 3, 1, 0 },
		{ 0, 1, 1, 1, 2, 1, 1, 1, 0, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
		{ 0, 1, 2, 1, 0, 1, 2, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 1, 1, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 1, 1, 3, 0 },
		{ 0, 3, 1, 2, 1, 4, 1, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	};//初始化地图
	ShowMap(arr);
	int row = 8, line = 5;//小人初始横纵坐标
	int boxs= 4;//还未到达目标点的盒子数
	while (boxs)
	{
		Move(arr, &row, &line);//移动小人
		ShowMap(arr);
		Judge(arr, &boxs);//剩余目标点数
		if (boxs == 0)
			printf("恭喜你，通关成功\n");
	}

}