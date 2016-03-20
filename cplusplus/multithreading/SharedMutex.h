#pragma once
#include <mutex>
#include <condition_variable>

class SharedMutex
{
	std::mutex m;
	std::condition_variable cond_var;
	int nReaders = 0;
	bool hasWriter = false;
public:
	SharedMutex() {
	}

	void lock() {
		std::unique_lock<std::mutex> lock(m);
		while (hasWriter || nReaders > 0)
			cond_var.wait(lock);
		hasWriter = true;
	}

	void shared_lock() {
		std::unique_lock<std::mutex> lock(m);
		while(hasWriter)
			cond_var.wait(lock);
		nReaders++;
	}

	void unlock() {
		std::unique_lock<std::mutex> lock(m);
		if (!hasWriter)
			throw std::logic_error("not locked");
		hasWriter = false;
		cond_var.notify_all();
	}

	void shared_unlock() {
		std::unique_lock<std::mutex> lock(m);
		if (nReaders == 0)
			throw std::logic_error("not locked");
		nReaders--;
		cond_var.notify_all();
	}

};
