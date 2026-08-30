/**
 * 상욱 조교는 동호에게 N개의 문제를 주고서, 각각의 문제를 풀었을 때 컵라면을 몇 개 줄 것인지 제시 하였다. 
 * 하지만 동호의 찌를듯한 자신감에 소심한 상욱 조교는 각각의 문제에 대해 데드라인을 정하였다.
 * 
 * 문제 번호	1	2	3	4	5	6	7
 * 데드라인	    1	1	3	3	2	2	6
 * 컵라면 수	6	7	2	1	4	5	1
 * 위와 같은 상황에서 동호가 2, 6, 3, 1, 7, 5, 4 순으로 숙제를 한다면 2, 6, 3, 7번 문제를 시간 내에 풀어 총 15개의 컵라면을 받을 수 있다.
 * 
 * 문제는 동호가 받을 수 있는 최대 컵라면 수를 구하는 것이다. 위의 예에서는 15가 최대이다.
 * 문제를 푸는데는 단위 시간 1이 걸리며, 각 문제의 데드라인은 N이하의 자연수이다. 또, 
 * 각 문제를 풀 때 받을 수 있는 컵라면 수와 최대로 받을 수 있는 컵라면 수는 모두 231보다 작은 자연수이다.
 * 
 * 입력
 * 7
 * 1 6
 * 1 7
 * 3 2
 * 3 1
 * 2 4
 * 2 5
 * 6 1
 * 
 * 출력
 * 15
 */

#include <bits/stdc++.h>
using namespace std;

struct cmp {
    bool operator()(const int& a, const int& b){
        return a > b;
    }
};

int main(){
    int N;
    cin >> N; 

    vector<pair<int, int>> ramens;
    int n, m;

    for(int i=0; i<N; ++i){
        cin >> n >> m;
        ramens.push_back(make_pair(n, m));
    }

    sort(ramens.begin(), ramens.end());

    priority_queue<int, vector<int>, cmp> pq;
    
    for(auto& [deadline, quantity] : ramens){
        pq.push(quantity);
        
        if(pq.size() > deadline){
            pq.pop();
        }
    }

    long long max_ramen = 0;

    while(!pq.empty()){
        max_ramen += pq.top();
        pq.pop();
    }

    cout << max_ramen << "\n";
    
    return 0;
}

/**
 * Key Takeaways
 * 
 * 전번에 풀었던 문제랑도 비슷하게 deadline 기준으로 정렬된 배열을 돌면서,
 * deadline == the # problems he can have solved.
 * 이거를 기억하고 풀어야한다.
 */