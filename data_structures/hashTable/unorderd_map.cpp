#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 * std::unordered_map<int, int>의 단순화된 버전
 * Collision resolution: separate chaining
 */
class UnorderedMap {
private:
    struct Node {
        int key;   // 원래 key
        int val;
        Node* next;

        Node(int k, int v, Node* n = nullptr)
            : key(k), val(v), next(n) {}
    };

    std::vector<Node*> buckets;
    std::size_t element_count;

    std::size_t getBucketIndex(int key) const {
        return std::hash<int>{}(key) % buckets.size();
    }

    Node* findNode(Node* node, int lookupKey) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->key == lookupKey) {
            return node;
        }

        return findNode(node->next, lookupKey);
    }

    const Node* findNode(
        const Node* node,
        int lookupKey
    ) const {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->key == lookupKey) {
            return node;
        }

        return findNode(node->next, lookupKey);
    }

    void destroyBucket(Node* node) {
        if (node == nullptr) {
            return;
        }

        destroyBucket(node->next);
        delete node;
    }

    void destroyBuckets() {
        for (Node*& head : buckets) {
            destroyBucket(head);
            head = nullptr;
        }

        element_count = 0;
    }

    void rehash(std::size_t newBucketCount) {
        if (newBucketCount == 0) {
            throw std::invalid_argument(
                "Bucket count must be greater than zero"
            );
        }

        std::vector<Node*> newBuckets(
            newBucketCount,
            nullptr
        );

        for (Node* head : buckets) {
            Node* current = head;

            while (current != nullptr) {
                Node* next = current->next;

                std::size_t newIndex =
                    std::hash<int>{}(current->key)
                    % newBucketCount;

                current->next = newBuckets[newIndex];
                newBuckets[newIndex] = current;

                current = next;
            }
        }

        buckets = std::move(newBuckets);
    }

public:
    explicit UnorderedMap(std::size_t bucketCount = 8)
        : buckets(bucketCount, nullptr),
          element_count(0) {
        if (bucketCount == 0) {
            throw std::invalid_argument(
                "Bucket count must be greater than zero"
            );
        }
    }

    ~UnorderedMap() {
        destroyBuckets();
    }

    UnorderedMap(const UnorderedMap&) = delete;

    UnorderedMap& operator=(
        const UnorderedMap&
    ) = delete;

    void put(int key, int value) {
        std::size_t bucketIndex =
            getBucketIndex(key);

        Node* found =
            findNode(buckets[bucketIndex], key);

        if (found != nullptr) {
            found->val = value;
            return;
        }

        buckets[bucketIndex] =
            new Node(
                key,
                value,
                buckets[bucketIndex]
            );

        element_count++;

        double loadFactor =
            static_cast<double>(element_count)
            / buckets.size();

        if (loadFactor > 0.75) {
            rehash(buckets.size() * 2);
        }
    }

    int get(int key) const {
        std::size_t bucketIndex =
            getBucketIndex(key);

        const Node* found =
            findNode(buckets[bucketIndex], key);

        if (found == nullptr) {
            throw std::out_of_range(
                "The node with the given key was not found"
            );
        }

        return found->val;
    }

    bool contains(int key) const {
        std::size_t bucketIndex =
            getBucketIndex(key);

        return findNode(
            buckets[bucketIndex],
            key
        ) != nullptr;
    }

    bool erase(int key) {
        std::size_t bucketIndex =
            getBucketIndex(key);

        Node* current = buckets[bucketIndex];
        Node* previous = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (previous == nullptr) {
                    buckets[bucketIndex] =
                        current->next;
                } else {
                    previous->next =
                        current->next;
                }

                delete current;
                element_count--;
                return true;
            }

            previous = current;
            current = current->next;
        }

        return false;
    }

    std::size_t size() const {
        return element_count;
    }

    bool empty() const {
        return element_count == 0;
    }
};