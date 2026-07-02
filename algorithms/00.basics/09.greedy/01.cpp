/**
 * 골동푼ㅁ 수집가인 큰돌은 세계 여러 곳을 여행하며  희귀한 골동품을 수집해 왔습니다.
 * 이번에 큰돌은 거대한 골동품 박람회에 참섬하리고 했고, 박람회에서는 다양한 골동품들이 
 * 전시되어 있으며, 각각의 골동품은 무게 M[i]과 가치 V[i]를 가지고 있습니다.
 * 큰돌은 자신이 수집한 골동품들을 보관할 수 있는 고급가방 K개를 가지고 있습니다.
 * 각 가방은 최대 C[i] 무게까지 골동품을 담을 수 있으며, 가방 하나에는 최대 한 개의 골동품만 넣을 수 있습니다.
 * 큰돌을 가능한 한 최대한 가치 있는 골동품들을 가방에 넣어서 가져가려고 합니다.
 * ㅇ큰돌이 수집할 수 있는 골동품의 가치의 합의 최대ㅐ값을 구하는 프로그램을 작성하세요.
 * 
 * 
 * 입력
 * 
 * 첫째 줄에 골동품의 수 N과 가방의 수 K가 주어진다.
 * (1<= N, K <= 300,000)
 * 
 * 다음 N개의 줄에는 각 골동품의 정보 (M[i], V[i])가 주어진다.
 * (1 <= M[i], V[i] <= 1,000,000,000)
 *
 * 다음 K개의 줄에는 각 가방이 담을 수 있는 최대 무게 C[i]가 주어진다.
 * (1 <= C[i] <= 1,000,000,000)
 *
 *
 * 출력
 *
 * 큰돌이 가져갈 수 있는 골동품 가치의 합의 최댓값을 출력한다.
 * (하나의 가방에는 최대 한 개의 골동품만 넣을 수 있고,
 *  골동품의 무게 M[i]가 가방의 용량 C[j]보다 작거나 같아야 담을 수 있다.)
 *
 *
 * 예제 입력
 *
 * 3 2
 * 6 13
 * 4 8
 * 2 6
 * 5
 * 4
 *
 * 예제 출력
 *
 * 14
 *
 * 설명)
 *  - 가방 용량: 5, 4
 *  - 가치가 큰 골동품부터 담을 수 있는 가방에 배정한다.
 *  - (M=6, V=13): 용량 5, 4 중 6을 담을 수 있는 가방이 없어 포기.
 *  - (M=4, V=8) : 담을 수 있는 가방(용량 4, 5) 중 하나에 배정.
 *  - (M=2, V=6) : 남은 가방에 배정.
 *  - 합 = 8 + 6 = 14
 * */

#include <bits/stdc++.h>
using namespace std;

struct cmp {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b){
        return a.second > b.second;
    }
};

int main(){
    int num_of_antique, num_of_bags;
    cin >> num_of_antique >> num_of_bags;

    vector<pair<int, int>> antique;
    int wei, val;
    for(int i=0; i < num_of_antique; ++i){
        cin >> wei >> val;
        antique.push_back(make_pair(wei, val));
    }

    sort(antique.begin(), antique.end(), cmp());   
    
    multiset<int> bags;
    int bag_wei;
    for(int i=0; i<num_of_bags; ++i){
        cin >> bag_wei;
        bags.insert(bag_wei);
    }
    
    // now iterating through the antique, do the binary search.
    long long total_weight = 0;

    for(int i=0; i<num_of_antique; ++i){
        auto [wei, val] = antique[i];
        auto it = bags.lower_bound(wei);
        if(it != bags.end()) {
            total_weight += val;
            bags.erase(it);
        }
    }

    cout << total_weight;

    return 0;
}