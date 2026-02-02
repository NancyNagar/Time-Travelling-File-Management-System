#include <iostream>
#include <sstream>
#include <string>
#include "filesystem.hpp"

using namespace std;
// just reading the command and then calling functions 
int main() {
    FileSystem fs;
    string line;

    while (getline(cin, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        string cmd;
        iss >> cmd;

        if (cmd == "CREATE") {
            string filename; iss >> filename;
            fs.createFile(filename);

        } else if (cmd == "READ") {
            string filename; iss >> filename;
            fs.readFile(filename);

        } else if (cmd == "INSERT") {
            string filename; iss >> filename;
            string content; getline(iss, content);
            if (!content.empty() && content[0] == ' ') content.erase(0,1);
            fs.insertIntoFile(filename, content);

        } else if (cmd == "UPDATE") {
            string filename; iss >> filename;
            string content; getline(iss, content);
            if (!content.empty() && content[0] == ' ') content.erase(0,1);
            fs.updateFile(filename, content);

        } else if (cmd == "SNAPSHOT") {
            string filename; iss >> filename;
            string message; getline(iss, message);
            if (!message.empty() && message[0] == ' ') message.erase(0,1);
            fs.snapshotFile(filename, message);

        } else if (cmd == "ROLLBACK") {
            string filename; iss >> filename;
            int versionID;
            if (iss >> versionID) fs.rollbackFile(filename, true, versionID);
            else fs.rollbackFile(filename, false, -1);

        } else if (cmd == "HISTORY") {
            string filename; iss >> filename;
            fs.historyFile(filename);

        } else if (cmd == "RECENT_FILES") {
            int num; if (iss >> num) fs.recentFiles(num); else fs.recentFiles(-1);

        } else if (cmd == "BIGGEST_TREES") {
            int num; if (iss >> num) fs.biggestTrees(num); else fs.biggestTrees(-1);

        } else if (cmd == "EXIT") {
            cout << "Exiting" << endl;
            break;

        } else {
            cout << "ERROR: Unknown command " << cmd << endl;
        }
    }
    return 0;
}
