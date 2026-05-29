### lvalue and rvalue

**lvalue**: objects(object is a region of storage in the execution environment that can store a value. -> any type, including primitive types like `int` and `float`, as well as user-defined types like classes and structs are technically objects.) that have an identifiable, persistent memory location and can appear on both the left and right sides of an assignment.

**rvalue**: temporary values or objects that do not persist beyond the expression that uses them and can only appear on the right side of an assignment.
-> 딱 그 줄에서만 연산을 위해 임시로 만들어졌다가, 세미콜론(`;`)을 찍고 다음 줄로 넘어가면 메모리에서 즉시 사라지는 값.

When C++ gets an parameter, it can specify how to get one.
1. `void func(int& a)` means it will get only **lvalues** as parameters, meaning no rvalue can be passed as the parameter.
2. `void func(int&& a)` means it will get only **rvalues** as parameters, meaning no lvalue can be passed as the parameter.

Most of the times, **number 1** is true, except when `const` keyword is appended at front. so for example, `void func(const int& a)`.

```cpp
#include <iostream>
#include <string>

void func(int& a){
    cout << "this function was fired~!" << "\n";
}

int main(){
    func(10); // error: expects an lvalue for 1st argument.
    return 0;
}
```

위 예시와는 다르게 `const int& a` 같은식으로 매개변수를 받으면 `rvalue` 도 들어올 수 있다. 왜냐하면 `const`를 붙인다는 것은 컴파일러에게 "I am not going to modify this data" 라고 Read-Only를 선언하는 것과 같기 때문이다. 컴파일러는 원래라면 즉시 소멸시켜야 할 그 임시 값을 위해 메모리 한 칸에 임시 공간을 마련 하고, 그 방의 주소를 함수가 끝날 때까지 유지시켜 준다.

결국은 성능 때문인데, C++ 설계짜들은 "값을 수정하지 않을 거라면, 굳이 임시 객체를 무겁게 복사하지 말고 참조만 쓱 당겨서 쓰게 해주자" 라고 판단한거임.

Also, **number 2** is true, except for when we use `std::move()`. 

```cpp
std::vector<double> large_data = loadMarketData();
// Case A: Copy(Slow)
// std::vector<double> target = large_data;
// -> copies the entire data one by one. memory is wasted.

// Case B: Move (Fast)
std::vector<double> target = std::move(large_data);
// now, target is the new owner of this data.
// now target holds the start address of the object in the heap.
// large_data's size is now 0.
```

C++14 부터 move 의 definition 
```cpp
template< class T >
constexpr std::remove_reference_t<T>&& move(T && t) noexcept;
```
1. `T&& t` gate
`T &&` 는 우측값만 받는 게 아니고, Forwarding Reference 라는 universal 입구임.
parameter 로 어떤 종류의 값이 들어오는지에 따라 T의 타입 추론이 다르게 되면서 정해진 룰셋에 따라 결국 원래의 참조값을 유지해줌.

    e.g.) if the parameter was lvalue(`int x = 10;`) `T` becomes `int&` and `int&` +`&&` is lvalue 

    e.g.) if the parameter was rvalue(`10`) `T` becomes `int` and `int` + `&&` is rvalue.

2. `std::remove_reference_t<T>`

    이거는 `std::move` 의 심장과도 같음. 만약 우리가 `int x = 10` 같은 거를 넘겼다고 치면, `T`는 `int&`가 됨. 우리의 목표(move의 목표는) 이 친구를 우측값으로 바꾸는거니까 컴파일러한테 이렇게 명령하는거임. "야, `T`에 `&`든 `&&`든 뭐가 붙어있든 간에 일단 다 떼어버려! 순수한 알맹이 타입만 남겨!"

    e.g.) `remove_reference_t<int&>` -> `int`

    e.g.) `remove_reference_t<int&&>` -> `int`

3. `&&` 우측값 강제 casting.

    2번에서 reference 껍데기가 다 벗겨진 순수한 `int` 알맹이가 나왔으면, 그 위로 `&&`를 붙여주는거임.


| 호출 형태 (Argument) | `void f(int&)` | `void f(const int&)` | `void f(int&&)` | **최종 선택 (Best Match)** |
| :--- | :---: | :---: | :---: | :--- |
| **`int x = 10; f(x);`** (Lvalue) | **1순위** | 2순위 | 호출 불가 | **`f(int&)`** |
| **`f(10);`** (Rvalue) | 호출 불가 | 2순위 | **1순위** | **`f(int&&)`** |
| **`const int y = 20; f(y);`** | 호출 불가 | **1순위** | 호출 불가 | **`f(const int&)`** |
| **`f(std::move(x));`** | 호출 불가 | 2순위 | **1순위** | **`f(int&&)`** |

| Feature | `int* ptr = new int[1000];` (Raw Pointer) | `std::vector<int> v(1000);` (Object/RAII) |
| :--- | :--- | :--- |
| **Stack Occupancy** | Exactly **8 bytes** (Stores only the memory address) | Typically **24-32 bytes** (Address + Size + Capacity) |
| **Ownership** | **None.** It is just a signpost pointing to a location. | **Strong.** It acts as the owner/manager of the allocated memory. |
| **Auto Cleanup** | **No.** Manual `delete[]` is required, or it causes a **Memory Leak**. | **Yes.** The **Destructor** automatically frees memory when it goes out of scope. |
| **Copy Behavior** | **Shallow Copy.** Only the address is copied, leading to potential "Double Free" errors. | **Deep Copy.** The entire data is duplicated. Supports **Move Semantics** for efficiency. |
| **Safety** | High risk of memory leaks and dangling pointers. | High safety through automated lifecycle management. |

