#include <string>

using namespace std;

class Trie {
private:
    struct Node {
        Node* children[26];
        bool isEnd;

        Node() : children{}, isEnd(false) {}
    };

    Node* root;

    void destroy(Node* node) {
        if (node == nullptr) {
            return;
        }

        for (Node* child : node->children) {
            destroy(child);
        }

        delete node;
    }

public:
    Trie() : root(new Node()) {}

    ~Trie() {
        destroy(root);
    }

    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    void insert(const string& word) {
        Node* current = root;

        for (char c : word) {
            int index = c - 'a';

            if (current->children[index] == nullptr) {
                current->children[index] = new Node();
            }

            current = current->children[index];
        }

        current->isEnd = true;
    }

    bool search(const string& word) const {
        const Node* current = root;

        for (char c : word) {
            int index = c - 'a';

            if (current->children[index] == nullptr) {
                return false;
            }

            current = current->children[index];
        }

        return current->isEnd;
    }

    bool startsWith(const string& prefix) const {
        const Node* current = root;

        for (char c : prefix) {
            int index = c - 'a';

            if (current->children[index] == nullptr) {
                return false;
            }

            current = current->children[index];
        }

        return true;
    }
};