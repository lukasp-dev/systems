# Modern C++ Mini Program: `string_view` + `span` + safe parsing/casting

Goal: simulate NIC-style packet handling **without UB-heavy raw casts**.

What this teaches:
1. `std::span<std::byte>` for safe buffer slicing
2. `std::string_view` for no-copy text handling
3. explicit endian parsing (network byte order)
4. `std::bit_cast` for safe bit-level representation view

---

## 1) Packet format

Assume wire format:
- bytes `[0..1]`: `type` (`uint16_t`, big-endian)
- bytes `[2..3]`: `payload_len` (`uint16_t`, big-endian)
- bytes `[4..7]`: `session_id` (`uint32_t`, big-endian)
- bytes `[8..]`: payload bytes

Why not `reinterpret_cast<Header*>` directly?
- alignment risk
- endian mismatch
- strict-aliasing/portability issues

---

## 2) Full example source

```cpp
#include <bit>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

struct Header {
    std::uint16_t type;        // big-endian
    std::uint16_t payload_len; // big-endian
    std::uint32_t session_id;  // big-endian
};

constexpr std::uint16_t read_u16_be(std::span<const std::byte> s, std::size_t pos) {
    return (static_cast<std::uint16_t>(std::to_integer<unsigned>(s[pos])) << 8) |
           static_cast<std::uint16_t>(std::to_integer<unsigned>(s[pos + 1]));
}

constexpr std::uint32_t read_u32_be(std::span<const std::byte> s, std::size_t pos) {
    return (static_cast<std::uint32_t>(std::to_integer<unsigned>(s[pos])) << 24) |
           (static_cast<std::uint32_t>(std::to_integer<unsigned>(s[pos + 1])) << 16) |
           (static_cast<std::uint32_t>(std::to_integer<unsigned>(s[pos + 2])) << 8) |
           static_cast<std::uint32_t>(std::to_integer<unsigned>(s[pos + 3]));
}

std::optional<Header> parse_header(std::span<const std::byte> frame) {
    if (frame.size() < 8) return std::nullopt;
    Header h{
        .type = read_u16_be(frame, 0),
        .payload_len = read_u16_be(frame, 2),
        .session_id = read_u32_be(frame, 4),
    };
    if (frame.size() < 8u + h.payload_len) return std::nullopt;
    return h;
}

std::string_view payload_ascii_view(std::span<const std::byte> frame, const Header& h) {
    auto payload = frame.subspan(8, h.payload_len);
    // safe as long as payload is ASCII/UTF-8 bytes and frame stays alive
    return {reinterpret_cast<const char*>(payload.data()), payload.size()};
}

enum class PacketType : std::uint16_t {
    Ping = 1,
    Data = 2,
};

PacketType packet_type_or_default(std::uint16_t raw) {
    switch (raw) {
        case 1: return PacketType::Ping;
        case 2: return PacketType::Data;
        default: return PacketType::Data;
    }
}

void push_u16_be(std::vector<std::byte>& out, std::uint16_t v) {
    out.push_back(static_cast<std::byte>((v >> 8) & 0xFF));
    out.push_back(static_cast<std::byte>(v & 0xFF));
}

void push_u32_be(std::vector<std::byte>& out, std::uint32_t v) {
    out.push_back(static_cast<std::byte>((v >> 24) & 0xFF));
    out.push_back(static_cast<std::byte>((v >> 16) & 0xFF));
    out.push_back(static_cast<std::byte>((v >> 8) & 0xFF));
    out.push_back(static_cast<std::byte>(v & 0xFF));
}

int main() {
    std::string_view command = "SEND /hello";
    if (command.starts_with("SEND ")) {
        std::cout << "command payload: " << command.substr(5) << "\n";
    }

    constexpr std::string_view msg = "hello_nic";
    std::vector<std::byte> frame;
    frame.reserve(8 + msg.size());

    push_u16_be(frame, 2); // PacketType::Data
    push_u16_be(frame, static_cast<std::uint16_t>(msg.size()));
    push_u32_be(frame, 0xA1B2C3D4);
    for (char c : msg) frame.push_back(static_cast<std::byte>(c));

    auto parsed = parse_header(frame);
    if (!parsed) {
        std::cout << "invalid frame\n";
        return 0;
    }

    PacketType pt = packet_type_or_default(parsed->type); // enum cast boundary
    std::string_view payload = payload_ascii_view(frame, *parsed); // no-copy view

    std::cout << "session_id=0x" << std::hex << parsed->session_id << std::dec << "\n";
    std::cout << "payload=" << payload << "\n";
    std::cout << "type=" << (pt == PacketType::Data ? "Data" : "Ping") << "\n";

    float temperature = 36.5f;
    std::uint32_t raw_bits = std::bit_cast<std::uint32_t>(temperature);
    std::cout << "float bits=0x" << std::hex << raw_bits << std::dec << "\n";
}
```

    ---

    ## 3) Walkthrough (line-of-thinking)

    ### `parse_header`
    - checks minimum header size first
    - parses each integer field with dedicated big-endian readers
    - validates that full payload is actually present
    - returns `std::optional<Header>` for parse success/failure

    ### `payload_ascii_view`
    - builds a `span` subview for payload
    - returns `string_view` over same bytes
    - zero-copy, but caller must ensure original frame lifetime

    ### `packet_type_or_default`
    - converts raw wire value to strong enum boundary
    - unknown values handled by default branch

    ---

    ## 4) Interview takeaways from this example

    1. "I avoid direct struct reinterpretation for wire data; I parse bytes explicitly."
    2. "I use `span`/`string_view` to avoid copies and keep interfaces clean."
    3. "I represent parse failure with `optional`, not magic values."
    4. "For bit-level inspection I use `bit_cast`, not unsafe pointer punning."

    ---

    ## 5) Try-it-yourself exercises

    1. Add checksum byte and verify it during parsing.
    2. Add packet types (`Ack`, `Error`) and switch handling.
    3. Reject non-ASCII payload cleanly.
    4. Return structured parse error enum instead of just `nullopt`.

    ---

    ## 6) Wrong vs Correct implementations

    ### Case A: Direct struct reinterpret cast (wrong in practice)

    ```cpp
    // DON'T do this on wire bytes:
    // Header* h = reinterpret_cast<Header*>(frame.data());
    ```

    Problems:
    1. packet bytes may not satisfy `Header` alignment
    2. wire is big-endian; host may be little-endian
    3. padding/layout assumptions can differ across builds/ABIs

    Correct approach:
    - parse fixed-width fields from bytes explicitly (`read_u16_be`, `read_u32_be`)

    ### Case B: Dangling `string_view`

    Wrong:
    ```cpp
    std::string_view make_view_bad() {
        std::string tmp = "abc";
        return tmp; // dangling
    }
    ```

    Correct:
    ```cpp
    std::string make_owner() {
        return "abc";
    }
    ```
    or return a view only when owner lifetime is guaranteed externally.

    ### Case C: Enum conversion without boundary check

    Wrong:
    ```cpp
    PacketType p = static_cast<PacketType>(raw_from_wire); // blindly trusts value
    ```

    Correct:
    - keep a conversion function (`packet_type_or_default`) with explicit `switch`.
