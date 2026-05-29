# 08: `shared_ptr` and `weak_ptr`

### 1) `shared_ptr` model
- Multiple owners share one object.
- Control block tracks strong/weak counts.
- Object is destroyed when strong count becomes zero.

```cpp
auto p1 = std::make_shared<int>(42);
auto p2 = p1; // strong count increments
```

### 2) Performance and correctness notes
- Ref-count increments/decrements are atomic (thread-safe for ownership ops).
- Pointee access itself is not synchronized automatically.
- `shared_ptr` has overhead; do not use it as default pointer type.

### 3) Cycle problem

```cpp
struct B;
struct A { std::shared_ptr<B> b; };
struct B { std::shared_ptr<A> a; }; // cycle leak
```

Neither object reaches count zero.

### 4) Break cycle with `weak_ptr`

```cpp
struct B;
struct A { std::shared_ptr<B> b; };
struct B { std::weak_ptr<A> a; }; // no ownership cycle
```

### 5) Safe access from `weak_ptr`

```cpp
if (auto sp = weak.lock()) {
    // object still alive
}
```

### 6) `enable_shared_from_this`
If an object needs to produce `shared_ptr<this>`, inherit from `std::enable_shared_from_this<T>`.
Never build a new `shared_ptr(this)` manually inside an already shared object.