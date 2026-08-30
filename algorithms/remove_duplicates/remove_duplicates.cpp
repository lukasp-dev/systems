#include <bits/stdc++.h>
using namespace std;

void print(vector<int>& v) {
    for(int it : v) {
        cout << it << " ";
    }
    cout << "\n";
}

int main() {
    vector<int> v;
    for(int i=1; i<=5; i++){
        v.push_back(i);
        v.push_back(i);
    }
    
    auto it = unique(v.begin(), v.end());
    print(v);

    v.erase(it, v.end());

    print(v);

    
    return 0;
}

/**
 * unique()
 * 
 * 범위 안에 있는 요소 중 앞에서부터 서로를 비교해가며 중복되는 요소를 제거하고 나머지 요소들은 삭제하지 않고 그대로 두는 함수
 */