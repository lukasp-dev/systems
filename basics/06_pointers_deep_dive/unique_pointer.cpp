#include <bits/stdc++.h>
using namespace std;

template<typename T>
class unique_pointer {
    T* m_ptr = nullptr;

public:
    unique_pointer() = default;
    explicit unique_pointer(T* ptr) : m_ptr(ptr) {}

    // 막는 이유: unique_pointer는 한 resource에 owner가 하나만 있어야 하기 때문
    // copy constructor 막기
    /**
     * A a;
     * A b(a); 이런 상황을 막기 위함.
     * // b를 새로 만들면서 a를 복사하려는 상황
     */
    unique_pointer(const unique_pointer&) = delete;
    // copy assignment operator 막기
    /**
     * A a;
     * A b;
     * a = b; 이런 상황을 막기 위함.
     * // 이미 존재하는 a에 b를 복사해서 대입하려는 상황
     */
    unique_pointer& operator=(const unique_pointer&) = delete;

    unique_pointer(unique_pointer&& src) : m_ptr(src.m_ptr) {
        src.m_ptr = nullptr;
    }

    unique_pointer& operator=(unique_pointer&& src) {
        if(this != &src) {
            delete m_ptr;
            m_ptr = src.m_ptr;
            src.m_ptr = nullptr;
        }

        return *this;
    }

    ~unique_pointer() {
        delete m_ptr;
    }

    T* operator->() {
        return m_ptr;
    }
};



int main(){
    unique_pointer<int> up1(new int(2));
    
    // cannot do this
    // unique_pointer<int> up2 = up1;

    auto up2 = move(up1);
    return 0;
}