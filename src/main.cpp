#include<iostream>
#include<string>
#include<sstream>
#include "core/kv_store.h"
#include "network/server.h"

using namespace std;

/*int main(){
    KVStore kv;
    string input;

    cout << "ShardCore CLI Started. Type EXIT to exit\n";
    cout << "Commands: PUT key value | GET key | DELETE key | EXIT\n";
    
    while(true){
        cout << "\n> ";
        getline(cin, input);

        stringstream ss(input);

        string command , key , value;

        cin >> command;

        if(command == "PUT"){
            ss >> key >> value;

            if(key.empty() || value.empty()) {
                cout << "Invalid Key or Value\n";
                continue;
            }
            kv.put(key , value);
            cout << "Stored: [" << key << " → " << value << "]\n";
        }

        else if(command == "GET"){
            ss >> key;

            if(key.empty()){
                cout << "Enter valid key";
                continue;
            }

            string result = kv.get(key);

            if(result == "No Key exists"){
                cout << "Key not found: " << key << "\n";
            }

            else cout << key << " -> " << value << "\n";
        }

        else if(command == "DELETE"){
            cin >> key;

            if(key.empty()){
                cout << "Enter valid key";
                continue;
            }
            
            string result = kv.get(key);

            if(result == "No Key exists"){
                cout << "Key not found: " << key << "\n";
            }

            else{
                kv.remove(key);
                cout << "Key and Value Deleted Successfully!";
            }
        }

        else if(command == "EXIT"){
            cout << "Exiting ShardCore";
            break;
        }

        else cout << "Invalid Command\n";
    }
}*/


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: shardcore <port>\n";
        cout << "Example: shardcore 9090\n";
        return 1;
    }

    int port = stoi(argv[1]);

    cout << "Starting ShardCore node on port: " << port << endl;

    Server server(port);
    server.start();

    return 0;
}