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
            node->left = insertNode(node, value, inserted);
        } else if(value > node->val){
            node->right = insertNode(node, value, inserted);
        }
        return node;
    };
    
    bool searchNode(const Node* node, int value) const {
        if(node == nullptr) return false;
        
        if(value < node->val){
            return searchNode(node->left, value);
        } else if(value > node->val){
            return searchNode(node->right, value);
        } else {
            return true;
        }
    };

    Node* eraseNode(Node* node, int value, bool& erased) {
        if(node == nullptr) return nullptr;

        if(value <  node->val) {
            node->left = eraseNode(node->left, value, erased);
        } else if(value > node->val) {
            node->right = eraseNode(node->right, value, erased);
        } else { // found the node to delete
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

            // Case 3: both child
            

        }
    };

    const Node* findMinNode(const Node* node) const {
        if(node == nullptr) return nullptr;
    };
    const Node* findMaxNode(const Node* node) const {
        if(node == nullptr) return nullptr;
    };

    void inorder(const Node* node, vector<int>& result) const;
    void preorder(const Node* node, vector<int>& result) const;
    void postorder(const Node* node, vector<int>& result) const;

    int height(const Node* node) const;

    bool validateBST(
        const Node* node,
        optional<long long> lowerBound,
        optional<long long> upperBound
    ) const;

    void destroy(Node* node);

public:
    BST();
    ~BST();

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    bool insert(int value){
        bool inserted = false;
        root = insertNode(root, value, inserted);
        return inserted;
    };
    bool search(int value) const {
        return search(value);
    };

    bool erase(int value){
        bool erased = false;
        root = eraseNode(root, value, erased);
        return erased;
    };

    optional<int> findMin() const;
    optional<int> findMax() const;

    vector<int> inorder() const;
    vector<int> preorder() const;
    vector<int> postorder() const;
    vector<int> levelOrde() const;

    int height() const;
    bool empty() const;
    bool validateBST() const;
};