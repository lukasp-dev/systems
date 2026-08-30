#include <bits/stdc++.h> 
using namespace std;

void dfs(vector<vector<bool>>& a, vector<bool>& visited, int here){
    visited[here] = true;

    for(int next = 0; next < 4; ++next){
        if(a[here][next] && !visited[next]){
            dfs(a, visited, next);
        }
    }
}

int main(){
    vector<vector<bool>> a(4, vector<bool>(4, false));
    a[0][1] = 1;
    a[1][0] = 1;

    a[1][2] = 1;
    a[2][1] = 1;

    a[0][2] = 1;
    a[2][0] = 1;

    a[0][3] = 1;
    a[3][0] = 1;

    vector<bool> visited(4, false);

    int _cnt = 0;

    for(int i=0; i<4; ++i){
        if(!visited[i]){
            _cnt++;
            dfs(a, visited, i);
        }
    }
    
    cout << _cnt << "\n";

    return 0;
}

/**
 * 인접 행렬: 그래프에서 정점과 간선의 관계를 나타내는 bool 타입의 정사각형 행렬
 * 0은 두 정점 사이의 경로가 없음, 1은 두 정점 사이의 경로가 있음
 * e.g.) a[from][to] = 1;
 */