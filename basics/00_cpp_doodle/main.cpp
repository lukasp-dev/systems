#include <iostream> 
#include <string>
#include <cstdint>
#include <utility>   // std::move, std::swap
#include <cstddef>   // std::size_t

// 01. class 와 object 기본
class Student {
private:
    std::string name;
    int score;
public:
    Student(std::string n, int s):name(std::move(n)), score(s){}
    
    void print() const {
        std::cout << name << " " << score << '\n';
    }
};

/** 02. Encapsulation
 * 내부 데이터를 직접 건드리지 못하게 하고, 함수로만 다루게 함.
 */
class BankAccount {
private:
    int balance;
public:
    BankAccount() : balance(0){}

    void deposit(int amount){
        if (amount > 0) balance += amount;
    }

    bool withdraw(int amount){
        if(amount > balance) return false;
        balance -= amount;
        return true;
    }

    int getBalance(){
        return balance;
    }
};

/** 시스템 관점 (캐시/패딩)
 *  멤버 순서를 조정해 메모리 패딩을 줄일 수 있다.
 */

class OrderBook {
private:
    double last_price; // 8 bytes
    uint64_t sequence; // 8 bytes
    int32_t volume;    // 4 bytes
    char symbol[4];    // 4 bytes -> 24 bytes, 잘 정렬됨
public:
    void update(double p, int32_t v) {
        last_price = p;
        volume = v;
    }
    double get_price() const {
        return last_price;
    };
};

class InefficientOrderBook {
private:
    int32_t volume;    // 4 bytes
    double last_price; // 8 bytes
    uint64_t sequence; // 8 bytes
    char symbol[4];    // 4 bytes -> 32 bytes
public:
    void update(double p, int32_t v) {
        last_price = p;
        volume = v;
    }
    double get_price() const {
        return last_price;
    };
};

/** 3. Constructor
 * 객체가 만들어질 때 자동으로 호출되는 함수.
 */
class Person {
private: 
    std::string name;
public:
    // 만약에 Person(std::string n) : name(n) 했으면 n 이 lvalue 여서 "값 복사(copy)" 임.
    // move(n)이 "n은 이제 가져가도 됨" -> rvalue 처럼 보이게 캐스팅 해줌.
    Person(std::string n) : name(std::move(n)){}
};
// explicit 생성자
class Meter {
public:
    explicit Meter(int v) : value(v) {}
    int value;
};

/** 4. Destructor
 * 객체가 사라질 때 자동으로 호출되는 함수
 */
class Test {
public:
    ~Test() {
        std::cout << "destroyed\n";
    }
};
// destructor 가 중요한 이유: 메모리, 파일, 소켓, 락 같은 자원을 정리 하기 위해.
#include <cstdio>

class FileWrapper {
private:
    FILE* file;

public: 
    explicit FileWrapper(const char* path) {
        file = fopen(path, "r");
    }

    ~FileWrapper() {
        if (file) fclose(file);
    }

    // 복사 금지 (소유권 하나만)
    // 클래스이름(const 클래스이름&) = delete; // FileWrapper a = b 금지
    // 클래스이름& operator=(const 클래스이름&) = delete; // 이미 존재하는 a와 관련: a = b 금지
    FileWrapper(const FileWrapper&) = delete;
    FileWrapper& operator=(const FileWrapper&) = delete;
};

/** 5.Inheritance
 * 기존 클래스를 기반으로 새 클래스를 만드는 것
 */
class Animal{
public:
    void eat() {
        std::cout << "eat\n";
    }
};

class Dog : public Animal {
public:
    void bark(){
        std::cout << "bark\n";
    }
};

// public v. private inheritance
class Base {
public:
    void talk() {}
};

class DerivedA : public Base { // 외부에서도 Base로 취급 가능
};
class DerivedB : private Base { // 내부 구현용 - 외부에 Base 노출 안함
    void work() { talk(); }
};

