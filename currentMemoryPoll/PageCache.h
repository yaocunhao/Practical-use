#pragma once
#include "common.h"



class PageCache
{
public:
	//返回单例对象
	static PageCache* GetInstance()
	{
		return &_sInst;
	}

	//向系统申请K页内存挂到自由链表
	void* SystemAllocPage(size_t k);
	Span* NewSpan(size_t k);//返回一个块个central cache
	
	//获取从对象到span的映射
	Span *MapObjectToSpan(void *obj);

	//释放空闲span回到Pagecache，并且合并相邻的span
	void ReleaseSpanToPageCache(Span* span);

	std::recursive_mutex _mtx;//单例的，因此锁也只有一把

private:
	SpanList _spanList[129];// 按页进行映射，0位置不用，下标为几，桶下面挂的就是几页大小的内存块

	std::unordered_map<PageID, Span*> _idSpanMap; //使用基数树不需要加锁
	//TCMalloc_PageMap2<32-PAGE_SHIFT> _idSpanMap;


	//设计成单例模式
	PageCache()
	{}

	static PageCache _sInst;
	PageCache(const PageCache &p) = delete;
	PageCache& operator =(PageCache&p) = delete;
};


