# Modern Value Types (with when/why/how)

These types show up in modern C++ code reviews and interviews because they make intent explicit.

---

## 1) `std::optional<T>`: "value may be absent"

Use when:
- function can legitimately return "no result"
- you want to avoid magic sentinels (`-1`, `""`, etc.)

```cpp
#include <optional>
#include <string_view>

std::optional<int> parse_port(std::string_view s) {
    if (s.empty()) return std::nullopt;
    int port = 0;
    for (char c : s) {
        if (c < '0' || c > '9') return std::nullopt;
        port = port * 10 + (c - '0');
    }
    if (port < 1 || port > 65535) return std::nullopt;
    return port;
}
```

Interview point:
- "I use `optional` so invalid input is modeled in the type system."

---

## 2) `std::variant<A, B, ...>`: "exactly one of known alternatives"

Use when:
- payload can be one of several known message types
- you want compile-time checked handling

```cpp
#include <iostream>
#include <string>
#include <variant>

struct Ping { int id; };
struct Text { std::string body; };
using Message = std::variant<Ping, Text>;

void handle(const Message& m) {
    std::visit([](const auto& msg) {
        using T = std::decay_t<decltype(msg)>;
        if constexpr (std::is_same_v<T, Ping>) {
            std::cout << "Ping id=" << msg.id << "\n";
        } else if constexpr (std::is_same_v<T, Text>) {
            std::cout << "Text body=" << msg.body << "\n";
        }
    }, m);
}
```

Interview point:
- "`variant` forces me to handle every alternative explicitly."

---

## 3) `std::any`: dynamic type box (last resort)

Use when:
- exact type list is not known at compile time
- plugin/extensibility boundary needs loose typing

```cpp
#include <any>
#include <iostream>
#include <string>

int main() {
    std::any payload = std::string("hello");

    if (const auto* s = std::any_cast<std::string>(&payload)) {
        std::cout << *s << "\n";
    }
}
```

Interview point:
- "`any` is flexible but loses static guarantees and may cost more."

---

## 4) `std::string_view`: read-only non-owning text view

Use when:
- function reads string data but does not own or mutate it
- you want fewer copies

```cpp
#include <iostream>
#include <string>
#include <string_view>

bool starts_with_http(std::string_view s) {
    return s.starts_with("http://") || s.starts_with("https://");
}

int main() {
    std::string url = "https://example.com";
    std::cout << starts_with_http(url) << "\n";
}
```

Lifetime rule:
- `string_view` is just pointer+length. The original string must outlive the view.

---

## 5) `std::span<T>`: non-owning array view

Use when:
- function needs contiguous sequence (`vector`, array, C-array) without copying
- you want safer "pointer + size"

```cpp
#include <iostream>
#include <span>
#include <vector>

int sum(std::span<const int> nums) {
    int s = 0;
    for (int x : nums) s += x;
    return s;
}

int main() {
    std::vector<int> v{1,2,3,4};
    std::cout << sum(v) << "\n"; // 10
}
```

---

## 6) Fast selection cheatsheet

1. Maybe no value -> `optional<T>`
2. One of known set -> `variant<...>`
3. Unknown runtime type -> `any` (careful)
4. Read-only string input -> `string_view`
5. Read-only/mutable buffer view -> `span<const T>` / `span<T>`
