
#include "PageCache.h"

PageCache PageCache::_sInst;//��������ʵ����

//��ϵͳ����Kҳ�ڴ�ҵ�����������
void* PageCache::SystemAllocPage(size_t k)
{
	return ::SystemAlloc(k);
}


Span* PageCache::NewSpan(size_t k)
{
	//�õݹ�������ֹ�Լ������Լ�������ѭ��
	std::lock_guard<std::recursive_mutex> lock(_mtx);

	//���ֱ���������NPAGES�Ĵ���ڴ棬ֱ����ϵͳҪ
	if (k >= NPAGES)
	{
		void *ptr = SystemAllocPage(k);
		Span* span = new Span;
		span->_pageId = (ADDRESS_INT)ptr >> PAGE_SHIFT;//���ҳ��
		span->_n = k;//���ҳ��

		_idSpanMap[span->_pageId] = span;//����ҳ����ҳ����ӳ���ϵ

		return span;
	}
	//���ж����ú��ʵ�λ����û���ڴ�
	if (!_spanList[k].Empty())//��Ϊ�գ���ʾ�����ڴ�飬ֱ�ӷ��ؼ���
	{
		return _spanList[k].PopFront();
	}

	//�Ҵ��ҳ�г�С��
	for (size_t i = k+1; i < NPAGES; ++i)
	{
		//��ҳ��С,�г�Kҳ��span����
		//ʣ��(i-k)ҳ�ҵ�(i-k)��λ����ȥ
		if (!_spanList[i].Empty())
		{
			//β�г�һ��Kҳ��span
			Span *span = _spanList[i].PopFront();

			Span *split = new Span;//���ص�ҳ
			split->_pageId = span->_pageId + span->_n - k;
			split->_n = k;

			//�ı��г�����ҳ�ź�span֮��Ĺ�ϵ
			for (PageID i = 0; i < k; i++)
			{
				_idSpanMap[split->_pageId + i] = split;
			}

			span->_n -= k;

			_spanList[span->_n].PushFront(span);//ʣ��Ĺһ�ȥ�����spanû�иı䣬ֱ�ӹҵ��µ�λ�ü���

			return split;//����һ��span��central cache
		}
	}

	//��һ�ν�������ҳ�ı�ȫΪ�գ���Ҫ��ϵͳҪ
	//����һ������ҳ��������128��λ��

	Span* bigSpan = new Span;//����һ���ڵ����

	//��ϵͳҪ�ڴ� -> Ҫ128ҳ
	void *memory = SystemAllocPage(NPAGES - 1);//��ΪҪ�и���Բ�����bigSpan�ڵ�֮��


	bigSpan->_pageId = (size_t)(memory) >> 12;//ҳ��Ϊ��ǰ�ĵ�ַ/4K
	bigSpan->_n = NPAGES - 1;//ҳ��

	//��ҳ�ź�span����ӳ���ϵ
	for (PageID i = 0; i < bigSpan->_n; i++)
	{
		PageID	id = bigSpan->_pageId + i;
		_idSpanMap[id] = bigSpan;
	}

		_spanList[NPAGES - 1].PushFront(bigSpan);//���ڴ�����Ӧ���б�֮��

	//�����Լ����ٴ�ִ�����̣���128���и�
	return NewSpan(k);
}	

//��ȡ�Ӷ���span��ӳ��
Span * PageCache::MapObjectToSpan(void* obj)
{
	//��ֹ���ｨ��ӳ���ϵ�������Ҳ���ý����޸�
	std::lock_guard<std::recursive_mutex> lock(_mtx);

	PageID id = (ADDRESS_INT)obj >> PAGE_SHIFT;//��ʾ�ڵڼ�ҳ
	auto ret = _idSpanMap.find(id);//�����Ƿ��ڹ�ϣ����

	if (ret != _idSpanMap.end())//������
	{
		return ret->second;//��ȡ��Ӧ��span
	}
	else//������϶���ĳ��������
	{
		assert(false);
		return nullptr;
	}

}


//�ͷſ���span�ص�Pagecache�����Һϲ����ڵ�span
void PageCache::ReleaseSpanToPageCache(Span* span)
{
	//����128ҳ��ֱ�Ӵ�ϵͳ����ģ�������ڴ滹��ϵͳ
	if (span->_n >= NPAGES)
	{
		//_idSpanMap.erase(span->_pageId);//�Ӷ�Ӧ��ӳ���ϵ֮��ɾ��
		_idSpanMap.erase(span->_pageId);

		void *ptr = (void*)(span->_pageId << PAGE_SHIFT);//ҳ��<<12 = ��ַ
		SystemFree(ptr);//�ͷ������ַ

		delete span;
		return;
	}

	std::lock_guard<std::recursive_mutex> lock(_mtx);


	//����ֱ����ϵͳ����ģ����ص�page��spanlist֮��

	//���ǰ�����sapnҳ�����кϲ�,����ڴ���Ƭ����
	//��ǰ�ϲ�
	while (true)
	{
		PageID preId = span->_pageId - 1;//ǰ��һ��ҳ��ҳ��
		auto ret = _idSpanMap.find(preId);

		//ǰ��һ��ҳ��span�����ڻ���δ����
		if (ret == _idSpanMap.end())
		{
			break;
		}

		Span* preSpan = ret->second;//�ҵ�ǰ���span
		
		//ǰ��ҳ�Ļ���ʹ����
		if (preSpan->_usecount != 0)//ǰ���ҳ����ʹ�ã����ܺϲ�
		{
			break;
		}

		//����128ҳ���ܺ�
		if (preSpan->_n + span->_n >= NPAGES)
		{
			break;
		}

		//�ߵ����˵��ǰ���ҳҲ�������ˣ��ǿ��Խ��кϲ���

		//�Ӷ�Ӧ��spanlist����ǰspan��ǰ���span������
		_spanList[preSpan->_n].Erase(preSpan);
		_spanList[span->_n].Erase(span);

		//�ϲ�span
		span->_pageId = preSpan->_pageId;
		span->_n += preSpan->_n;


		//����ҳ֮���ӳ���ϵ -> �ϲ�֮��ı�ӳ���ϵ
		for (PageID i = 0; i < preSpan->_n; i++)
		{
			_idSpanMap[preSpan->_pageId + i] = span;
		}

			delete preSpan;
	}

	//���ϲ�
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

		//����128ҳ���ܺ�
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

	//���ϲ�֮��Ĵ���ڴ棬���������spanlist֮��	
	_spanList[span->_n].PushFront(span);

}