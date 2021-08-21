
#include "PageCache.h"

PageCache PageCache::_sInst;//单例对象实例化

//向系统申请K页内存挂到自由链表中
void* PageCache::SystemAllocPage(size_t k)
{
	return ::SystemAlloc(k);
}


Span* PageCache::NewSpan(size_t k)
{
	//用递归锁，防止自己调用自己进入死循环
	std::lock_guard<std::recursive_mutex> lock(_mtx);

	//针对直接申请大于NPAGES的大块内存，直接找系统要
	if (k >= NPAGES)
	{
		void *ptr = SystemAllocPage(k);
		Span* span = new Span;
		span->_pageId = (ADDRESS_INT)ptr >> PAGE_SHIFT;//获得页号
		span->_n = k;//获得页数

		_idSpanMap[span->_pageId] = span;//建立页号与页数的映射关系

		return span;
	}
	//先判断正好合适的位置有没有内存
	if (!_spanList[k].Empty())//不为空，表示挂了内存块，直接返回即可
	{
		return _spanList[k].PopFront();
	}

	//找大的页切成小的
	for (size_t i = k+1; i < NPAGES; ++i)
	{
		//大页切小,切成K页的span返回
		//剩余(i-k)页挂到(i-k)的位置上去
		if (!_spanList[i].Empty())
		{
			//尾切出一个K页的span
			Span *span = _spanList[i].PopFront();

			Span *split = new Span;//返回的页
			split->_pageId = span->_pageId + span->_n - k;
			split->_n = k;

			//改变切出来的页号和span之间的关系
			for (PageID i = 0; i < k; i++)
			{
				_idSpanMap[split->_pageId + i] = split;
			}

			span->_n -= k;

			_spanList[span->_n].PushFront(span);//剩余的挂回去，这个span没有改变，直接挂到新的位置即可

			return split;//返回一个span给central cache
		}
	}

	//第一次进来，大页的表全为空，需要问系统要
	//申请一个最大的页，插入至128的位置

	Span* bigSpan = new Span;//定义一个节点出来

	//问系统要内存 -> 要128页
	void *memory = SystemAllocPage(NPAGES - 1);//因为要切割，所以不放入bigSpan节点之中


	bigSpan->_pageId = (size_t)(memory) >> 12;//页号为当前的地址/4K
	bigSpan->_n = NPAGES - 1;//页数

	//按页号和span建立映射关系
	for (PageID i = 0; i < bigSpan->_n; i++)
	{
		PageID	id = bigSpan->_pageId + i;
		_idSpanMap[id] = bigSpan;
	}

		_spanList[NPAGES - 1].PushFront(bigSpan);//将内存插入对应的列表之中

	//调用自己，再次执行流程，从128中切割
	return NewSpan(k);
}	

//获取从对象到span的映射
Span * PageCache::MapObjectToSpan(void* obj)
{
	//防止这里建设映射关系，外面的也调用进行修改
	std::lock_guard<std::recursive_mutex> lock(_mtx);

	PageID id = (ADDRESS_INT)obj >> PAGE_SHIFT;//表示在第几页
	auto ret = _idSpanMap.find(id);//查找是否在哈希表中

	if (ret != _idSpanMap.end())//在里面
	{
		return ret->second;//获取对应的span
	}
	else//到这里肯定是某处出错了
	{
		assert(false);
		return nullptr;
	}

}


//释放空闲span回到Pagecache，并且合并相邻的span
void PageCache::ReleaseSpanToPageCache(Span* span)
{
	//大于128页，直接从系统申请的，将这块内存还给系统
	if (span->_n >= NPAGES)
	{
		//_idSpanMap.erase(span->_pageId);//从对应的映射关系之中删除
		_idSpanMap.erase(span->_pageId);

		void *ptr = (void*)(span->_pageId << PAGE_SHIFT);//页号<<12 = 地址
		SystemFree(ptr);//释放这个地址

		delete span;
		return;
	}

	std::lock_guard<std::recursive_mutex> lock(_mtx);


	//不是直接向系统申请的，返回到page的spanlist之中

	//检查前后空闲sapn页，进行合并,解决内存碎片问题
	//向前合并
	while (true)
	{
		PageID preId = span->_pageId - 1;//前面一个页的页号
		auto ret = _idSpanMap.find(preId);

		//前面一个页的span不存在或者未分配
		if (ret == _idSpanMap.end())
		{
			break;
		}

		Span* preSpan = ret->second;//找到前面的span
		
		//前面页的还在使用中
		if (preSpan->_usecount != 0)//前面的页还在使用，则不能合并
		{
			break;
		}

		//超过128页不能合
		if (preSpan->_n + span->_n >= NPAGES)
		{
			break;
		}

		//走到这里，说明前面的页也返回来了，是可以进行合并的

		//从对应的spanlist将当前span和前面的span拿下来
		_spanList[preSpan->_n].Erase(preSpan);
		_spanList[span->_n].Erase(span);

		//合并span
		span->_pageId = preSpan->_pageId;
		span->_n += preSpan->_n;


		//更新页之间的映射关系 -> 合并之后改变映射关系
		for (PageID i = 0; i < preSpan->_n; i++)
		{
			_idSpanMap[preSpan->_pageId + i] = span;
		}

			delete preSpan;
	}

	//向后合并
	while (1)
	{
		PageID nextId= span->_pageId + span->_n;
		auto ret = _idSpanMap.find(nextId);
		if (ret == _idSpanMap.end())
		{
			break;
		}
		Span *nextSpan = ret->second;

		if (nextSpan->_usecount != 0)
		{
			break;
		}

		//超过128页不能合
		if (nextSpan->_n + span->_n >= NPAGES)
		{
			break;
		}

		_spanList[nextSpan->_n].Erase(nextSpan);

		span->_n += nextSpan->_n;
		for (PageID i = 0; i < nextSpan->_n; i++)
		{
			_idSpanMap[nextSpan->_pageId + i] = span;
		}
		delete nextSpan;
	}

	//将合并之后的大块内存，从新添加至spanlist之中	
	_spanList[span->_n].PushFront(span);

}