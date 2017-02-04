//header

#ifndef __PROCESSOR_H_
#define __PROCESSOR_H_

#include "SafeQueue.hpp"

class Extension;

template <class T, class U>
class Processor
{
public:

	Processor(Extension *p) : ext(p) {}

	void start(std::function<std::vector<U>(T)> f, int c)
	{
		if (started) return;

		int expected = std::thread::hardware_concurrency();

		if (c == -1 || c > (int)expected)
			c = expected;

#ifdef EXTENSION_DEBUG_OUTPUT_WINDOWS
		ext->console->info("Loading worker threads..");
#endif

		for (int i = 0; i < c; i++)
		{
			std::thread t(&Processor::run, this, f);
			t.detach();
		}

		started = true;

#ifdef EXTENSION_DEBUG_OUTPUT_WINDOWS
		ext->console->info("Worker threads loaded!");
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
		while (true)
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

	Extension *ext;

	SafeQueue<U> result_q;
	SafeQueue<T> request_q;
};
#endif
