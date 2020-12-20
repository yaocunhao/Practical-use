#include "game.h"

void ShowMap(char arr[][LINE])//��ӡ��ͼ
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			if (arr[i][j] == 0)
				printf("��");//��ӡǽ��
			else if (arr[i][j] == 1)//��ӡ�հ�
				printf("  ");
			else if (arr[i][j] == 2)//��ӡ����
				printf("��");
			else if (arr[i][j] == 3)//��ӡĿ�ĵ�
				printf("��");
			else if (arr[i][j] == 4)//��ӡ����
				printf("��");
			else if (arr[i][j] == 5)//��ӡ�����Ŀ����ص����ͼ��
				printf("��");
			else if (arr[i][j] == 6)//���ӽ���Ŀ�����ͼ��
				printf("��");
		}
		printf("\n");	
	}
}
void Move(char arr[][LINE], int *row, int *line, char retarr[][LINE],int *retrow, int *retline)//�ƶ�С�˶�
{
	while (1)
	{
		int newrow = *row;
		int newline = *line;
		int quit = 0;
		printf("��ͨ��w,s,a,d��������������,����r����\n");
		int move=0;
		int c = 0;
		while (!quit)
		{
			move = getchar();//�ӱ�׼�����ȡ�ַ�
			while((c=getchar())!='\n');//�Ե�������ַ��Լ��ո�
			switch (move)
			{
			case 'w':
				newrow--;//�����ƶ�
				quit = 1;
				break;
			case 's':
				newrow++;//�����ƶ�
				quit = 1;
				break;
			case 'a':
				newline--;//�����ƶ�
				quit = 1;
				break;
			case 'd':
				newline++;//�����ƶ�
				quit = 1;
				break;
			case 'r':
				quit = 1;
				break;
			default:
				printf("�����������������\n");
				break;
			}
		}
		if (move == 'r')//����
		{
			system("cls");
			ShowMap(retarr);//��ӡ�ɵ�ͼ
			Retain(arr, retarr);//��ͼ���ݸ���Ϊ��һ��������
			//��С�������滻Ϊ��һ����С������
			*row = *retrow;
			*line = *retline;
			break;
		}
		Save(retarr, arr, row, line, retrow, retline);//��ʱ�Ѿ�����Ҫ���أ��������ǰ����һ��
		if (arr[newrow][newline] == 0 )//��һ������Ϊǽ��
		{
			printf("��ֹǰ��\n");
			break;
		}
		
		else//ǰ��Ϊ�հף����ӣ�����Ŀ��㣬�������ӵ�Ŀ����Ľ��ͼ��(Ҳ������)
		{
			if (arr[newrow][newline] == 1)//Ϊ�հ�
			{
				arr[newrow][newline]= 4;//��С���ƶ��������
				if (arr[*row][*line] == 5)//��ʱС����Ŀ���غϣ�
				{
					arr[*row][*line] = 3;//��ô������ͱ��Ŀ����ˣ�
				}
				else
				{
					arr[*row][*line] = 1;//С�˵ĵط���Ϊ�հ�
				}
				//�������
				*row = newrow;
				*line = newline;
			}
			else if (arr[newrow][newline] == 3)//ΪĿ���
			{
				arr[newrow][newline]=5;//С�˶���Ŀ����غϵ�ͼ��
				
				if (arr[*row][*line] == 5)//��ʱС����Ŀ���غϣ�
				{
					arr[*row][*line] = 3;//��ô������ͱ��Ŀ����ˣ�
				}
				else
				{
					arr[*row][*line] = 1;//С�˵ĵط���Ϊ�հ�
				}
				//�������
				*row = newrow;
				*line = newline;
			}
			else if (arr[newrow][newline] == 2||arr[newrow][newline]==6)//��һ����Ϊ���ӻ������Ӻ�Ŀ���Ľ��ͼ��
			{
				int nextrow = newrow;
				int nextline = newline;
				if (move == 'w')//��
					nextrow = newrow - 1;
				else if (move == 's')//��
					nextrow = newrow + 1;
				else if (move == 'a')//��
					nextline = newline - 1;
				else//��
					nextline = newline + 1;
				if (arr[nextrow][nextline] == 0 || arr[nextrow][nextline] == 2||arr[nextrow][nextline]==6)
					//����ǰ����ǽ�ڻ������ӻ������Ӻ�Ŀ���Ľ��
				{
					printf("�ƶ�����,����ǰ�����ϰ�\n");
					break;
				}

				else //����ǰ�治��ǽ��
				{
					if (arr[nextrow][nextline] == 3)//����ǰ����Ŀ���
					{
						arr[nextrow][nextline] = 6;//����ǰŲ��ͼ��������Ӻ�Ŀ����غϵ�ͼ��
					}
					else//����ǰ���ǿհ�
					{
						arr[nextrow][nextline] = 2;//�հ״��������
					}
					if (arr[newrow][newline] == 6)//ԭ����λ����Ŀ���
					{
						arr[newrow][newline] = 5;//���Ŀ����С�˶��Ľ��
					}
					else
					{
						arr[newrow][newline] = 4;//����λ�ñ�Ϊ��
					}
					if (arr[*row][*line] == 5)//��ʱС����Ŀ����غϣ�
					{
						arr[*row][*line] = 3;//��ô������ͱ��Ŀ����ˣ�
					}
					else
					{
						arr[*row][*line] = 1;//С�˵ĵط���Ϊ�հ�
					}
					//�������
					*row = newrow;
					*line = newline;
				}
			}

		}
		//�����ƶ�֮��ĵ�ͼ��ӡ��
		system("cls");
		ShowMap(arr);
		break;
	}

}
void  Judge(char arr[][LINE],int *boxs)
{
	int temp = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			if (arr[i][j] == 3||arr[i][j]==5)//ʣ��Ŀ������Լ����������Ŀ���	
				temp++;
		}
	}
	*boxs = temp;
}
void Retain(char retarr[][LINE],char arr[][LINE])//����ɵ�ͼ
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			retarr[i][j] = arr[i][j];
		}
	}
}
void Save(char retarr[][LINE], char arr[][LINE], int *row, int *line, int *retrow, int *retline)//����ǰ���ݱ���
{
	Retain(retarr, arr);//������ǰ���䱣��һ�ݣ�
	//С���������ǰ����һ��
	*retrow = *row;
	*retline = *line;
}

