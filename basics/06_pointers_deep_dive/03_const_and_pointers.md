### The Four Flavors of Const Pointers
use the "Read from Right to left" rule.

### **C++ Pointer Constness Summary**

| Syntax | Read it as... | Can change Address? | Can change Data? |
| :--- | :--- | :---: | :---: |
| `int* p` | Pointer to **int** | ✅ Yes | ✅ Yes |
| `const int* p` | Pointer to **const int** | ✅ Yes | ❌ No |
| `int* const p` | **const pointer** to **int** | ❌ No | ✅ Yes |
| `const int* const p` | **const pointer** to **const int** | ❌ No | ❌ No |

---

Note: int const* p is identical to const int* p. Both mean the data is immutable.

### **Quick Reference Code**

```cpp
int x = 10;
int y = 20;

// 1. Pointer to int
int* p1 = &x;
p1 = &y;    // OK
*p1 = 30;   // OK

// 2. Pointer to const int
const int* p2 = &x;
p2 = &y;    // OK
// *p2 = 30; // ERROR

// 3. const pointer to int
int* const p3 = &x;
// p3 = &y;  // ERROR
*p3 = 30;   // OK

// 4. const pointer to const int
const int* const p4 = &x;
// p4 = &y;  // ERROR
// *p4 = 30; // ERROR

---

### Extra interview notes

### Top-level vs low-level const
- Top-level const: constness of the object itself (e.g., `int* const p`).
- Low-level const: constness of pointee/referred value (e.g., `const int* p`).
- Template type deduction treats these differently, so this is a common interview trap.

### API design pattern
- Read-only buffer: `const T* data, std::size_t n` or `std::span<const T>`.
- Mutable buffer: `T* data, std::size_t n` or `std::span<T>`.
