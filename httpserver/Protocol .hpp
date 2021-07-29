#pragma once
#include  "common.hpp"
#include "Tool.hpp"
#include "Sock.hpp"
#include "Log.hpp"

#define WEBROOT "wwwroot"
#define HOMEPAGE "index.html" //不带参数时，默认页面
#define VERSION "HTTP/1.0" //响应的http为1.0，因此不需要进行兼容性检测(新版本自动兼容老版本)

static std::string CodeToStaus(int code)
{
  switch(code)
  {
    case 200:
      return "OK";
    case 404:
      return "Not Found";
    case 500:
      return "server error";
    default:
      return "not status";
  }
}

static std::string SuffixToDesc(const std::string &suffix)
{
  if(suffix==".js")
    return "application/ecmascript";
  else if(suffix==".css")
    return "text/css";
  else if(suffix==  ".jpg")
    return "image/jpeg";
  else
    return "text/html";//默认返回一个网页
}



//请求模块
class HttpRequest
{
  private:
    //协议接收模块
    std::string request_line;//请求行
    std::vector<std::string >request_header;//请求报文
    std::string blank;//空行
    std::string request_body;//正文
  
  private:

    //请求行解析接收
    std::string method;//请求方法
    std::string uri;
    std::string version;

    //请求报文解析接收
    std::unordered_map<std::string,std::string> header_kv;

    //将conten_length设置出来，后续直接使用
    size_t content_length;

    std::string path;//请求服务器资源
    std::string query_string;//请求参数
    
    //标定请求的是网页资源还是程序资源
    bool cgi;

    //打开网页的大小 -> 判断文件是否打开时可以获得(stat)
    ssize_t file_size;
    
    std::string suffix;//资源后缀


  public:
    HttpRequest()
    :blank("\n")
    ,content_length(-1)
    ,path(WEBROOT)//默认的web根目录
    ,cgi(false)
    ,suffix("text/html")//默认是一张网页
    {}
  
    //将读取到的请求行内容设置进来
    void SetRequestLine(const std::string &line)
    {
      request_line=line;
    }
    
    //将请求行分成三个部分 -> 请求方法、url、版本号
    void RequestLinePares()
    {
      //分成三部分
      Tool::StringParse(request_line,method,uri,version);
    //  LOG(Notice,request_line);
    //  LOG(Notice,method);
    //  LOG(Notice,uri);
    //  LOG(Notice,version);
    }
  
    //将读取到的请求报文行设置进来
    void InsertHeaderLine(const std::string &line)
    {
      request_header.push_back(line);
      LOG(Notice,line);
    }

  //将请求报文设置成map，形成映射关系
    void RequestHeaderPares()
    {
        for(auto&e:request_header)
        {
          std::string key;
          std::string val;
          Tool::MakeStringToKv(e,key,val);//将一行请求报文拆分成K/V值
          
          header_kv[key]=val;
          if(key=="Content-Length")
          {
            content_length=stoi(val);
          }

          LOG(Notice,key);
          LOG(Notice,val);
          LOG(Notice,e);
        }

    }

    //判断是否需要读取正文
    //判断方法是否为POST 和 conten-lenthes是否为0
    bool isNeedRecvBody()
    {
      //post有可能大小写
      if(strcasecmp(method.c_str(),"POST")==0 && content_length > 0)
      {
        cgi=true;//有正文，表示使用CGI
        return true;
      }
      return false;
    }

    
    //获得正文长度
    ssize_t GetContentLength()
    {
      return content_length;
    }
  
    //设置正文内容
    void SetRequestBody(std::string body)
    {
      request_body=body;
    }
    
    //判断方法是否合法
    bool IsMethod()
    {
      if(strcasecmp(method.c_str(),"GET")==0 || strcasecmp(method.c_str(),"POST")==0)
      {
        return true;
      }
      return false;
    }

    //判断是否为Get方法
    bool IsGet()
    {
      return strcasecmp(method.c_str(),"GET")==0;
    }

    bool IsPost()
    {
      return strcasecmp(method.c_str(),"POST")==0;
    }

    //解析UR
    
