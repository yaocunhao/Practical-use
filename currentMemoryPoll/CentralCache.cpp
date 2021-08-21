
#include "CentralCache.h"
#include "PageCache.h"
 
CentralCache CentralCache::_sInst;
 
// 从SpanList或者page cache获取一个span
Span* CentralCache::GetOneSpan(SpanList &list, size_t size)
{
	//去spanlist中去找还有内存的span
	Span* it = list.Begin();//获取双向链表的头结点

	while (it != list.End())
	{
		//判断是否有内存
		if (it->_list)//不为空，表示这个位置是有内存的
		{
			return it;
		}
		it = it->_next;
	}

	//运行到此，说明对应位置的span链中都没有内存了，只能找page cache进行申请
	//page cache返回的都是以页为单位的内存
	Span *span = PageCache::GetInstance()->NewSpan(SizeClass::NumMovePage(size));//参数为申请多少页，调用封装好的接口进行计算

	//切分好挂在list之中

	//算起始地址，地址/4K = 页号
	char *start = (char*)(span->_pageId << PAGE_SHIFT);
	char *end = start + (span->_n << PAGE_SHIFT);
	
	while (start < end)
	{
		//此处的size需要进行处理
		//如果不是对齐数，比如给个9，内存块就被切成9了，所以在上一层的时候就需要进行处理
		char *next = start + size;//计算出来下一小块内存的起始位置

		//头插
		NextObj(start) = span->_list;
		span->_list = start;

		start = next;
	}

	span->_objsize = size;//保存切出来的对象大小
	list.PushFront(span);
	return span;

}

// 从中心缓存获取一定数量的对象给thread cache
//start 、end 输出型参数
//n：对方期望获取多少个
//size：对方期望获取的字节的大小
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t size)
{
	size_t pos = SizeClass::Index(size);//计算在哪一个对应的位置

	//加锁 ->申请同样的大小则加锁，否则不需要
	//_spanLists[pos].Lock(); -> 抛异常就资源泄漏了-> 使用lock_guard
	std::lock_guard<std::mutex> lock(_spanLists[pos]._mtx);

	Span* span = GetOneSpan(_spanLists[pos], size);//获取一个大块内存span

	//要了一个span，从中切n个小对象出来(硬性需求1个，实际多要)	
	start = span->_list;//大块内存的起始地址
	end = start;//防止只有一个对象的情况
		
	void *prev = start;
	size_t i = 1;
	while (i <= n && end!=nullptr)//走n-1步，并且end不能为空
	{
		prev = end;
		end = NextObj(end);
		i++;
		span->_usecount++;//每取走一个++
	}

	span->_list = end;//更新spanlist起始位置
	end = prev;
	NextObj(end) = nullptr;//断开与原来Span的链接


	return i-1;//获取的个数
}


void CentralCache::ReleaseListToSpans(void* start, size_t byte_size)
{
	//计算对应的span处于那个坐标之下
	size_t pos = SizeClass::Index(byte_size);

	std::lock_guard<std::mutex> lock(_spanLists[pos]._mtx);//临界资源，加锁


	while (start)
	{
		void *next = NextObj(start);

		//找start内存块属于哪个span
		Span* span = PageCache::GetInstance()->MapObjectToSpan(start);

		//将start对应的小块内存，头插至span之中
		NextObj(start) = span->_list;
		span->_list = start;
		

		if (span->_usecount == 0)//说明span中切出去的都还回来了,然后还给中心page
		{
			_spanLists[pos].Erase(span);//从spanlist之中弹出
			span->_list = nullptr;//断开与其他span的链接
			PageCache::GetInstance()->ReleaseSpanToPageCache(span);

		}
		start = next;//再次对下一块小个内存进行操作
	}
}