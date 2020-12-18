#include "preson.h"
void Menu()
{
	printf("###1.添加联系人信息############2.删除指定联系人信息####################\n");
	printf("###3.查找指定联系人信息#########4.修改指定人信息#######################\n");
	printf("###5.显示所有联系人信息#########6.清空所有联系人#######################\n");
	printf("###7.以名字排序所有联系人#######8.退出#################################\n");
}

void Open(preson **peo)//开辟空间函数
{
	(*peo)= (preson*)malloc(sizeof(preson)+sizeof(list)* 2);//柔性数组在结构体中不占据空间，因此需要额外开辟空间
	if (*peo== NULL)
	{
		printf("开辟空间失败\n");
			exit(-1);
	}
	(*peo)->size = 0;//记录当前成员数
	(*peo)->capacity = 2;//记录总空间容量
}
static void Judge(preson **peo)//进行当前空间容量判定
{
	preson *cur = *peo;
	if (cur == NULL)//清空联系人后再添加
	{
		Open(peo);
	}
	else if (cur->capacity == cur->size)//当前成员数和空间容量相等代表通讯录容量满了
	{
		cur = (preson *)realloc(cur, sizeof(preson)+sizeof(list)*(cur->capacity) * 2);//增多了浪费，增少了麻烦，每次增加当前容量的两倍
		if (cur!= NULL)
		{
			printf("增容成功\n");
			cur->capacity = 2 * cur->capacity;
			printf("增容成功\n");
			*peo = cur;
		}
		else
			exit(-1);
	}
}

void Add(preson **peo)//添加联系人信息
{
	Judge(peo);
	preson *sp = *peo;
	printf("请输入联系人姓名\n");
	scanf("%s", sp->people[sp->size].name);
	printf("请输入联系人性别\n");
	scanf("%s", sp->people[sp->size].sex);
	printf("请输入联系人年龄\n");
	scanf("%d", &(sp->people[sp->size].age));//这里一定要取地址，因为这个表达式对应的是数组中的某个值
	printf("请输入联系人电话\n");
	scanf("%s", sp->people[sp->size].tel);
	printf("请输入联系人地址\n");
	scanf("%s", sp->people[sp->size].address);
	sp->size++;//成员个数+1
}
int Found(preson **peo)//找到并打印联系人信息
{
	if (*peo== NULL)
	{
		Open(peo);
	}
	preson *sp = *peo;
	printf("请输入名字\n");
	char name[20];//临时储存输入的名字
	scanf("%s", name);
	int i = 0;
	for (i = 0; i < sp->size; i++)
	{
		if (strcmp(sp->people[i].name, name) == 0)//找到了
		{
			printf("姓名：%s 性别：%s 年龄：%d 电话：%s 地址：%s\n", sp->people[i].name, sp->people[i].sex,
				sp->people[i].age, sp->people[i].tel, sp->people[i].address);
			break;
		}
	}
	if (i == sp->size || sp->size == 0)//上面循环跑完了或者成员个数为零时代表没有这个人；
	{
		printf("没有这个人，请确认你的输入信息是否正确\n");
		return -1;
	}
	else	
		return i;
	

}
void Delect(preson **peo)//删除联系人信息
{
	int sub = Found(peo);
	preson *sp = *peo;
	if (sub!=-1)
	{
		while (sub < sp->size-1)
		{
			sp->people[sub] = sp->people[sub + 1];//覆盖掉
			sub++;
		}
		sp->size--;//成员减1
		printf("删除成功\n");
	}
}
void Alter(preson **peo)//修改联系人信息
{
	int sub=Found(peo);
	preson *sp = *peo;
	int select = 0;
	int quit = 0;
	if (sub != -1)
	{
		while (!quit)
		{
			printf("请选择你要修改联系人什么信息\n");
			printf("##1.姓名#############2.性别#######\n");
			printf("##3.年龄#############4.电话#######\n");
			printf("###5.地址############6.退出#######\n");
			printf("#################################\n");
			scanf("%d", &select);
			switch (select)
			{
			case 1:
				printf("请输入姓名\n");
				scanf("%s", sp->people[sub].name);
				system("cls");
				break;
			case 2:
				printf("请输入性别\n");
				scanf("%s", sp->people[sub].sex);
				system("cls");
				break;
			case 3:
				printf("请输入年龄\n");
				scanf("%d", &(sp->people[sub].age));
				system("cls");
				break;
			case 4:
				printf("请输入电话\n");
				scanf("%s", sp->people[sub].tel);
				system("cls");
				break;
			case 5:
				printf("请输入地址\n");
				scanf("%s", sp->people[sub].address);
				system("cls");
				break;
			case 6:
				quit = 1;
				break;
			default:
				printf("输入错误请从新输入\n");
			}
		}
		printf("修改成功\n");
	}
}
void Show(preson **peo)//显示所有联系人
{

	if (*peo == NULL)//联系人列表为空
	{
		Open(peo);
	}
	preson *sp = *peo;
	if (sp->size == 0)
		printf("联系人名单为空\n");
	for (int i = 0; i < sp->size; i++)
	{
		printf("姓名：%s 性别：%s 年龄：%d 电话：%s 地址：%s\n", sp->people[i].name, sp->people[i].sex,
		sp->people[i].age, sp->people[i].tel, sp->people[i].address);
	}
}
void Empty(preson **peo)//清空所有联系人
{
	free(*peo);
	*peo = NULL;//防止野指针
	printf("联系人已清空\n");
}


static int Compare(const list *cur, const list*next)//排序回调函数
{	
	return (strcmp(cur->name, next->name));
}
void Rank(preson *sp)//以姓氏排序
{
	if (sp == NULL || sp->size == 0)
		printf("联系人链表为空\n");
	else
		qsort(sp->people[0].name,sp->size,sizeof(list),Compare);
}

void Save_book(preson *sp)//将数据保存到磁盘之中
{	
	assert(sp);
	FILE *fp = fopen("code.log", "wb");
	if (fp == NULL)
	{
		printf("打开文件失败\n");
	}
	fwrite(sp, sizeof(preson), 1, fp);//先将一个结构体大小存入
	fwrite(sp->people, sizeof(list), sp->size, fp);//再将柔型数组的大小存入
		printf("保存成功\n");
	fclose(fp);
}

void Read(preson **peo)//读取文件
{
	assert(*peo);
	preson prev;
	FILE *fp = fopen("code.log", "rb");
	if (fp == NULL)
	{
		printf("文件打开失败\n");
		return;
	}
	fread(&prev, sizeof(preson), 1, fp);//注意fread是将读取的内容存储于prev指向的内存块，因此不能定义一个空指针；

	if (prev.capacity != 0)//文件夹中的内容不能为空
	{
		int count = sizeof(preson)+(prev.size)*sizeof(list);
		*peo = (preson *)malloc(count);//开辟好空间
		fread((*peo)->people,sizeof(list),prev.size,fp);
		(*peo)->capacity = prev.size;
		(*peo)->size = prev.size;
		fclose(fp);
		printf("数据读取成功\n");
	}
	else
		printf("文件夹中没有内容\n");

}