# C++ OOP — 면접 직전 완주 가이드

> 영상 없이 VS Code에서 **따라 치면서** 끝까지 읽을 수 있도록 정리한 문서.
> 레포의 `01_core_grammar_and_oop/`, `interview_questions/CppNuts/`, `oop/` 내용을 통합했습니다.

---

## 시작 전 최종 정리 — 지금 이것만 읽고 들어가기

### 이 문서가 하는 일

**C++로 class를 만들고, OOP 코드를 치는 것** — 면접에서 손으로 구현하는 부분의 기반.

핵심은 class 문법이 아니라 네 가지:

```text
lifetime        — 언제 생성·소멸되는가 (RAII, destructor)
ownership       — 누가 자원을 소유하는가 (unique_ptr, shared_ptr)
virtual dispatch — 런타임에 어떤 함수가 호출되는가 (virtual, vtable)
copy / move     — 값이 어떻게 전달되는가 (Rule of 0/3/5)
```

### 이 문서로 “코딩 가능”한 것 (algo/DS 제외)


| 할 수 있어야 함                                           | 문서 섹션         |
| --------------------------------------------------- | ------------- |
| `Order`, `Plane`, `Scheduler` 같은 **class 설계·멤버 구현** | §1–§8, §29    |
| `virtual` / abstract / `override` / **다형성**         | §9–§12, §25   |
| **RAII**, stack vs heap, smart pointer              | §5, §21–§22   |
| copy/move, Rule of 0/3/5                            | §17–§20       |
| interface + `unique_ptr` factory                    | §29           |
| `fct(&str)` 같은 **포인터 문제**                           | §6 (+ 기본 포인터) |


### 이 문서만으로는 안 되는 것 (다음에 볼 것)

```text
알고리즘 / 자료구조 로직     → algorithms/, greedy, heap 등 (별도)
STL 복잡도·컨테이너 선택    → basics/08_curriculum_and_interview/02_stl_and_complexity.md
TCP/UDP, 네트워크 이론       → system-and-networks/networking/
process / thread / OS       → OS/OSTEP/, system-and-networks/thread/
malloc 직접 구현            → 시스템 메모리 (별도)
티켓팅 / 트레이딩 설계       → 설계 답변 연습 (코딩 아님)
itoa / endian               → OOP 필수 아님, 포인터 연습 (짧게)
```

### 오늘 공부 루트 (한 세션 몰아치기)

```text
1. §0–§8   읽으며 01_class_basics.cpp, 02_raii.cpp 타이핑
2. §9–§16  03_polymorphism.cpp — Logger / FileLogger / ConsoleLogger + virtual destructor
3. §17–§22 04_rule_of_five.cpp — shallow copy 문제 → unique_ptr로 Rule of Zero
4. §29     05_factory.cpp — Notifier 예제 통째로
5. oop/main.cpp 컴파일해 보기
6. §31 체크리스트 소리 내어 말하기
```

### 끝나고 10분 자가 테스트 (algo 없이)

아래 **2개**가 되면 OOP 코딩 면접 기반 OK:

```cpp
// 1) Plane / Order 스켈레톤 (로직 단순해도 됨 — class 설계가 목적)
class Plane {
    int departure_time_, capacity_, remaining_;
public:
    explicit Plane(int dep, int cap) : departure_time_(dep), capacity_(cap), remaining_(cap) {}
    bool departs_after(int t) const { return departure_time_ > t; }
    int remaining() const { return remaining_; }
    bool allocate(int size) {
        if (size > remaining_) return false;
        remaining_ -= size;
        return true;
    }
};

// 2) virtual + unique_ptr
struct Base { virtual ~Base() = default; virtual void f() const = 0; };
struct Derived : Base { void f() const override {} };
std::unique_ptr<Base> p = std::make_unique<Derived>();
```

`itoa` / endian은 **OOP와 별개** — 포인터 연습은 OOP 끝난 뒤 30분이면 충분.

### 면접 한 문장 (외워두기)

> C++ OOP is about tying resource management to object lifetime (RAII), expressing ownership with smart pointers, achieving runtime polymorphism with virtual functions, and handling value semantics through copy and move — with a virtual destructor whenever you delete through a base pointer.

### Top 3 (막히면 여기로 돌아오기)

```text
1. Base*로 delete → virtual ~Base
2. 소유권 → unique_ptr(단독), shared_ptr(공유), weak_ptr(순환 참조 끊기)
3. 값 복사로 Base에 Derived 넣지 말 것 (slicing) — 포인터/참조로 다형성
```

**이제 §0부터 따라 치면서 들어가면 된다.**

---

## 이 문서를 쓰는 법

1. VS Code에서 이 파일을 왼쪽에, 새 `.cpp` 파일을 오른쪽에 둡니다.
2. 각 섹션의 코드를 **직접 타이핑**하고 컴파일해 봅니다.

```bash
# macOS / Linux (예시)
g++ -std=c++17 -Wall -Wextra -o out main.cpp && ./out
```

1. `면접 답변` 박스는 소리 내어 말해 봅니다.
2. `함정` / `Pitfall` 표시는 면접에서 반드시 나올 수 있는 지점입니다.

### 추천 실습 폴더

```text
basics/cpp_interview_practice/
  01_class_basics.cpp
  02_raii.cpp
  03_polymorphism.cpp
  04_rule_of_five.cpp
  05_factory.cpp
```

