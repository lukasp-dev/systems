# 02: Pointer Arithmetic (Systems & Performance)
In high-performance C++, pointer arithmetic is the foundation of **Data Locality** (the practice of organizing data in memory so that it is physically close to the processing unit that will consume it). Understanding how the CPU calculates addresses is critical for optimizing loops and minimizing cache misses.

### 01. The Scaling Factor
When you add an integer `n` to a pointer, the CPU does not increment the address by `n` bytes. It increments by `n * sizeof(T)`.

```cpp
int32_t* p = reinterpret_cast<int32_t*>(0x1000);
p = p + 1; //Address becomes 0x1004 (int32_t is 4 bytes);
double* d = reinterpret_cast<double*>(0x1000);
d = d + 1; //Address becomes 0x1008 (double is 8 bytes);
```
if asked about `void*` arithmetic, it is illegal in ISO C++ because the size of `void` is unknown. Some compilers (like GCC) treat `void*` as 1-byte as an extension, but in most interviews, stick to the standard: it requires a cast.

### 02. Hardware Efficiency: The `LEA` instruction
Modern x86_64 CPUs use the **LEA (Load Effective Address)** instruction to perform pointer arithmetic.
- Mechanism: LEA can calculate `Base + (Index * Scale) + Displacement` in a single clock cycle.
- Performance: This is why array indexing (`arr[i]`) is essentially free. The CPU computes the offset and fetches the data without needing a separate multiplication instruction.

### 03. Pointer Subtraction & `std::ptrdiff_t`
Subtracting two pointers of the same type yields the **distance in elements**, not bytes.
- Result Type: `std::ptrdiff_t` (a signed integer type).
- The Formula: `(address_A - Address_B) / sizeof(T)`.
- Constraint: Subtracting pointers that do not point to the same array results in Undefined Behavior (UB).

### 04 Array Decay & Pointer Identity
In C++, an array name decays into a pointer to its first element in most expressions. However, they are not the same:
```cpp
int arr[10];
int* ptr = arr;

// Critical Interview Distinction:
sizeof(arr); // Returns 40 (Total size of the array)
sizeof(ptr); // Returns 8  (Size of the memory address)
```

---

### UB boundaries you should memorize

1. `p + n` is valid only within the same array object (or one-past-the-end).
2. Dereferencing one-past-the-end is undefined behavior.
3. Pointer subtraction is valid only for pointers into the same array object.
4. Arithmetic on null pointer is undefined behavior.

### Modern note
For many APIs, `std::span<T>` gives safer indexing/iteration while still compiling down efficiently.
