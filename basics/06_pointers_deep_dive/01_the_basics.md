# 01:The Basics of Pointers
In C++, a pointer is a primitive type that hold a **Virtual Memory address**. While high-level languages treat "references" as magic handles, HFT engineers treat pointers as **offsets** that interact directly with the CPU's memory controller.

### 1. The Anatomy of an Address
On a 65-bit architecture (x86_64 or ARM64), `sizeof(void*)` is 8 bytes. However, the hardware uses a trick.
- The 48-bit Limit: Most modern CPUs only use the lower 48 bits for actual addressing ($2^{48} \approx 256$).
- Sign Extension: Bits 48-63 must match bit 47. This is called **Canonical Address**. If you try to dereference on a non-canonical address, the CPU triggers a `segmentation fault` at the hardware level.
    - 결론적으로, 현재의 64 비트 컴퓨터는 64비트라는 이름표만 달고 있고, 실제로는 가상 메모리의 중간 부분을 잘라낸 채 양 끝만 사용하고 있다. 모든 부분을 다 본다기보다는, 약속된 범위 안에서만 메모리를 사용하도록 하드웨어적으로 강제하고 있는 셈.
- MMU: Your pointer doesn't point to a physical RAM pin. It points to a **Virtual Address Space (VAS)**. The Memory Management Unit (MMU) translates this to physical RAM using Page Tables.

### 2. Pointer Declaration and `nullptr`
Modern C++ (C++11 and later) strictly enforces `nullptr`.
```cpp
int32_t value = 42;
int32_t* ptr = &value; // pointer to 4-byte integer
```
Why the `nullptr` is mandatory?
- **Type Safety:** `NULL` is a macro for `0`(an integer). `nullptr` is a keyword of type `std::nullptr_t`.
- **Overload Resolution:** 
```cpp
void find(int x); // (1)
void find(int* p); // (2)

find(NULL); // Ambiguous or calls (1)
find(nullptr); //Correctly calls (2)
/*
nullptr isn't forced or hacked into an in*. Rather, it is a special object natively designed to be compatible with any pointer type, which is why correctly resolves to the second function.
*/
```

### 3. Alignment and Padding
A pointer should always point to an address that is a multiple of the data type's size.
- A uint64_t* (8 bytes) should point to an address ending in 0, 8, or G in hex.

### 4. Memory Segments
Knowing "where" a pointer points is the first step in debugging high-performance systems.

1. Stack: Local variables. L1-cache friendly. Lifetime ends at `}`.

2. Heap: new / malloc. Slower to allocate. Causes fragmentation.

3. Data/BSS: Global and static variables.

4. Text: Compiled machine instructions (Read-Only).

---

### Quick correction + interview precision
- On modern desktop/server systems we call it 64-bit architecture (not 65-bit).
- Typical user-space virtual addresses are a canonical subset of 64 bits (platform-dependent exact width).
- Core interview point: pointer width is commonly 8 bytes on 64-bit targets, but always trust `sizeof(void*)` for portability.

### Practical debugging tip
When a crash says "invalid read/write", ask:
1. Is pointer null?
2. Is pointer dangling?
3. Is alignment valid for target type?
4. Is the pointee lifetime still active?