/** 6.Composition
 * has-a 관계. 권장
 */
class Engine {
public:
    void start() {
        std::cout << "engine start\n";
    }
};

class Car {
private:
    Engine engine;
public:
    void drive(){
        engine.start();
        std::cout << "drive\n";
    }
};

/** 7. Polymorphism
 * 같은 인터페이스로 다른 동작을 하게 하는 것
 */
class Logger {
public:
    virtual void write(const std::string& msg){
        std::cout << "[log] " << msg << '\n';

    }
};

class FileLogger : public Logger {
public:
    void write(const std::string& msg) override {
        std::cout << "[file] " << msg << '\n';
    }
};

class ConsoleLogger : public Logger {
public:
    void write(const std::string& msg) override {
        std::cout << "[console] " << msg << '\n';
    }
};

void emit(Logger& logger, const std::string& msg){
    logger.write(msg);
}

/** 8. Virtual Function 
 * virtual = "자식에서 override 가능하고, 런타임에 실체 객체 타입 기준으로 호출하라."
 */
class Document {
public:
    virtual void render() {
        std::cout << "generic document\n";
    }
    virtual ~Document() = default; // virtual 소멸자를 해줘야함. 그래야 자식 클래스도 소멸됨.
};

class PdfDocument : public Document {
public:
    void render() override {
        std::cout << "render PDF\n";
    }
};

// final - 더 이상 override 금지
class Exporter{
public:
    virtual void export_data() {}
};

class CsvExporter : public Exporter {
    void export_data() override final {}
};

class FastCsvExporter : public CsvExporter {
    // void export_data() override {} // error - final
};

/** 9. Pure virtual & abstract class 
 * 순수 가상 함수 = 구현을 자식에게 강제하는 함수.
 */

class Shape {
public:
    virtual double area() const = 0; // pure virtual
    virtual ~Shape() = default;
};

// = 0 이 붙으면 pure virtual. 이런 함수가 하나라도 있으면 abstract class -> 직접 객체 생성 불가.
// 자식이 반드시 구현해야 한다.
// 면접 답변: An abstract class has at least one pure virtual function and cannot be
// instantiated. It defines an interface for derived classes.
class Circle : public Shape {
private:
    double r;

public:
    explicit Circle(double radius) : r(radius) {};

    double area() const override {
        return 3.1415926535 * r * r;
    }
};

/** 10. virtual destructor 
 * C++ OOP 면접에서 거의 필수.
 * 
 * 면접 답변 (거의 외워도 됨)
 * If a class is intended to be sued polymorphically, its destructor should be virtual.
 * Otherwise deleting a derived object through a base pointer can cause undefined behavior.
 */

// 잘못된 코드
class Connection {
public:
    ~Connection() {  // NOT virtual
        std::cout << "Connection destructor\n";
    }

    /* 올바른 코드
    virtual ~Connection(){
        std::cout << "Connection destructor\n";
    }
    */
};

class TcpConnection : public Connection {
    int* buffer;
public:
    TcpConnection() : buffer(new int[100]) {}
    ~TcpConnection() {
        delete[] buffer;
        std::cout << "TcpConnection destructor\n";
    }
};

/** 11. Overloading v. Overriding
 *       Overloading | Overriding
 *  시점  컴파일 타임    |   런타임 
 *  관계  같은 클래스,   |  부모 virtual 함수를
 *       다른 파라미터  |  자식이 재정의
 *  키워드    없음      |   override
 */
// Overloading
class Printer {
public:
    void print(int x) {std::cout << x << '\n'; }
    void print(std::string s) {std::cout << s << '\n';}
};

// Overriding
class Task {
public:
    virtual void execute() {std::cout << "generic task\n";}
    virtual ~Task() = default;
};

class DownloadTask : public Task {
public:
    void execute() override { std::cout << "download files\n"; }
};

/** 12. Access Specifiers 
 */
