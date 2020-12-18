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
	int capacity;//容量
	int size;//当前人数
	list people[0];//结构体柔性数组，存储客户信息结构体
}preson;

void Menu();//菜单
void Open(preson **peo);//开辟空间函数
void Add(preson **pep);//添加联系人信息
int Found(preson **peo);//找到并打印联系人信息
void Delect(preson **peo);//删除联系人信息
void Alter(preson **peo);//修改联系人信息
void Show(preson **peo);//显示所有联系人
void Empty(preson **peo);//清空所有联系人
void Rank(preson *sp);//以姓氏排序
void Read(preson **peo);//读取文件
void Save_book(preson *sp);//将数据保存到磁盘之中


#endif