/*
	    _    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
	third party notice: Sebastian Ax
*/

#ifndef __SAFE_QUEUE_H_
#define __SAFE_QUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class SafeQueue
{
public:
	SafeQueue(void)
		: q()
		, m()
		, c()
	{}

	~SafeQueue(void)
	{}

	void enqueue(T t)
	{
		std::lock_guard<std::mutex> lock(m);
		q.push(t);
		c.notify_one();
	}

	T dequeue(void)
	{
		std::unique_lock<std::mutex> lock(m);
		while (q.empty())
		{
			c.wait(lock);
		}
		T val = q.front();
		q.pop();
		return val;
	}

	bool try_pop_result(T& r)
	{
		std::lock_guard<std::mutex> lock(m);
		if (!q.empty())
		{
			r = q.front();
			q.pop();
			return true;
		}
		return false;
	}

	bool try_pop_results(std::vector<T> &r, unsigned int current_size, unsigned int max_size)
	{
		max_size--;
		if (current_size >= max_size) return false;
		std::lock_guard<std::mutex> lock(m);
		if (q.empty()) return false;
		while (!q.empty())
		{
			T temp = q.front();
			int len = temp.ResultData.length() + 4;
			if (len + current_size > max_size) break;
			q.pop();
			r.push_back(temp);
			current_size += len;
		}
		return !r.empty();
	}

	size_t size()
	{
		std::lock_guard<std::mutex> lock(m);
		return q.size();
	}

private:
	std::queue<T> q;
	mutable std::mutex m;
	std::condition_variable c;
};
#endif