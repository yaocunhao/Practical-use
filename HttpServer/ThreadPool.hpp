#pragma once

#include "common.hpp"
#include "Protocol.hpp"

typedef void (*_handle)(int);

class Task//接入任务 -> 套接字、方法
{
  private:
    int sock;
    _handle handle;

  public:
    Task(int _sock)
      :sock(_sock)
      ,handle(Entry::HandlerHttp)
  {}

    void Run()//传入套接字，通过Entry中的函数处理任务
    {
      handle(sock);
    }
};

class ThreadPool
{
  private:
    std::queue<Task*>qe;
    int num;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
  
  public:

    ThreadPool(int _num=3)//默认给3个线程
      :num(_num)
    {}

    void Lock()
    {
      pthread_mutex_lock(&mtx);
    }
    
    void ULock()
    {
      pthread_mutex_unlock(&mtx);
    }

    void Wait()
    {
      pthread_cond_wait(&cond,&mtx);
    }

    void WakeUp()
    {
      pthread_cond_signal(&cond);
    }

    bool IsEmpty()
    {
      return qe.empty();
    }

    static void *RunFunc(void *arg)
    {
      ThreadPool *tp=(ThreadPool*)arg;
      while(true)
      {
        tp->Lock();
        while(tp->IsEmpty())//为空则进入等待状态
        {
          tp->Wait();
        }
        Task *task=tp->qe.front();//取任务出来
        tp->qe.pop();
        tp->ULock();

        task->Run();//处理任务
        delete task;//任务处理完毕
      }
    }

    void InitPool()//初始化线程池
    {
      pthread_mutex_init(&mtx,nullptr);
      pthread_cond_init(&cond,nullptr);

      pthread_t tid;
      for(int i=0;i < num;i++)
      {
        pthread_create(&tid,nullptr,RunFunc,this);//创建线程去执行任务
        pthread_detach(tid);
      }
    }

    void PushTask(Task *task)
    {
      Lock();
      qe.push(task);
      ULock();
      WakeUp();//放入任务唤醒线程去执行任务
    }



    ~ThreadPool()
    {
      pthread_mutex_destroy(&mtx);
      pthread_cond_destroy(&cond);
    }
    
  
};
