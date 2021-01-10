#include"plane.h"

//����ͼƬ����
IMAGE background;//����ͼ
IMAGE plane_back;//�ɻ�����ͼ
IMAGE plane_mask;//�ɻ�����ͼ
IMAGE fplane_back;//�л�����ͼ
IMAGE fplane_mask;//�л�����ͼ
IMAGE bullte_back;//�ӵ�����ͼ
IMAGE bullte_mask;//�ӵ�����ͼ
IMAGE fdie_back;//���ел�Ч������ͼ
IMAGE fdie_mask;//���ел�Ч������ͼ
IMAGE pdie_back;//�����б���ͼ
IMAGE pdie_mask;//����������ͼ
IMAGE god_plane;//�ɻ�����ʱ����״̬

//����ȫ�ֱ���
mem plane;//�ɻ��ṹ��
mem bullte[MAX];//�ӵ��ṹ������
mem fplane[FMAX];//�л��ṹ��
mem fdie[FMAX];//���ел�


DWORD b_time_start, b_time_last; //�ӵ�ʱ��仯��
DWORD pf_time_start, pf_time_last; //�л�����ʱ��仯��
DWORD p_time_start, p_time_last; //�������޵�ʱ��仯��

int flag;//��������ֵ
int boom;//��ը��־
int count;//�÷ּ�����


void Load()//ͼƬ����
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
void DataInit()//���ݳ�ʼ��
{
	//�ɻ������ݳ�ʼ��
	plane.x = SIZE / 2 - PSIZE / 2; //�ɻ���ʼλ���ڱ���ͼ������
	plane.y = SIZE - PSIZE;
	plane.lable = true; //�ɻ������Ǵ���

	//�ӵ���ʼ��
	for (int i = 0; i < MAX; i++)
	{
		bullte[i].lable = false;//�ӵ�û���ռ�ǰ�ǲ����ڵ�
	}

	//ʱ��仯��ʼ��
	b_time_start = b_time_last = GetTickCount();//���������ڵĺ�����
	pf_time_start = pf_time_last = GetTickCount();
	p_time_start = p_time_last = GetTickCount();//�������޵�ʱ��仯��

	//�л���ʼ��
	for (int i = 0; i<FMAX; i++)
	{
		fplane[i].lable = false;
	}
	//���ел�Ч����ʼ��
	for (int i = 0; i < FMAX; i++)
	{
		fdie[i].lable = false;
	}

	//����ֵ��ʼ��
	flag = 5;

	//������ըЧ������
	boom = 0;

	//�÷ּ�����
	count = 0;

}

void PlaneCtr()//ͨ�����������������ı�ɻ�������
{
	if (GetAsyncKeyState(VK_UP))//�����ƶ�
	{
		if (plane.y>0)//����Խ��
		plane.y-=0.8;
	}
	if (GetAsyncKeyState(VK_DOWN))//�����ƶ�
	{
		if (plane.y<SIZE-PSIZE)//y���ֵ600���ɻ�ͼƬ��СΪ80
		plane.y+=0.8;
	}
	if (GetAsyncKeyState(VK_LEFT))//�����ƶ�
	{
		if (plane.x>-PSIZE/2)//��������ǽ��һ�����
		plane.x-=0.8;
	}
	if (GetAsyncKeyState(VK_RIGHT))//�����ƶ�
	{
		if (plane.x<SIZE-PSIZE/2)
		plane.x+=0.8;
	}
	if (GetAsyncKeyState(VK_SPACE) && b_time_last - b_time_start>80)//�����ӵ���ÿ���ӵ����Ϊ80����
	{
		BullterCre();//�ո����ӵ�
		b_time_start = b_time_last;
	}
	b_time_last = GetTickCount();
}

void Show()//�������
{	
	BeginBatchDraw();//˫�����ͼ������˸

	putimage(0, 0, &background);
	Score();//����������ֵ��ʾ

	if (plane.lable)//�ɻ����ʹ�ӡ�ɻ�
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
			//�ɻ�����
			plane.x = SIZE / 2 - PSIZE / 2; //�ɻ���ʼλ���ڱ���ͼ������
			plane.y = SIZE - PSIZE;
		}
	}
	if (plane.lable == false&&boom==0&&flag!=0)
	{
		if (p_time_last - p_time_start>=4000)//�޵�ʱ��3��
		{
			plane.lable = true;
			p_time_start = p_time_last;
		}
		p_time_last = GetTickCount();

		putimage((int)plane.x, (int)plane.y, &plane_mask, SRCAND);
		putimage((int)plane.x, (int)plane.y, &god_plane, SRCPAINT);
	}
	for (int i = 0; i < MAX; i++)//ѭ���ж��ӵ�
	{
		if (bullte[i].lable)//�ӵ����ھʹ�ӡ�ӵ�
		{
			putimage((int)bullte[i].x, (int)bullte[i].y, &bullte_mask, SRCAND);
			putimage((int)bullte[i].x, (int)bullte[i].y, &bullte_back, SRCPAINT);
		}
	}

	for (int i = 0; i < FMAX; i++)
	{
		if (fdie[i].lable)//��ӡ����Ч��
		{
			putimage((int)fdie[i].x, (int)fdie[i].y, &fdie_mask, SRCAND);
			putimage((int)fdie[i].x, (int)fdie[i].y, &fdie_back, SRCPAINT);
		}

		if (fplane[i].lable)//��ӡ�л�
		{
			putimage((int)fplane[i].x, (int)fplane[i].y, &fplane_mask, SRCAND);
			putimage((int)fplane[i].x, (int)fplane[i].y, &fplane_back, SRCPAINT);
		}
	}

	EndBatchDraw();
}

