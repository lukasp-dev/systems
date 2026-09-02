/**
 * CONCURRANCY: Multiple things can happen at once, the order matters, and sometimes taks have to wait on shared resources.
 * PARALLELISM: Everything happens at once, instantaneously, and no waiting is required.
 */

 #include <iostream>
 #include <thread> // include the thread library

 // test function which we'll launch threads from
 void test(int x) {
    std::cout << "hello from the thread " << x << std::endl;
    std::cout << "argument passed to the thread: " << x << std::endl;
 }

 int main() {
    // createa new thread and pass one parameter
    std::thread t1(&test, 100);
    // join with the main thread, which is the same as
    // hey, ain thread, --wait until t1 is finished
    t1.join(); // main thread will wait for t1 to finish

    // continue executing the main thread
    std::cout << "hello from my main thread" << std::endl;

    return 0;
 }