#include "preson.h"
void Menu()
{
	printf("###1.�����ϵ����Ϣ############2.ɾ��ָ����ϵ����Ϣ####################\n");
	printf("###3.����ָ����ϵ����Ϣ#########4.�޸�ָ������Ϣ#######################\n");
	printf("###5.��ʾ������ϵ����Ϣ#########6.���������ϵ��#######################\n");
	printf("###7.����������������ϵ��#######8.�˳�#################################\n");
}

void Open(preson **peo)//���ٿռ亯��
{
	(*peo)= (preson*)malloc(sizeof(preson)+sizeof(list)* 2);//���������ڽṹ���в�ռ�ݿռ䣬�����Ҫ���⿪�ٿռ�
	if (*peo== NULL)
	{
		printf("���ٿռ�ʧ��\n");
			exit(-1);
	}
	(*peo)->size = 0;//��¼��ǰ��Ա��
	(*peo)->capacity = 2;//��¼�ܿռ�����
}
static void Judge(preson **peo)//���е�ǰ�ռ������ж�
{
	preson *cur = *peo;
	if (cur == NULL)//�����ϵ�˺������
	{
		Open(peo);
	}
	else if (cur->capacity == cur->size)//��ǰ��Ա���Ϳռ�������ȴ���ͨѶ¼��������
	{
		cur = (preson *)realloc(cur, sizeof(preson)+sizeof(list)*(cur->capacity) * 2);//�������˷ѣ��������鷳��ÿ�����ӵ�ǰ����������
		if (cur!= NULL)
		{
			printf("���ݳɹ�\n");
			cur->capacity = 2 * cur->capacity;
			printf("���ݳɹ�\n");
			*peo = cur;
		}
		else
			exit(-1);
	}
}

void Add(preson **peo)//�����ϵ����Ϣ
{
	Judge(peo);
	preson *sp = *peo;
	printf("��������ϵ������\n");
	scanf("%s", sp->people[sp->size].name);
	printf("��������ϵ���Ա�\n");
	scanf("%s", sp->people[sp->size].sex);
	printf("��������ϵ������\n");
	scanf("%d", &(sp->people[sp->size].age));//����һ��Ҫȡ��ַ����Ϊ������ʽ��Ӧ���������е�ĳ��ֵ
	printf("��������ϵ�˵绰\n");
	scanf("%s", sp->people[sp->size].tel);
	printf("��������ϵ�˵�ַ\n");
	scanf("%s", sp->people[sp->size].address);
	sp->size++;//��Ա����+1
}
int Found(preson **peo)//�ҵ�����ӡ��ϵ����Ϣ
{
	if (*peo== NULL)
	{
		Open(peo);
	}
	preson *sp = *peo;
	printf("����������\n");
	char name[20];//��ʱ�������������
	scanf("%s", name);
	int i = 0;
	for (i = 0; i < sp->size; i++)
	{
		if (strcmp(sp->people[i].name, name) == 0)//�ҵ���
		{
			printf("������%s �Ա�%s ���䣺%d �绰��%s ��ַ��%s\n", sp->people[i].name, sp->people[i].sex,
				sp->people[i].age, sp->people[i].tel, sp->people[i].address);
			break;
		}
	}
	if (i == sp->size || sp->size == 0)//����ѭ�������˻��߳�Ա����Ϊ��ʱ����û������ˣ�
	{
		printf("û������ˣ���ȷ�����������Ϣ�Ƿ���ȷ\n");
		return -1;
	}
	else	
		return i;
	

}
void Delect(preson **peo)//ɾ����ϵ����Ϣ
{
	int sub = Found(peo);
	preson *sp = *peo;
	if (sub!=-1)
	{
		while (sub < sp->size-1)
		{
			sp->people[sub] = sp->people[sub + 1];//���ǵ�
			sub++;
		}
		sp->size--;//��Ա��1
		printf("ɾ���ɹ�\n");
	}
}
void Alter(preson **peo)//�޸���ϵ����Ϣ
{
	int sub=Found(peo);
	preson *sp = *peo;
	int select = 0;
	int quit = 0;
	if (sub != -1)
	{
		while (!quit)
		{
			printf("��ѡ����Ҫ�޸���ϵ��ʲô��Ϣ\n");
			printf("##1.����#############2.�Ա�#######\n");
			printf("##3.����#############4.�绰#######\n");
			printf("###5.��ַ############6.�˳�#######\n");
			printf("#################################\n");
			scanf("%d", &select);
			switch (select)
			{
			case 1:
				printf("����������\n");
				scanf("%s", sp->people[sub].name);
				system("cls");
				break;
			case 2:
				printf("�������Ա�\n");
				scanf("%s", sp->people[sub].sex);
				system("cls");
				break;
			case 3:
				printf("����������\n");
				scanf("%d", &(sp->people[sub].age));
				system("cls");
				break;
			case 4:
				printf("������绰\n");
				scanf("%s", sp->people[sub].tel);
				system("cls");
				break;
			case 5:
				printf("�������ַ\n");
				scanf("%s", sp->people[sub].address);
				system("cls");
				break;
			case 6:
				quit = 1;
				break;
			default:
				printf("����������������\n");
			}
		}
		printf("�޸ĳɹ�\n");
	}
}
void Show(preson **peo)//��ʾ������ϵ��
{

	if (*peo == NULL)//��ϵ���б�Ϊ��
	{
		Open(peo);
	}
	preson *sp = *peo;
	if (sp->size == 0)
		printf("��ϵ������Ϊ��\n");
	for (int i = 0; i < sp->size; i++)
	{
		printf("������%s �Ա�%s ���䣺%d �绰��%s ��ַ��%s\n", sp->people[i].name, sp->people[i].sex,
		sp->people[i].age, sp->people[i].tel, sp->people[i].address);
	}
}
void Empty(preson **peo)//���������ϵ��
{
	free(*peo);
	*peo = NULL;//��ֹҰָ��
	printf("��ϵ�������\n");
}


