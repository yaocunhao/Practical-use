#pragma once

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <exception>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <math.h>
#include <map>
#include <stdlib.h>
#include <mutex>
#include <unordered_map>
#include <thread>
#include <windows.h>



//宏不方便调试
static const size_t MAX_BYTES = 64 * 1024;//最大的内存，大于这个数，去page cache中去切割
static const size_t NFREELISTS = 184;//最大桶的个数
static const size_t NPAGES = 129;//对应的页数
static const size_t PAGE_SHIFT = 12;//2^12=4k，32位系统下1页4K，64下1页=8/16K

//跨平台
#ifndef _WIN32
	typedef size_t  ADDRESS_INT;
	typedef size_t PageID;
#else
	typedef unsigned long long ADDRESS_INT;
	typedef unsigned long long PageID;
#endif



//管理对齐和映射关系
class SizeClass
{
public:
	// 控制在1%-12%左右的内碎片浪费
	// [1,128]				8byte对齐       freelist[0,16)
	// [129,1024]			16byte对齐		freelist[16,72)
	// [1025,8*1024]		128byte对齐		freelist[72,128)
	// [8*1024+1,64*1024]	1024byte对齐  	freelist[128,184)

	//RountUP切小时需要 -> 对齐到对齐数的整数倍

	//[1,8]   8    +7     8~15 &(~7)  ~(0000 0111)=1111 1000 本质就是将低三位全部丢弃掉 -> 即对齐到了
	// 8:0000  1000	 & 1111 1000 = 8 
	// 9:0000  1001	 & 1111 1000 = 8
	// 10:0000 1010  & 1111 1000 = 8

	//[9,16] 16    +7     16~23

	static size_t _RoundUp(size_t bytes, size_t align)
	{
		return (((bytes)+align - 1) & ~(align - 1));
	}

	// 对齐大小计算，浪费大概在1%-12%左右
	static inline size_t RoundUp(size_t bytes)//传入一个需要的内存大小，通过算法对应到映射的位置
	{
		//assert(bytes <= MAX_BYTES);
		if (bytes <= 128){
			return _RoundUp(bytes, 8);
		}
		else if (bytes <= 1024){
			return _RoundUp(bytes, 16);
		}
		else if (bytes <= 8192){
			return _RoundUp(bytes, 128);
		}
		else if (bytes <= 65536){
			return _RoundUp(bytes, 1024);
		}
		else
		{
			return _RoundUp(bytes, 1 << PAGE_SHIFT);//1页为单位进行内存对齐
		}
		return -1;
	}


//计算映射的位置
	inline static size_t _Index(size_t bytes, size_t ANumber)//传入对象大小和对齐数
	{
		// 假设当前对齐数为8 ->ANumber=3-> bytes+(1<<3)=bytes+7:在下一个位置
		//>>ANumber 其实就是除8 ，因为加了对齐数-1，所以在下一个位置，最后再减去1

		return ((bytes + (1 << ANumber) - 1) >> ANumber) - 1;//对齐最好为2的整数倍，才好进行左移和右移
	}

	// 计算映射的哪一个自由链表桶
	static inline size_t Index(size_t bytes)
	{
		assert(bytes <= MAX_BYTES);//不能大于64K，大于64K向 page cache申请

		// 每个对齐区间有多少个桶
		static int GArray[4] = { 16, 56, 56, 56 };

		if (bytes <= 128)//对齐数为8
		{
			return _Index(bytes, 3);
		}
		else if (bytes <= 1024)//对齐数为16
		{
			return _Index(bytes - 128, 4) + GArray[0];//不是从0开始的，所以需要 -128 && 加上前面的桶的数量
		}
		else if (bytes <= 8192)//对齐数为128
		{
			return _Index(bytes - 1024, 7) + GArray[1] +
				GArray[0];
		}
		else if (bytes <= 65536)//对齐数为1024
		{
			return _Index(bytes - 8192, 10) + GArray[2] +
				GArray[1] + GArray[0];
		}

		assert(false);
		return -1;
	}

	//一次从中心缓存期望获取多少个
	//size比较大则获取少一点，比较小则多一点 
	static size_t NumMoveSize(size_t size)//size单个对象的大小
	{
		if (size == 0)
			return 0;
		int num = MAX_BYTES / size;
		if (num < 2)
			num = 2;
		if (num > 512)
			num = 512;
		return num;
	}



