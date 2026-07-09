/**
 * A skip list is a sorted linked list with multiple short cut levels. I search from the highest level, 
 * move right while the next value is smaller than the target, and move down otherwise, For insert and delete,
 * I first collect the predecessor node at each level in an update array, then rewire pointers. 
 * Each node is promoted randomly with probability 1/2, so the expected height is O(log n), and search, insert, and delete
 * are expected O(log n). Space is expected O(n)
 */

#include <iostream>
#include <vector>
#include <random>
#include <climits>

using namespace std;

class SkipList {
private:
    struct Node {
        int val;
        vector<Node*> next;
        
        Node(int v, int level) : val(v), next(level, nullptr) {}
    };

    static constexpr int MAX_LEVEL = 32;
    Node* head;
    int currentLevel;

    int randomLevel(){
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<int> dist(0, 1);

        int lvl = 1;

        while(lvl < MAX_LEVEL && dist(gen) == 1){
            ++lvl;
        }
        
        return lvl;
    }

public:
    SkipList() {
        head = new Node(INT_MIN, MAX_LEVEL);
        currentLevel = 1;
    }

    bool search(int target){
        Node* cur = head;
        
        for(int level = currentLevel - 1; level >=0; --level){
            while(cur->next[level] && cur->next[level]->val < target) {
                cur = cur->next[level];
            }
        }

        cur = cur->next[0];

        return cur && cur->val == target;
    }

    void add(int num){
        vector<Node*> update(MAX_LEVEL, nullptr);
        Node* cur = head;

        for (int level = currentLevel - 1; level >= 0; --level){
            while (cur->next[level] && cur->next[level]->val < num){
                cur = cur->next[level];
            }
            update[level] = cur;
        }

        int nodeLevel = randomLevel();

        if (nodeLevel > currentLevel){
            for (int level = currentLevel; level < nodeLevel; ++level){
                update[level] = head;
            }
            currentLevel = nodeLevel;
        }
    
        Node* newNode = new Node(num, nodeLevel);
        
        for (int level = 0; level < nodeLevel; ++level){
            newNode->next[level] = update[level]->next[level];
            update[level]->next[level] = newNode;
        }
    }

    bool erase(int num) {
        vector<Node*> update(MAX_LEVEL, nullptr);
        Node* cur = head;
        
        for (int level = currentLevel - 1; level >=0; --level) {
            while (cur->next[level] && cur->next[level]->val < num){
                cur = cur->next[level];
            }
            update[level] = cur;
        }
        
        Node * target = update[0]->next[0];

        if (!target || target->val != num) return false;

        for(int level = 0; level < currentLevel; ++level) {
            if (update[level]->next[level] != target) break;
        }
        update[level]->next[level] = target->next[level];
    }

    delete target;

    while(currentLevel > 1 && !head->next[currentLevel - 1]) --currentLevel;

    return true;
};

int main(){

    return 0;
}