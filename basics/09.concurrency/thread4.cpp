#include <iostream>
#include <thread>
#include <vector>

static int shared_value = 0;

void shared_value_increment() {
    shared_value = shared_value + 1;
}

int main() {
    std::vector<std::thread> threads;
    for(int i=0; i<10000; i++) {
        threads.push_back(std::thread(shared_value_increment));
    }
    for(auto &t : threads) {
        t.join();
    }

    std::cout << "Shared value:" << shared_value << std::endl;
    return 0;
}

/**
 * when ran, we see some of the different values for shared_value
 * This is called a race condition.
 * 
 * a@as-MacBook-Pro 09.concurrency % ./thread4
 * Shared value:10000
 * a@as-MacBook-Pro 09.concurrency % ./thread4
 * Shared value:9995
 * a@as-MacBook-Pro 09.concurrency % ./thread4
 * Shared value:10000
 * a@as-MacBook-Pro 09.concurrency % ./thread4
 * Shared value:9999
 * a@as-MacBook-Pro 09.concurrency % ./thread4
 * Shared value:10000
 * a@as-MacBook-Pro 09.concurrency % ./thread4
 */