#include "HttpServer.hpp"
#include "Log.hpp"

void Usage(char *arg)
{
  cout<<arg<<":"<<"enter port error"<<endl;
  exit(0);
}

int main(int argc,char* argv[])
{
  if(argc !=1 && argc !=2)
  {
    Usage(argv[0]);
  }
  HttpServer *svr;
  
  if(argc==1)//没有参数，使用默认端口
  {
    svr=HttpServer::GetInstance(8888);
  }
  else
  {
    svr=HttpServer::GetInstance(atoi(argv[1]));
  }
  svr->Init();
  svr->Start();
  
  daemon(1,1);
  return 0;
}
