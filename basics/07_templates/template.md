# Template

template is such a broad topic, some might think it's just the generics in Java, but it is much more complicated than that.

```cpp
#include <iostream>
#include <string>
void Print(int value){
    std::vout << value << std::endl;
}

void Print(std::string value){
    std::vout << value << std::endl;
}

void Print(float value){
    std::vout << value << std::endl;
}

int main(){
    Print(5);
    Print("hello");
    Print(10.2f);

    return 0;
}
```

This obviously works but we could use a template to define the function once. Templates, you define it before the return type.

```cpp
#include <iostream>
#include <string>

template <typename T> // T can be called anything
// you can also write `template <class T>` it works the same internally
void Print(T value){
    std::cout << value << "\n";
}

int main(){
    Print(5);
    Print("hello");
    Print(10.2f);

    return 0;
}
```

### Two-Phase Lookup in C++

When a compiler encounters a template, it ddoesn't compile it all at once. Instead, it performs the check in two odistinct stages.

**Phase 1: Definitional Time (Synctax Check)**
The compiler parses the template when it first sees the dfinition. at this point, the template parameter `T` is unknown.

- "Non-dependent names" (names that do not depend on `T`) is checked at this time
- **Examples:** Syntax errors, missing semicolons, and calls to functions or variables that have nothing to do with `T`.

**Phase 2: Instantiation Time (Logic Check)**
The compiler checks the template again when you actually use it (e.g., `Print(10)`). Now, `T` is known (e.g, `T` is `int`).

- "Dependent names" (names that depend on `T`) is checked.
- **Example:** Does `T` support the `+` operator? Does `T` have a member function called `draw()`?
- This is **where the actual machine code is generated**. If the logic doesn't make sense for that specific `T`, the compiler fails here.

```cpp
#include <iostream>
#include <string>

template <typename T>
void ProcessData(const T& data) {
    // PHASE 1 ERROR:
    // 'unknown_var' does not depend on T.
    // The compiler knows this is wrong immediately.
    // std::cout << unknown_var;

    // PHASE 2 ERROR:
    // This depends on T. If T is 'int', this is fine.
    // If T is a 'std::string', you cannot multiply it by 2.
    // The compiler only catches this when you call ProcessData<std::string>().
    std::cout << (data * 2) << "\n";
}

int main() {
    ProcessData(10); // Works! T is int, (10 * 2) is valid.

    // ProcessData(std::string("Hello")); // ERROR in Phase 2!
    // String doesn't support multiplication.

    return 0;
}
```

### The Core Philosophy of Template: Shifting Runtime to Compile-Time

In languags like Java or Python, the system asks, "What type is this object?" while the program is running. In C++, tmeplates allow us to move that question to the **Compilation Phase.**

- Compiler's Job: When the compiler sees a template, it "instantiates" it. It locks in the type `T` and writes a dedicated piece of mahcine code specifically for that type.
- Result: The runtime overhead becomes **Zero**. The program doesn't think. It just executes preoptimized instructions.

### Why the logic after `T` is Critical

When you say the code following the template declaration is "finalized", you are referring to two massive performance wins:

1. Inlining (Eliminating Function Calls)
   Standard function calls require the CPU to "jump" to a different memory address, which can disrupt the pipeline. Becasue a template'stype is fixxed at compile-time, the compiler can perform Inlining, literally copying the function's logic directly into the caller's code. This removes the jump overhead entirely.
   템플릿을 쓰면 컴파일러가 타입 정보를 미리 알 수 있어서, 비효율적인 점프 과정을 생략하고 코드를 직접 넣는 인라인 최적화가 가능해서 성능이 좋아진다.

2. Static Memory Layout
   In HPC, the heap (`malloc`/`new`) is your enemy becasue it's slow. Templates allow for Static Allocation(컴파일 타임에 메모리를 얼마나 ㅆㄹ지 이미 계산이 끝나있다).
   ```cpp
   template <typename T, int Size>
   struct StaticArray {
       T data[Size];
   };
   ```
   This ensuresthe memory is reserved on the **Stack**, which is lightening-fast and requires zero dynamic allocation logic during execution.

### Taking this further: `if constexpr`

`if constexpr` allows the compiler to evaluate an `if` statement at compile-time.

```cpp
template <typename T>
void calculate(T value) {
    if constexpr (std::is_integral_v<T>) {
        // optimized for integers
    } else {
        // optimized for decimals
    }
}
```

The compiler looks at the condition, picks the correct branch, and completely deletes the other one from the final executable. There is no `if` statement left in the machine code.

### Variadic Pack Expansion

Variadic Pack Expansion is the process of "unpacking" a parameter pack (Args...) into a comma-separated list of elements or expressions at compile-time.

**1. Common Expansion Pattern (C++11/14)**
Standard ways to expand a pack into function arguments or type lists.

