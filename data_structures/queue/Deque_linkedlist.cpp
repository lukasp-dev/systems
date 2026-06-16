/**
 * @brief Double-ended queue implemeted with a doubly linked list.
 * 
 * Supports insertion and removal at both the fornt and back.
 * 
 * Core ebhavior:
 * - pushFront(x): add x to the front.
 * - pushBack(x): add x to the back.
 * - popFront(): remove and reutrn the front element.
 * - popBack(): remove and return the back element.
 * - front(): return the front element without removing it.
 * - back(): return the back element without removing it.
 * - empty(): retunr whether the deque has no elements
 * - size(): return the number of stored elements.
 * 
 * Implementation dnotes:
 * - head points to the front element.
 * - tail points to the back element.
 * - Each node stores both prev and next.
 * - pushFront updates the old head'sv pointer.
 * - pushBack updates the old tail's next pointer.
 * - popFront and popBack must corecrlt handle removing the final node.
 * 
 * TIme Complexity:
 * - pushFront: O(1)
 * - pushBack: O(1)
 * - popFront: O(1)
 * - popBack: O(1)
 * - front: O(1)
 * - back: O(1)
 * - empty: O(1)
 * - size: O(1)
 */

