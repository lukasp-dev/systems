# 04: Stack vs Heap (Interview Perspective)

### 1) Storage duration first, not "where memory is"
- Automatic storage duration: local variables, usually stack lifetime.
- Dynamic storage duration: memory allocated by allocator (`new`, `malloc`, containers, smart pointers).
- Static storage duration: globals/statics, lifetime is entire program.

### 2) Stack
- Fast allocation/deallocation (pointer move in stack frame).
- Lifetime tied to scope.
- Great for temporary objects and deterministic cleanup.
- Limited size and cannot outlive scope.

### 3) Heap
- Lifetime can outlive function scope.
- Allocation/deallocation are more expensive and may fragment memory.
- Needed for large dynamic structures and polymorphic lifetime management.

### 4) Interview-safe rule
- Do not manually `new`/`delete` by default.
- Prefer RAII owners:
  - `std::vector<T>` for dynamic arrays
  - `std::string` for text buffers
  - `std::unique_ptr<T>` for owning single objects

### 5) Typical bugs
1. Memory leak: forgot to release dynamic storage.
2. Dangling pointer/reference: object destroyed but pointer reused.
3. Double delete: same dynamic object released twice.

### 6) Example

```cpp
std::unique_ptr<int> makeValue() {
    auto p = std::make_unique<int>(42); // heap object, RAII-owned
    return p; // ownership moves out safely
}
```