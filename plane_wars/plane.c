#include "plane.h"

char str[ROW][LINE]={ // 0����߿�(��)��1������Ϸ����(�ո�),2����ɻ�(��)
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

void Showmap(char str[][LINE])//��ͼչʾ
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			if (str[i][j] == 0)// 0����߿�(��)
				printf("��");
			else if (str[i][j] == 1)
				printf("  ");//1������Ϸ����(�ո�)
			else if (str[i][j] == 2)//2����ɻ�(��)
				printf("��");
			else if (str[i][j] == 3)//3��ʾ�ӵ���
				printf("��");
			else if (str[i][j] == 4)//4��ʾ�ط��ɻ���
				printf("��");
			else if (str[i][j] == 5)//5��ʾ�л��ӵ�
				printf("��");
		}
		printf("\n");
	}
}

void Move(char str[][ROW],int row,int line)//�����ƶ�
{
	int count = 0;//ͳ�Ʋ������ɵл�
	int newrow = row;
	int newline = line;
	while (1)
	{
		int quit = 0;
		while (!quit)
		{
			printf("w s a d ������������\n");
			int ch = 0;
			int move = 0;//�����ƶ�����
			move = getchar();
			while ((ch = getchar()) != '\n');//�Ե��ո�ȶ���ķ���
			switch (move)
			{
			case 'w'://����
				newrow--;
				quit = 1;//�ɹ��ƶ�һ�ξ��˳�����ӡ��ͼ�γɶ�̬Ч��
				break;
			case 's'://��
				newrow++;
				quit = 1;
				break;
			case 'a'://��
				newline--;
				quit = 1;
				break;
			case 'd'://��
				newline++;
				quit = 1;
				break;
			default:
				printf("�����������������\n");
				break;
			}
		}

		if (str[newrow][newline] != 0)//��Ϊǽ��,��������
		{
			int judge=Judge(str, newrow, newline);//�����ж�
			if (judge)//�ж����л��ӵ�����
				break;
			str[newrow][newline] = 2;
			str[row][line] = 1;
			row = newrow;
			line = newline;
		}
		else//�������ݽ�������
		{
			newrow = row;
			newline = line;
		}
		int size = Buttle(str, row, line);//�����ӵ�������size�Ǹ������ӵ�����
		FadeButtle(str, row, line, size);//�����ӵ�
		if (count % 2 == 0)//ÿ�ƶ��Ĵ�����һ���л�
		{
			Plan(str);
		}
		count++;
	}
}
int Buttle(char str[][LINE],int row,int line)//�ӵ�
{
	int size =1;//��1��ʼ��Ҫ�ж��ӵ�ǰ�Ƿ���ǽ��
	while ((size != 6) && str[row - size][line]!=0)//�ɻ�ǰ�����5���ӵ�,�ӵ�������ǽ
	{
		str[row-size][line]=3;//3��ʾ�ӵ�
		size++;
		system("cls");
		Showmap(str);//��̬�ӵ�
	}
	return size;
}
void FadeButtle(char str[][LINE], int row, int line,int size)
{
	size--;//�����ӵ�ʱ����������һ��
	while (size)
	{
		str[row - size][line] = 1;//�����ӵ��������ӵ�����ɿհ�
		size--;
	}
}
void Plan(char str[][LINE])//�л�����
{
	srand((unsigned)time(NULL));//�������������
	//�л�����
	int foerow = 0;
	int foeline = 0;
	while (1)
	{
		foerow = (rand() %(ROW-2)) + 1;//+1�����겻��Ϊǽ��
		foeline = (rand() % (LINE-2)) + 1;
		if (str[foerow][foeline] != 2 && str[foerow][foeline] != 3)//������ֵĵл�����һ���������ҷ��ɻ���ײ
		{
			str[foerow][foeline] = 4;//�ط��ɻ�������4��ͼ���æ���
			FeoButtle(str, foerow, foeline);
			break;
		}
	}
}
void FeoButtle(char str[][LINE],int foerow,int foeline)//�л��ӵ�
{
	//�л�����ʱ�ӵ�����ֱ��ײ���������ӵ����ܵ�ǽ�ڲ�
	if (str[foerow - 1][foeline] != 0 && str[foerow - 1][foeline] != 2)//�Ϸ�
		str[foerow - 1][foeline] = 5;
	if (str[foerow + 1][foeline] != 0 && str[foerow + 1][foeline] != 2)//�·�
		str[foerow + 1][foeline] = 5;
	if (str[foerow][foeline-1] != 0 && str[foerow][foeline-1] != 2)//��
		str[foerow][foeline-1] = 5;
	if (str[foerow][foeline + 1] != 0 && str[foerow][foeline + 1] != 2)//�ҷ�
		str[foerow][foeline + 1] = 5;
}
int Judge(char str[][LINE],int row,int line)//�жϺ���
{
	if (str[row][line] == 5)
	{
		printf("�㱻�ӵ������ˣ���Ϸ����\n");
		return 1;
	}
	else
		return 0;
}
void game()
{
	int row = 18, line = 8;//�ɻ���ʼλ��
	Plan(str);//���������һ�ܵл�
	Showmap(str);
	Move(str,row,line);
}