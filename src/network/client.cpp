#include "client.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include "../routing/hash_ring.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void Client::start() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    // Initialize Hash Ring
    HashRing ring;

    cout << "\nInitializing Hash Ring...\n";

    ring.addNode(9090);
    ring.addNode(9091);
    ring.addNode(9092);

    ring.printRing();

    cout << "\n===== ShardCore Client =====\n";
    cout << "Commands:\n";
    cout << "PUT <key> <value>\n";
    cout << "GET <key>\n";
    cout << "DELETE <key>\n";
    cout << "EXIT\n";

    string input;

    while (true) {
        cout << "\nEnter command: ";
        getline(cin, input);

        if (input == "EXIT") break;

        stringstream ss(input);
        string command, key;
        ss >> command >> key;

        if (key.empty()) {
            cout << "Invalid input. Please provide a key.\n";
            continue;
        }

        // Get primary + replica
        vector<int> nodes = ring.getNodes(key);

        if (nodes.size() < 2) {
            cout << "Not enough nodes for replication\n";
            continue;
        }

        int primary = nodes[0];
        int replica = nodes[1];

        cout << "\nPrimary Node: " << primary << endl;
        cout << "Replica Node: " << replica << endl;

        // ===================== PUT =====================
        if (command == "PUT") {

            // Send to primary
            cout << "Sending to Primary...\n";

            SOCKET sock1 = socket(AF_INET, SOCK_STREAM, 0);

            sockaddr_in server1{};
            server1.sin_family = AF_INET;
            server1.sin_port = htons(primary);
            server1.sin_addr.s_addr = inet_addr("127.0.0.1");

            if (connect(sock1, (struct sockaddr*)&server1, sizeof(server1)) >= 0) {
                send(sock1, input.c_str(), input.size(), 0);

                char buffer[1024] = {0};
                recv(sock1, buffer, 1024, 0);

                cout << "Primary Response: " << buffer;

                closesocket(sock1);
            } else {
                cout << "Primary connection failed\n";
            }

            // Send to replica
            cout << "Replicating to Replica...\n";

            SOCKET sock2 = socket(AF_INET, SOCK_STREAM, 0);

            sockaddr_in server2{};
            server2.sin_family = AF_INET;
            server2.sin_port = htons(replica);
            server2.sin_addr.s_addr = inet_addr("127.0.0.1");

            if (connect(sock2, (struct sockaddr*)&server2, sizeof(server2)) >= 0) {
                send(sock2, input.c_str(), input.size(), 0);
                closesocket(sock2);
            } else {
                cout << "Replica connection failed\n";
            }
        }

        // ===================== GET =====================
        else if (command == "GET") {

            string response = "";

            // Try primary
            SOCKET sock1 = socket(AF_INET, SOCK_STREAM, 0);

            sockaddr_in server1{};
            server1.sin_family = AF_INET;
            server1.sin_port = htons(primary);
            server1.sin_addr.s_addr = inet_addr("127.0.0.1");

            if (connect(sock1, (struct sockaddr*)&server1, sizeof(server1)) >= 0) {
                send(sock1, input.c_str(), input.size(), 0);

                char buffer[1024] = {0};
                recv(sock1, buffer, 1024, 0);

                response = buffer;
                closesocket(sock1);
            } else {
                cout << "Primary failed, trying replica...\n";

                SOCKET sock2 = socket(AF_INET, SOCK_STREAM, 0);

                sockaddr_in server2{};
                server2.sin_family = AF_INET;
                server2.sin_port = htons(replica);
                server2.sin_addr.s_addr = inet_addr("127.0.0.1");

                if (connect(sock2, (struct sockaddr*)&server2, sizeof(server2)) >= 0) {
                    send(sock2, input.c_str(), input.size(), 0);

                    char buffer[1024] = {0};
                    recv(sock2, buffer, 1024, 0);

                    response = buffer;
                    closesocket(sock2);
                } else {
                    cout << "Replica also failed\n";
                }
            }

            cout << "Response: " << response;
        }

        // ===================== DELETE =====================
        else if (command == "DELETE") {

            cout << "Deleting from Primary and Replica...\n";

            for (int port : nodes) {
                SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

                sockaddr_in server{};
                server.sin_family = AF_INET;
                server.sin_port = htons(port);
                server.sin_addr.s_addr = inet_addr("127.0.0.1");

                if (connect(sock, (struct sockaddr*)&server, sizeof(server)) >= 0) {
                    send(sock, input.c_str(), input.size(), 0);
                    closesocket(sock);
                }
            }

            cout << "Delete operation completed\n";
        }

        else {
            cout << "Invalid command\n";
        }
    }

    WSACleanup();
}