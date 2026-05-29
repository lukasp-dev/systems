# 07: `std::unique_ptr` Deep Dive

### 1) Core properties
- Exclusive ownership.
- Copy disabled, move enabled.
- Destructor automatically calls deleter.

```cpp
std::unique_ptr<int> a = std::make_unique<int>(5);
std::unique_ptr<int> b = std::move(a); // ownership transfer
```

After move, `a` becomes empty (`nullptr` state).

### 2) Pass/return patterns
- Return by value is efficient (move or NRVO).
- Take by value when function should consume ownership.
- Take `T*` or `T&` for non-owning use.

```cpp
std::unique_ptr<Foo> makeFoo() {
    return std::make_unique<Foo>();
}

void consume(std::unique_ptr<Foo> f); // ownership sink
void observe(const Foo& f);           // no ownership
```

### 3) Custom deleter

```cpp
using FilePtr = std::unique_ptr<FILE, int(*)(FILE*)>;
FilePtr fp(std::fopen("data.txt", "r"), &std::fclose);
```

Useful for non-memory resources (FILE handles, sockets, OS handles).

### 4) Arrays
- Prefer `std::vector<T>` usually.
- If needed, `std::unique_ptr<T[]>` supports dynamic arrays.

```cpp
auto arr = std::make_unique<int[]>(100);
arr[0] = 1;
```

### 5) Interview gotchas
1. Do not copy `unique_ptr`.
2. Do not create two `unique_ptr` from same raw pointer.
3. Use `std::move` when transferring ownership.