/**
 * 1. 루트 노드 찾기
 *    - Preorder의 가장 첫 번째 원소는 무조건 전체 트리의 루트이다.
 * 2. 왼쪽과 오른쪽 나누기
 *    - Inorder 결과에서 방금 찾은 루트의 위치를 찾고
 *    - Inorder 의 정의가 (왼쪽 -> 루트 -> 오른쪽) 이므로 1을 기준으로 
 *      왼쪽에 있는 숫자들은 전부 왼쪽 자식 트리
 *      오른쪽에 있는 숫자들은 전부 오른쪽 자식 트리에 속한다.
 *    - 쪼개진 서브트리에서 다시 1~2 단계 반복
 */
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr){};
};

unordered_map<int, int> inorder_idx;
int preorder_idx = 0;

Node* buildTree(const vector<int>& preorder, int in_start, int in_end){
    if(in_start > in_end){
        return nullptr;
    }

    int root_val = preorder[preorder_idx++];
    Node* root = new Node(root_val);

    int root_inorder_pos = inorder_idx[root_val];

    root->left = buildTree(preorder, in_start, root_inorder_pos - 1);
    root->right = buildTree(preorder, root_inorder_pos + 1, in_end);

    return root;
}

void printPost(Node* root){
    if(root == nullptr) return;

    printPost(root->left);
    printPost(root->right);
    cout << root->data << " ";
}

int main(){
    vector<int> preorder = {1, 2, 4, 5, 3};
    vector<int> inorder = {4, 2, 5, 1, 3};

    for (int i=0; i<inorder.size(); ++i){
        inorder_idx[inorder[i]] = i;
    }

    Node* root = buildTree(preorder, 0, inorder.size()-1);

    cout << "복원된 트리의 Postorder 결과: ";
    printPost(root);
    cout << "\n";

    return 0;
}
