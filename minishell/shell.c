#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/wait.h>

#define SIZE 128

int main()
{
  char str[SIZE]={0};
  while(1)
  {
    printf("[myshell@VM-0-6-centos Shell]* ");//命令行提示

     str[0]=0;//字符数组清空，字符串/0结尾，因此第一个值为0就是清空
    
    fgets(str,SIZE,stdin);//从标准输入获取字符串

    str[strlen(str)-1]=0;//fgets会将/n带入

  char *args[SIZE]={0};//将命令解析出来

  args[0]=strtok(str," ");//以空格为分隔符解析出来
  
  int sub=1;
  do
  {
    args[sub]=strtok(NULL," ");
    if(!args[sub])
      break;//解析完毕
    sub++;
  }
  while(1);

  pid_t id=fork();

    if(id<0)
    {
      perror("fork error\n"); 
    }
    
    if(id==0)//child
    {
      execvp(args[0],args);//函数替换

      //调用失败
      printf("comman not found\n");
      exit(1);
    }

    //父进程不用判定，子进程调用替换函数后就被替换掉了
    
    int st;
    waitpid(id,&st,0);//以阻塞的方式等待；
    printf("status_code:%d\n",st>>8&&0xff);//打印子进程状态码
  }

return 0;
}
