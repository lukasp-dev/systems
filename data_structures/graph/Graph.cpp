#include <bits/stdc++.h>
using namespace std;

class Graph {
private:
    vector<vector<int>> adjacencyList;
    bool directed;

    static size_t checkedVertexCount(int vertexCount) {
        if (vertexCount < 0) {
            throw invalid_argument(
                "Vertex count cannot be negative"
            );
        }

        return static_cast<size_t>(vertexCount);
    }

    void validateVertex(int vertex) const {
        if (vertex < 0 ||
            vertex >= static_cast<int>(adjacencyList.size())) {
            throw out_of_range("Invalid vertex");
        }
    }

    // DFS 재귀 helper
    void dfsHelper(
        int current,
        vector<bool>& visited,
        vector<int>& traversalOrder
    ) const {
        // TODO
    }

    // 무방향 그래프 cycle detection helper
    bool hasUndirectedCycleHelper(
        int current,
        int parent,
        vector<bool>& visited
    ) const {
        // TODO
        return false;
    }

    // 방향 그래프 cycle detection helper
    bool hasDirectedCycleHelper(
        int current,
        vector<int>& state
    ) const {
        // TODO
        return false;
    }

public:
    explicit Graph(
        int vertexCount,
        bool isDirected = false
    )
        : adjacencyList(checkedVertexCount(vertexCount)), directed(isDirected) {}

    int vertexCount() const {
        return static_cast<int>(
            adjacencyList.size()
        );
    }

    bool isDirected() const {
        return directed;
    }

    void addEdge(int from, int to) {
        validateVertex(from);
        validateVertex(to);

        adjacencyList[from].push_back(to);

        if (!directed) {
            adjacencyList[to].push_back(from);
        }
    }

    const vector<int>& neighbors(int vertex) const {
        validateVertex(vertex);
        return adjacencyList[vertex];
    }

    // 너비 우선 탐색
    vector<int> bfs(int start) const {
        validateVertex(start);

        vector<bool> visited(vertexCount(), false);
        vector<int> traversalOrder;
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while(!q.empty()){
            int current = q.front(); q.pop();

            traversalOrder.push_back(current);
            
            for(int nei : adjacencyList[current]){
                if(!visited[nei]) {
                    visited[nei] = true;
                    q.push(nei);
                }
            }
        }

        return traversalOrder;
    }

    // 깊이 우선 탐색
    vector<int> dfs(int start) const {
        // TODO
        return {};
    }

    // 전체 그래프 DFS
    // disconnected graph까지 모두 순회
    vector<int> dfsAll() const {
        // TODO
        return {};
    }

    // 연결 요소 개수
    // 주로 무방향 그래프에서 사용
    int countConnectedComponents() const {
        // TODO
        return 0;
    }

    // 두 노드가 연결되어 있는지
    bool hasPath(int source, int destination) const {
        // TODO
        return false;
    }

    // 그래프에 cycle이 있는지
    // directed 여부에 따라 다른 방식 사용
    bool hasCycle() const {
        // TODO
        return false;
    }

    // 이분 그래프 여부
    bool isBipartite() const {
        // TODO
        return false;
    }

    // 위상 정렬
    // 방향 그래프에서만 가능
    // cycle이 있으면 빈 vector 반환
    vector<int> topologicalSort() const {
        // TODO
        return {};
    }

    // 그래프 출력
    void print() const {
        for (int vertex = 0;
             vertex < vertexCount();
             ++vertex) {

            cout << vertex << ": ";

            for (int neighbor :
                 adjacencyList[vertex]) {
                cout << neighbor << ' ';
            }

            cout << '\n';
        }
    }
};