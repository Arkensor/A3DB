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

template <class TRequest, class TResult>
class Processor
{
public:

	Processor(Extension *p) : extensionPtr(p)
	{
		static_assert(std::is_base_of<ResultBase, TResult>::value, "TResult must derive from ResultBase");
	}

	void start(std::function<std::vector<TResult>(TRequest)> f, int c)
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

	void add(TRequest r)
	{
		request_q.enqueue(r);
	}

	bool try_get_result(TResult& r)
	{
		return result_q.try_pop_result(r);
	}

	bool try_get_results(std::vector<TResult>& r, unsigned int current_size, unsigned int max_size)
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

	bool started = false;

	Extension *extensionPtr;

	SafeQueue<TResult> result_q;
	SafeQueue<TRequest> request_q;

	void run(std::function<std::vector<TResult>(TRequest)> f)
	{
		while (!extensionPtr->shutDown && extensionPtr->allGood)
		{
			TRequest arg = request_q.dequeue();
			std::vector<TResult> results = f(arg);
			for (TResult result : results)
			{
				result_q.enqueue(result);
			}
		}
	}
};
#endif