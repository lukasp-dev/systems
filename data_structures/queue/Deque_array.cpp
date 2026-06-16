/**
 * @brief Double-ended queue implementated with a dynamic circular array.
 * 
 * Supports insertion and removal at both the front and back.
 * 
 * Core behavior:
 * - pushFront(x): add x to the front.
 * - pushBack(x): add x to the back.
 * - popFront(): remove and return the front element.
 * - popBack(): remove and return the back element.
 * - front(): return the front element without removing it.
 * - back(): return the back element without removing it.
 * - empty(): return whether the deque has no elements.
 * - size(): return the number of stored elements.
 * 
 * Implementation notes:
 * - Uses a circular array so both ends can move without shifting.
 * - frontIndex storeds the physical index of the logical front.
 * - The logical bakc is at (frontIndex + count - 1) % capacity. 
 * - pushFront moves frontIndex one step backward before writing.
 * - pushBack writes at (frontIndex + count) % capacity.
 * - When full,capcity soubles and elements are copied i logical order/
 * 
 * Time Complexity:
 * - pushFront: O(1) Amortized
 * - pushBack: O(1) Amoretized
 * - popFront: O(1)
 * - popBack: O(1)
 * - front: O(1)
 * - back: O(1)
 * - empty: O(1)
 * - size: O(1)
 */

