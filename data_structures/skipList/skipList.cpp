/**
 * A skip list is a sorted linked list with multiple short cut levels. I search from the highest level, 
 * move right while the next value is smaller than the target, and move down otherwise, For insert and delete,
 * I first collect the predecessor node at each level in an update array, then rewire pointers. 
 * Each node is promoted randomly with probability 1/2, so the expected height is O(log n), and search, insert, and delete
 * are expected O(log n). Space is expected O(n)
 */

#include <bits/stdc++.h>
using namespace std;

class SkipList{
private:
    struct Node {
        int val;
        vector<Node*> next;

        Node(int v, int level):val(v), next(level, nullptr){}
    };

    int getRandomHeight(){
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution dist(0, 1);
        
        int randomLevel = 1;
        while(randomLevel < MAX_LEVEL && dist(gen) == 1){
            ++randomLevel;
        }

        return randomLevel;
    }
    
    static int constexpr MAX_LEVEL = 32;
    Node* head;
    int currentLevel; // tracking the current height
public:
    SkipList(){
        head = new Node(INT_MIN, MAX_LEVEL);
        currentLevel = 1;
    }

    bool search(int target){
        Node* cur = head;

        for(int level = currentLevel - 1; level>=0; --level){
            while(cur->next[level] && cur->next[level]->val < target){
                cur = cur->next[level];
            }
        }

        cur = cur->next[0];

        return cur && cur->val == target;
    }

    void add(int v){
        Node* cur = head;
        vector<Node*> update(MAX_LEVEL, nullptr);

        for(int level = currentLevel - 1; level>=0; --level){
            while(cur->next[level] && cur->next[level]->val < v){
                cur = cur->next[level];
            }
            update[level] = cur;
        }

        int randomLevel = getRandomHeight();
        if(randomLevel > currentLevel){
            for(int level = currentLevel; level < randomLevel; ++level){
                update[level] = head;
            }

            currentLevel = randomLevel;
        }

        Node* newNode = new Node(v, randomLevel);

        for(int level = 0; level < randomLevel; ++level){
            newNode->next[level] = update[level]->next[level];
            update[level]->next[level] = newNode;
        }
    }

    bool erase(int target){
        Node* cur = head;
        vector<Node*> update(MAX_LEVEL, nullptr);

        for(int level = currentLevel - 1; level >= 0; --level){
            while(cur->next[level] && cur->next[level]->val < target){
                cur = cur->next[level]; 
            }
            update[level] = cur;
        }
        
        Node* erase_node = update[0]->next[0];

        if(!erase_node || erase_node->val != target) return false;

        for(int level = 0; level < currentLevel; ++level){
            if(update[level]->next[level] != erase_node) continue;
            update[level]->next[level] = erase_node->next[level];
        }

        delete erase_node;

        while(currentLevel > 1 && head->next[currentLevel - 1] == nullptr) --currentLevel;

        return true;
    }
};

int main(){
    
    return 0;
}