void BullterCre()//���˿ո񣬵��ú����������ӵ�
{
	for (int i = 0; i < MAX; i++)
	{
		if (!bullte[i].lable)//�ӵ������ھʹ����ӵ�
		{
			//�ӷɻ�����ǰ������ȥ
			bullte[i].x = plane.x + PSIZE / 2 - BSIZE / 2;
			bullte[i].y = plane.y - BSIZE;
			bullte[i].lable = true;
			break;//һ�οո����һ���ӵ�
		}
	}
}
void BullterMove()//�ӵ��ƶ�
{
	for (int i = 0; i < MAX; i++)
	{
		if (bullte[i].lable)//�ӵ����ڣ�����y����ͼ���
		{
			bullte[i].y--;//������������ƶ�

			if (bullte[i].y < 0)//�ӵ����գ������´�ȫ�Ƿ����ȥ���ӵ�
			{
				bullte[i].lable = false;
			}
		}
	}
}
void FplaneCre()//�л�����
{
	for (int i = 0; i < FMAX; i++)
	{
		if (!fplane[i].lable&&pf_time_last-pf_time_start>100)//û�ел��ͽ��й�������ʱ�亯�����Ʋ����ٶ�
		{
			fplane[i].lable = true;
			fplane[i].x = rand() % SIZE - PSIZE;//���õг����ڴ�����
			fplane[i].y = 0;
			pf_time_start = pf_time_last;
			break;//ÿ��һ��
		}
		pf_time_last = GetTickCount();//��ȡ�µ�ʱ��
	}
}
void FplaneMove()//�л��ƶ�
{
	for (int i = 0; i < FMAX; i++)
	{
		if (fplane[i].lable)
		{
			fplane[i].y+=0.3;//�л�������

			if (fplane[i].y>SIZE)//�л�����
				fplane[i].lable = false;
		}
	}
}
void Hit()//����л�
{
	for (int i = 0; i < FMAX; i++)
	{
		if (!fplane[i].lable)//����������һ���ж�
			continue;

		for (int j = 0; j < MAX; j++)
		{
			if (!bullte[j].lable)
				continue;//�ӵ�������Ҳ����һ���ж�

			if (bullte[j].x>fplane[i].x&&  //�ӵ��͵л��н�������ʧ
				bullte[j].x<fplane[i].x + PSIZE
				&&bullte[j].y>fplane[i].y&&
				bullte[j].y <fplane[i].y + PSIZE) 
			{
				fdie[i].lable = true;//����Ч��
				fdie[i].x = fplane[i].x;
				fdie[i].y = fplane[i].y;
				bullte[j].lable = false;
				fplane[i].lable = false;
				count++;
			}
		}
	}
}



void FdieRetain()//���к�Ч������һ��ʱ��
{
	for (int i = 0; i < FMAX; i++)
	{
		if (rand() % 40 == 2)
		{
			fdie[i].lable = false;
		}
	}

}

void PDie()//���л�����
{
	for (int i = 0; i < FMAX; i++)
	{
		if (!fplane[i].lable)
			continue;

		if (plane.x>fplane[i].x&&plane.x<fplane[i].x + PSIZE
			&&plane.y>fplane[i].y&&plane.y < fplane[i].y + PSIZE&&plane.lable)
		{
			plane.lable = false;
			flag-=1;//����ֵ-1

			boom = 1;//�����ըͼƬ
			
			break;
		}
	}
}

void Score()//������ʾ
{
	char myscore[MAX] = "��ǰ�÷�Ϊ��";
	char num[20];
	sprintf(num, "%d", count);
	settextcolor(WHITE);
	outtextxy(0, 0, myscore);//�������
	outtextxy(80, 0, num);

	char _myscore[MAX] = "��ǰ����ֵ��";
	char _num[20];
	sprintf(_num, "%d", flag);
	settextcolor(WHITE);
	outtextxy(0, 50, _myscore);
	outtextxy(90, 50, _num);//���ʣ������ֵ

}

void Game()
{
	//����ͼƬ���ͳ�ʼ������
	Load();
	DataInit();
	srand((unsigned)time(NULL));//�������������

	while (1)
	{
		Show();//ͼ����ʾ
		if (flag==0)
			break;
		PlaneCtr();//�ٿ�
		BullterMove();//�ӵ��ƶ�
		FplaneCre();//�л�����		
		Hit();//�ж��Ƿ���ел�
		FplaneMove();//�л�������
		FdieRetain();//���ел���Ч������һ��ʱ��
		PDie();//���л�����
	}

}



