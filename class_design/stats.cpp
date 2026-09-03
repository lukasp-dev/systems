/**
Design a data structure that supports the following operations on a stream of numbers:

insert(x): add a number x
get_max(): return the maximum number inserted so far
get_mean(): return the average of all numbers inserted so far
get_mode(): return the most frequently inserted number
Follow-up:

Support the same query operations for the most recent k inserted numbers:

get_max_recent_k(): return the maximum among the most recent k numbers
get_mean_recent_k(): return the average of the most recent k numbers
get_mode_recent_k(): return the mode among the most recent k numbers
*/
/**
 * FOLLOW UP questions on my side:
 * If multiple numbers have the same maximum frequency, how should ties be broken?
 *   -> Resolved here as: the largest value wins. Both the global mode and the
 *      recent-k mode follow that same rule.
 *
 * If fewer than k numbers have been inserted, what should the recent-k queries return?
 *   -> Resolved here as: they answer over everything inserted so far (window size
 *      min(count, k)) and only throw when the stream is empty.
 */
#include <bits/stdc++.h>
using namespace std;

class DataStructure {
int global_max;

double sum;
int count;

unordered_map<int, int> freq;
int current_mode;
int max_freq;
int k;

queue<int> recent_k_window;
double recent_sum;
// (value, index) pairs, values strictly decreasing from front to back
deque<pair<int, int>> monotonic_deque;

unordered_map<int, int> freq_k;
// (frequency, value) pairs, so rbegin() is the mode of the window
set<pair<int, int>> mode_set_k;

    void window_add(int x) {
        int prev = freq_k[x];
        if (prev > 0) {
            mode_set_k.erase({prev, x});
        }

        freq_k[x] = prev + 1;
        mode_set_k.insert({prev + 1, x});
    }

    void window_remove(int x) {
        int prev = freq_k[x];
        mode_set_k.erase({prev, x});

        if (prev == 1) {
            freq_k.erase(x);
        } else {
            freq_k[x] = prev - 1;
            mode_set_k.insert({prev - 1, x});
        }
    }

public:
    DataStructure(int k):
        global_max(INT_MIN),
        sum(0.0),
        count(0),
        current_mode(0),
        max_freq(0),
        k(k),
        recent_sum(0.0) {
        if (k <= 0) {
            throw invalid_argument("k must be positive");
        }
    }

    void insert(int x) {
        int index = count;

        sum += x;
        count += 1;

        if(global_max < x) {
            global_max = x;
        }

        freq[x]++;
        if(freq[x] > max_freq || (freq[x] == max_freq && x > current_mode)) {
            max_freq = freq[x];
            current_mode = x;
        }

        recent_k_window.push(x);
        recent_sum += x;
        window_add(x);

        if(static_cast<int>(recent_k_window.size()) > k) {
            int old_one = recent_k_window.front(); recent_k_window.pop();
            recent_sum -= old_one;
            window_remove(old_one);
        }

        while(!monotonic_deque.empty() && monotonic_deque.back().first <= x) {
            monotonic_deque.pop_back();
        }

        monotonic_deque.push_back({x, index});
        while(monotonic_deque.front().second <= index - k) {
            monotonic_deque.pop_front();
        }
    }

    int get_max() {
        if(count == 0) {
            throw runtime_error("Cannot get max of an empty stream.");
        }

        return global_max;
    }

    double get_mean() {
        if(count == 0) {
            throw runtime_error("Cannot get mean of an empty stream.");
        }

        return (double)sum / count; 
    }

    int get_mode() {
        if(count == 0) {
            throw runtime_error("Cannot get mode of an empty stream.");
        }

        return current_mode;
    }

    double get_mean_recent_k() {
        if(recent_k_window.empty()) {
            throw runtime_error("Cannot get mean of an empty stream.");
        }

        return recent_sum / recent_k_window.size();
    }

    int get_max_recent_k() {
        if(monotonic_deque.empty()) {
            throw runtime_error("Cannot get max of an empty stream.");
        }

        return monotonic_deque.front().first;
    }

    int get_mode_recent_k() {
        if(mode_set_k.empty()) {
            throw runtime_error("Cannot get mode of an empty stream.");
        }

        return mode_set_k.rbegin()->second;
    }
};