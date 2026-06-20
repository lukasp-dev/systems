#include <bits/stdc++.h> 
using namespace std;

class BubbleSort{
public:
    void print(vector<int>& arr) const {
        for (int it : arr) {
            cout << it << "\n";
        }
    }
};


int main(){
    int n = 7;
    vector<int> arr = {10, 20, 1, -1, 8, 100, 118};
    
    for(int i=0; i<n-1; ++i){
        for(int j=0; j<n-i-1; ++j){
            if(arr[j] > arr[j+1]){
                swap(arr[j], arr[j+1]);
            }
        }
    }

    BubbleSort bs;
    bs.print(arr);

    return 0;
}