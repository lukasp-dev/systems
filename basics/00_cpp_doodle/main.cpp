// 01. class 와 object 기본
#include <iostream> 
#include <string>

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
class Base {
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
    static int count;

    
}


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

    
    
    return 0;
}