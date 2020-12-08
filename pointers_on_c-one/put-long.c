#include <stdio.h>
#include <windows.h>
#pragma warning (disable :4996)
//编写一个程序，一行行的读取输入行，直至到达文件尾。算出每行输入行的长度，然后把最长的哪行打印出来。
//为了简单起见，你可以假定所有的输入行均不超过1000个字符

#define MAX 1000
int main()
{
	char str[MAX][MAX];
	int longsub = 0;//记录最长的数组下标
	for (int i = 0; i < MAX; i++)
	{
		gets(str[i]);
		if (strlen(str[i])>strlen(str[longsub]))
			longsub = i;
		if ((strcmp(str[i], "-1") == 0))//以-1为结束标志位
			break;
	}
	printf("%s\n", str[longsub]);
	system("pause");
	return 0;
}