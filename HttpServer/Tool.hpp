#pragma once
#include "common.hpp"

class Tool
{
  public:

    //分割请求行
    static void StringParse(std::string &requese_line,std::string &method,std::string &uri,std::string &version)
    {
      //采用stringstreamm分割字符串
        std::stringstream str(requese_line);
        str>>method>>uri>>version;
    }

    //将一行请求报文拆分成KV值
    static void MakeStringToKv(std::string line,std::string &key,std::string &val)
    {
      size_t pos=line.find(": ");
      if(pos!=std::string::npos)
      {
        key=line.substr(0,pos);
        val=line.substr(pos+2,line.size());
      }
    }
};

