#include "hash_ring.h"
#include <iostream>
#include <functional>

using namespace std;

size_t HashRing::hashFunction(const string& key) {
    hash<string> hash_fn;
    return hash_fn(key);
}

void HashRing::addNode(int port) {
    size_t hashValue = hashFunction(to_string(port));
    ring[hashValue] = port;

    cout << "[HashRing] Added node on port " << port
         << " with hash " << hashValue << endl;
}

vector<int> HashRing::getNodes(const string& key) {
    vector<int> result;

    if (ring.empty()) {
        cout << "[HashRing] No nodes available\n";
        return result;
    }

    size_t hashValue = hashFunction(key);
    cout << "[HashRing] Key '" << key << "' hashed to " << hashValue << endl;

    auto it = ring.lower_bound(hashValue);

    // wrap if needed
    if (it == ring.end()) {
        it = ring.begin();
        cout << "[HashRing] Wrapping to first node\n";
    }

    int primary = it->second;
    result.push_back(primary);

    cout << "[HashRing] Primary Node: " << primary << endl;

    // get replica (next node in ring)
    it++;
    if (it == ring.end()) {
        it = ring.begin();
    }

    int replica = it->second;
    result.push_back(replica);

    cout << "[HashRing] Replica Node: " << replica << endl;

    return result;
}

void HashRing::printRing() {
    cout << "\n[HashRing] Current Ring:\n";
    for (auto& node : ring) {
        cout << "Hash: " << node.first << " -> Port: " << node.second << endl;
    }
    cout << endl;
}