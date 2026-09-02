/**
 * CONCURRANCY: Multiple things can happen at once, the order matters, and sometimes taks have to wait on shared resources.
 * PARALLELISM: Everything happens at once, instantaneously, and no waiting is required.
 */

 #include <iostream>
 #include <thread> // include the thread library



 int main() {

    // lambda function
    auto lambda=[](int x) {
        std::cout << "hello from the thread " << x << std::endl;
        std::cout << "argument passed to the thread: " << x << std::endl;
    };

    // createa new thread and pass one parameter
    // pass the lambda function to the thread
    // do not pass &lambda, because lambda is already a pointer
    std::thread t1(lambda, 100); 
    // join with the main thread, which is the same as
    // hey, ain thread, --wait until t1 is finished
    t1.join(); // main thread will wait for t1 to finish

    // continue executing the main thread
    std::cout << "hello from my main thread" << std::endl;

    return 0;
 }