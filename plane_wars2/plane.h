#ifndef _PLANE_H_
#define _PLANE_H_

#include <stdio.h>
#include <graphics.h>
#include <time.h>

#define  SIZE 800 //������С
#define  PSIZE 80 //�ɻ���С
#define  BSIZE 40 //�ӵ���С
#define  MAX 10//�ӵ���Ŀ
#define  FMAX 10//�л���Ŀ
typedef struct Member
{
	double x;
	double y;
	bool lable;
}mem;

void Game();
void Load();//ͼƬ����
void DataInit();//���ݳ�ʼ��
void PlaneCtr();//ͨ�����������������ı�ɻ�������
void Show();//�������
void BullterCre();//���˿ո񣬵��ú����������ӵ�
void BullterMove();//�ӵ��ƶ�
void FplaneCre();//�л�����
void FplaneMove();//�л��ƶ�
void Hit();//����л�




#endif