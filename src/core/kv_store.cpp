#include "kv_store.h"
#include<sstream>
#include<iostream>

using namespace std;

void KVStore::put(string& key , string& val){
    store[key] = val;
    persist("PUT " + key + " " + val);
}

string KVStore::get(string& key){
    if(store.find(key) != store.end()){
        return store[key];
    }
    return "No Key exists";
}

void KVStore::remove(string& key){
    if(store.find(key) != store.end()){
        store.erase(key);
        persist("DELETE " + key);
    }
}

void KVStore::persist(const std::string& command) {
    ofstream file(logFile, ios::app);
    file << command << std::endl;
}

void KVStore::loadFromDisk() {
    ifstream file(logFile);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        string command, key, value;
        ss >> command >> key;

        if (command == "PUT") {
            ss >> value;
            store[key] = value;
        }
        else if (command == "DELETE") {
            store.erase(key);
        }
    }

    cout << "[Persistence] Data loaded from disk\n";
}