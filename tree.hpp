#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <string>
#include <vector>
#include <ctime>

using namespace std;
// defining the tree
struct TreeNode {
    int version_id;
    string content;
    string message;
    time_t created_time;
    time_t snapshot_time;
    TreeNode* parent;
    vector<TreeNode*> children;

    TreeNode(int id, const string& cont, const string& msg, time_t t)
        : version_id(id), content(cont), message(msg),
          created_time(t), snapshot_time(0), parent(nullptr) {
        if (!msg.empty()) snapshot_time = t;
    }

    bool isSnapshot() const { return snapshot_time != 0; }
};

#endif
