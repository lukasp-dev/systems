// Monotonic Stack
// stack 안의 값들이 항상 한 방향으로 정렬된 상태를 유지하게 쓰는 기법
/**
 * [3, 1, 4, 2]
 * 우리가 만약에 각 숫자의 오른쪽에서 처음 나오는 더 큰 숫자
 * 를 알고 싶다고 하면,
 * 3->4, 1->4, 4->None, 2->None
 * 이렇게 된다.
 * 
 * Monotonic Stack 의 핵심:
 * 현재 숫자가 stack의 규칙을 깨뜨리는 동안 pop 을 해서 규칙을 유지하게 한다.
 */

// Daily Temperatures
/**
 * 매일의 기온이 담긴 정수 배열 temperatures가 주어진다.
 * answer[i]는 i번째 날 이후에 더 따뜻한 날이 오기까지 기다려야 하는 일수다.
 * 그런 날이 없으면 answer[i] = 0이다.
 *
 * vector<int> dailyTemperatures(vector<int>& temperatures);
 *
 * 예시 1
 * Input:  temperatures = [73,74,75,71,69,72,76,73]
 * Output: [1,1,4,2,1,1,0,0]
 *
 * 예시 2
 * Input:  temperatures = [30,40,50,60]
 * Output: [1,1,1,0]
 *
 * 예시 3
 * Input:  temperatures = [30,60,90]
 * Output: [1,1,0]
 *
 * 제약
 * 1 <= temperatures.length <= 10^5
 * 30 <= temperatures[i] <= 100
 */

#include <bits/stdc++.h>
using namespace std;

/*
// increasing: next smaller
for(int i=0; i<n; i++) {
    while(!st.empty() && nums[st.top()] > nums[i]) st.pop();
    st.push(i);
}

// decreasing: next greater
for(int i=0; i<n; i++) {
    while(!st.empty() && nums[st.top()] < nums[i]) st.pop();
    st.push(i);
}
*/

class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int n = temperatures.size();
        vector<int> ans(n, 0);
        stack<int> monotonic_stack;
        
        for(int i=0; i<n; i++) {
            while(!monotonic_stack.empty() &&
                temperatures[monotonic_stack.top()] < temperatures[i]) {
                    int top_idx = monotonic_stack.top();
                    monotonic_stack.pop();
                    ans[top_idx] = i - top_idx;
            }
            monotonic_stack.push(i);
        }
        return ans;
    }
};
