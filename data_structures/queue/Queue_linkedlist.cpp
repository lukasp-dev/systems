#include <iostream>
#include <stdexcept>
using namespace std;

/**
 * @brief FIFO queue implemented with a singly linked list.
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
 * Implementation notes:
 * - head points to the front element.
 * - tail points to the back element.
 * - enqueue attaches a new node after tail.
 * - dequeue removes the head node.
 * - when the final node is removed, both head and tail become nullptr.
 * 
 * Time Complexity:
 * - enqueue: O(1)
 * - dequeue: O(1)
 * - front: O(1)
 * - empty: O(1)
 * - size: O(1)
 */

template <typename T>
class LinkedQueue {
private: 
    struct Node {
        T value;
        Node* next;
        
        Node(const T& value) {
            this->value = value;
            this->next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int count;

public:
    LinkedQueue(){
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    ~LinkedQueue() {
        while(!empty()){
            dequeue();
        }
    }

    void enqueue(const T& value){
        Node* new_node = new Node(value);

        if(empty()){
            head = new_node;
            tail = new_node;
        }else {
            tail->next = new_node;
            tail = new_node;
        }

        count++;
    }
    
    T dequeue(){
        if(empty()){
            throw runtime_error("The Queue is empty.");
        }

        Node* oldHead = head;
        T value = oldHead->value;

        head = head->next;
        delete oldHead;
        count--;

        if(count == 0){
            tail = nullptr;
        }
        
        return value;
    }
    
    T front() const {
        if(empty()){
            throw runtime_error("The Queue is empty.");
        }
        return head->value;
    }

    bool empty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }
};