    //Get方法解析
    //get方法，路径 ->path，参数(如果有)->query_string
    void GetUriParse()
    {
      std::size_t pos=uri.find('?');
      if(pos==std::string::npos)//没有问号，即没有参数
      {
        path+=uri;
      }
      else
      {
        cgi=true;//有参数，使用cgi
        path+=uri.substr(0,pos);//路径
        query_string=uri.substr(pos+1);//参数
      }
    }

    //POST方法解析
    void PostUriParse()
    {
      path+=uri;
    }
    
    //获取资源路径
    std::string GetPath()
    {
      return path;
    }
  
    //判断是否需要添加默认界面路径
    void IsAddHomePage()
    {
      // path 如果是以/结尾表示需要添加默认路径
      if(path.back()=='/')
        path+=HOMEPAGE;
    }

  //设置资源路径
    void SetPath(std::string &path_string)
    {
      path=path_string;
    }

  //设置是否是Cgi
    void SetCgi()
    {
      cgi=true;
    }
  //判断是否是Cgi
    bool IsCgi()
    {
      return cgi;
    }
  
    //设置打开的文件大小
    void  SetFileSize(ssize_t size)
    {
      file_size=size;
    }

    //获得打开的文件大小
    ssize_t GetFileSize()
    {
      return file_size;
    }

    //获取参数
    std::string GetQueryStrinng()
    {
      return query_string;
    }

    std::string GetBody()
    {
      return request_body;
    }

    std::string GetMethod()//获得方法
    {
      return method;
    }

    std::string MakeSuffix()//分析返回后缀
    {
       size_t pos=path.find(".");
       if(pos!=std::string::npos)//找到了
       {
          suffix=path.substr(pos);//后缀以点开头的
       }
      return suffix;
    }


    ~HttpRequest()
    {}

};


//响应模块
class HttpResponse
{
  private:
    std::string response_line;//响应行
    std::vector<std::string> responser_header;//响应报头
    std::string blank;//空行
    std::string response_body;//响应正文

  public:

    HttpResponse()
      :blank("\r\n")
    {}

    void SetResponseLine(const std::string &line)
    {
      response_line=line;
    }

    std::string GetResponseLine()//获得首行
    {
      return response_line;
    }

    const std::vector<std::string> GetResponseHeader()
    {
      responser_header.push_back(blank);//将空行添加进去，一同进行处理
      return responser_header;
    }

    void AddHeader(const std::string &ct)
    {
      responser_header.push_back(ct);
    }

    ~HttpResponse()
    {}

};



//网络通信模块
class EndPoint
{
  private:
    int sock;
    HttpRequest  req;
    HttpResponse res;
  
  void GetRequestLine()//处理请求行
  {
    std::string line;
    Sock::GetLine(sock,line);//读取请求行
    req.SetRequestLine(line);//设置请求行
    req.RequestLinePares();//进行解析
  }

  //制作响应行  
  //制作响应首行=版本号+状态码+状态码解释(传入状态码即可制作)
    void MakeResponseLine(int code)
    {
      std::string response_line;

      response_line +=VERSION;
      response_line +=" ";
      response_line +=std::to_string(code);
      response_line +=" ";
      response_line +=CodeToStaus(code);//将状态码转成状态码描述
      response_line+="\r\n";

      res.SetResponseLine(response_line);//将制作的状态行填入响应的协议之中
    }


    //处理请求报文，顺带处理空行
  void GetRequestHeader()
  {
    //读取并设置请求报文
    std::string line;
    do
    {
      line="";//每次读取前清空
      Sock::GetLine(sock,line);//按行读取请求报文
      req.InsertHeaderLine(line);//设置请求报文 -> 将读取到的行，插入请求报文之中

    }while(!line.empty());//读到空行时为空 -> 将空行处理了(将空行读取出来了)

    //解析请求报文
    req.RequestHeaderPares();
  }
  
  //制作响应报文
  void SetRequestHeaderLine()
  {
    std::string suffix=req.MakeSuffix();//获得后缀
    std::string content_type="Content-Type: ";
    content_type+=SuffixToDesc(suffix);
    content_type+="\r\n";
    res.AddHeader(content_type);//将content_type添加至响应报文之中
  }



