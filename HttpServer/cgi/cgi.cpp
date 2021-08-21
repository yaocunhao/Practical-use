#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <unistd.h>


//替换为计算机程序 -> 输出加减乘除的结果
void CalData(std::string &qs)
{

  std::string part1,part2;
  int x=0,y=0;

  size_t  pos=qs.find("&");//两个参数之间用&进行链接，因此用&进行分开
  if(pos!=std::string::npos)
  {
    part1=qs.substr(0,pos);
    part2=qs.substr(pos+1);
  }
  
  //参数 a=100 -> 找到等号，等号后面就是参数 -> 参数为字符串，将参数转为整数
  size_t pos1=part1.find("=");
  size_t pos2=part2.find("=");

  if(pos1!=std::string::npos)
  {
    x=stoi(part1.substr(pos1+1));
  }
  if(pos2!=std::string::npos)
  {
    y=stoi(part2.substr(pos2+1));
  }
  
  //输出结果
  std::cout<<"<html>"<<std::endl;
  std::cout<<"<h1>"<<x<<"+"<<y<<"="<<(x+y)<<"</h1>"<<std::endl;
  std::cout<<"<h1>"<<x<<"-"<<y<<"="<<(x-y)<<"</h1>"<<std::endl;
  std::cout<<"<h1>"<<x<<"*"<<y<<"="<<(x*y)<<"</h1>"<<std::endl;
  std::cout<<"<h1>"<<x<<"/"<<y<<"="<<(x/y)<<"</h1>"<<std::endl;
  std::cout<<"</html>"<<std::endl;
}

int main()
{
  std::string method;//方法
  std::string query_string;//参数

  if(getenv("METHOD"))//获得方法
  {
    method=getenv("METHOD");
  }
  else
  {
    return 1;
  }

  //判断是什么方法,get方法从环境变量中获取参数，post方法从管道中获取
  if(strcasecmp(method.c_str(),"GET")==0)//get方法的参数存储在环境变量中
  {
    query_string=getenv("QUERY_STRING");
  }
  else if(strcasecmp(method.c_str(),"POST")==0)//POST方法的参数在管道之中
  {
    //不确定管道的文件描述符是那个
    //约定从0读取，从1写入（在替换之间进行dup2重定向）
    
    ssize_t cl=atoi(getenv("CONTENT-LENGTH"));//获得长度
    char c='C';
    while(cl)//读取参数
    {
      read(0,&c,1);
      query_string.push_back(c);
      cl--;
    }
  }
    //进行数据的处理
    CalData(query_string); 

  return 0;
}
