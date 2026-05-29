# One-page Quick Sheet: High-impact C++ Pitfalls

Use this as final review before interviews.

---

## 1) Dangling pointer/reference (lifetime bug)

```cpp
int* bad() {
    int x = 10;
    return &x; // dangling after function returns
}
```

Fix patterns:
- return by value
- return owning smart pointer
- ensure referenced object outlives reference

---

## 2) Missing virtual destructor in polymorphic base

```cpp
struct Base { virtual void f() {} }; // bad
struct Derived : Base { ~Derived() {/*release*/} };
// delete Base* that points to Derived -> UB
```

Correct:

```cpp
struct Base {
    virtual ~Base() = default;
    virtual void f() {}
};
```

---

## 3) Accidental expensive copies

```cpp
void log(std::string s);            // copy
void logFast(std::string_view s);   // no copy for read-only
```

Rule:
- read-only text input -> `string_view`
- large read-only object -> `const T&`

---

## 4) Iterator invalidation during mutation

```cpp
std::vector<int> v{1,2,3};
auto it = v.begin();
v.push_back(4); // may reallocate, it becomes invalid
```

Rule:
- mutate container carefully while iterating
- know invalidation policy per container

---

## 5) `shared_ptr` ownership cycle leak

```cpp
struct B;
struct A { std::shared_ptr<B> b; };
struct B { std::shared_ptr<A> a; }; // leak cycle
```

Fix:
- one direction should be `std::weak_ptr`

---

## 6) Data race (undefined behavior)

If one thread writes and another reads/writes same object without synchronization, behavior is undefined.

```cpp
int counter = 0; // shared
// thread 1: counter++;
// thread 2: counter++;
```

Fix:
- `std::mutex` + `std::lock_guard`
- or `std::atomic<int>` for simple counters

---

## 7) Cast misuse

Cast policy:
1. `static_cast` for normal explicit conversions
2. `dynamic_cast` for checked downcasts in polymorphic hierarchy
3. `const_cast` only for cv-qualification changes (careful)
4. `reinterpret_cast` only for low-level boundaries
5. `std::bit_cast` for same-size trivially-copyable bit reinterpretation

Avoid C-style cast: too permissive, easy to hide dangerous conversions.

---

## 8) `string_view`/`span` lifetime mistake

`string_view` and `span` do not own memory.

```cpp
std::string_view badView() {
    std::string s = "temp";
    return s; // dangling view
}
```

Rule:
- owner must outlive the view.

---

## 9) Rule-of-0/3/5 inconsistency

If class manages resource manually, incomplete copy/move/destructor set causes leaks or double free.

Rule:
- prefer Rule of 0 with RAII members
- if manual ownership exists, implement full set deliberately
