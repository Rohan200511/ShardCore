#ifndef KV_STORE_H
#define KV_STORE_H
#include<fstream>

#include<unordered_map>
#include<string>
using namespace std;

class KVStore{
private:
    unordered_map<string,string>store;
    string logFile = "data.log";
    void persist(const std::string& command);
    
public:
    void loadFromDisk();
    void put(string& key , string& val);
    string get(string& key);
    void remove(string& key);
};

#endif