// Binary Search

#include <iostream>

// 1. Exact Target 찾기
int binarySearchExact(vectotr<int> nums, int target) {
    int l = 0;
    int r = nums.size() - 1;

    while(l <= r) {
        int mid = l + (r-l) / 2;

        if(nums[mid] == target) {
            return mid;
        } else if(nums[mid] < target) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }

    return -1;
}

// 2. First occurence of target
int binarySearchFirstOccurence(vector<int>& nums, int target) {
    int l = 0;
    int r = nums.size() - 1;
    int ans = -1;

    while(l <= r) {
        int mid = l + (r-l) / 2;

        if(nums[mid] == target) {
            ans = mid;
            //더 왼쪽에도 target 이 있을 수 있음.
            r = mid - 1;
        } else if (nums[mid] < target) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }

    return ans;
}

// Last occurence of target
int binarySearchLastOccurence(vector<int>& nums, int target) {
    int l = 0;
    int r = nums.size() - 1;
    int ans = -1;

    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (nums[mid] == target) {
            ans = mid;

            // 더 오른쪽에도 target이 있을 수 있음
            l = mid + 1;
        }
        else if (nums[mid] < target) {
            l = mid + 1;
        }
        else {
            r = mid - 1;
        }
    }

    return ans;
}

/**
Koko에서는 상태가 이런 식이잖아:
F F F F T T T T
        ^
     first true

mid가 가능하면:
r = mid;

왜냐하면 정답이 mid거나 그보다 왼쪽일 수 있으니까.
반대로 mid가 불가능하면:
l = mid + 1;

mid 이하의 false 구간은 전부 버리는 거고.

그래서 범위가:

[l ---------------- r]

        ↓

      [l ----- r]

          ↓

          [x]
          l=r

이렇게 최소 feasible 값 하나로 수렴해.

그래서 네 말대로 감각적으로는:

l을 계속 오른쪽으로 밀어서 first true에 꽂는 풀이라고 생각해도 좋아.
다만 r도 같이 왼쪽으로 당겨지고 있다는 건 기억해야 해.

if (can(mid))
    r = mid;       // 오른쪽 경계를 왼쪽으로
else
    l = mid + 1;   // 왼쪽 경계를 오른쪽으로

결국 둘이 first true에서 만나는 구조야. 
 */

class Solution {
private:
    bool canEatAll(vector<int>& piles, int eating_rate, int h) {
        int time_took = 0;
        for(int pile : piles) {
            time_took += pile/eating_rate;
            if(pile%eating_rate) time_took++;
        }

        return time_took <= h;
    }

public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int l = 1;
        int r = *max_element(piles.begin(), piles.end());

        while(l < r) {
            long long mid = l + (r - l)/2;

            if(canEatAll(piles, mid, h)){
                r = mid;
            } else {
                l = mid + 1;
            }
        }

        return l;
    }
};