### 관련 레포 파일 (더 깊게 볼 때)


| 주제                           | 경로                                                              |
| ---------------------------- | --------------------------------------------------------------- |
| OOP interview core + factory | `basics/01_core_grammar_and_oop/04.class.md`                    |
| lvalue / rvalue / move       | `basics/02_value_categories/lvalue_and_rvalue.md`               |
| static 멤버                    | `basics/01_core_grammar_and_oop/06.static_members.md`           |
| RAII                         | `basics/05_raii_and_pointer_pitfalls/RAII.md`                   |
| smart pointers               | `basics/06_pointers_deep_dive/07_unique_ptr.md` 등               |
| 면접 pitfalls                  | `basics/08_curriculum_and_interview/05_quick_sheet_pitfalls.md` |
| 상속 실습 코드                     | `oop/main.cpp`, `oop/AbstractEmployee.hpp`                      |
| OOP 4대 특성 + CRTP             | `interview_questions/CppNuts/Question2.md`                      |
| Diamond problem              | `interview_questions/CppNuts/Question24.md`                     |
| const correctness            | `interview_questions/CppNuts/Question30.md`                     |


---

## Table of Contents

**핵심 한 줄:** C++ OOP 면접은 class 문법이 아니라 `**lifetime`, `ownership`, `virtual dispatch`, `copy/move`** 를 이해하냐가 관건이다.

