# Combination

## 공식
- 서로 다른 n개 중 r개 조합: `nCr = n! / (r!(n-r)!)`

## 핵심 포인트
- **순서가 중요하지 않음** → 조합
- `start`로 다음 후보를 제한해서 **중복 없이** 뽑음
- 시간복잡도: 출력되는 조합 수만큼 `O(nCr * r)` (출력 포함)

## 재귀 흐름
- 현재 선택된 원소 개수가 r개가 되면 출력
- 다음 선택은 `i+1`부터 시작해서 같은 원소를 두 번 고르지 않게 함

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> combi;

void printV(){
    for(int i=0; i<combi.size(); ++i){
        cout << combi[i] << " ";
    }
    cout << "\n";
}

void makeCombi(int n, int r, int start){
    if(combi.size() == r){
        printV();
        return;
    }
    
    for(int i=start; i<n; ++i){
        combi.push_back(i);
        makeCombi(n, r, i+1);
        combi.pop_back();
    }
}

int main(){
    makeCombi(10, 3, 0);
    return 0;
}
```

## 비트마스크로 조합 만들기
```cpp
vector<int> a = {1,2,3,4,5};
int n = a.size(), r = 3;
vector<int> mask(n, 0);
fill(mask.end() - r, mask.end(), 1); // 0..0 1..1
do {
    vector<int> pick;
    for(int i=0; i<n; ++i){
        if(mask[i]) pick.push_back(a[i]);
    }
    // pick이 조합
} while (next_permutation(mask.begin(), mask.end()));
```

---

### Full source (combination.cpp)

```cpp
#include <vector>
using namespace std;

void makeCombination(int n , int r, vector<vector<int>>& ret, vector<int>& vec, int start){
    if(vec.size() == r){
        ret.push_back(vec);
        return;
    }

    for(int i = start; i < n; i++){
        vec.push_back(i);
        makeCombination(n, r, ret, vec, i+1);
        vec.pop_back();
    }
}

int main(){
    int n = 3, r=2;

    vector<vector<int>> ret;
    vector<int> vec;
    makeCombination(n, r, ret, vec, 0);

    return 0;
}
```
