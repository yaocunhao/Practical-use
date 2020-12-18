#ifndef _PRESOM_H_
#define _PRESOM_H_

#include <stdio.h>
#include <windows.h>
#include <assert.h>
#pragma warning (disable :4996)

typedef struct List
{
	char name[20];
	char sex[20];
	int age;
	char tel[20];
	char address[50];
}list;

typedef struct Preson
{
	int capacity;//����
	int size;//��ǰ����
	list people[0];//�ṹ���������飬�洢�ͻ���Ϣ�ṹ��
}preson;

void Menu();//�˵�
void Open(preson **peo);//���ٿռ亯��
void Add(preson **pep);//�����ϵ����Ϣ
int Found(preson **peo);//�ҵ�����ӡ��ϵ����Ϣ
void Delect(preson **peo);//ɾ����ϵ����Ϣ
void Alter(preson **peo);//�޸���ϵ����Ϣ
void Show(preson **peo);//��ʾ������ϵ��
void Empty(preson **peo);//���������ϵ��
void Rank(preson *sp);//����������
void Read(preson **peo);//��ȡ�ļ�
void Save_book(preson *sp);//�����ݱ��浽����֮��


#endif