  //处理正文
  void GetRequestBody()
  {
    ssize_t  len=req.GetContentLength();
    char c;
    std::string body;
    while(len)
    {
      ssize_t size=recv(sock,&c,1,0);
      body.push_back(c);
      len--;
    }
    //插入正文之中
    req.SetRequestBody(body);
  }


  public:
    EndPoint(int _sock)
    :sock(_sock)
    {}

//-------------------处理请求---------------------
     void RecvRequest()//处理请求
     {
        //获取完整http请求
        //分析http请求
        
        //处理请求行
        GetRequestLine();

        //处理请求报文，顺带处理了空行
        GetRequestHeader();
        
        //判断是否需要读取正文 ->  content-lenths!=0 && 方法为POST
        if(req.isNeedRecvBody())
        {
          GetRequestBody();
        }
     }

 //-----------------------分析请求、制作响应-----------------
 
     void MakeResponse()//分析请求、制作响应
     {

       //*********方法的处理*****
       
       //只处理GET和POST方法
       
       int code=200;//状态码初始化为200
       std::string path_string ;

       if(!req.IsMethod())//判断方法是否合法
       {
          LOG(Waring,"method not legal");
          code=404;

          goto end;//不和法则进行跳转
       }

      //合法请求
      //GET方法：    资源路径 -> path、  参数(如果有) -> query_string
      //POST方法：   资源路径 -> uri
      if(req.IsGet())//是GET请求
      {
        req.GetUriParse();
      }
      else//POST方法
      {
        req.PostUriParse();
      }
      req.IsAddHomePage();//判断是否需要添加默认路径

      path_string=req.GetPath();//获取资源路径
      LOG(Notice,path_string);

      
      //*********资源的判断和处理
      
      //判断请求的资源是否存在(根据资源路径进行查看)
      //stat ==0表示文件存在,==-1表示文件不存在
      
      struct stat st;

      if (stat(path_string.c_str(),&st) < 0)//不存在
      {
        LOG(Waring,"path is not found 404!");
        code=404;
        goto end;
      }
      else//文件存在
      {
       if(S_ISDIR(st.st_mode))//是一个目录，给目录下添加对应的index(每个目录都有一个对应的index)
       {
          // /1/2/3 所以结尾是一个文件名，因此需要添加 / index.html
          path_string +="/";
          req.SetPath(path_string);
          req.IsAddHomePage();
       }
       else//不是目录
       {
         //拥有者、所属者、其它人，任意一个有执行(x)权限，即是可执行
          if(st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH)  
          {
            //可执行程序，应该使用cgi
            req.SetCgi();
          }
          else
          {
            //正常的网页请求 
          }
       }
       if(!req.IsCgi())//不是Cgi则有文件大小
       {
          //设置文件大小
          req.SetFileSize(st.st_size);
       }
      }


end:
     MakeResponseLine(code);//制作响应行
     
     SetRequestHeaderLine();//制作响应报头

     }
    



     //将网页的内容发送过去
     void ExceNonCgi(const std::string &path)
     {
        int fd=open(path.c_str(),O_RDONLY);//以读的方式打开文件
        if(fd < 0)//打开文件失败
        {
          LOG(Error,"open path error!!");
          return ;
        }
        sendfile(sock,fd,nullptr,req.GetFileSize());//将网页发送过去
        
          close(fd);
     }
    
