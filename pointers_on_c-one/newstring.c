//�����Ӧ���б��ַ�������
#include <stdio.h>
#include <windows.h>
#pragma warning (disable :4996)

#define MAX_NUM 20
#define MAX_STRING 100
int ColumnNum(int arr[])
{
	int n = 0;
	printf("����������Ҫ��ȡ����,�Ը�����β\n");
	while (n < MAX_NUM)
	{
		scanf("%d", &arr[n]);
		if (arr[n] < 0)
			break;
		n++;
	}
	return n;
}

void Print_string(int arr[], char str[],int size,int len)
{
	char newstr[MAX_STRING] = {0};
	int count = 0;//���Լ���
	int curnum = 0;//��¼��ǰԪ�ظ���;
	for (int i = 0; i < size; i=i+2)
	{
		if (curnum >= MAX_STRING)
			break;//�������ˣ�
		count = arr[i + 1] - arr[i]+1;//��������Ԫ��֮���Ԫ�ظ���
		if (count>len)//��������Ԫ�س���
			continue;
		if (curnum + count > MAX_STRING)//�ռ䲻��
		{
			strncpy(newstr + curnum, str+arr[i], MAX_STRING - curnum);
			curnum += MAX_STRING - curnum;
		}
		else
		{
			strncpy(newstr + curnum, str + arr[i], count);//�����ַ���ȥ
			curnum += count;
		}
	}
	newstr[curnum] = '\0';//��β��־��\0��
	printf("%s\n", newstr);
}

int main()
{
	int arr[MAX_NUM];
	char str[MAX_STRING];
	int size = ColumnNum(arr);//�����б�,��ó���
	printf("�������ַ���\n");
	getchar();
	gets(str);//�����ַ�����ע��Խ��
	int len = strlen(str);
	Print_string(arr,str,size,len);//��ȡ�µ��ַ�����
	system("pause");
	return 0;
}