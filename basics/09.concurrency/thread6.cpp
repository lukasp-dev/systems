// In this program, we will use atomic
#include <iostream>
#include <vector>
#include <atomic>
#include <thread>

std::atomic<int> shared_value = 0;

int main() {
    auto increment_shared=[]() {
        shared_value++;
    };

    std::vector<std::thread> threads;
    for(int i=0; i<10000; i++) {
        threads.push_back(std::thread(increment_shared));
    }

    for(auto& t : threads) {
        t.join();
    }

    std::cout << shared_value << "\n";

    return 0;
}