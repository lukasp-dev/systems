## 🍔 Greedy (탐욕법) 핵심 정리

### 0. 한 줄 정의

**"매 순간 지금 당장 가장 좋아 보이는 선택(local optimum)을 하면, 최종적으로 전체 최적해(global optimum)에 도달한다"** 는 전략.

- 뒤를 돌아보지 않고, 앞도 깊게 내다보지 않음. **현재 단계의 최선**만 고른다.
- 완전탐색(`08`)처럼 모든 경우를 보지 않으므로 **빠르다** (보통 정렬 + 한 번 스캔 = `O(N log N)`).
- 단, **항상 정답이 되는 건 아니다.** 정당성 증명이 핵심.

---

### 1. 언제 쓸 수 있나? (성립 조건)

Greedy가 정답을 보장하려면 아래 두 성질을 만족해야 한다.

- **Greedy Choice Property (탐욕적 선택 속성)**
  지금의 탐욕적 선택이 이후 선택에 의해 뒤집힐 필요가 없다. 즉 **현재 최선의 선택을 포함하는 최적해가 반드시 존재**한다.

- **Optimal Substructure (최적 부분 구조)**
  큰 문제의 최적해가 부분 문제의 최적해로 구성된다. (이건 DP와도 공유하는 성질)

> 💡 **DP vs Greedy**
> 부분 구조는 둘 다 가지지만, DP는 "여러 후보를 모두 계산 후 최선"을, Greedy는 "후보를 고민 없이 하나로 확정"한다. 그래서 Greedy가 더 빠르지만 적용 범위는 좁다.

---

### 2. 대표적인 반례 (함정)

탐욕이 **틀리는** 고전 예시 — 동전 거스름돈.

- 동전 단위: `{1, 4, 5}`, 목표 금액: `8`
- **Greedy:** 큰 것부터 → `5 + 1 + 1 + 1` = **4개**
- **정답(DP):** `4 + 4` = **2개**

> 동전이 `{1, 5, 10, 50, ...}`처럼 배수 구조면 greedy가 맞지만, 위처럼 어긋나면 틀린다. **"큰 것부터 = 항상 정답"이 아니다.**

---

### 3. 풀이 패턴 (실전 루틴)

1. **정렬 기준 정하기** — 무엇을 기준으로 "가장 좋아 보이는지" 정의한다. (끝나는 시간, 단위 무게당 가치, 마감 기한 등)
2. **한 번 스캔하며 확정** — 조건에 맞으면 채택, 아니면 버린다.
3. **정당성 증명** — "Exchange Argument": *탐욕적 선택 대신 다른 선택을 한 최적해가 있다고 가정해도, 그것을 탐욕적 선택으로 바꿔치기해도 손해가 없다*를 보인다.

---

### 4. 고전 예제

#### (a) Activity Selection — 회의실 배정

겹치지 않게 최대한 많은 회의를 잡기. **핵심: "끝나는 시간"이 빠른 순으로 정렬.**

```cpp
#include <algorithm>
#include <vector>
using namespace std;

// {start, end}
int maxActivities(vector<pair<int,int>>& v) {
    // 끝나는 시간(second) 기준 오름차순 정렬
    sort(v.begin(), v.end(), [](auto& a, auto& b){
        return a.second < b.second;
    });

    int count = 0;
    int lastEnd = -1;
    for (auto& [start, end] : v) {
        if (start >= lastEnd) {   // 직전 회의가 끝난 뒤에 시작 가능하면
            count++;
            lastEnd = end;        // 채택하고 마지막 종료시간 갱신
        }
    }
    return count;
}
```

> **왜 "시작 시간"이 아니라 "끝나는 시간"인가?**
> 가장 빨리 끝나는 회의를 고르면 **뒤에 남는 시간이 최대**가 되어, 더 많은 회의를 넣을 여지가 생긴다. (Exchange argument로 증명 가능)

---

#### (b) Fractional Knapsack — 쪼갤 수 있는 배낭

물건을 잘라 담을 수 있을 때. **핵심: "무게당 가치(value/weight)"가 높은 순으로 담는다.**

```cpp
double fractionalKnapsack(int capacity, vector<pair<int,int>>& items) {
    // items: {weight, value}, 단위 가치 기준 내림차순 정렬
    sort(items.begin(), items.end(), [](auto& a, auto& b){
        return (double)a.second / a.first > (double)b.second / b.first;
    });

    double total = 0.0;
    for (auto& [w, val] : items) {
        if (capacity >= w) {          // 통째로 담기
            capacity -= w;
            total += val;
        } else {                      // 남은 만큼만 잘라서 담기
            total += val * ((double)capacity / w);
            break;
        }
    }
    return total;
}
```

> ⚠️ **주의:** 자를 수 없는 **0/1 Knapsack**은 greedy로 풀리지 않는다 → DP 필요.

---

### 5. 요약

| 항목 | 내용 |
|------|------|
| 핵심 | 매 순간 local optimum 선택 |
| 시간복잡도 | 보통 `O(N log N)` (정렬 지배적) |
| 장점 | 빠르고 구현이 단순 |
| 단점 | 적용 조건이 까다롭고, 정당성 증명이 필수 |
| 안 될 때 | Greedy choice가 깨지면 → **DP**로 |

> 🔑 **결론:** Greedy는 "빠른 만큼 의심하라". 코드를 짜기 전에 **반례부터 떠올려 보고**, 안 떠오르면 그때 정당성을 증명하자.
