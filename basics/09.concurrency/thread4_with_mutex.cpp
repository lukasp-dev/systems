#include <iostream>
#include <thread> 
#include <vector>
#include <mutex>

/**
 * Mutext - Mutual Exclusion 
 * 
 * We will use a mutex to protect the shared_value
 * so that only one thread can access it at a time
 * 
 * sometimes mutex is also called a binary semaphore
 */

std::mutex gLock;
static int shared_value = 0;

void shared_value_increment(){
    gLock.lock();
    shared_value = shared_value + 1;
    shared_value = shared_value + 1;
    gLock.unlock();
}

int main() {
    std::vector<std::thread> threads;

    for(int i=0; i<10000; i++) {
        threads.push_back(std::thread(shared_value_increment));
    }

    for(auto &t : threads) {
        t.join();
    }

    std::cout << "Shared value: " << shared_value << std::endl;

    return 0;
}
/**
 * when ran, we always get the same value for shared_value
 * 
 * a@as-MacBook-Pro 09.concurrency % ./thread4_with_mutex
 * Shared value: 10000
 * a@as-MacBook-Pro 09.concurrency % ./thread4_with_mutex
 * Shared value: 10000
 * a@as-MacBook-Pro 09.concurrency % ./thread4_with_mutex
 * Shared value: 10000
 * a@as-MacBook-Pro 09.concurrency % ./thread4_with_mutex
 * Shared value: 10000
 * a@as-MacBook-Pro 09.concurrency % ./thread4_with_mutex
 * Shared value: 10000
 * a@as-MacBook-Pro 09.concurrency % ./thread4_with_mutex
 * Shared value: 10000
 * a@as-MacBook-Pro 09.concurrency % 
 */