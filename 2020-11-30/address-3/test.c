#include "test.h"
void Menu()
{
	printf("###1.�����ϵ����Ϣ############2.ɾ��ָ����ϵ����Ϣ####################\n");
	printf("###3.����ָ����ϵ����Ϣ#########4.�޸�ָ������Ϣ#######################\n");
	printf("###5.��ʾ������ϵ����Ϣ#########6.���������ϵ��#######################\n");
	printf("###7.����������������ϵ��#######8.�˳�#################################\n");

}
void Save_book(address *sp)
{
	FILE *fp = fopen("book.log", "wb");
	if (fp == NULL)
	{
		return ;
	}
	fwrite(sp, sizeof(address), 1, fp);//����ͨѶ¼
	fwrite(sp->call, sizeof(list), sp->capacity, fp);//���濪�ٳ����Ŀռ�֮�е�����	
	fclose(fp);
}

void Open(address *p)//���ٿռ亯��
{
	FILE *fp = fopen("book.log", "rb");
	if (fp == NULL)
	{
		p->call = (list *)malloc(TWO*sizeof(list));//��TWO=2����������Ϣ���ָ������ڴ�
		if (p->call != NULL)
		{
			p->count = 0;//��¼�洢�����ļ�����count
			p->capacity = 2;//���ٿռ�������������ÿ�ο��ٿռ��ṩ2�δ洢���᣻
		}
	}
	else
	{
		printf("��ȡ������\n");
		address temp;
		fread(&temp, sizeof(address), 1, fp);//��ȡͨѶ¼address
		int size = sizeof(list)*temp.capacity;//��ȡ�����Ŀռ��С
		p->capacity = temp.capacity;
		p->count = temp.count;	
		p->call= (list *)malloc(size);//���ٿռ�
		if (p->call == NULL)
		{
			exit(1);
		}
		fread(p->call, sizeof(list), p->capacity, fp);
		fclose(fp);
		printf("��ȡ�ɹ�\n");
	}

}

int Comp(char name[],address *sp)//Ѱ�Ҷ�Ӧ�����֣�
{
	int i = 0;
	for (i = 0; i < sp->count; i++)
	{
		if ((strcmp(sp->call[i].name, name)) == 0)
			return i;//�����ҵ����±ꣻ
	}
	return -1;//�Ҳ�������-1
}


static void Judge(address *sp)//���пռ������ж�
{
	if (sp->capacity == 0)//�ɸ������ͨѶ¼���¿��ٿռ䣻
	{
		sp->call = (list *)malloc(TWO*sizeof(list));//��TWO=2����������Ϣ���ָ������ڴ�
		if (sp->call != NULL)
		{
			sp->count = 0;//��¼�洢�����ļ�����count
			sp->capacity = 2;//���ٿռ�������������ÿ�ο��ٿռ��ṩ2�δ洢���᣻
		}
	}
	else if (sp->count == sp->capacity)//�ϴο��ٵ��ڴ�����
	{
		list *dst = (list *)realloc(sp->call,(sp->capacity+TWO)*sizeof(list));//���������ռ�;
		if (dst != NULL)
		{
			sp->call = dst;//�����ٳɹ��ĵ�ַ���ظ�ԭָ��
			sp->capacity += 2;//�ռ�����������+2��
		}
	}


}
void Add(address *p)//���Ӻ���
{
	Judge(p);//���пռ��ж��Ƿ��㹻��
	printf("��������ϵ������\n");

	scanf("%s", p->call[p->count].name);
	printf("��������ϵ���Ա�\n");
	scanf("%s", p->call[p->count].sex);
	printf("��������ϵ������\n");
	scanf("%d", &(p->call[p->count].age));
	printf("��������ϵ�˵绰\n");
	scanf("%s", p->call[p->count].tel);
	printf("��������ϵ��סַ\n");
	scanf("%s", p->call[p->count].address);
	p->count++;//������+1��
}

