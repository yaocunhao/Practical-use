#include <stdio.h>
#include <windows.h>
#pragma warning (disable :4996)
//��дһ�����򣬴ӱ�׼�������һЩ�ַ������Ұ���д�뵽��׼����ϡ���ͬʱӦ�ü���checksum��ֵ����д���ַ��ĺ���
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