#include "snake.h"

void MapPrint(char(*arr)[LINE])
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			//��һ�У����һ�У���һ�У����һ�ж���ǽ��
			if (arr[i][j]==1)
				printf("��");
			else if (arr[i][j] == 2)
				printf("��");//��ͷ
			else if (arr[i][j] == 3)
				printf("��");//��ӡʳ��
			else if (arr[i][j] == 4)
				printf("��");//��ӡ����
			else
				printf("  ");//һ��ǽ��ռ�����ո�
		}
		printf("\n");
	}
}
void Move(int *row, int *line,char *move)//��ͷ�ƶ�������ͨ����ȡ����ļ��̷�������ı���ͷ����
{
		if (GetAsyncKeyState(VK_UP))//��
		{
			*move = 'w';
		}
		else if (GetAsyncKeyState(VK_DOWN))//��
		{
			*move = 's';
		}
		else if (GetAsyncKeyState(VK_LEFT))//��
		{
			*move = 'a';
		}
		else if (GetAsyncKeyState(VK_RIGHT))//��
		{
			*move = 'd';
		}
		GetCoordinate(row, line, move);//����õķ������Ϣ�����ȥ
}

void GetCoordinate(int *row, int *line, char *move)//��ͷ�Զ��ƶ���ͨ����ȡ�ķ�����Ϣ��������ͷ���ĸ������Զ���
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
			if (arr[i][j] == 3)//ʳ��Ϊ3
				flag = 1;
		}
	}
	if (!flag)//û��ʳ�������ʳ��
	{
		*x = rand() % ROW;
		*y = rand() % LINE;
	}
}

void GetBody(char arr[][LINE],int food_x,int food_y,int row,int line,int temp_x,int temp_y)//����ߵ�����
{
	if (food_x == row&&food_y == line)//�Ե�ʳ��
	{
		arr[temp_x][temp_y] = 4;//����
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

	};//��ͼ

	int row = 10, line = 10;//��ͷ��ʼλ��
	arr[row][10] = 2;

	char move = 'w';//��ͷ��ʼ�����ƶ�

	int food_x = 0, food_y = 0;//ʳ���x,y����
	srand((unsigned)time(NULL));//���������

	//����һ����ͷ�ƶ�ǰ����
	int temp_x = row;
	int temp_y = line;

	while (1)
	{
		
		system("cls");

		food(arr, &food_x, &food_y);//����ʳ��
		arr[food_x][food_y] = 3;

		MapPrint(arr);//��ӡͼ��

		arr[row][line] = 0;//��ͷԭ��λ�ñ�Ϊ�հ�
		
		//������ͷ�ƶ�ǰ��λ��
		temp_x = row;
		temp_y = line;

		Move(&row, &line,&move);//��ͷ�ƶ�
		arr[row][line] = 2;//��λ�ñ����ͷ

		GetBody(arr,food_x,food_y,row,line,temp_x,temp_y);//�ж��Ƿ�Ե�ʳ��

		Sleep(200);//ͨ����������ʱ����������ͷ�ƶ��ٶ�


	}

	

}

