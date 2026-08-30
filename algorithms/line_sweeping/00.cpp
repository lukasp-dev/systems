/**
 * 큰돌이는 못 말리는 화가야!
 * 
 * 큰돌은 새로운 예술 프로젝트를 위해 도화지 위에 여러 선을 긋기로 했습니다.
 * 그는 도화지의 특정 위치에서 다른 위치까지 여러 번 선을 그릴 수 있습니다.
 * 하지만 선을 그을 때마다 이미 그어진 선 위에 다시 선을 그을 수 있으며,
 * 이렇게 여러번 그어진 부분은 한 번만 계산됩니다.
 * 
 * 큰돌이 도화지에 그린 모든 선들의 총 길이를 구하는 프로그램을 작성하세요.
 * 여러 번 그어진 선이 있더라도, 겹쳐진 부분은 한 번만 계산해야 합니다.
 * 
 * 천째 줄에 선을 그은 횟수 N(1<=N<=1,000,000)이 주어지고 그 다음 N개의 줄에는
 * 선을 그을 때 선택한 두 범의 위치 a, b(-1,000,000,000 <= a, b <= 1,000,000,000)
 * 가 주어진다.
 * 
 * 입력 1
 * 5
 * 0 2 
 * 1 5 
 * 3 7
 * ...
 */

#include <bits/stdc++.h>
using namespace std;

int main(){    
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int a, b;
    vector<pair<int, int>> record;
    for(int i=0; i<n; ++i){
        cin >> a >> b;
        if(a > b) swap(a, b);
        record.push_back(make_pair(a, b));
    }

    sort(record.begin(), record.end());

    auto [left, right] = record[0];
    long long total_length = 0;
    for(int i=1; i<n; i++){
        auto [curr_l, curr_r] = record[i];
        if(right < curr_l){
            total_length += (right - left);
            left = curr_l;
            right = curr_r;
        } else {
            right = max(right, curr_r);
        }
    }
    
    total_length += (right - left);
    cout << total_length << "\n";

    return 0;
}