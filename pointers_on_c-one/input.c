#include <stdio.h>
#include <windows.h>
#pragma warning (disable :4996)
//��дһ�����򣬴ӱ�׼�����ȡ�������룬ÿ�����붼Ҫ��ӡ����׼����ϣ�ǰ��Ҫ�����кš�
//��д�������ʱҪ��ͼ�ó����ܹ����������еĳ���û������

int main()
{
	int lable = 1;//�ж�����
	int row = 1;//����
	int ch;//����Ϊ�����Ǳ��⽫�ַ�����ΪEOF,ͬʱEOF��һ������ֵ�����ַ�λ���࣬���뱣֤ch���Խ�����
	while ((ch = getchar()) != EOF)//����һ���ַ�
	{
		if (lable)
		{
			printf("%d��", row);//��ӡ�к�
			lable = 0;//����һ��ֻ��ӡһ���к�
			row++;//�к�+1
		}
		putchar(ch);//��ӡ�ַ�
		if (ch == '\n')// '\n'���������Ѿ�������ϣ�������һ�У�
			lable = 1;//�ٴδ�ӡ�к�

	}
	system("pause");
	return 0;
}