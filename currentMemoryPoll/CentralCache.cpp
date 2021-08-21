
#include "CentralCache.h"
#include "PageCache.h"
 
CentralCache CentralCache::_sInst;
 
// ��SpanList����page cache��ȡһ��span
Span* CentralCache::GetOneSpan(SpanList &list, size_t size)
{
	//ȥspanlist��ȥ�һ����ڴ��span
	Span* it = list.Begin();//��ȡ˫�������ͷ���

	while (it != list.End())
	{
		//�ж��Ƿ����ڴ�
		if (it->_list)//��Ϊ�գ���ʾ���λ�������ڴ��
		{
			return it;
		}
		it = it->_next;
	}

	//���е��ˣ�˵����Ӧλ�õ�span���ж�û���ڴ��ˣ�ֻ����page cache��������
	//page cache���صĶ�����ҳΪ��λ���ڴ�
	Span *span = PageCache::GetInstance()->NewSpan(SizeClass::NumMovePage(size));//����Ϊ�������ҳ�����÷�װ�õĽӿڽ��м���

	//�зֺù���list֮��

	//����ʼ��ַ����ַ/4K = ҳ��
	char *start = (char*)(span->_pageId << PAGE_SHIFT);
	char *end = start + (span->_n << PAGE_SHIFT);
	
	while (start < end)
	{
		//�˴���size��Ҫ���д���
		//������Ƕ��������������9���ڴ��ͱ��г�9�ˣ���������һ���ʱ�����Ҫ���д���
		char *next = start + size;//���������һС���ڴ����ʼλ��

		//ͷ��
		NextObj(start) = span->_list;
		span->_list = start;

		start = next;
	}

	span->_objsize = size;//�����г����Ķ����С
	list.PushFront(span);
	return span;

}

// �����Ļ����ȡһ�������Ķ����thread cache
//start ��end ����Ͳ���
//n���Է�������ȡ���ٸ�
//size���Է�������ȡ���ֽڵĴ�С
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t size)
{
	size_t pos = SizeClass::Index(size);//��������һ����Ӧ��λ��

	//���� ->����ͬ���Ĵ�С�������������Ҫ
	//_spanLists[pos].Lock(); -> ���쳣����Դй©��-> ʹ��lock_guard
	std::lock_guard<std::mutex> lock(_spanLists[pos]._mtx);

	Span* span = GetOneSpan(_spanLists[pos], size);//��ȡһ������ڴ�span

	//Ҫ��һ��span��������n��С�������(Ӳ������1����ʵ�ʶ�Ҫ)	
	start = span->_list;//����ڴ����ʼ��ַ
	end = start;//��ֹֻ��һ����������
		
	void *prev = start;
	size_t i = 1;
	while (i <= n && end!=nullptr)//��n-1��������end����Ϊ��
	{
		prev = end;
		end = NextObj(end);
		i++;
		span->_usecount++;//ÿȡ��һ��++
	}

	span->_list = end;//����spanlist��ʼλ��
	end = prev;
	NextObj(end) = nullptr;//�Ͽ���ԭ��Span������


	return i-1;//��ȡ�ĸ���
}


void CentralCache::ReleaseListToSpans(void* start, size_t byte_size)
{
	//�����Ӧ��span�����Ǹ�����֮��
	size_t pos = SizeClass::Index(byte_size);

	std::lock_guard<std::mutex> lock(_spanLists[pos]._mtx);//�ٽ���Դ������


	while (start)
	{
		void *next = NextObj(start);

		//��start�ڴ�������ĸ�span
		Span* span = PageCache::GetInstance()->MapObjectToSpan(start);

		//��start��Ӧ��С���ڴ棬ͷ����span֮��
		NextObj(start) = span->_list;
		span->_list = start;
		

		if (span->_usecount == 0)//˵��span���г�ȥ�Ķ���������,Ȼ�󻹸�����page
		{
			_spanLists[pos].Erase(span);//��spanlist֮�е���
			span->_list = nullptr;//�Ͽ�������span������
			PageCache::GetInstance()->ReleaseSpanToPageCache(span);

		}
		start = next;//�ٴζ���һ��С���ڴ���в���
	}
}