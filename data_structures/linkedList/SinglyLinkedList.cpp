#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
struct Node {
    T val;
    Node<T>* next; // Need to specify <T>
};

template <typename T>
class LinkedList {
    Node<T>* head; // Need to specify <T>
public:
    LinkedList() : head{nullptr} {}

    ~LinkedList() {
        while (head) {
            Node<T>* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    void addFront(T data) {
        Node<T>* newNode = new Node<T>{data, head};
        head = newNode;
    }

    void addBack(T data) {
        Node<T>* newNode = new Node<T>{data, nullptr};
        if (!head) {
            head = newNode;
            return;
        }
        Node<T>* tmp = head;
        while (tmp->next != nullptr) {
            tmp = tmp->next;
        }
        tmp->next = newNode;
    }

    T removeFront() {
        if (head == nullptr) throw runtime_error("List is empty.");
        Node<T>* tmp = head;
        T removed = tmp->val;
        head = head->next;
        delete tmp;
        return removed;
    }

    T removeBack() {
        if (head == nullptr) throw runtime_error("List is empty.");
        
        if (head->next == nullptr) {
            T removed = head->val;
            delete head;
            head = nullptr;
            return removed;
        }

        Node<T>* prev = head;
        while (prev->next->next) {
            prev = prev->next;
        }
        Node<T>* curr = prev->next;
        T removed = curr->val;
        delete curr;
        prev->next = nullptr;
        return removed;
    }

    int find(T data) {
        Node<T>* tmp = head;
        int idx = 0;
        while (tmp) {
            if (tmp->val == data) return idx;
            tmp = tmp->next;
            ++idx;
        }
        return -1;
    }

    Node<T>* reverseRecursive(Node<T>* node) {
        if (!node || !node->next) return node;
        Node<T>* newHead = reverseRecursive(node->next);
        node->next->next = node;
        node->next = nullptr;
        return newHead;
    }

    void reverse() {
        head = reverseRecursive(head);
    }
    
    /**
     * This only works with std::streamable
     * if not, have to overload the << operator
     */
    void print() {
        Node<T>* tmp = head;
        while(tmp) {
            cout << tmp->val << " -> ";
            tmp = tmp->next;
        }
        cout << "NULL" << endl;
    }
};

int main() {
    // Specify the template type <int>
    LinkedList<int> linked_list;
    linked_list.addFront(30);
    linked_list.addFront(20);
    linked_list.addFront(10);
    linked_list.addBack(40);

    cout << "Original: ";
    linked_list.print();

    linked_list.reverse();
    cout << "Reversed: ";
    linked_list.print();

    cout << "Index of 20: " << linked_list.find(20) << endl;

    return 0;
}