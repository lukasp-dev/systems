#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class ArrayList{
int capacity;
int size;
T* arr;

void regrow(){
    int newCapacity = capacity*2;
    T* newArr = new T[newCapacity];
    for(int i=0; i<size; ++i){
        newArr[i] = arr[i];
    }

    delete[] arr;
    arr = newArr;
    capacity = newCapacity;
}

public:
ArrayList ():capacity{10}, size{0}, arr{new T[10]} {}
ArrayList(const int& capacity):capacity{capacity}, size{0}, arr{new T[capacity]}{}
~ArrayList(){
    delete[] this->arr;
}

void addFront(T data){
    addAtIndex(0, data);
}

void addBack(T data){
    addAtIndex(size, data);
}

void addAtIndex(int idx, T data){
    if(idx < 0 || idx > size) throw out_of_range("bad index");
    if(size == capacity) regrow();

    for(int i=size; i>idx; --i){
        arr[i] = arr[i-1];
    }

    arr[idx] = data;
    ++size;
}

T removeFront(){
    return removeAtIndex(0);
}

T removeBack(){
    return removeAtIndex(size-1);
}

T removeAtIndex(int idx){
    if(idx < 0 || idx >= size) throw out_of_range("bad index");

    T removed = arr[idx];
    
    for(int i=idx; i<size-1; ++i){
        arr[i] = arr[i+1];
    }

    --size;
    return removed;
}

/**
 * Prints the current status of the Arraylist
 * 
 * Caution: this is only valid for std::streamable.
 * If not std::streamable, have to overload the << operator.
 */
void printStatusQuo(){
    T* tmp = arr;
    for(int i = 0; i < size; ++i){
            cout << *(tmp + i) << " "; 
        }
        cout << "\n";
    }
};

int main() {
    try {
        ArrayList<int> al; 
        al.addBack(10);  
        al.addAtIndex(1, 23);
        al.addBack(20);  
        al.addFront(5);

        al.printStatusQuo();

        cout << "Removed: " << al.removeFront() << endl; 
        cout << "Back: " << al.removeBack() << endl;

        al.printStatusQuo();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}