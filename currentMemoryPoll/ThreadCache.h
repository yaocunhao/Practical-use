#include "common.h"

class ThreadCache
{
public:
	//申请内存，size为内存的大小
	void *Allocate(size_t size);

	// 当前自由链表中没有内存了，从中心缓存获取对象
	void* FetchFromCentralCache(size_t index, size_t size);



	//释放内存，传入指针，释放那个内存，头插至freelist之中
	void Deallocate(void *ptr,size_t size);

	// 释放对象时，链表过长时，回收内存回到中心缓存
	void ListTooLong(FreeList& list, size_t size);


private:
	FreeList _freeLists[NFREELISTS];//按对齐方式进行映射
};

//告诉给编译器此变量为线程内部使用，每个线程都会copy一份给本地用(每个线程独享)
static __declspec(thread) ThreadCache* tls_threadcache=nullptr;//指向ThreadCache 的指针