class AccessBase {
private:
    int a;       // 클래스 내부에서만

protected:
    int b;       // 클래스 내부 + 자식 클래스

public:
    int c;       // 어디서든
};


/** 13. Static member 
 * 객체마다 있는 게 아니라 클래스 전체가 공유하는 멤버.
 */
class Counter {
public:
    // Counter a, b; a.count, b.count 는 메모리 공간을 공유함
    // 그리고 이거는 선언(declaration)이어서 실제 저장 공간은 아직 없다.
    // 따라서 클래스 밖에서 정의(definition)가 필요하다.
    static int count;

    Counter() {++count;}
    ~Counter() {--count;}

    static int getCount() {return count;}
};

// class 안에 static 멤버 변수를 만들 때는 declaration 단계가 메모리에 실체는 아직 없고,
// 이것처럼 클래스 밖에서 정의할 때 실제 메모리가 생성된다.
int Counter::count = 0; // 정적 멤버 정의

// c++17 이후에는!!
// inline static 덕분에 클래스 내부에서 정의와 초기화를 동시에 가능하다.
/*
class Counter {
public:
    inline static int count = 0;
};
*/

// static 함수의 장단점
/*
# 일반 멤버 함수의 경우
obj.foo() -> 이거 사실 foo(&obj) 이런식으로 동작해서 암묵적으로 this 포인터를 전달받음.
즉 일반 멤버 함수는 암묵적으로 this 포인터를 전달받는거임. 
```cpp
class A {
public:
    int x;

    void foo() {
            std::cout << this << '\n';
    }
};
```
그래서 일반 멤버 함수는 객체 상태(x, y 등)에 접근할 수 있음.

# static 함수의 경우
특정 객체에 속하지 않는다.
```cpp
class A {
public:
    static void foo();
};
```
호출할 때도 객체가 필요 없다.
```cpp
A::foo()
```
그래서 진짜 개념적으로 컴파일러는 this 포인터를 전달할 의무가 없음 -> 안함.
```cpp
// 컴파일 에러 남
class A {
public:
    static void foo() {
        std::cout << this;
    }
};
```
### non-static 멤버도 접근 불가
```cpp
class A {
public:
    int x;
    static int count;

    static void foo() {
        x = 10; // 에러
        count++; // 이건 괜찮음
    }
};
```
### 객체를 명시적으로 받으면 사용 가능
```cpp
class A {
    int x;

    satic void foo(A& obj){
        std::cout << obj.x;
    }
};
```

### static 함수의 장점?
1.객체 생성 없이 호출 가능
2.객체 상태에 의존하지 않음 (e.g. static int add(int a, int b))
3. 전역 함수보다 캠슐화가 좋음 -> 관련 기능을 클래스 내부에 묶어둘 수 있다.

### static 함수의 단점
1. 객체 데이터 접근 불가
2. 다형성 불가
3. OOP 성격이 약함
*/

/**14. const member function & const-correctness
 * 객체 상태를 바꾸지 않ㅇ는 멤버 함수에 const 를 붙인다.
 */

class Profile{
private:
    std::string name;
public:
    Profile(const std::string& n):name(std::move(n)){}

    std::string getName() const {
        return name;
    }

    void setName(const std::string& n) {
        name = n;
    }
};

/** 15. Copy constructor & shallow v. deep copy 
 * 객체를 복사해서 새 객체를 만들 때 호출
*/
class Contact {
public:
    std::string name;
    Contact(std::string n) : name(std::move(n)){}
    Contact(const Contact& other) : name(other.name) {}
};

class Data {
public:
    int* p;

    explicit Data(int value) : p(new int(value)) {}
    ~Data() { delete p; }
    // 컴파일러가 만드는 copy ctor = shallow copy (주소만 복사)
};

class DeepData{
private:
    int* p;

public:
    explicit DeepData(int value) : p(new int(value)) {} 

    DeepData(const DeepData& other) : p(new int(*other.p)) {}

