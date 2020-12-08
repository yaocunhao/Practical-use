#include <stdio.h>
#include <windows.h>
#pragma warning (disable :4996)
//编写一个程序，从标准输入读入一些字符，并且把它写入到标准输出上。它同时应该计算checksum的值，并写在字符的后面
int main()
{
	int ch = 0;
	int checksum = 0;
	while ((ch = getchar() != '\n'))
	{
		checksum += ch;
		putchar(ch);
		printf("%d\n", ch);
	}
	system("pause");
	return 0;
}