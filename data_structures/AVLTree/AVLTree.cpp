#include <iostream>
#include <algorithm>
using namespace std;

class AVLTree {
private:
    struct Node {
        int val;
        int height;
        Node* left;
        Node* right;

        explicit Node(int value) : val(value), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // ---
    // Basic Helpers
    // ---

    int getHeight(const Node* node) const {
        return node == nullptr ? 0 : node->height;
    }

    int getBalance(const Node* node) const {
        if(node == nullptr) {
            return 0;
        }

        return getHeight(node->left) - getHeight(node->right);
    }

    void updateHeight(Node* node) {
        if(node == nullptr) {
            return;
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }

    // ---
    // Rotations
    // ---

    Node* rotateRight(Node* y) {
        // TODO:
        //        y                 x
        //       / \               / \
        //      x   C     ->       A   y
        //     / \                   / \
        //    A   B                 B   C
        
        // y->left 가 nullptr 가 아닐 떄 불렀다고 했을 때.
        Node* x = y->left;
        Node* child = x->right;
        
        x->right = y;
        y->left = child;

        // x가 회전전의 y를 읽을 수 있으므로
        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        // TODO:
        //      x                     y
        //     / \                   / \
        //    A   y       ->        x   C
        //       / \               / \
        //      B   C             A   B
        // x->right 이 nullptr 가 아닐 때 불렸다고 했을 때.
        Node* y = x->right;
        Node* child = y->left;

        y->left = x;
        x->right = child;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // ---
    // Rebalancing
    // ---

    Node* rebalance(Node* node) {
        if(node == nullptr) {
            return nullptr;
        }

        updateHeight(node);

        int balance = getBalance(node);

        // Left Heavy
        if (balance > 1) {
            // TODO
            // LL case
            // LR case
        }

        // Right Heavy
        if (balance < -1) {
            // TODO
            // RR case
            // RL case
        }

        return node;
    }

    // ---
    // Recursive operations
    // ---
    Node* insertNode(Node* node, int value, bool& inserted) {
        if(node == nullptr) {
            Node* node = new Node(value);
            inserted = true;

            return new Node;
        }

        if(node->val < value){
            node->right = insertNode(node->right, value, inserted);
        } else if(node->val > value){
            node->left = insertNode(node->left, value, inserted);
        } else {
            return node;
        }

        return rebalance(node);
    }

    Node* findMin(Node* node) const {
        if(node == nullptr) {
            return nullptr;
        }

        if(node->left == nullptr) {
            return node;
        }

        return findMin(node->left);
    }

    Node* eraseNode(Node* node, int value, bool& erased) {
        if(node == nullptr) {
            return nullptr;
        }

        if(value < node->val) {
            node->left = eraseNode(node->left, value, erased);
        } else if(value < node->val) {
            node->right = eraseNode(node->right, value, erased);
        }
        erased = true;

        if(!node->left && !node->right) {
            delete node;
            return nullptr;
        } 
        
        if (!node->left) {
            Node* child = node->right;
            delete node;
            return child;
        } 
        
        if (!node->right) {
            Node* child = node->left;
            delete node;
            return child;
        } 

        Node* successor = findMin(node->right);
        node->val = successor->val;
        bool dummy = false;
        node->right = eraseNode(node->right, node->val, dummy);

        return rebalance(node);
    }

    bool searchNode(const Node* node, int value) const {
        if(node == nullptr) {
            return false;
        }

        if(value < node->val) {
            return searchNode(node->left, value);
        } else if(value > node->val) {
            return searchNode(node->right, value);
        }

        return node;
    }

    void inorderNode(const Node* node) const {
        if(node == nullptr) return;

        inorderNode(node->left);
        cout << node->val << "\n";
        inorderNode(node->right);
    }

    void destroyNode(Node* node) {
        if(!node) return; 

        destroyNode(node->left);
        destroyNode(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        destroyNode(root);
    }

    bool insert(int value) {
        bool inserted = false;
        root = insertNode(root, value, inserted);
        return inserted;
    }

    bool erase(int value) {
        bool erased = false;
        root = eraseNode(root, value, erased);
        return erased;
    }

    bool search(int value) const {
        return searchNode(root, value);
    }

    bool empty() const {
        return root == nullptr;
    }

    int height() const {
        return getHeight(root);
    }

    void inorder() const {
        inorderNode(root);
        cout << '\n';
    }
};
