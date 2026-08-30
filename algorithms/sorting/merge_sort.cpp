#include <bits/stdc++.h>
using namespace std;

class Solution{
public:
    void mergeSort(vector<int>& nums) {
        vector<int> temp(nums.size());
        sortHelper(nums, temp, 0, nums.size()-1);
    }

    void sortHelper(vector<int>& nums, vector<int>& temp, int l, int r){
        // Base case:
        // If the subarray has 0 or 1 element, it is already sorted.
        if(l >= r){
            return;
        }

        int mid = l + (r - l)/2;
        sortHelper(nums, temp, l, mid);
        sortHelper(nums, temp, mid+1, r);

        merge(nums, temp, l, mid, r);
    }

    void merge(vector<int>& nums, vector<int>& temp, int l, int mid, int r){
        int i = l; // the current element in the left half
        int j = mid + 1; // the current element in the right half
        int k = l; // the position where we write into temp.

        while (i <= mid && j <= r) {
            if(nums[i] <= nums[j]){ // stable sort guarantee.
                temp[k] = nums[i];
                i++;
            } else {
                temp[k] = nums[j];
                j++;
            }

            k++;
        }

        while(i <= mid){
            temp[k] = nums[i];
            i++;
            k++;
        }

        while(j <= r){
            temp[k] = nums[j];
            j++;
            k++;
        }

        for(int i=l; i<=r; ++i){
            nums[i] = temp[i];
        }
    }

    void print(vector<int>& vec){
        for(int it : vec){
            cout << it << " ";
        }
        cout << "\n";
    }
};

int main(){
    Solution s;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 100); // 범위가 양 끝 포함
    vector<int> vec;

    for(int i=0; i<10; ++i){
        vec.push_back(dist(gen));
    }

    cout << "original vector: ";
    s.print(vec);
    cout << "\n";

    s.mergeSort(vec);
    cout << "sorted vector: ";
    s.print(vec);

    return 0;
}