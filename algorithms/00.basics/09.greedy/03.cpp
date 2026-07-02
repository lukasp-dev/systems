/**
 * 순회강연
 * 
 * 한 저명한 학자에게 n(0 ≤ n ≤ 10,000)개의 대학에서 강연 요청을 해 왔다. 
 * 각 대학에서는 d(1 ≤ d ≤ 10,000)일 안에 와서 강연을 해 주면 p(1 ≤ p ≤ 10,000)만큼의 강연료를 지불하겠다고 알려왔다.
 * 각 대학에서 제시하는 d와 p값은 서로 다를 수도 있다. 이 학자는 이를 바탕으로, 가장 많은 돈을 벌 수 있도록 순회강연을 하려 한다. 
 * 강연의 특성상, 이 학자는 하루에 최대 한 곳에서만 강연을 할 수 있다.
 * 
 * 예를 들어 네 대학에서 제시한 p값이 각각 50, 10, 20, 30이고, d값이 차례로 2, 1, 2, 1 이라고 하자. 
 * 이럴 때에는 첫째 날에 4번 대학에서 강연을 하고, 둘째 날에 1번 대학에서 강연을 하면 80만큼의 돈을 벌 수 있다.
 * 
 * 입력
 * 첫째 줄에 정수 n이 주어진다, 다음 n개의 줄에는 각 대학에서 제시한 p값과 d값이 주어진다.
 * 
 * 출력
 * 첫째 줄에 최대로 벌 수 있는 돈을 출력한다.
 */

#include <bits/stdc++.h>
using namespace std;

struct cmp{
    bool operator()(const pair<int, int>& a, const pair<int, int>& b){
        return a.second < b.second;
    }
};

struct cmp_pq{
    bool operator()(const int& a , const int& b){
        return a > b;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; 
    cin >> N;
    int p, d;

    vector<pair<int, int>> requests;

    for(int i=0; i<N; ++i){
        cin >> p >> d;
        requests.push_back(make_pair(p, d));
    }

    sort(requests.begin(), requests.end(), cmp());

    priority_queue<int, vector<int>, cmp_pq> pq;
    for(int i=0; i< requests.size(); ++i){
        auto [price, day] = requests[i];
        pq.push(price);
        if(pq.size() > day){
            pq.pop();
        }
    }

    int total_income = 0;
    while(!pq.empty()){
        total_income += pq.top();
        pq.pop();
    }

    cout << total_income << "\n";

    return 0;
}