    // [방식 A] 수동 copy assignment (아래 copy-and-swap과 둘 중 하나만 활성화 가능)
    // 둘 다 두면 operator= 오버로드가 모호해져(ambiguous) 컴파일 에러.
    /*
    DeepData& operator=(const DeepData& other) {
        if(this == &other) return *this; // self-assignment 방어
        int* newp = new int(*other.p);
        delete p;
        p = newp;
        return *this;
    }
    */

    // 위 처럼 해도 무관한데 copy-and-swap 방식도 있음
    // 위 방식이나 copy-and-swap 방식은 "자원을 직접 관리헤애 할 때" 쓰는 정석적이고 견고한
    // 관용구지만, 진짜 modern C++의 방향은
    // "애초에 직접 자원 관리를 하지 말자(Rule of Zero)임." => RAII 타입에 의존 (뒤에 나올거임)
    // 어쨌든 지금은 copy-and-swap 방식

    void swap(DeepData& rhs) noexcept {
        std::swap(p, rhs.p);
    }

    DeepData& operator=(DeepData other){ // reference 로 받으면 안됨.
        // other 이 친구는 어짜피 함수 스택콜 빠지면서 Destructor 호출함
        swap(other);
        return *this;
    }

    ~DeepData() {delete p;}
};

/** 16. Copy assignment operator
 * 이미 존재하는 객체에 다른 객체를 대입할 때.
 * 
 * Data a(10);
 * Data b(10);
 * b = a;      // copy assignment
 * 
 * Data c = a; // 이거는 copy constructor!! (다른 거)
 * 
 * Data b = a; => copy constructor
 * b = a; => copy assignment
 * 면접에서 이 차이 많이 물어봄.
 * 
 * deep copy 는 상황이 두 가지인데, 각 상황마다 담당 함수가 다름.
 * 1. 내 메모리가 아직 없음 -> 그냥 새로 할당만
 * 2. 내가 이미 갖고 있던 메모리를 먼저 정리해야함.
 * 
 * 핵심 차이: "기존 메모리 정리가 필요한가"
 * DeepData a(10);
 * DeepData b(20); // b는 이미 [20] 메모리를 소유 중
 * 
 * b = a; // 대입 -> 방식 A/B 호출
 * 
 * 함수                                   |	  deep copy를 직접 하나?
 * 복사 생성자 DeepData(const DeepData&)   |  O — new int(*other.p)
 * 방식 A operator=(const DeepData&)	  |   O — new int(*other.p) (중복)
 * 방식 B operator=(DeepData other)	      |   X — 파라미터 복사 때 복사 생성자가 대신 해줌
 */

/** 17. Rule of Three / Five / Zero
 * C++ OOP 면접 핵심 중 하나
 * 
 * # Rule of Three
 * 다음 중 하나라도 직접 정의하면, 보통 셋 다 필요할 가능성이 높다:
 * destructor
 * copy constructor
 * copy assignment operator 
 * 
 * # Rule of Five (C++11~)
 * destructor
 * copy constructor 
 * copy assignmnet operator
 * move constructor
 * move assignment operator
 * 
 * # Rule of Zero (가장 권장)
 * 직접 자원 관리하지 말고
 * std::string, std::vector, std::unique_ptr 등 RAII 타입에 맡기면
 * 특수 멤버 함수를 직접 안 만들어도 된다.
 */

/** 18. Move semantics (lvalue, rvalue, std::move)
 * move = 복사하지 않고 rvalue에서 "자원을 훔쳐" 온다 (복사보다 쌈).
 *
 * std::vector<int> a = {1,2,3};
 * std::vector<int> b = std::move(a); // a의 내부 버퍼를 b로 "훔침"
 * // a는 valid but unspecified state (보통 비어 있음)
 *
 * std::move 자체는 아무것도 이동시키지 않고, 그저 rvalue로 cast해서
 * move 생성자/move 대입이 불리도록 "허락"해주는 것일 뿐이다.
 */

