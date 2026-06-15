#include <bits/stdc++.h>
using namespace std;

int isPrime(int num){
    if(num == 0) return 0;
    if(num == 1) return 0;

    for(int i=2; i<=sqrt(num); ++i){
        if(num%i == 0) return 0;
    }

    return 1;
}

 int go(const vector<int>& rods, int step, int sum){
    if(rods.size() == step){
        return isPrime(sum);
    }

    return go(rods, step+1, sum + rods[step]) + go(rods, step+1, sum);
 }
 

int main(){
    int N; cin >> N;
    vector<int> rods;
    int rod;
    for(int i=0; i<N; ++i){
        cin >> rod;
        rods.push_back(rod);
    }
    vector<int> vec;
    cout << go(rods, 0, 0) << "\n";
}