void Delect(address *p)//ɾָ���˺���
{
	int i = 0;//���ղ��ҵ���
	char name[20];
	printf("������Ҫɾ����ָ���˵�����\n");
	scanf("%s", name);
	i = Comp(name,p);
	if (i == -1)
	{
		printf("���޴���\n");
	}
	else
	{
		Judge(p);//���пռ��ж�������Խ�磻
		for (i; i < p->count; i++)//���ǣ����ú�������ݽ�ǰ�������ȫ�����ǣ�
		{
			p->call[i] = p->call[i + 1];
		}
		p->count--;//��Ա��1����������1
		printf("ɾ���ɹ�\n");
	}
	
}

int Found(address *p)//���ҳ�Ա
{
	char name[NUM];
	printf("��������Ҫ���ҵ�����\n");
	scanf("%s", name);
	int j = Comp(name,p);//���շ���ֵ���ҵ�����ֵΪ��Ӧ�ĳ�Ա�±�
	if (j == -1)
	{
		printf("���޴���\n");
	}
	else
		printf("������%s �Ա�%s ����:%d �绰��%s סַ��%s\n", p->call[j].name,
		p->call[j].sex,p->call[j].age,p->call[j].tel,p->call[j].address);

	return j;//����Alter��������
}

void Alter(address *p)//�޸�ָ������Ϣ
{
	int select = 0;
	char newname[NUM];//��Ҫ�޸ĵ�����
	char newsex[NUM];//��Ҫ�޸ĵ��Ա�
	int  neweag;//��Ҫ�޸ĵ�����
	char newtel[NUM];//��Ҫ�޸ĵĵ绰
	char newaddress[NUM];//��Ҫ�޸ĵĵ�ַ;
	int j=Found(p);//���ҵ������
	if (j !=-1)
	{
		while (!select)
		{
			printf("��ѡ����Ҫ�޸ĵ���Ϣ\n");
			printf("********************************\n");
			printf("**1.����**********2.�Ա�*********\n");
			printf("**3.����**********4.�绰*********\n");
			printf("**5.סַ**********6.�˳�*********\n");
			printf("********************************\n");
			scanf("%d", &select);
			switch (select)
			{
			case 1:
				printf("�������µ�����\n");
				scanf("%s", newname);
				strcpy(p->call[j].name, newname);
				select = 0;
				break;
			case 2:
				printf("�������µ��Ա�\n");
				scanf("%s", newsex);
				strcpy(p->call[j].sex, newsex);
				select = 0;
				break;
			case 3:
				printf("�������µ�����\n");
				scanf("%d", &neweag);
				p->call[j].age = neweag;
				select = 0;
				break;
			case 4:
				printf("�������µĵ绰\n");
				scanf("%s", newtel);
				strcpy(p->call[j].tel, newtel);
				select = 0;
				break;
			case 5:
				printf("�������µ�סַ\n");
				scanf("%s", newaddress);
				strcpy(p->call[j].address, newaddress);
				select = 0;
				break;
			case 6:
				select = 1;
				break;
			default:
				printf("�������󣬴�������\n");
				select = 0;
				break;
			}
		}
	}

}

void Show(address *p)//��ʾ��������Ϣ
{
	if (p->count == 0)
		printf("��ϵ���б�Ϊ��\n");
	for (int j = 0; j < p->count; j++)//����һ�������һ������Ϣ��������ӡ����
	{
		printf("������%s �Ա�%s ����:%d �绰��%s סַ��%s\n", p->call[j].name,
			p->call[j].sex, p->call[j].age, p->call[j].tel, p->call[j].address);
	}
}

void Empty(address *p)//���ͨѶ¼			
{
	if (p->count == 0)
	{
		printf("��ϵ���б�Ϊ��\n");
	}
	else
	{
		free(p->call);//�ͷŵ�������ڴ�
		p->count = 0;//����������
		p->capacity=0;//��������������
		printf("����ɹ�\n");
	}
}

static int Compar(const list *x, const list *y)//����ص�����
{
	return strcmp(x->name, y->name);
}
void Rank(address *p)
{
		if (p->count==0)
		{
			printf("��ϵ���б�Ϊ��\n");
		}

	else
		qsort(p->call->name, p->count, sizeof(list), Compar);//����qsort��������
}
