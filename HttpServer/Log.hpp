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

#define Notice  1 //正常
#define Warning 2 //警告
#define Error   3 //错误
#define Fatal   4 //严重错误

#define LOG(level,message) log(#level,message,__FILE__,__LINE__)

void log(std::string level,std::string message,std::string filename,size_t line)
{
  //获得时间
  struct timeval time;
  gettimeofday(&time,NULL);
  
  std::string log_string="[";
  log_string+=level;
  log_string+="]";
  log_string+="[";
  log_string+=message;
  log_string+="]";
  log_string+="[";
  log_string+=std::to_string(time.tv_sec);
  log_string+="]";
  log_string+="[";
  log_string+=filename;
  log_string+="]";
  log_string+="[";
  log_string+=std::to_string(line);
  log_string+="]";
  log_string+="\r\n";
  
  //将日志信息写入到文件之中
  int fd=open("log.txt",O_CREAT|O_APPEND|O_RDWR,0666);
  write(fd,log_string.c_str(),log_string.size());
  close(fd);
  
  //将日志信息打印出来(方便调试)
  //cout<<"["<<level<<"]"<<"["<<message<<"]"<<"["<<time.tv_sec<<"]"<<"["<<filename<<"]"<<"["<<line<<"]"<<endl;
}