// [주의] 이 클래스는 move가 내부에서 무슨 일을 하는지 보여주려는 "학습용"임.
// 실무에서는 이렇게 raw 포인터를 직접 들고 move를 손으로 안 쓴다.
// 아래 [Rule of Zero 버전]처럼 RAII 타입(std::string 등)에 맡기고 =default로 끝낸다.
class StringWrapper {
    char* data;
    std::size_t len;
public:
    // move constructor: 남의 자원을 훔쳐온다
    StringWrapper(StringWrapper&& other) noexcept
        : data(other.data), len(other.len) {  // ① 포인터만 가져옴 (new 안 함)
        other.data = nullptr;                 // ② 원본을 빈 껍데기로 (안 하면 double free)
        other.len = 0;
    }

    // move assignment
    StringWrapper& operator=(StringWrapper&& other) noexcept {
        if (this == &other) return *this; // 자기 대입 방어
        delete[] data;                    // 내가 갖던 옛 자원 해제
        data = other.data;                // 훔치기
        len = other.len;
        other.data = nullptr;             // 원본 무력화
        other.len = 0;
        return *this;
    }

    // noexcept가 중요: std::vector가 reallocate 할 때 move가 noexcept면 move,
    // 아니면 copy를 선택한다. move ctor에 noexcept 붙이는 습관.
};

// [Rule of Zero 버전] 실무에서는 이렇게 쓴다 — move를 한 줄도 안 쓴다.
// class StringWrapper {
//     std::string data;   // std::string이 자기 버퍼의 move를 이미 올바르게 구현해둠.
//     // move ctor/assignment/소멸자 전부 컴파일러가 자동 생성.
// };

/** 18.Smart Pointers 
 * +-------------+----------------+-------------------+--------------------------+
 * | 스마트 포인터 | 소유권         | 복사 가능 여부     | 용도                     |
 * +-------------+----------------+-------------------+--------------------------+
 * | unique_ptr  | 단독 (Exclusive)| 불가 (이동만 가능) | 기본값, 소유자 하나      |
 * | shared_ptr  | 공유 (Shared)   | 가능 (refcount++) | 여러 곳이 같이 소유      |
 * | weak_ptr    | 없음 (Observer) | N/A               | 순환 참조 끊기, 관찰만   |
 * +-------------+----------------+-------------------+--------------------------+
 *
 * # 왜 new/delete 대신에 smart pointer?
 * RAII - 소멸자가 다종으로 해제해서 leak/double-free를 막는다. 예외가 나도 안전
 * # unique vs shared 언제?
 * 기본은 unique(단독 소유, 오버헤드 0). 진짜 여러 주체가 고유해야 할 때만 shared. shared는 refcount 관리 비용이 있음.
 * # 순환 참조 (circular reference) - 거의 무조건 물어봄
 * struct Parent {std::shared_ptr<Child> child; };
 * struct Child {std::shared_ptr<Parent> parent; }; 
 */
#include <memory>

struct Resource {
    int id;
    Resource(int i) : id(i) {
        std::cout << "acquire: " << id << '\n';
    }
    ~Resource() {
        std::cout << "release: " << id << '\n';
    }
};

// make_unique 는 클래스 외부 함수라 constructor 가 private 일 경우 이렇게 많이들 씀.
// std::make_unique는 std 라이브러리 코드 안에 있는 함수라서 Widget의 private 생성자에 접근할 수 없음.
// (위 Resource 와 이름이 겹치면 재정의 에러라 Widget 으로 분리함)
class Widget {
private:
    int value;

    explicit Widget(int x) : value(x){}
public:
    static std::unique_ptr<Widget> Create(int x ) {
        return std::unique_ptr<Widget>(new Widget(x)); // 그리고 unique_ptr 는 포인터를 받아야함.
    }

    // 뒤에 main 에서 auto it = Widget::Create(2);
};

