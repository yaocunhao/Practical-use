#include "game.h"

//定义图片变量
IMAGE mm;//背景
IMAGE room;//地板
IMAGE box;//箱子
IMAGE people;//人物
IMAGE wall;//墙壁
IMAGE lable;//目标点
IMAGE over;//箱子进入目标点
IMAGE people2;//人物进入目标点

void Load()
{
	initgraph(1000, 1000);
	loadimage(&room,"room.jpg",50, 50);
	loadimage(&mm, "mm.jpg", 1000, 1000);
	loadimage(&box, "box.jpg", 50, 50);
	loadimage(&people, "people.jpg", 50, 50);
	loadimage(&wall, "wall.jpg", 50, 50);
	loadimage(&lable, "lable.jpg", 50, 50);
	loadimage(&over, "over.jpg", 50, 50);
	loadimage(&people2, "people2.jpg", 50, 50);
}

void ShowMap(char arr[][LINE])//打印地图
{
	putimage(0, 0, &mm);
	int x = 0;
	int y = 0;
	for (int i = 0; i < ROW; i++)
	{
		y = i * 50;
		for (int j = 0; j < LINE; j++)
		{
			x = j * 50;
			if (arr[i][j]==1)//地板
				putimage(x, y, &room);
			if (arr[i][j] == 0)//墙壁
				putimage(x, y, &wall);
			else if (arr[i][j] == 2)//打印箱子
				putimage(x, y, &box);
			else if (arr[i][j] == 3)//打印目的点
				putimage(x, y, &lable);
			else if (arr[i][j] == 4)//打印人物
				putimage(x, y, &people);
			else if (arr[i][j] == 5)//打印人物和目标点重叠后的图案
				putimage(x, y, &people2);
			else if (arr[i][j] == 6)//箱子进入目标点后的图案
				putimage(x, y, &over);
		}
	}
}
void Move(char arr[][LINE], int *row, int *line, char retarr[][LINE], int *retrow, int *retline)//移动小人儿
{
	while (1)
	{
		int newrow = *row;
		int newline = *line;
		int quit = 0;
		outtextxy(0,500,"请通过w,s,a,d，控制上下左右,输入r返回");
		//printf("请通过w,s,a,d，控制上下左右,输入r返回\n");
		int move = 0;
		int c = 0;
		while (!quit)
		{
			move = getchar();//从标准输入读取字符
			while ((c = getchar()) != '\n');//吃掉多余的字符以及空格
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
			case 'r':
				quit = 1;
				break;
			default:
				outtextxy(0, 500, "输入有误，请从新输入");
				//printf("输入有误，请从新输入\n");
				break;
			}
		}
		if (move == 'r')//返回
		{
			cleardevice();
			ShowMap(retarr);//打印旧地图
			Retain(arr, retarr);//地图内容更换为上一步的内容
			//将小人坐标替换为上一步的小人坐标
			*row = *retrow;
			*line = *retline;
			break;
		}
		Save(retarr, arr, row, line, retrow, retline);//此时已经不需要返回，坐标更新前保存一份
		if (arr[newrow][newline] == 0)//下一个坐标为墙壁
		{
			printf("禁止前行\n");
			break;
		}

		else//前面为空白，箱子，或者目标点，或者箱子到目标点后的结合图案(也是箱子)
		{
			if (arr[newrow][newline] == 1)//为空白
			{
				arr[newrow][newline] = 4;//将小人移动到坐标点
				if (arr[*row][*line] == 5)//此时小人与目标重合，
				{
					arr[*row][*line] = 3;//那么出来后就变成目标点了；
				}
				else
				{
					arr[*row][*line] = 1;//小人的地方变为空白
				}
				//坐标更新
				*row = newrow;
				*line = newline;
			}
			else if (arr[newrow][newline] == 3)//为目标点
			{
				arr[newrow][newline] = 5;//小人儿和目标点重合的图像

				if (arr[*row][*line] == 5)//此时小人与目标重合，
				{
					arr[*row][*line] = 3;//那么出来后就变成目标点了；
				}
				else
				{
					arr[*row][*line] = 1;//小人的地方变为空白
				}
				//坐标更新
				*row = newrow;
				*line = newline;
			}
			else if (arr[newrow][newline] == 2 || arr[newrow][newline] == 6)//下一个点为箱子或者箱子和目标点的结合图案
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
				if (arr[nextrow][nextline] == 0 || arr[nextrow][nextline] == 2 || arr[nextrow][nextline] == 6)
					//箱子前面是墙壁或者箱子或者箱子和目标点的结合
				{
					printf("移动不了,箱子前方有障碍\n");
					break;
				}

				else //箱子前面不是墙壁
				{
					if (arr[nextrow][nextline] == 3)//箱子前面是目标点
					{
						arr[nextrow][nextline] = 6;//箱子前挪，图案变成箱子和目标点重合的图案
					}
					else//箱子前面是空白
					{
						arr[nextrow][nextline] = 2;//空白处变成箱子
					}
					if (arr[newrow][newline] == 6)//原箱子位置是目标点
					{
						arr[newrow][newline] = 5;//变成目标点和小人儿的结合
					}
					else
					{
						arr[newrow][newline] = 4;//箱子位置变为人
					}
					if (arr[*row][*line] == 5)//此时小人与目标点重合，
					{
						arr[*row][*line] = 3;//那么出来后就变成目标点了；
					}
					else
					{
						arr[*row][*line] = 1;//小人的地方变为空白
					}
					//坐标更新
					*row = newrow;
					*line = newline;
				}
			}

		}
		//进行移动之后的地图打印；
		cleardevice();
		ShowMap(arr);
		break;
	}

}
void  Judge(char arr[][LINE], int *boxs)
{
	int temp = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			if (arr[i][j] == 3 || arr[i][j] == 5)//剩余目标点数以及人物进入了目标点	
				temp++;
		}
	}
	*boxs = temp;
}
void Retain(char retarr[][LINE], char arr[][LINE])//保存旧地图
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			retarr[i][j] = arr[i][j];
		}
	}
}
void Save(char retarr[][LINE], char arr[][LINE], int *row, int *line, int *retrow, int *retline)//更新前内容保存
{
	Retain(retarr, arr);//内容新前将其保存一份；
	//小人坐标更新前保存一份
	*retrow = *row;
	*retline = *line;
}

