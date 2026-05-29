# Permutation

C++ 에서 permutation 관한 함수가 두 개가 있음. 
`next_permutation(시작, 끝+1)`: 오름차순 정렬
`prev_permutation(시작, 끝+1)`: 내림차순 정렬

## 공식
- 서로 다른 n개 전체 순열: `n!`
- 서로 다른 n개 중 r개 순열: `nPr = n! / (n-r)!`
- 중복 원소가 있을 때: `n! / (k1! * k2! * ... )`
  - 같은 값끼리 바꿔도 동일한 배열이므로, 그만큼 **중복 카운트된 경우를 나누는** 의미
  - 예: `1,1,2,2,3` → `5! / (2! * 2!) = 30`

## 핵심 포인트
- 두 함수 모두 **현재 배열을 직접 변경**함.
- **반환값**: 다음(혹은 이전) 순열이 있으면 `true`, 더 이상 없으면 `false`.
- 전체 순열을 만들려면 **정렬된 상태에서 시작**해야 함.
- 시간복잡도: 한 번 호출당 `O(n)`.

## 전체 순열 생성 패턴
```cpp
vector<int> a = {1, 2, 3};
sort(a.begin(), a.end()); // 꼭 정렬하고 시작

do {
    // a는 현재 순열
} while (next_permutation(a.begin(), a.end()));
```

## 중복 원소가 있을 때
정렬 후 `next_permutation`으로 돌리면 **중복 없는 순열만** 사전식 순서로 나옴.

## prev_permutation 사용 예시
```cpp
vector<int> a = {3, 2, 1};
sort(a.begin(), a.end(), greater<int>()); // 내림차순으로 시작

do {
    // a는 현재 순열
} while (prev_permutation(a.begin(), a.end()));
```

## 재귀함수를 이용한 순열 구하기

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> perm;
vector<int> visited;

void printV(const vector<int>& perm){
    for(int i=0; i<perm.size(); ++i){
        cout << perm[i] << " ";
    }
    cout << "\n";
}

void makePermutation(int n, int r, int depth){
    if(depth == r){
        printV(perm);
        return;
    }

    for(int i=0; i<n; ++i){
        if(visited[i]) continue;
        visited[i] = 1;
        perm.push_back(i); // 0..n-1 순열
        makePermutation(n, r, depth+1);
        perm.pop_back();
        visited[i] = 0;
    }
}

int main(){
    int n = 3, r = 2;
    visited.assign(n, 0);
    makePermutation(n, r, 0);
    return 0;
}
```

---

### Full source (permutation.cpp)

```cpp
#include <iostream>
#include <vector>

using namespace std;

void makePermutation(int n, int r, vector<vector<int>>& ret, vector<int>& vec, vector<bool>& visited){
    if(vec.size() == r){
        ret.push_back(vec);
    }

    for(int i=0; i<n; i++){
        if(visited[i]) continue;

        visited[i] = true;
        vec.push_back(i);
        makePermutation(n, r, ret, vec, visited); 
        vec.pop_back();
        visited[i] = false;
    }
}

int main(){
    int n = 3;
    int r = 3;
    vector<vector<int>> ret;
    vector<int> vec;
    vector<bool> visited(n, false);

    makePermutation(n, r, ret, vec, visited);

    return 0;
}
```
