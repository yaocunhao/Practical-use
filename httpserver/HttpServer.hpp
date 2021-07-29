#pragma once
#include "common.hpp"
#include "Sock.hpp"
#include "Protocol.hpp"
#include "ThreadPool.hpp"

class HttpServer
{
  private:
    int port;
    int lsock;
    ThreadPool *tp;
    
    static HttpServer *HttpPtr;
    static pthread_mutex_t mtx;

    HttpServer(int _port)
    :port(_port)
    ,lsock(-1)
    ,tp(nullptr)
    {}

  public:
    
    static HttpServer* GetInstance(int port)
    {
      if(HttpPtr==nullptr)
      {
        pthread_mutex_lock(&mtx);
        if(HttpPtr==nullptr)
        {
          HttpPtr=new HttpServer(port);
        }
        pthread_mutex_unlock(&mtx);
      }
      return HttpPtr;
    }

    void Init()
    {
      lsock = Sock::Socket();//获取套接字
      Sock::SetSockOpt(lsock);//端口复用
      Sock::Bind(lsock,port);//绑定
      Sock::Listen(lsock);//侦听套接字
      
      //接入线程池
      tp=new ThreadPool;
      tp->InitPool();
    }

    void Start()
    {
      while(true)
      {
        int sock=Sock::Accept(lsock);
        if(sock < 0)
        {
          continue ;
        }
        LOG(Notice,"get a new link");
        
        //有链接来了,创建一个任务
        Task *tk=new Task(sock);
        tp->PushTask(tk);//将任务放入队列之中
      }
    }


    ~HttpServer()
    {
      if(lsock >=0 )
        close(lsock);
    }

};

HttpServer* HttpServer::HttpPtr=nullptr;
pthread_mutex_t HttpServer::mtx=PTHREAD_MUTEX_INITIALIZER;

