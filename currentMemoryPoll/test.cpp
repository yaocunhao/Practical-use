#include "ConcurrentAlloc.h"

#include "ConcurrentAlloc.h"

void TestMalloc(size_t ThreadNum, size_t Number, size_t round)
{
	//ThreadNum个线程、每个线程跑Number轮、每轮申请round次空间释放round次空间

	std::vector<std::thread> Thread(ThreadNum);//threadNum个线程
	size_t MallocTime = 0;
	size_t FreeTime = 0;

	//ThreadNum个线程
	for (size_t i = 0; i < ThreadNum; i++)
	{
		Thread[i] = std::thread([&]
		{
			std::vector<void*> Memvector;//保存开辟出来的空间的地址
			Memvector.resize(round);

			for (size_t j = 0; j < Number; j++)//每个线程跑Number次
			{
				//每个线程申请round次内存
				size_t begin1 = clock();
				for (size_t k = 0; k < round; k++)
				{
					Memvector[k] = malloc(100);
				}
				size_t end1 = clock();


				//释放round次内存
				size_t begin2 = clock();
				for (size_t k = 0; k < round; k++)
				{
					free(Memvector[k]);
				}
				size_t end2 = clock();

				Memvector.clear();

				MallocTime += end2 - begin1;//[]表达式捕捉的变量，大家都可以用
				FreeTime += end1 - begin1;

			}
		});
	}

	for (auto&e : Thread)
	{
		e.join();
	}

	printf("%u个线程并发执行%u轮次，每轮次malloc %u次: 花费：%u ms\n", ThreadNum, round, Number, MallocTime);

	printf("%u个线程并发执行%u轮次，每轮次free %u次: 花费：%u ms\n", ThreadNum, round, Number, FreeTime);

	printf("%u个线程并发执行malloc和free %u次，总计花费：%u ms\n", ThreadNum, ThreadNum*round*Number, MallocTime + FreeTime);
}


void TestConcurrentAlloc(size_t ThreadNum, size_t Number, size_t round)
{
	std::vector<std::thread> Thread(ThreadNum);//threadNum个线程
	size_t MallocTime = 0;
	size_t FreeTime = 0;

	//ThreadNum个线程
	for (size_t i = 0; i < ThreadNum; i++)
	{
		Thread[i] = std::thread([&]
		{
			std::vector<void*> Memvector;//保存开辟出来的空间的地址
			Memvector.resize(round);

			for (size_t j = 0; j < Number; j++)//每个线程跑Number次
			{
				//每个线程申请round次内存
				size_t begin1 = clock();
				for (size_t k = 0; k < round; k++)
				{
					Memvector[k] = ConcurrentAlloc(100);
				}
				size_t end1 = clock();


				//释放round次内存
				size_t begin2 = clock();
				for (size_t k = 0; k < round; k++)
				{
					ConcurrentFree(Memvector[k]);
				}
				size_t end2 = clock();

				Memvector.clear();

				MallocTime += end2 - begin1;//[]表达式捕捉的变量，大家都可以用
				FreeTime += end1 - begin1;

			}
		});
	}

	for (auto&e : Thread)
	{
		e.join();
	}

	printf("%u个线程并发执行%u轮次，每轮次malloc %u次: 花费：%u ms\n", ThreadNum, round, Number, MallocTime);

	printf("%u个线程并发执行%u轮次，每轮次free %u次: 花费：%u ms\n", ThreadNum, round, Number, FreeTime);

	printf("%u个线程并发执行malloc和free %u次，总计花费：%u ms\n", ThreadNum, ThreadNum*round*Number, MallocTime + FreeTime);
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