	// 计算一次向系统获取几个页
	//单个对象 8byte
	//..
	//单个对象 64KB

	static size_t NumMovePage(size_t size)//size单个对象的大小
	{
		size_t num = NumMoveSize(size);//计算给多少个对象
		size_t npage = num*size;//总大小(字节数)

		npage >>= 12;//算是多少页(字节/1024*4)
		if (npage == 0)//最少给一页
			npage = 1;

		return npage;
	}

};

//获得指针指向的空间前面几个字节的内容
inline void* & NextObj(void *obj)
{
	return *(void**)obj;
}



//thread cache 自由链表
class FreeList
{
public:

	//从central cache中获取了多个小块内存，将剩余的一连串内存挂至对应位置的自由链表之中
	void PushRange(void *start, void *end, int n)
	{
		//长串的内存进行头插
		NextObj(end) = _head;
		_head = start;
		_size += n;
	}

	void PopRange(void*& start,void *&end,int n)//Pop n个
	{
		start = _head;
		end = _head;
		for (int i = 0; i < n; i++)
		{
			end = _head;
			_head = NextObj(_head);
		}

		NextObj(end) = nullptr;//断开前后的链接
		_size -= n;
	}
	void Push(void *obj)//头插
	{
		NextObj(obj) = _head;//取前面的字节，保存头指针
		_head = obj;
		_size ++;
	}

	void *Pop()//头删
	{
		void *obj = _head;
		_head = NextObj(_head);
		NextObj(obj) = nullptr;//断开链接
		_size--;
		return obj;
	}

	bool Empty()
	{
		return _head == nullptr;
	}

	size_t MaxSize()
	{
		return _max_size;
	}

	void SetMaxSize(size_t n)
	{
		_max_size = n;
	}

	size_t Size()
	{
		return _size;
	}
private:
	void *_head = nullptr;
	size_t _max_size=1;//每次申请多少块小内存
	size_t _size=0;//小块内存的个数
};

//////////////////////////////////
//span->用来管理大块内存  -> 给central cache 和page cache用

//64位下，内存为2^64 ，一个页为2^12 ， 2^64/2^12=2^52 size_t 是编不下的
//重定义出来，64位系统下就改成longlong类型或者int_64
//typedef size_t PageID;
struct Span
{
	PageID _pageId=0;//页号，一个跨度，找前后span进行合并更大
	size_t _n=0;//从这个页号开始的页数

	//一个Span用完了，还需要申请一个新的span，就进行管理起来
	Span* _next=nullptr;
	Span* _prev=nullptr;

	void *_list=nullptr;//大块内存切小链接起来，这样回收回来的也方便链接
	size_t _usecount=0;//使用计数，等于0时，说明所有对象都回来了
	size_t _objsize=0;//切出来的单个对象的大小，一个span只且给一个大小
};


//SpanList 用来管理大块内存的双向链表
class SpanList
{
public:
	SpanList()
	{
		_head = new Span;
		_head->_next = _head;
		_head->_prev = _head;
	}

	Span* Begin()
	{
		return _head->_next;
	}
	Span* End()
	{
		return _head;
	}

	void PushFront(Span* span)
	{
		Insert(Begin(), span);
	}

	Span* PopFront()
	{
		Span* span = Begin();
		Erase(span);

		return span;
	}


	void Insert(Span* cur, Span* newspan)
	{
		Span* prev = cur->_prev;
		
		prev->_next = newspan;
		newspan->_prev = prev;

		newspan->_next = cur;
		cur->_prev = newspan;
	}

	void Erase(Span* cur)
	{
		assert(cur != _head);//带头双向循环链表，头不能删

		Span* prev = cur->_prev;
		Span* next = cur->_next;

		prev->_next = next;
		next->_prev = prev;
	}

	bool Empty()
	{
		return _head->_next == _head;
	}

	std::mutex _mtx;

private:
	Span *_head;
};


//向系统以页为单位申请内存
inline static void* SystemAlloc(size_t kpage)
{
#ifdef _WIN32_
	//32位下，(1<<12)=1k，
	void* ptr = VirtualAlloc(0, kpage*(1 << PAGE_SHIFT),MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	// brk mmap等
#endif
	if (ptr == nullptr)
		throw std::bad_alloc();
	return ptr;
}

inline static void SystemFree(void* ptr)
{
#ifdef _WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	// sbrk unmmap等
#endif
} 

