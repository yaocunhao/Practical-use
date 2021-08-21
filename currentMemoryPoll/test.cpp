#include "ConcurrentAlloc.h"

void BenchmarkMalloc(size_t ntimes, size_t nworks, size_t rounds)
{
	// nworks个线程
	//每个线程跑rounds轮
	//每个线程跑ntimes次

	std::vector<std::thread> vthread(nworks);// nworks个线程，先用数组保存起来
	size_t malloc_costtime = 0;
	size_t free_costtime = 0;

	for (size_t k = 0; k < nworks; ++k)
	{
		vthread[k] = std::thread([&]()
		{
			std::vector<void*> v;
			v.reserve(ntimes);

			for (size_t j = 0; j < rounds; ++j)//每个线程跑rounds轮
			{
				size_t begin1 = clock();
				for (size_t i = 0; i < ntimes; i++)//每个线程跑ntimes次
				{
					v.push_back(malloc(260));//线程开辟空间
				}
				size_t end1 = clock();

				size_t begin2 = clock();
				for (size_t i = 0; i < ntimes; i++)	//空间的销毁
				{
					free(v[i]);
				}
				size_t end2 = clock();
				v.clear();

				malloc_costtime += end1 - begin1;//[]表达式捕捉的变量，大家都可以用
				free_costtime += end2 - begin2;
			}
		});
	}



	for (auto& t : vthread)//等待线程
	{
		t.join();
	}

	printf("%u个线程并发执行%u轮次，每轮次malloc %u次: 花费：%u ms\n", nworks, rounds, ntimes, malloc_costtime);

	printf("%u个线程并发执行%u轮次，每轮次free %u次: 花费：%u ms\n", nworks, rounds, ntimes, free_costtime);

	printf("%u个线程并发malloc&free %u次，总计花费：%u ms\n", nworks, nworks*rounds*ntimes, malloc_costtime + free_costtime);
}


// 单轮次申请释放次数 线程数 轮次
void BenchmarkConcurrentMalloc(size_t ntimes, size_t nworks, size_t rounds)
{
	std::vector<std::thread> vthread(nworks); // n
	size_t malloc_costtime = 0;
	size_t free_costtime = 0;

	for (size_t k = 0; k < nworks; ++k)
	{
		vthread[k] = std::thread([&]() {
			std::vector<void*> v;
			v.reserve(ntimes);

			for (size_t j = 0; j < rounds; ++j)
			{
				size_t begin1 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					v.push_back(ConcurrentAlloc(260));
				}
				size_t end1 = clock();

				size_t begin2 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					ConcurrentFree(v[i]);
				}
				size_t end2 = clock();
				v.clear();

				malloc_costtime += end1 - begin1;
				free_costtime += end2 - begin2;
			}
		});
	}

	for (auto& t : vthread)
	{
		t.join();
	}

	printf("%u个线程并发执行%u轮次，每轮次concurrent alloc %u次: 花费：%u ms\n",
		nworks, rounds, ntimes, malloc_costtime);

	printf("%u个线程并发执行%u轮次，每轮次concurrent dealloc %u次: 花费：%u ms\n",
		nworks, rounds, ntimes, free_costtime);

	printf("%u个线程并发concurrent alloc&dealloc %u次，总计花费：%u ms\n",
		nworks, nworks*rounds*ntimes, malloc_costtime + free_costtime);
}

int main()
{
	cout << "==========================================================" << endl;
	BenchmarkMalloc(1000, 6, 6);
	cout << endl << endl;

	BenchmarkConcurrentMalloc(1000, 6, 6);
	cout << "==========================================================" << endl;

	getchar();
	return 0;
}

