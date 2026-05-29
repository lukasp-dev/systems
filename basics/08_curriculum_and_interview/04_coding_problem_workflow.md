# Coding Problem Workflow (How to perform in the room)

This is not just "how to solve."  
This is "how to communicate while solving."

---

## 1) 6-step interview workflow

1. **Clarify** constraints and expected behavior.
2. **State brute-force** quickly (shows complete reasoning).
3. **Propose optimized plan** and justify data structure choice.
4. **Declare complexity** before coding.
5. **Implement in small safe steps**.
6. **Dry-run edge cases** and mention failure modes.

---

## 2) Clarification checklist (say this out loud)

Before coding, ask:
1. Can there be duplicate values?
2. Is one valid answer guaranteed?
3. What should return if not found?
4. Input size range (affects complexity target)?
5. Are negative values possible?

This makes you look structured and reduces rework.

---

## 3) Example: Two Sum (communication + code)

### Brute force
- check every pair `(i, j)` -> O(n^2), O(1) extra space.

### Optimized
- hash map `value -> index`
- for each `nums[i]`, check if `target - nums[i]` already seen
- O(n) time, O(n) space

```cpp
#include <unordered_map>
#include <utility>
#include <vector>

std::pair<int, int> twoSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> index_of;
    index_of.reserve(nums.size());

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int need = target - nums[i];
        auto it = index_of.find(need);
        if (it != index_of.end()) {
            return {it->second, i};
        }
        index_of[nums[i]] = i;
    }
    return {-1, -1};
}
```

Complexity:
- Time: O(n) average
- Space: O(n)

---

## 4) Dry-run template

Use this input:
- `nums = [2, 7, 11, 15], target = 9`

Trace:
1. i=0, need=7, map empty -> store 2:0
2. i=1, need=2, found at 0 -> return (0,1)

Then test edge cases:
- empty vector
- no answer
- duplicate values (`[3,3]`, target 6)

---

## 5) Common failure points in interviews

1. Coding too early without constraints.
2. Complexity not stated until asked.
3. Off-by-one or duplicate handling bug.
4. Using wrong container (`map` when `unordered_map` intended).
5. No dry-run after coding.

---

## 6) One-minute self-check before "done"

1. Correct on normal case?
2. Correct on edge cases?
3. Complexity stated?
4. Any undefined behavior risk?
5. Names/readability good?
