#pragma once
#include "common.h"

class CentralCache
{
public:

	//���ص�������
	static CentralCache* GetInstance()
	{
		return &_sInst;
	}

	// ��Span֮�н�ȡn����СΪsize�Ķ����thread cache
	//start��end����Ͳ�����startΪ��ȡ�ڴ����㣬endΪ��ȡ�ڴ���յ�
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t size);

	// ��SpanList����page cache��ȡһ��span   
	Span* GetOneSpan(SpanList &list, size_t size);

	// ��һ�������Ķ����ͷŵ�span���
		void ReleaseListToSpans(void* start, size_t byte_size);

private:
	SpanList _spanLists[NFREELISTS];//�����뷽ʽ����ӳ��
	
	//����ģʽ
	CentralCache()
	{}

	CentralCache(const CentralCache&c) = delete;
	CentralCache& operator=(CentralCache& c) = delete;


	static CentralCache _sInst;//����һ��ȫ�ֵ����Ļ���Ķ��󣬶���̵߳��õ���ͬһ�����Ļ���
};

