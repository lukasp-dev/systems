# 09: Advanced Casting (Modern C++)

### 1) `static_cast`
- Compile-time checked conversions between related/static-compatible types.
- No runtime polymorphic safety check.

```cpp
double d = 3.14;
int i = static_cast<int>(d); // truncates
```

### 2) `dynamic_cast`
- Runtime-checked cast for polymorphic class hierarchies.
- Requires at least one virtual function in base.
- Pointer cast failure returns `nullptr`; reference cast throws `std::bad_cast`.

```cpp
Base* b = get();
if (Derived* d = dynamic_cast<Derived*>(b)) {
    // safe downcast
}
```

### 3) `const_cast`
- Adds/removes cv-qualifiers (`const`, `volatile`).
- Safe only if original object was non-const.
- Writing through casted pointer to truly const object is undefined behavior.

### 4) `reinterpret_cast`
- Bit-level reinterpretation of pointer/integer representations.
- Highly platform-dependent; use only for low-level boundaries.
- Easy source of undefined behavior if alignment/aliasing rules are broken.

### 5) `std::bit_cast` (C++20)
- Safer bitwise cast for trivially copyable same-size types.
- No type-punning UB from aliasing.

```cpp
std::uint32_t bits = std::bit_cast<std::uint32_t>(1.0f);
```

### 6) Interview policy
1. Prefer no cast if possible.
2. If cast needed, choose most restrictive safe cast.
3. Avoid C-style cast because it can silently do multiple cast categories.

---

### 7) UB boundary: strict aliasing and alignment

Wrong pattern (type-punning via pointer reinterpret):

```cpp
float f = 1.0f;
auto p = reinterpret_cast<std::uint32_t*>(&f); // aliasing violation risk
std::uint32_t bits = *p;                       // UB-prone
```

Why risky:
1. strict aliasing: object may only be read through allowed compatible types
2. optimizer assumes alias rules and may miscompile this pattern

Safer alternative:

```cpp
std::uint32_t bits = std::bit_cast<std::uint32_t>(f); // C++20
```

---

### 8) Alignment UB example

```cpp
alignas(1) unsigned char buf[8]{};
auto p64 = reinterpret_cast<std::uint64_t*>(buf); // may be misaligned
// *p64 = 123; // potential UB on platforms requiring alignment
```

Safe alternative:
- read/write bytes explicitly, or
- use `std::memcpy` / `std::bit_cast` on properly aligned objects.

---

### 9) `dynamic_cast` failure behavior (pointer vs reference)

```cpp
Base* bp = get_base();
if (auto* dp = dynamic_cast<Derived*>(bp)) {
    // pointer cast success
}

try {
    Derived& dr = dynamic_cast<Derived&>(*bp); // throws on failure
} catch (const std::bad_cast&) {
    // handle mismatch
}
```

Use pointer form when failure is normal control flow.