#include "suduko.h"


//文件定义
IMAGE zero_mask;
IMAGE zero_back;
IMAGE one_mask;
IMAGE one_back;
IMAGE two_mask;
IMAGE two_back;
IMAGE there_mask;
IMAGE there_back;
IMAGE four_mask;
IMAGE four_back;
IMAGE five_mask;
IMAGE five_back;
IMAGE six_mask;
IMAGE six_back;
IMAGE seven_mask;
IMAGE seven_back;
IMAGE eight_mask;
IMAGE eight_back;
IMAGE nine_mask;
IMAGE nine_back;
IMAGE background;
IMAGE fail;

void Load()//图片加载
{
	loadimage(&background, "background.jpg", SIZE, SIZE);
	loadimage(&fail, "fail.jpg", SIZE, SIZE);
	loadimage(&zero_back, "zero_back.jpg", NUM, NUM);
	loadimage(&zero_mask, "zero_mask.jpg", NUM, NUM);
	loadimage(&one_back, "one_back.jpg", NUM, NUM);
	loadimage(&one_mask, "one_mask.jpg", NUM, NUM);
	loadimage(&two_back, "two_back.jpg", NUM, NUM);
	loadimage(&two_mask, "two_mask.jpg", NUM, NUM);
	loadimage(&there_back, "there_back.jpg", NUM, NUM);
	loadimage(&there_mask, "there_mask.jpg", NUM, NUM);
	loadimage(&four_back, "four_back.jpg", NUM, NUM);
	loadimage(&four_mask, "four_mask.jpg", NUM, NUM);
	loadimage(&five_back, "five_back.jpg", NUM, NUM);
	loadimage(&five_mask, "five_mask.jpg", NUM, NUM);
	loadimage(&six_back, "six_back.jpg", NUM, NUM);
	loadimage(&six_mask, "six_mask.jpg", NUM, NUM); 
	loadimage(&seven_mask, "seven_mask.jpg", NUM, NUM);
	loadimage(&seven_back, "seven_back.jpg", NUM, NUM);
	loadimage(&eight_mask, "eight_mask.jpg", NUM, NUM);
	loadimage(&eight_back, "eight_back.jpg", NUM, NUM);
	loadimage(&nine_back, "nine_back.jpg", NUM, NUM);
	loadimage(&nine_mask, "nine_mask.jpg", NUM, NUM);
}


char *str = "012345678";
void Show(char sudo[][COL],int row,int col)
{
	BeginBatchDraw();//双缓冲绘图，防闪烁
	putimage(0, 0, &background, SRCPAINT);

	for (int i = 0; i < col; i++)//输出下标
	{
		outtextxy(0, i*NUM+NUM/2, str[i]);
		outtextxy(i*NUM+50, 0, str[i]);
	}
	
	

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (sudo[i][j] == '.')
			{
				putimage(j*NUM + NUM2, i*NUM + NUM2, &zero_mask, SRCAND);//一行行打印，一行之中改变的是x的坐标
				putimage(j*NUM + NUM2, i*NUM + NUM2, &zero_back, SRCPAINT);
			}
			else if (sudo[i][j] == '1')
			{
				putimage(j*NUM + NUM2, i*NUM + NUM2, &one_mask, SRCAND);
				putimage(j*NUM + NUM2, i*NUM + NUM2, &one_back, SRCPAINT);
			}
			else if (sudo[i][j] == '2')
			{
				putimage(j*NUM + NUM2, i*NUM + NUM2, &two_mask, SRCAND);
				putimage(j*NUM + NUM2, i*NUM + NUM2, &two_back, SRCPAINT);
			}
			else if (sudo[i][j] == '3')
			{
				putimage(j*NUM + NUM2, i*NUM + NUM2, &there_mask, SRCAND);
				putimage(j*NUM + NUM2, i*NUM + NUM2, &there_back, SRCPAINT);
			}
			else if (sudo[i][j] == '4')
			{
				putimage(j*NUM + NUM2, i*NUM + NUM2, &four_mask, SRCAND);
				putimage(j*NUM + NUM2, i*NUM + NUM2, &four_back, SRCPAINT);
			}
			else if (sudo[i][j] == '5')
			{
				putimage(j*NUM + NUM2, i*NUM + NUM2, &five_mask, SRCAND);
				putimage(j*NUM + NUM2, i*NUM + NUM2, &five_back, SRCPAINT);
			}
			else if (sudo[i][j] == '6')
			{
				putimage(j*NUM + NUM2, i*NUM + NUM2, &six_mask, SRCAND);
				putimage(j*NUM + NUM2, i*NUM + NUM2, &six_back, SRCPAINT);
			}
			else if (sudo[i][j] == '7')
			{
				putimage(j*NUM + NUM2, i*NUM + NUM2, &seven_mask, SRCAND);
				putimage(j*NUM + NUM2, i*NUM + NUM2, &seven_back, SRCPAINT);
			}
			else if (sudo[i][j] == '8')
			{
				putimage(j*NUM + NUM2, i*NUM + NUM2, &eight_mask, SRCAND);
				putimage(j*NUM + NUM2, i*NUM + NUM2, &eight_back, SRCPAINT);
			}
			else if (sudo[i][j] == '9')
			{
				putimage(j*NUM +NUM2, i*NUM + NUM2, &nine_mask, SRCAND);
				putimage(j*NUM + NUM2, i*NUM + NUM2, &nine_back, SRCPAINT);
			}

		}
	}

	EndBatchDraw();
}