1. [면접 핵심 4축](#0-면접-핵심-4축)
2. [OOP 큰 그림 — class와 object](#1-oop-큰-그림--class와-object)
3. [class vs struct](#2-class-vs-struct)
4. [Encapsulation (캡슐화)](#3-encapsulation-캡슐화)
5. [Constructor (생성자)](#4-constructor-생성자)
6. [Destructor (소멸자)와 RAII](#5-destructor-소멸자와-raii)
7. [this pointer](#6-this-pointer)
8. [Inheritance (상속)](#7-inheritance-상속)
9. [Composition (포함 관계)](#8-composition-포함-관계)
10. [Polymorphism (다형성)](#9-polymorphism-다형성)
11. [virtual function](#10-virtual-function)
12. [Pure virtual & abstract class](#11-pure-virtual--abstract-class)
13. [Virtual destructor](#12-virtual-destructor)
14. [Overloading vs Overriding](#13-overloading-vs-overriding)
15. [Access specifiers](#14-access-specifiers)
16. [Static member](#15-static-member)
17. [Const member function & const-correctness](#16-const-member-function--const-correctness)
18. [Copy constructor & shallow vs deep copy](#17-copy-constructor--shallow-vs-deep-copy)
19. [Copy assignment operator](#18-copy-assignment-operator)
20. [Rule of Three / Five / Zero](#19-rule-of-three--five--zero)
21. [Move semantics (lvalue, rvalue, std::move)](#20-move-semantics-lvalue-rvalue-stdmove)
22. [Smart pointers (ownership)](#21-smart-pointers-ownership)
23. [Stack object vs heap object](#22-stack-object-vs-heap-object)
24. [vtable / vptr & static vs dynamic dispatch](#23-vtable--vptr--static-vs-dynamic-dispatch)
25. [Object slicing](#24-object-slicing)
26. [Interface-style abstract class](#25-interface-style-abstract-class)
27. [explicit, final, override, =default, =delete](#26-explicit-final-override-default-delete)
28. [Diamond problem & virtual inheritance](#27-diamond-problem--virtual-inheritance)
29. [CRTP & compile-time polymorphism](#28-crtp--compile-time-polymorphism)
30. [End-to-end design: interface + factory + unique_ptr](#29-end-to-end-design-interface--factory--unique_ptr)
31. [High-impact pitfalls (면접 직전)](#30-high-impact-pitfalls-면접-직전)
32. [최종 치트시트 & Top 3](#31-최종-치트시트--top-3)

---

## 0. 면접 핵심 4축

```text
lifetime   — 객체가 언제 생성되고 언제 파괴되는가 (stack scope, RAII)
ownership  — 누가 메모리/자원을 소유하는가 (unique_ptr, shared_ptr)
virtual dispatch — 런타임에 어떤 함수가 호출되는가 (vtable)
copy/move  — 복사·이동 시 자원이 어떻게 전달되는가 (Rule of 0/3/5)
```

> **면접 답변**
> C++ OOP is not just about classes. It is about deterministic object lifetime, explicit ownership, runtime polymorphism through virtual functions, and value semantics including copy and move.

---

## 1. OOP 큰 그림 — class와 object

OOP는 **데이터**와 **함수**를 하나의 **객체**로 묶어 다루는 방식이다.

```cpp
#include <iostream>
#include <string>

class Student {
private:
    std::string name;
    int score;

public:
    Student(std::string n, int s) : name(std::move(n)), score(s) {}

    void print() const {
        std::cout << name << " " << score << '\n';
    }
};

int main() {
    Student s("Alice", 95);
    s.print();
}
```

```text
class           = 설계도 (타입)
object          = 메모리에 실제로 만들어진 인스턴스
member variable = 객체가 가진 데이터
member function = 객체가 할 수 있는 동작
```

> **면접 답변**
> A class defines data and behavior together. An object is an instance of a class stored in memory.

---

## 2. class vs struct

C++에서 `class`와 `struct`는 **거의 같은 언어 기능**이다. 실질적 차이는 **기본 접근 제어자**뿐.

```cpp
class A {
    int x; // private by default
};

struct B {
    int x; // public by default
};
```


|         | `class`   | `struct` |
| ------- | --------- | -------- |
| 기본 접근   | `private` | `public` |
| 상속 시 기본 | `private` | `public` |


> **면접 답변**
> In C++, `class` and `struct` are almost the same except default access: class is private, struct is public. Use whichever communicates intent better.

---

## 3. Encapsulation (캡슐화)

내부 데이터를 **직접 건드리지 못하게** 하고, **함수로만** 다루게 하는 것.

```cpp
class BankAccount {
private:
    int balance;

public:
    BankAccount() : balance(0) {}

    void deposit(int amount) {
        if (amount > 0) balance += amount;
    }

    bool withdraw(int amount) {
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }

    int getBalance() const {
        return balance;
    }
};
```

**왜 하는가?**

```text
잘못된 값 방지        — balance가 음수가 되지 않게
내부 구현 숨김        — 나중에 내부 표현 바꿔도 API 유지
invariant 유지        — 항상 지켜져야 하는 조건 보호
```

**invariant** 예: 계좌 잔액은 항상 0 이상.

시스템 관점 (캐시/패딩): 멤버 순서를 조정해 메모리 패딩을 줄일 수 있다.

```cpp
class OrderBook {
private:
    double last_price;   // 8 bytes
    uint64_t sequence;   // 8 bytes
    int32_t volume;      // 4 bytes
    char symbol[4];      // 4 bytes  → 24 bytes, 잘 정렬됨
public:
    void update(double p, int32_t v) { last_price = p; volume = v; }
    double get_price() const { return last_price; }
};
```

> **면접 답변**
> Encapsulation hides internal state and exposes controlled operations, so the object can protect its invariants.

---

## 4. Constructor (생성자)

객체가 **만들어질 때** 자동으로 호출되는 함수.

```cpp
class Person {
private:
    std::string name;

public:
    Person(std::string n) : name(std::move(n)) {}  // ✅ 초기화 리스트
};
```

### 초기화 리스트 vs 대입

```cpp
// ✅ 좋음: 멤버를 처음부터 초기화
Person(std::string n) : name(std::move(n)) {}

// ⚠️ 나쁨: 기본 생성 후 대입에 가까움
Person(std::string n) {
    name = n;
}
```

`string`처럼 복잡한 타입은 초기화 리스트가 **불필요한 기본 생성 + 대입**을 피한다.

### explicit 생성자 (미리 알아두기 — §26에서 상세)

```cpp
class Meter {
    explicit Meter(int v) : value(v) {}
    int value;
};

// Meter m = 10;  // error — 암시적 변환 방지
Meter m(10);      // OK
```

> **면접 답변**
> Constructor initializes an object. In C++, member initializer lists are preferred because they directly initialize members.

---

## 5. Destructor (소멸자)와 RAII

객체가 **사라질 때** 자동으로 호출되는 함수.

```cpp
class Test {
public:
    ~Test() {
        std::cout << "destroyed\n";
    }
};
```

C++에서 destructor가 중요한 이유: **메모리, 파일, 소켓, 락** 같은 자원을 정리하기 위해.

```cpp
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
    FileWrapper(const FileWrapper&) = delete;
    FileWrapper& operator=(const FileWrapper&) = delete;
};
```

### RAII

```text
Resource Acquisition Is Initialization

객체 생성  → 자원 획득
객체 소멸  → 자원 해제 (자동)
```

스코프를 벗어나면 destructor가 **반드시** 호출된다 (stack 객체).

> **면접 답변**
> RAII means tying resource management to object lifetime. The constructor acquires the resource and the destructor releases it automatically.

**함정:** destructor를 **수동으로** 호출(`obj.~T()`)한 뒤 객체가 스코프 끝에서 **또** destructor가 호출되면 double free / double destroy. (`06.static_members.md` 참고)

---

## 6. this pointer

멤버 함수 안에는 숨겨진 포인터 `**this`** 가 있다. 현재 객체를 가리킨다.

```cpp
class Person {
private:
    std::string name;

public:
    Person(std::string name) {
        this->name = std::move(name);  // 왼쪽: 멤버, 오른쪽: 매개변수
    }
};
```

컴파일러 관점:

```cpp
Person p("Tom");
p.print();
// 내부적으로: Person::print(&p);
```

- **일반 멤버 함수:** `TYPE* const this` — 주소 변경 불가, 값 변경 가능
- **const 멤버 함수:** `const TYPE* const this` — 값 변경 불가 (§16)

`static` 멤버 함수에는 `**this`가 없다**.

> **면접 답변**
> `this` is a pointer to the current object inside a non-static member function.

---

## 7. Inheritance (상속)

기존 클래스를 기반으로 새 클래스를 만드는 것. **is-a** 관계.

```cpp
#include <iostream>

class Animal {
public:
    void eat() {
        std::cout << "eat\n";
    }
};

class Dog : public Animal {
public:
    void bark() {
        std::cout << "bark\n";
    }
};

int main() {
    Dog d;
    d.eat();   // Animal에서 물려받음
    d.bark();
}
```

> **면접 답변**
> Inheritance models an "is-a" relationship. A Dog is an Animal.

**주의:**

```text
Dog is an Animal     → OK
Car is an Engine     → 이상함
Car has an Engine    → composition이 더 적절 (§8)
```

코드 재사용만을 위해 상속을 남발하지 말 것.

### public vs private inheritance

```cpp
class Base {
public:
    void talk() {}
};

class DerivedA : public Base {};   // 외부에서도 Base로 취급 가능

class DerivedB : private Base {    // 내부 구현용 — 외부에 Base 노출 안 함
    void work() { talk(); }        // 클래스 내부에서는 사용 가능
};

// DerivedB b;
// b.talk();  // error — private inheritance
```

---

## 8. Composition (포함 관계)

**has-a** 관계. 상속보다 **더 자주 권장**되는 방식.

```cpp
class Engine {
public:
    void start() {
        std::cout << "engine start\n";
    }
};

class Car {
private:
    Engine engine;  // Car has an Engine

public:
    void drive() {
        engine.start();
        std::cout << "drive\n";
    }
};
```

> **면접 답변**
> Prefer composition over inheritance when the relationship is "has-a" rather than "is-a".

---

## 9. Polymorphism (다형성)

**같은 인터페이스**로 **다른 동작**을 하게 하는 것.

```cpp
#include <iostream>
#include <string>

class Logger {
public:
    virtual void write(const std::string& msg) {
        std::cout << "[log] " << msg << '\n';
    }
    virtual ~Logger() = default;
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

void emit(Logger& logger, const std::string& msg) {
    logger.write(msg);  // 실제 객체 타입에 따라 다른 출력
}

int main() {
    ConsoleLogger console;
    FileLogger file;

    emit(console, "order placed");  // [console] order placed
    emit(file, "order placed");     // [file] order placed

    Logger* p = new FileLogger();
    p->write("shipped");            // [file] shipped
    delete p;
}
```

`virtual`이 없으면 **부모 타입 포인터**로 호출할 때 **부모 함수**가 불릴 수 있다.

> **면접 답변**
> Runtime polymorphism in C++ is achieved using virtual functions. The function call is resolved at runtime based on the actual object type.

---

## 10. virtual function

`virtual` = "자식에서 override 가능하고, **런타임에 실제 객체 타입** 기준으로 호출하라."

```cpp
#include <iostream>

class Document {
public:
    virtual void render() {
        std::cout << "generic document\n";
    }
    virtual ~Document() = default;
};

class PdfDocument : public Document {
public:
    void render() override {
        std::cout << "render PDF\n";
    }
};

int main() {
    Document* doc = new PdfDocument();
    doc->render();  // render PDF
    delete doc;
}
```

### override — 반드시 붙이기

```cpp
void render() override  // 시그니처 안 맞으면 컴파일 에러
```

`override` 없이 오타 나면 **새 함수를 만든 것**이 되어 버그가 조용히 생긴다.

### final — 더 이상 override 금지

```cpp
class Exporter {
public:
    virtual void export_data() {}
};

class CsvExporter : public Exporter {
    void export_data() override final {}
};

class FastCsvExporter : public CsvExporter {
    // void export_data() override {}  // error — final
};
```

---

## 11. Pure virtual & abstract class

**순수 가상 함수** = 구현을 자식에게 **강제**하는 함수.

```cpp
class Shape {
public:
    virtual double area() const = 0;  // pure virtual
    virtual ~Shape() = default;
};
```

`= 0`이 붙으면 pure virtual. 이런 함수가 **하나라도** 있으면 **abstract class** → **직접 객체 생성 불가**.

```cpp
// Shape s;  // error — abstract class
```

자식이 **반드시** 구현해야 한다.

```cpp
class Circle : public Shape {
private:
    double r;

public:
    explicit Circle(double radius) : r(radius) {}

    double area() const override {
        return 3.1415926535 * r * r;
    }
};
```

> **면접 답변**
> An abstract class has at least one pure virtual function and cannot be instantiated. It defines an interface for derived classes.

---

## 12. Virtual destructor

**C++ OOP 면접에서 거의 필수.**

### 잘못된 코드

```cpp
#include <iostream>

class Connection {
public:
    ~Connection() {  // NOT virtual
        std::cout << "Connection destructor\n";
    }
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

int main() {
    Connection* c = new TcpConnection();
    delete c;  // ⚠️ Connection destructor만 호출 → TcpConnection::~TcpConnection 미호출 → leak / UB
}
```

### 올바른 코드

```cpp
class Connection {
public:
    virtual ~Connection() {
        std::cout << "Connection destructor\n";
    }
};
```

**규칙:** 다형적으로 쓸 클래스(virtual 함수가 있거나 `Connection*`로 `TcpConnection`을 delete할 가능성)는 destructor를 `**virtual`** 로.

> **면접 답변** (거의 외워도 됨)
> If a class is intended to be used polymorphically, its destructor should be virtual. Otherwise deleting a derived object through a base pointer can cause undefined behavior.

---

## 13. Overloading vs Overriding


|     | Overloading         | Overriding                   |
| --- | ------------------- | ---------------------------- |
| 시점  | **컴파일 타임**          | **런타임**                      |
| 관계  | 같은 클래스, **다른 파라미터** | 부모 `virtual` 함수를 자식이 **재정의** |
| 키워드 | 없음                  | `override`                   |


### Overloading

```cpp
class Printer {
public:
    void print(int x) { std::cout << x << '\n'; }
    void print(std::string s) { std::cout << s << '\n'; }
};
```

### Overriding

```cpp
class Task {
public:
    virtual void execute() { std::cout << "generic task\n"; }
    virtual ~Task() = default;
};

class DownloadTask : public Task {
public:
    void execute() override { std::cout << "download files\n"; }
};
```

```text
Overloading  = compile-time polymorphism
Overriding   = runtime polymorphism
```

**함정:** 부모에 `virtual void f(int)`가 있는데 자식에 `void f(double)`를 만들면 **override가 아니라 overload** — `override`를 붙이면 컴파일러가 잡아준다.

---

## 14. Access specifiers

```cpp
class Base {
private:
    int a;       // 클래스 내부에서만

protected:
    int b;       // 클래스 내부 + 자식 클래스

public:
    int c;       // 어디서든
};
```


| 지정자         | 접근 범위           |
| ----------- | --------------- |
| `public`    | 어디서든            |
| `private`   | 자기 클래스만         |
| `protected` | 자기 클래스 + 파생 클래스 |


자식은 `b`, `c`에 접근 가능하지만 `a`에는 **직접** 접근 불가.

> **면접 답변**
> `private` hides members from everyone except the class itself. `protected` allows derived classes to access them. `public` exposes them to users of the class.

실무: 데이터는 `**private`**, 행동은 `**public` API**로. `protected`는 상속 설계가 명확할 때만.

---

## 15. Static member

**객체마다** 있는 게 아니라 **클래스 전체가 공유**하는 멤버.

```cpp
#include <iostream>

class Counter {
public:
    static int count;

    Counter() { ++count; }
    ~Counter() { --count; }

    static int getCount() { return count; }
};

int Counter::count = 0;  // 정적 멤버 정의 (클래스 밖)

int main() {
    Counter a, b;
    std::cout << Counter::count << '\n';      // 2
    std::cout << Counter::getCount() << '\n'; // 2
}
```

C++17 이후:

```cpp
class Counter {
public:
    inline static int count = 0;  // 클래스 안에서 바로 정의 가능
};
```

> **면접 답변**
> Static members belong to the class, not to individual objects.

`static` 멤버 함수:

- `**this` 없음**
- 일반(non-static) 멤버 변수에 **직접 접근 불가**
- `static` 멤버끼리는 접근 가능

---

## 16. Const member function & const-correctness

객체 상태를 **바꾸지 않는** 멤버 함수에 `const`를 붙인다.

```cpp
class Profile {
private:
    std::string name;

public:
    std::string getName() const {
        return name;
    }

    void setName(const std::string& n) {
        name = n;
    }
};
```

`const` 멤버 함수 안에서는 **non-mutable** 멤버 변수를 수정할 수 없다.

### const 객체

```cpp
const Profile p("Tom");
// p.setName("Bob");  // error
std::cout << p.getName();  // OK — const 멤버 함수만 호출 가능
```

### this의 타입


| 함수 종류    | this의 타입 (개념)                  |
| -------- | ------------------------------ |
| 일반 멤버    | `T* const` — 객체 내용 수정 가능       |
| const 멤버 | `const T* const` — 객체 내용 수정 불가 |


> **면접 답변**
> A const member function promises not to modify the observable state of the object.

---

## 17. Copy constructor & shallow vs deep copy

객체를 **복사해서 새 객체를 만들 때** 호출.

```cpp
class Contact {
public:
    std::string name;

    Contact(std::string n) : name(std::move(n)) {}

    Contact(const Contact& other) : name(other.name) {}
};

int main() {
    Contact a("Tom");
    Contact b = a;  // copy constructor
}
```

### 포인터 멤버 — shallow copy 문제

```cpp
class Data {
public:
    int* p;

    explicit Data(int value) : p(new int(value)) {}

    ~Data() { delete p; }
    // 컴파일러가 만드는 copy ctor = shallow copy (주소만 복사)
};
```

```cpp
Data a(10);
Data b = a;  // a.p와 b.p가 같은 주소
// 둘 다 destructor에서 delete → double free → UB
```

### Deep copy

```cpp
class Data {
private:
    int* p;

public:
    explicit Data(int value) : p(new int(value)) {}

    Data(const Data& other) : p(new int(*other.p)) {}  // 새 메모리에 값 복사

    Data& operator=(const Data& other) {
        if (this == &other) return *this;
        int* newp = new int(*other.p);
        delete p;
        p = newp;
        return *this;
    }

    ~Data() { delete p; }
};
```

> **면접 답변**
> The default copy constructor performs shallow copy. If the class owns dynamic memory, we may need a custom copy constructor for deep copy.

---

## 18. Copy assignment operator

**이미 존재하는** 객체에 다른 객체를 **대입**할 때.

```cpp
Data a(10);
Data b(20);
b = a;       // copy assignment

Data c = a;  // copy constructor (다른 것!)
```


| 표현            | 호출되는 것               |
| ------------- | -------------------- |
| `Data b = a;` | **copy constructor** |
| `b = a;`      | **copy assignment**  |


면접에서 이 차이 **자주** 물어본다.

### copy-and-swap idiom (면접 플러스)

```cpp
void swap(Data& rhs) noexcept {
    std::swap(p, rhs.p);
}

Data& operator=(Data other) {  // pass by value → copy or move
    swap(other);
    return *this;              // other destructor가 이전 자원 정리
}
```

---

## 19. Rule of Three / Five / Zero

C++ OOP 면접 **핵심 중 하나**.

### Rule of Three

다음 중 **하나라도** 직접 정의하면, 보통 **셋 다** 필요할 가능성이 높다:

```text
destructor
copy constructor
copy assignment operator
```

### Rule of Five (C++11~)

```text
destructor
copy constructor
copy assignment operator
move constructor
move assignment operator
```

### Rule of Zero (가장 권장)

```text
직접 자원 관리하지 말고
std::string, std::vector, std::unique_ptr 등 RAII 타입에 맡기면
특수 멤버 함수를 직접 안 만들어도 된다.
```

### 레포 예제 (Rule of Five + copy-and-swap)

```cpp
#include <algorithm>
#include <cstddef>
#include <memory>

class Buffer {
public:
    explicit Buffer(std::size_t n) : size_(n), data_(std::make_unique<int[]>(n)) {}
    ~Buffer() = default;

    Buffer(const Buffer& other) : Buffer(other.size_) {
        std::copy(other.data_.get(), other.data_.get() + size_, data_.get());
    }

    Buffer& operator=(const Buffer& other) {
        if (this == &other) return *this;
        Buffer temp(other);
        swap(temp);
        return *this;
    }

    Buffer(Buffer&&) noexcept = default;
    Buffer& operator=(Buffer&&) noexcept = default;

    void swap(Buffer& rhs) noexcept {
        std::swap(size_, rhs.size_);
        data_.swap(rhs.data_);
    }

private:
    std::size_t size_{0};
    std::unique_ptr<int[]> data_{};
};
```

> **면접 답변**
> Prefer Rule of Zero. Use RAII types like `std::vector`, `std::string`, and smart pointers instead of manually managing resources.

---

## 20. Move semantics (lvalue, rvalue, std::move)

### lvalue vs rvalue


|     | lvalue               | rvalue               |
| --- | -------------------- | -------------------- |
| 의미  | 이름 있고, 주소 잡을 수 있는 객체 | 임시 값                 |
| 수명  | 보통 여러 문장에 걸침         | 보통 한 표현식 안에서만        |
| 예   | `int x = 10;` 의 `x`  | `10`, `a + b`, 임시 객체 |


```cpp
void takes_lvalue(int& x) {}
void takes_rvalue(int&& x) {}

int main() {
    int a = 10;
    takes_lvalue(a);   // OK
    // takes_lvalue(10);  // error

    takes_rvalue(10);  // OK
    // takes_rvalue(a);   // error
}
```

`const int&`는 rvalue도 받을 수 있다 (읽기 전용 임시 연장).

### std::move — 소유권 이전

```cpp
#include <utility>
#include <vector>
#include <string>

std::vector<int> a = {1, 2, 3, 4, 5};
std::vector<int> b = std::move(a);  // a의 내부 버퍼를 b로 "훔침"
// a는 valid but unspecified state (보통 비어 있음)
```

**move constructor / move assignment**는 rvalue에서 **자원을 훔쳐 온다** (복사보다 싸다).

```cpp
class StringWrapper {
    char* data;
    std::size_t len;
public:
    // move constructor
    StringWrapper(StringWrapper&& other) noexcept
        : data(other.data), len(other.len) {
        other.data = nullptr;
        other.len = 0;
    }

    // move assignment
    StringWrapper& operator=(StringWrapper&& other) noexcept {
        if (this == &other) return *this;
        delete[] data;
        data = other.data;
        len = other.len;
        other.data = nullptr;
        other.len = 0;
        return *this;
    }
};
```

### noexcept on move

`std::vector` 등이 **reallocate** 할 때 move가 `noexcept`이면 move, 아니면 copy를 선택한다. move ctor에 `noexcept` 붙이는 습관.

> **면접 답변**
> Move semantics transfers ownership of resources instead of copying them. `std::move` casts to rvalue to enable move construction or move assignment. This is critical for performance with containers and Rule of Five.

---

## 21. Smart pointers (ownership)

### unique_ptr — 단독 소유

```cpp
#include <memory>

std::unique_ptr<int> p = std::make_unique<int>(10);
// 복사 불가, 이동 가능
std::unique_ptr<int> q = std::move(p);
```

### shared_ptr — 공유 소유 (참조 카운트)

```cpp
std::shared_ptr<int> p1 = std::make_shared<int>(10);
std::shared_ptr<int> p2 = p1;  // refcount = 2
// 마지막 shared_ptr 소멸 시 delete
```

### weak_ptr — 소유하지 않고 관찰

```cpp
std::weak_ptr<int> w = p1;
// w.lock() → shared_ptr (아직 살아 있으면)
```

**순환 참조 함정:**

```cpp
struct Child;
struct Parent { std::shared_ptr<Child> child; };
struct Child  { std::shared_ptr<Parent> parent; };  // leak — 서로 refcount 1씩 남음
```

한쪽을 `std::weak_ptr`로 바꿔 끊는다.

> **면접 답변**
> `unique_ptr` represents exclusive ownership. `shared_ptr` represents shared ownership using reference counting. `weak_ptr` observes a shared object without owning it.

**함정:** 다형적 타입을 `shared_ptr<Base>`로 쓸 때도 **virtual destructor** 필요.

---

## 22. Stack object vs heap object

```cpp
Student s("Tom", 90);   // stack — 스코프 끝나면 자동 소멸

auto p = std::make_unique<Student>("Tom", 90);  // heap — unique_ptr이 소유
// delete 불필요
```


|     | Stack              | Heap (`new` / smart ptr) |
| --- | ------------------ | ------------------------ |
| 할당  | 자동                 | 수동 / smart ptr           |
| 해제  | 스코프 끝 → destructor | 소유자가 해제                  |
| 속도  | 보통 더 빠름            | 상대적으로 느림                 |
| 크기  | 스택 한도 제한           | 힙이 더 큼                   |


> **면접 답변**
> Stack objects are automatically destroyed when they go out of scope. Heap objects need explicit ownership management, preferably with smart pointers.

**요즘 C++:** raw `new`/`delete`보다 `**make_unique` / `make_shared`** 선호.

---

## 23. vtable / vptr & static vs dynamic dispatch

### Runtime (virtual)

```cpp
Document* doc = new PdfDocument();
doc->render();
```

1. `doc`가 가리키는 객체의 **vptr** 읽기
2. **vtable**에서 `render` 슬롯 찾기
3. `PdfDocument::render` 호출

```text
vtable = virtual function table (클래스당 하나)
vptr   = 객체 안에서 vtable을 가리키는 포인터
```

**비용:**

- vtable 메모리
- indirect call → **인라이닝 어려움**
- 분기 예측 실패 가능

### Compile-time (non-virtual, template)

컴파일러가 호출 지점을 **고정**. 인라이닝 가능. **런타임 유연성 없음**.


|      | static dispatch | dynamic dispatch  |
| ---- | --------------- | ----------------- |
| 바인딩  | 컴파일 타임          | 런타임               |
| 메커니즘 | 직접 호출           | vtable            |
| 유연성  | 낮음              | 높음 (플러그인, 런타임 교체) |
| 성능   | 보통 더 빠름         | 약간의 오버헤드          |


> **면접 답변**
> Virtual functions are typically implemented using a vtable. Each polymorphic object has a vptr pointing to the table of virtual functions, enabling runtime dispatch. The C++ standard does not mandate vtables, but most implementations use them.

---

## 24. Object slicing

**값으로** derived를 base에 넣으면 derived 부분이 **잘려 나간다**.

```cpp
class Widget {
public:
    int width;
    virtual ~Widget() = default;
};

class Button : public Widget {
public:
    int label_id;
};

int main() {
    Button btn;
    btn.label_id = 42;
    Widget w = btn;  // slicing — label_id 잘림
}
```

다형성을 쓰려면 **포인터 또는 참조**:

```cpp
Widget& ref = btn;
Widget* ptr = &btn;
```

> **면접 답변**
> Object slicing happens when a derived object is copied into a base object by value, losing the derived part. Use references or pointers for polymorphism.

---

## 25. Interface-style abstract class

Java의 `interface` 키워드는 없지만 **abstract class**로 비슷하게 만든다.

```cpp
class Drawable {
public:
    virtual void draw() const = 0;
    virtual ~Drawable() = default;  // interface여도 virtual destructor!
};
```

`oop/AbstractEmployee.hpp` 패턴:

```cpp
class AbstractEmployee {
public:
    virtual void askForPromotion() = 0;
};
```

`oop/main.cpp`에서 `Employee`, `Developer`, `Teacher`가 구현.

---

## 26. explicit, final, override, =default, =delete


| 키워드         | 용도                       |
| ----------- | ------------------------ |
| `explicit`  | 암시적 변환 생성자 방지            |
| `override`  | virtual 함수 재정의 (시그니처 검사) |
| `final`     | 더 이상 override / 상속 금지    |
| `= default` | 컴파일러 생성 특수 멤버 사용         |
| `= delete`  | 함수 사용 금지 (복사 금지 등)       |


```cpp
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
```

---

## 27. Diamond problem & virtual inheritance

다중 상속에서 같은 base가 **두 번** 들어가는 문제.

```text
      Stream
      /    \
InputStream OutputStream
      \    /
     IOStream
```

```cpp
class Stream       { public: int _a; };
class InputStream  : public Stream { public: int _b; };
class OutputStream : public Stream { public: int _c; };
class IOStream     : public InputStream, public OutputStream { public: int _d; };

// IOStream obj;
// obj._a;  // error — Stream이 InputStream 안에 하나, OutputStream 안에 하나
```

### 해결: virtual inheritance

```cpp
class InputStream  : virtual public Stream {};
class OutputStream : virtual public Stream {};
class IOStream     : public InputStream, public OutputStream {};

// IOStream obj;
// obj._a;  // OK — Stream이 하나만
```

> **면접 답변**
> The diamond problem occurs when a class inherits the same base through multiple paths, leading to ambiguity. Virtual inheritance ensures only one shared base subobject exists.

---

## 28. CRTP & compile-time polymorphism

virtual 오버헤드를 피하면서 **계층처럼** 쓰는 패턴.

```cpp
template <typename Derived>
class BaseProcessor {
public:
    void execute() {
        static_cast<Derived*>(this)->logic();
    }
};

class OrderProcessor : public BaseProcessor<OrderProcessor> {
public:
    void logic() {
        // 주문 처리 로직
    }
};

int main() {
    OrderProcessor op;
    op.execute();  // virtual 없이 OrderProcessor::logic 호출
}
```

**언제 virtual vs template?**


| 상황                    | 선택              |
| --------------------- | --------------- |
| 런타임에 구현 교체 (플러그인, 설정) | `virtual`       |
| 타입이 컴파일 타임에 고정, 성능 중요 | template / CRTP |


---

## 29. End-to-end design: interface + factory + unique_ptr

면접에서 **설계 능력** 보여주는 패턴 (`04.class.md` §11).

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Notifier {
public:
    virtual ~Notifier() = default;
    virtual void send(std::string_view msg) = 0;
};

class EmailNotifier final : public Notifier {
public:
    void send(std::string_view msg) override {
        std::cout << "[EMAIL] " << msg << '\n';
    }
};

class SmsNotifier final : public Notifier {
public:
    void send(std::string_view msg) override {
        std::cout << "[SMS] " << msg << '\n';
    }
};

enum class Channel { Email, Sms };

std::unique_ptr<Notifier> make_notifier(Channel c) {
    switch (c) {
        case Channel::Email: return std::make_unique<EmailNotifier>();
        case Channel::Sms:   return std::make_unique<SmsNotifier>();
    }
    return std::make_unique<EmailNotifier>();
}

int main() {
    std::vector<std::unique_ptr<Notifier>> pipeline;
    pipeline.push_back(make_notifier(Channel::Email));
    pipeline.push_back(make_notifier(Channel::Sms));

    for (auto& n : pipeline) {
        n->send("system healthy");
    }
}
```

**이 설계가 강한 이유:**

```text
1. 인터페이스 경계가 명확 (Notifier)
2. 다형적 삭제 안전 (virtual ~Notifier)
3. 소유권 명확 (unique_ptr)
4. 호출 코드는 구체 클래스가 아닌 추상화에 의존
```

---

## 30. High-impact pitfalls (면접 직전)

### 1) Dangling pointer / reference

```cpp
int* bad() {
    int x = 10;
    return &x;  // 함수 끝나면 x 소멸 → dangling
}
```

**Fix:** 값 반환, owning smart pointer, 참조 수명 보장.

### 2) Virtual destructor 누락

§12 참고.

### 3) Accidental expensive copy

```cpp
void log(std::string s);              // 매번 복사
void log_fast(std::string_view s);    // 읽기 전용 — 복사 없음
```

### 4) Iterator invalidation

```cpp
std::vector<int> v{1, 2, 3};
auto it = v.begin();
v.push_back(4);  // reallocate 가능 → it 무효
```

### 5) shared_ptr 순환 참조

§21 참고 — `weak_ptr`로 끊기.

### 6) string_view / span 수명

```cpp
std::string_view bad() {
    std::string s = "temp";
    return s;  // dangling view
}
```

view/span은 **소유하지 않음** — owner가 view보다 오래 살아야 함.

### 7) Rule of 0/3/5 불완전

자원을 직접 관리하면서 destructor만 만들고 copy/move를 빼먹으면 leak / double free.

---

## 31. 최종 치트시트 & Top 3

### 면접 30분 전 체크리스트

```text
class / object / member
private / public / protected
constructor / destructor / initializer list
RAII
inheritance (is-a) vs composition (has-a)
virtual / override / final
pure virtual / abstract class
virtual destructor
overloading vs overriding
copy ctor vs copy assignment
shallow copy vs deep copy
Rule of Three / Five / Zero
move ctor / move assignment / std::move
unique_ptr / shared_ptr / weak_ptr
stack vs heap
object slicing
vtable / vptr
diamond problem / virtual inheritance
CRTP (compile-time polymorphism)
explicit / =delete / =default
```

### 한 문장 최종 정리

> C++supports encapsulation, inheritance, abstraction, and polymorphism through classes, access specifiers, virtual functions, and abstract classes. Unlike Java, C++ also has deterministic object lifetime, destructors, RAII, value semantics, copy/move constructors, and manual or smart-pointer-based ownership management.

### 가장 중요한 Top 3

```text
1. virtual function → runtime polymorphism; Base*로 delete하면 virtual ~Base
2. ownership → unique_ptr(단독), shared_ptr(공유), weak_ptr(순환 참조 방지)
3. lifetime + copy/move → Rule of Zero, slicing 주의, move로 복사 비용 절감
```

### 추천 학습 순서 (이 문서 기준)

```text
Day 1: §1–§8  (class, encapsulation, ctor/dtor, RAII, inheritance, composition)
Day 2: §9–§16 (polymorphism, virtual, abstract, static, const)
Day 3: §17–§22 (copy/move, Rule of 0/3/5, smart ptr, stack/heap)
Day 4: §23–§29 (vtable, slicing, diamond, CRTP, factory)
Day 5: §30–§31 + 레포 pitfalls + oop/main.cpp 직접 컴파일
```

---

> **마지막으로**
> 이 문서는 **algo/DS 빼고 C++ OOP 코딩**을 커버한다. plane/leaderboard의 **class 뼈대**, virtual, RAII, smart ptr, factory까지 여기서 끝낸다. 로직·STL·네트워크·OS는 다음 문서로.
> 공부 끝나면 §31 체크리스트 + `oop/main.cpp` 컴파일로 마무리.

