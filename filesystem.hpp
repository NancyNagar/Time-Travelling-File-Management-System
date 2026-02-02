#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include "hashmap.hpp"
#include "heap.hpp"

using namespace std;
// defined all the tasks that the code should do here
struct File {
    TreeNode* root;
    TreeNode* active_version;
    HashMap version_map;
    int total_versions;
    string filename;
    time_t last_modified;

    File(const string& name) : version_map(101) {
        root = new TreeNode(0, "", "root snapshot", time(nullptr));
        active_version = root;
        version_map.put(0, root);
        total_versions = 1;
        filename = name;
        last_modified = root->created_time;
    }
};

class FileSystem {
public:
    FileSystem() {}
    ~FileSystem() {
        for (File* f : files) {
            vector<TreeNode*> stack{f->root};

            while (!stack.empty()) {
                TreeNode* n = stack.back(); stack.pop_back();
                for (auto c : n->children) stack.push_back(c);
                delete n;
            }
            delete f;
        }
    }

    void createFile(const string& filename) {
        if (findFile(filename)) {
            cout << "ERROR: File " << filename << " already exists" << endl;
            return;
        }

        File* f = new File(filename);

        files.push_back(f);

        cout << "Created file " << filename << " with snapshot version 0" << endl;
    }

    void readFile(const string& filename) {
        File* f = findFile(filename);

        if (!f) { cout << "ERROR: File: " << filename << " not found "<<  endl; return; }

        cout << f->active_version->content << endl;
    }

    void insertIntoFile(const string& filename, const string& content) {
        File* f = findFile(filename); 

        if (!f) { cout << "ERROR: File "<< filename<< " not found"  << endl; return; }

        TreeNode* act = f->active_version;

        if (act->isSnapshot()) {
            int vid = f->total_versions;
            string newcontent = act->content + content; 
            TreeNode* node = new TreeNode(vid, newcontent, "", time(nullptr));
            node->parent = act;
            act->children.push_back(node);
            f->version_map.put(vid, node);
            f->active_version = node;
            f->total_versions++;
            f->last_modified = node->created_time;
            cout << "New version " << vid << " created for file " << filename << endl;
        } else {
            act->content += content; 
            act->created_time = time(nullptr);
            f->last_modified = act->created_time;
            cout << "Modified active version " << act->version_id << " of file " << filename << endl;
        }
    }

    void updateFile(const string& filename, const string& content) {
        File* f = findFile(filename);

        if (!f) { cout << "ERROR: File " << filename << " not found " << endl; return; }

        TreeNode* act = f->active_version;

        if (act->isSnapshot()) {
            int vid = f->total_versions;
            TreeNode* node = new TreeNode(vid, content, "", time(nullptr)); 
            node->parent = act;
            act->children.push_back(node);
            f->version_map.put(vid, node);
            f->active_version = node;
            f->total_versions++;
            f->last_modified = node->created_time;
            cout << "New version " << vid << " created for file " << filename << endl;
        } else {
            act->content = content; 
            act->created_time = time(nullptr);
            f->last_modified = act->created_time;
            cout << "Modified active version " << act->version_id << " of file " << filename << endl;
        }
    }

    void snapshotFile(const string& filename, const string& message) {
        File* f = findFile(filename);

        if (!f) { cout << "ERROR: File" << filename << " not found " << endl; return; }

        TreeNode* act = f->active_version;

        if (act->isSnapshot()) {
            act->message = message;
            act->snapshot_time = time(nullptr);
            f->last_modified = act->snapshot_time;
            cout << "Updated snapshot for version " << act->version_id << " of file " << filename << endl;
        } else {
            act->message = message;
            act->snapshot_time = time(nullptr);
            f->last_modified = act->snapshot_time;
            cout << "Snapshot created for active version " << act->version_id << " of file " << filename << endl;
        }
    }

    void rollbackFile(const string& filename, bool hasId, int versionID) {
        File* f = findFile(filename);

        if (!f) { cout << "ERROR: File " << filename << " not found " << endl; return; }

        if (!hasId) {
            TreeNode* parent = f->active_version->parent;

            if (!parent) {
                cout << "ERROR: Already at root version, therefore cannot rollback" << endl;
                return;
            }
            f->active_version = parent;
            f->last_modified = time(nullptr);
            cout << "Active version for file " << filename << " set to " << parent->version_id << endl;
        } else {
            TreeNode* node = nullptr;
            if (!f->version_map.get(versionID, node) || node == nullptr) {
                cout << "ERROR: Version " << versionID << " not found for file " << filename << endl;
                return;
            }
            f->active_version = node;
            f->last_modified = time(nullptr);
            cout << "Active version for file " << filename << " set to " << versionID << endl;
        }
    }

    void historyFile(const string& filename) {
        File* f = findFile(filename);

        if (!f) { cout << "ERROR: File" << filename << " not found " << endl; return; }

        vector<TreeNode*> path;

        TreeNode* cur = f->active_version;

        while (cur) {
            if (cur->isSnapshot()) path.push_back(cur);
            cur = cur->parent;
        }
    
        for (TreeNode* n : path) {
            time_t t = n->snapshot_time;
            tm *tmPtr = localtime(&t);
            char buf[64];
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tmPtr);
            cout << "Version " << n->version_id << ": " << buf << " " << n->message << endl;
        }
    }

    void recentFiles(int num) {
        MaxHeap heap(false); 

        for (auto f : files) heap.push(f);

        if (num == -1) num = files.size();

        int count = 0;

        while (!heap.empty() && count < num) {
            File* f = heap.top(); heap.pop();
            time_t t = f->last_modified;
            tm *tmPtr = localtime(&t);
            char buf[64];
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tmPtr);
            cout << f->filename << " " << buf << endl;
            count++;
        }
    }

    void biggestTrees(int num) {
        MaxHeap heap(true); 

        for (auto f : files) heap.push(f);

        if (num == -1) num = files.size();

        int count = 0;

        while (!heap.empty() && count < num) {
            File* f = heap.top(); heap.pop();
            cout << f->filename << " " << f->total_versions << endl;
            count++;
        }
    }

    vector<File*> files;
    File* findFile(const string& filename) {
        for (auto f : files) if (f->filename == filename) return f;
        return nullptr;
    }
};

// Link Heap priority function
inline bool MaxHeap::higherPriority(File* a, File* b) {
    if (compare_v_id)
        return a->total_versions > b->total_versions;
    else
        return a->last_modified > b->last_modified;
}

#endif
