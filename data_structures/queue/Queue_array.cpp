#include <iostream>
#include <stdexcept>
using namespace std;

/**
 * @brief FIFO queue implemented with a dynamuc circular array.
 * 
 * Supports insertion at the back and removal from the front.
 * 
 * Core behavior:
 * - enqueue(x): add x to the back of the queue.
 * - dequeue(): remove and return the front element.
 * - front(): return the front element without removing it.
 * - empty(): return whether the queue has no elements.
 * - size(): return the number of stored elements.
 * 
 * Implementation note:
 * - Uses a circular array to avoid shifting elements.
 * - frontIndex stores the physical index of the logical front.
 * - The next back position is (frontIndex + count) % capacity  
 *   As long as we know this, we don't have to have a standalone back pointer.
 * - When the array is full, capacity doubles and elemetns are copied in logical order.
 * 
 * Time Complexity:
 * - enqueue: O(1) amortized
 * - dequeue: O(1)
 * - front: O(1)
 * - empty: O(1)
 * - size: O(1)
 */

template <typename T>
class ArrayQueue{
private:
    T* data;
    int capacity;
    int count;
    int frontIndex;

    void resize(int newCapacity) {
        T* newData = new T[newCapacity];

        for(int i = 0; i < count; ++i){
            newData[i] = data[(frontIndex + i) % capacity];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity; 
        frontIndex = 0;
    }

public:
    ArrayQueue(int initialCapacity = 4) {
        capacity = initialCapacity;
        count = 0;
        frontIndex = 0;
        data = new T[capacity];
    }

    ~ArrayQueue(){
        delete[] data;
    }

    void enqueue(const T& value){
        if(count == capacity){
            resize(capacity * 2);
        }

        data[(frontIndex + count) % capacity] = value;
        count++;
    }

    T dequeue(){
        if(empty()){
            throw runtime_error("Queue is empty.");
        }
        
        T value = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return value;
    }

    T front() const {
        if(empty()){
            throw runtime_error("Queue is empty.");
        }
        return data[frontIndex];
    }

    bool empty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }
};

int main(){

    return 0;
}