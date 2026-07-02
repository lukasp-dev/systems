/**
 * 큰돌 교수님의 과제는 너무 어려워!!
 * 
 * 큰돌 교수님은 데이터 분석 과목을 담당하고 있으며, 학생들의 성정 분석을 과제로 내주셨습니다.
 * 교수님은 과제에서 최하위 성적을 받은 5명의 학생에게 추가적인 지도를 해주기로 결정했습니다.
 * 이들 중 성적이 좋지 않은 5명을 선발하여 따로 지도를 해주려고 합니다.
 * 
 * 교수님을 돕기 위해 학생들의 최종 성적이 주어질 때, 서적이 좋지 않은 5명의 학생을 선택하여
 * 성적이 낮은 순서대로 출력하는 프로그램을 작성하세요.
 * 
 * 입력
 * 첫째 줄에 학생의 수 N 이 주어집니다. (6<=N<=50,000,000)
 */

#include <bits/stdc++.h>
using namespace std;

struct cmp{
  bool operator()(const int& a, const int& b){
    return a < b;
  }  
};

int main(){
    int N;
    cin >> N;

    priority_queue<int, vector<int>, cmp> pq;

    int curr;
    for(int i=0; i<N; ++i){
        cin >> curr;
        pq.push(curr);
        if(pq.size() > 5){
            pq.pop();
        }
    }

    stack<int> tmp;
    while(!pq.empty()){
        tmp.push(pq.top());
        pq.pop();
    }

    while(!tmp.empty()){
        cout << tmp.top() << "\n";
        tmp.pop();
    }

    return 0;
}