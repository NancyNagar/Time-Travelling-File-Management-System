#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <vector>
#include <utility>
#include "tree.hpp"

using namespace std;
// defined hashmap and just implemented basic functions
class HashMap {
public:
    HashMap(int cap = 101) {
        if (cap <= 0) cap = 101;
        
        capacity = cap;

        entry.resize(capacity);
    }

    void put(int key, TreeNode* value) {
        int idx = hashKey(key);

        for (pair<int, TreeNode*>& p : entry[idx]) {
            if (p.first == key) { p.second = value; return; }
        }
        entry[idx].push_back({key, value});
    }

    bool get(int key, TreeNode*& out) {
        int idx = hashKey(key);

        for (pair<int, TreeNode*>& p : entry[idx]) {
            if (p.first == key) { out = p.second; return true; }
        }
        return false;
    }

    bool contains(int key) {
        TreeNode* dummy;

        return get(key, dummy);
    }

    void remove(int key) {
        int idx = hashKey(key);

        vector<pair<int, TreeNode*>>&bucket = entry[idx];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) { bucket.erase(it); return; }
        }
    } 

    vector<vector<pair<int, TreeNode*>>> entry;
    int capacity;

    int hashKey(int key) { return key % capacity;  }
};

#endif
