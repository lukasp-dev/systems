#include <bits/stdc++.h>
using namespace std;

class BST{
private:
    struct Node {
        int val;
        Node* left;
        Node* right;

        explicit Node(int value): val(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Recursive Helpers
    Node* insertNode(Node* node, int value, bool& inserted){
        if(node == nullptr){
            inserted = true;
            return new Node(value);
        }

        if(value < node->val){
            node->left = insertNode(node->left, value, inserted);
        } else if(value > node->val){
            node->right = insertNode(node->right, value, inserted);
        }
        return node;
    }
    
    bool searchNode(const Node* node, int value) const {
        if(node == nullptr) return false;
        
        if(value < node->val){
            return searchNode(node->left, value);
        } else if(value > node->val){
            return searchNode(node->right, value);
        } else {
            return true;
        }
    }

    Node* eraseNode(Node* node, int value, bool& erased) {
        if(node == nullptr) return nullptr;

        if(value <  node->val) {
            node->left = eraseNode(node->left, value, erased);
            return node;
        }
        
        if(value > node->val) {
            node->right = eraseNode(node->right, value, erased);
            return node;
        }

        erased = true;

        // Case 1: No children
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        // Case 2-A: right child only
        if (node->left == nullptr){
            Node* child = node->right;
            delete node;
            return child;
        }

        // Case 2-B: left child only
        if (node->right == nullptr) {
            Node* child = node->left;
            delete node;
            return child;
        }

        // Case 3: both children
        const Node* successor = findMinNode(node->right);
        node->val = successor->val;
        bool dummy = false;
        node->right = eraseNode(node->right, successor->val, dummy);

        return node;
    }

    const Node* findMinNode(const Node* node) const {
        if(node == nullptr) return nullptr;
        if(node->left == nullptr) return node;

        return findMinNode(node->left);
    }
    const Node* findMaxNode(const Node* node) const {
        if(node == nullptr) return nullptr;
        if(node->right == nullptr) return node;

        return findMaxNode(node->right);
    }

    void inorder(const Node* node, vector<int>& result) const {
        if(node == nullptr) return;

        inorder(node->left, result);
        result.push_back(node->val);
        inorder(node->right, result);
    }
    void preorder(const Node* node, vector<int>& result) const {
        if(node == nullptr) return;
        result.push_back(node->val);
        preorder(node->left, result);
        preorder(node->right, result);
    }
    void postorder(const Node* node, vector<int>& result) const {
        if(node == nullptr) return;
        postorder(node->left, result);
        postorder(node->right, result);
        result.push_back(node->val);
    }

    int height(const Node* node) const {
        if(node == nullptr) return 0;

        return 1 + max(height(node->left), height(node->right));
    }

    bool validateBST(
        const Node* node,
        optional<long long> lowerBound,
        optional<long long> upperBound
    ) const {
        if (node == nullptr) return true;

        if (lowerBound && node->val <= *lowerBound){
            return false;
        }

        if (upperBound && node->val >= *upperBound){
            return false;
        }

        return validateBST(node->left, lowerBound, node->val) && validateBST(node->right, node->val, upperBound);
    }

    void destroy(Node* node) { // post order 로 지우면 된다.
        if(node == nullptr) return;

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() {
        destroy(root);   
    }

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    bool insert(int value){
        bool inserted = false;
        root = insertNode(root, value, inserted);
        return inserted;
    }
    bool search(int value) const {
        return searchNode(root, value);
    }

    bool erase(int value){
        bool erased = false;
        root = eraseNode(root, value, erased);
        return erased;
    }

    optional<int> findMin() const {
        const Node* node = findMinNode(root);

        if (node == nullptr) return nullopt;
        return node->val;
    }
    optional<int> findMax() const {
        const Node* node = findMaxNode(root);
        
        if(node == nullptr) return nullopt;
        return node->val;
    }

    vector<int> inorder() const {
        vector<int> result;
        inorder(root, result);
        return result;
    }
    vector<int> preorder() const {
        vector<int> result;
        preorder(root, result);
        return result;
    }
    vector<int> postorder() const {
        vector<int> result;
        postorder(root, result);
        return result;
    }
    vector<int> levelOrder() const {
        if(root == nullptr) return {};

        vector<int> results;

        queue<const Node*> q;
        q.push(root);

        while(!q.empty()){
            const Node* node = q.front(); q.pop();

            if(node->left) q.push(node->left);
            if(node->right) q.push(node->right);

            results.push_back(node->val);
        }

        return results;
    }

    int height() const {
        return height(root);
    }
    bool empty() const {
        return root == nullptr;
    }
    bool validateBST() const {
        return validateBST(root, nullopt, nullopt);
    }
};