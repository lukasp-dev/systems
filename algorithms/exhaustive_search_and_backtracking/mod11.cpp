#include <bits/stdc++.h>
using namespace std;

int _max = -1;

void go(const vector<int>& vec, int depth, int sum){
    if(vec.size() == depth){
        _max = max(_max, sum%11);
        return;
    }

    go(vec, depth+1, sum + vec[depth]);
    go(vec, depth+1, sum);
}

int main(){
    int N;
    cin >> N;
    vector<int> vec;

    for(int i=0; i<N; ++i){
        int num;
        cin >> num;
        vec.push_back(num);
    }

    go(vec, 0, 0);
    cout << _max << "\n";

    return 0;
}

/**
 * N과 N개의 자연수가 주어진다.
 * 여기서 몇개의 숫자를 골라 합을 mod 11을 했을 때 나오는 가장 큰수를 구하라.
 * 10
 * 24 35 38 40 49 59 60 67 83 98
 */