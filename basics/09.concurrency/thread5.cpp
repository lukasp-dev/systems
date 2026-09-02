/**
 * what if in the previous example, I commented out the gLock.unlock();?
 * the thread will never release the lock, and the main thread will be blocked
 * That is called a deadlock. (Lock is never released)
 * 
 */

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

static int shared_value = 0;
std::mutex gLock;

void shared_value_increment(){
    // lock_guard is a wrapper around the mutex that ensures the mutex is unlocked when the lock_guard goes out of scope
    // so it's like RAII (Resource Acquisition Is Initialization)
    // if I didn't use lock_guard, I would have to manually unlock in both
    // after the catch block and inside the try block.
    std::lock_guard<std::mutex> lock(gLock);
    // gLock.lock();
    try {
        shared_value = shared_value + 1;
        throw std::runtime_error("Error: test");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // gLock.unlock();
    }
    // gLock.unlock();
}

int main() {

    std::vector<std::thread> threads;
    for(int i=0; i<10; i++) {
        threads.push_back(std::thread(shared_value_increment));
    }
    for(auto &t : threads) {
        t.join();
    }

    std::cout << "Shared value: " << shared_value << std::endl;

    return 0;
}