---

#### move example

```cpp
#include <iostream>
#include <vector>
#include <utility> // need this for std::move

class DataManager{
public:
    void processData(std::vector<double>&& data){
        std::cout << "=== Inside processData (RValue version) ---" << std::endl;

        my_storage = std::move(data);
        std::cout << "Data moved to storage. Size: " << my_storage.size() << std::endl;
    }

private:
    std::vector<double> my_storage;
};

int main(){
    std::vector<double> large_lvalue(100000, 1.23);
    std::cout << "Original lvalue size: " << large_lvalue.size() << std::endl;

    DataManager manager;

    manager.processData(std::move(large_lvalue));

    std::cout << "--- After Move ---" << std::endl;
    std::cout << "Original lvalue size: " << large_lvalue.size() << std::endl;

    return 0;
}

// Original lvalue size: 100000
// === Inside processData (RValue version) ---
// Data moved to storage. Size: 100000
// --- After Move ---
// Original lvalue size: 0
```

### `move` 에 대하여 
물리적으로는 주소가 존재함에도 불구하고, 언어 차원에서 주소 접근을 막아버림으로써 이를 `Rvalue`로 취급하는 것.
(Despite having a persistent memory location in physical terms, the language specification forbids taking its address, effectively categorizing it as an rvalue.)

More specifically, 
"In C++, an **rvalue** is essentially an expression that does not have a persistent **identity** in the eyes of the compiler. When we use `std::move(x)`, we are not physically moving the object in memory; rather, we are **casting** it to an **xvalue** (expiring value).

Even though the object still resides at the same **physical memory address**, the C++ type system treats the result of `std::move` as an anonymous temporary. By **restricting address-of operations** on this result, the language ensures that the object's resources can be safely 'stolen' or moved without the risk of the original identifier being misused."

**another example of `move`**
```cpp
#include <iostream>
#include <cstdint>
#include <cstring>
#include <utility> // std::move 사용을 위해 명시적으로 포함

class String {
public:
    String() : m_Data(nullptr), m_Size(0) {} // 기본 생성자에서 멤버 변수 안전하게 초기화

    String(const char* string) {
        printf("Created!\n");
        m_Size = strlen(string); // \0 제오하고 진짜 글자 수만 셈.
        
        // ⭕ 수정 1: C스타일 문자열은 끝에 널 문자('\0')가 필요하므로 m_Size + 1 크기로 할당
        m_Data = new char[m_Size + 1]; 
        memcpy(m_Data, string, m_Size);
        m_Data[m_Size] = '\0'; // 맨 끝에 널 문자 명시적으로 삽입
    }
    
    // 이동 생성자 (Move Constructor)
    String(String&& other) noexcept {
        printf("Moved!\n");
        m_Size = other.m_Size;
        m_Data = other.m_Data; // 원본의 주소값을 쏙 뺏어옴
        
        // 원본 빈 껍데기로 만들기
        other.m_Size = 0;
        other.m_Data = nullptr;
    }
    
    // 복사 생성자 (Copy Constructor)
    String(const String& other) {
        printf("Copied!\n");
        m_Size = other.m_Size;
        
        // ⭕ 수정 1 반영: 복사할 때도 안전하게 m_Size + 1 크기로 힙 할당
        m_Data = new char[m_Size + 1]; 
        memcpy(m_Data, other.m_Data, m_Size);
        m_Data[m_Size] = '\0';
    }
    
    void Print() const { // 읽기 전용 함수이므로 const 추가
        // 0번 주소가 들어오는 nullptr 데레퍼런스 방지 안전장치
        if (!m_Data) {
            printf("[Empty String]\n");
            return;
        }

        for (uint32_t i = 0; i < m_Size; ++i) {
            printf("%c", m_Data[i]);
        }
        printf("\n");
    }
    
    ~String() {
        printf("deleted!\n");
        // ⭕ 수정 2: new[] 배열로 할당한 메모리는 반드시 delete[] 로 해제해야 합니다!
        // (C++에서 delete에 nullptr을 넣는 것은 안전하므로 별도의 if문 체크는 필요 없습니다.)
        delete[] m_Data; 
    }

private:
    char* m_Data = nullptr; // 선언과 동시에 nullptr 초기화 습관화
    uint32_t m_Size = 0;
};

class Entity {
public:
    // 인자 값을 그대로 복사하는 경우
    Entity(const String& name)
        : m_Name(name) {}
    
    // 인자 값이 임시 객체(rvalue)여서 이사를 보내는 경우
    Entity(String&& name) 
        : m_Name(std::move(name)) {} // name 자체는 lvalue이므로 다시 std::move로 묶어서 이동 생성자 호출 유도
        
    void PrintName() const { // const 추가
        m_Name.Print();
    }

private:
    String m_Name;
};

int main() {
    Entity entity("Cherno");
    entity.PrintName();
    
    std::cin.get();
    return 0;
}
```