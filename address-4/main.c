#include "preson.h"
int main()
{
	preson *peo=NULL;
	Open(&peo);//���ٿռ�
	Read(&peo);
	int quit = 0;
	int select = 0;
	while (!quit)
	{
		Menu();
		printf("��������ѡ����\n");
		scanf("%d", &select);
		switch (select)
		{
		case 1:
			system("cls");
			Add(&peo);//�����ϵ��,��ַ
			break;
		case 2:
			system("cls");
			Delect(&peo);//ɾ����ϵ��
			break;
		case 3:
			system("cls");
			Found(&peo);//����
			break;
		case 4:
			system("cls");
			Alter(&peo);//�޸�
			break;
		case 5:
			system("cls");
			Show(&peo);//��ʾ������ϵ��
			break;
		case 6:
			system("cls");
			Empty(&peo);//���
			break;
		case 7:
			system("cls");
			Rank(peo);//����������
			break;
		case 8:
			quit = 1;
			Save_book(peo);
			break;//�˳�
		default:
			printf("��������\n");
		}

	}
	system("pause");
	return 0;
}