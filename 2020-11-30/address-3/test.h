#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <windows.h>
#pragma warning (disable :4996)
#include <stdlib.h>


#define TWO 2
#define NUM 20
typedef struct LIST //���Դ���ĳ�Ա��Ϣ�б�
{
	char name[20];
	char sex[20];
	int age;
	char tel[20];
	char address[50];
}list;

typedef struct ADDRESS_BOOK
{
	list *call;//������Ϣ��
	int count;//�����洢����
	int  capacity;//��ǰͨѶ¼����
}address;

void Menu();//�˵�
void Add();//�����ϵ����Ϣ
void Delect();//ɾ����ϵ����Ϣ
int Found();//�ҵ�����ӡ��ϵ����Ϣ
void Alter();//�޸���ϵ����Ϣ
void Show();//��ʾ������ϵ��
void Empty();//���������ϵ��
void Rank();//����������
void Open();//���ٿռ亯��
void Save_book();//�����ݱ��浽����֮��
#endif