#include <stdio.h>
#include <windows.h>
#pragma warning (disable :4996)
//��дһ������һ���еĶ�ȡ�����У�ֱ�������ļ�β�����ÿ�������еĳ��ȣ�Ȼ���������д�ӡ������
//Ϊ�˼����������Լٶ����е������о�������1000���ַ�

#define MAX 1000
int main()
{
	char str[MAX][MAX];
	int longsub = 0;//��¼��������±�
	for (int i = 0; i < MAX; i++)
	{
		gets(str[i]);
		if (strlen(str[i])>strlen(str[longsub]))
			longsub = i;
		if ((strcmp(str[i], "-1") == 0))//��-1Ϊ������־λ
			break;
	}
	printf("%s\n", str[longsub]);
	system("pause");
	return 0;
}