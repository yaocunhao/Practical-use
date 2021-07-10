#pragma once
#include "common.h"

class CentralCache
{
public:

	//返回单例对象
	static CentralCache* GetInstance()
	{
		return &_sInst;
	}

	// 从Span之中截取n个大小为size的对象给thread cache
	//start、end输出型参数，start为截取内存的起点，end为截取内存的终点
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t size);

	// 从SpanList或者page cache获取一个span   
	Span* GetOneSpan(SpanList &list, size_t size);

	// 将一定数量的对象释放到span跨度
		void ReleaseListToSpans(void* start, size_t byte_size);

private:
	SpanList _spanLists[NFREELISTS];//按对齐方式进行映射
	
	//单例模式
	CentralCache()
	{}

	CentralCache(const CentralCache&c) = delete;
	CentralCache& operator=(CentralCache& c) = delete;


	static CentralCache _sInst;//定义一个全局的中心缓存的对象，多个线程调用的是同一个中心缓存
};

