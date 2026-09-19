#include <vector>
#include <queue>
using namespace std;

class TopologicalSort {
private:
    int V;
    vector<vector<int>> graph;

public:
    TopologicalSort() : V(V), graph(V){}
    
    void addEdge(int u, int v) {
        graph[u].push_back(v);
    }

    vector<int> sort() {
        vector<int> indegree(V, 0);
        
        for(int u=0; u<V; u++) {
            for(int v : graph[u]) {
                indegree[v]++;
            }
        }

        queue<int> q;
        for(int i = 0; i < V; i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }

        vector<int> result;

        while(!q.empty()) {
            int u = q.front(); q.pop();
            
            result.push_back(u);

            for(int v : graph[u]) {
                indegree[v]--;

                if(indegree[v] == 0) {
                    q.push(v);
                }
            }
        }

        // Cycle Exist
        if(result.size() != V) {
            return {};
        }

        return result;
    }
};