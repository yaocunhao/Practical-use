#include "test.h"

int main()
{
	address book;//����ͨѶ¼book
	Open(&book);//��ͨѶ¼���ٿռ�
	int select = 0;
	while (!select)
	{
		Menu();
		printf("���������ѡ��\n");
		scanf("%d", &select);
		getchar();
		switch (select)
		{
		case 1:
			system("cls");
			Add(&book);//�����ϵ��,��ַ
			select = 0;
			break;
		case 2:
			system("cls");
			Delect(&book);//ɾ����ϵ��
			select = 0;
			break;
		case 3:
			system("cls");
			Found(&book);//����
			select = 0;
			break;
		case 4:
			system("cls");
			Alter(&book);//�޸�
			select = 0;
			break;
		case 5:
			system("cls");
			Show(&book);//��ʾ
			select = 0;
			break;
		case 6:
			system("cls");
			Empty(&book);//���
			select = 0;
			break;
		case 7:
			system("cls");
			Rank(&book);//����������
			select = 0;
			break;
		case 8:
			select = 1;
			Save_book(&book);
			break;//�˳�
		default:
			printf("��������\n");
			select = 0;
		}
	}
	system("pause");
	return 0;
}