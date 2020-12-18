#include "preson.h"
int main()
{
	preson *peo=NULL;
	Open(&peo);//开辟空间
	Read(&peo);
	int quit = 0;
	int select = 0;
	while (!quit)
	{
		Menu();
		printf("请根据序号选择功能\n");
		scanf("%d", &select);
		switch (select)
		{
		case 1:
			system("cls");
			Add(&peo);//添加联系人,传址
			break;
		case 2:
			system("cls");
			Delect(&peo);//删除联系人
			break;
		case 3:
			system("cls");
			Found(&peo);//查找
			break;
		case 4:
			system("cls");
			Alter(&peo);//修改
			break;
		case 5:
			system("cls");
			Show(&peo);//显示所有联系人
			break;
		case 6:
			system("cls");
			Empty(&peo);//清空
			break;
		case 7:
			system("cls");
			Rank(peo);//以姓氏排序
			break;
		case 8:
			quit = 1;
			Save_book(peo);
			break;//退出
		default:
			printf("输入有误\n");
		}

	}
	system("pause");
	return 0;
}