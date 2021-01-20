//
// Created by lire on 8/19/20.
//

#include "Semaphore.h"
#include <limits.h>

//constructor
Semaphore::Semaphore() {
    this->count = 0;
    this->initialCount = 0;
    maxCount = INT_MAX;
}

void Semaphore::wait() {
    std::unique_lock<std::mutex> lock(mux);
    while (count == 0) {
        cv.wait(lock);
    }
    count--;
}

void Semaphore::signal() {
    std::unique_lock<std::mutex> lock(mux);
    if (count < maxCount) {
        count++;
    }
    cv.notify_one();
}
