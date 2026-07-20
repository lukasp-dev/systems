#include <bits/stdc++.h>
using namespace std;

class UnionFind {
private:
    vector<int> parent;
    vector<int> size; // 집합 크기는 각 component 의 root의 size만 유효하다.

public:
    explicit UnionFind(int n) : parent(n), size(n, 1){
        iota(parent.begin(), parent.end(), 0); // 세 번째 인자는 채우기 시작할 초기값임.
    }

    // x가 속한 집합의 대표자를 찾는 함수.
    int find(int x){
        if(parent[x] == x) {
            return x;
        }

        parent[x] = find(parent[x]); // path compression
        /*
        예) 4 → 3 → 2 → 1 → 0
        4 ─┐
        3 ─┤
        2 ─┼→ 0
        1 ─┘
        */
        return parent[x];
    }

    // 두 원소가 속한 집합의 root를 찾아서 하나로 합치는 함수
    bool unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);

        if(rootA == rootB) return false;

        // union by size: 작은 트리를 큰 트리 밑에 붙임.
        if(size[rootA] < size[rootB]) {
            swap(rootA, rootB);
        }

        parent[rootB] = rootA;
        size[rootA] += size[rootB];

        return true;
    }

    bool connected(int a, int b) {
        return find(a) == find(b);
    }
};