// 순환 참조(circular reference) 데모용 — 섹션 18 Smart Pointers 소속.
// Parent가 Child를 먼저 참조하므로 전방 선언(forward declaration) 필요.
class Child;

class Parent {
public:
    std::shared_ptr<Child> c;
};

class Child {
public:
    std::shared_ptr<Parent> p;
};

/** 19. Virtual Table (vtable/vptr)
 * "Animal* 인데 어떻게 Dog::speak()가 불리지?" 에 대한 답.
 *
 * # 핵심 아이디어
 * vptr  : 객체마다 1개. 자기 클래스의 vtable을 가리키는 숨은 포인터.
 * vtable: 클래스마다 1개. 그 클래스 virtual 함수들의 주소 배열.
 *
 * # 슬롯 규칙 (제일 중요)
 * 슬롯 번호(인덱스)는 고정, 슬롯 안의 함수 주소만 실제 타입에 따라 바꿔치기.
 * override한 슬롯만 교체되고, 안 한 슬롯은 부모 함수 그대로 상속.
 *
 *   Animal vtable          Dog vtable (speak만 override)
 *   [0] Animal::speak      [0] Dog::speak       // 교체됨
 *   [1] Animal::getName    [1] Animal::getName  // 그대로 상속
 *
 * # a->speak() 호출 과정 (dynamic dispatch)
 *   1) 객체의 vptr을 따라 vtable을 찾고
 *   2) speak의 고정 슬롯[0]에서 함수 주소를 꺼내
 *   3) 그 주소로 점프  ->  실제 객체가 Dog면 Dog::speak 실행.
 *
 * # 왜 vptr은 객체당, vtable은 클래스당?
 * Dog d1,d2,d3; 함수 주소는 다 똑같으니 vtable은 1개만 만들어 공유.
 * 대신 객체마다 "내 타입이 뭔지" 가리키는 vptr만 하나씩 가진다.
 *
 * # sizeof가 커지는 이유
 * struct A { int x; };                    // 4  (int만)
 * struct B { virtual void f(); int x; };  // 16 (vptr 8 + int 4 + padding 4)
 * virtual이 몇 개든 vptr은 객체당 딱 1개만 추가된다.
 *
 * # 면접 한 줄
 * virtual 함수가 있으면 객체마다 vptr이 생기고 클래스의 vtable을 가리킨다.
 * 호출 시 vptr을 통해 실제 타입의 함수 주소를 찾아 호출하므로 런타임 다형성이 된다.
 * => 그래서 virtual destructor도 vtable로 올바른 소멸자를 찾아 부를 수 있다 (섹션 10).
 */
struct Animal2 {
    virtual void speak()   { std::cout << "...\n"; }
    virtual std::string getName() { return "animal"; }
    virtual ~Animal2() = default;
};
struct Dog2 : Animal2 {
    void speak() override { std::cout << "Woof\n"; } // 슬롯[0]만 교체, getName은 상속
};


