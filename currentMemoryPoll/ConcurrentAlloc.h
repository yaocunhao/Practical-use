#pragma once 

#include "common.h"
#include "ThreadCache.h"
#include "PageCache.h"

static  void *ConcurrentAlloc(size_t size)
{

	try{

		if (size > MAX_BYTES)//大于64K(16页)
		{
			//PageCache 里面管理的就是大页，去找它
			if (size > MAX_BYTES)
			{
				size_t npage = SizeClass::RoundUp(size) >> PAGE_SHIFT;
				Span *span = PageCache::GetInstance()->NewSpan(npage);
				span->_objsize = size;

				void *ptr = (void*)(span->_pageId << PAGE_SHIFT);
				return ptr;
			}

		}
		else//去找freelist
		{
			if (tls_threadcache == nullptr)//为空则创建一个
			{
				tls_threadcache = new ThreadCache;
			}
			return tls_threadcache->Allocate(size);
		}
	}
	catch (const std::exception &e)
	{
		cout << e.what() << endl;
	}
}

static void *ConcurrentFree(void *ptr)
{
	try
	{

		//地址<<12得到页号，根据页号和span的映射找到对应的span
		Span* span = PageCache::GetInstance()->MapObjectToSpan(ptr);
		size_t size = span->_objsize;//获得对应的span之中划分出来的对象的大小

		if (size > MAX_BYTES)//大于64K
		{
			PageCache::GetInstance()->ReleaseSpanToPageCache(span);
		}
		else
		{
			assert(tls_threadcache);//释放的时候，就不应该为空了
			tls_threadcache->Deallocate(ptr, size);
		}
		//测试
		return nullptr;
	}
	catch (const std::exception &e)
	{
		cout << e.what() << endl;
	}
}