#include <algorithm>
#include <vector>
#include <stdexcept>

using namespace std;

class MinHeap {
private:
    void siftUp(int index){
        if(index <= 0) return;
        
        int parent_idx = (index - 1) / 2;
        if(heap[index] < heap[parent_idx]){
            swap(heap[index], heap[parent_idx]);
            
            siftUp(parent_idx);
        }
    }

    void siftDown(int index){
        int n = static_cast<int>(heap.size());
        if(index >= n) return;

        int left = index*2 + 1;
        int right = index*2 + 2;
        int smallest = index;

        if(left < n && heap[left] < heap[smallest]){
            smallest = left;
        }

        if(right < n && heap[right] < heap[smallest]){
            smallest = right;
        }

        if(smallest == index) return;

        swap(heap[smallest], heap[index]);
        siftDown(smallest);
    }

    vector<int> heap;
public:
    void push(int value){
        int n = static_cast<int>(heap.size());
        heap.push_back(value);
        siftUp(n);
    }
    void pop(){
        if(heap.empty()) throw out_of_range("MinHeap is empty");

        swap(heap[0], heap.back());
        heap.pop_back();
        
        if(!heap.empty()) siftDown(0);
    }
    int top() const {
        if(heap.empty()) throw out_of_range("MinHeap is empty");
        
        return heap[0];
    }
    bool empty() const {
        return heap.empty();
    }
    size_t size() const {
        return heap.size();
    }
};