int main(){
    // 01
    Student s("Alice", 95);
    s.print();

    // 02. Encapsulation
    std::cout << sizeof(OrderBook) << '\n';
    std::cout << sizeof(InefficientOrderBook) << '\n';

    // 03. Constructor
    // Meter m = 10; // error: no viable conversion from 'int' to 'Meter'
    Meter m(10); // OK

    // 04. Destructor
    FileWrapper a("m.txt");
    // FileWrapper b = a; // error: call to deleted constructor of 'FileWrapper'
    FileWrapper b("m.txt");
    // a = b; //error: overload resolution selected deleted operator '='

    // 05. Inheritance
    Dog d;
    d.eat(); // Animal 에서 물려받음.
    d.bark();

    DerivedA a_object;
    a_object.talk();
    DerivedB b_object;
    // b_object.talk(); // error: 'talk' is a private member of 'Base'
    
    // 06.Composition
    Car car;
    car.drive();

    // 07.Polymorphism
    ConsoleLogger console;
    FileLogger file;

    emit(console, "order placed"); // [console] order placed
    emit(file, "order placed"); // [file] order placed

    Logger* p = new FileLogger();
    p->write("shipped"); // [file] shipped
    delete p;

    // 08.virtual function
    Document* doc = new PdfDocument();
    doc->render();
    delete doc;

    // 09. Pure virtual & abstract class
    // Shape s // error - abstract class

    // 10. virtual Destructor
    Connection* c = new TcpConnection();
    delete c;  // ⚠️ Connection destructor만 호출 → TcpConnection::~TcpConnection 미호출 → leak / UB

    // 13. static member
    Counter x, y;
    std::cout << Counter::count << '\n';
    std::cout << Counter::getCount() << '\n';

    // 14. const member function & const-correctness
    const Profile person("Tom");
    // p.setName("Bob"); // error
    std::cout << person.getName();

    // 15. Copy constructor & shallow copy & deep copy
    Contact c1("Tom");
    Contact contactCopy = c1; // copy constructor (클래스 내부에 두 번째로 정의된 constructor가 불림)

    Data d1(10);
    Data d2 = d1; // d1.p와 d2.p가 같은 주소
    // 둘 다 destructor에서 delete -> double free -> UB(Undefined Behavior)

    // 18. Smart Pointers
    auto up = std::make_unique<Resource>(1); // 1 은 Resource 생성자로 전달되는 인자
    // auto copy = up; // compile error 복사 금지
    auto moved = std::move(up); //  소유권 이동, up은 이제 nullptr
    auto sp1 = std::make_shared<Resource>(2);
    std::cout << sp1.use_count(); //1
    /*
    shared_ptr는 객체와 참조 카운트를 관리하는 control block을 필요로 한다.
    shared_ptr<T>(new T(...)) 는 일반적으로 객체와 control block을 별도로 할당하여 heap allocation이 2번 발생할 수 있다.
    반면 std::make_shared<T>(...) 는 객체와 control block을 하나의 연속된 메모리 블록에 함께 할당하여 일반적으로 heap allocation이 1번만 발생하므로 더 효율적이다.
    */ 
    // shared pointer refcount 관찰
    auto sp2 = std::make_shared<Resource>(2);
    {
        auto sp3 = sp2;
        std::cout << sp2.use_count(); // 2
    } // sp2 소멸 -> refCount--;
    std::cout << sp1.use_count(); // 1

    // circular reference in case
    /**
    class Parent {
    public:
        std::shared_ptr<Child> c;
    };

    class Child {
    public:
        std::shared_ptr<Parent> p;
    };

    이제 p.reset(), c.reset() 하면? 외부 변수만 없어짐
    Parent count = 1 <--- Child 가 들고 있음
    Child count = 1 <--- Parent 가 들고 있음

    so... we need to make one of the shared_ptr weak_ptr.
    weak_ptr 는 share_ptr 가 관리하는 객체를 소유하지 않고 구경만 하는 포인터.
    std::weak_ptr<Resource> wp = p;
    여기서 wp는 객체를 가리키지만:
    - ref count 증가 안 함
    - 객체 수명 연장 안 함
    - delete 책임 없음

    그니까 만약에
    class Parent {
    public:
        std::shared_ptr<Child> c;
    };

    class Child {
    public:
        std::weak_ptr<Paarent> p;
    };

    이렇게 되면 아래처럼 코드를 써도
    Parent count 1
    Child count 2
    
    1. parent_ptr.reset() 하면 parent count => 0 ~Parent 즉시 소멸
    2. Parent가 소멸되면 내부 멤버도 소멸: std::shared_ptr<Child> c;
    3. child count now 1, scope 끝나면 destructor 불림.
     */
    auto parent_ptr = std::make_shared<Parent>();
    auto child_ptr = std::make_shared<Child>();

    parent_ptr->c = child_ptr;
    child_ptr->p = parent_ptr;
    
    return 0;
}