static int Compare(const list *cur, const list*next)//����ص�����
{	
	return (strcmp(cur->name, next->name));
}
void Rank(preson *sp)//����������
{
	if (sp == NULL || sp->size == 0)
		printf("��ϵ������Ϊ��\n");
	else
		qsort(sp->people[0].name,sp->size,sizeof(list),Compare);
}

void Save_book(preson *sp)//�����ݱ��浽����֮��
{	
	assert(sp);
	FILE *fp = fopen("code.log", "wb");
	if (fp == NULL)
	{
		printf("���ļ�ʧ��\n");
	}
	fwrite(sp, sizeof(preson), 1, fp);//�Ƚ�һ���ṹ���С����
	fwrite(sp->people, sizeof(list), sp->size, fp);//�ٽ���������Ĵ�С����
		printf("����ɹ�\n");
	fclose(fp);
}

void Read(preson **peo)//��ȡ�ļ�
{
	assert(*peo);
	preson prev;
	FILE *fp = fopen("code.log", "rb");
	if (fp == NULL)
	{
		printf("�ļ���ʧ��\n");
		return;
	}
	fread(&prev, sizeof(preson), 1, fp);//ע��fread�ǽ���ȡ�����ݴ洢��prevָ����ڴ�飬��˲��ܶ���һ����ָ�룻

	if (prev.capacity != 0)//�ļ����е����ݲ���Ϊ��
	{
		int count = sizeof(preson)+(prev.size)*sizeof(list);
		*peo = (preson *)malloc(count);//���ٺÿռ�
		fread((*peo)->people,sizeof(list),prev.size,fp);
		(*peo)->capacity = prev.size;
		(*peo)->size = prev.size;
		fclose(fp);
		printf("���ݶ�ȡ�ɹ�\n");
	}
	else
		printf("�ļ�����û������\n");

}