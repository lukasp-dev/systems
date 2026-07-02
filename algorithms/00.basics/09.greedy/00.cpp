/**
 * 첫 줄에 학생의 수 N이 주어진다 . (1<=N<=100,000)
 * 
 * 다음 N개의 줄에는 각 학생의 도착 시간과 떠나는 시간이 주어진다. 
 * 한 좌석은 동시에 한 명만 쓸 수 있다 (시간이 겹치면 안 됨).
 * 도착 시간과 떠나는 시간은 정수로 주어지며, 도착 시간은 떠나는 시간보다 항상 작거나 같다.
 * 
 * 출력
 * 최대 몇 명의 학생들이 도서관 좌석을 사용할 수 있는지 출력
 * 
 * 5
 * 1 4
 * 2 5 
 * 3 6
 * 5 7
 * 8 9
 */

#include <bits/stdc++.h> 
using namespace std;

vector<pair<int, int>> in_and_out;

struct cmp{
    bool operator()(const pair<int, int>& a, const pair<int, int>& b){
        if(a.second == b.second){
            return a.first < b.first;
        }else{
            return a.second < b.second;
        }
    }
};

int main(){
    int n;
    cin >> n;

    int _in, _out;
    for(int i=0; i<n; ++i){
        cin >> _in >> _out;
        in_and_out.push_back(make_pair(_in, _out));
    }

    sort(in_and_out.begin(), in_and_out.end(), cmp());

    int cnt = 0;
    int last_out = -1;
    for(int i=0; i<n; ++i){
        auto [in, out] = in_and_out[i];
        if(in >= last_out){
            cnt++;
            last_out = out;
        }
    }

    cout << cnt << "\n";
    return 0;
}