     //CGI处理方式
     void ExceCgi()
     {
       // std::string arg;
       // if(req.IsGet())
       // {
       //   arg=req.GetQueryStrinng();//Get方法，参数在请求资源之中
       // }
       // else
       // {
       //   arg=req.GetBody();//POST方法，参数在正文之中
       // }
      
      //父子进程都拿到了数据，但是子进程进行了程序替换，代码和数据都没有了
      //因此需要进行进程间的通信-> 采用管道的方式
      
      //通过环境变量将参数传递给子进程 -> GET -> query_string
      //通过管道传递给子进程 -> Post -> body
      //参数太长(比如在body中)，是不适合用环境变量传输的
      
      //站在被调用进程的角度
       
       std::string content_length_env;//POST方法需要长度
       std::string method=req.GetMethod();//获得方法
       //将方法保存在环境变量之中
       std::string method_env="METHOD=";
       method_env+=method;

       std::string query;//定义参数
       std::string query_env="QUERY_STRING=";
       std::string body;//正文
       std::string path=req.GetPath();//资源路径
      
       //管道通信是单向的 -> 创建两个管道
       int pipe_in[2]={0};
       int pipe_out[2]={0};
       pipe(pipe_in);
       pipe(pipe_out);
        
       putenv((char*)method_env.c_str());//将方法导入环境变量

        pid_t id=fork();
        if(id == 0)//child&&进程程序替换
        {
          LOG(Notice,method);
          //通过in来读，通过out来进行输出
          close(pipe_in[1]);
          close(pipe_out[0]);
          
          //重定向 -> 替换之后，就可以直接用0、1文件描述符进行操作
         dup2(pipe_in[0],0);
         dup2(pipe_out[1],1);

        
          if(req.IsGet())//GET方法获取参数
          {
            query=req.GetQueryStrinng();//获得参数
            //设计环境变量
            query_env="QUERY_STRING=";
            query_env+=query;
            putenv((char*)query_env.c_str());//将GET方法的参数导入环境变量
          }
          else if(req.IsPost())//post方法则需要设置长度 -> 通过管道读取多少字节
          {
            content_length_env = "CONTENT-LENGTH=";
            content_length_env+=std::to_string(req.GetContentLength());
            putenv((char*)content_length_env.c_str());//导入环境变量
          }
          else
          {
            //todo
          }

          //进行程序替换
          execl(path.c_str(),path.c_str(),nullptr);//带路径执行,不需要选项
          exit(0);//替换失败

        }
        //father 通过in来进行写，out来进行读
          close(pipe_in[0]);
          close(pipe_out[1]);
          char c='C';
          if(req.IsPost())
          {
            body=req.GetBody();//POST,父进程从正文读取参数通过管道发送给子进程
            for(auto&e:body)
            {
              write(pipe_in[1],&e,1);
            }
          }

          //父进程从管道读取处理的结果，发送给客户端
          ssize_t s=0;
          do
          {
            s=read(pipe_out[0],&c,1);

            if(s > 0)
              send(sock,&c,1,0);
          }while(s > 0);

          waitpid(id,nullptr,0);
     }


     void SendResponse()//发送响应
     {
       //-----发送首行------
        std::string line=res.GetResponseLine();
        send(sock,line.c_str(),line.size(),0);//发送响应首行
       
      //---发送响应报头和空行-------  
        auto &header=res.GetResponseHeader();//获得响应报文
        for(auto&e:header)//发送响应报文
        {
          send(sock,e.c_str(),e.size(),0);
        }
      
      //---发送正文----
      
      //如果正文是一个网页 -> 将网页打开&&发回即可
      //如果传了参数，正文需要创建子进程，子进程以cgi的方式执行新的程序，将参数传入程序之中
      //程序处理完数据，将结果发送出去
      //---判断执行模式----
      //是cgi创建进程，执行新的程序，非cgi返回网页
      
        if(req.IsCgi())
        {
          //是cgi，创建子进程执行任务，返回结果
          LOG(Notice,"use Cgi");
          ExceCgi();
        }
        else
        {
          LOG(Notice,"Not use Cgi");
          std::string path=req.GetPath();
          ExceNonCgi(path);
        }
     }

    ~EndPoint()
    {
      if(sock >= 0 )
        close(sock);
    }

};






class Entry
{
  public:
   static void HandlerHttp(int sock)
   {
    //命令行式宏定义
#ifdef  DEBUG
          
     char request[10240];
     recv(sock,request,sizeof(request),0);
     cout<<request<<endl;
     close(sock);

#else
    
     EndPoint *ep=new EndPoint(sock);
     ep->RecvRequest(); //处理请求
     ep->MakeResponse();//制作响应
     ep->SendResponse();//发送响应

     delete ep;

#endif

   }

};
