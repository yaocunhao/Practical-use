#include "suduko.h"


//�ļ�����
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

void Load()//ͼƬ����
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
	BeginBatchDraw();//˫�����ͼ������˸
	putimage(0, 0, &background, SRCPAINT);

	for (int i = 0; i < col; i++)//����±�
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
				putimage(j*NUM + NUM2, i*NUM + NUM2, &zero_mask, SRCAND);//һ���д�ӡ��һ��֮�иı����x������
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
	char *str = "��������������Ҫ���������";
	char*str2 = "ÿ����һ�����ְ��س�����һ�¸�����";
	char *str3 = "�����������������";

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

void Word()//��������
{
	//��������
	LOGFONT w;
	gettextstyle(&w);//�����������
	w.lfHeight = 30;//��
	w.lfWeight = 50;//��
	settextcolor(YELLOW);
	settextstyle(&w);//����������ʽ
	setbkmode(TRANSPARENT);//��������͸������
}

bool JudgeNum(char sudu[][COL], int row, int col, char ch)//�ж��Ƿ��ܹ������ַ�
{
	for (int i = 0; i<9; i++)
	{
		if (sudu[row][i] == ch)//��ǰ���Ƿ����ظ�����
			return false;

		if (sudu[i][col] == ch)//��ǰ���Ƿ����ظ�����
			return false;

		if (sudu[3 * (row / 3) + i / 3][3 * (col / 3) + i % 3] == ch)//��ǰ�Ź������ظ�����
			return false;
	}
	return true;//��������

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
	 //����ĺ�������
	 int x = 0;
	 int y = 0;

	 Show(sudo, ROW, COL);


	 if (!Input(sudo, ROW, COL, &x, &y))//ʧ��
	 {
		 cleardevice();//����
		 putimage(0, 0, &fail, SRCPAINT);
		 getchar();

		 break;
	 }
	 if (x==8&&y==8)//ʤ����
	 {
		 cleardevice();//����
		 char *str = "��ϲ�㣬��ս�ɹ���";
		 outtextxy(0, 0, str);
	 }

	cleardevice();//����

 }
 

}