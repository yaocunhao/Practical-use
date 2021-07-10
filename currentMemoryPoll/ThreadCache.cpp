#include "ThreadCache.h"
#include "CentralCache.h"


// �����Ļ����ȡ����
void* ThreadCache::FetchFromCentralCache(size_t pos, size_t size)//��pos��Ϊ��ȷ��λ�ã���size��Ϊ�˶�λ������С
{
	//Ϊ�˱����˷ѣ���ȡ�����ʱ�򣬲����������ķ���
	//��ȡһ�����󣬷������ڣ�������,MaxSize��1��ʼ���󣬵�����512���ں��û�м�ֵ��
	//����������أ� �õ������Է��ض��ٶ��� vs ��ǰ�����ֵ -> ����������������

	//������ȡ���ٿռ�
	size_t batchNum = fmin(SizeClass::NumMoveSize(size), _freeLists[pos].MaxSize());


	//�������������ķ�ֵ��
	//���MaxSize�ǽ�Сֵ����ÿ����������һ���̶��˾Ͳ�������
	//8�ֽڶ������512����maxsize���513����˵�������������512���������ٽ��������ѭ��
	if (_freeLists[pos].MaxSize() == batchNum)
	{
		_freeLists[pos].SetMaxSize(_freeLists[pos].MaxSize() + 1);
	}

	//ȥ���Ļ����ȡbatch_num�����󣬸�������ָ�룬����Ͳ�������¼��ȡ�Ŀռ�
	void *start = nullptr;
	void *end = nullptr;

	//��һ������������ֵ��������span������ˣ�ֻʣһ���ռ䣬�Ͳ�����
	//RoundUp��size���뵽������

	size_t actualNum = CentralCache::GetInstance()->FetchRangeObj(start, end, batchNum, SizeClass::RoundUp(size));
	
	assert(actualNum >0);//ʵ�ʻ�ȡ�Ĵ�С���С�ڵ���0��˵�������˴���
	
	//>1����һ����ʣ�µĹҵ�������������һ������
	if (actualNum >1)
	{
		//����actualNum - 1 ��Ϊ�˼�¼��ǰ��С���ڴ�ĸ���
		_freeLists[pos].PushRange(NextObj(start), end,actualNum-1);//actualNumʵ�ʻ�ȡ�Ĵ�С����
	}

	return start;//���ص�һ������ĵ�ַһ��
}


void *ThreadCache::Allocate(size_t size)//size�ڴ��С
{
	size_t pos = SizeClass::Index(size);//�õ�ӳ���λ��

	if (!_freeLists[pos].Empty())//��������ǿգ�������������л�ȡ
	{
		return _freeLists[pos].Pop();//����һ���ڴ�
	}
	else//central cache��ȥ�и��һ��������ȥcentral cache��ȥȡ
	{
		//��һ��������ȥcentral cache��ȥȡ
		//��������������һ������ö�һ�㣬��������˷�

		return FetchFromCentralCache(pos, size);//���õ���ӳ��λ�ú���Ҫ�Ķ����С�����ȥ
	}
}







//�ͷ��ڴ棬����ָ�룬�ͷ��Ǹ��ڴ棬ͷ����freelist֮��
void ThreadCache::Deallocate(void *ptr, size_t size)
{
	size_t pos = SizeClass::Index(size);//����λ��	
	_freeLists[pos].Push(ptr);//ͷ��

	//thread cache���ڴ�̫���ˣ�����central cache ֮��
	if (_freeLists[pos].Size() > _freeLists[pos].MaxSize())//[2,512]�õ�Խ�ٻ���Խ��
	{
		ListTooLong(_freeLists[pos], size);
	}
}

// �ͷŶ���ʱ���������ʱ�������ڴ�ص����Ļ���
void ThreadCache::ListTooLong(FreeList& list, size_t size)
{
	//���㻹����
	size_t batchNum = list.MaxSize();
	void *start = nullptr;
	void *end = nullptr;
	list.PopRange(start, end, batchNum);//һ���Ի�batchNum�����ȴ�freelist��ȡ����

	CentralCache::GetInstance()->ReleaseListToSpans(start, size);//�黹��central cache
}

