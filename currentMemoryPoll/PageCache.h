#pragma once
#include "common.h"



class PageCache
{
public:
	//���ص�������
	static PageCache* GetInstance()
	{
		return &_sInst;
	}

	//��ϵͳ����Kҳ�ڴ�ҵ���������
	void* SystemAllocPage(size_t k);
	Span* NewSpan(size_t k);//����һ�����central cache
	
	//��ȡ�Ӷ���span��ӳ��
	Span *MapObjectToSpan(void *obj);

	//�ͷſ���span�ص�Pagecache�����Һϲ����ڵ�span
	void ReleaseSpanToPageCache(Span* span);

	std::recursive_mutex _mtx;//�����ģ������Ҳֻ��һ��

private:
	SpanList _spanList[129];// ��ҳ����ӳ�䣬0λ�ò��ã��±�Ϊ����Ͱ����ҵľ��Ǽ�ҳ��С���ڴ��

	std::unordered_map<PageID, Span*> _idSpanMap; //ʹ�û���������Ҫ����
	//TCMalloc_PageMap2<32-PAGE_SHIFT> _idSpanMap;


	//��Ƴɵ���ģʽ
	PageCache()
	{}

	static PageCache _sInst;
	PageCache(const PageCache &p) = delete;
	PageCache& operator =(PageCache&p) = delete;
};


