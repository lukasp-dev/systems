#include <iostream>
#include <thread>
#include <vector>

/**
 * In this program, we will launch multiple threads
 * and see how they run in parallel.
 *  */

int main() {
    auto lambda=[](int x) {
        std::cout << "Hello from the thread" << std::this_thread::get_id() << x << std::endl;
        std::cout << "Argument passed in: "
 << x << std::endl;
    };

    std::vector<std::thread> threads;
    for(int i=0; i< 10; i++) {
        threads.push_back(std::thread(lambda, i));
    }

    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Hello from the main thread" << std::endl;

    return 0;
}