int Input(char sudo[][COL],int row,int col,int *x,int *y)
{
	int flag = 1;

	char ch='a';
	char *str = "请输入横纵坐标和要填入的数字";
	char*str2 = "每输入一个数字按回车输入一下个数字";
	char *str3 = "输入有误，请从新输入";

	outtextxy(100, 580,str);
	outtextxy(100, 610,str2);

	while (1)
	{
		scanf("%d %d %c", x, y, &ch);

		if (*x >= 0 && *x < 9 && *y >= 0 && *y < 9 && ch - '0' >= 0 && ch - '0' <= 9 && sudo[*x][*y] == '.')
		{
			if (JudgeNum(sudo, *x, *y, ch) == false)
				flag = 0;

			sudo[*x][*y] = ch;
			break;
		}
		else
			outtextxy(100, 650, str3);
	}

	return flag;

}

void Word()//文字设置
{
	//文字设置
	LOGFONT w;
	gettextstyle(&w);//获得文字设置
	w.lfHeight = 30;//高
	w.lfWeight = 50;//宽
	settextcolor(YELLOW);
	settextstyle(&w);//设置字体样式
	setbkmode(TRANSPARENT);//设置文字透明背景
}

bool JudgeNum(char sudu[][COL], int row, int col, char ch)//判断是否能够放入字符
{
	for (int i = 0; i<9; i++)
	{
		if (sudu[row][i] == ch)//当前行是否有重复数字
			return false;

		if (sudu[i][col] == ch)//当前列是否有重复数字
			return false;

		if (sudu[3 * (row / 3) + i / 3][3 * (col / 3) + i % 3] == ch)//当前九宫格有重复数字
			return false;
	}
	return true;//可以填入

}
void Game()
{
	Word();

 char sudo[ROW][COL] = 
		{
			{ '5', '3', '.', '.', '7', '.', '.', '.', '.' },
			{ '6', '.', '.', '1', '9', '5', '.', '.', '.' },
			{ '.', '9', '8', '.', '.', '.', '.', '6', '.' },
			{ '8', '.', '.', '.', '6', '.', '.', '.', '3' },
			{ '4', '.', '.', '8', '.', '3', '.', '.', '1' },
			{ '7', '.', '.', '.', '2', '.', '.', '.', '6' },
			{ '.', '6', '.', '.', '.', '.', '2', '8', '.' },
			{ '.', '.', '.', '4', '1', '9', '.', '.', '5' },
			{ '.', '.', '.', '.', '8', '.', '.', '7', '9' } };
	
 int count = 0;
 while (1)
 {
	 //输入的横纵坐标
	 int x = 0;
	 int y = 0;

	 Show(sudo, ROW, COL);


	 if (!Input(sudo, ROW, COL, &x, &y))//失败
	 {
		 cleardevice();//清屏
		 putimage(0, 0, &fail, SRCPAINT);
		 getchar();

		 break;
	 }
	 if (x==8&&y==8)//胜利了
	 {
		 cleardevice();//清屏
		 char *str = "恭喜你，挑战成功！";
		 outtextxy(0, 0, str);
	 }

	cleardevice();//清屏

 }
 

}