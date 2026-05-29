# Pitfalls of Pointers

Some commonly come up errors when dealing with pointers


### 1. `nullptr` dereference
A pointer is often initialized to `nullptr` to signify it isn't holding a valid memory address yet. If your code attempts to read or write to that pointer before assigning it a valid address, the operating system will immediately crash your program (typically with a Segmentation Fault).

```cpp
int* ptr = nullptr;
*ptr = 42; // CRASH! Trying to write 42 to address 0x0
```

Always check if a pointer is valid before dereferencing it, or use modern language features like references that cannot be null.

```cpp
if (ptr != nullptr) {
    *ptr = 42;
}
```

### 2. memory leaks
A memory leak happens when you allocate memory on the heap but lose the pointer to that memory without freeing it first.

When you use operators like `new` or functions like `malloc()`, you are telling the OS to reserve a chunk memory. If you overwrite the pointer holding that address, or if the pointer goes out of scope, that memory remains "reserved" but completely unreachable by your program. If this happens repeatedly, your program will eventually consume all available memory and crash.

```cpp
void leakMemory() {
    int* ptr = new int(10); // Allocated on the heap
    // Missing: delete ptr;
} // ptr goes out of scope here. The memory it pointed to is now leaked.
```
Ensure every allocation has a matching deallocation (delete or free). In modern C++, the best practice is to avoid raw pointers for ownership entirely and use Smart Pointers (`std::unique_ptr`, `std::shared_ptr`), which automatically free memory when they go out of scope.

### 3. dangling pointer
A dangling pointer is a pointer that points to a memory address that has already been deallocated or freed.

```cpp
#include <iostream>

int* getDanglingPointer() {
    int local_var = 100;

    return &local_var;
}

int main(){
    int* ptr = getDanglingPointer();

    std::cout << *ptr << std::endl;

    return 0;
}
```

1. When `getDanglingPointer()` executes, a space for local_var is reserved on the Stack memory.

2. The return statement hands over that memory address to `ptr` in the `main` function.

3. Immediately after handing over the address, the function terminates, and the stack frame containing `local_var` is completely wiped/reclaimed by the OS.

4. Consequently, `ptr` in the `main` function is left holding a ghost address, a room that no longer exists.

**use smart pointers!**
```cpp
#include <memory>

std::unique_ptr<int> getSafePointer() {
    // Allocated on the Heap, so it does not get destroyed when the function ends
    return std::make_unique<int>(100); 
} // Ownership is safely transferred to the main function
```

### 4. double free
This is often a direct consequence of a dangling pointer. If you free a pointer, don't set it to `nullptr`, and then call `delete` or `free()` on it again later, the memory management system gets corrupted. Security vulnerabilities (like heap exploitation) often exploit double free errors. 

```cpp
int* ptr = new int(100);
delete ptr;

// .. tons of code in between ...

delete ptr; // CRASH.
```

**use smart pointers!**
```cpp
#include <memory>

void modernApproach() {
    // Allocate memory safely using std::make_unique
    std::unique_ptr<int> ptr = std::make_unique<int>(100);
    
    // Use it like a normal pointer
    *ptr = 200; 
    
} // <-- ptr goes out of scope HERE. Memory is automatically freed once.
  // No need to call delete, no risk of a double free!
```