#include "ThreadCache.h"
#include "CentralCache.h"


// 从中心缓存获取对象
void* ThreadCache::FetchFromCentralCache(size_t pos, size_t size)//给pos是为了确定位置，给size是为了定位个数大小
{
	//为了避免浪费，获取对象的时候，采用慢启动的方法
	//获取一批对象，反馈调节，慢启动,MaxSize从1开始增大，当大于512等于后就没有价值了
	//如何慢启动呢？ 得到最多可以返回多少对象 vs 当前的最大值 -> 来进行慢启动控制

	//期望获取多少空间
	size_t batchNum = fmin(SizeClass::NumMoveSize(size), _freeLists[pos].MaxSize());


	//增到了慢启动的阀值了
	//如果MaxSize是较小值，则每次增大，增到一定程度了就不会增了
	//8字节对象最多512个，maxsize最大513，因此的期望个数就是512个，不会再进入下面的循环
	if (_freeLists[pos].MaxSize() == batchNum)
	{
		_freeLists[pos].SetMaxSize(_freeLists[pos].MaxSize() + 1);
	}

	//去中心缓存获取batch_num个对象，给定两个指针，输出型参数，记录获取的空间
	void *start = nullptr;
	void *end = nullptr;

	//不一定给了期望的值，比如在span的最后了，只剩一个空间，就不会多给
	//RoundUp：size对齐到对齐数

	size_t actualNum = CentralCache::GetInstance()->FetchRangeObj(start, end, batchNum, SizeClass::RoundUp(size));
	
	assert(actualNum >0);//实际获取的大小如果小于等于0，说明出现了错误
	
	//>1返回一个，剩下的挂到自由链表，挂入一批对象
	if (actualNum >1)
	{
		//传入actualNum - 1 是为了记录当前的小块内存的个数
		_freeLists[pos].PushRange(NextObj(start), end,actualNum-1);//actualNum实际获取的大小个数
	}

	return start;//返回第一个对象的地址一个
}


void *ThreadCache::Allocate(size_t size)//size内存大小
{
	size_t pos = SizeClass::Index(size);//得到映射的位置

	if (!_freeLists[pos].Empty())//自由链表非空，则从自由链表中获取
	{
		return _freeLists[pos].Pop();//返回一块内存
	}
	else//central cache中去切割，给一批，减少去central cache中去取
	{
		//给一批，减少去central cache中去取
		//慢启动，先拿少一点点再拿多一点，避免造成浪费

		return FetchFromCentralCache(pos, size);//将得到的映射位置和需要的对象大小传入进去
	}
}







//释放内存，传入指针，释放那个内存，头插至freelist之中
void ThreadCache::Deallocate(void *ptr, size_t size)
{
	size_t pos = SizeClass::Index(size);//计算位置	
	_freeLists[pos].Push(ptr);//头插

	//thread cache中内存太多了，还到central cache 之中
	if (_freeLists[pos].Size() > _freeLists[pos].MaxSize())//[2,512]用得越少还得越快
	{
		ListTooLong(_freeLists[pos], size);
	}
}

// 释放对象时，链表过长时，回收内存回到中心缓存
void ThreadCache::ListTooLong(FreeList& list, size_t size)
{
	//计算还多少
	size_t batchNum = list.MaxSize();
	void *start = nullptr;
	void *end = nullptr;
	list.PopRange(start, end, batchNum);//一次性还batchNum个，先从freelist中取出来

	CentralCache::GetInstance()->ReleaseListToSpans(start, size);//归还给central cache
}

