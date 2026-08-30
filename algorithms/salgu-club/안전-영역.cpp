#include <bits/stdc++.h>
using namespace std;

int ret = -1;
const vector<int> dy = {-1, 0, 1, 0};
const vector<int> dx = {0, -1, 0, 1};

void dfs(vector<vector<int>>& grid, vector<vector<int>>& visited, int y, int x, int N, int h){
    // maybe do something
    visited[y][x] = 1;

    for(int i=0; i<4; ++i){
        int ny = y + dy[i], nx = x + dx[i];
        
        if(ny < 0 || ny >= N || nx < 0 || nx >= N || visited[ny][nx] || grid[ny][nx] <= h) continue;

        dfs(grid, visited, ny, nx, N, h);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; cin >> N;
    vector<vector<int>> grid(N, vector<int>(N, 0));
    int _maxH = -1;
    
    for(int i=0; i<N; ++i){
        for(int j=0; j<N; ++j){
            cin >> grid[i][j];
            _maxH = max(_maxH, grid[i][j]);
        }
    }

    for(int h = 0; h <= _maxH; ++h){
        vector<vector<int>> visited(N, vector<int>(N, 0));
        int num_of_safe_area = 0;
        
        for(int i=0; i<N; ++i){
            for(int j=0; j<N; ++j){
                if(!visited[i][j] && grid[i][j] > h){
                    dfs(grid, visited, i, j, N, h);
                    num_of_safe_area++;
                }
            }
        }

        ret = max(ret, num_of_safe_area);
    }

    cout << ret << "\n";

    return 0;
}