void SlectMap(char arr[][LINE], char map[][ROW][LINE],int select)//ѡ���ͼ
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < LINE; j++)
		{
			arr[i][j] = map[select][i][j];
		}
	}
}
char map[2][10][10]={ //0λǽ�ڡ���1λ�հף�2Ϊ���ӡ�3ΪĿ�ĵ�裬4Ϊ�ˡ�,5ΪС�˶���Ŀ����غϡ6Ϊ���ӽ�����Ŀ����

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
   },//��ͼ1
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
	char arr[ROW][LINE];
	int select = 0;
	int quit = 0;
	while (!quit)
	{
		printf("��ѡ����Ĺؿ�\n");
		printf("###########################\n");
		printf("1.��һ��##########2.�ڶ���###\n");
		printf("###########################\n");
		scanf("%d", &select);
		switch (select)
		{
		case 1:
			select = 0;//�����±��0��ʼ
			SlectMap(arr, map, select);
			quit = 1;
			break;
		case 2:
			select = 1;
			SlectMap(arr, map, select);
			quit = 1;
			break;
		default :
			printf("�����������������\n");
		}
	}
	ShowMap(arr);
	int row = 8, line = 5;//С�˳�ʼ��������
	int boxs= 4;//��δ����Ŀ���ĺ�����
	char retarr[ROW][LINE];
	Retain(retarr, arr);//�ȱ���һ�ݣ�ֱ�Ӱ����ز������
	int retrow=row, retline=line;//�ɵ�С�˵�ַ
	while (boxs)
	{
		Move(arr, &row, &line,retarr,&retrow,&retline);//�ƶ�С��
		Judge(arr, &boxs);//ʣ��Ŀ�����
		if (boxs == 0)
			printf("��ϲ�㣬ͨ�سɹ�\n");
	}
}

