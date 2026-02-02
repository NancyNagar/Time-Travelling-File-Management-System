#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>
using namespace std;
// defined heap and just implemented basic functions
struct File; 

class MaxHeap {
public:
    vector<File*> data;
    
    MaxHeap(bool byVersion) { compare_v_id = byVersion; }

    void push(File* f) {
        data.push_back(f);
        sift_up(data.size()-1);
    }

    File* top() {
        if (data.empty()) return nullptr;
        return data[0];
    }

    void pop() {
        if (data.empty()) return;
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) sift_down(0);
    }

    bool empty() { return data.empty(); }
    
    int size() { return data.size(); }



    bool compare_v_id;

    void sift_up(int idx) {
        while (idx > 0) {
            int p = (idx-1)/2;

            if (higherPriority(data[idx], data[p])) {
                swap(data[idx], data[p]);
                idx = p;
            } else break;
        }
    }

    void sift_down(int idx) {
        int n = data.size();

        while (true) {
            int l = 2*idx+1, r = 2*idx+2, best = idx;

            if (l<n && higherPriority(data[l], data[best])) best = l;

            if (r<n && higherPriority(data[r], data[best])) best = r;

            if (best!=idx) {
                swap(data[idx], data[best]);
                idx = best;
            } else break;
        }
    }

    bool higherPriority(File* a, File* b);
};

#endif
