/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

#ifndef __PROCESSOR_H_
#define __PROCESSOR_H_

#include <thread>

#include "SafeQueue.hpp"
#include "Extension.hpp"

template <class T, class U>
class Processor
{
public:

	Processor(Extension *p) : extensionPtr(p) {}

	void start(std::function<std::vector<U>(T)> f, int c)
	{
		if (started) return;

		int expected = std::thread::hardware_concurrency();

		if (c == -1 || c > (int)expected)
			c = expected;

		#ifdef CONSOLE_DEBUG
		extensionPtr->console->info("Loading worker threads..");
		#endif

		for (int i = 0; i < c; i++)
		{
			std::thread t([=] {run(f);});
			t.detach();
		}

		started = true;

		#ifdef CONSOLE_DEBUG
		extensionPtr->console->info("Worker threads loaded!");
		#endif
	}

	void add(T r)
	{
		request_q.enqueue(r);
	}

	bool try_get_result(U& r)
	{
		return result_q.try_pop_result(r);
	}

	bool try_get_results(std::vector<U>& r, unsigned int current_size, unsigned int max_size)
	{
		return result_q.try_pop_results(r, current_size, max_size);
	}

	size_t request_size()
	{
		return result_q.size();
	}

	size_t result_size()
	{
		return result_q.size();
	}

private:

	void run(std::function<std::vector<U>(T)> f)
	{
		while (!extensionPtr->shutDown && extensionPtr->allGood)
		{
			T arg = request_q.dequeue();
			std::vector<U> results = f(arg);
			for (U result : results)
			{
				result_q.enqueue(result);
			}
		}
	}

	bool started = false;

	Extension *extensionPtr;

	SafeQueue<U> result_q;
	SafeQueue<T> request_q;
};
#endif