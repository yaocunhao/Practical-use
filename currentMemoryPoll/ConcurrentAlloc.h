#pragma once 

#include "common.h"
#include "ThreadCache.h"
#include "PageCache.h"

static  void *ConcurrentAlloc(size_t size)
{

	try{

		if (size > MAX_BYTES)//����64K(16ҳ)
		{
			//PageCache �������ľ��Ǵ�ҳ��ȥ����
			if (size > MAX_BYTES)
			{
				size_t npage = SizeClass::RoundUp(size) >> PAGE_SHIFT;
				Span *span = PageCache::GetInstance()->NewSpan(npage);
				span->_objsize = size;

				void *ptr = (void*)(span->_pageId << PAGE_SHIFT);
				return ptr;
			}

		}
		else//ȥ��freelist
		{
			if (tls_threadcache == nullptr)//Ϊ���򴴽�һ��
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

		//��ַ<<12�õ�ҳ�ţ�����ҳ�ź�span��ӳ���ҵ���Ӧ��span
		Span* span = PageCache::GetInstance()->MapObjectToSpan(ptr);
		size_t size = span->_objsize;//��ö�Ӧ��span֮�л��ֳ����Ķ���Ĵ�С

		if (size > MAX_BYTES)//����64K
		{
			PageCache::GetInstance()->ReleaseSpanToPageCache(span);
		}
		else
		{
			assert(tls_threadcache);//�ͷŵ�ʱ�򣬾Ͳ�Ӧ��Ϊ����
			tls_threadcache->Deallocate(ptr, size);
		}
		//����
		return nullptr;
	}
	catch (const std::exception &e)
	{
		cout << e.what() << endl;
	}
}