| Pattern          | Code Example             | Expanded Result (Conceptual)    |
| :--------------- | :----------------------- | :------------------------------ |
| Simple Expansion | `f(args...);`            | `f(arg1, arg2, arg3);`          |
| Nested Function  | `f(g(args)...);`         | `f(g(arg1), g(arg2), g(arg3));` |
| Type Expansion   | `std::tuple<Ts...>`      | `std::tuple<int, double, char>` |
| Brace Init List  | `int arr[] = {args...};` | `int arr[] = {1, 2, 3};`        |

**2. Fold Expressions (C++17)**
A modern , powerful way to reduce or expand a pack using binary operators without manual recursion.

| Type         | Syntax             | Description                                                |
| :----------- | :----------------- | :--------------------------------------------------------- |
| Unary Fold   | `(... + args)`     | Sums all elements: `((arg1 + arg2) + arg3)`                |
| Binary Fold  | `(0 + ... + args)` | Sums with an initial value: `(((0 + arg1) + arg2) + arg3)` |
| Logical Fold | `(... && args)`    | Returns true if all elements are true (Short-circuiting).  |

**3. Comma Fold Pattern (C++17)**

Used to execute a series of actions (like lambda calls) for each element in the pack sequentially.

```cpp
// Syntax: ( (Action), ... );
( [&] (const auto& data) {
    // Process individual element 'data'
}(params), ... );

// doesn't have to be the lambda
template<typename... Objs>
void startAll(Objs&... objs) {
    (objs.start(), ...); // 각 객체의 start() 메서드 실행
}
```

### Key Terminology: Parameter Packs

1. **Parameter Pack**: A template or function parameter that accepts zero or more arguments.
   - **Template Parameter Pack**: `typename... Ts` (A bundle of types)
   - **Function Parameter Pack**: `Ts... args` (A bundle of values)

2. **Packing**: The act of declaring a pack to receive multiple arguments.
3. **Unpacking (Expansion)**: The act of spreading the bundle into individual elements using the ellipsis `...`.

### Putting it altogether

```cpp
#include <iostream>
#include <string>
#include <type_traits>

template <typename... Ts>
void processMarketData(const Ts&... params){
    ([&](const auto& data) {
        // Use a local alias for readability and safety
        using T = std::decay_t<decltype(data)>;

        if constexpr (std::is_integral_v<T>) {
            std::cout << "[INT/VOL] Fast-path integer processing: " << data << "\n";
        }
        else if constexpr (std::is_floating_point_v<T>) {
            std::cout << "[FLOAT/PRICE] Precision FP processing: " << data << "\n";
        }
        else if constexpr (std::is_same_v<T, std::string> ||
                            std::is_same_v<T, const char*>) {
            std::cout << "[STRING/TICKER] Symbol mapping: " << data << "\n";
        }
    }(params), ...);
}

int main(){
    std::string ticker = "NVDA";
    double price = 825.50;
    int volume = 5000;

    std::cout << "--- First Execution (Ticker, Price, Volume) ---\n";
    processMarketData(ticker, price, volume);

    std::cout << "\n--- Second Execution (Price only) ---\n";
    processMarketData(99.99);

    return 0;
}
```

---

### Template vs Virtual: when to choose which

Both are polymorphism, but at different times:
- Template: compile-time polymorphism (static dispatch)
- Virtual: runtime polymorphism (dynamic dispatch)

### Quick tradeoff table

| Topic | Template | Virtual |
|---|---|---|
| Binding time | compile time | runtime |
| Dispatch cost | usually inlined, no vtable hop | vtable indirection |
| Binary size | can grow (multiple instantiations) | usually smaller per interface |
| Flexibility | types known at compile time | types chosen at runtime |
| ABI/plugin boundary | harder | natural fit |

### Example A: Template version (fast path)

```cpp
template <typename Strategy>
double apply_fee(double price, Strategy s) {
    return s(price); // often inlined
}

struct LowFee {
    double operator()(double p) const { return p * 0.999; }
};
```

### Example B: Virtual version (runtime selected)

```cpp
class FeeStrategy {
public:
    virtual ~FeeStrategy() = default;
    virtual double apply(double price) const = 0;
};

class LowFeeV final : public FeeStrategy {
public:
    double apply(double p) const override { return p * 0.999; }
};
```

Interview wording:
"If strategy set is known at compile time and hot path performance matters, I prefer templates.  
If strategy is chosen dynamically (config/plugin/runtime input), I prefer virtual interface."

---

### Add constraints with concepts (C++20)

Without constraints, template errors can be noisy.  
Concepts make required operations explicit.

```cpp
#include <concepts>

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template <Addable T>
T add(T a, T b) {
    return a + b;
}
```

This improves compile errors and API readability.
