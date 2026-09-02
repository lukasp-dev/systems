#include <iostream>
#include <thread> 
#include <vector>

// we will use the jthread instead of the thread in this program

int main() {
    auto lambda=[](int x) {
        std::cout << "Hello from lambda function" << std::endl;
        std::cout << "Argument passed in: " << x << std::endl;
    };

    // After C++20, we can use std::jthread instead of std::thread
    // std::jthread is a thread that will automatically join when it goes out of scope
    // so it thread.join() for us at the end of the current scope
    std::vector<std::jthread> jThreads;
    for(int i=0; i<10; i++) {
        jThreads.push_back(std::jthread(lambda, i));
    }

    std::cout << "Hello from the main thread" << std::endl;
}