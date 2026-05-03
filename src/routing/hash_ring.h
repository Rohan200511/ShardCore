#ifndef HASH_RING_H
#define HASH_RING_H

#include <map>
#include <vector>
#include <string>

using namespace std;

class HashRing {
private:
    map<size_t, int> ring;

    size_t hashFunction(const string& key);

public:
    void addNode(int port);

    int getNode(const string& key); 

    vector<int> getNodes(const string& key); 

    void printRing();
};

#endif