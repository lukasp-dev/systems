/**
 * 세계적인 도둑 상덕이는 보석점을 털기로 결심했다.
 * 상덕이가 털 보석점에는 보석이 총 N개 있다. 
 * 각 보석은 무게 Mi와 가격 Vi를 가지고 있다. 
 * 상덕이는 가방을 K개 가지고 있고, 각 가방에 담을 수 있는 최대 무게는 Ci이다. 
 * 가방에는 최대 한 개의 보석만 넣을 수 있다.
 * 
 * 상덕이가 훔칠 수 있는 보석의 최대 가격을 구하는 프로그램을 작성하시오.
 * 
 * 입력
 * 첫째 줄에 N과 K가 주어진다. (1 ≤ N, K ≤ 300,000)
 * 다음 N개 줄에는 각 보석의 정보 Mi와 Vi가 주어진다. (0 ≤ Mi, Vi ≤ 1,000,000)
 * 다음 K개 줄에는 가방에 담을 수 있는 최대 무게 Ci가 주어진다. (1 ≤ Ci ≤ 100,000,000)
 * 모든 숫자는 양의 정수이다.
 * 
 * 출력
 * 첫째 줄에 상덕이가 훔칠 수 있는 보석 가격의 합의 최댓값을 출력한다.
 */

#include <bits/stdc++.h>
using namespace std;

struct cmp{
    bool operator()(const pair<int, int>& a, const pair<int, int>& b){
        return a.second > b.second;
    }  
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    vector<pair<int, int>> gems;
    int wei, price;
    for(int i=0; i<N; ++i){
        cin >> wei >> price;
        gems.push_back(make_pair(wei, price));
    }

    sort(gems.begin(), gems.end(), cmp());

    multiset<int> max_wei;
    for(int i=0; i<K; ++i){
        cin >> wei;
        max_wei.insert(wei);
    }

    long long total_wei = 0;
    for(auto& [wei, price] : gems){
        auto it = max_wei.lower_bound(wei);
        if(it != max_wei.end()){
            total_wei += price;
            max_wei.erase(it);
        }
    }

    cout << total_wei << "\n";

    return 0;
}