#include "common.h"

class ThreadCache
{
public:
	//�����ڴ棬sizeΪ�ڴ�Ĵ�С
	void *Allocate(size_t size);

	// ��ǰ����������û���ڴ��ˣ������Ļ����ȡ����
	void* FetchFromCentralCache(size_t index, size_t size);



	//�ͷ��ڴ棬����ָ�룬�ͷ��Ǹ��ڴ棬ͷ����freelist֮��
	void Deallocate(void *ptr,size_t size);

	// �ͷŶ���ʱ���������ʱ�������ڴ�ص����Ļ���
	void ListTooLong(FreeList& list, size_t size);


private:
	FreeList _freeLists[NFREELISTS];//�����뷽ʽ����ӳ��
};

//���߸��������˱���Ϊ�߳��ڲ�ʹ�ã�ÿ���̶߳���copyһ�ݸ�������(ÿ���̶߳���)
static __declspec(thread) ThreadCache* tls_threadcache=nullptr;//ָ��ThreadCache ��ָ��