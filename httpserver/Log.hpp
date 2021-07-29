#pragma once
#include "common.hpp"


//退出码
enum ERR 
{
  SocketErr,
  BindErr,
  ListenErr,
  AcceptErr,
  ArgErr
};

#define Notice  1
#define Warning 2
#define Error   3
#define Fatal   4

#define LOG(level,message) log(#level,message,__FILE__,__LINE__)

void log(std::string level,std::string message,std::string filename,size_t line)
{
  //获得时间
  struct timeval time;
  gettimeofday(&time,NULL);
  
  int fd=open("log.txt",O_CREAT|O_APPEND);
  cout<<"["<<level<<"]"<<"["<<message<<"]"<<"["<<time.tv_sec<<"]"<<"["<<filename<<"]"<<"["<<line<<"]"<<endl;
  

}
