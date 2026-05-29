# STL + Complexity (Interview-ready, detailed)

This file is for one goal: **choose the right container fast and defend the choice clearly**.

---

## 1) Container choice framework (the actual decision flow)

Ask these in order:
1. Do I need key-value lookup?
2. Do I need sorted order?
3. Do I need random indexing?
4. Will I insert/erase a lot in the middle?
5. Do I care about memory locality/cache behavior?

If unsure, start with `std::vector` and justify why.

---

## 2) High-yield container table

| Container | Best use | Lookup | Insert/Erase | Ordering | Interview note |
|---|---|---|---|---|---|
| `std::vector<T>` | default sequence | O(1) by index, O(n) search | back amortized O(1), middle O(n) | insertion order | cache-friendly, fastest in practice for many tasks |
| `std::string` | text buffer | O(1) by index | append amortized O(1) | character order | contiguous like vector |
| `std::unordered_map<K,V>` | fast average key lookup | average O(1), worst O(n) | average O(1) | no order | watch hash quality + rehash behavior |
| `std::map<K,V>` | ordered key queries | O(log n) | O(log n) | sorted by key | use for range queries (`lower_bound`) |
| `std::set<T>` | ordered unique values | O(log n) | O(log n) | sorted | uniqueness + ordering together |

---

## 3) Algorithm complexity you should say out loud

- `std::sort`: O(n log n)
- `std::find`: O(n)
- `std::binary_search` / `std::lower_bound` (sorted range): O(log n)
- `unordered_map::find`: average O(1), worst O(n)
- `map::find`: O(log n)

---

## 3.1) Why `unordered_map` is average O(1) (actual mechanics)

`unordered_map` is a **hash table**:
1. It keeps an array of buckets.
2. For key `k`, it computes `h = hash(k)`.
3. It maps that hash to a bucket index (`h % bucket_count` conceptually).
4. It searches only inside that bucket's collision group.

If hash distribution is good, each bucket holds only a small number of elements on average.

### Core quantity: load factor

\[
\alpha = \frac{n}{m}
\]

- `n`: number of stored elements
- `m`: number of buckets
- `alpha`: average elements per bucket

Expected lookup cost is O(1 + alpha).  
When implementation keeps `alpha` bounded (via rehash), this becomes expected O(1).

### Why collisions matter

Two different keys can map to same bucket:

```text
buckets:
[0]: (kiwi,3) -> (pear,1)
[1]:
[2]: (apple,2)
[3]: (banana,1) -> (grape,4)
```

Lookup for `"banana"`:
1. hash("banana") -> bucket 3
2. scan only chain in bucket 3

If chains are short on average, lookup is constant expected time.

### Why worst-case is O(n)

If many keys collide into one bucket (bad hash or adversarial input), bucket scan becomes linear.
Then find/insert/erase can degrade to O(n).

### Rehash and amortized behavior

When load factor grows too much:
1. container allocates more buckets
2. re-inserts elements into new bucket layout

Single rehash is expensive, but spread across many inserts gives amortized expected O(1) insertion.

### Practical interview-quality wording

"`unordered_map` is expected O(1) because key lookup narrows to one bucket by hash, and implementations maintain bounded load factor via rehash. Worst-case is O(n) under heavy collisions."

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{9, 1, 7, 1, 5};
    std::sort(v.begin(), v.end()); // O(n log n)

    // sorted: 1 1 5 7 9
    bool has7 = std::binary_search(v.begin(), v.end(), 7); // O(log n)
    auto first_ge_6 = std::lower_bound(v.begin(), v.end(), 6); // O(log n)

    std::cout << std::boolalpha << has7 << "\n";
    if (first_ge_6 != v.end()) std::cout << *first_ge_6 << "\n"; // 7
}
```

---

## 4) Iterator/reference invalidation (frequent interview trap)

### `vector`
- reallocation: invalidates **all** iterators/references/pointers to elements.
- `erase(pos)`: invalidates iterators at/after `pos`.

```cpp
std::vector<int> a{1,2,3};
auto it = a.begin();
a.push_back(4);           // may reallocate
// using *it here may be UB if reallocated
```

### `map` / `set`
- insert: existing iterators stay valid.
- erase: only erased iterator invalidated.

### `unordered_map`
- rehash can invalidate iterators.
- references to elements remain valid unless erased (but iterators may move).

---

## 5) Practical tradeoff example: word frequency

```cpp
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> words{"kiwi","apple","kiwi","banana","apple","kiwi"};

    std::unordered_map<std::string, int> fast;
    for (const auto& w : words) fast[w]++;

    // If we need sorted report:
    std::map<std::string, int> sorted(fast.begin(), fast.end());
    for (const auto& [word, cnt] : sorted) {
        std::cout << word << ": " << cnt << "\n";
    }
}
```

Reasoning:
- Count phase: `unordered_map` for average O(1).
- Output phase: convert to `map` for deterministic order.

---

## 6) Interview answer template

Use this style:
1. "I’ll use `unordered_map` for average O(1) lookup."
2. "If sorted traversal is required, I’ll switch to `map` or sort extracted keys."
3. "I’ll watch iterator invalidation if container mutates during traversal."

That answer sounds both correct and senior enough for new-grad.
