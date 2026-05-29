# 05: Function Pointers and Callable Objects

### 1) Basic function pointer syntax

```cpp
int add(int a, int b) { return a + b; }
int (*fp)(int, int) = &add;
int r = fp(2, 3); // 5
```

- `fp` stores code address of a compatible free function.

### 2) Callback-style API

```cpp
using Callback = void(*)(int);

void run(Callback cb) {
    cb(10);
}
```

This is common in C APIs and low-overhead interfaces.

### 3) Lambda interactions
- Non-capturing lambda can convert to function pointer.
- Capturing lambda cannot convert directly (it has state).

```cpp
auto no_capture = [](int x) { return x + 1; }; // convertible
auto capture = [offset = 3](int x) { return x + offset; }; // not convertible
```

### 4) `std::function` vs function pointer
- Function pointer:
  - very lightweight
  - only free/static/member-wrapper style without captures
- `std::function`:
  - type-erased general callable wrapper
  - can hold capturing lambdas/functors
  - may have extra overhead

### 5) Member function pointers (different type)

```cpp
struct A {
    int mul2(int x) const { return 2 * x; }
};

int (A::*pmf)(int) const = &A::mul2;
A a;
int y = (a.*pmf)(4); // 8
```