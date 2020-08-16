#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore {
	std::mutex mux;
	std::condition_variable cv;
	int count;
	int maxCount;

public:
	void wait();
	void signal();
	Semaphore(int count, int maxCount); //maxCount is the max value the semaphore can assume (binary semaphore if maxCount = 1)
	Semaphore(int count); //maxCount = INT_MAX
	Semaphore(); //count=0 maxCount=INT_MAX
};
