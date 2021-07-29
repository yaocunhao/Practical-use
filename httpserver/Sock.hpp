#pragma once
#include "common.hpp"
#include "Log.hpp"
#define NUM 5



class Sock
{
  public:

    static int Socket()
    {
      int sock=socket(AF_INET,SOCK_STREAM,0);
      if(sock < 0)//创建套接字失败
      {
        //将原有写入日志
        LOG(Fatal,"socket create error");
        exit(SocketErr);
      }
        return sock;
    }

    static void Bind(int sock,int port)
    {
      struct sockaddr_in local;
      local.sin_family=AF_INET;
      local.sin_port=htons(port);
      local.sin_addr.s_addr=htonl(INADDR_ANY);
        
      if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
      {
        LOG(Fatal,"bind error");
        exit(BindErr);
      }
    }

    static void Listen(int sock)
    {
      if(listen(sock,NUM)< 0)
      {
        LOG(Fatal,"listen error");
        exit(ListenErr);
      }
    }

    static int Accept(int lsock)
    {
      struct sockaddr_in peer;
      socklen_t len=sizeof(peer);
      size_t sock = accept(lsock,(struct sockaddr*)&peer,&len);

      if(sock < 0)
      {
        LOG(Warning,"accept error");
      }

      return sock;
    }

    //端口复用
    static void SetSockOpt(int sock)
    {
      int op=1;
      setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));
    }

    //读取一行内容
    static void GetLine(int sock,std::string &line)
    {
      //换行符有多种情况 \n ,\r,\r\n
      //按字符读取，便于处理换行符
      
      char c='c';

      while(c!='\n')
      {
        ssize_t size=recv(sock,&c,1,0);

        if(size > 0)
        {
          if(c=='\r')//读到了末尾，对终止符进行处理
          {
            //MSG_PEEK 并不会将数据读取上来，只是进行窥探
            ssize_t _size=recv(sock,&c,1,MSG_PEEK);
            
            if(_size > 0 && c=='\n')//分割符号是\r\n
            {
                recv(sock,&c,1,0);
            }
            else//最后都变成\n
            {
              c='\n';
            }
          }

        }
        //读取到的内容是常规字符
        //读取到的内容是\n
        //不管什么情况，终止符都变成了\n
        if(c!='\n')
        line.push_back(c);
      }

    }
};
