//输出对应的列标字符串内容
#include <stdio.h>
#include <windows.h>
#pragma warning (disable :4996)

#define MAX_NUM 20
#define MAX_STRING 100
int ColumnNum(int arr[])
{
	int n = 0;
	printf("请输入所需要读取的列,以负数结尾\n");
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
	int count = 0;//用以计数
	int curnum = 0;//记录当前元素个数;
	for (int i = 0; i < size; i=i+2)
	{
		if (curnum >= MAX_STRING)
			break;//数组满了；
		count = arr[i + 1] - arr[i]+1;//两个数组元素之间的元素个数
		if (count>len)//大于整个元素长度
			continue;
		if (curnum + count > MAX_STRING)//空间不够
		{
			strncpy(newstr + curnum, str+arr[i], MAX_STRING - curnum);
			curnum += MAX_STRING - curnum;
		}
		else
		{
			strncpy(newstr + curnum, str + arr[i], count);//复制字符过去
			curnum += count;
		}
	}
	newstr[curnum] = '\0';//结尾标志‘\0’
	printf("%s\n", newstr);
}

int main()
{
	int arr[MAX_NUM];
	char str[MAX_STRING];
	int size = ColumnNum(arr);//输入列标,获得长度
	printf("请输入字符串\n");
	getchar();
	gets(str);//输入字符串，注意越界
	int len = strlen(str);
	Print_string(arr,str,size,len);//读取新的字符串；
	system("pause");
	return 0;
}