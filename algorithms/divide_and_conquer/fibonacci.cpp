#include <bits/stdc++.h>
using namespace std;

int fibonacci(int n){
    if(n == 0 || n==1){
        return n;
    }else{
        return fibonacci(n-1) + fibonacci(n-2);
    }
}

int betterFib(int n){
    if (n == 0 || n == 1) return n;
    
    int prev2 = 0;
    int prev1 = 1;
    int current = 0;
    
    for (int i = 2; i <= n; ++i) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}


int main(){
    int N;

    cin >> N;

    cout << "the fibonacci number for " << N << " is: " << fibonacci(N) << "\n";
    cout << "with the better fibonacci number algorithm: " << betterFib(N) << "\n";
    return 0;
}