void SlectMap(char arr[][LINE], char map[][ROW][LINE], int select)//选择地图
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			arr[i][j] = map[select][i][j];
		}
	}
}
char map[2][10][10] = { //0位墙壁■，1位空白，2为箱子●，3为目的点¤，4为人♀,5为小人儿与目标点重合★，6为箱子进入了目标点☆

	{
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
	},//地图1
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 3, 0, 0, 0, 0, 0, 1, 3, 0 },
		{ 0, 3, 0, 1, 1, 1, 1, 3, 1, 0 },
		{ 0, 1, 1, 1, 2, 1, 2, 1, 1, 0 },
		{ 0, 1, 2, 1, 1, 1, 1, 0, 0, 0 },
		{ 0, 1, 1, 2, 0, 1, 1, 0, 0, 0 },
		{ 0, 1, 0, 1, 1, 1, 2, 1, 1, 0 },
		{ 0, 3, 0, 2, 1, 0, 1, 1, 0, 0 },
		{ 0, 3, 1, 1, 1, 4, 1, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	},
};

void Game()
{
	Load();
	char arr[ROW][LINE];
	int select = 0;
	int quit = 0;
	while (!quit)
	{
		outtextxy(0,0,"请选择你的关卡");
		outtextxy(0,30,"###########################");
		outtextxy(0,60,"1.第一关##########2.第二关###");
		outtextxy(0,90,"###########################");
		scanf("%d", &select);
		switch (select)
		{
		case 1:
			select = 0;//数组下标从0开始
			SlectMap(arr, map, select);
			quit = 1;
			break;
		case 2:
			select = 1;
			SlectMap(arr, map, select);
			quit = 1;
			break;
		default:
			outtextxy(0, 0, "输入有误，请从新输入");
			printf("输入有误，请从新输入\n");
		}
	}
	ShowMap(arr);
	int row = 8, line = 5;//小人初始横纵坐标
	int boxs = 4;//还未到达目标点的盒子数
	char retarr[ROW][LINE];
	Retain(retarr, arr);//先保存一份，直接按返回不会出错
	int retrow = row, retline = line;//旧的小人地址
	while (boxs)
	{
		Move(arr, &row, &line, retarr, &retrow, &retline);//移动小人
		Judge(arr, &boxs);//剩余目标点数
		if (boxs == 0)
			printf("恭喜你，通关成功\n");
	}
}


