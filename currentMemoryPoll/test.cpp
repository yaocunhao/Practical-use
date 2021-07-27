#include "ConcurrentAlloc.h"

#include "ConcurrentAlloc.h"

void TestMalloc(size_t ThreadNum, size_t Number, size_t round)
{
	//ThreadNum���̡߳�ÿ���߳���Number�֡�ÿ������round�οռ��ͷ�round�οռ�

	std::vector<std::thread> Thread(ThreadNum);//threadNum���߳�
	size_t MallocTime = 0;
	size_t FreeTime = 0;

	//ThreadNum���߳�
	for (size_t i = 0; i < ThreadNum; i++)
	{
		Thread[i] = std::thread([&]
		{
			std::vector<void*> Memvector;//���濪�ٳ����Ŀռ�ĵ�ַ
			Memvector.resize(round);

			for (size_t j = 0; j < Number; j++)//ÿ���߳���Number��
			{
				//ÿ���߳�����round���ڴ�
				size_t begin1 = clock();
				for (size_t k = 0; k < round; k++)
				{
					Memvector[k] = malloc(100);
				}
				size_t end1 = clock();


				//�ͷ�round���ڴ�
				size_t begin2 = clock();
				for (size_t k = 0; k < round; k++)
				{
					free(Memvector[k]);
				}
				size_t end2 = clock();

				Memvector.clear();

				MallocTime += end2 - begin1;//[]���ʽ��׽�ı�������Ҷ�������
				FreeTime += end1 - begin1;

			}
		});
	}

	for (auto&e : Thread)
	{
		e.join();
	}

	printf("%u���̲߳���ִ��%u�ִΣ�ÿ�ִ�malloc %u��: ���ѣ�%u ms\n", ThreadNum, round, Number, MallocTime);

	printf("%u���̲߳���ִ��%u�ִΣ�ÿ�ִ�free %u��: ���ѣ�%u ms\n", ThreadNum, round, Number, FreeTime);

	printf("%u���̲߳���ִ��malloc��free %u�Σ��ܼƻ��ѣ�%u ms\n", ThreadNum, ThreadNum*round*Number, MallocTime + FreeTime);
}


void TestConcurrentAlloc(size_t ThreadNum, size_t Number, size_t round)
{
	std::vector<std::thread> Thread(ThreadNum);//threadNum���߳�
	size_t MallocTime = 0;
	size_t FreeTime = 0;

	//ThreadNum���߳�
	for (size_t i = 0; i < ThreadNum; i++)
	{
		Thread[i] = std::thread([&]
		{
			std::vector<void*> Memvector;//���濪�ٳ����Ŀռ�ĵ�ַ
			Memvector.resize(round);

			for (size_t j = 0; j < Number; j++)//ÿ���߳���Number��
			{
				//ÿ���߳�����round���ڴ�
				size_t begin1 = clock();
				for (size_t k = 0; k < round; k++)
				{
					Memvector[k] = ConcurrentAlloc(100);
				}
				size_t end1 = clock();


				//�ͷ�round���ڴ�
				size_t begin2 = clock();
				for (size_t k = 0; k < round; k++)
				{
					ConcurrentFree(Memvector[k]);
				}
				size_t end2 = clock();

				Memvector.clear();

				MallocTime += end2 - begin1;//[]���ʽ��׽�ı�������Ҷ�������
				FreeTime += end1 - begin1;

			}
		});
	}

	for (auto&e : Thread)
	{
		e.join();
	}

	printf("%u���̲߳���ִ��%u�ִΣ�ÿ�ִ�malloc %u��: ���ѣ�%u ms\n", ThreadNum, round, Number, MallocTime);

	printf("%u���̲߳���ִ��%u�ִΣ�ÿ�ִ�free %u��: ���ѣ�%u ms\n", ThreadNum, round, Number, FreeTime);

	printf("%u���̲߳���ִ��malloc��free %u�Σ��ܼƻ��ѣ�%u ms\n", ThreadNum, ThreadNum*round*Number, MallocTime + FreeTime);
}

int main()
{
	cout << "-------------------------------Malloc-------------------------------------" << endl;
	TestMalloc(6, 6, 2000);
	cout << endl << endl;

	cout << "-------------------------CouncurrentAlloc---------------------------------" << endl;

	TestConcurrentAlloc(6, 6, 2000);


	getchar();
	return 0;
}
