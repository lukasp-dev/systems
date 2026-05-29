# RAII

### Resource Acquisition Is Initialization
C++ 의 창시자인 Bjarne Stroustrup이 고안한 핵심 프로그래밍 디자인 패턴, 자원(Resource)의 수명을 객체(Object)의 수명과 일치시키는 기법. 

### 1. 자원의 획득은 생성자에서 이루어진다.
### 2. 자원의 해제는 소멸자에서 이루어진다.

``` cpp
#include <iostream>

class Collection{
    public:
        Collection(){
            data = new int[10];
        }

        ~ Collection(){
            delete[] data;
        }

        int& operator[] (size_t index) {
            return data[index];
        }

        const int& operator[] (size_t index) const {
            return data[index];
        }

    private:
        int* data;
};

// 데이터를 읽기만 하는 함수
void printFirstElement(const Collection& c){
    //  const 버전이 compiler 에 의해 자동으로 호출됨
    std::cout << "The first element: " << c[0] << "\n";
}

int main() {
    Collection my_collection;

    my_collection[0] = 7;
    my_collection[1] = 99;
    
    printFirstElement(my_collection);
    
    return 0;
}
```


C++ reference 예시 코드
```cpp
std::mutex m;

void bad() 
{
    m.lock();             // acquire the mutex
    f();                  // if f() throws an exception, the mutex is never released
    if (!everything_ok())
        return;           // early return, the mutex is never released
    m.unlock();           // if bad() reaches this statement, the mutex is released
}

void good()
{
    std::lock_guard<std::mutex> lk(m); // RAII class: mutex acquisition is initialization
    f();                               // if f() throws an exception, the mutex is released
    if (!everything_ok())
        return;                        // early return, the mutex is released
}                                      // if good() returns normally, the mutex is released
```