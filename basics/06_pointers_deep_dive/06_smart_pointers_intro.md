# 06: Smart Pointers Intro

### Why smart pointers exist
Raw pointers do not encode ownership. Smart pointers encode lifetime ownership in types.

### 1) `std::unique_ptr<T>`
- Single owner.
- Non-copyable, movable.
- Fast default ownership tool.

### 2) `std::shared_ptr<T>`
- Shared ownership with reference counting.
- Object is destroyed when strong count reaches zero.
- Use only when multi-owner lifetime is truly needed.

### 3) `std::weak_ptr<T>`
- Non-owning observer of `shared_ptr` managed object.
- Does not extend object lifetime.
- Used to break cyclic ownership graphs.

### 4) Preferred creation helpers

```cpp
auto u = std::make_unique<int>(10);
auto s = std::make_shared<int>(20);
```

They are exception-safe and cleaner than raw `new`.

### 5) Ownership semantics summary
1. Need exactly one owner -> `unique_ptr`
2. Need many owners -> `shared_ptr`
3. Need observer from shared graph -> `weak_ptr`