#include <stdio.h>
#include <windows.h>
#pragma warning (disable :4996)
//编写一个程序，从标准输入读取几行输入，每行输入都要打印到标准输出上，前面要加上行号。
//编写这个程序时要试图让程序能够处理输入行的长度没有限制

int main()
{
	int lable = 1;//判断条件
	int row = 1;//行数
	int ch;//定义为整形是避免将字符解释为EOF,同时EOF是一个整形值，比字符位数多，必须保证ch可以接收它
	while ((ch = getchar()) != EOF)//输入一大串字符
	{
		if (lable)
		{
			printf("%d：", row);//打印行号
			lable = 0;//控制一行只打印一次行号
			row++;//行号+1
		}
		putchar(ch);//打印字符
		if (ch == '\n')// '\n'代表这行已经输入完毕，进入下一行；
			lable = 1;//再次打印行号

	}
	system("pause");
	return 0;
}