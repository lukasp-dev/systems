#include <bits/stdc++.h>
using namespace std;

int N = 4;
int _cnt = 0;

void dfs(vector<vector<int>>& adj, vector<bool>& visited, int here){
    visited[here] = true;

    for(int next : adj[here]){
        if(!visited[next]){
            dfs(adj, visited, next);
        }
    }
}

int main(){
    vector<vector<int>> adj(N);

    adj[0].push_back(1);
    adj[1].push_back(0);

    adj[1].push_back(2);
    adj[2].push_back(1); 

    adj[0].push_back(2);
    adj[2].push_back(0);

    adj[0].push_back(3);
    adj[3].push_back(0);

    vector<bool> visited(N, false);

    for(int i=0; i<N; ++i) {
        if(!visited[i]) {
            _cnt++;
            dfs(adj, visited, i);
        }
    }

    cout << _cnt << "\n";

    return 0;
}